#ifndef __mdkDenseImage3D_FileIO_hpp
#define __mdkDenseImage3D_FileIO_hpp

namespace mdk
{

template<typename PixelType>
bool Save3DScalarImageAsJsonDataFile(const DenseImage3D<PixelType>& InputImage, const String& FilePathAndName)
{
	int_max ByteNumber = GetByteNumberOfScalar(PixelType(0));
    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ Save3DScalarImageAsJsonDataFile(...)")
        return false;
    }
    //-------------------------------------------------------------------------------------
	JsonObject JObject;

	JObject["ObjectType"] = "DenseImage3D";
	JObject["PixelType"] = GetScalarTypeName(PixelType(0));

    DenseVector<int_max> Size = InputImage.GetSize();
	JObject["Size"] = Size;

	DenseVector<double> Spacing = InputImage.GetSpacing();
	JObject["Spacing"] = Spacing;

	DenseVector<double> Origin = InputImage.GetOrigin();
	JObject["Origin"] = Origin;

    DenseMatrix<double> Orientation = InputImage.GetOrientation();
	JObject["Orientation"] = Orientation;

	String FilePath = ExtractFilePath(FilePathAndName);
	String DataFileName = ExtractFileName(FilePathAndName) + ".Pixel.data";
	JObject["PixelArray"] = DataFileName;

	bool IsOK = true;
	if (JsonFile::Save(JObject, FilePathAndName) == false)
	{
		IsOK = false;
	}
	if (SaveScalarArrayAsDataFile(InputImage.GetPixelPointer(), InputImage.GetPixelNumber(), FilePath + DataFileName) == false)
	{
		IsOK = false;
	}
	return IsOK;
}


template<typename PixelType>
bool Load3DScalarImageFromJsonDataFile(DenseImage3D<PixelType>& OutputImage, const String& FilePathAndName)
{
	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ Load3DScalarImageFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		MDK_Warning("Json file is empty @ Load3DScalarImageFromJsonDataFile(...)")
		return true;
	}
	//---------------------------------------------
	auto it = JObject.find("ObjectType");
	if (it != JObject.end())
    {
		auto ObjectType = it->second.GetString();
		if (ObjectType != "DenseImage3D")
        {
            MDK_Error("ObjectType is not DenseImage3D @ Load3DScalarImageFromJsonDataFile(...)")
			return false;
        }
    }
    else
    {
        MDK_Error("Couldn't get ObjectType @ Load3DScalarImageFromJsonDataFile(...)")
		return false;
    }
    //---------------------------------------------------
	String PixelTypeInDataFile;
	it = JObject.find("PixelType");
	if (it != JObject.end())
    {
		PixelTypeInDataFile = it->second.GetString();
    }
    else
    {
        MDK_Error("Couldn't get PixelType @ Load3DScalarImageFromJsonDataFile(...)")
		return false;
    }
    //---------------------------------------------------
	DenseVector<int_max, 3> Size;
	it = JObject.find("Size");
	if (it != JObject.end())
    {
        auto tempArray = it->second.ToScalarArray<int_max>();
		if (tempArray.GetElementNumber() != 3)
		{
			MDK_Error("Size vector is wrong @ Load3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Size[0] = tempArray[0];
		Size[1] = tempArray[1];
		Size[2] = tempArray[2];
    }
    else
    {
        MDK_Error("Couldn't get Size @ Load3DScalarImageFromJsonDataFile(...)")
		return false;
    }
    //---------------------------------------------
	DenseVector<double, 3> Spacing;
	it = JObject.find("Spacing");
	if (it != JObject.end())
    {
		auto tempArray = it->second.ToScalarArray<double>();
		if (tempArray.GetElementNumber() != 3)
		{
			MDK_Error("Spacing vector is wrong @ Load3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Spacing[0] = tempArray[0];
		Spacing[1] = tempArray[1];
		Spacing[2] = tempArray[2];
    }
    else
    {
        MDK_Error("Couldn't get Spacing @ Load3DScalarImageFromJsonDataFile(...)")
		return false;
    }
	//--------------------------------------
	DenseVector<double, 3> Origin;
	it = JObject.find("Origin");
	if (it != JObject.end())
    {
		auto tempArray = it->second.ToScalarArray<double>();
		if (tempArray.GetElementNumber() != 3)
		{
			MDK_Error("Origin vector is wrong @ Load3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Origin[0] = tempArray[0];
		Origin[1] = tempArray[1];
		Origin[2] = tempArray[2];
    }
    else
    {
        MDK_Error("Couldn't get Origin @ Load3DScalarImageFromJsonDataFile(...)")
		return false;
    }
	//------------------------------------
	DenseMatrix<double> Orientation;
	it = JObject.find("Orientation");
	if (it != JObject.end())
    {
		auto tempOrientation = it->second.ToScalarArray<double>();
		if (tempOrientation.GetElementNumber() != 9)
		{
			MDK_Error("Orientation size is wrong @ Load3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Orientation = tempOrientation;
		Orientation.Reshape(3, 3);
    }
    else
    {
        MDK_Error("Couldn't get Orientation @ Load3DScalarImageFromJsonDataFile(...)")
		return false;
    }
    
	//----------------- Read data ------------------

	String DataFilePathAndName = ExtractFilePath(FilePathAndName);
	it = JObject.find("PixelArray");
	if (it != JObject.end())
	{
		DataFilePathAndName += it->second.GetString();		
	}
	else
	{
		MDK_Error("Couldn't get PixelArray @ Load3DScalarImageFromJsonDataFile(...)")
		return false;
	}

	// allocate memory
	OutputImage.SetSize(Size);
	OutputImage.SetSpacing(Spacing);
	OutputImage.SetOrigin(Origin);
	OutputImage.SetOrientation(Orientation);

	if (LoadScalarArrayFromDataFile(OutputImage.GetPixelPointer(), OutputImage.GetPixelNumber(), DataFilePathAndName, PixelTypeInDataFile) == false)
	{
		OutputImage.Clear();
		return false;
	}
	return true;  
}


template<typename PixelType>
bool Load3DScalarImageFromDICOMSeries(DenseImage3D<PixelType>& OutputImage, const String& FilePath)
{
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
        return false;
    }

	return ConvertITK3DScalarImageToMDK3DScalarImage(ITKImageReader->GetOutput(), OutputImage);
}


template<typename PixelType>
bool Load3DScalarImageFromSingleDICOMFile(DenseImage3D<PixelType>& OutputImage, const String& FilePathAndName)
{
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
        return false;
    }

	return ConvertITK3DScalarImageToMDK3DScalarImage(ITKImageReader->GetOutput(), OutputImage);
}

}//namespace mdk

#endif