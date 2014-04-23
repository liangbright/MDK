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

    uint_max s = PairList.size();

    out << "{\n";
    for (uint_max i = 0; i < s; ++i)
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


// note:
// do not include file type in FilePathAndName
// i.e., FilePathAndName = "C:/Data/MatrixData", not  "C:/Data/MatrixData.data" or "C:/Data/MatrixData.json"
//
template<typename ScalarType>
DenseMatrix<ScalarType> LoadScalarDenseMatrixFromJsonDataFile(const std::string& FilePathAndName)
{
    DenseMatrix<ScalarType> OutputMatrix;

    //----------------------------------------------------------

    auto ReferenceScalar = ScalarType(0);

    auto OutputScalarTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputScalarTypeName(OutputScalarTypeName_temp.c_str());

    int_max OutputByteNumber = CalByteNumberOfScalar(ReferenceScalar);

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
        MDK_Error("Couldn't get ScalarType @ LoadScalarDenseMatrixFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMatrix;
    }

    //---------------------------------------------------

    int_max RowNumber = 0;

    it = HeaderObject.find("RowNumber");
    if (it != HeaderObject.end())
    {
        RowNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get RowNumber @ LoadScalarDenseMatrixFromJsonDataFile(...)")
            HeaderFile.close();
        return OutputMatrix;
    }

    int_max ColNumber = 0;

    it = HeaderObject.find("ColNumber");
    if (it != HeaderObject.end())
    {
        ColNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get ColNumber @ LoadScalarDenseMatrixFromJsonDataFile(...)")
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
        Internal_LoadScalarDenseMatrixFromJsonDataFile<ScalarType, ScalarType>(OutputMatrix, DataFile, RowNumber, ColNumber, OutputByteNumber);

        DataFile.close();

        return OutputMatrix;
    }

    MDK_Warning("OutputScalarTypeName != InputScalarTypeName, Output may be inaccurate @ LoadScalarDenseMatrixFromJsonDataFile(...)")

    Internal_LoadScalarDenseMatrixFromJsonDataFile(OutputMatrix, DataFile, RowNumber, ColNumber, InputScalarTypeName);

    DataFile.close();

    return OutputMatrix;
}


template<typename OutputScalarType>
void Internal_LoadScalarDenseMatrixFromJsonDataFile(DenseMatrix<OutputScalarType>& OutputMatrix,
                                                    QFile& DataFile, int_max RowNumber, int_max ColNumber,
                                                    const std::string& InputScalarTypeName)
{
    if (InputScalarTypeName == "double")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, double>(OutputMatrix, DataFile, RowNumber, ColNumber, 8);
    }
    else if (InputScalarTypeName == "float")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, float>(OutputMatrix, DataFile, RowNumber, ColNumber, 4);
    }
    else if (InputScalarTypeName == "int8")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, int8>(OutputMatrix, DataFile, RowNumber, ColNumber, 1);
    }
    else if (InputScalarTypeName == "int16")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, int16>(OutputMatrix, DataFile, RowNumber, ColNumber, 2);
    }
    else if (InputScalarTypeName == "int32")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, int32>(OutputMatrix, DataFile, RowNumber, ColNumber, 4);
    }
    else if (InputScalarTypeName == "int64")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, int64>(OutputMatrix, DataFile, RowNumber, ColNumber, 8);
    }
    else if (InputScalarTypeName == "uint8")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, uint8>(OutputMatrix, DataFile, RowNumber, ColNumber, 1);
    }
    else if (InputScalarTypeName == "uint16")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, uint16>(OutputMatrix, DataFile, RowNumber, ColNumber, 2);
    }
    else if (InputScalarTypeName == "uint32")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, uint32>(OutputMatrix, DataFile, RowNumber, ColNumber, 4);
    }
    else if (InputScalarTypeName == "uint64")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, uint64>(OutputMatrix, DataFile, RowNumber, ColNumber, 8);
    }
    else
    {
        MDK_Error("unknown ScalarType of data file @ Internal_LoadScalarDenseMatrixFromJsonDataFile(...) ")
    }
}


template<typename OutputScalarType, typename InputScalarType>
void Internal_LoadScalarDenseMatrixFromJsonDataFile(DenseMatrix<OutputScalarType>& OutputMatrix, 
                                                    QFile& DataFile, int_max RowNumber, int_max ColNumber, 
                                                    int_max BytesOfInputScalarType)
{
    int_max BypesofDataFile = DataFile.size();

    if (BypesofDataFile != RowNumber*ColNumber * BytesOfInputScalarType)
    {
        MDK_Error("Data file size is not equal to matrix size @ Internal_LoadScalarDenseMatrixFromJsonDataFile(...)")
        return;
    }

    OutputMatrix.FastResize(RowNumber, ColNumber);

    auto ElementPointer = OutputMatrix.GetElementPointer();

    auto tempScalar = InputScalarType(0);

    for (int_max i = 0; i < RowNumber*ColNumber; ++i)
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
bool SaveScalarDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const std::string& FilePathAndName)
{
    ScalarType tempScalar = ScalarType(0);

    int_max ByteNumber = CalByteNumberOfScalar(tempScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of matrix @ SaveScalarDenseMatrixAsJsonDataFile(...)")
            return false;
    }

    //----------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(4);

    auto Size = InputMatrix.GetSize();

    PairList[0].Name = "MatrixType";
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
        MDK_Error("Couldn't open file to write matrix data @ SaveScalarDenseMatrixAsJsonDataFile(...)")
            return false;
    }

    int_max L = Size.ColNumber*Size.RowNumber;

    auto RawPointer = (char*)InputMatrix.GetElementPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();

    return true;
}


template<typename ScalarType>
Image3D<ScalarType> LoadGrayScale3DImageFromDICOMSeries(const std::string& FilePath)
{
    Image3D<ScalarType> OutputImage;


    typedef itk::Image< ScalarType, 3 >               ITKImageType;

    typedef itk::ImageSeriesReader< ITKImageType >    ITKImageReaderType;

    typedef itk::GDCMImageIO                          ImageIOType;
    typedef itk::GDCMSeriesFileNames                  InputNamesGeneratorType;

    ImageIOType::Pointer gdcmIO = ImageIOType::New();
    InputNamesGeneratorType::Pointer inputNames = InputNamesGeneratorType::New();
    inputNames->SetInputDirectory(FilePath);

    const ITKImageReaderType::FileNamesContainer & filenames = inputNames->GetInputFileNames();

    ITKImageReaderType::Pointer ITKImageReader = ITKImageReaderType::New();

    ITKImageReader->SetImageIO(gdcmIO);
    ITKImageReader->SetFileNames(filenames);

    try
    {
        ITKImageReader->Update();
    }
    catch (itk::ExceptionObject &excp)
    {
        std::cerr << "Exception thrown while reading the dicom series @ LoadGrayScale3DImageFromDICOMSeries(...)" << std::endl;
        std::cerr << excp << std::endl;
        return OutputImage;
    }

    ITKImageType::Pointer ITKImage = ITKImageReader->GetOutput();

    auto Size = ITKImage->GetBufferedRegion().GetSize();
    auto Spacing = ITKImage->GetSpacing();
    auto Origin = ITKImage->GetOrigin();

    OutputImage.SetSize(Size[0], Size[1], Size[2]);
    OutputImage.SetPixelSpacing(Spacing[0], Spacing[1], Spacing[2]);
    OutputImage.SetPhysicalOrigin(Origin[0], Origin[1], Origin[2]);

    auto RawPointerOfITKImage = ITKImage->GetBufferPointer();

    auto RawPointerOfOutputImage = OutputImage.GetPixelPointer();

    for (int_max i = 0; i < OutputImage.GetPixelNumber(); ++i)
    {
        RawPointerOfOutputImage[i] = RawPointerOfITKImage[i];
    }
    
    return OutputImage;
}


// note:
// do not include file type in FilePathAndName
// i.e., FilePathAndName = "C:/Data/ImageData", not  "C:/Data/ImageData.data" or "C:/Data/ImageData.json"
//
template<typename ScalarType>
Image3D<ScalarType> LoadGrayScale3DImageFromJsonDataFile(const std::string& FilePathAndName)
{
}


// note:
// do not include file type in FilePathAndName
// i.e., FilePathAndName = "C:/Data/ImageData", not  "C:/Data/ImageData.data" or "C:/Data/ImageData.json"
//
template<typename ScalarType>
bool SaveGrayScale3DImageAsJsonDataFile(const Image3D<ScalarType>& InputImage, const std::string& FilePathAndName)
{
    ScalarType ReferenceScalar = ScalarType(0);

    int_max ByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ SaveGrayScale3DImageAsJsonDataFile(...)")
        return false;
    }

    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);
    QString QScalarTypeName(ScalarTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(11);

    PairList[0].Name = "ImageType";
    PairList[0].Value = "GrayScaleImage";

    PairList[1].Name = "PixelType";
    PairList[1].Value = QScalarTypeName;

    auto Size = InputImage.GetSize();

    PairList[2].Name = "Size_x";
    PairList[2].Value = QString::number(Size.Lx);

    PairList[3].Name = "Size_y";
    PairList[3].Value = QString::number(Size.Ly);

    PairList[4].Name = "Size_z";
    PairList[4].Value = QString::number(Size.Lz);

    auto Spacing = InputImage.GetPixelSpacing();

    PairList[5].Name = "Spacing_x";
    PairList[5].Value = QString::number(Spacing.Sx);

    PairList[6].Name = "Spacing_y";
    PairList[6].Value = QString::number(Spacing.Sy);

    PairList[7].Name = "Spacing_z";
    PairList[7].Value = QString::number(Spacing.Sz);

    auto Origin = InputImage.GetPhysicalOrigin();

    PairList[8].Name = "Origin_x";
    PairList[8].Value = QString::number(Origin.x);

    PairList[9].Name = "Origin_y";
    PairList[9].Value = QString::number(Origin.y);

    PairList[10].Name = "Origin_z";
    PairList[10].Value = QString::number(Origin.z);

    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.c_str());

    WritePairListAsJsonFile(PairList, QFilePathAndName);

    // write data file  --------------------------------------------------

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write image data @ SaveGrayScale3DImageAsJsonDataFile(...)")
        return false;
    }

    int_max L = Size.Lx * Size.Ly * Size.Lz;

    auto RawPointer = (char*)InputImage.GetPixelPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();
    DataFile.close();

    return true;
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType> LoadFeatureDictionaryForSparseCoding(const std::string& FilePathAndName)
{
    FeatureDictionaryForSparseCoding<ScalarType> Dictionary;

    //----------------------------------------------------------

    auto ReferenceScalar = ScalarType(0);

    auto OutputScalarTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputScalarTypeName(OutputScalarTypeName_temp.c_str());

    int_max OutputByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.c_str());

    QFile HeaderFile(QFilePathAndName + ".json");

    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open header file:" << FilePathAndName)
        return Dictionary;
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
        MDK_Error("Couldn't get ScalarType @ LoadFeatureDictionaryForSparseCoding(...)")
        HeaderFile.close();
        return Dictionary;
    }

    //---------------------------------------------------

    it = HeaderObject.find("Name");
    if (it != HeaderObject.end())
    {
        Dictionary.m_Name = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get Name @ LoadFeatureDictionaryForSparseCoding(...)")
        HeaderFile.close();
        return Dictionary;
    }

    //---------------------------------------------------

    int_max RowNumber = 0;

    it = HeaderObject.find("RowNumber");
    if (it != HeaderObject.end())
    {
        RowNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get RowNumber @ LoadFeatureDictionaryForSparseCoding(...)")
        HeaderFile.close();
        return Dictionary;
    }

    int_max ColNumber = 0;

    it = HeaderObject.find("ColNumber");
    if (it != HeaderObject.end())
    {
        ColNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get ColNumber @ LoadFeatureDictionaryForSparseCoding(...)")
        HeaderFile.close();
        return Dictionary;
    }

    HeaderFile.close();

    //-------------------------------------------------- Read data ---------------------------------------------------------//

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open data file:" << FilePathAndName)
        return Dictionary;
    }

    //----------------------------------------------------------------------------------------------

    if (OutputScalarTypeName == InputScalarTypeName)
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<ScalarType, ScalarType>(Dictionary.m_BasisMatrix, DataFile, RowNumber, ColNumber, OutputByteNumber);
        Internal_LoadScalarDenseMatrixFromJsonDataFile<ScalarType, ScalarType>(Dictionary.m_ReconstructionStd, DataFile, 1, ColNumber, OutputByteNumber);

        DataFile.close();

        return Dictionary;
    }

    MDK_Warning("OutputScalarTypeName != InputScalarTypeName, Output may be inaccurate @ LoadFeatureDictionaryForSparseCoding(...)")

    Internal_LoadScalarDenseMatrixFromJsonDataFile(Dictionary.m_BasisMatrix, DataFile, RowNumber, ColNumber, InputScalarTypeName.toStdString());
    Internal_LoadScalarDenseMatrixFromJsonDataFile(Dictionary.m_ReconstructionStd, DataFile, 1, ColNumber, InputScalarTypeName.toStdString());

    DataFile.close();

    return Dictionary;
}


template<typename ScalarType>
bool SaveFeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, const std::string& FilePathAndName)
{
    ScalarType ReferenceScalar = ScalarType(0);

    int_max ByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ SaveFeatureDictionaryForSparseCoding(...)")
        return false;
    }

    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);
    QString QScalarTypeName(ScalarTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(5);

    PairList[0].Name = "DictionaryType";
    PairList[0].Value = "FeatureDictionaryForSparseCoding";

    PairList[1].Name = "Name";
    PairList[1].Value = Dictionary.GetName().c_str();

    PairList[2].Name = "ScalarType";
    PairList[2].Value = QScalarTypeName;

    auto Size = Dictionary.GetSize();

    PairList[3].Name = "RowNumber";
    PairList[3].Value = QString::number(Size.RowNumber);

    PairList[4].Name = "ColNumber";
    PairList[4].Value = QString::number(Size.ColNumber);

    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.c_str());

    WritePairListAsJsonFile(PairList, QFilePathAndName);

    // write data file  --------------------------------------------------

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write data @ SaveFeatureDictionaryForSparseCoding(...)")
            return false;
    }

    // step 1 : write BasisMatrix
    int_max L = Size.RowNumber * Size.ColNumber;
    auto RawPointer = (char*)Dictionary.BasisMatrix().GetElementPointer();
    DataFile.write(RawPointer, L*ByteNumber);

    // step 2 : write ReconstructionStd
    L = Dictionary.m_ReconstructionStd.GetElementNumber();
    RawPointer = (char*)Dictionary.m_ReconstructionStd.GetElementPointer();
    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();
    DataFile.close();

    return true;
}


}//namespace mdk

#endif