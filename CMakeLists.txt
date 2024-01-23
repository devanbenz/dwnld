cmake_minimum_required(VERSION 3.27)
project(dwnld)

set(CMAKE_CXX_STANDARD 23)

find_package(CURL REQUIRED)

add_library(dwnld_lib
        include/logger/Logger.h
        include/logger/Logger.cpp
        include/base-downloader/BaseDownloader.h
        include/http-downloader/HttpDownloader.h
        include/http-downloader/HttpDownloader.cpp)

target_include_directories(dwnld_lib PUBLIC
        include/base-downloader
        include/http-downloader
        include/logger
        ${CURL_INCLUDE_DIRS})

add_executable(dwnld main.cpp)

target_link_libraries(dwnld dwnld_lib ${CURL_LIBRARIES})