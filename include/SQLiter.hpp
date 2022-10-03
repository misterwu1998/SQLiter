#if !defined(_SQLiter_hpp)
#define _SQLiter_hpp

#include <unordered_map>
#include <string>
#include <vector>
#include "sqlite3.h"

// struct sqlite3;
// struct sqlite3_stmt;

namespace SQLiter
{
  class SQLiteConnection
  {
  private:
    ::sqlite3* pDb;
    std::unordered_map<std::string, ::sqlite3_stmt*> statements;

  public:

    SQLiteConnection(
        std::string const& dbFilePath
      , int32_t flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE);
  
    /// @brief 仅当SQLiteConnection与数据库文件的连接处于开启状态时，返回true
    operator bool();

    /// @brief 预先编译一条SQL语句
    /// @param sentence 未绑定的参数用“?”代替
    /// @return 0成功；-1出错
    int prepareStatement(std::string const& sentence);

    /// @brief 舍弃预先编译的一条SQL语句
    /// @param sentence 未绑定的参数用“?”代替
    /// @return 1成功；0表示这个语句未被编译过；-1出错
    int finalizeStatement(std::string const& sentence);

    /// @brief 给一个已经编译过的SQL语句绑定参数
    /// @param sentence 
    /// @param index 第几个参数？从1开始计数
    /// @param data 
    /// @param size 
    /// @return 0表示成功；-1表示语句未编译过；-2表示其它错误
    int bind(std::string const& sentence, int32_t index, const void* data, uint32_t size);

    /// @brief 给一个已经编译过的SQL语句绑定参数
    /// @param sentence 
    /// @param index 第几个参数？从1开始计数
    /// @param value 
    /// @return 0表示成功；-1表示语句未编译过；-2表示其它错误
    int bind(std::string const& sentence, int32_t index, double value);

    /// @brief 给一个已经编译过的SQL语句绑定参数
    /// @param sentence 
    /// @param index 第几个参数？从1开始计数
    /// @param value 
    /// @return 0表示成功；-1表示语句未编译过；-2表示其它错误
    int bind(std::string const& sentence, int32_t index, int64_t value);

    /// @brief 给一个已经编译过的SQL语句绑定参数
    /// @param sentence 
    /// @param index 第几个参数？从1开始计数
    /// @param value 
    /// @return 0表示成功；-1表示语句未编译过；-2表示其它错误
    int bind(std::string const& sentence, int32_t index, int32_t value);

    /// @brief 给一个已经编译过的SQL语句绑定参数
    /// @param sentence 
    /// @param index 第几个参数？从1开始计数
    /// @return 0表示成功；-1表示语句未编译过；-2表示其它错误
    int bind(std::string const& sentence, int32_t index);

    /// @brief 给一个已经编译过的SQL语句绑定参数
    /// @param sentence 
    /// @param index 第几个参数？从1开始计数
    /// @param text 
    /// @return 0表示成功；-1表示语句未编译过；-2表示其它错误
    int bind(std::string const& sentence, int32_t index, std::string const& text);

    std::vector<std::shared_ptr<void>> getNextRow()

    ~SQLiteConnection();
    
  };
  
} // namespace SQLiter

#endif // _SQLiter_hpp
