#pragma once

#include "mdkFileIO.h"
#include "mdkVTKDataObject_FileIO.h"
#include "mdkVTKDataStructureConversion.h"

namespace mdk
{

// forward declare ---------------------
template<typename ScalarType>
class Mesh;
//--------------------------------------

template<typename ScalarType>
bool SaveMeshAsJsonDataFile(const Mesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadMeshFromJsonDataFile(Mesh<ScalarType>& OutputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool SaveMeshAsVTKFile(const Mesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadMeshMeshFromVTKFile(Mesh<ScalarType>& OutputMesh, const String& FilePathAndName);

}//namespace mdk

#include "mdkMesh_FileIO.hpp"
