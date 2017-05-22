cmake_minimum_required(VERSION 3.8)
 
PROJECT(Test_ResampleMeshBoundary)

set(MDK_DIR "C:/Research/MDK/MDK_Code")
find_package(MDK REQUIRED)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

add_executable(Test_ResampleMeshBoundary ${Test_FILES})

target_link_libraries(Test_ResampleMeshBoundary MDK_LIB)