#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO.h"
#include "mdkVTKDataObject_FileIO.h"

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

template<typename ScalarType>
bool SaveMeshAsPLYFile(const Mesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadMeshMeshFromPLYFile(Mesh<ScalarType>& OutputMesh, const String& FilePathAndName);

}//namespace mdk

#include "mdkMesh_FileIO.hpp"
