cmake_minimum_required(VERSION 3.31)
project(simpledb)

set(CMAKE_CXX_STANDARD 20)
include_directories(include db)
add_executable(simpledb main.cpp src/db_manager.cpp
        src/table_manager.cpp src/table.cpp
)
