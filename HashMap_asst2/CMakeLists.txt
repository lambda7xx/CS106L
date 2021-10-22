cmake_minimum_required (VERSION 3.5)
project(HashMap CXX C )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
set(CMAKE_CXX_FLAGS "-std=c++14")

add_executable(myHashMap  main.cpp hashmap.h student_main.cpp test_settings.cpp hashmap_iterator.h hashmap.cpp tests.cpp )