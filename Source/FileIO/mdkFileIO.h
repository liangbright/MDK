#ifndef __mdkFileIO_h
#define __mdkFileIO_h

#include <string>


#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSeriesReader.h"


#include <QString.h>
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonObject>


#include "mdkImage3D.h"
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
Image3D<ScalarType> LoadGrayScale3DImageFromDICOMSeries(const std::string& FilePath);

template<typename ScalarType>
Image3D<ScalarType> LoadGrayScale3DImageFromJsonDataFile(const std::string& FilePathAndName);

template<typename ScalarType>
bool SaveGrayScale3DImageAsJsonDataFile(const Image3D<ScalarType>& InputImage, const std::string& FilePathAndName);

template<typename ScalarType>
DenseMatrix<ScalarType> LoadScalarDenseMatrixFromJsonDataFile(const std::string& FilePathAndName);

template<typename OutputScalarType, typename InputScalarType>
void Internal_LoadScalarDenseMatrixFromJsonDataFile(DenseMatrix<OutputScalarType>& OutputMatrix, QFile& DataFile, int_max RowNumber, int_max ColNumber, int_max BytesOfInputScalarType);

template<typename ScalarType>
bool SaveScalarDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const std::string& FilePathAndName);

}

#include "mdkFileIO.hpp"

#endif