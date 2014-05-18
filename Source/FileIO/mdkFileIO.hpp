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

    auto s = int_max(PairList.size());

    out << "{\n";
    for (int_max i = 0; i < s; ++i)
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
template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ElementType>& InputMatrix, const CharString& FilePathAndName)
{
    ElementType tempScalar = ElementType(0);

    int_max ByteNumber = CalByteNumberOfScalar(tempScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of matrix @ SaveDenseMatrixAsJsonDataFile(...)")
        return false;
    }

    //----------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(4);

    auto Size = InputMatrix.GetSize();

    PairList[0].Name = "MatrixType";
    PairList[0].Value = "DenseMatrix";

    auto ElementTypeName = FindScalarTypeName(tempScalar);
    QString QElementTypeName(ElementTypeName.c_str());

    PairList[1].Name = "ElementType";
    PairList[1].Value = QElementTypeName;

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
        MDK_Error("Couldn't open file to write matrix data @ SaveDenseMatrixAsJsonDataFile(...)")
        return false;
    }

    int_max L = Size.ColNumber*Size.RowNumber;

    auto RawPointer = (char*)InputMatrix.GetElementPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();

    return true;
}


template<typename ElementType>
DenseMatrix<ElementType> LoadDenseMatrixFromJsonDataFile(const CharString& FilePathAndName)
{
    DenseMatrix<ElementType> OutputMatrix;

    //----------------------------------------------------------

    auto ReferenceScalar = ElementType(0);

    auto OutputElementTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputElementTypeName(OutputElementTypeName_temp.c_str());

    int_max ByteNumberOfOutputElementType = CalByteNumberOfScalar(ReferenceScalar);

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

    QString InputElementTypeName;

    auto it = HeaderObject.find("ElementType");
    if (it != HeaderObject.end())
    {
        InputElementTypeName = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get ElementType @ LoadDenseMatrixFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get RowNumber @ LoadDenseMatrixFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get ColNumber @ LoadDenseMatrixFromJsonDataFile(...)")
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

    if (OutputElementTypeName == InputElementTypeName)
    {
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(OutputMatrix, DataFile, ByteNumberOfOutputElementType);
    }
    else
    {
        MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ LoadDenseMatrixFromJsonDataFile(...)")

        Internal_LoadDenseMatrixFromJsonDataFile(OutputMatrix, DataFile, InputElementTypeName);
    }

    DataFile.close();

    return OutputMatrix;
}


template<typename OutputElementType>
void Internal_LoadDenseMatrixFromJsonDataFile(DenseMatrix<OutputElementType>& OutputMatrix, QFile& DataFile, const QString& InputElementTypeName)
{
    if (InputElementTypeName == "double")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, double>(OutputMatrix, DataFile, 8);
    }
    else if (InputElementTypeName == "float")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, float>(OutputMatrix, DataFile, 4);
    }
    else if (InputElementTypeName == "int8")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, int8>(OutputMatrix, DataFile, 1);
    }
    else if (InputElementTypeName == "int16")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, int16>(OutputMatrix, DataFile, 2);
    }
    else if (InputElementTypeName == "int32")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, int32>(OutputMatrix, DataFile, 4);
    }
    else if (InputElementTypeName == "int64")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, int64>(OutputMatrix, DataFile, 8);
    }
    else if (InputElementTypeName == "uint8")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, uint8>(OutputMatrix, DataFile, 1);
    }
    else if (InputElementTypeName == "uint16")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, uint16>(OutputMatrix, DataFile, 2);
    }
    else if (InputElementTypeName == "uint32")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, uint32>(OutputMatrix, DataFile, 4);
    }
    else if (InputElementTypeName == "uint64")
    {
        Internal_LoadDenseMatrixFromJsonDataFile<OutputElementType, uint64>(OutputMatrix, DataFile, 8);
    }
    else
    {
        MDK_Error("unknown ElementType of data file @ Internal_LoadDenseMatrixFromJsonDataFile(...) ")
    }
}


template<typename OutputElementType, typename InputElementType>
void Internal_LoadDenseMatrixFromJsonDataFile(DenseMatrix<OutputElementType>& OutputMatrix, QFile& DataFile, int_max ByteNumberOfInputElementType)
{
    int_max BypesofDataFile = DataFile.size();

    int_max ElementNumber = OutputMatrix.GetElementNumber();

    if (BypesofDataFile != ElementNumber * ByteNumberOfInputElementType)
    {
        MDK_Error("Data file size is not equal to matrix size @ Internal_LoadDenseMatrixFromJsonDataFile(...)")
        return;
    }

    auto ElementPointer = OutputMatrix.GetElementPointer();

    auto tempScalar = InputElementType(0);

    for (int_max i = 0; i < ElementNumber; ++i)
    {
        DataFile.read((char*)(&tempScalar), ByteNumberOfInputElementType);

        ElementPointer[i] = OutputElementType(tempScalar);
    }
}



// for example: FilePathAndName = "C:/Data/SomeImage.json"
// header is saved in SomeImage.json
// data is saved in SomeImage.json.data
//
template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const Image3D<PixelType>& InputImage, const CharString& FilePathAndName)
{
    auto ReferenceScalar = PixelType(0);

    int_max ByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ Save3DScalarImageAsJsonDataFile(...)")
        return false;
    }

    auto ElementTypeName = FindScalarTypeName(ReferenceScalar);
    QString QElementTypeName(ElementTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList;
    PairList.resize(7);

    PairList[0].Name = "ObjectType";
    PairList[0].Value = "Image";

    PairList[1].Name = "Dimension";
    PairList[1].Value = "3";

    PairList[2].Name = "PixelType";
    PairList[2].Value = QElementTypeName;

    auto Size = InputImage.GetSize();

    PairList[3].Name = "Size";
    PairList[3].Value = QString::number(Size.Lx) + ", " + QString::number(Size.Ly) + ", " + QString::number(Size.Lz);

    auto Spacing = InputImage.GetSpacing();

    PairList[4].Name = "Spacing";
    PairList[4].Value = QString::number(Spacing.Sx) + ", " + QString::number(Spacing.Sy) + ", " + QString::number(Spacing.Sz);

    auto Origin = InputImage.GetOrigin();

    PairList[5].Name = "Origin";
    PairList[5].Value = QString::number(Origin.x) + ", " + QString::number(Origin.y) + ", " + QString::number(Origin.z);

    auto Orientation = InputImage.GetOrientation();

    PairList[6].Name = "Orientation";
    PairList[6].Value =   QString::number(Orientation(0,0)) + ","
                        + QString::number(Orientation(1,0)) + ","
                        + QString::number(Orientation(2,0)) + ","
                        + QString::number(Orientation(0,1)) + ","
                        + QString::number(Orientation(1,1)) + ","
                        + QString::number(Orientation(2,1)) + ","
                        + QString::number(Orientation(0,2)) + ","
                        + QString::number(Orientation(1,2)) + ","
                        + QString::number(Orientation(2,2));

    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    WritePairListAsJsonFile(PairList, QFilePathAndName);

    // write data file  --------------------------------------------------

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write image data @ Save3DScalarImageAsJsonDataFile(...)")
        return false;
    }

    int_max L = Size.Lx * Size.Ly * Size.Lz;

    auto RawPointer = (char*)InputImage.GetPixelPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();
    DataFile.close();

    return true;
}


template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromJsonDataFile(const CharString& FilePathAndName)
{
    Image3D<PixelType> OutputImage;

    //----------------------------------------------------------

    auto ReferenceScalar = PixelType(0);

    auto OutputElementTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputElementTypeName(OutputElementTypeName_temp.c_str());

    int_max ByteNumberOfOutputElementType = CalByteNumberOfScalar(ReferenceScalar);

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    QFile HeaderFile(QFilePathAndName);

    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open Header File @ Load3DScalarImageFromJsonDataFile(...)")
        return OutputImage;
    }

    //----------------------------------------------------------//
    QByteArray HeaderContent = HeaderFile.readAll();
    QJsonDocument HeaderDoc(QJsonDocument::fromJson(HeaderContent));
    QJsonObject HeaderObject = HeaderDoc.object();
    //-----------------------------------------------------------//

    auto it = HeaderObject.find("Dimension");
    if (it != HeaderObject.end())
    {
        auto Dimension = it.value().toString().toLongLong();

        if (Dimension != 3)
        {
            MDK_Error("Dimension is not 3 @ Load3DScalarImageFromJsonDataFile(...)")
            HeaderFile.close();
            return OutputImage;
        }
    }
    else
    {
        MDK_Error("Couldn't get Dimension @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    //---------------------------------------------------

    QString InputElementTypeName = 0;

    it = HeaderObject.find("PixelType");
    if (it != HeaderObject.end())
    {
        InputElementTypeName = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get PixelType @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    //---------------------------------------------------

    QString SizeStr;

    it = HeaderObject.find("Size");
    if (it != HeaderObject.end())
    {
        SizeStr = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get Size @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    auto SizeValue = SizeStr.split(",");
    if (SizeValue.size() != 3)
    {
        MDK_Error("Size vector is wrong @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    ImageSize3D Size;
    Size.Lx = SizeValue[0].toLongLong();
    Size.Ly = SizeValue[1].toLongLong();
    Size.Lz = SizeValue[2].toLongLong();

   
    QString SpacingStr;

    it = HeaderObject.find("Spacing");
    if (it != HeaderObject.end())
    {
        SpacingStr = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get Spacing @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    auto SpacingValue = SpacingStr.split(",");
    if (SpacingValue.size() != 3)
    {
        MDK_Error("Spacing vector is wrong @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    ImageSpacing3D Spacing;
    Spacing.Sx = SpacingValue[0].toDouble();
    Spacing.Sy = SpacingValue[1].toDouble();
    Spacing.Sz = SpacingValue[2].toDouble();


    QString OriginStr;

    it = HeaderObject.find("Origin");
    if (it != HeaderObject.end())
    {
        OriginStr = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get Origin @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    auto OriginValue = OriginStr.split(",");
    if (OriginValue.size() != 3)
    {
        MDK_Error("Origin vector is wrong @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    ImageOrigin3D Origin;
    Origin.x = OriginValue[0].toDouble();
    Origin.y = OriginValue[1].toDouble();
    Origin.z = OriginValue[2].toDouble();


    QString OrientationStr;

    it = HeaderObject.find("Orientation");
    if (it != HeaderObject.end())
    {
        OrientationStr = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get Orientation @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    auto OrientationValue = OrientationStr.split(",");
    if (OrientationValue.size() != 9)
    {
        MDK_Error("Orientation Matrix size is wrong @ Load3DScalarImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    DenseMatrix<double> Orientation(3, 3);
    for (int_max k = 0; k < 9; ++k)
    {
        Orientation[k] = OrientationValue[k].toDouble();
    }

    //-----------------------

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
    OutputImage.SetSpacing(Spacing);
    OutputImage.SetOrigin(Origin);
    OutputImage.SetOrientation(Orientation);

    //----------------------------------------------------------------------------------------------

    if (OutputElementTypeName == InputElementTypeName)
    {
        Internal_Load3DScalarImageFromJsonDataFile<PixelType, PixelType>(OutputImage, DataFile, ByteNumberOfOutputElementType);

        DataFile.close();

        return OutputImage;
    }

    MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ Load3DScalarImageFromJsonDataFile(...)")

    Internal_Load3DScalarImageFromJsonDataFile(OutputImage, DataFile, InputElementTypeName);

    DataFile.close();

    return OutputImage;
}


template<typename OutputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, const QString& InputElementTypeName)
{
    if (InputElementTypeName == "double")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, double>(OutputImage, DataFile, 8);
    }
    else if (InputElementTypeName == "float")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, float>(OutputImage, DataFile, 4);
    }
    else if (InputElementTypeName == "int8")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, int8>(OutputImage, DataFile, 1);
    }
    else if (InputElementTypeName == "int16")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, int16>(OutputImage, DataFile, 2);
    }
    else if (InputElementTypeName == "int32")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, int32>(OutputImage, DataFile, 4);
    }
    else if (InputElementTypeName == "int64")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, int64>(OutputImage, DataFile, 8);
    }
    else if (InputElementTypeName == "uint8")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, uint8>(OutputImage, DataFile, 1);
    }
    else if (InputElementTypeName == "uint16")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, uint16>(OutputImage, DataFile, 2);
    }
    else if (InputElementTypeName == "uint32")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, uint32>(OutputImage, DataFile, 4);
    }
    else if (InputElementTypeName == "uint64")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, uint64>(OutputImage, DataFile, 8);
    }
    else
    {
        MDK_Error("unknown ElementType of data file @ Internal_Load3DScalarImageFromJsonDataFile(...) ")
        DataFile.close();
        OutputImage.Clear();
    }
}


template<typename OutputPixelType, typename InputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, int_max ByteNumberOfInputElementType)
{
    int_max BypesofDataFile = DataFile.size();

    auto PixelNumber = OutputImage.GetPixelNumber();

    if (BypesofDataFile != PixelNumber * ByteNumberOfInputElementType)
    {
        MDK_Error("Data file size is not equal to image size @ Internal_Load3DScalarImageFromJsonDataFile(...)")
        DataFile.close();
        OutputImage.Clear();
        return;
    }

    auto PixelPointer = OutputImage.GetPixelPointer();

    auto tempScalar = InputPixelType(0);

    for (int_max i = 0; i < PixelNumber; ++i)
    {
        DataFile.read((char*)(&tempScalar), ByteNumberOfInputElementType);

        PixelPointer[i] = OutputPixelType(tempScalar);
    }
}


template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromDICOMSeries(const CharString& FilePath)
{
    Image3D<PixelType> OutputImage;


    typedef itk::Image< PixelType, 3 >                ITKImageType;

    typedef itk::ImageSeriesReader< ITKImageType >    ITKImageReaderType;

    typedef itk::GDCMImageIO                          ImageIOType;
    typedef itk::GDCMSeriesFileNames                  InputNamesGeneratorType;

    ImageIOType::Pointer gdcmIO = ImageIOType::New();
    InputNamesGeneratorType::Pointer inputNames = InputNamesGeneratorType::New();
    inputNames->SetInputDirectory(FilePath.StdString());

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
        std::cerr << "Exception thrown while reading the dicom series @ Load3DScalarImageFromDICOMSeries(...)" << std::endl;
        std::cerr << excp << std::endl;
        return OutputImage;
    }

    OutputImage = ConvertITK3DScalarImageToMDK3DScalarImage(ITKImageReader->GetOutput());

    return OutputImage;
}


template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromSingleDICOMFile(const CharString& FilePathAndName)
{
    Image3D<PixelType> OutputImage;

    typedef itk::Image<PixelType, 3>  ITKImageType;
    typedef itk::ImageFileReader<ImageType> ITKImageReaderType;

    ITKImageReaderType::Pointer  ITKImageReader = ITKImageReaderType::New();

    ITKImageReader->SetFileName(FilePathAndName.StdString());

    try
    {
        ITKImageReader->Update();
    }
    catch (itk::ExceptionObject & err)
    {
        std::cerr << "ExceptionObject caught while reading the dicom file @ Load3DScalarImageFromSingleDICOMFile(...)" << std::endl;
        std::cerr << err << std::endl;
        return OutputImage;
    }

    OutputImage = ConvertITK3DScalarImageToMDK3DScalarImage(ITKImageReader->GetOutput());

    return OutputImage;
}


template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(Image3D<PixelType>& InputImage, bool SharePixelData)
{
    auto InputSize = InputImage.GetSize();
    auto InputOrigin = InputImage.GetOrigin();
    auto InputSpacing = InputImage.GetSpacing();
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

    ITKImportFilterType::DirectionType Direction;
    typedef itk::SpacePrecisionType SpacePrecisionType;

    const DenseMatrix<double>& Orientation = InputImage.GetOrientation();
    for (int j = 0; j < 3; ++j)
    {
        for (int i = 0; i < 3; ++i)
        {
            Direction(i, j) = SpacePrecisionType(Orientation(i, j));
        }
    }
    importFilter->SetDirection(Direction);
    
    if (SharePixelData == true)
    {       
        bool ITKImageWillOwnPixelDataArray = false;
        importFilter->SetImportPointer(InputImage.GetPixelPointer(), InputPixelNumber, ITKImageWillOwnPixelDataArray);
    }
    else
    {
        PixelType* PixelDataArray = new PixelType[InputPixelNumber];
        for (int_max i = 0; i < InputPixelNumber; ++i)
        {
            PixelDataArray[i] = InputImage[i];
        }

        bool ITKImageWillOwnPixelDataArray = true;
        importFilter->SetImportPointer(PixelDataArray, InputPixelNumber, ITKImageWillOwnPixelDataArray);
    }

    importFilter->Update();

    return importFilter;
}


template<typename PixelType>
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(Image3D<PixelType>& InputImage)
{
    bool SharePixelData = true;
    auto importFilter = ConvertMDK3DScalarImageToITK3DScalarImage(InputImage, SharePixelData);

    typedef itk::Image<PixelType, 3>  ITKImageType;
    typedef itk::ImageDuplicator<ITKImageType> DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(importFilter->GetOutput());
    duplicator->Update();

    ITKImageType::Pointer ITKImage = duplicator->GetOutput();

    return ITKImage;
}


template<typename PixelType>
Image3D<PixelType> ConvertITK3DScalarImageToMDK3DScalarImage(const itk::Image<PixelType, 3>* ITKImage)
{
    Image3D<PixelType> OutputImage;

    if (ITKImage == nullptr)
    {
        MDK_Error("Invalid input @ ConvertITK3DScalarImageToMDK3DScalarImage(...)")
        return OutputImage;
    }

    auto Size = ITKImage->GetBufferedRegion().GetSize();
    auto Spacing = ITKImage->GetSpacing();
    auto Origin = ITKImage->GetOrigin();
    auto Direction = ITKImage->GetDirection();

    OutputImage.SetSize(Size[0], Size[1], Size[2]);
    OutputImage.SetSpacing(Spacing[0], Spacing[1], Spacing[2]);
    OutputImage.SetOrigin(Origin[0], Origin[1], Origin[2]);

    DenseMatrix<double> Orientation(3, 3);
    for (int j = 0; j < 3; ++j)
    {
        for (int i = 0; i < 3; ++i)
        {
            Orientation(i, j) = double(Direction(i, j));
        }
    }
    OutputImage.SetOrientation(Orientation);

    auto RawPointerOfITKImage = ITKImage->GetBufferPointer();

    for (int_max i = 0; i < OutputImage.GetPixelNumber(); ++i)
    {
        OutputImage[i] = RawPointerOfITKImage[i];
    }

    return OutputImage;
}


template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const Image3D<PixelType>& InputImage)
{
    auto VTKImage = vtkSmartPointer<vtkImageData>::New();

    auto Size = InputImage.GetSize();
    auto Origin = InputImage.GetOrigin();
    auto Spacing = InputImage.GetSpacing();

    auto PtrOfInputImage = InputImage.GetPixelPointer();

    VTKImage->SetExtent(0, Size.Lx - 1, 0, Size.Ly - 1, 0, Size.Lz - 1);
    VTKImage->SetOrigin(Origin.x, Origin.y, Origin.z);    
    VTKImage->SetSpacing(Spacing.Sx, Spacing.Sy, Spacing.Sz);

    auto ReferenceScalar = PixelType(0);
    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);

    if (ScalarTypeName == "double")
    {
        VTKImage->AllocateScalars(VTK_DOUBLE, 1);

        auto PtrOfVTKImage = static_cast<double*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "float")
    {
        VTKImage->AllocateScalars(VTK_FLOAT, 1);

        auto PtrOfVTKImage = static_cast<float*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "int8")
    {
        VTKImage->AllocateScalars(VTK_CHAR, 1);

        auto PtrOfVTKImage = static_cast<int8*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "int16")
    {
        VTKImage->AllocateScalars(VTK_SHORT, 1);

        auto PtrOfVTKImage = static_cast<int16*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "int32")
    {
        VTKImage->AllocateScalars(VTK_INT, 1);

        auto PtrOfVTKImage = static_cast<int32*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "int64")
    {
        VTKImage->AllocateScalars(VTK_LONG_LONG, 1);

        auto PtrOfVTKImage = static_cast<int64*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "uint8")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

        auto PtrOfVTKImage = static_cast<uint8*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "uint16")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_SHORT, 1);

        auto PtrOfVTKImage = static_cast<uint16*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "uint32")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_INT, 1);

        auto PtrOfVTKImage = static_cast<uint32*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "uint64")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_LONG_LONG, 1);

        auto PtrOfVTKImage = static_cast<uint64*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else
    {
        MDK_Error("unknown ScalarType @ ConvertMDK3DScalarImageToVTK3DScalarImage(...) ")
    }

    return VTKImage;
}


template<typename PixelType>
Image3D<PixelType> ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage)
{
    int Extent[6];
    VTKImage->GetExtent(Extent);

    double Spacing[3];
    VTKImage->GetSpacing(Spacing);

    double Origin[3];
    VTKImage->GetOrigin(Origin);

    auto VTKScalarType = VTKImage->GetScalarType();

    Image3D<PixelType> OutputImage;
    OutputImage.SetSize(Extent[1] + 1, Extent[3] + 1, Extent[5] + 1);
    OutputImage.SetOrigin(Origin[0], Origin[1], Origin[2]);
    OutputImage.SetSpacing(Spacing[0], Spacing[1], Spacing[2]);

    auto PtrOfOutputImage = OutputImage.GetPixelPointer();

    if (VTKScalarType == "double")
    {
        auto PtrOfVTKImage = static_cast<double*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "float")
    {
        auto PtrOfVTKImage = static_cast<float*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "int8")
    {
        auto PtrOfVTKImage = static_cast<int8*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "int16")
    {
        auto PtrOfVTKImage = static_cast<int16*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "int32")
    {
        auto PtrOfVTKImage = static_cast<int32*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "int64")
    {
        auto PtrOfVTKImage = static_cast<int64*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "uint8")
    {
        auto PtrOfVTKImage = static_cast<uint8*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "uint16")
    {
        auto PtrOfVTKImage = static_cast<uint16*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "uint32")
    {
        auto PtrOfVTKImage = static_cast<uint32*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (ScalarTypeName == "uint64")
    {
        auto PtrOfVTKImage = static_cast<uint64*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else
    {
        MDK_Error("unknown ScalarType @ ConvertVTK3DScalarImageToMDK3DScalarImage(...) ")
    }

    return OutputImage;
}


// for example: FilePathAndName = "C:/Data/SomeDictionary.json"
//
template<typename ElementType>
bool SaveFeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const CharString& FilePathAndName)
{
    ElementType ReferenceScalar = ElementType(0);

    int_max ByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ SaveFeatureDictionaryForSparseCoding(...)")
        return false;
    }

    auto ElementTypeName = FindScalarTypeName(ReferenceScalar);
    QString QElementTypeName(ElementTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(5);

    PairList[0].Name = "DictionaryType";
    PairList[0].Value = "FeatureDictionaryForSparseCoding";

    PairList[1].Name = "Name";
    PairList[1].Value = Dictionary.GetName().StdString().c_str();

    PairList[2].Name = "ElementType";
    PairList[2].Value = QElementTypeName;

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

    // step 2 : write VarianceOfL1Distance
    L = Dictionary.VarianceOfL1Distance().GetElementNumber();
    RawPointer = (char*)Dictionary.VarianceOfL1Distance().GetElementPointer();
    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();
    DataFile.close();

    return true;
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> LoadFeatureDictionaryForSparseCoding(const CharString& FilePathAndName)
{
    FeatureDictionaryForSparseCoding<ElementType> Dictionary;

    //----------------------------------------------------------

    auto ReferenceScalar = ElementType(0);

    auto OutputElementTypeName_temp = FindScalarTypeName(ReferenceScalar);

    QString OutputElementTypeName(OutputElementTypeName_temp.c_str());

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

    QString InputElementTypeName = 0;

    auto it = HeaderObject.find("ElementType");
    if (it != HeaderObject.end())
    {
        InputElementTypeName = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get ElementType @ LoadFeatureDictionaryForSparseCoding(...)")
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

    Dictionary.SetName(Name);
    Dictionary.BasisMatrix().FastResize(RowNumber, ColNumber);
    Dictionary.VarianceOfL1Distance().FastResize(1, ColNumber);
    Dictionary.VarianceOfL2Distance().FastResize(1, ColNumber);
    Dictionary.VarianceOfKLDivergence().FastResize(1, ColNumber);
    Dictionary.VarianceOfReconstruction().FastResize(1, ColNumber);

    if (OutputElementTypeName == InputElementTypeName)
    {
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.BasisMatrix(), DataFile, OutputByteNumber);
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.VarianceOfL1Distance(), DataFile, OutputByteNumber);
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.VarianceOfL2Distance(), DataFile, OutputByteNumber);
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.VarianceOfKLDivergence(), DataFile, OutputByteNumber);
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.VarianceOfReconstruction(), DataFile, OutputByteNumber);
    }
    else
    {
        MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ LoadFeatureDictionaryForSparseCoding(...)")

        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.BasisMatrix(), DataFile, InputElementTypeName);
        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.VarianceOfL1Distance(), DataFile, InputElementTypeName);
        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.VarianceOfL2Distance(), DataFile, InputElementTypeName);
        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.VarianceOfKLDivergence(), DataFile, InputElementTypeName);
        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.VarianceOfReconstruction(), DataFile, InputElementTypeName);
    }

    DataFile.close();

    return Dictionary;
}


}//namespace mdk

#endif