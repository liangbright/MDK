#ifndef __mdkFileIO_Image3D_hpp
#define __mdkFileIO_Image3D_hpp

namespace mdk
{

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const Image3D<PixelType>& InputImage, const std::string& JsonFilePathAndName)
{
	if (Save3DScalarImageAsJsonDataFile_Header(InputImage, JsonFilePathAndName) == false)
	{
		return false;
	}
	std::string DataFilePathAndName = JsonFilePathAndName + ".data";
	return Save3DScalarImageAsJsonDataFile_Data(InputImage, DataFilePathAndName);
}


template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile_Header(const Image3D<PixelType>& InputImage, const std::string& JsonFilePathAndName)
{
	int_max ByteNumber = GetByteNumberOfScalar(PixelType(0));
    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ Save3DScalarImageAsJsonDataFile_Header(...)")
        return false;
    }
    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList;
    PairList.resize(6);

    PairList[0].Name = "ObjectType";
    PairList[0].Value = "Image3D";

	auto ElementTypeName = GetScalarTypeName(PixelType(0));
	QString QElementTypeName(ElementTypeName.c_str());
    PairList[1].Name = "PixelType";
    PairList[1].Value = QElementTypeName;

    auto Size = InputImage.GetSize();
    PairList[2].Name = "Size";
    PairList[2].Value = QString::number(Size.Lx) + ", " + QString::number(Size.Ly) + ", " + QString::number(Size.Lz);

    auto Spacing = InputImage.GetSpacing();

    PairList[3].Name = "Spacing";
    PairList[3].Value = QString::number(Spacing.Sx) + ", " + QString::number(Spacing.Sy) + ", " + QString::number(Spacing.Sz);

    auto Origin = InputImage.GetOrigin();
    PairList[4].Name = "Origin";
    PairList[4].Value = QString::number(Origin.x) + ", " + QString::number(Origin.y) + ", " + QString::number(Origin.z);

    auto Orientation = InputImage.GetOrientation();
    PairList[5].Name = "Orientation";
    PairList[5].Value =   QString::number(Orientation(0,0)) + ","
                        + QString::number(Orientation(1,0)) + ","
                        + QString::number(Orientation(2,0)) + ","
                        + QString::number(Orientation(0,1)) + ","
                        + QString::number(Orientation(1,1)) + ","
                        + QString::number(Orientation(2,1)) + ","
                        + QString::number(Orientation(0,2)) + ","
                        + QString::number(Orientation(1,2)) + ","
                        + QString::number(Orientation(2,2));

    // write header file (json)
	QString QFilePathAndName(JsonFilePathAndName.c_str());
	return SaveNameValuePairListAsJsonFile(PairList, QFilePathAndName);
}


template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile_Data(const Image3D<PixelType>& InputImage, const std::string& DataFilePathAndName)
{
	QString QFilePathAndName(DataFilePathAndName.c_str());
	QFile DataFile(QFilePathAndName);

	if (!DataFile.open(QIODevice::WriteOnly))
	{
		MDK_Error("Couldn't open file to write image data @ Save3DScalarImageAsJsonDataFile_Data(...)")
		return false;
	}

	auto PixelNumber = InputImage.GetPixelNumber();
	auto ByteNumber = GetByteNumberOfScalar(PixelType(0));

	DataFile.write((char*)InputImage.GetPixelPointer(), PixelNumber*ByteNumber);
	DataFile.flush();
	DataFile.close();

	return true;
}


template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromJsonDataFile(const std::string& JsonFilePathAndName)
{
    Image3D<PixelType> OutputImage;
    //---------------------------------------------- Read header --------------------------------------------------------//
	QString QFilePathAndName(JsonFilePathAndName.c_str());
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

    auto it = HeaderObject.find("ObjectType");
    if (it != HeaderObject.end())
    {
		auto ObjectType = it.value().toString();

		if (ObjectType != "Image3D")
        {
            MDK_Error("ObjectType is not Image3D @ Load3DScalarImageFromJsonDataFile(...)")
            HeaderFile.close();
            return OutputImage;
        }
    }
    else
    {
        MDK_Error("Couldn't get ObjectType @ Load3DScalarImageFromJsonDataFile(...)")
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
    //---------------------------------------------
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
	//--------------------------------------
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
	//------------------------------------
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
    //-------------------------------
    HeaderFile.close();

    //-------------------------------------------------- Read data ---------------------------------------------------------//

    // allocate memory
    OutputImage.SetSize(Size);
    OutputImage.SetSpacing(Spacing);
    OutputImage.SetOrigin(Origin);
    OutputImage.SetOrientation(Orientation);

	std::string DataFilePathAndName = JsonFilePathAndName + ".data";
	auto OutputPixelTypeName = GetScalarTypeName(PixelType(0));	
    if (OutputPixelTypeName == InputPixelTypeName)
    {
		Load3DScalarImageFromJsonDataFile_Data<PixelType, PixelType>(OutputImage, DataFilePathAndName);
    }
	else
	{
		MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ Load3DScalarImageFromJsonDataFile(...)")

		Load3DScalarImageFromJsonDataFile_Data(OutputImage, InputPixelTypeName, DataFilePathAndName);
	}

    return OutputImage;
}


template<typename OutputPixelType>
void Load3DScalarImageFromJsonDataFile_Data(Image3D<OutputPixelType>& OutputImage, const std::string& InputPixelTypeName, const std::string& DataFilePathAndName)
{
    if (InputPixelTypeName == "double")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, double>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "float")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, float>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "int8")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, int8>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "int16")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, int16>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "int32")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, int32>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "int64")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, int64>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "uint8")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, uint8>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "uint16")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, uint16>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "uint32")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, uint32>(OutputImage, DataFilePathAndName);
    }
    else if (InputPixelTypeName == "uint64")
    {
		Load3DScalarImageFromJsonDataFile_Data<OutputPixelType, uint64>(OutputImage, DataFilePathAndName);
    }
    else
    {
        MDK_Error("unknown PixelType of data file @ Internal_Load3DScalarImageFromJsonDataFile(...) ")
        OutputImage.Clear();
    }
}


template<typename OutputPixelType, typename InputPixelType>
void Load3DScalarImageFromJsonDataFile_Data(Image3D<OutputPixelType>& OutputImage, const std::string& DataFilePathAndName)
{
	QString QFilePathAndName(DataFilePathAndName.c_str());
	QFile DataFile(QFilePathAndName);
	if (!DataFile.open(QIODevice::ReadOnly))
	{
		MDK_Error("Couldn't open data file:" << FilePathAndName)
		OutputImage.Clear();
		return;
	}

	int_max BypesofDataFile = int_max(DataFile.size());
	int_max PixelNumber = OutputImage.GetPixelNumber();
	int_max ByteNumberOfOutputPixelType = GetByteNumberOfScalar(PixelType(0));
    if (BypesofDataFile != PixelNumber * ByteNumberOfOutputPixelType)
    {
        MDK_Error("Data file size is not equal to image size @ Load3DScalarImageFromJsonDataFile_Data(...)")
        DataFile.close();
        OutputImage.Clear();
        return;
    }

    auto PixelPointer = OutputImage.GetPixelPointer();

    for (int_max i = 0; i < PixelNumber; ++i)
    {
		auto tempScalar = InputPixelType(0);
        DataFile.read((char*)(&tempScalar), ByteNumberOfOutputPixelType);
        PixelPointer[i] = OutputPixelType(tempScalar);
    }
	DataFile.close();
}


template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromDICOMSeries(const std::string& FilePath)
{
    Image3D<PixelType> OutputImage;

    typedef itk::Image< PixelType, 3 >                ITKImageType;
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
        std::cerr << "Exception thrown while reading the dicom series @ Load3DScalarImageFromDICOMSeries(...)" << std::endl;
        std::cerr << excp << std::endl;
        return OutputImage;
    }

    OutputImage = ConvertITK3DScalarImageToMDK3DScalarImage(ITKImageReader->GetOutput());

    return OutputImage;
}


template<typename PixelType>
Image3D<PixelType> Load3DScalarImageFromSingleDICOMFile(const std::string& FilePathAndName)
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

}//namespace mdk

#endif