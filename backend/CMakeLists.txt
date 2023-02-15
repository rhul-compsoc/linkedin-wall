# Project Setup
cmake_minimum_required(VERSION 3.9)

project(
  LinkedinWallBackend
  VERSION 0.1
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(COMPILER_FLAGS
    "-Og -Wno-unused-parameter -Wall -Wextra -Wpedantic -Werror -g")
set(CMAKE_C_FLAGS "${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS}")

set(REPO_URL "https://github.com/rhul-compsoc/linkedin-wall")

message(STATUS "Building ${CMAKE_PROJECT_NAME}")
message(STATUS "Git repo: ${REPO_URL}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "Build OS: ${CMAKE_SYSTEM_NAME}")

# Get the branch name
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Commit info: ${GIT_COMMIT_HASH}@${GIT_BRANCH}")

# Version info
add_definitions("-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")
add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")
add_definitions(
  "-DVERSION=\"${CMAKE_PROJECT_VERSION} (${GIT_COMMIT_HASH}@${GIT_BRANCH})\"")
add_definitions("-DOS=\"${CMAKE_SYSTEM_NAME}\"")
add_definitions("-DREPO_URL=\"${REPO_URL}\"")
add_definitions("-DBUFFER_LENGTH=4096")
add_definitions("-DPROJECT_VERSION=\"${CMAKE_PROJECT_VERSION}\"")
add_definitions("-DPROJECT_NAME=\"${CMAKE_PROJECT_NAME}\"")

# __FILENAME__
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
add_definitions("-D__FILENAME__=(__FILE__ + SOURCE_PATH_SIZE)")

# Setup doxygen
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

# Add Libraries
add_subdirectory(json)

set(LIBS curl nlohmann_json::nlohmann_json pqxx pq pthread)

# Sources
set(CONFIG_SRC ./config/parser.h ./config/parser.cpp ./config/config.h
               ./config/config.cpp)
set(MODEL_SRC ./model/auth.h ./model/auth.cpp)
set(SERVER_SRC ./server/server.h ./server/server.cpp ./server/connection.h
               ./server/connection.cpp)
set(SRC
    ./testing.h/logger.h
    ./testing.h/logger.c
    ./testing.h/ansi_colour.h
    ./mongoose/mongoose.h
    ./mongoose/mongoose.c
    ${MODEL_SRC}
    ${CONFIG_SRC}
    ${SERVER_SRC})
set(TEST ${SRC} ./testing.h/testing.h ./testing.h/testing.c ./test_main.cpp
         ./config/test_config.h ./config/test_config.cpp)

# Main executable
add_executable(linkedin-wall ./main.cpp ${SRC})
target_link_libraries(linkedin-wall ${LIBS})
doxygen_add_docs(linkedin-wall-docs ${SRC} "Generate documentation pages")

# Test executable
add_executable(linkedin-wall-tests ${TEST})
target_link_libraries(linkedin-wall-tests ${LIBS})

enable_testing()
add_test(linkedin-wall-tests linkedin-wall-tests)
