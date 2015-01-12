#ifndef mdk_VTKDataObject_FileIO_h
#define mdk_VTKDataObject_FileIO_h

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkImageData.h>

#include "mdkString.h"
#include "mdkFileIO.h"
//#include "mdkTypeInference.h"

#include "mdkVTKDataStructureConversion.h"

namespace mdk
{

bool LoadVTKPolygonMeshFromJsonDataFile(vtkPolyData& OutputVTKMesh, const String& FilePathAndName);
bool LoadVTKPolygonMeshFromJsonDataFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName);

bool LoadVTK3DScalarImageFromJsonDataFile(vtkImageData& OutputVTKImage, VTKDataTypeEnum OutputPixelType, const String& FilePathAndName);
bool LoadVTK3DScalarImageFromJsonDataFile(vtkImageData* OutputVTKImage, VTKDataTypeEnum OutputPixelType, const String& FilePathAndName);
}// namespace mdk

#endif