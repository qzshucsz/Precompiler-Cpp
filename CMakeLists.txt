cmake_minimum_required(VERSION 3.9)
project(Precomplier)

set(CMAKE_CXX_STANDARD 11)

add_executable(Precomplier main.cpp MRegex.cpp MRegex.h IncludeHeadFilePath.cpp IncludeHeadFilePath.h StringProcessor.cpp StringProcessor.h)