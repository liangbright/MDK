#pragma once

#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkNumericSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImportImageFilter.h>
#include <itkImageDuplicator.h>

#include "mdkFileIO.h"
#include "mdkITKDataStructureConversion.h"

namespace mdk
{
// forward declare ---------------
template<typename PixelType>
class DenseImage3D;
//---------------------------------

// can not Save/Load DenseImage or SparseImage use the same function
// unless use std::enable_if to conditionally compile a member function
//template<typename ImageType>
//ImageType Load3DScalarImageFromJsonDataFile(const String& JsonFilePathAndName);

// save/load Scalar 3D Image from Json data file
// for example: JsonFilePathAndName = "C:/Data/SomeImage.json"
// header is saved in SomeImage.json
// data is saved in SomeImage.json.data

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const DenseImage3D<PixelType>& InputImage, const String& FilePathAndName);

template<typename PixelType>
bool Load3DScalarImageFromJsonDataFile(DenseImage3D<PixelType>& OutputImage, const String& FilePathAndName);

template<typename PixelType>
bool Load3DScalarImageFromDICOMSeries(DenseImage3D<PixelType>& OutputImage, const String& FilePath);

template<typename PixelType>
bool Load3DScalarImageFromSingleFile(DenseImage3D<PixelType>& OutputImage, const String& FilePathAndName);

}// namespace mdk

#include "mdkDenseImage3D_FileIO.hpp"
