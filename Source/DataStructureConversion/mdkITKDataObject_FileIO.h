#ifndef mdk_ITKDataObject_FileIO_h
#define mdk_ITKDataObject_FileIO_h

#include <itkImage.h>

#include "mdkType.h"
#include "mdkString.h"
#include "mdkFileIO.h"

namespace mdk
{

template<typename PixelType>
bool LoadITK3DScalarImageFromJsonDataFile(itk::Image<PixelType, 3>& OutputITKImage, const String& FilePathAndName);

template<typename PixelType>
bool LoadITK3DScalarImageFromJsonDataFile(itk::Image<PixelType, 3>* OutputITKImage, const String& FilePathAndName);

}// namespace mdk

#include "mdkITKDataObject_FileIO.hpp"

#endif