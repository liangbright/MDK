#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO.h"
#include "mdkVTKDataObject_FileIO.h"
#include "mdkVTKDataStructureConversion.h"

namespace mdk
{

// forward declare ---------------------
template<typename ScalarType>
class PolygonMesh;
//--------------------------------------

template<typename ScalarType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadPolygonMeshFromJsonDataFile(PolygonMesh<ScalarType>& OutputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadPolygonMeshMeshFromVTKFile(PolygonMesh<ScalarType>& OutputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool SavePolygonMeshAsPLYFile(const PolygonMesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadPolygonMeshMeshFromPLYFile(PolygonMesh<ScalarType>& OutputMesh, const String& FilePathAndName);

}//namespace mdk

#include "mdkPolygonMesh_FileIO.hpp"
