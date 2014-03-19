cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK_TestFeatureCoding)

set(VTK_DIR C:/Programs/VTK/VTK_6.1_VC2013_Build)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt5Widgets)

set(OpenCV_DIR C:/Programs/OpenCV/VC2013_build)

find_package(OpenCV REQUIRED)

set(Debug_DIR "C:/Research/MDK/Code/Source/Debug")

set(Common_DIR "C:/Research/MDK/Code/Source/Common")

set(Image_DIR "C:/Research/MDK/Code/Source/Image")

set(ImageFilter_DIR "C:/Research/MDK/Code/Source/ImageFilter")

set(LinearAlgebra_DIR "C:/Research/MDK/Code/Source/LinearAlgebra")

set(FeatureCoding_DIR "C:/Research/MDK/Code/Source/FeatureCoding")

set(FileIO_DIR "C:/Research/MDK/Code/Source/FileIO")

set(Arma_DIR "C:/Research/MDK/armadillo_4.000.2")

set(Arma_Source_DIR "C:/Research/MDK/armadillo_4.000.2/armadillo_bits")

set(Lapack_Blas_DIR "C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release")

include_directories(
${CMAKE_CURRENT_SOURCE_DIR} 
${Common_DIR} 
${Debug_DIR} 
${FileIO_DIR} 
${ImageData_DIR} 
${ImageFilter_DIR} 
${LinearAlgebra_DIR}
${FeatureCoding_DIR}
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
${LinearAlgebra_DIR}/mdkMatrix.cpp

${LinearAlgebra_DIR}/mdkShadowMatrix.h
${LinearAlgebra_DIR}/mdkShadowMatrix.hpp

${LinearAlgebra_DIR}/mdkGlueMatrixForLinearCombination.h
${LinearAlgebra_DIR}/mdkGlueMatrixForLinearCombination.hpp

${LinearAlgebra_DIR}/mdkGlueMatrixForMultiplication.h
${LinearAlgebra_DIR}/mdkGlueMatrixForMultiplication.hpp

${LinearAlgebra_DIR}/mdkLinearAlgebra.h

${Common_DIR}/mdkTypeToOpenCVTranslation.h
${Common_DIR}/mdkTypeToOpenCVTranslation.hpp

${FeatureCoding_DIR}/mdkFeatureDictionary.h
${FeatureCoding_DIR}/mdkFeatureDictionary.hpp

${FeatureCoding_DIR}/mdkFeatureEncoder.h
${FeatureCoding_DIR}/mdkFeatureEncoder.hpp

${FeatureCoding_DIR}/mdkFeatureEncoderDictionaryBuilder.h
${FeatureCoding_DIR}/mdkFeatureEncoderDictionaryBuilder.hpp

${FeatureCoding_DIR}/mdkFeatureSparseEncoder.h
${FeatureCoding_DIR}/mdkFeatureSparseEncoder.hpp

${FeatureCoding_DIR}/mdkKMeansDictionaryBuilder.h
${FeatureCoding_DIR}/mdkKMeansDictionaryBuilder.hpp

${FeatureCoding_DIR}/mdkKNNReconstructionSparseEncoder.h
${FeatureCoding_DIR}/mdkKNNReconstructionSparseEncoder.hpp

${FeatureCoding_DIR}/mdkKNNSoftAssignSparseEncoder.h
${FeatureCoding_DIR}/mdkKNNSoftAssignSparseEncoder.hpp

)

set(LinearALgebraLib_win64 
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/lapack_win64_MT.lib  
C:/Research/MDK/LinearALgebraLib_win64/Lapack_win64_release/blas_win64_MT.lib
)

message(STATUS ${Test_FILES})

message(STATUS ${Source_FILES})

message(STATUS ${Arma_FILES})

add_executable(MDK_TestFeatureCoding ${Test_FILES} ${Source_FILES} ${Arma_FILES})

target_link_libraries(MDK_TestFeatureCoding ${LinearALgebraLib_win64} ${VTK_LIBRARIES} ${OpenCV_LIBS})