cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
PROJECT(Test_GradientFilter3D)

set(MDK_DIR "C:/Research/MDK/MDK_Code")
find_package(MDK REQUIRED)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

add_executable(Test_GradientFilter3D ${Test_FILES})

target_link_libraries(Test_GradientFilter3D MDK_LIB)