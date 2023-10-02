cmake_minimum_required(VERSION 3.20)

project(Calibron12 CXX)

set(CMAKE_CXX_STANDARD 17)

set(Calibron12_CPP_FILES src/main.cpp src/Calibron12.cpp src/rectangle.cpp src/Solution.cpp src/render_rectangle.cpp src/svg.cpp )
set(Calibron12_H_FILES src/Solution.h src/rectangle.h src/render_rectangle.h src/svg.h src/SimpleTimer.h) 
add_executable(${PROJECT_NAME} ${Calibron12_CPP_FILES} ${Calibron12_H_FILES})

set(PROJECT_NAME_TEST ${PROJECT_NAME}_TEST ) 
set(Calibron12_TEST_CPP_FILES src_test/main.cpp src_test/rectangle_test.cpp src/rectangle.cpp)
add_executable(PROJECT_NAME_TEST ${Calibron12_TEST_CPP_FILES} ${Calibron12_H_FILES})
target_include_directories(PROJECT_NAME_TEST PRIVATE src/)


set(SVG_CPP_FILES SVG/main.cpp src/svg.cpp src/render_rectangle.cpp src/rectangle.cpp)
add_executable(SVG ${SVG_CPP_FILES} ${Calibron12_H_FILES})
