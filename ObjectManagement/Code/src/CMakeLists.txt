PROJECT(ObjectManagement)
 
include_directories(${CMAKE_SOURCE_DIR}/src)

add_library(MdkObjectManagement_LIB mdkDebugMarco.h mdkObject.h mdkObject.hpp mdkObjectManager.h mdkObjectManager.cpp mdkObjectBase.h mdkObjectBase.cpp mdkObjectBaseInfo.h mdkObjectBaseInfo.cpp)