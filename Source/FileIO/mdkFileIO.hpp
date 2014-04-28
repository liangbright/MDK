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
template<typename ElementType>
bool Save3DImageAsJsonDataFile(const Image3D<ElementType>& InputImage, const CharString& FilePathAndName)
{
    ElementType ReferenceScalar = ElementType(0);

    int_max ByteNumber = CalByteNumberOfScalar(ReferenceScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ Save3DImageAsJsonDataFile(...)")
        return false;
    }

    auto ElementTypeName = FindScalarTypeName(ReferenceScalar);
    QString QElementTypeName(ElementTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(12);

    PairList[0].Name = "Dimension";
    PairList[0].Value = "3";

    PairList[1].Name = "PixelType";
    PairList[1].Value = QElementTypeName;

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

    auto Orientation = InputImage.GetOrientation();

    PairList[11].Name = "Orientation";
    PairList[11].Value =  QString::number(Orientation(0,0)) + ","
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
        MDK_Error("Couldn't open file to write image data @ Save3DImageAsJsonDataFile(...)")
        return false;
    }

    int_max L = Size.Lx * Size.Ly * Size.Lz;

    auto RawPointer = (char*)InputImage.GetPixelPointer();

    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();
    DataFile.close();

    return true;
}


template<typename ElementType>
Image3D<ElementType> Load3DImageFromJsonDataFile(const CharString& FilePathAndName)
{
    Image3D<ElementType> OutputImage;

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
        MDK_Error("Couldn't open Header File @ Load3DImageFromJsonDataFile(...)")
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
            MDK_Error("Dimension is not 3 @ Load3DImageFromJsonDataFile(...)")
            HeaderFile.close();
            return OutputImage;
        }
    }
    else
    {
        MDK_Error("Couldn't get Dimension @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get PixelType @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Size_x @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Size_y @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Size_z @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Spacing_x @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Spacing_y @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Spacing_z @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Origin_x @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Origin_y @ Load3DImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get Origin_z @ Load3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    QString OrientationStr;

    it = HeaderObject.find("Orientation");
    if (it != HeaderObject.end())
    {
        OrientationStr = it.value().toString();
    }
    else
    {
        MDK_Error("Couldn't get Orientation @ Load3DImageFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputImage;
    }

    auto OrientationValue = OrientationStr.split(",");
    if (OrientationValue.size() != 9)
    {
        MDK_Error("Orientation Matrix size is wrong @ Load3DImageFromJsonDataFile(...)")
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
    OutputImage.SetPixelSpacing(Spacing);
    OutputImage.SetPhysicalOrigin(Origin);
    OutputImage.SetOrientation(Orientation);

    //----------------------------------------------------------------------------------------------

    if (OutputElementTypeName == InputElementTypeName)
    {
        Internal_Load3DImageFromJsonDataFile<ElementType, ElementType>(OutputImage, DataFile, ByteNumberOfOutputElementType);

        DataFile.close();

        return OutputImage;
    }

    MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ Load3DImageFromJsonDataFile(...)")

    Internal_Load3DImageFromJsonDataFile(OutputImage, DataFile, InputElementTypeName);

    DataFile.close();

    return OutputImage;
}


template<typename OutputElementType>
void Internal_Load3DImageFromJsonDataFile(Image3D<OutputElementType>& OutputImage, QFile& DataFile, const QString& InputElementTypeName)
{
    if (InputElementTypeName == "double")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, double>(OutputImage, DataFile, 8);
    }
    else if (InputElementTypeName == "float")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, float>(OutputImage, DataFile, 4);
    }
    else if (InputElementTypeName == "int8")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, int8>(OutputImage, DataFile, 1);
    }
    else if (InputElementTypeName == "int16")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, int16>(OutputImage, DataFile, 2);
    }
    else if (InputElementTypeName == "int32")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, int32>(OutputImage, DataFile, 4);
    }
    else if (InputElementTypeName == "int64")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, int64>(OutputImage, DataFile, 8);
    }
    else if (InputElementTypeName == "uint8")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, uint8>(OutputImage, DataFile, 1);
    }
    else if (InputElementTypeName == "uint16")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, uint16>(OutputImage, DataFile, 2);
    }
    else if (InputElementTypeName == "uint32")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, uint32>(OutputImage, DataFile, 4);
    }
    else if (InputElementTypeName == "uint64")
    {
        Internal_Load3DImageFromJsonDataFile<OutputElementType, uint64>(OutputImage, DataFile, 8);
    }
    else
    {
        MDK_Error("unknown ElementType of data file @ Internal_Load3DImageFromJsonDataFile(...) ")
        DataFile.close();
        OutputImage.Clear();
    }
}


template<typename OutputElementType, typename InputElementType>
void Internal_Load3DImageFromJsonDataFile(Image3D<OutputElementType>& OutputImage, QFile& DataFile, int_max ByteNumberOfInputElementType)
{
    int_max BypesofDataFile = DataFile.size();

    auto PixelNumber = OutputImage.GetPixelNumber();

    if (BypesofDataFile != PixelNumber * ByteNumberOfInputElementType)
    {
        MDK_Error("Data file size is not equal to image size @ Internal_Load3DImageFromJsonDataFile(...)")
        DataFile.close();
        OutputImage.Clear();
        return;
    }

    auto PixelPointer = OutputImage.GetPixelPointer();

    auto tempScalar = InputElementType(0);

    for (int_max i = 0; i < PixelNumber; ++i)
    {
        DataFile.read((char*)(&tempScalar), ByteNumberOfInputElementType);

        PixelPointer[i] = OutputElementType(tempScalar);
    }
}


template<typename ElementType>
Image3D<ElementType> Load3DImageFromDICOMSeries(const CharString& FilePath)
{
    Image3D<ElementType> OutputImage;


    typedef itk::Image< ElementType, 3 >               ITKImageType;

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
        std::cerr << "Exception thrown while reading the dicom series @ Load3DImageFromDICOMSeries(...)" << std::endl;
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

    ITKImportFilterType::DirectionType Direction;
    typedef itk::SpacePrecisionType SpacePrecisionType;

    DenseMatrix<double>& Orientation = InputImage->GetOrientation();
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
    auto Direction = ITKImage->GetDirection();

    OutputImage.SetSize(Size[0], Size[1], Size[2]);
    OutputImage.SetPixelSpacing(Spacing[0], Spacing[1], Spacing[2]);
    OutputImage.SetPhysicalOrigin(Origin[0], Origin[1], Origin[2]);

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

    // step 2 : write StandardDeviationOfL1Distance
    L = Dictionary.StandardDeviationOfL1Distance().GetElementNumber();
    RawPointer = (char*)Dictionary.StandardDeviationOfL1Distance().GetElementPointer();
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
    Dictionary.StandardDeviationOfL1Distance().FastResize(1, ColNumber);
    Dictionary.StandardDeviationOfL2Distance().FastResize(1, ColNumber);
    Dictionary.StandardDeviationOfKLDivergence().FastResize(1, ColNumber);
    Dictionary.StandardDeviationOfReconstruction().FastResize(1, ColNumber);

    if (OutputElementTypeName == InputElementTypeName)
    {
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.BasisMatrix(), DataFile, OutputByteNumber);
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.StandardDeviationOfL1Distance(), DataFile, OutputByteNumber);
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.StandardDeviationOfL2Distance(), DataFile, OutputByteNumber);
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.StandardDeviationOfKLDivergence(), DataFile, OutputByteNumber);
        Internal_LoadDenseMatrixFromJsonDataFile<ElementType, ElementType>(Dictionary.StandardDeviationOfReconstruction(), DataFile, OutputByteNumber);
    }
    else
    {
        MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ LoadFeatureDictionaryForSparseCoding(...)")

        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.BasisMatrix(), DataFile, InputElementTypeName);
        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.StandardDeviationOfL1Distance(), DataFile, InputElementTypeName);
        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.StandardDeviationOfL2Distance(), DataFile, InputElementTypeName);
        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.StandardDeviationOfKLDivergence(), DataFile, InputElementTypeName);
        Internal_LoadDenseMatrixFromJsonDataFile(Dictionary.StandardDeviationOfReconstruction(), DataFile, InputElementTypeName);
    }

    DataFile.close();

    return Dictionary;
}


}//namespace mdk

#endif