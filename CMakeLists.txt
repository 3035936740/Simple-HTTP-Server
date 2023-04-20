# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#

cmake_minimum_required (VERSION 3.20)

#vcpkg
# 手动设置到你的vcpkg根目录
set(VCPKG_ROOT "/./home/software/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
CACHE STRING "Vcpkg toolchain file")

#指定C和C++的版本
set(CMAKE_C_COMPILER "/usr/local/bin/gcc")
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CATKIN_TOPLEVEL TRUE)
set(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++")

# cmake版本大于3.12使用C++20的标准

option(OPEN_CXX_20_VERSION "开启C++20" ON)

if (OPEN_CXX_20_VERSION)
	if (CMAKE_VERSION VERSION_GREATER 3.12)
	  set(CMAKE_CXX_STANDARD 20)
	endif()
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (websiteAPI VERSION 0.0.1 LANGUAGES C CXX)
# 输出项目信息
message(STATUS ${PROJECT_NAME} ${PROJECT_VERSION_MAJOR} ${PROJECT_VERSION_MINOR})

# 指定最终的可执行文件的位置
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

##### 添加三方库
# 设置第三方库编译文件输出路径
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 定义三方库变量及路径
set(FMT_MASTER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fmt-master)
set(HTTPLIB_MASTER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cpp-httplib-master)
set(JSON_DEVELOP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/json-develop)
set(SPDLOG_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/spdlog-1.x)
set(YAML_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/yaml-cpp-master)
set(SQLITE_WARPPER_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/sqlite_orm)
set(SQLITE_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/sqlite3_x64-linux)
set(SQLITE_PP_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/sqlitepp)
set(SOURCE_LOCATION_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/source_location-master)
set(JWT_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/jwt-cpp-master)
set(BCRYPT_PATH     ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Bcrypt-cpp-master)

#查找包
#find_package(yaml-cpp REQUIRED)
#find_package(spdlog REQUIRED)
#find_package(nlohmann_json REQUIRED)
#find_package(httplib REQUIRED)
#find_package(fmt REQUIRED)

# 将三方库头文件加入项目
include_directories(${FMT_MASTER_PATH}/include)
include_directories(${SPDLOG_PATH}/include)
include_directories(${HTTPLIB_MASTER_PATH})
include_directories(${JSON_DEVELOP_PATH}/include)
include_directories(${YAML_PATH}/include)
include_directories(${SQLITE_PATH}/include)
include_directories(${SQLITE_PP_PATH}/include)
include_directories(${SQLITE_WARPPER_PATH}/include)
include_directories(${SOURCE_LOCATION_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${JWT_PATH}/include)
include_directories(${BCRYPT_PATH}/include)

link_directories(${SQLITE_PATH}/lib)
link_directories(${SQLITE_PP_PATH}/lib)
# 将三方库加入项目作为子项目
add_subdirectory(${FMT_MASTER_PATH})
add_subdirectory(${SPDLOG_PATH})
add_subdirectory(${HTTPLIB_MASTER_PATH})
add_subdirectory(${JSON_DEVELOP_PATH})
add_subdirectory(${YAML_PATH})
add_subdirectory(${SQLITE_WARPPER_PATH})
add_subdirectory(${SQLITE_PP_PATH})
add_subdirectory(${BCRYPT_PATH})

##### 结束

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# 将源代码添加到此项目的可执行文件。
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
add_executable (${PROJECT_NAME} ${SRC_LIST})

# 将指定目录文件所有复制到二进制文件目录
file(GLOB allCopyFiles  "${CMAKE_CURRENT_SOURCE_DIR}/resource/*")
file(COPY ${allCopyFiles} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 链接第三方库(必须在add_executable后面)
#find_package(jsoncpp CONFIG REQUIRED)
#JsonCpp::JsonCpp
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SqliteOrm REQUIRED)
target_link_libraries(${PROJECT_NAME} fmt httplib nlohmann_json spdlog yaml-cpp sqlite3 unofficial::sqlite3::sqlite3 sqlite_orm::sqlite_orm SQLitePP bcrypt)

message(G:${CMAKE_C_COMPILER_VERSION},GXX:${CMAKE_CXX_COMPILER_VERSION},CXX STANDARD:${CMAKE_CXX_STANDARD})
message(final)