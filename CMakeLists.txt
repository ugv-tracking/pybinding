cmake_minimum_required(VERSION 2.8)
project(test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include_directories(SYSTEM ${CONAN_INCLUDE_DIRS})
include(CheckCXXCompilerFlag)

# for pybind11
find_package(PythonLibs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PCL 1.8 REQUIRED)

include_directories(
    ${PYTHON_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/pybind11/include
    ${OpenCV_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS})

set(LINK_LIBS ${OpenCV_LIBRARIES} ${PCL_LIBRARIES})
set(LINK_DEPS ${OpenCV_DEFINITIONS} ${PCL_DEFINITIONS})

#FILE(GLOB_RECURSE SRC_LIST "src/*.cpp")
#FILE(GLOB_RECURSE HAD_LIST "src/*.h")

# Create the binding library
add_library(example SHARED
    example.cpp
)
add_dependencies(example ${LINK_DEPS})
target_link_libraries(example ${LINK_LIBS})
set_target_properties(example PROPERTIES PREFIX "")
set_target_properties(example PROPERTIES SUFFIX ".so")

add_custom_command(TARGET example POST_BUILD COMMAND strip ${PROJECT_BINARY_DIR}/example.so)
