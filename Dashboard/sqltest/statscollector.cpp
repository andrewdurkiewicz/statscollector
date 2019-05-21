#include <iostream> 
#include <sqlite3.h> 
#include <string> 
#include <stdio.h>
#include <unistd.h>  
#include <cmath>    
using namespace std; 
//to run:  g++ -o statscollector statscollector.cpp -lsqlite3 && ./statscollector

static int callback(void* data, int argc, char** argv, char** azColName) 
{ 
    int i; 
    fprintf(stderr, "%s: ", (const char*)data); 
  
    for (i = 0; i < argc; i++) 
    { 
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); 
    } 
  
    printf("\n"); 
    return 0; 
} 

//CREATE TABLE 'stats' ( 'Time' INTEGER NOT NULL, 'UL' REAL, 'DL' REAL, 'Throughput' REAL);
  
void SQL_CMD(string comand);
sqlite3 *db;
char *zErrMsg = 0;

int main(int argc, char* argv[])
{
   for(int i = 0; i<10800; i++)
   {
      cout << i << endl;
      int rc = sqlite3_open("StatsCollector.db", &db);
      if (rc == 0)
      {
        //checks if database exist, if not, create it and the table
        SQL_CMD("StatsCollector.db");
        SQL_CMD("CREATE TABLE 'stats' ( 'Time' INTEGER NOT NULL, 'UL' REAL, 'DL' REAL, 'Throughput' REAL);");

      }
      SQL_CMD("ATTACH DATABASE 'StatsCollector' as 'statscollector';");

      char buffer[200];
      sprintf(buffer,"INSERT INTO stats (Time,UL,DL,Throughput) VALUES (%i,%-.2f,%-.2f,%-.2f);",i, i*2.2, i*4.3, i*1.5);
      SQL_CMD(buffer);
      //SQL_CMD("SELECT * FROM stats WHERE 1;");
      sqlite3_close(db);

      //usleep(60*pow(10,6)); //sleep for 1 minute 

   }

}

void SQL_CMD(string command){

   sqlite3_exec(db, command.c_str(), callback, NULL, NULL);
}

