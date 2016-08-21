#pragma once

#include "mdkTextFile.h"
#include "mdkVTKDataObject_FileIO.h"

namespace mdk
{

//------------------------------------ solid element mesh to VTKUnstructuredGrid ------------------------------------------
template<typename ScalarType>
vtkSmartPointer<vtkUnstructuredGrid> ConvertFiniteElementMeshToVTKUnstructuredGrid(const FiniteElementMesh<ScalarType>& InputMesh);

template<typename ScalarType>
bool ConvertFiniteElementMeshToVTKUnstructuredGrid(const FiniteElementMesh<ScalarType>& InputMesh, vtkUnstructuredGrid* VTKMesh);

template<typename ScalarType>
bool ConvertVTKUnstructuredGridToFiniteElementMesh(vtkUnstructuredGrid* VTKMesh, FiniteElementMesh<ScalarType>& OutputMesh);

//--------------------------------------------- shell element to VTKPolyData ----------------------------------------------
template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertFiniteElementMeshToVTKPolyData(const FiniteElementMesh<ScalarType>& InputMesh);

//---------------------------------------------------------------------------------------------------------------------------

template<typename ScalarType>
bool SaveFiniteElementMeshAsVTKFile(const FiniteElementMesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadFiniteElementMeshFromVTKFile(FiniteElementMesh<ScalarType>& OutputMesh, const String& FilePathAndName);

//---------------------------------------------------------------------------------------------------------------------------

template<typename ScalarType>
bool SaveFiniteElementMeshAsJsonDataFile(const FiniteElementMesh<ScalarType>& InputMesh, const String& FilePathAndName);

template<typename ScalarType>
bool LoadFiniteElementMeshFromJsonDataFile(FiniteElementMesh<ScalarType>& OutputMesh, const String& FilePathAndName);

//---------------------------------------------------------------------------------------------------------------------------
template<typename ScalarType>
bool SaveFiniteElementMeshAsAbaqusINPFile(const FiniteElementMesh<ScalarType>& InputMesh, const String& FilePathAndName);

}

#include "mdkFiniteElementMesh_FileIO.hpp"
