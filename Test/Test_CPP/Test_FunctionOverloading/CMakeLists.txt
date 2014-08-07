cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK_Test_FunctionOverloading)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
)

FILE(GLOB Test_FILES "*.h" "*.cpp")

add_executable(MDK_Test_FunctionOverloading ${Test_FILES})