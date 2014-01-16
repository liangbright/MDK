cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64")

PROJECT(MDK_Test_Core)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
set(VTKLIB_DIR "C:/Programs/VTK/VTK_6_VC_Build/lib")
 
find_package(Qt5Widgets REQUIRED QUIET)

set(Debug_DIR "C:/Research/MDK/Code/Source/Common/Debug")

set(Core_DIR "C:/Research/MDK/Code/Source/Common/Core")
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Core_DIR} ${Debug_DIR})
  
FILE(GLOB Test_FILES "*.h" "*.hpp" "*.cpp")

set(Core_FILES 
${Core_DIR}/mdkType.h

${Core_DIR}/mdkObject.h
${Core_DIR}/mdkObject.cpp

${Core_DIR}/mdkDataObject.h
${Core_DIR}/mdkDataObject.cpp

${Core_DIR}/mdkAlgorithmObject.h
${Core_DIR}/mdkAlgorithmObject.cpp

${Core_DIR}/mdkDataArray.h
${Core_DIR}/mdkDataArray.hpp

#${Core_DIR}/mdkPointSet.h
#${Core_DIR}/mdkPointSet.cpp


${Core_DIR}/mdkMatrix.h
${Core_DIR}/mdkMatrix.hpp

${Core_DIR}/mdkImage.h
${Core_DIR}/mdkImage.cpp

#${Core_DIR}/mdkImageRegion.h
#${Core_DIR}/mdkImageRegion.cpp


${Debug_DIR}/mdkDebug.h
)


message(STATUS Show_FILES)

message(STATUS ${Test_FILES})

message(STATUS ${Core_FILES})

add_executable(MDK_Test_Core ${Test_FILES} ${Core_FILES})

qt5_use_modules(MDK_Test_Core Core GUI)

target_link_libraries(MDK_Test_Core ${VTK_LIBRARIES}) 