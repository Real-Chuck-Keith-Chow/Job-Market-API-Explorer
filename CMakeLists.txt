cmake_minimum_required(VERSION 3.12)
project(JobMarketAPIExplorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_SQLITE "Build with SQLite support" OFF)

# Find required packages
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

if(WITH_SQLITE)
    find_package(SQLite3 REQUIRED)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/third_party)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add executable
add_executable(job_explorer
    src/main.cpp
    src/ApiClient.cpp
    src/JobParser.cpp
)

if(WITH_SQLITE)
    target_sources(job_explorer PRIVATE src/Database.cpp)
    target_link_libraries(job_explorer PRIVATE SQLite::SQLite3)
    target_compile_definitions(job_explorer PRIVATE HAVE_SQLITE)
endif()

# Link libraries
target_link_libraries(job_explorer
    CURL::libcurl
    OpenSSL::SSL
)
