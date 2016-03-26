#pragma once

#include "mdkPolygonMesh_FileIO.h"

namespace mdk
{
// forward declare -------------------
template<typename MeshAttributeType>
class TriangleMesh;
//-----------------------------------

//-----------------------------  only save point and face -------------------------------------------------------
template<typename MeshAttributeType>
bool SaveTriangleMeshAsJsonDataFile(const TriangleMesh<MeshAttributeType>& InputMesh, const String& JsonFilePathAndName);

template<typename MeshAttributeType>
bool LoadTriangleMeshFromJsonDataFile(TriangleMesh<MeshAttributeType>& OutputMesh, const String& JsonFilePathAndName);

template<typename MeshAttributeType>
bool SaveTriangleMeshAsVTKFile(const TriangleMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName);

template<typename MeshAttributeType>
bool LoadTriangleMeshFromVTKFile(TriangleMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName);

}//namespace mdk

#include "mdkTriangleMesh_FileIO.hpp"
