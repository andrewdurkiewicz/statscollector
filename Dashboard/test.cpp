#include <jsoncpp/json/json.h>
#include <iostream>
using namespace std;
Json::Value coord(int t, double u, double d, double throughput)
{
    Json::Value result;
    result["Time"] = t;
    result["UL"] = u;
    result["DL"] = d;
    result["Throughput"] = throughput;
    return result;    
}

void make_event(Json::Value & event)
{
    Json::Value & coords = event["Cords"];

}
int main(){
    vector <Json::Value> elements; 
    elements.push_back(coord(1, 2.2, 3.3 ,4.4));
    cout << elements[0] << endl;
    return 0;
}