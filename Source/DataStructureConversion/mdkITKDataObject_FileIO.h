#pragma once

#include <itkImage.h>

#include "mdkTypeInference.h"
#include "mdkString.h"
#include "mdkFileIO.h"
#include "mdkDenseImage3D_FileIO.h"

namespace mdk
{
template<typename PixelType>
bool SaveITK3DScalarImageAsJsonDataFile(itk::Image<PixelType, 3>* OutputITKImage, const String& FilePathAndName);

// directly load from file, not converted from mdk::DenseImage3D
//template<typename PixelType>
//bool LoadITK3DScalarImageFromJsonDataFile(itk::Image<PixelType, 3>* OutputITKImage, const String& FilePathAndName);

}// namespace mdk

#include "mdkITKDataObject_FileIO.hpp"
