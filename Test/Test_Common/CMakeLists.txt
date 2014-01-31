cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK_Test)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
set(VTKLIB_DIR "C:/Programs/VTK/VTK_6_VC_Build/lib")
 
find_package(Qt5Widgets REQUIRED QUIET)

set(Debug_DIR "C:/Research/MDK/Code/Source/Debug")

set(Common_DIR "C:/Research/MDK/Code/Source/Common")

set(ImageFilter_DIR "C:/Research/MDK/Code/Source/ImageFilter")

set(Arma_DIR "C:/Research/MDK/Code/Source")

set(Lapack_Blas_DIR "C:/Research/MDK/LinearALgebraLib_win64")
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Common_DIR} ${Debug_DIR} ${Arma_DIR} ${Lapack_Blas_DIR} ${ImageFilter_DIR})

FILE(GLOB Arma_FILES "C:/Research/MDK/Code/Source/armadillo_bits/*.hpp")

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

set(Source_FILES 

${Debug_DIR}/mdkDebug.h

${Common_DIR}/mdkType.h

${Common_DIR}/mdkObject.h
${Common_DIR}/mdkObject.cpp

${Common_DIR}/mdkMatrix.h
${Common_DIR}/mdkMatrix.hpp

${Common_DIR}/mdkLinearAlgebra.h
${Common_DIR}/mdkLinearAlgebra.hpp

${Common_DIR}/mdk3DImage.h
${Common_DIR}/mdk3DImage.hpp

${ImageFilter_DIR}/mdk3DImageFilter.h
${ImageFilter_DIR}/mdk3DImageFilter.cpp

#${ImageFilter_DIR}/mdk3DImageFilterFunction.h
#${ImageFilter_DIR}/mdk3DImageFilterFunction.cpp


#${Core_DIR}/mdkImageRegionProperty.h
#${Core_DIR}/mdkImageRegionProperty.cpp
)

set(LinearALgebraLib_win64 C:/Research/MDK/LinearALgebraLib_win64/lapack_win64_MT.lib  C:/Research/MDK/LinearALgebraLib_win64/blas_win64_MT.lib)

message(STATUS ${Test_FILES})

message(STATUS ${Core_FILES})

message(STATUS ${Arma_FILES})

add_executable(MDK_Test ${Test_FILES} ${Source_FILES} ${Arma_FILES})

qt5_use_modules(MDK_Test Core GUI)

target_link_libraries(MDK_Test ${VTK_LIBRARIES} ${LinearALgebraLib_win64}) 