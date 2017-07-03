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
class PolyhedronMesh;
//--------------------------------------

template<typename ScalarType>
bool SavePolyhedronMeshAsJsonDataFile(const PolyhedronMesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadPolyhedronMeshFromJsonDataFile(PolyhedronMesh<ScalarType>& OutputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool SavePolyhedronMeshAsVTKFile(const PolyhedronMesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadPolyhedronMeshFromVTKFile(PolyhedronMesh<ScalarType>& OutputMesh, const String& FilePathAndName);

}//namespace mdk

#include "mdkPolyhedronMesh_FileIO.hpp"
