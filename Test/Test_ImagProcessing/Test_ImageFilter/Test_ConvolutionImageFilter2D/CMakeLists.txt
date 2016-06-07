cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
PROJECT(Test_ConvolutionImageFilter2D)

set(MDK_DIR "C:/Research/MDK/MDK_Code")
find_package(MDK REQUIRED)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

add_executable(Test_ConvolutionImageFilter2D ${Test_FILES})

target_link_libraries(Test_ConvolutionImageFilter2D MDK_LIB)