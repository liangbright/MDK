cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK_Test_SparseMatrix)

set(VTK_DIR C:/Programs/VTK/VTK_6.1_VC2013_Build)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt5Widgets)

set(MDK_Source_DIR "C:/Research/MDK/Code/Source")

set(Arma_DIR "C:/Research/MDK/armadillo")

set(Lapack_Blas_DIR "C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release")

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
${MDK_Source_DIR}
${MDK_Source_DIR}/Debug
${MDK_Source_DIR}/Common
${MDK_Source_DIR}/LinearAlgebra
${MDK_Source_DIR}/LinearAlgebra/DenseMatrix
${MDK_Source_DIR}/LinearAlgebra/SparseMatrix
${Arma_DIR} 
${Arma_Source_DIR} 
${Lapack_Blas_DIR}
)

#FILE(GLOB Arma_FILES ${Arma_DIR}/armadillo_bits/*.hpp)

FILE(GLOB Arma_FILES C:/Research/MDK/armadillo/armadillo_bits/*.hpp)

set(Arma_FILES ${Arma_FILES} C:/Research/MDK/armadillo/armadillo.h)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

set(Source_FILES 

${MDK_Source_DIR}/Common/mdkObject.cpp

${MDK_Source_DIR}/LinearAlgebra/mdkMatrix_Common.cpp

)

set(LinearALgebraLib_win64 
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/lapack_win64_MT.lib  
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/blas_win64_MT.lib
)

message(STATUS ${Test_FILES})

message(STATUS ${Source_FILES})

message(STATUS ${Arma_FILES})

add_executable(MDK_Test_SparseMatrix ${Test_FILES} ${Source_FILES} ${Arma_FILES})

target_link_libraries(MDK_Test_SparseMatrix ${LinearALgebraLib_win64} ${VTK_LIBRARIES}) 