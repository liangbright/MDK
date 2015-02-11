#ifndef mdk_TriangleMesh_FileIO_h
#define mdk_TriangleMesh_FileIO_h

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO.h"
#include "mdkVTKDataStructureConversion.h"

namespace mdk
{
// forward declare -------------------
template<typename MeshAttributeType>
class TriangleMesh;
//-----------------------------------

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

#endif