﻿#pragma once

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
class DenseImage2D;
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
bool Save2DScalarImageAsJsonDataFile(const DenseImage2D<PixelType>& InputImage, const String& FilePathAndName);

template<typename PixelType>
bool Load2DScalarImageFromJsonDataFile(DenseImage2D<PixelType>& OutputImage, const String& FilePathAndName);

template<typename PixelType>
bool Load2DScalarImageFromSingleDICOMFile(DenseImage2D<PixelType>& OutputImage, const String& FilePathAndName);

}// namespace mdk

#include "mdkDenseImage2D_FileIO.hpp"
