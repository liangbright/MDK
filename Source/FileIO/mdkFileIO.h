#ifndef __mdkFileIO_h
#define __mdkFileIO_h

#include <string>


#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImportImageFilter.h"


#include <QString.h>
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonObject>

#include "mdkType.h"
#include "mdkDebugConfig.h"
#include "mdkString.h"


namespace mdk
{
//---------------------------------------------//
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class SparseMatrix;

template<typename ElementType>
class Image2D;

template<typename ElementType>
class Image3D;

template<typename ElementType>
class FeatureDictionaryForSparseCoding;

template<typename ElementType>
class FeatureDictionaryForDensenCoding;
//---------------------------------------------//


//------------------------------------- write data information as *.json file----------------------------------------------------//
struct NameValueQStringPair
{
    QString  Name;
    QString  Value;
};

bool WritePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName);

//------------------------------------- save/load DenseMatrix from Json data file --------------------------------------------------------//

template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ElementType>& InputMatrix, const CharString& FilePathAndName);

template<typename ElementType>
DenseMatrix<ElementType> LoadDenseMatrixFromJsonDataFile(const CharString& FilePathAndName);

template<typename OutputElementType>
void Internal_LoadDenseMatrixFromJsonDataFile(DenseMatrix<OutputElementType>& OutputMatrix, QFile& DataFile, const QString& InputElementTypeName);

template<typename OutputElementType, typename InputElementType>
void Internal_LoadDenseMatrixFromJsonDataFile(DenseMatrix<OutputElementType>& OutputMatrix,  QFile& DataFile, int_max ByteNumberOfInputElementType);

//------------------------------------- save/load SparseMatrix from Json data file -------------------------------------------------------//

//------------------------------------- save/load ScalarImage2D and ScalarImage3D from Json data file --------------------------------------------//

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const Image3D<PixelType>& InputImage, const CharString& FilePathAndName);

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromJsonDataFile(const CharString& FilePathAndName);

template<typename OutputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, const QString& InputElementTypeName);

template<typename OutputPixelType, typename InputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, int_max ByteNumberOfInputElementType);


//------------------------------------- load ScalarImage2D and ScalarImage3D from DICOM file --------------------------------------------//

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromDICOMSeries(const CharString& FilePath);

// -------------------------------------- convert mdk image to itk image--------------------------------------------------------------------//

//copy or share data
template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(const Image3D<PixelType>& InputImage, bool SharePixelData);

//copy data
template<typename PixelType>
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(const Image3D<PixelType>& InputImage);

// -------------------------------------- convert itk image to mdk image--------------------------------------------------------------------//

//copy data
template<typename PixelType>
Image3D<PixelType> ConvertITK3DScalarImageToMDK3DScalarImage(const itk::Image<PixelType, 3>* ITKImage);

//----------------------------------- save/load FeatureDictionary from Json data file -----------------------------------------------------//

template<typename ElementType>
bool SaveFeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const CharString& FilePathAndName);

template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> LoadFeatureDictionaryForSparseCoding(const CharString& FilePathAndName);


}

#include "mdkFileIO.hpp"

#endif