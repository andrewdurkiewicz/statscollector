#include <stdio.h>
#include <string.h>
#include "civetweb.h"
#include <expresscpp.h>
#include <tools.h>

// This function will be called by civetweb on every new request.
static int begin_request_handler(struct mg_connection *conn)
{
    Express::Request req;
    Express::Response res;

    const struct mg_request_info *request_info = mg_get_request_info(conn);

    req.request_uri = std::string(request_info->uri);
    req.request_method = std::string(request_info->request_method);

    if (request_info->content_length > 0)
    {
        char post_data[1024] = {0};
        mg_read(conn, post_data, sizeof(post_data));
        req.body_raw = std::string(post_data);

        for (int i = 0; i < 64; ++i)
        {
            std::string name = std::string(request_info->http_headers[i].name);
            if (name == "Content-Type")
            {
                req.content_type = std::string(request_info->http_headers[i].value);
                break;
            }
        }

        //Parse POST Body if JSON
        if (req.content_type == "application/json")
        {
            Json::Reader reader;
            reader.parse(req.body_raw, req.body);
        }
    }

    if (request_info->query_string)
        Express::parseHTTPQuery(req.query, std::string(request_info->query_string));

    app->processRequest(req, res);

    char content[1024];

    // Prepare the message we're going to send
    int content_length = snprintf(content, sizeof(content), "%s", res.response.c_str());

    // Send HTTP reply to the client
    mg_printf(conn,
              "HTTP/1.1 200 OK\r\n"
              "Access-Control-Allow-Origin: *\r\n"
              "Content-Type: application/json\r\n"
              "Content-Length: %d\r\n"        // Always set Content-Length
              "\r\n"
              "%s",
              content_length, content);

    // Returning non-zero tells civetweb that our function has replied to
    // the client, and civetweb should not send client any more data.
    return 1;
}

void civetweb_listen(std::string port)
{
    struct mg_context *ctx;
    struct mg_callbacks callbacks;

    // List of options. Last element must be NULL.
    const char *options[] = {"listening_ports", port.c_str(), NULL};

    // Prepare callbacks structure. We have only one callback, the rest are NULL.
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = begin_request_handler;

    // Start the web server.
    ctx = mg_start(&callbacks, NULL, options);

    // Wait until user hits "enter". Server is running in separate thread.
    // Navigating to http://localhost:8080 will invoke begin_request_handler().
    getchar();

    // Stop the server.
    mg_stop(ctx);
}