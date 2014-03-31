#ifndef __mdkFileIO_hpp
#define __mdkFileIO_hpp

//#include "mdkFileIO.h"

namespace mdk
{

bool WritePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName)
{    
    QFile::remove(FilePathAndName + "~temp~.json");

    QFile JsonFile(FilePathAndName + "~temp~.json");

    if (!JsonFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to save result @ WritePairListAsJsonFile(...)")
        return false;
    }

    QTextStream out(&JsonFile);

    uint64 s = PairList.size();

    out << "{\n";
    for (uint64 i = 0; i < s; ++i)
    {
        out << "\"" << PairList[i].Name << "\"" << " : " << "\"" << PairList[i].Value << "\"";

        if (i < s - 1)
        {
            out << "," << "\n";
        }
        else
        {
            out << "\n";
        }
    }
    out << "}\n";

    out.flush();

    QFile::remove(FilePathAndName + ".json");

    JsonFile.rename(FilePathAndName + ".json");

    JsonFile.close();

    return true;
}


template<typename ScalarType>
Image<ScalarType> LoadGrayScaleImageFromDICOMFile(const std::string& FilePathAndName)
{
    Image<ScalarType> OutputImage;

    auto reader = vtkSmartPointer<vtkDICOMImageReader>::New();

    reader->SetDirectoryName(FilePathAndName.c_str());

    reader->Update();

    auto vtkImage = reader->GetOutput();

    int dims[3];

    vtkImage->GetDimensions(dims);

    double Origin[3];

    vtkImage->GetOrigin(Origin);

    double Spacing[3];

    vtkImage->GetSpacing(Spacing);

    OutputImage.ReInitialize(dims[0],    dims[1],    dims[2],
                             Origin[0],  Origin[1],  Origin[2],
                             Spacing[0], Spacing[1], Spacing[2]);

    for (int z = 0; z < dims[2]; ++z)
    {
        for (int y = 0; y < dims[1]; ++y)
        {
            for (int x = 0; x < dims[0]; ++x)
            {
                OutputImage(x, y, z) = ScalarType(vtkImage->GetScalarComponentAsFloat(x, y, z, 0));
            }
        }
    }

    return OutputImage;
}


// note:
// do not include file type in FilePathAndName
// i.e., FilePathAndName = "C:/Data/ImageData", not  "C:/Data/ImageData.data" or "C:/Data/ImageData.json"
//
template<typename ScalarType>
bool LoadGrayScaleImageFromDataFile(const std::string& FilePathAndName, const Image<ScalarType>& InputImage)
{
    ScalarType ReferenceScalar = ScalarType(0);

    int64 ByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ LoadGrayScaleImageFromDataFile(...)")
        return false;
    }

    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);
    QString QScalarTypeName(ScalarTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(11);

    PairList[0].Name = "DataType";
    PairList[0].Value = "GrayScaleImage";

    PairList[1].Name = "ScalarType";
    PairList[1].Value = QScalarTypeName;

    auto Dimension = InputImage.GetImageDimension();

    PairList[2].Name = "Dimension_x";
    PairList[2].Value = QString::number(Dimension.Lx);

    PairList[3].Name = "Dimension_y";
    PairList[3].Value = QString::number(Dimension.Ly);

    PairList[4].Name = "Dimension_z";
    PairList[4].Value = QString::number(Dimension.Lz);

    auto Origin = InputImage.GetPhysicalOrigin();

    PairList[5].Name = "Origin_x";
    PairList[5].Value = QString::number(Origin.x);

    PairList[6].Name = "Origin_y";
    PairList[6].Value = QString::number(Origin.y);

    PairList[7].Name = "Origin_z";
    PairList[7].Value = QString::number(Origin.z);

    auto Spacing = InputImage.GetVoxelPhysicalSize();

    PairList[8].Name = "Spacing_x";
    PairList[8].Value = QString::number(Spacing.Sx);

    PairList[9].Name = "Spacing_y";
    PairList[9].Value = QString::number(Spacing.Sy);

    PairList[10].Name = "Spacing_z";
    PairList[10].Value = QString::number(Spacing.Sz);


    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.c_str());

    WritePairListAsJsonFile(PairList, QFilePathAndName);

    // write data file  --------------------------------------------------

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write image data @ SaveGrayScaleImageAsDataFile(...)")
        return false;
    }

    int64 L = Dimension.Lx * Dimension.Ly * Dimension.Lz;

    auto RawPointer = (char*)InputImage.GetVoxelPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();

    DataFile.close();

    return true;
}


// note:
// do not include file type in FilePathAndName
// i.e., FilePathAndName = "C:/Data/ImageData", not  "C:/Data/ImageData.data" or "C:/Data/ImageData.json"
//
template<typename ScalarType>
bool SaveGrayScaleImageAsDataFile(const std::string& FilePathAndName, const Image<ScalarType>& InputImage)
{
    ScalarType ReferenceScalar = ScalarType(0);

    int64 ByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ SaveGrayScaleImageAsDataFile(...)")
        return false;
    }

    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);
    QString QScalarTypeName(ScalarTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(11);

    PairList[0].Name = "DataType";
    PairList[0].Value = "GrayScaleImage";

    PairList[1].Name = "ScalarType";
    PairList[1].Value = QScalarTypeName;

    auto Dimension = InputImage.GetDimension();

    PairList[2].Name = "Dimension_x";
    PairList[2].Value = QString::number(Dimension.Lx);

    PairList[3].Name = "Dimension_y";
    PairList[3].Value = QString::number(Dimension.Ly);

    PairList[4].Name = "Dimension_z";
    PairList[4].Value = QString::number(Dimension.Lz);

    auto Origin = InputImage.GetPhysicalOrigin();

    PairList[5].Name = "Origin_x";
    PairList[5].Value = QString::number(Origin.x);

    PairList[6].Name = "Origin_y";
    PairList[6].Value = QString::number(Origin.y);

    PairList[7].Name = "Origin_z";
    PairList[7].Value = QString::number(Origin.z);

    auto Spacing = InputImage.GetVoxelSpacing();

    PairList[8].Name = "Spacing_x";
    PairList[8].Value = QString::number(Spacing.Sx);

    PairList[9].Name = "Spacing_y";
    PairList[9].Value = QString::number(Spacing.Sy);

    PairList[10].Name = "Spacing_z";
    PairList[10].Value = QString::number(Spacing.Sz);


    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.c_str());

    WritePairListAsJsonFile(PairList, QFilePathAndName);

    // write data file  --------------------------------------------------

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write image data @ SaveGrayScaleImageAsDataFile(...)")
        return false;
    }

    int64 L = Dimension.Lx * Dimension.Ly * Dimension.Lz;

    auto RawPointer = (char*)InputImage.GetVoxelPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();

    DataFile.close();

    return true;
}


// note:
// do not include file type in FilePathAndName
// i.e., FilePathAndName = "C:/Data/MatrixData", not  "C:/Data/MatrixData.data" or "C:/Data/MatrixData.json"
//
template<typename ScalarType>
DenseMatrix<ScalarType> LoadScalarDenseMatrixFromDataFile(const std::string& FilePathAndName)
{
    DenseMatrix<ScalarType> OutputMatrix;

    //----------------------------------------------------------

    auto ReferenceScalar = ScalarType(0);

    auto OutputScalarTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputScalarTypeName(OutputScalarTypeName_temp.c_str());

    int64 OutputByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.c_str());

    QFile HeaderFile(QFilePathAndName + ".json");

    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open Header File.")
        return OutputMatrix;
    }

    //----------------------------------------------------------//
    QByteArray HeaderContent = HeaderFile.readAll();
    QJsonDocument HeaderDoc(QJsonDocument::fromJson(HeaderContent));
    QJsonObject HeaderObject = HeaderDoc.object();
    //-----------------------------------------------------------//
   
    QString InputScalarTypeName = 0;

    auto it = HeaderObject.find("ScalarType");
    if (it != HeaderObject.end())
    {
        InputScalarTypeName = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get ScalarType @ LoadScalarDenseMatrixFromDataFile(...)")
        HeaderFile.close();
        return OutputMatrix;
    }

    //---------------------------------------------------

    int64 RowNumber = 0;

    it = HeaderObject.find("RowNumber");
    if (it != HeaderObject.end())
    {
        RowNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get RowNumber @ LoadScalarDenseMatrixFromDataFile(...)")
        HeaderFile.close();
        return OutputMatrix;
    }

    int64 ColNumber = 0;

    it = HeaderObject.find("ColNumber");
    if (it != HeaderObject.end())
    {
        ColNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get ColNumber @ LoadScalarDenseMatrixFromDataFile(...)")
        HeaderFile.close();
        return OutputMatrix;
    }
    
    HeaderFile.close();

    //-------------------------------------------------- Read data ---------------------------------------------------------//

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open data file:" << FilePathAndName)
        return OutputMatrix;
    }

    //----------------------------------------------------------------------------------------------

    if (OutputScalarTypeName == InputScalarTypeName)
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, ScalarType>(OutputMatrix, DataFile, RowNumber, ColNumber, OutputByteNumber);

        DataFile.close();

        return OutputMatrix;
    }

    MDK_Warning("OutputScalarTypeName != InputScalarTypeName, Output may be wrong @ LoadScalarDenseMatrixFromDataFile(...)")


    if (InputScalarTypeName == "double")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, double>(OutputMatrix, DataFile, RowNumber, ColNumber, 8);
    }
    else if (InputScalarTypeName == "float")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, float>(OutputMatrix, DataFile, RowNumber, ColNumber, 4);
    }
    else if (InputScalarTypeName == "int8")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, int8>(OutputMatrix, DataFile, RowNumber, ColNumber, 1);
    }
    else if (InputScalarTypeName == "int16")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, int16>(OutputMatrix, DataFile, RowNumber, ColNumber, 2);
    }
    else if (InputScalarTypeName == "int32")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, int32>(OutputMatrix, DataFile, RowNumber, ColNumber, 4);
    }
    else if (InputScalarTypeName == "int64")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, int64>(OutputMatrix, DataFile, RowNumber, ColNumber, 8);
    }
    else if (InputScalarTypeName == "uint8")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, uint8>(OutputMatrix, DataFile, RowNumber, ColNumber, 1);
    }
    else if (InputScalarTypeName == "uint16")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, uint16>(OutputMatrix, DataFile, RowNumber, ColNumber, 2);
    }
    else if (InputScalarTypeName == "uint32")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, uint32>(OutputMatrix, DataFile, RowNumber, ColNumber, 4);
    }
    else if (InputScalarTypeName == "uint64")
    {
        Internal_LoadScalarDenseMatrixFromDataFile<ScalarType, uint64>(OutputMatrix, DataFile, RowNumber, ColNumber, 8);
    }
    else
    {
        MDK_Error("unknown ScalarType of data file @ LoadScalarDenseMatrixFromDataFile(...) ")
    }

    DataFile.close();

    return OutputMatrix;
}


template<typename OutputScalarType, typename InputScalarType>
void Internal_LoadScalarDenseMatrixFromDataFile(DenseMatrix<OutputScalarType>& OutputMatrix, QFile& DataFile, int64 RowNumber, int64 ColNumber, int64 BytesOfInputScalarType)
{
    int64 BypesofDataFile = DataFile.size();

    if (BypesofDataFile != RowNumber*ColNumber * BytesOfInputScalarType)
    {
        MDK_Error("Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile(...)")
        return;
    }

    OutputMatrix.FastResize(RowNumber, ColNumber);

    auto ElementPointer = OutputMatrix.GetElementPointer();

    auto tempScalar = InputScalarType(0);

    for (int64 i = 0; i < RowNumber*ColNumber; ++i)
    {
        DataFile.read((char*)(&tempScalar), BytesOfInputScalarType);

        ElementPointer[i] = OutputScalarType(tempScalar);
    }
}

// note:
// do not include file type in FilePathAndName
// i.e., FilePathAndName = "C:/Data/MatrixData", not  "C:/Data/MatrixData.data" or "C:/Data/MatrixData.json"
//
template<typename ScalarType>
bool SaveScalarDenseMatrixAsDataFile(const std::string& FilePathAndName, const DenseMatrix<ScalarType>& InputMatrix)
{
    ScalarType tempScalar = ScalarType(0);

    int64 ByteNumber = CalByteNumberOfScalar(tempScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of matrix @ SaveScalarDenseMatrixAsDataFile(...)")
        return false;
    }

    //----------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(4);

    auto Size = InputMatrix.GetSize();

    PairList[0].Name = "DataType";
    PairList[0].Value = "ScalarDenseMatrix";

    auto ScalarTypeName = FindScalarTypeName(tempScalar);
    QString QScalarTypeName(ScalarTypeName.c_str());

    PairList[1].Name = "ScalarType";
    PairList[1].Value = QScalarTypeName;

    PairList[2].Name = "RowNumber";
    PairList[2].Value = QString::number(Size.RowNumber);

    PairList[3].Name = "ColNumber";
    PairList[3].Value = QString::number(Size.ColNumber);

    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.c_str());

    WritePairListAsJsonFile(PairList, QFilePathAndName);

    // write data file  --------------------------------------------------

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write matrix data @ SaveScalarDenseMatrixAsDataFile(...)")
        return false;
    }

    int64 L = Size.ColNumber*Size.RowNumber;

    auto RawPointer = (char*)InputMatrix.GetElementPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();

    return true;
}


}//namespace mdk

#endif