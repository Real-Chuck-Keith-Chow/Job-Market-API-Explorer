cmake_minimum_required(VERSION 3.12)
project(JobMarketAPIExplorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/third_party)

# Add executable
add_executable(job_explorer
    src/main.cpp
    src/ApiClient.cpp
    src/JobParser.cpp
    src/Database.cpp
)

# Link libraries
target_link_libraries(job_explorer ${CURL_LIBRARIES} OpenSSL::SSL)
