cmake_minimum_required(VERSION 3.12)
project(JobMarketAPIExplorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/third_party)

add_executable(job_explorer
    src/main.cpp
    src/ApiClient.cpp
    src/JobParser.cpp
    src/Database.cpp
)

target_link_libraries(job_explorer
    ${CURL_LIBRARIES}
    OpenSSL::SSL
    SQLite::SQLite3
)
