#ifndef __mdkFileIO_DenseImage3D_h
#define __mdkFileIO_DenseImage3D_h

#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkNumericSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImportImageFilter.h>
#include <itkImageDuplicator.h>

#include "mdkDataStructureConversion.h"
#include "mdkFileIO_Json.h"

namespace mdk
{
// forward declare ---------------
template<typename PixelType>
class DenseImage3D;
//---------------------------------

// save/load Scalar 3D Image from Json data file
// for example: JsonFilePathAndName = "C:/Data/SomeImage.json"
// header is saved in SomeImage.json
// data is saved in SomeImage.json.data

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const DenseImage3D<PixelType>& InputImage, const std::string& JsonFilePathAndName);

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile_Header(const DenseImage3D<PixelType>& InputImage, const std::string& JsonFilePathAndName);

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile_Data(const DenseImage3D<PixelType>& InputImage, const std::string& DataFilePathAndName);

// can not Load DenseImage or SparseImage use the same function
// unless use std::enable_if to conditionally compile a member function
//template<typename ImageType>
//ImageType Load3DScalarImageFromJsonDataFile(const std::string& JsonFilePathAndName);

template<typename PixelType>
bool Load3DScalarImageFromJsonDataFile(DenseImage3D<PixelType>& OutputImage, const std::string& JsonFilePathAndName);

template<typename OutputPixelType>
bool Load3DScalarImageFromJsonDataFile_Data(DenseImage3D<OutputPixelType>& OutputImage, const std::string& InputPixelTypeName, const std::string& DataFilePathAndName);;

template<typename OutputPixelType, typename InputPixelType>
bool Load3DScalarImageFromJsonDataFile_Data(DenseImage3D<OutputPixelType>& OutputImage, const std::string& DataFilePathAndName);

template<typename PixelType>
bool Load3DScalarImageFromDICOMSeries(DenseImage3D<PixelType>& OutputImage, const std::string& FilePath);

template<typename PixelType>
bool Load3DScalarImageFromSingleDICOMFile(DenseImage3D<PixelType>& OutputImage, const std::string& FilePathAndName);

}// namespace mdk

#include "mdkFileIO_DenseImage3D.hpp"

#endif