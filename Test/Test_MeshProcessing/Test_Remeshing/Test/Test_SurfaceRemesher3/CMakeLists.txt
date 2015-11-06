cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
PROJECT(Test_SurfaceRemesher3)

set(MDK_DIR "C:/Research/MDK/MDK_Code")
find_package(MDK REQUIRED)

set(Source_DIR C:/Research/Mesh/Remeshing/Source)

include_directories(${Source_DIR})

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

add_executable(Test_SurfaceRemesher3 ${Test_FILES})

target_link_libraries(Test_SurfaceRemesher3 MDK_LIB)