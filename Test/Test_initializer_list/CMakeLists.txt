cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK_Test_initializer_list)

set(VTK_DIR C:/Programs/VTK/VTK_6.1_VC2013_Build)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt5Widgets)

set(Debug_DIR "C:/Research/MDK/Code/Source/Debug")

set(Common_DIR "C:/Research/MDK/Code/Source/Common")

set(ImageData_DIR "C:/Research/MDK/Code/Source/ImageData")

set(LinearAlgebra_DIR "C:/Research/MDK/Code/Source/LinearAlgebra")

set(Arma_DIR "C:/Research/MDK/armadillo_4.000.2")

set(Arma_Source_DIR "C:/Research/MDK/armadillo_4.000.2/armadillo_bits")

set(Lapack_Blas_DIR "C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release")

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
${Common_DIR} 
${Debug_DIR} 
${ImageData_DIR} 
${LinearAlgebra_DIR}
${Arma_DIR} 
${Arma_Source_DIR} 
${Lapack_Blas_DIR}
)

#FILE(GLOB Arma_FILES ${Arma_Source_DIR}/*.hpp)

FILE(GLOB Arma_FILES C:/Research/MDK/armadillo_4.000.2/armadillo_bits/*.hpp)

set(Arma_FILES ${Arma_FILES} C:/Research/MDK/armadillo_4.000.2/armadillo.h)

FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

set(Source_FILES 

${Debug_DIR}/mdkDebugConfig.h

${Common_DIR}/mdkType.h
${Common_DIR}/mdkType.hpp

${Common_DIR}/mdkObject.h
${Common_DIR}/mdkObject.cpp

${LinearAlgebra_DIR}/mdkMatrix.h
${LinearAlgebra_DIR}/mdkMatrix.hpp

${LinearAlgebra_DIR}/mdkShadowMatrix.h
${LinearAlgebra_DIR}/mdkShadowMatrix.hpp

${LinearAlgebra_DIR}/mdkGlueMatrix.h
${LinearAlgebra_DIR}/mdkGlueMatrix.hpp

${LinearAlgebra_DIR}/mdkLinearAlgebra.h
${LinearAlgebra_DIR}/mdkLinearAlgebra.hpp

${ImageData_DIR}/mdk3DImage.h
${ImageData_DIR}/mdk3DImage.hpp
)

set(LinearALgebraLib_win64
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/lapack_win64_MT.lib  
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/blas_win64_MT.lib
)

message(STATUS ${Test_FILES})

message(STATUS ${Source_FILES})

message(STATUS ${Arma_FILES})

add_executable(Test_initializer_list ${Test_FILES} ${Source_FILES} ${Arma_FILES})

target_link_libraries(Test_initializer_list ${LinearALgebraLib_win64} ${VTK_LIBRARIES}) 