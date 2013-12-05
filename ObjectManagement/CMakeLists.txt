cmake_minimum_required(VERSION 2.8)
 
PROJECT(ObjectManagement)

include_directories(${CMAKE_SOURCE_DIR})
 
add_executable(Test_ObjectManagement mdkDebugMarco.h mdkObject.h mdkObject.hpp mdkObjectManager.h mdkObjectManager.cpp mdkObjectBase.h mdkObjectBase.cpp mdkObjectBaseInfo.h mdkObjectBaseInfo.cpp mdkImageFilter.h mdkImageFilter.cpp test.cpp)