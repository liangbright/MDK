#ifndef __mdkFileIO_Image3D_h
#define __mdkFileIO_Image3D_h

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
class Image3D;
//---------------------------------

// save/load Scalar 3D Image from Json data file
// for example: JsonFilePathAndName = "C:/Data/SomeImage.json"
// header is saved in SomeImage.json
// data is saved in SomeImage.json.data

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const Image3D<PixelType>& InputImage, const std::string& JsonFilePathAndName);

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile_Header(const Image3D<PixelType>& InputImage, const std::string& JsonFilePathAndName);

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile_Data(const Image3D<PixelType>& InputImage, const std::string& DataFilePathAndName);

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromJsonDataFile(const std::string& JsonFilePathAndName);

template<typename OutputPixelType>
void Load3DScalarImageFromJsonDataFile_Data(Image3D<OutputPixelType>& OutputImage, const std::string& InputPixelTypeName, const std::string& DataFilePathAndName);;

template<typename OutputPixelType, typename InputPixelType>
void Load3DScalarImageFromJsonDataFile_Data(Image3D<OutputPixelType>& OutputImage, const std::string& DataFilePathAndName);

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromDICOMSeries(const std::string& FilePath);

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromSingleDICOMFile(const std::string& FilePathAndName);

}// namespace mdk

#include "mdkFileIO_Image3D.hpp"

#endif