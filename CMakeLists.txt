## --------------------------------------------------------
## cmake file for the 'ipmt' project.
##
## @author: Vinicius Carneiro <vcps@cin.ufpe.br>
## --------------------------------------------------------

# --------------- PROJECT ---------------
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set (PROJECT_NAME "ipmt" CXX)
project(${PROJECT_NAME})


# --------------- SETTING VARS ---------------
# dirs
set(PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES src/*.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# --------------- INCLUDING DIRS ---------------
include_directories(${PROJECT_SOURCE_DECLARATION_DIRECTORY})
include_directories("libs")


# --------------- INCLUDING EXECUTABLES ---------------
add_executable(${PROJECT_NAME} ${SOURCES})
