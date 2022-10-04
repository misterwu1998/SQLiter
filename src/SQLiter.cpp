#include "sqlite3.h"
#include "SQLiter.hpp"

namespace SQLiter
{
  Connection::Connection(
      std::string const& dbFilePath
    , int32_t flags){
    if(SQLITE_OK != sqlite3_open_v2(dbFilePath.c_str(), &db, flags, NULL)){
      db = NULL;
    }
  }
  
  Connection::Connection(Connection&& c){
    db = c.db;
    c.db = NULL;
  }

  bool Connection::isOpen() const{
    return db!=NULL;
  }

  Connection::~Connection(){
    if(db!=NULL){
      sqlite3_close_v2(db);
    }
  }

  Statement::Statement(Connection const& c, std::string const& sentence){
    if(SQLITE_OK != sqlite3_prepare_v2(c.db, sentence.c_str(), sentence.length(), &stm, NULL)){
      stm = NULL;
    }
    resultCodeNow = 0;
  }

  Statement::Statement(Statement&& s){
    stm = s.stm;
    s.stm = NULL;
    resultCodeNow = 0;
  }

  bool Statement::isPrepared() const{
    return stm!=NULL;
  }

  int Statement::bind(int32_t index){
    if(stm==NULL){
      return -1;
    }
    if(SQLITE_OK != sqlite3_bind_null(stm,index)){
      return -1;
    }
    return 0;
  }

  int Statement::bind(int32_t index, int32_t value){
    if(stm==NULL){
      return -1;
    }
    if(SQLITE_OK != sqlite3_bind_int(stm,index,value)){
      return -1;
    }
    return 0;
  }

  int Statement::bind(int32_t index, int64_t value){
    if(stm==NULL){
      return -1;
    }
    if(SQLITE_OK != sqlite3_bind_int64(stm,index,value)){
      return -1;
    }
    return 0;
  }

  int Statement::bind(int32_t index, double value){
    if(stm==NULL){
      return -1;
    }
    if(SQLITE_OK != sqlite3_bind_double(stm,index,value)){
      return -1;
    }
    return 0;
  }

  int Statement::bind(int32_t index, std::string const& text){
    if(stm==NULL){
      return -1;
    }
    if(SQLITE_OK != sqlite3_bind_text(stm,index, text.c_str(), text.size(), NULL)){
      return -1;
    }
    return 0;
  }

  int Statement::bind(int32_t index, const void* data, uint32_t size){
    if(stm==NULL){
      return -1;
    }
    if(SQLITE_OK != sqlite3_bind_blob(stm,index,data,size,NULL)){
      return -1;
    }
    return 0;
  }  

  int Statement::step(){
    this->resultCodeNow = sqlite3_step(stm);
    return resultCodeNow;
  }

  bool Statement::rowLoaded() const{
    return SQLITE_ROW == resultCodeNow;
  }

  Statement::~Statement(){
    if(isPrepared())    sqlite3_finalize(stm);
  }

} // namespace SQLiter
