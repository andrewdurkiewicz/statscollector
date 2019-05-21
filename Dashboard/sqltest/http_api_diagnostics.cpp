/*
    Author: Bryan Gillespie
*/
#include "include/http_api_dashboard.h"


using json = nlohmann::json;
using namespace Express;
using namespace std;

extern MQ::SmartQueue q;

STATIC string events_db_path = FM::events_db;
STATIC string stats_analysis_results_path = "/misc/stats_analysis_results.json";

static bool addTimestampCondition(vector<string> & conditions, const string & field, const string & op, Request & req, Response & res)
{
    if (req.query.count(field)) {
        if (!isISOTimestamp(req.query[field])) {
            json error = {{"error", "invalid " + field + ": " + req.query[field]}};
            res.status(400).send(error);
            return false;
        }

        conditions.push_back(col_timestamp + op + "\"" + req.query[field] + "\"");
    }

    return true;
}

static bool addIntegerCondition(
    vector<string> & conditions,
    const string & col,
    const string & field,
    const string & op,
    Request & req,
    Response & res
)
{
    try {
        if (req.query.count(field)) {
            int id = stoi(req.query[field]);
            conditions.push_back(col + op + to_string(id));
        }
    }
    catch (const invalid_argument &) {
        json error = {{"error", "invalid " + field}};
        res.status(400).send(error);
        return false;
    }

    return true;
}

/**
    @optional in query: startTimestamp (string)
        Lower limit of event timestamps in ISO 8601 format

    @optional in query: endTimestamp (string)
        Upper limit of event timestamps in ISO 8601 format

    @optional in query: startEvent (integer)
        Lower limit of event id

    @optional in query: endEvent (integer)
        Upper limit of event id

    @optional in query: event (integer)
        Exact match event id

    @optional in query: severity ([0,1,2,3,4])
        Numerical severity

    @optional in query: limit (integer)
        Max number of entries to return (default: 100)

    @response 400 Invalid filter term provided

    @response 500 Problem accessing DB

    @tags statistics

    Get event logs with customizable filters
*/
void getEvents(Request & req, Response & res)
{
    json response = json::array();
    vector<string> conditions;

    if (!addTimestampCondition(conditions, "startTimestamp", ">=", req, res)      ||
        !addTimestampCondition(conditions, "endTimestamp", "<=", req, res)        ||
        !addIntegerCondition(conditions, col_event, "startEvent", ">=", req, res) ||
        !addIntegerCondition(conditions, col_event, "endEvent", "<=", req, res)   ||
        !addIntegerCondition(conditions, col_event, "event", "=", req, res)       ||
        !addIntegerCondition(conditions, col_severity, "severity", "=", req, res))
        return;

    int limit = 100;
    if (req.query.count("limit")) {
        try {
            limit = stoi(req.query["limit"]);
        }
        catch (const invalid_argument & ex) {
            json error = {{"error", "invalid limit: " + req.query["limit"]}};
            res.status(400).send(error);
            return;
        }
    }

    string condition;
    if (!conditions.empty())
        condition = "where " + join(conditions, " and ");

    try {
        SQLite::Database db(events_db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        if (!db.tableExists(FM::events_table)) {
            res.send(response);
            return;
        }

        string q = fmt::format("select {}, {}, {}, {} from {} {} order by id desc limit {}",
                        col_event,
                        col_severity,
                        col_timestamp,
                        col_description,
                        FM::events_table,
                        condition,
                        to_string(limit)
                    );

        SQLite::Statement query(db, q);
        while (query.executeStep()) {
            json event;
            event[col_event]        = (int) query.getColumn(col_event_num);
            ReportSeverity severity = (ReportSeverity) (int) query.getColumn(col_severity_num);
            event[col_severity]     = (severityToStr.count(severity)) ? severityToStr.at(severity) : "unknown";
            event[col_timestamp]    = (const char *) query.getColumn(col_timestamp_num);
            event[col_description]  = (const char *) query.getColumn(col_description_num);
            response.push_back(event);
        }
    }
    catch (const SQLite::Exception & ex) {
        json error = {{"error", string("SQLite3: ") + ex.what()}};
        res.status(500).send(error);
        return;
    }
    catch (const out_of_range & ex) {
        json error = {{"error", ex.what()}};
        res.status(500).send(error);
        return;
    }

    res.send(response);
}

/**
   Get Statistic Results

    @tags statistics
*/
void getStatistics(Request &, Response & res)
{
    json response;
    json results;
    if (!readJsonFromFile(results, stats_analysis_results_path))
    {
        response["error"] = fmt::format("Could not parse {}", stats_analysis_results_path);
        res.status(500).send(response);
        return;
    }
    response = results;
    res.send(response);
}

/**
    Get list of active statecode and active recovery efforts
*/
void getActiveFaultInfo(Request &, Response & res)
{
    json response;

    auto fm_stats = fmGetStats();
    response["active_statecodes"] = json::array();
    for (auto & sc : fm_stats.active_statecodes)
        response["active_statecodes"].push_back(map_statecode_to_dotted.at((StateCode) sc));

    response["current_recovery_actions"] = json::array();
    for (auto & action : fm_stats.current_recovery_actions)
        response["current_recovery_actions"].push_back(map_ra_enum_to_string.at((RecoveryActions) action));

    response["per_statecode_stats"] = json::object();
    for (auto & sc : fm_stats.per_statecode_stats) {
        response["per_statecode_stats"][sc.first] = json::object();
        response["per_statecode_stats"][sc.first]["transitions"] = sc.second.transitions;
        response["per_statecode_stats"][sc.first]["total_duration_s"] = sc.second.total_duration_s;
    }

    auto uptime_s        = getUptimeSeconds();
    response["uptime_s"] = uptime_s;

    response["availability_percent"] = 0.0f;
    if (fm_stats.per_statecode_stats.count("0.0.0")) {
        auto available_s = fm_stats.per_statecode_stats.at("0.0.0").total_duration_s;
        response["availability_percent"] = (float) available_s * 100 / uptime_s;
    }

    response["current_recovery_action_index"] = fm_stats.current_recovery_action_index;
    response["modem_commanded_down"]          = fm_stats.modem_commanded_down;

    res.send(response);
}

/**
   Clear events log database
*/
void clearEvents(Request &, Response & res)
{
    auto header = q.makeHeader(MQ::MessageType::clear_stats);
    q.send(MQ::FAULTMAN, "", header);

    json response = {{ "status", "ok" }};
    res.send(response);
}

/**
   Clear statistics database
*/
void clearStatistics(Request &, Response & res)
{
    auto header = q.makeHeader(MQ::MessageType::clear_stats);
    q.send(MQ::STATS_COLLECTOR, "", header);

    json response = {{ "status", "ok" }};
    res.send(response);
}

/**
    @router /diagnostics
*/
void initDiagnosticsRouter(Router & router)
{
    router.get("/events", getEvents);
    router.get("/events/clear", clearEvents);
    router.get("/statistics", getStatistics);
    router.get("/statistics/clear", clearStatistics);
    router.get("/active_faults", getActiveFaultInfo);
}
