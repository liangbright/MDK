cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(Test_with_mdk)

set(MDK_DIR "C:/Research/MDK/MDK/Source")

set(Arma_DIR "C:/Research/MDK/armadillo")

set(Eigen_DIR "C:/Research/MDK/eigen")

set(qpOASES_DIR "C:/Research/MDK/qpOASES")

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
${MDK_DIR}/Debug
${MDK_DIR}/Common
${MDK_DIR}/LinearAlgebra/Common
${MDK_DIR}/LinearAlgebra/DenseMatrix
${Arma_DIR}
${Eigen_DIR}
${qpOASES_DIR}
)

set(MDK_CPP_FILES 
${MDK_DIR}/Common/mdkObject.cpp
)

FILE(GLOB qpOASES_CCP_FILES C:/Research/MDK/qpOASES/qpOASES/*.cpp)

set(LinearALgebraLib_win64 
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/lapack_win64_MT.lib  
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/blas_win64_MT.lib
)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

message(STATUS ${Test_FILES})

add_executable(Test_with_mdk ${Test_FILES} ${MDK_CPP_FILES} ${qpOASES_CCP_FILES})

target_link_libraries(Test_with_mdk ${LinearALgebraLib_win64}) 