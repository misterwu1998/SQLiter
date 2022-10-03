#include "SQLiter.hpp"
#include <stdlib.h>
#include <iostream>

int main(int argc, char const *argv[])
{
  if(false){

    sqlite3* pDb;
    int ret = sqlite3_open_v2("test.db", &pDb
    , SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE
    , NULL);
    if(SQLITE_OK != ret){
      exit(-1);
    }

    const char* sentence = "create table if not exists student (id integer, name text not null);";
    sqlite3_stmt* pStm;
    ret = sqlite3_prepare_v2(pDb, sentence, -1, &pStm, NULL);
    if(SQLITE_OK != ret){
      exit(-2);
    }
    sqlite3_step(pStm);
    sqlite3_finalize(pStm);

    sentence = "insert into student(id, name) values(?,?)";
    ret = sqlite3_prepare_v2(pDb, sentence, -1, &pStm, NULL);
    if(SQLITE_OK != ret){
      exit(-2);
    }
    ret = sqlite3_bind_int(pStm,1,100);
    ret = sqlite3_bind_text(pStm,2,"John",-1,NULL);
    if(SQLITE_OK != ret){
      exit(-3);
    }
    sqlite3_step(pStm);
    sqlite3_finalize(pStm);

    sentence = "select name from student where id < ?;";
    ret = sqlite3_prepare_v2(pDb, sentence, -1, &pStm, NULL);
    if(SQLITE_OK != ret){
      exit(-2);
    }
    ret = sqlite3_bind_int(pStm,1,200);
    if(SQLITE_OK != ret){
      exit(-3);
    }
    while(SQLITE_ROW == sqlite3_step(pStm)){//又查到一行
      auto name = sqlite3_column_text(pStm,0);
      std::cout << name << std::endl;
    }
    sqlite3_finalize(pStm);

    sqlite3_close_v2(pDb);
    return 0;

  }

  SQLiter::SQLiteConnection c("test.db");
  if(!c){
    exit(-1);
  }
  std::string sentence("select name from student where id < ?;");
  int ret = c.prepareStatement(sentence);
  ret = c.bind(sentence,1, (int32_t)200);

}
