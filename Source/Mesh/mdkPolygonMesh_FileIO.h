#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO.h"
#include "mdkVTKDataObject_FileIO.h"

namespace mdk
{

// forward declare ---------------------
template<typename MeshAttributeType>
class PolygonMesh;
//--------------------------------------

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName);

template<typename MeshAttributeType>
bool LoadPolygonMeshFromJsonDataFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName);

template<typename MeshAttributeType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName);

template<typename MeshAttributeType>
bool LoadPolygonMeshMeshFromVTKFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName);

template<typename MeshAttributeType>
bool SavePolygonMeshAsPLYFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName);

template<typename MeshAttributeType>
bool LoadPolygonMeshMeshFromPLYFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName);

}//namespace mdk

#include "mdkPolygonMesh_FileIO.hpp"
