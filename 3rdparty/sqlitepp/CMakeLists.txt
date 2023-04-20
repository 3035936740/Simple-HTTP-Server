cmake_minimum_required(VERSION 3.12)

#创建工程文件
project(SQLitePP)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE_DIR}/include/sqlite3pp.cpp ${PROJECT_SOURCE_DIR}/include/sqlite3ppext.cpp)

set(CMAKE_BUILD_TYPE "Release")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
