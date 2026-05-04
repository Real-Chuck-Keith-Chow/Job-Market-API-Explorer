cmake_minimum_required(VERSION 3.15)

project(JobMarketAPIExplorer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(JobMarketAPIExplorer
    src/main.cpp
    src/ApiClient.cpp
    src/Database.cpp
    src/JobParser.cpp
)

target_include_directories(JobMarketAPIExplorer PRIVATE
    src
    src/model
    third_party
)

target_link_libraries(JobMarketAPIExplorer PRIVATE
    CURL::libcurl
    SQLite::SQLite3
)
