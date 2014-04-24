cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")
 
PROJECT(MDK_Test_SPAMSOnlineDictionaryBuilder)

set(MDK_DIR "C:/Research/MDK/MDK/Source")

set(Arma_DIR "C:/Research/MDK/armadillo")

set(Eigen_DIR "C:/Research/MDK/eigen")

set(qpOASES_DIR "C:/Research/MDK/qpOASES")

set(Spams_DIR "C:/Research/MDK/spams")

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
${MDK_DIR}/Debug
${MDK_DIR}/Common
${MDK_DIR}/FileIO
${MDK_DIR}/Multithreading
${MDK_DIR}/LinearAlgebra/Common
${MDK_DIR}/LinearAlgebra/DenseMatrix
${MDK_DIR}/LinearAlgebra/SparseMatrix
${MDK_DIR}/LinearAlgebra/SparseVector
${MDK_DIR}/Optimization
${MDK_DIR}/FeatureCoding
${Arma_DIR} 
${Eigen_DIR}
${qpOASES_DIR}
${Spams_DIR}
)

set(MDK_CPP_FILES 
${MDK_DIR}/Common/mdkObject.cpp
)

FILE(GLOB qpOASES_CCP_FILES C:/Research/MDK/qpOASES/qpOASES/*.cpp)

set(LinearALgebraLib_win64 
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/lapack_win64_MT.lib  
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/blas_win64_MT.lib
)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED QUIET)

set(OpenCV_DIR C:/Programs/OpenCV/VC2013_build)
find_package(OpenCV REQUIRED)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

message(STATUS ${Test_FILES})

add_executable(MDK_Test_SPAMSOnlineDictionaryBuilder ${Test_FILES} ${MDK_CPP_FILES} ${qpOASES_CCP_FILES})

qt5_use_modules(MDK_Test_SPAMSOnlineDictionaryBuilder Core)

target_link_libraries(MDK_Test_SPAMSOnlineDictionaryBuilder ${LinearALgebraLib_win64} ${OpenCV_LIBS} ${ITK_LIBRARIES})