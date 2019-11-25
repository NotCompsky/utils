cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)


set(CMAKE_CXX_STANDARD 17)


add_executable(qt-ask main.cpp name_dialog.cpp)
target_link_libraries(qt-ask Qt5::Widgets)
