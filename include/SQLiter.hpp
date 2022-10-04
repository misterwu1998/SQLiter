#if !defined(_SQLiter_hpp)
#define _SQLiter_hpp

#include <string>
#include "sqlite3.h"

namespace SQLiter
{
  struct Statement;

  struct Connection
  {
  private:
    friend struct Statement;
    ::sqlite3* db;
    Connection(Connection const&){}
    bool operator=(Connection const&){}
  
  public:
    Connection(
        std::string const& dbFilePath
      , int32_t flags = SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_NOMUTEX|SQLITE_OPEN_SHAREDCACHE);
    Connection(Connection&& c);
    bool isOpen() const;
    ~Connection();

  };

  /// @tparam DataType int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, long double, const unsigned char*, or const void*
  template <typename DataType> struct DataTypeCode {};
  template <> struct DataTypeCode<int16_t> {static const int code = SQLITE_INTEGER;};
  template <> struct DataTypeCode<uint16_t> {static const int code = SQLITE_INTEGER;};
  template <> struct DataTypeCode<int32_t> {static const int code = SQLITE_INTEGER;};
  template <> struct DataTypeCode<uint32_t> {static const int code = SQLITE_INTEGER;};
  template <> struct DataTypeCode<int64_t> {static const int code = SQLITE_INTEGER;};
  template <> struct DataTypeCode<uint64_t> {static const int code = SQLITE_INTEGER;};
  template <> struct DataTypeCode<float> {static const int code = SQLITE_FLOAT;};
  template <> struct DataTypeCode<double> {static const int code = SQLITE_FLOAT;};
  template <> struct DataTypeCode<long double> {static const int code = SQLITE_FLOAT;};
  template <> struct DataTypeCode<const void*> {static const int code = SQLITE_BLOB;};
  template <> struct DataTypeCode<const unsigned char*> {static const int code = SQLITE_TEXT;};

  /// @brief Functor to get result from result-set.
  /// @tparam DataType int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, long double, const unsigned char*, or const void*
  template <typename DataType> struct DataFunctor{};
  template <> struct DataFunctor<int16_t> {using type = int32_t (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int;};
  template <> struct DataFunctor<uint16_t> {using type = int32_t (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int;};
  template <> struct DataFunctor<int32_t> {using type = int32_t (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int;};
  template <> struct DataFunctor<uint32_t> {using type = long long (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int64;};
  template <> struct DataFunctor<int64_t> {using type = long long (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int64;};
  template <> struct DataFunctor<uint64_t> {using type = long long (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_int64;};
  template <> struct DataFunctor<float> {using type = double (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_double;};
  template <> struct DataFunctor<double> {using type = double (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_double;};
  template <> struct DataFunctor<long double> {using type = double (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_double;};
  template <> struct DataFunctor<const void*> {using type = const void* (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_blob;};
  template <> struct DataFunctor<const unsigned char*> {using type = const unsigned char* (*)(::sqlite3_stmt*,int32_t); constexpr static type f = sqlite3_column_text;};
  
  struct Statement
  {
  private:

    ::sqlite3_stmt* stm;
    Statement(Statement const&){}
    bool operator=(Statement const&){}
    uint32_t resultCodeNow = 0;
  
  public:
    Statement(Connection const& c, std::string const& sentence);
    Statement(Statement&& s);
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
      if(DataTypeCode<ResultType>::code != sqlite3_column_type(stm,columnIndex)){
        return -1;
      }
      result = DataFunctor<ResultType>::f(stm,columnIndex);
      return 1;
    }

    ~Statement();
    
  };
  
} // namespace SQLiter

#endif // _SQLiter_hpp
