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
        MDK_Error << "Couldn't open file to save result @ WritePairListAsJsonFile" << '\n';
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

    return JsonFile.rename(FilePathAndName + ".json");
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
        MDK_Error << "Unknown type of image @ LoadGrayScaleImageFromDataFile" << '\n';
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
        MDK_Error << "Couldn't open file to write image data @ SaveGrayScaleImageAsDataFile" << '\n';
        return false;
    }

    int64 L = Dimension.Lx * Dimension.Ly * Dimension.Lz;

    auto RawPointer = (char*)InputImage.GetVoxelPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();

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
        MDK_Error << "Unknown type of image @ SaveGrayScaleImageAsDataFile" << '\n';
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
        MDK_Error << "Couldn't open file to write image data @ SaveGrayScaleImageAsDataFile" << '\n';
        return false;
    }

    int64 L = Dimension.Lx * Dimension.Ly * Dimension.Lz;

    auto RawPointer = (char*)InputImage.GetVoxelPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();

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

    ScalarType ReferenceScalar = ScalarType(0);

    auto OutputScalarTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputScalarTypeName(OutputScalarTypeName_temp.c_str());

    int64 OutputByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.c_str());

    QFile HeaderFile(QFilePathAndName + ".json");

    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error << "Couldn't open HeaderFile." << '\n' ;
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
        MDK_Error << "Couldn't get ScalarType @ LoadScalarDenseMatrixFromDataFile" << '\n';
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
        MDK_Error << "Couldn't get RowNumber @ LoadScalarDenseMatrixFromDataFile" << '\n';
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
        MDK_Error << "Couldn't get ColNumber @ LoadScalarDenseMatrixFromDataFile" << '\n';
        return OutputMatrix;
    }
    
    //-------------------------------------------------- Read data ---------------------------------------------------------//

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error << "Couldn't open data file:" << FilePathAndName;
        return OutputMatrix;
    }

    if (OutputScalarTypeName == InputScalarTypeName)
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * OutputByteNumber)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        DataFile.read((char*)(OutputMatrix.GetElementPointer()), BypesofDataFile);

        return OutputMatrix;
    }

    //---------------------------------------

    MDK_Warning << "OutputScalarTypeName != InputScalarTypeName, Output may be wrong @ LoadScalarDenseMatrixFromDataFile" << '\n';

    if (InputScalarTypeName == "double")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber*8)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        double tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {            
            DataFile.read((char*)(&tempScalar), 8);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "float")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber*4)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        float tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {            
            DataFile.read((char*)(&tempScalar), 4);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "int8")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * 1)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        int8 tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {            
            DataFile.read((char*)(&tempScalar), 1);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "int16")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * 2)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        int16 tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {            
            DataFile.read((char*)(&tempScalar), 2);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "int32")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * 4)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        int32 tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {            
            DataFile.read((char*)(&tempScalar), 4);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "int64")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * 8)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        int64 tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {           
            DataFile.read((char*)(&tempScalar), 8);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "uint8")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * 1)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        uint8 tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {
            DataFile.read((char*)(&tempScalar), 1);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "uint16")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * 2)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        uint16 tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {            
            DataFile.read((char*)(&tempScalar), 2);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "uint32")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * 4)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        uint32 tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {
            DataFile.read((char*)(&tempScalar), 4);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else if (InputScalarTypeName == "uint64")
    {
        int64 BypesofDataFile = DataFile.size();

        if (BypesofDataFile != RowNumber*ColNumber * 8)
        {
            MDK_Error << "Data file size is not equal to matrix size @ LoadScalarDenseMatrixFromDataFile" << '\n';
            return OutputMatrix;
        }

        OutputMatrix.Resize(RowNumber, ColNumber);

        auto ElementPointer = OutputMatrix.GetElementPointer();

        uint64 tempScalar = 0;

        for (int64 i = 0; i < RowNumber*ColNumber; ++i)
        {
            DataFile.read((char*)(&tempScalar), 8);

            ElementPointer[i] = ScalarType(tempScalar);
        }
    }
    else
    {
        MDK_Error << "unknown ScalarType of data file @ LoadScalarDenseMatrixFromDataFile " << '\n';
    }

    return OutputMatrix;
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
        MDK_Error << "Unknown type of matrix @ SaveScalarDenseMatrixAsDataFile" << '\n';
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
        MDK_Error << "Couldn't open file to write matrix data @ SaveScalarDenseMatrixAsDataFile" << '\n';
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