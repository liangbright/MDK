cmake_minimum_required(VERSION 2.8)
 
#if(POLICY CMP0020)
#  cmake_policy(SET CMP0020 NEW)
#endif()
 
PROJECT(Test_TriangleMesh)

set(MDK_DIR "C:/Research/MDK/MDK_Code")
find_package(MDK REQUIRED)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

message(STATUS ${Test_FILES})

add_executable(Test_TriangleMesh ${Test_FILES})

target_link_libraries(Test_TriangleMesh MDK_LIB)