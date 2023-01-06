#if !defined(_SQLiter_hpp)
#define _SQLiter_hpp

#include <string>
#include "sqlite3.h"

class sqliter_Statement;

class sqliter_Connection
{
private:
  friend class sqliter_Statement;
  sqlite3* db;
  sqliter_Connection(sqliter_Connection const&){}
  bool operator=(sqliter_Connection const&){}

public:
  sqliter_Connection(
      std::string const& dbFilePath
    , int32_t flags = SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_NOMUTEX|SQLITE_OPEN_SHAREDCACHE);
  sqliter_Connection(sqliter_Connection&& c);
  bool isOpen() const;
  ~sqliter_Connection();

};

/// @tparam DataType int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, long double, const unsigned char*, or const void*
template <typename DataType> struct sqliter_DataTypeCode {};
template <> struct sqliter_DataTypeCode<int16_t> {static const int code = SQLITE_INTEGER;};
template <> struct sqliter_DataTypeCode<uint16_t> {static const int code = SQLITE_INTEGER;};
template <> struct sqliter_DataTypeCode<int32_t> {static const int code = SQLITE_INTEGER;};
template <> struct sqliter_DataTypeCode<uint32_t> {static const int code = SQLITE_INTEGER;};
template <> struct sqliter_DataTypeCode<int64_t> {static const int code = SQLITE_INTEGER;};
template <> struct sqliter_DataTypeCode<uint64_t> {static const int code = SQLITE_INTEGER;};
template <> struct sqliter_DataTypeCode<float> {static const int code = SQLITE_FLOAT;};
template <> struct sqliter_DataTypeCode<double> {static const int code = SQLITE_FLOAT;};
template <> struct sqliter_DataTypeCode<long double> {static const int code = SQLITE_FLOAT;};
template <> struct sqliter_DataTypeCode<const void*> {static const int code = SQLITE_BLOB;};
template <> struct sqliter_DataTypeCode<const unsigned char*> {static const int code = SQLITE_TEXT;};

/// @brief Functor to get result from result-set.
/// @tparam DataType int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, long double, const unsigned char*, or const void*
template <typename DataType> struct sqliter_DataFunctor{};
template <> struct sqliter_DataFunctor<int16_t> {using type = int32_t (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int;};
template <> struct sqliter_DataFunctor<uint16_t> {using type = int32_t (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int;};
template <> struct sqliter_DataFunctor<int32_t> {using type = int32_t (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int;};
template <> struct sqliter_DataFunctor<uint32_t> {using type = long long (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int64;};
template <> struct sqliter_DataFunctor<int64_t> {using type = long long (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int64;};
template <> struct sqliter_DataFunctor<uint64_t> {using type = long long (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int64;};
template <> struct sqliter_DataFunctor<float> {using type = double (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_double;};
template <> struct sqliter_DataFunctor<double> {using type = double (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_double;};
template <> struct sqliter_DataFunctor<long double> {using type = double (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_double;};
template <> struct sqliter_DataFunctor<const void*> {using type = const void* (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_blob;};
template <> struct sqliter_DataFunctor<const unsigned char*> {using type = const unsigned char* (*)(sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_text;};

class sqliter_Statement
{
private:

  sqlite3_stmt* stm;
  sqliter_Statement(sqliter_Statement const&){}
  bool operator=(sqliter_Statement const&){}
  uint32_t resultCodeNow = 0;

public:
  sqliter_Statement(sqliter_Connection const& c, std::string const& sentence);
  sqliter_Statement(sqliter_Statement&& s);
  bool isPrepared() const;

  /// @brief 绑定参数到NULL值
  /// @param index 
  /// @return 0 成功; -1 出错
  int bind(int32_t index);

  int bind(int32_t index, int32_t value);
  int bind(int32_t index, int64_t value);
  int bind(int32_t index, double value);
  int bind(int32_t index, std::string const& text);
  int bind(int32_t index, const void* data, uint32_t size);

  /// @return SQLITE_BUSY, SQLITE_DONE, SQLITE_ROW, SQLITE_ERROR, or SQLITE_MISUSE
  int step();

  bool rowLoaded() const;

  /// @brief 
  /// @tparam ResultType int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, long double, const unsigned char*, or const void*
  /// @param columnIndex 
  /// @param result 
  /// @return 1 成功; 0 暂无数据; -1 指定的列索与ResultType不匹配; -2 其它错误
  template<typename ResultType>
  int getResultFromCurrentRow(int32_t columnIndex, ResultType& result){
    if(!isPrepared()){
      return -2;
    }
    if(!rowLoaded()){
      return 0;
    }
    if(sqliter_DataTypeCode<ResultType>::code != sqlite3_column_type(stm,columnIndex)){
      return -1;
    }
    result = sqliter_DataFunctor<ResultType>::f(stm,columnIndex);
    return 1;
  }

  ~sqliter_Statement();
  
};

#endif // _SQLiter_hpp
