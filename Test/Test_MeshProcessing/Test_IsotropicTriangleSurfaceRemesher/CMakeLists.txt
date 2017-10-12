cmake_minimum_required(VERSION 3.8)
 
PROJECT(IsoTriangleRemesher)

set(MDK_DIR "C:/Research/MDK/MDK_Code" CACHE PATH "path")
find_package(MDK REQUIRED)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

add_executable(IsoTriangleRemesher ${Test_FILES})

target_link_libraries(IsoTriangleRemesher MDK_LIB)