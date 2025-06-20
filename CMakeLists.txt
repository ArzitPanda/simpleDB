cmake_minimum_required(VERSION 3.31)
project(simpledb)

set(CMAKE_CXX_STANDARD 20)
include_directories(include db src/utils src/parser)
add_executable(simpledb main.cpp src/db_manager.cpp
        src/table_manager.cpp src/table.cpp src/utils/TableUtils.cpp
        src/parser/sql_parser.cpp
)
