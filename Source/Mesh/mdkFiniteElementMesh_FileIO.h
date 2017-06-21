#pragma once

#include "mdkTextFile.h"
#include "mdkVTKDataObject_FileIO.h"

namespace mdk
{

//------------------------------------ solid element mesh to VTKUnstructuredGrid ------------------------------------------
template<typename ScalarType>
vtkSmartPointer<vtkUnstructuredGrid> ConvertMDKFiniteElementMeshToVTKUnstructuredGrid(const FiniteElementMesh<ScalarType>& InputMesh);

template<typename ScalarType>
bool ConvertVTKUnstructuredGridToMDKFiniteElementMesh(vtkUnstructuredGrid* VTKMesh, FiniteElementMesh<ScalarType>& OutputMesh);

//----------------------------------- shell/membrane element mesh to VTKPolyData --------------------------------------------
template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMDKFiniteElementMeshToVTKPolyData(const FiniteElementMesh<ScalarType>& InputMesh);

template<typename ScalarType>
bool ConvertVTKPolyDataToMDKFiniteElementMesh(vtkPolyData* VTKMesh, FiniteElementMesh<ScalarType>& OutputMesh);

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
