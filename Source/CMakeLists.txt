cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
find_package(Qt5Widgets REQUIRED QUIET)
   
set(VTKLIB_DIR "C:/Programs/VTK/VTK_6_VC_Build/lib")


set(MDK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

set(MDK_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/Debug)

add_subdirectory(Common)

add_subdirectory(ImageFilter) 