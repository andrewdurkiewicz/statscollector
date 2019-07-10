#include "json/json.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(){
	Json::Value root;
	Json::Reader reader;
	std::ifstream file("/home/user/statscollector/Dashboard/stats_available.json");
	file >> root;
    if(!reader.parse(file, root, true))
	{
            cout<<"error";
	}
    return 0;
}