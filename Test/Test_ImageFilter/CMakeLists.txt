cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK_Test)

set(VTK_DIR C:/Programs/VTK/VTK_6.1_VC2013_Build)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt5Widgets)

set(Debug_DIR "C:/Research/MDK/Code/Source/Debug")

set(Common_DIR "C:/Research/MDK/Code/Source/Common")

set(ImageFilter_DIR "C:/Research/MDK/Code/Source/ImageFilter")

set(FileIO_DIR "C:/Research/MDK/Code/Source/FileIO")

set(Arma_DIR "C:/Research/MDK/armadillo_4.000.2")

set(Arma_Source_DIR "C:/Research/MDK/armadillo_4.000.2/armadillo_bits")

set(Lapack_Blas_DIR "C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release")

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
${Common_DIR} ${Debug_DIR} ${ImageFilter_DIR} ${FileIO_DIR}
${Arma_DIR} ${Arma_Source_DIR} ${Lapack_Blas_DIR}
)

#FILE(GLOB Arma_FILES ${Arma_Source_DIR}/*.hpp)

FILE(GLOB Arma_FILES C:/Research/MDK/armadillo_4.000.2/armadillo_bits/*.hpp)

set(Arma_FILES ${Arma_FILES} C:/Research/MDK/armadillo_4.000.2/armadillo.h)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

set(Source_FILES 

${Debug_DIR}/mdkDebugConfig.h

${Common_DIR}/mdkType.h

${Common_DIR}/mdkObject.h
${Common_DIR}/mdkObject.cpp

${Common_DIR}/mdkMatrix.h
${Common_DIR}/mdkMatrix.hpp

${Common_DIR}/mdk3DImageVoxel.h
${Common_DIR}/mdk3DImageVoxel.hpp

${Common_DIR}/mdk3DImage.h
${Common_DIR}/mdk3DImage.hpp

${ImageFilter_DIR}/mdk3DImageFilter.h
${ImageFilter_DIR}/mdk3DImageFilter.hpp

${ImageFilter_DIR}/mdk3DImageConvolutionFilterBase.h
${ImageFilter_DIR}/mdk3DImageConvolutionFilterBase.cpp

${ImageFilter_DIR}/mdk3DImageConvolutionFilter.h
${ImageFilter_DIR}/mdk3DImageConvolutionFilter.hpp

${FileIO_DIR}/mdkFileIO.h
${FileIO_DIR}/mdkFileIO.cpp
)

set(LinearALgebraLib_win64 C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/lapack_win64_MT.lib  C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/blas_win64_MT.lib)

message(STATUS ${Test_FILES})

message(STATUS ${Source_FILES})

message(STATUS ${Arma_FILES})

add_executable(MDK_Test ${Test_FILES} ${Source_FILES} ${Arma_FILES})

target_link_libraries(MDK_Test ${LinearALgebraLib_win64} ${VTK_LIBRARIES}) 