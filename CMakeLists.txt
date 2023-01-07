cmake_minimum_required(VERSION 3.0.0)
project(SQLiter VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include/)
include_directories(/usr/local/sqlite/include/) # Reason: when installing SQLite, PREFIX path of configure was set to /usr/local/sqlite // 原因：进入SQLite源码目录执行configure时，我指定了prefix=/usr/local/sqlite

add_executable(TestSQLiter test.cpp)
target_link_directories(TestSQLiter PUBLIC /usr/local/sqlite/lib/) # Reason: when installing SQLite, PREFIX path of configure was set to /usr/local/sqlite // 原因：进入SQLite源码目录执行configure时，我指定了prefix=/usr/local/sqlite
target_link_libraries(TestSQLiter sqlite3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
