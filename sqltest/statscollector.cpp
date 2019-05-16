#include <iostream> 
#include <sqlite3.h> 
#include <string> 
#include <stdio.h>
#include <unistd.h>      
using namespace std; 
  
static int callback(void* data, int argc, char** argv, char** azColName) 
{ 
    int i; 
    fprintf(stderr, "%s: ", (const char*)data); 
  
    for (i = 0; i < argc; i++) { 
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); 
    } 
  
    printf("\n"); 
    return 0; 
} 


  
void SQL_CMD(string comand);
sqlite3 *db;
char *zErrMsg = 0;
int rc = sqlite3_open("testDB.db", &db);
int i = 0;
int main(int argc, char* argv[]){
   while(1){
      i++;
      SQL_CMD("ATTACH DATABASE 'StatsCollector' as 'statscollector';");
      
      cout << "STATE OF TABLE BEFORE INSERT" << endl;
      SQL_CMD("SELECT * FROM stats WHERE 1;");

      char buffer[200];
      sprintf(buffer,"INSERT INTO stats (Time,UL,DL,Throughput) VALUES (%i,%-.2f,%-.2f,%-.2f);",i, i*2.2, i*4.3, i*1.5);
      SQL_CMD(buffer);

      cout << "STATE OF TABLE AFTER INSERT" << endl;
      SQL_CMD("SELECT * FROM stats WHERE 1;");
      usleep(10000);

   }
   sqlite3_close(db);

}



void SQL_CMD(string command){

   sqlite3_exec(db, command.c_str(), callback, NULL, NULL);
}

