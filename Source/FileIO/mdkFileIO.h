#ifndef __mdkFileIO_h
#define __mdkFileIO_h

#include <string>


#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>


#include <QString.h>
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonObject>


#include "mdkType.h"
#include "mdkImage.h"
#include "mdkDenseMatrix.h"


namespace mdk
{

struct NameValueQStringPair
{
    QString  Name;
    QString  Value;
};

bool WritePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName);

template<typename ScalarType>
Image<ScalarType> LoadGrayScaleImageFromDICOMFile(const std::string& FilePathAndName);

template<typename ScalarType>
Image<ScalarType> LoadGrayScaleImageFromDataFile(const std::string& FilePathAndName);

template<typename ScalarType>
bool SaveGrayScaleImageAsDataFile(const std::string& FilePathAndName, const Image<ScalarType>& InputImage);

template<typename ScalarType>
DenseMatrix<ScalarType> LoadScalarDenseMatrixFromDataFile(const std::string& FilePathAndName);

template<typename OutputScalarType, typename InputScalarType>
void Internal_LoadScalarDenseMatrixFromDataFile(DenseMatrix<OutputScalarType>& OutputMatrix, QFile& DataFile, int64 RowNumber, int64 ColNumber, int64 BytesOfInputScalarType);

template<typename ScalarType>
bool SaveScalarDenseMatrixAsDataFile(const std::string& FilePathAndName, const DenseMatrix<ScalarType>& InputMatrix);

}

#include "mdkFileIO.hpp"

#endif