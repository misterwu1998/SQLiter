#include "sqlite3.h"
#include "SQLiter.hpp"

sqliter_Connection::sqliter_Connection(
    std::string const& dbFilePath
  , int32_t flags){
  if(SQLITE_OK != sqlite3_open_v2(dbFilePath.c_str(), &db, flags, NULL)){
    db = NULL;
  }
}

sqliter_Connection::sqliter_Connection(sqliter_Connection&& c){
  db = c.db;
  c.db = NULL;
}

bool sqliter_Connection::isOpen() const{
  return db!=NULL;
}

sqliter_Connection::~sqliter_Connection(){
  if(db!=NULL){
    sqlite3_close_v2(db);
  }
}

sqliter_Statement::sqliter_Statement(sqliter_Connection const& c, std::string const& sentence){
  if(SQLITE_OK != sqlite3_prepare_v2(c.db, sentence.c_str(), sentence.length(), &stm, NULL)){
    stm = NULL;
  }
  resultCodeNow = 0;
}

sqliter_Statement::sqliter_Statement(sqliter_Statement&& s){
  stm = s.stm;
  s.stm = NULL;
  resultCodeNow = 0;
}

bool sqliter_Statement::isPrepared() const{
  return stm!=NULL;
}

int sqliter_Statement::bind(int32_t index){
  if(stm==NULL){
    return -1;
  }
  if(SQLITE_OK != sqlite3_bind_null(stm,index)){
    return -1;
  }
  return 0;
}

int sqliter_Statement::bind(int32_t index, int32_t value){
  if(stm==NULL){
    return -1;
  }
  if(SQLITE_OK != sqlite3_bind_int(stm,index,value)){
    return -1;
  }
  return 0;
}

int sqliter_Statement::bind(int32_t index, int64_t value){
  if(stm==NULL){
    return -1;
  }
  if(SQLITE_OK != sqlite3_bind_int64(stm,index,value)){
    return -1;
  }
  return 0;
}

int sqliter_Statement::bind(int32_t index, double value){
  if(stm==NULL){
    return -1;
  }
  if(SQLITE_OK != sqlite3_bind_double(stm,index,value)){
    return -1;
  }
  return 0;
}

int sqliter_Statement::bind(int32_t index, std::string const& text){
  if(stm==NULL){
    return -1;
  }
  if(SQLITE_OK != sqlite3_bind_text(stm,index, text.c_str(), text.size(), NULL)){
    return -1;
  }
  return 0;
}

int sqliter_Statement::bind(int32_t index, const void* data, uint32_t size){
  if(stm==NULL){
    return -1;
  }
  if(SQLITE_OK != sqlite3_bind_blob(stm,index,data,size,NULL)){
    return -1;
  }
  return 0;
}  

int sqliter_Statement::step(){
  this->resultCodeNow = sqlite3_step(stm);
  return resultCodeNow;
}

bool sqliter_Statement::rowLoaded() const{
  return SQLITE_ROW == resultCodeNow;
}

sqliter_Statement::~sqliter_Statement(){
  if(isPrepared())    sqlite3_finalize(stm);
}
