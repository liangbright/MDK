#ifndef __mdkFileIO_h
#define __mdkFileIO_h

#include <string>


#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkNumericSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImportImageFilter.h>
#include <itkImageDuplicator.h>


#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>


#include <QString.h>
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonObject>


#include "mdkType.h"
#include "mdkDebugConfig.h"
#include "mdkString.h"
#include "mdkDataStructureConversion.h"


namespace mdk
{
//------------- forward declare-------------------//
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class SparseMatrix;

template<typename PixelType>
class Image2D;

template<typename PixelType>
class Image3D;

template<typename ScalarType>
class TriangleMesh;

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
void Internal_LoadDenseMatrixFromJsonDataFile(DenseMatrix<OutputElementType>& OutputMatrix, QFile& DataFile, const std::string& InputElementTypeName);

template<typename OutputElementType, typename InputElementType>
void Internal_LoadDenseMatrixFromJsonDataFile(DenseMatrix<OutputElementType>& OutputMatrix,  QFile& DataFile, int_max ByteNumberOfInputElementType);

//------------------------------------- save/load SparseMatrix from Json data file -------------------------------------------------------//

//------------------------------------- save/load ScalarImage2D and ScalarImage3D from Json data file --------------------------------------------//

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const Image3D<PixelType>& InputImage, const CharString& FilePathAndName);

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromJsonDataFile(const CharString& FilePathAndName);

template<typename OutputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, const std::string& InputPixelTypeName);

template<typename OutputPixelType, typename InputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, int_max ByteNumberOfInputPixelType);

//------------------------------------- load ScalarImage2D and ScalarImage3D from DICOM file --------------------------------------------//

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromDICOMSeries(const CharString& FilePath);

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromSingleDICOMFile(const CharString& FilePathAndName);

//------------------------------------- save/load TriangleMesh from Json data file --------------------------------------------------------//

template<typename ScalarType>
bool SaveTriangleMeshAsJsonDataFile(const TriangleMesh<ScalarType>& InputMesh, const CharString& FilePathAndName);

template<typename ScalarType = double>
TriangleMesh<ScalarType> LoadTriangleMeshFromJsonDataFile(const CharString& FilePathAndName, bool Flag_BuildLinkAndAdjacency = true);

template<typename ScalarType>
bool SaveTriangleMeshAsVTKFile(const TriangleMesh<ScalarType>& InputMesh, const CharString& FilePathAndName);

template<typename ScalarType = double>
TriangleMesh<ScalarType> LoadTriangleMeshFromVTKFile(const CharString& FilePathAndName, bool Flag_BuildLinkAndAdjacency = true);

//----------------------------------- save/load FeatureDictionary from Json data file -----------------------------------------------------//

template<typename ElementType>
bool SaveFeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const CharString& FilePathAndName);

template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> LoadFeatureDictionaryForSparseCoding(const CharString& FilePathAndName);


}

#include "mdkFileIO.hpp"

#endif