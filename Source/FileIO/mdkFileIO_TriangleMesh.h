#ifndef __mdkFileIO_TriangleMesh_h
#define __mdkFileIO_TriangleMesh_h

#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkFileIO_Common.h"
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

#include "mdkFileIO_TriangleMesh.hpp"

#endif