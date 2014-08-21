#ifndef __mdkFileIO_TriangleMesh_h
#define __mdkFileIO_TriangleMesh_h

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO_Json.h"

namespace mdk
{
// forward declare -------------------
template<typename MeshAttributeType>
class TriangleMesh;
//-----------------------------------

template<typename MeshAttributeType>
bool SaveTriangleMeshAsJsonDataFile(const TriangleMesh<MeshAttributeType>& InputMesh, const std::string& JsonFilePathAndName);

template<typename MeshAttributeType>
bool SaveTriangleMeshAsJsonDataFile_Header(const TriangleMesh<MeshAttributeType>& InputMesh, const std::string& JsonFilePathAndName);

template<typename MeshAttributeType>
bool SaveTriangleMeshAsJsonDataFile_Data(const TriangleMesh<MeshAttributeType>& InputMesh, const std::string& DataFilePathAndName);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> LoadTriangleMeshFromJsonDataFile(const std::string& JsonFilePathAndName);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> LoadTriangleMeshFromJsonDataFile_Data(const std::string& DataFilePathAndName, 
																	  int_max PointNumber, int_max CellNumber,
																	  const std::string& ScalarTypeName);

template<typename MeshAttributeType>
bool SaveTriangleMeshAsVTKFile(const TriangleMesh<MeshAttributeType>& InputMesh, const std::string& FilePathAndName);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> LoadTriangleMeshFromVTKFile(const std::string& FilePathAndName);

}//namespace mdk

#include "mdkFileIO_TriangleMesh.hpp"

#endif