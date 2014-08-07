cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()
 
PROJECT(Test_KNNAverageOnlineDictionaryBuilder)

set(MDK_DIR "C:/Research/MDK")
find_package(MDK REQUIRED)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

add_executable(Test_KNNAverageOnlineDictionaryBuilder ${Test_FILES})

target_link_libraries(Test_KNNAverageOnlineDictionaryBuilder MDK_LIB)