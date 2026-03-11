cmake_minimum_required(VERSION 3.20)
project(SenderProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_COMPILER g++)

add_executable(sender src/main.cpp)

target_include_directories(sender PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(sender PRIVATE argumentsea scok)

install(TARGETS sender RUNTIME DESTINATION /usr/bin)