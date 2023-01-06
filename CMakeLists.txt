cmake_minimum_required(VERSION 3.0.0)
project(SQLiter VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include/)

add_library(SQLiter STATIC ./src/SQLiter.cpp)
# target_link_directories(SQLiter PUBLIC /mnt/d/SQLite/usr/local/lib/) # Reason: when installing SQLite, PREFIX path of configure was set to /mnt/d/SQLite/usr/local/ // 原因：进入SQLite源码目录执行configure时，我指定了prefix=/mnt/d/SQLite/usr/local/
target_link_directories(SQLiter PUBLIC /usr/local/sqlite/lib/) # Reason: when installing SQLite, PREFIX path of configure was set to /usr/local/sqlite/ // 原因：进入SQLite源码目录执行configure时，我指定了prefix=/usr/local/sqlite/
target_link_libraries(SQLiter sqlite3)

add_executable(TestSQLiter test.cpp)
# target_link_directories(TestSQLiter PUBLIC /mnt/d/SQLite/usr/local/lib/) # Reason: when installing SQLite, PREFIX path of configure was set to /mnt/d/SQLite/usr/local/ // 原因：进入SQLite源码目录执行configure时，我指定了prefix=/mnt/d/SQLite/usr/local/
target_link_directories(TestSQLiter PUBLIC ./build/)
target_link_directories(TestSQLiter PUBLIC /usr/local/sqlite/lib/) # Reason: when installing SQLite, PREFIX path of configure was set to /usr/local/sqlite/ // 原因：进入SQLite源码目录执行configure时，我指定了prefix=/usr/local/sqlite/
target_link_libraries(TestSQLiter SQLiter sqlite3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
