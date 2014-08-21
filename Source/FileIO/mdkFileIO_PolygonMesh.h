#ifndef __mdkFileIO_PolygonMesh_h
#define __mdkFileIO_PolygonMesh_h

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO_Json.h"

namespace mdk
{

// forward declare ---------------------
template<typename MeshAttributeType>
class PolygonMesh;
//--------------------------------------

// save/load PolygonMesh from Json data file or vtk file

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& JsonFilePathAndName);

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile_Header(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& JsonFilePathAndName);

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile_Data(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& DataFilePathAndName);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> LoadPolygonMeshFromJsonDataFile(const std::string& JsonFilePathAndName);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> LoadPolygonMeshFromJsonDataFile_Data(const std::string& DataFilePathAndName,
																	int_max PointNumber, int_max CellNumber,
																    const std::string& ScalarTypeName);

template<typename MeshAttributeType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& FilePathAndName);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> LoadPolygonMeshMeshFromVTKFile(const std::string& FilePathAndName);

}//namespace mdk

#include "mdkFileIO_PolygonMesh.hpp"

#endif