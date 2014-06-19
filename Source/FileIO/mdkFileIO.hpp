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

    auto OutputElementTypeName = FindScalarTypeName(ReferenceScalar);

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

    std::string InputElementTypeName;

    auto it = HeaderObject.find("ElementType");
    if (it != HeaderObject.end())
    {
        InputElementTypeName = it.value().toString().toStdString();
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
void Internal_LoadDenseMatrixFromJsonDataFile(DenseMatrix<OutputElementType>& OutputMatrix, QFile& DataFile, const std::string& InputElementTypeName)
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

    auto OutputPixelTypeName = FindScalarTypeName(ReferenceScalar);

    int_max ByteNumberOfOutputPixelType = CalByteNumberOfScalar(ReferenceScalar);

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

    std::string InputPixelTypeName;

    it = HeaderObject.find("PixelType");
    if (it != HeaderObject.end())
    {
        InputPixelTypeName = it.value().toString().toStdString();
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

    if (OutputPixelTypeName == InputPixelTypeName)
    {
        Internal_Load3DScalarImageFromJsonDataFile<PixelType, PixelType>(OutputImage, DataFile, ByteNumberOfOutputPixelType);

        DataFile.close();

        return OutputImage;
    }

    MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ Load3DScalarImageFromJsonDataFile(...)")

    Internal_Load3DScalarImageFromJsonDataFile(OutputImage, DataFile, InputPixelTypeName);

    DataFile.close();

    return OutputImage;
}


template<typename OutputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, const std::string& InputPixelTypeName)
{
    if (InputPixelTypeName == "double")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, double>(OutputImage, DataFile, 8);
    }
    else if (InputPixelTypeName == "float")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, float>(OutputImage, DataFile, 4);
    }
    else if (InputPixelTypeName == "int8")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, int8>(OutputImage, DataFile, 1);
    }
    else if (InputPixelTypeName == "int16")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, int16>(OutputImage, DataFile, 2);
    }
    else if (InputPixelTypeName == "int32")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, int32>(OutputImage, DataFile, 4);
    }
    else if (InputPixelTypeName == "int64")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, int64>(OutputImage, DataFile, 8);
    }
    else if (InputPixelTypeName == "uint8")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, uint8>(OutputImage, DataFile, 1);
    }
    else if (InputPixelTypeName == "uint16")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, uint16>(OutputImage, DataFile, 2);
    }
    else if (InputPixelTypeName == "uint32")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, uint32>(OutputImage, DataFile, 4);
    }
    else if (InputPixelTypeName == "uint64")
    {
        Internal_Load3DScalarImageFromJsonDataFile<OutputPixelType, uint64>(OutputImage, DataFile, 8);
    }
    else
    {
        MDK_Error("unknown PixelType of data file @ Internal_Load3DScalarImageFromJsonDataFile(...) ")
        DataFile.close();
        OutputImage.Clear();
    }
}


template<typename OutputPixelType, typename InputPixelType>
void Internal_Load3DScalarImageFromJsonDataFile(Image3D<OutputPixelType>& OutputImage, QFile& DataFile, int_max ByteNumberOfOutputPixelType)
{
    int_max BypesofDataFile = DataFile.size();

    auto PixelNumber = OutputImage.GetPixelNumber();

    if (BypesofDataFile != PixelNumber * ByteNumberOfOutputPixelType)
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
        DataFile.read((char*)(&tempScalar), ByteNumberOfOutputPixelType);

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


template<typename ScalarType>
bool SaveTriangleMeshAsJsonDataFile(const TriangleMesh<ScalarType>& InputMesh, const CharString& FilePathAndName)
{
    if (CalByteNumberOfScalar(ScalarType(0)) <= 0)
    {
        MDK_Error("Unknown ScalarType @ SaveTriangleMeshAsJsonDataFile(...)")
        return false;
    }

    auto ScalarTypeName = FindScalarTypeName(ScalarType(0));
    QString QScalarTypeName(ScalarTypeName.c_str());

    auto IndexTypeName = FindScalarTypeName(int_max(0));
    QString QIndexTypeName(IndexTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList;
    PairList.resize(5);

    PairList[0].Name = "ObjectType";
    PairList[0].Value = "TriangleMesh";

    PairList[1].Name = "ScalarType";
    PairList[1].Value = QScalarTypeName;

    PairList[2].Name = "IndexType";
    PairList[2].Value = QIndexTypeName;

    auto VertexNumber = InputMesh.GetVertexNumber();

    PairList[3].Name = "VertexNumber";
    PairList[3].Value = QString::number(VertexNumber);

    auto TriangleNumber = InputMesh.GetTriangleNumber();

    PairList[4].Name = "TriangleNumber";
    PairList[4].Value = QString::number(TriangleNumber);

    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    WritePairListAsJsonFile(PairList, QFilePathAndName);

    if (InputMesh.IsEmpty() == true)
    {
        return true;
    }

    // write vertex to data file  --------------------------------------------------

    QFile VertexDataFile(QFilePathAndName + ".data.vertex");

    if (!VertexDataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write vertex data @ SaveTriangleMeshAsJsonDataFile(...)")
        return false;
    }

    VertexDataFile.write((char*)InputMesh.VertexPositionTable().GetElementPointer(), 
                         InputMesh.VertexPositionTable().GetElementNumber()*CalByteNumberOfScalar(ScalarType(0)));
    VertexDataFile.flush();
    VertexDataFile.close();

    if (InputMesh.VertexGlobalIDList().IsEmpty() == false)
    {
        QFile VertexDataFile_id(QFilePathAndName + ".data.vertex_id");

        if (!VertexDataFile_id.open(QIODevice::WriteOnly))
        {
            MDK_Error("Couldn't open file to write vertex data @ SaveTriangleMeshAsJsonDataFile(...)")
            return false;
        }

        VertexDataFile_id.write((char*)InputMesh.VertexGlobalIDList().GetElementPointer(),
            InputMesh.VertexGlobalIDList().GetElementNumber()*CalByteNumberOfScalar(int_max(0)));

        VertexDataFile_id.flush();
        VertexDataFile_id.close();
    }

    //write triangle to data file -----------------------------------------------------

    QFile TriangleDataFile(QFilePathAndName + ".data.triangle");

    if (!TriangleDataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write triangle data @ SaveTriangleMeshAsJsonDataFile(...)")
        return false;
    }

    QTextStream Stream_out(&TriangleDataFile);

    const DataArray<DenseVector<int_max>>& TriangleData = InputMesh.VertexIndexTable();

    for (int_max i = 0; i < InputMesh.GetTriangleNumber(); ++i)
    {
        const DenseVector<int_max>& Triangle_i = TriangleData[i];

        for (int_max n = 0; n < Triangle_i.GetElementNumber(); ++n)
        {
            Stream_out << QString::number(Triangle_i[n]);

            if (n < Triangle_i.GetElementNumber() - 1)
            {
                Stream_out << ", ";
            }
        }

        if (i < InputMesh.GetTriangleNumber() - 1)
        {
            Stream_out << "\n";
        }
    }

    Stream_out.flush();

    TriangleDataFile.flush();
    TriangleDataFile.close();

    if (InputMesh.TriangleGlobalIDList().IsEmpty() == false)
    {
        QFile TriangleDataFile_id(QFilePathAndName + ".data.triangle_id");

        if (!TriangleDataFile_id.open(QIODevice::WriteOnly))
        {
            MDK_Error("Couldn't open file to write triangle data @ SaveTriangleMeshAsJsonDataFile(...)")
            return false;
        }

        TriangleDataFile_id.write((char*)InputMesh.TriangleGlobalIDList().GetElementPointer(),
                                  InputMesh.TriangleGlobalIDList().GetElementNumber()*CalByteNumberOfScalar(int_max(0)));
        TriangleDataFile_id.flush();
        TriangleDataFile_id.close();
    }

    return true;
}


template<typename ScalarType>
TriangleMesh<ScalarType> LoadTriangleMeshFromJsonDataFile(const CharString& FilePathAndName)
{
    TriangleMesh<ScalarType> OutputMesh;

    //----------------------------------------------------------

    auto OutputScalarTypeName = FindScalarTypeName(ScalarType(0));

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    QFile HeaderFile(QFilePathAndName);

    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open Header File @ LoadTriangleMeshFromJsonDataFile(...)")
        return OutputMesh;
    }

    //----------------------------------------------------------//
    QByteArray HeaderContent = HeaderFile.readAll();
    QJsonDocument HeaderDoc(QJsonDocument::fromJson(HeaderContent));
    QJsonObject HeaderObject = HeaderDoc.object();
    //-----------------------------------------------------------//

    //---------------------------------------------------

    std::string InputScalarTypeTypeName;

    it = HeaderObject.find("ScalarType");
    if (it != HeaderObject.end())
    {
        InputPixelTypeName = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get ScalarType @ LoadTriangleMeshFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMesh;
    }

    std::string InputIndexTypeTypeName;

    it = HeaderObject.find("IndexType");
    if (it != HeaderObject.end())
    {
        InputIndexTypeTypeName = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get IndexType @ LoadTriangleMeshFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMesh;
    }

    int_max VertexNumber = 0;

    it = HeaderObject.find("VertexNumber");
    if (it != HeaderObject.end())
    {
        VertexNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get VertexNumber @ LoadTriangleMeshFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMesh;
    }

    int_max TriangleNumber = 0;

    it = HeaderObject.find("TriangleNumber");
    if (it != HeaderObject.end())
    {
        TriangleNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get TriangleNumber @ LoadTriangleMeshFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMesh;
    }

    if (VertexNumber <= 0 || TriangleNumber <= 0)
    {
        return OutputMesh;
    }

    //--------------------------------- read data --------------------------------------------------------//

    // read Vertex from *.data.vertex file

    QFile VertexDataFile(QFilePathAndName + ".data.vertex");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open vertex data file:" << FilePathAndName)
        return OutputMesh;
    }

    DenseMatrix<ScalarType> Vertex(3, VertexNumber);

    if (OutputScalarTypeName != InputScalarTypeTypeName)
    {
        MDK_Warning("OutputScalarTypeName != InputScalarTypeTypeName, Output may be inaccurate @ LoadTriangleMeshFromJsonDataFile(...)")            
    }

    Internal_LoadDenseMatrixFromJsonDataFile(Vertex, VertexDataFile, InputScalarTypeTypeName);

    VertexDataFile.close();

    // read Triangle from *.data.triangle file

    QFile TriangleDataFile(QFilePathAndName + ".data.triangle");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open triangle data file:" << FilePathAndName)
        return OutputMesh;
    }

    DataArray<DenseVector<int_max>> Triangle;
    Triangle.FastResize(TriangleNumber);

    QTextStream stream_in(&PolygonDataFile);

    for (int_max i = 0; i < OutputMesh.TriangleNumber(); ++i)
    {
        DenseVector<int_max>& Triangle_i = Triangle[i];

        auto ValueList = stream_in.readLine().split(",");

        auto tempsize = ValueList.size();

        for (int_max n = 0; n < tempsize; ++n)
        {
            auto tempIndex = ValueList[n].toLongLong();
            Triangle_i.Append(tempIndex);
        }
    }

    TriangleDataFile.close();

    //--------------------------------------------------------------------------

    OutputMesh.Construct(std::move(Vertex), std::move(Triangle));

    return OutputMesh;
}


template<typename ScalarType>
bool SaveTriangleMeshAsVTKFile(const TriangleMesh<ScalarType>& InputMesh, const CharString& FilePathAndName)
{
    auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(InputMesh);

    auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName(FilePathAndName.StdString().c_str());
    writer->SetInputData(VTKMesh);

    try
    {
        writer->Write();
    }
    catch (...)
    {
        MDK_Error(" Can not write data @ SaveTriangleMeshAsVTKFile(...) ")
        return false;
    }    

    return true;
}


template<typename ScalarType>
TriangleMesh<ScalarType> LoadTriangleMeshFromVTKFile(const CharString& FilePathAndName)
{
    TriangleMesh<ScalarType> OutputMesh;

    auto Reader = vtkSmartPointer<vtkPolyDataReader>::New();
    Reader->SetFileName(FilePathAndName.StdString().c_str());
    
    try
    {
        Reader->Update();
    }
    catch (...)
    {
        MDK_Error(" Can not write data @ SaveTriangleMeshAsVTKFile(...) ")
        return OutputMesh;
    }

    auto VTKPolyMesh = Reader->GetOutput();
    OutputMesh = ConvertVTKPolyDataToMDKTriangleMesh<ScalarType>(VTKPolyMesh);
    return OutputMesh;
}


template<typename ScalarType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<ScalarType>& InputMesh, const CharString& FilePathAndName)
{
    if (CalByteNumberOfScalar(ScalarType(0)) <= 0)
    {
        MDK_Error("Unknown ScalarType @ SavePolygonMeshAsJsonDataFile(...)")
        return false;
    }

    auto ScalarTypeName = FindScalarTypeName(ScalarType(0));
    QString QScalarTypeName(ScalarTypeName.c_str());

    auto IndexTypeName = FindScalarTypeName(int_max(0));
    QString QIndexTypeName(IndexTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList;
    PairList.resize(5);

    PairList[0].Name = "ObjectType";
    PairList[0].Value = "PolygonMesh";

    PairList[1].Name = "ScalarType";
    PairList[1].Value = QScalarTypeName;

    PairList[2].Name = "IndexType";
    PairList[2].Value = QIndexTypeName;

    auto VertexNumber = InputMesh.GetVertexNumber();

    PairList[3].Name = "VertexNumber";
    PairList[3].Value = QString::number(VertexNumber);

    auto CellNumber = InputMesh.GetCellNumber();

    PairList[4].Name = "CellNumber";
    PairList[4].Value = QString::number(CellNumber);

    // write header file (json) --------------------------------------------------

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    WritePairListAsJsonFile(PairList, QFilePathAndName);

    if (InputMesh.IsEmpty() == true)
    {
        return true;
    }

    // get data --------------------------------------------------------------------
    DataArray<DenseVector<int_max>> PolygonData;
    DenseMatrix<ScalarType> VertexData;
    InputMesh.GetVertexPositionTableAndCellTable(VertexData, PolygonData);

    // write vertex to data file  --------------------------------------------------

    QFile VertexDataFile(QFilePathAndName + ".data.vertex");

    if (!VertexDataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write vertex data @ SavePolygonMeshAsJsonDataFile(...)")
        return false;
    }

    VertexDataFile.write((char*)VertexData.GetElementPointer(), VertexData.GetElementNumber()*CalByteNumberOfScalar(ScalarType(0)));
    VertexDataFile.flush();
    VertexDataFile.close();

    /*
    if (InputMesh.VertexGlobalIDList().IsEmpty() == false)
    {
        QFile VertexDataFile_id(QFilePathAndName + ".data.vertex_id");

        if (!VertexDataFile_id.open(QIODevice::WriteOnly))
        {
            MDK_Error("Couldn't open file to write triangle data @ SavePolygonMeshAsJsonDataFile(...)")
            return false;
        }

        VertexDataFile_id.write((char*)VertexData.GetElementPointer(),
                                VertexData.GetColNumber()*CalByteNumberOfScalar(int_max(0)));
        VertexDataFile_id.flush();
        VertexDataFile_id.close();
    }
    */

    //write Polygon to data file -----------------------------------------------------

    QFile PolygonDataFile(QFilePathAndName + ".data.polygon");

    if (!PolygonDataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write triangle data @ SavePolygonMeshAsJsonDataFile(...)")
        return false;
    }

    QTextStream Stream_out(&PolygonDataFile);

    for (int_max i = 0; i < PolygonData.GetLength(); ++i)
    {
        const DenseVector<int_max>& Polygon_i = PolygonData[i];

        for (int_max n = 0; n < Polygon_i.GetElementNumber(); ++n)
        {
            Stream_out << QString::number(Polygon_i[n]);
            
            if (n < Polygon_i.GetLength() - 1)
            {
                Stream_out  << ", ";
            }
        }

        if (i < PolygonData.GetLength() - 1)
        {
            Stream_out << "\n";
        }
    }

    Stream_out.flush();

    PolygonDataFile.flush();
    PolygonDataFile.close();
    /*
    if (InputMesh.PolygonGlobalIDList().IsEmpty() == false)
    {
        QFile PolygonDataFile_id(QFilePathAndName + ".data.polygon_id");

        if (!PolygonDataFile_id.open(QIODevice::WriteOnly))
        {
            MDK_Error("Couldn't open file to write triangle data @ SavePolygonMeshAsJsonDataFile(...)")
                return false;
        }

        PolygonDataFile_id.write((char*)InputMesh.PolygonGlobalIDList().GetElementPointer(),
                                  InputMesh.PolygonGlobalIDList().GetElementNumber()*CalByteNumberOfScalar(int_max(0)));
        PolygonDataFile_id.flush();
        PolygonDataFile_id.close();
    }
    */
    return true;
}


template<typename ScalarType>
PolygonMesh<ScalarType> LoadPolygonMeshFromJsonDataFile(const CharString& FilePathAndName, bool Flag_BuildLinkAndAdjacency)
{
    PolygonMesh<ScalarType> OutputMesh;

    //----------------------------------------------------------

    auto OutputScalarTypeName = FindScalarTypeName(ScalarType(0));

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.StdString().c_str());

    QFile HeaderFile(QFilePathAndName);

    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open Header File @ LoadPolygonMeshFromJsonDataFile(...)")
        return OutputMesh;
    }

    //----------------------------------------------------------//
    QByteArray HeaderContent = HeaderFile.readAll();
    QJsonDocument HeaderDoc(QJsonDocument::fromJson(HeaderContent));
    QJsonObject HeaderObject = HeaderDoc.object();
    //-----------------------------------------------------------//

    //---------------------------------------------------

    std::string InputScalarTypeTypeName;

    it = HeaderObject.find("ScalarType");
    if (it != HeaderObject.end())
    {
        InputPixelTypeName = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get ScalarType @ LoadPolygoneMeshFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMesh;
    }

    std::string InputIndexTypeTypeName;

    it = HeaderObject.find("IndexType");
    if (it != HeaderObject.end())
    {
        InputIndexTypeTypeName = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get IndexType @ LoadPolygonMeshFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMesh;
    }

    int_max VertexNumber = 0;

    it = HeaderObject.find("VertexNumber");
    if (it != HeaderObject.end())
    {
        VertexNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get VertexNumber @ LoadPolygonMeshFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMesh;
    }

    int_max CellNumber = 0;

    it = HeaderObject.find("CellNumber");
    if (it != HeaderObject.end())
    {
        CellNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get PolygonNumber @ LoadPolygonMeshFromJsonDataFile(...)")
        HeaderFile.close();
        return OutputMesh;
    }

    if (VertexNumber <= 0 || CellNumber <= 0)
    {
        return OutputMesh;
    }

    //--------------------------------- read data --------------------------------------------------------//

    // read Vertex from *.data.vertex file

    QFile VertexDataFile(QFilePathAndName + ".data.vertex");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open vertex data file:" << FilePathAndName)
        return OutputMesh;
    }

    DenseMatrix<ScalarType> Vertex(3, VertexNumber);

    if (OutputScalarTypeName != InputScalarTypeTypeName)
    {
        MDK_Warning("OutputScalarTypeName != InputScalarTypeTypeName, Output may be inaccurate @ LoadTriangleMeshFromJsonDataFile(...)")
    }

    Internal_LoadDenseMatrixFromJsonDataFile(Vertex, VertexDataFile, InputScalarTypeTypeName);

    VertexDataFile.close();

    // read Polygon from *.data.polygon file

    QFile PolygonDataFile(QFilePathAndName + ".data.polygon");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open polygon data file:" << FilePathAndName)
        return OutputMesh;
    }

    DataArray<DenseVector<int_max>> Polygon;
    Polygon.FastResize(CellNumber);

    QTextStream stream_in(&PolygonDataFile);

    for (int_max i = 0; i < CellNumber; ++i)
    {
        DenseVector<int_max>& Polygon_i = Polygon[i];

        auto ValueList = stream_in.readLine().split(",");
        
        auto tempsize = ValueList.size();

        for (int_max n = 0; n < tempsize; ++n)
        {
            auto tempIndex = ValueList[n].toLongLong();
            Polygon_i.Append(tempIndex);
        }
    }

    PolygonDataFile.close();

    //--------------------------------------------------------------------------

    OutputMesh.Construct(Vertex, Polygon);

    return OutputMesh;
}


template<typename ScalarType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<ScalarType>& InputMesh, const CharString& FilePathAndName)
{
    auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);

    auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName(FilePathAndName.StdString().c_str());
    writer->SetInputData(VTKMesh);
    
    try
    {
        writer->Write();
    }
    catch (...)
    {
        MDK_Error(" Can not write data @ SavePolygonMeshAsVTKFile(...) ")
        return false;
    }

    return true;
}


template<typename ScalarType>
PolygonMesh<ScalarType> LoadPolygonMeshFromVTKFile(const CharString& FilePathAndName)
{
    PolygonMesh<ScalarType> OutputMesh;

    auto Reader = vtkSmartPointer<vtkPolyDataReader>::New();
    Reader->SetFileName(FilePathAndName.StdString().c_str());

    try
    {
        Reader->Update();
    }
    catch (...)
    {
        MDK_Error(" Can not write data @ SaveTriangleMeshAsVTKFile(...) ")        
        return OutputMesh;
    }

    auto VTKPolyMesh = Reader->GetOutput();
    OutputMesh = ConvertVTKPolyDataToMDKPolygonMesh<ScalarType>(VTKPolyMesh);
    return OutputMesh;
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

    auto OutputElementTypeName = FindScalarTypeName(ReferenceScalar);

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

    std::string InputElementTypeName;

    auto it = HeaderObject.find("ElementType");
    if (it != HeaderObject.end())
    {
        InputElementTypeName = it.value().toString().toStdString();
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