#ifndef __mdkFileIO_PolygonMesh_h
#define __mdkFileIO_PolygonMesh_h

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO_Common.h"
#include "mdkVTKDataStructureConversion.h"

namespace mdk
{

// forward declare ---------------------
template<typename MeshAttributeType>
class PolygonMesh;
//--------------------------------------

// save/load PolygonMesh from Json data file or vtk file

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName);

template<typename MeshAttributeType>
bool LoadPolygonMeshFromJsonDataFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& JsonFilePathAndName);

template<typename MeshAttributeType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName);

template<typename MeshAttributeType>
bool LoadPolygonMeshMeshFromVTKFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName);

}//namespace mdk

#include "mdkFileIO_PolygonMesh.hpp"

#endif