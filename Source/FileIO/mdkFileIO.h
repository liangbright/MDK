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
template<typename ScalarType>
class DenseMatrix;

template<typename ScalarType>
class SparseMatrix;

template<typename ScalarType>
class Image2D;

template<typename ScalarType>
class Image3D;

template<typename ScalarType>
class FeatureDictionaryForSparseCoding;

template<typename ScalarType>
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

template<typename ScalarType>
bool SaveScalarDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const CharString& FilePathAndName);

template<typename ScalarType>
DenseMatrix<ScalarType> LoadScalarDenseMatrixFromJsonDataFile(const CharString& FilePathAndName);

template<typename OutputScalarType>
void Internal_LoadScalarDenseMatrixFromJsonDataFile(DenseMatrix<OutputScalarType>& OutputMatrix, QFile& DataFile, const QString& InputScalarTypeName);

template<typename OutputScalarType, typename InputScalarType>
void Internal_LoadScalarDenseMatrixFromJsonDataFile(DenseMatrix<OutputScalarType>& OutputMatrix,  QFile& DataFile, int_max ByteNumberOfInputScalarType);

//------------------------------------- save/load SparseMatrix from Json data file -------------------------------------------------------//

//------------------------------------- save/load Image2D and Image3D from Json data file --------------------------------------------//

template<typename ScalarType>
bool SaveGrayScale3DImageAsJsonDataFile(const Image3D<ScalarType>& InputImage, const CharString& FilePathAndName);

template<typename ScalarType>
Image3D<ScalarType> LoadGrayScale3DImageFromJsonDataFile(const CharString& FilePathAndName);

template<typename OutputScalarType>
void Internal_LoadGrayScale3DImageFromJsonDataFile(Image3D<OutputScalarType>& OutputImage, QFile& DataFile, const QString& InputScalarTypeName);

template<typename OutputScalarType, typename InputScalarType>
void Internal_LoadGrayScale3DImageFromJsonDataFile(Image3D<OutputScalarType>& OutputImage, QFile& DataFile, int_max ByteNumberOfInputScalarType);


//------------------------------------- load Image2D and Image3D from DICOM file --------------------------------------------//

template<typename ScalarType>
Image3D<ScalarType> LoadGrayScale3DImageFromDICOMSeries(const CharString& FilePath);


// -------------------------------------- convert mdk image to itk image--------------------------------------------------------------------//

//copy or share data
template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DImageToITK3DImage(const Image3D<PixelType>& InputImage, bool SharePixelData);

//copy data
template<typename PixelType>
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK3DImageToITK3DImage(const Image3D<PixelType>& InputImage);

// -------------------------------------- convert itk image to mdk image--------------------------------------------------------------------//
//copy data
template<typename PixelType>
Image3D<PixelType> ConvertITK3DImageToMDK3DImage(const itk::Image<PixelType, 3>* ITKImage);

//----------------------------------- save/load FeatureDictionary from Json data file -----------------------------------------------------//

template<typename ScalarType>
bool SaveFeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, const CharString& FilePathAndName);

template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType> LoadFeatureDictionaryForSparseCoding(const CharString& FilePathAndName);


}

#include "mdkFileIO.hpp"

#endif