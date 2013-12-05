cmake_minimum_required(VERSION 2.8)
 
PROJECT(Test_ObjectManagement)

include_directories(${CMAKE_SOURCE_DIR})

set(src_DIR "C:/Research/MDK/MDK_0.1/ObjectManagement/Code/src")

include_directories(${src_DIR})

AUX_SOURCE_DIRECTORY(${src_DIR} SourceFile)
 
add_executable(Test_ObjectManagement ${SourceFile} mdkImageFilter.h mdkImageFilter.cpp test.cpp)