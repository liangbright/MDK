#ifndef __mdkFileIO_hpp
#define __mdkFileIO_hpp

//#include "mdkFileIO.h"

namespace mdk
{

//For example: FilePathAndName is "C:/Data/something.json"
//
bool WritePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName)
{    
    QFile::remove(FilePathAndName + "~temp~");

    QFile JsonFile(FilePathAndName + "~temp~");

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

    QFile::remove(FilePathAndName);

    JsonFile.rename(FilePathAndName);

    JsonFile.close();

    return true;
}


// for example: FilePathAndName = "C:/Data/SomeMatrix.json"
// header is saved in SomeMatrix.json
// data is saved in SomeMatrix.json.data
//
template<typename ScalarType>
bool SaveScalarDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const CharString& FilePathAndName)
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
    QString QScalarTypeName(ScalarTypeName.StdString().c_str());

    PairList[1].Name = "ScalarType";
    PairList[1].Value = QScalarTypeName;

    PairList[2].Name = "RowNumber";
    PairList[2].Value = QString::number(Size.RowNumber);

    PairList[3].Name = "ColNumber";
    PairList[3].Value = QString::number(Size.ColNumber);

    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

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
DenseMatrix<ScalarType> LoadScalarDenseMatrixFromJsonDataFile(const CharString& FilePathAndName)
{
    DenseMatrix<ScalarType> OutputMatrix;

    //----------------------------------------------------------

    auto ReferenceScalar = ScalarType(0);

    auto OutputScalarTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputScalarTypeName(OutputScalarTypeName_temp.c_str());

    int_max ByteNumberOfOutputScalarType = CalByteNumberOfScalar(ReferenceScalar);

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    QFile HeaderFile(QFilePathAndName);

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

    OutputMatrix.FastResize(RowNumber, ColNumber);

    if (OutputScalarTypeName == InputScalarTypeName)
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<ScalarType, ScalarType>(OutputMatrix, DataFile, ByteNumberOfOutputScalarType);
    }
    else
    {
        MDK_Warning("OutputScalarTypeName != InputScalarTypeName, Output may be inaccurate @ LoadScalarDenseMatrixFromJsonDataFile(...)")

        Internal_LoadScalarDenseMatrixFromJsonDataFile(OutputMatrix, DataFile, InputScalarTypeName);
    }

    DataFile.close();

    return OutputMatrix;
}


template<typename OutputScalarType>
void Internal_LoadScalarDenseMatrixFromJsonDataFile(DenseMatrix<OutputScalarType>& OutputMatrix, QFile& DataFile, const QString& InputScalarTypeName)
{
    if (InputScalarTypeName == "double")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, double>(OutputMatrix, DataFile, 8);
    }
    else if (InputScalarTypeName == "float")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, float>(OutputMatrix, DataFile, 4);
    }
    else if (InputScalarTypeName == "int8")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, int8>(OutputMatrix, DataFile, 1);
    }
    else if (InputScalarTypeName == "int16")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, int16>(OutputMatrix, DataFile, 2);
    }
    else if (InputScalarTypeName == "int32")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, int32>(OutputMatrix, DataFile, 4);
    }
    else if (InputScalarTypeName == "int64")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, int64>(OutputMatrix, DataFile, 8);
    }
    else if (InputScalarTypeName == "uint8")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, uint8>(OutputMatrix, DataFile, 1);
    }
    else if (InputScalarTypeName == "uint16")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, uint16>(OutputMatrix, DataFile, 2);
    }
    else if (InputScalarTypeName == "uint32")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, uint32>(OutputMatrix, DataFile, 4);
    }
    else if (InputScalarTypeName == "uint64")
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<OutputScalarType, uint64>(OutputMatrix, DataFile, 8);
    }
    else
    {
        MDK_Error("unknown ScalarType of data file @ Internal_LoadScalarDenseMatrixFromJsonDataFile(...) ")
    }
}


template<typename OutputScalarType, typename InputScalarType>
void Internal_LoadScalarDenseMatrixFromJsonDataFile(DenseMatrix<OutputScalarType>& OutputMatrix, QFile& DataFile, int_max ByteNumberOfInputScalarType)
{
    int_max BypesofDataFile = DataFile.size();

    int_max ElementNumber = OutputMatrix.GetElementNumber();

    if (BypesofDataFile != ElementNumber * ByteNumberOfInputScalarType)
    {
        MDK_Error("Data file size is not equal to matrix size @ Internal_LoadScalarDenseMatrixFromJsonDataFile(...)")
        return;
    }

    auto ElementPointer = OutputMatrix.GetElementPointer();

    auto tempScalar = InputScalarType(0);

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        DataFile.read((char*)(&tempScalar), ByteNumberOfInputScalarType);

        ElementPointer[i] = OutputScalarType(tempScalar);
    }
}



// for example: FilePathAndName = "C:/Data/SomeImage.json"
// header is saved in SomeImage.json
// data is saved in SomeImage.json.data
//
template<typename ScalarType>
bool SaveGrayScale3DImageAsJsonDataFile(const Image3D<ScalarType>& InputImage, const CharString& FilePathAndName)
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

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

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
Image3D<ScalarType> LoadGrayScale3DImageFromJsonDataFile(const CharString& FilePathAndName)
{
    Image3D<ScalarType> OutputImage;

    //----------------------------------------------------------

    auto ReferenceScalar = ScalarType(0);

    auto OutputScalarTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputScalarTypeName(OutputScalarTypeName_temp.c_str());

    int_max ByteNumberOfOutputScalarType = CalByteNumberOfScalar(ReferenceScalar);

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    QFile HeaderFile(QFilePathAndName);

    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open Header File @ LoadGrayScale3DImageFromJsonDataFile(...)")
        return OutputImage;
    }

    //----------------------------------------------------------//
    QByteArray HeaderContent = HeaderFile.readAll();
    QJsonDocument HeaderDoc(QJsonDocument::fromJson(HeaderContent));
    QJsonObject HeaderObject = HeaderDoc.object();
    //-----------------------------------------------------------//

    QString InputScalarTypeName = 0;

    auto it = HeaderObject.find("PixelType");
    if (it != HeaderObject.end())
    {
        InputScalarTypeName = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get PixelType @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    //---------------------------------------------------

    Image3DSize Size;

    it = HeaderObject.find("Size_x");
    if (it != HeaderObject.end())
    {
        Size.Lx = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get Size_x @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    it = HeaderObject.find("Size_y");
    if (it != HeaderObject.end())
    {
        Size.Ly = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get Size_y @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    it = HeaderObject.find("Size_z");
    if (it != HeaderObject.end())
    {
        Size.Lz = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get Size_z @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }


    Image3DPixelSpacing Spacing;

    it = HeaderObject.find("Spacing_x");
    if (it != HeaderObject.end())
    {
        Spacing.Sx = it.value().toString().toDouble();
    }
    else
    {
        MDK_Error("Couldn't get Spacing_x @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    it = HeaderObject.find("Spacing_y");
    if (it != HeaderObject.end())
    {
        Spacing.Sy = it.value().toString().toDouble();
    }
    else
    {
        MDK_Error("Couldn't get Spacing_y @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    it = HeaderObject.find("Spacing_z");
    if (it != HeaderObject.end())
    {
        Spacing.Sz = it.value().toString().toDouble();
    }
    else
    {
        MDK_Error("Couldn't get Spacing_z @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }


    Image3DPhysicalOrigin Origin;

    it = HeaderObject.find("Origin_x");
    if (it != HeaderObject.end())
    {
        Origin.x = it.value().toString().toDouble();
    }
    else
    {
        MDK_Error("Couldn't get Origin_x @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    it = HeaderObject.find("Origin_y");
    if (it != HeaderObject.end())
    {
        Origin.y = it.value().toString().toDouble();
    }
    else
    {
        MDK_Error("Couldn't get Origin_y @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    it = HeaderObject.find("Origin_z");
    if (it != HeaderObject.end())
    {
        Origin.z = it.value().toString().toDouble();
    }
    else
    {
        MDK_Error("Couldn't get Origin_z @ LoadGrayScale3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    HeaderFile.close();

    //-------------------------------------------------- Read data ---------------------------------------------------------//

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open data file:" << FilePathAndName)
        return OutputImage;
    }

    // allocate memory
    OutputImage.SetSize(Size);
    OutputImage.SetPixelSpacing(Spacing);
    OutputImage.SetPhysicalOrigin(Origin);

    //----------------------------------------------------------------------------------------------

    if (OutputScalarTypeName == InputScalarTypeName)
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<ScalarType, ScalarType>(OutputImage, DataFile, ByteNumberOfOutputScalarType);

        DataFile.close();

        return OutputImage;
    }

    MDK_Warning("OutputScalarTypeName != InputScalarTypeName, Output may be inaccurate @ LoadGrayScale3DImageFromJsonDataFile(...)")

    Internal_LoadGrayScale3DImageFromJsonDataFile(OutputImage, DataFile, InputScalarTypeName);

    DataFile.close();

    return OutputImage;
}


template<typename OutputScalarType>
void Internal_LoadGrayScale3DImageFromJsonDataFile(Image3D<OutputScalarType>& OutputImage, QFile& DataFile, const QString& InputScalarTypeName)
{
    if (InputScalarTypeName == "double")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, double>(OutputImage, DataFile, 8);
    }
    else if (InputScalarTypeName == "float")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, float>(OutputImage, DataFile, 4);
    }
    else if (InputScalarTypeName == "int8")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, int8>(OutputImage, DataFile, 1);
    }
    else if (InputScalarTypeName == "int16")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, int16>(OutputImage, DataFile, 2);
    }
    else if (InputScalarTypeName == "int32")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, int32>(OutputImage, DataFile, 4);
    }
    else if (InputScalarTypeName == "int64")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, int64>(OutputImage, DataFile, 8);
    }
    else if (InputScalarTypeName == "uint8")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, uint8>(OutputImage, DataFile, 1);
    }
    else if (InputScalarTypeName == "uint16")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, uint16>(OutputImage, DataFile, 2);
    }
    else if (InputScalarTypeName == "uint32")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, uint32>(OutputImage, DataFile, 4);
    }
    else if (InputScalarTypeName == "uint64")
    {
        Internal_LoadGrayScale3DImageFromJsonDataFile<OutputScalarType, uint64>(OutputImage, DataFile, 8);
    }
    else
    {
        MDK_Error("unknown ScalarType of data file @ Internal_LoadGrayScale3DImageFromJsonDataFile(...) ")
    }
}


template<typename OutputScalarType, typename InputScalarType>
void Internal_LoadGrayScale3DImageFromJsonDataFile(Image3D<OutputScalarType>& OutputImage, QFile& DataFile, int_max ByteNumberOfInputScalarType)
{
    int_max BypesofDataFile = DataFile.size();

    auto PixelNumber = OutputImage.GetPixelNumber();

    if (BypesofDataFile != PixelNumber * ByteNumberOfInputScalarType)
    {
        MDK_Error("Data file size is not equal to matrix size @ Internal_LoadGrayScale3DImageFromJsonDataFile(...)")
        return;
    }

    auto PixelPointer = OutputImage.GetPixelPointer();

    auto tempScalar = InputScalarType(0);

    for (int_max i = 0; i < PixelNumber; ++i)
    {
        DataFile.read((char*)(&tempScalar), ByteNumberOfInputScalarType);

        PixelPointer[i] = OutputScalarType(tempScalar);
    }
}


template<typename ScalarType>
Image3D<ScalarType> LoadGrayScale3DImageFromDICOMSeries(const CharString& FilePath)
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

    OutputImage = ConvertITK3DImageToMDK3DImage(ITKImageReader->GetOutput());

    return OutputImage;
}


template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DImageToITK3DImage(Image3D<PixelType>& InputImage, bool SharePixelData)
{
    auto InputSize = InputImage.GetSize();
    auto InputOrigin = InputImage.GetPhysicalOrigin();
    auto InputSpacing = InputImage.GetPixelSpacing();
    auto InputPixelNumber = InputImage.GetPixelNumber();

    typedef itk::Image<PixelType, 3> ITKImageType;
    typedef itk::ImportImageFilter<PixelType, 3> ITKImportFilterType;

    ITKImportFilterType::Pointer importFilter = ITKImportFilterType::New();

    ITKImportFilterType::SizeType size;
    size[0] = InputSize.Lx;
    size[1] = InputSize.Ly;
    size[2] = InputSize.Lz;

    ITKImportFilterType::IndexType start;
    start.Fill(0);
    ITKImportFilterType::RegionType region;
    region.SetIndex(start);
    region.SetSize(size);
    importFilter->SetRegion(region);

    const itk::SpacePrecisionType origin[3] = { InputOrigin.x, InputOrigin.y, InputOrigin.z };
    importFilter->SetOrigin(origin);

    const itk::SpacePrecisionType spacing[3] = { InputSpacing.Sx, InputSpacing.Sy, InputSpacing.Sz };
    importFilter->SetSpacing(spacing);

    // Orientation ???

    if (SharePixelData == true)
    {       
        bool ITKImageWillOwnPixelDataArray = false;
        importFilter->SetImportPointer(InputImage.GetPixelPointer(), InputPixelNumber, ITKImageWillOwnPixelDataArray);
    }
    else
    {
        PixelType * localBuffer = new PixelType[InputPixelNumber];
        for (int_max i = 0; i < InputPixelNumber; ++i)
        {
            localBuffer[i] = InputImage[i];
        }

        bool ITKImageWillOwnPixelDataArray = true;
        importFilter->SetImportPointer(localBuffer, InputPixelNumber, true);
    }

    importFilter->Update();

    return importFilter;
}


template<typename PixelType>
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK3DImageToITK3DImage(Image3D<PixelType>& InputImage)
{
    bool SharePixelData = false;
    auto importFilter = ConvertMDK3DImageToITK3DImage(InputImage, SharePixelData);

    itk::SmartPointer<itk::Image<PixelType, 3>> ITKImage = importFilter->GetOutput();

    return ITKImage;
}


template<typename PixelType>
Image3D<PixelType> ConvertITK3DImageToMDK3DImage(const itk::Image<PixelType, 3>* ITKImage)
{
    Image3D<PixelType> OutputImage;

    if (ITKImage == nullptr)
    {
        MDK_Error("Invalid input @ ConvertITK3DImageToMDK3DImage(...)")
        return OutputImage;
    }

    auto Size = ITKImage->GetBufferedRegion().GetSize();
    auto Spacing = ITKImage->GetSpacing();
    auto Origin = ITKImage->GetOrigin();

    OutputImage.SetSize(Size[0], Size[1], Size[2]);
    OutputImage.SetPixelSpacing(Spacing[0], Spacing[1], Spacing[2]);
    OutputImage.SetPhysicalOrigin(Origin[0], Origin[1], Origin[2]);

    auto RawPointerOfITKImage = ITKImage->GetBufferPointer();

    for (int_max i = 0; i < OutputImage.GetPixelNumber(); ++i)
    {
        OutputImage[i] = RawPointerOfITKImage[i];
    }

    return OutputImage;
}


// for example: FilePathAndName = "C:/Data/SomeDictionary.json"
//
template<typename ScalarType>
bool SaveFeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, const CharString& FilePathAndName)
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
    PairList[1].Value = Dictionary.Name().StdString().c_str();

    PairList[2].Name = "ScalarType";
    PairList[2].Value = QScalarTypeName;

    auto Size = Dictionary.GetSize();

    PairList[3].Name = "RowNumber";
    PairList[3].Value = QString::number(Size.RowNumber);

    PairList[4].Name = "ColNumber";
    PairList[4].Value = QString::number(Size.ColNumber);

    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

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
    L = Dictionary.ReconstructionStd().GetElementNumber();
    RawPointer = (char*)Dictionary.ReconstructionStd().GetElementPointer();
    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();
    DataFile.close();

    return true;
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType> LoadFeatureDictionaryForSparseCoding(const CharString& FilePathAndName)
{
    FeatureDictionaryForSparseCoding<ScalarType> Dictionary;

    //----------------------------------------------------------

    auto ReferenceScalar = ScalarType(0);

    auto OutputScalarTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputScalarTypeName(OutputScalarTypeName_temp.c_str());

    int_max OutputByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    QFile HeaderFile(QFilePathAndName);

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

    std::string Name;

    it = HeaderObject.find("Name");
    if (it != HeaderObject.end())
    {
         Name = it.value().toString().toStdString();
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

    Dictionary.Name() = Name;
    Dictionary.BasisMatrix().FastResize(RowNumber, ColNumber);
    Dictionary.ReconstructionStd().FastResize(1, ColNumber);

    if (OutputScalarTypeName == InputScalarTypeName)
    {
        Internal_LoadScalarDenseMatrixFromJsonDataFile<ScalarType, ScalarType>(Dictionary.BasisMatrix(), DataFile, OutputByteNumber);
        Internal_LoadScalarDenseMatrixFromJsonDataFile<ScalarType, ScalarType>(Dictionary.ReconstructionStd(), DataFile, OutputByteNumber);
    }
    else
    {
        MDK_Warning("OutputScalarTypeName != InputScalarTypeName, Output may be inaccurate @ LoadFeatureDictionaryForSparseCoding(...)")

        Internal_LoadScalarDenseMatrixFromJsonDataFile(Dictionary.BasisMatrix(), DataFile, InputScalarTypeName);
        Internal_LoadScalarDenseMatrixFromJsonDataFile(Dictionary.ReconstructionStd(), DataFile, InputScalarTypeName);
    }

    DataFile.close();

    return Dictionary;
}


}//namespace mdk

#endif