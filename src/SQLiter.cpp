#include <iostream>
#include "SQLiter.hpp"

#define CHECK_STATEMENT_BEFORE_BIND     \
  if(pDb==NULL){                        \
    return -1;                          \
  }                                     \
  auto it = statements.find(sentence);  \
  if(statements.end()==it){             \
    return -1;                          \
  }                                     \

namespace SQLiter
{
  SQLiteConnection::SQLiteConnection(
      std::string const& dbFilePath
    , int32_t flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE){
    if(SQLITE_OK != sqlite3_open_v2(dbFilePath.c_str(), &pDb, flags, NULL)){
      pDb = NULL;
    }
  }

  SQLiteConnection::operator bool(){
    return pDb!=NULL;
  }

  int SQLiteConnection::prepareStatement(std::string const& sentence){
    if(pDb==NULL){
      return -1;
    }
    sqlite3_stmt* pStm;
    if(SQLITE_OK == sqlite3_prepare_v2(pDb, sentence.c_str(), sentence.size(), &pStm, NULL)){
      auto it = statements.find(sentence);
      if(statements.end()==it){//暂无这个statement
        statements.insert({sentence,pStm});
      }else{//已有这个statement
        sqlite3_finalize(it->second);
        it->second = pStm;
      }
    }
    return 0;
  }

  int SQLiteConnection::finalizeStatement(std::string const& sentence){
    if(pDb==NULL){
      return -1;
    }
    auto it = statements.find(sentence);
    if(statements.end()==it){
      return 0;
    }else{
      sqlite3_finalize(it->second);
      statements.erase(it);
      return 1;
    }
  }

  int SQLiteConnection::bind(std::string const& sentence, int32_t index, const void* data, uint32_t size){
    CHECK_STATEMENT_BEFORE_BIND
    if(SQLITE_OK == sqlite3_bind_blob(it->second, index, data, size, NULL)){
      return 0;
    }
    return -2;
  }

  int SQLiteConnection::bind(std::string const& sentence, int32_t index, double value){
    CHECK_STATEMENT_BEFORE_BIND
    if(SQLITE_OK == sqlite3_bind_double(it->second, index, value)){
      return 0;
    }
    return -2;
  }

  int SQLiteConnection::bind(std::string const& sentence, int32_t index, int64_t value){
    CHECK_STATEMENT_BEFORE_BIND
    if(SQLITE_OK == sqlite3_bind_int64(it->second, index, value)){
      return 0;
    }
    return -2;
  }

  int SQLiteConnection::bind(std::string const& sentence, int32_t index, int32_t value){
    CHECK_STATEMENT_BEFORE_BIND
    if(SQLITE_OK == sqlite3_bind_int(it->second, index, value)){
      return 0;
    }
    return -2;
  }

  int SQLiteConnection::bind(std::string const& sentence, int32_t index){
    CHECK_STATEMENT_BEFORE_BIND
    if(SQLITE_OK == sqlite3_bind_null(it->second, index)){
      return 0;
    }
    return -2;
  }

  int SQLiteConnection::bind(std::string const& sentence, int32_t index, std::string const& text){
    CHECK_STATEMENT_BEFORE_BIND
    if(SQLITE_OK == sqlite3_bind_text(it->second, index, text.c_str(), text.length(), NULL)){
      return 0;
    }
    return -2;
  }

  SQLiteConnection::~SQLiteConnection(){
    for(auto& it : statements){
      sqlite3_finalize(it.second);
    }
    if(pDb){
      sqlite3_close_v2(pDb);
    }
  }

} // namespace SQLiter
