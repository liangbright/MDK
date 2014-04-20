cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK_Test_FileIO)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED QUIET)

set(MDK_DIR "C:/Research/MDK/MDK/Source")

set(Arma_DIR "C:/Research/MDK/armadillo")

set(Lapack_Blas_DIR "C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release")

set(Eigen_DIR "C:/Research/MDK/eigen")

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
${MDK_DIR}/Debug
${MDK_DIR}/Common
${MDK_DIR}/FileIO
${MDK_DIR}/LinearAlgebra/Common
${MDK_DIR}/LinearAlgebra/DenseMatrix
${MDK_DIR}/Image
${Arma_DIR}
${Lapack_Blas_DIR}
${Eigen_DIR}
)

set(Arma_FILES ${Arma_FILES} C:/Research/MDK/armadillo/armadillo.h)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

set(MDK_CPP_FILES 
${MDK_DIR}/Common/mdkObject.cpp
)

set(LinearALgebraLib_win64 
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/lapack_win64_MT.lib  
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/blas_win64_MT.lib
)

message(STATUS ${Test_FILES})

message(STATUS ${Source_FILES})

message(STATUS ${Arma_FILES})

add_executable(MDK_Test_FileIO ${Test_FILES} ${MDK_CPP_FILES})

qt5_use_modules(MDK_Test_FileIO Core)

target_link_libraries(MDK_Test_FileIO ${LinearALgebraLib_win64} ${ITK_LIBRARIES}) 