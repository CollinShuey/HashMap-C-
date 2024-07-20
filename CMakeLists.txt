cmake_minimum_required(VERSION 3.26)
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(MyProject
        UnitTests/tests.cpp
        HashMap/HashMap.tpp
        HashMap/HashMap.h
        # Add source files here
        # List other necessary .cpp files here
)

# Specify include directories
target_include_directories(MyProject PRIVATE
        ${PROJECT_SOURCE_DIR}/HashMap
        ${PROJECT_SOURCE_DIR}/Utils
)