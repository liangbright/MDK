#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkImageData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkPLYReader.h>
#include <vtkPLYWriter.h>

#include "mdkString.h"
#include "mdkFileIO.h"
//#include "mdkTypeInference.h"

#include "mdkVTKDataStructureConversion.h"

namespace mdk
{
bool SaveVTKPolyDataAsVTKFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName);//ASCII VTK file
vtkSmartPointer<vtkPolyData> LoadVTKPolyDataFromVTKFile(const String& FilePathAndName);

bool SaveVTKUnstructuredGridAsVTKFile(vtkUnstructuredGrid* OutputVTKMesh, const String& FilePathAndName);//ASCII VTK file
vtkSmartPointer<vtkUnstructuredGrid> LoadVTKUnstructuredGridFromVTKFile(const String& FilePathAndName);

bool SaveVTKPolyDataAsPLYFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName);//ASCII PLY file
vtkSmartPointer<vtkPolyData> LoadVTKPolyDataFromPLYFile(const String& FilePathAndName);

//directly load from file, not converted from mdk::PolygonMesh
//bool LoadVTKPolygonMeshFromJsonDataFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName);

//directly load from file, not converted from mdk::DenseImage3D
//bool LoadVTK3DScalarImageFromJsonDataFile(vtkImageData* OutputVTKImage, VTKDataTypeEnum OutputPixelType, const String& FilePathAndName);

}// namespace mdk
