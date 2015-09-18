#ifndef mdk_VTKDataObject_FileIO_h
#define mdk_VTKDataObject_FileIO_h

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkImageData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include "mdkString.h"
#include "mdkFileIO.h"
//#include "mdkTypeInference.h"

#include "mdkVTKDataStructureConversion.h"

namespace mdk
{
bool SaveVTKPolyDataAsVTKFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName);//ASCII VTK file

vtkSmartPointer<vtkPolyData> LoadVTKPolyDataFromVTKFile(const String& FilePathAndName);

bool LoadVTKPolyDataFromVTKFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName);

//directly load from file, not converted from mdk::PolygonMesh
//bool LoadVTKPolygonMeshFromJsonDataFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName);

//directly load from file, not converted from mdk::DenseImage3D
//bool LoadVTK3DScalarImageFromJsonDataFile(vtkImageData* OutputVTKImage, VTKDataTypeEnum OutputPixelType, const String& FilePathAndName);

}// namespace mdk

#endif