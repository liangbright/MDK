#ifndef __mdkFileIO_h
#define __mdkFileIO_h

#include <string>
#include <tuple>
#include <utility>

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

template<typename MeshAttributeType>
class TriangleMesh;

template<typename MeshAttributeType>
class PolygonMesh;

template<typename ScalaryType>
class FeatureDictionaryForSparseCoding;

template<typename ScalaryType>
class FeatureDictionaryForDensenCoding;
//---------------------------------------------//


//------------------------------------- write data information as *.json file----------------------------------------------------//
struct NameValueQStringPair
{
    QString  Name;
    QString  Value;
};

bool SaveNameValuePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName, bool Flag_Append = false);

struct NameValueStdStringPair
{
	std::string  Name;
	std::string  Value;
};

bool SaveNameValuePairListAsJsonFile(const std::vector<NameValueStdStringPair>& PairList, const std::string& FilePathAndName, bool Flag_Append = false);

//------------------------------------- save/load DenseMatrix from Json data file --------------------------------------------------------//

template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ElementType>& InputMatrix, const std::string& JsonFilePathAndName);

template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile_Header(const DenseMatrix<ElementType>& InputMatrix, const std::string& JsonFilePathAndName);

template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile_Data(const DenseMatrix<ElementType>& InputMatrix, const std::string& DataFilePathAndName);

template<typename ElementType>
DenseMatrix<ElementType> LoadDenseMatrixFromJsonDataFile(const std::string& JsonFilePathAndName);

// HeaderInfo: <MatrixSize, InputElementTypeName>
template<typename ElementType>
std::pair<MatrixSize, std::string> LoadDenseMatrixFromJsonDataFile_Header(const std::string& JsonFilePathAndName);

template<typename OutputElementType, typename InputElementType>
DenseMatrix<OutputElementType> LoadDenseMatrixFromJsonDataFile_Data(MatrixSize OutputMatrixSize, const std::string& DataFilePathAndName);

//------------------------------------- save/load SparseMatrix from Json data file -------------------------------------------------------//

//------------------------------------- save/load ScalarImage2D and ScalarImage3D from Json data file --------------------------------------------//

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const Image3D<PixelType>& InputImage, const std::string& FilePathAndName);

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromJsonDataFile(const std::string& FilePathAndName);

template<typename OutputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, const std::string& InputPixelTypeName);

template<typename OutputPixelType, typename InputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, int_max ByteNumberOfInputPixelType);

//------------------------------------- load ScalarImage2D and ScalarImage3D from DICOM file --------------------------------------------//

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromDICOMSeries(const std::string& FilePath);

template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromSingleDICOMFile(const std::string& FilePathAndName);

//------------------------------------- save/load TriangleMesh from Json data file or vtk file -------------------------------------------//

template<typename MeshAttributeType>
bool SaveTriangleMeshAsJsonDataFile(const TriangleMesh<MeshAttributeType>& InputMesh, const std::string& JsonFilePathAndName);

// no header, only data file
template<typename MeshAttributeType>
bool SaveTriangleMeshAsJsonDataFile_Data(const TriangleMesh<MeshAttributeType>& InputMesh, const std::string& DataFilePathAndName);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> LoadTriangleMeshFromJsonDataFile(const std::string& JsonFilePathAndName);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> LoadTriangleMeshFromJsonDataFile_Data(const std::string& DataFilePathAndName);

template<typename MeshAttributeType>
bool SaveTriangleMeshAsVTKFile(const TriangleMesh<MeshAttributeType>& InputMesh, const std::string& FilePathAndName);

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> LoadTriangleMeshFromVTKFile(const std::string& FilePathAndName);

//------------------------------------- save/load PolygonMesh from Json data file or vtk file -------------------------------------------//

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& FilePathAndName);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> LoadPolygonMeshFromJsonDataFile(const std::string& FilePathAndName);

template<typename MeshAttributeType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& FilePathAndName);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> LoadPolygonMeshMeshFromVTKFile(const std::string& FilePathAndName);

//----------------------------------- save/load FeatureDictionary from Json data file -----------------------------------------------------//

template<typename ElementType>
bool SaveFeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const std::string& FilePathAndName);

template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> LoadFeatureDictionaryForSparseCoding(const std::string& FilePathAndName);


}

#include "mdkFileIO.hpp"

#endif