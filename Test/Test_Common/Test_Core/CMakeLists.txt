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

${Debug_DIR}/mdkDebug.h

${Core_DIR}/mdkObject.h
${Core_DIR}/mdkObject.cpp

${Core_DIR}/mdkAbstractStructuredDataArrayTemplate.h
${Core_DIR}/mdkAbstractStructuredDataArrayTemplate.cpp

${Core_DIR}/mdkStructuredDataArrayTemplate.h
${Core_DIR}/mdkStructuredDataArrayTemplate.hpp

${Core_DIR}/mdkStructuredDataArray.h
${Core_DIR}/mdkStructuredDataArray.cpp

${Core_DIR}/mdkMatrix.h
${Core_DIR}/mdkMatrix.cpp

${Core_DIR}/mdkImage.h
${Core_DIR}/mdkImage.cpp

${Core_DIR}/mdkImageRegionProperty.h
${Core_DIR}/mdkImageRegionProperty.cpp

)


message(STATUS Show_FILES)

message(STATUS ${Test_FILES})

message(STATUS ${Core_FILES})

add_executable(MDK_Test_Core ${Test_FILES} ${Core_FILES})

qt5_use_modules(MDK_Test_Core Core GUI)

target_link_libraries(MDK_Test_Core ${VTK_LIBRARIES}) 