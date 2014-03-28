cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()
 
PROJECT(MDK_TestEigen)
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

set(VTK_DIR C:/Programs/VTK/VTK_6.1_VC2013_Build)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt5Widgets)

set(OpenCV_DIR C:/Programs/OpenCV/VC2013_build)

find_package(OpenCV REQUIRED)

set(MDK_DIR "C:/Research/MDK/Code/Source")

set(Arma_DIR "C:/Research/MDK/armadillo")

set(Lapack_Blas_DIR "C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release")

set(Eigen_DIR "C:/Research/MDK/eigen")

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
${MDK_DIR}/Debug
${MDK_DIR}/Common
${MDK_DIR}/LinearAlgebra/Common
${MDK_DIR}/LinearAlgebra/DenseMatrix
${MDK_DIR}/LinearAlgebra/SparseMatrix
${MDK_DIR}/FeatureCoding
${Arma_DIR} 
${Lapack_Blas_DIR}
${Eigen_DIR}
)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

set(CPP_FILES 

${MDK_DIR}/Common/mdkObject.cpp

${MDK_DIR}/LinearAlgebra/Common/mdkMatrix_Common.cpp
)

set(LinearALgebraLib_win64 
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/lapack_win64_MT.lib  
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/blas_win64_MT.lib
)

message(STATUS ${Test_FILES})

message(STATUS ${Source_FILES})

message(STATUS ${Arma_FILES})

add_executable(MDK_TestEigen ${Test_FILES} ${CPP_FILES} )

target_link_libraries(MDK_TestEigen ${LinearALgebraLib_win64} ${VTK_LIBRARIES} ${OpenCV_LIBS})