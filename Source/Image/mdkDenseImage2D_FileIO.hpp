#pragma once

namespace mdk
{

template<typename PixelType>
bool Save2DScalarImageAsJsonDataFile(const DenseImage2D<PixelType>& InputImage, const String& FilePathAndName)
{
	int_max ByteCount = GetByteCountOfScalar(PixelType(0));
    if (ByteCount <= 0)
    {
        MDK_Error("Unknown type of image @ Save2DScalarImageAsJsonDataFile(...)")
        return false;
    }
    //-------------------------------------------------------------------------------------
	JsonObject JObject;

	JObject["ObjectType"] = "DenseImage2D";
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
	if (SaveScalarArrayAsDataFile(InputImage.GetPixelPointer(), InputImage.GetPixelCount(), FilePath + DataFileName) == false)
	{
		IsOK = false;
	}
	return IsOK;
}


template<typename PixelType>
bool Load2DScalarImageFromJsonDataFile(DenseImage2D<PixelType>& OutputImage, const String& FilePathAndName)
{
	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ Load2DScalarImageFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		MDK_Warning("Json file is empty @ Load2DScalarImageFromJsonDataFile(...)")
		return true;
	}
	//---------------------------------------------
	auto it = JObject.find("ObjectType");
	if (it != JObject.end())
    {
		auto ObjectType = it->second.GetString();
		if (ObjectType != "DenseImage2D")
        {
            MDK_Error("ObjectType is not DenseImage2D @ Load2DScalarImageFromJsonDataFile(...)")
			return false;
        }
    }
    else
    {
        MDK_Error("Couldn't get ObjectType @ Load2DScalarImageFromJsonDataFile(...)")
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
        MDK_Error("Couldn't get PixelType @ Load2DScalarImageFromJsonDataFile(...)")
		return false;
    }
    //---------------------------------------------------
	DenseVector<int_max, 2> Size;
	it = JObject.find("Size");
	if (it != JObject.end())
    {
        auto tempArray = it->second.ToScalarArray<int_max>();
		if (tempArray.GetElementCount() != 2)
		{
			MDK_Error("Size vector is wrong @ Load2DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Size[0] = tempArray[0];
		Size[1] = tempArray[1];
    }
    else
    {
        MDK_Error("Couldn't get Size @ Load2DScalarImageFromJsonDataFile(...)")
		return false;
    }
    //---------------------------------------------
	DenseVector<double, 2> Spacing;
	it = JObject.find("Spacing");
	if (it != JObject.end())
    {
		auto tempArray = it->second.ToScalarArray<double>();
		if (tempArray.GetElementCount() != 2)
		{
			MDK_Error("Spacing vector is wrong @ Load2DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Spacing[0] = tempArray[0];
		Spacing[1] = tempArray[1];
    }
    else
    {
        MDK_Error("Couldn't get Spacing @ Load2DScalarImageFromJsonDataFile(...)")
		return false;
    }
	//--------------------------------------
	DenseVector<double, 3> Origin;
	it = JObject.find("Origin");
	if (it != JObject.end())
    {
		auto tempArray = it->second.ToScalarArray<double>();
		if (tempArray.GetElementCount() != 3)
		{
			MDK_Error("Origin vector is wrong @ Load2DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Origin[0] = tempArray[0];
		Origin[1] = tempArray[1];
		Origin[2] = tempArray[2];
    }
    else
    {
        MDK_Error("Couldn't get Origin @ Load2DScalarImageFromJsonDataFile(...)")
		return false;
    }
	//------------------------------------
	DenseMatrix<double> Orientation;
	it = JObject.find("Orientation");
	if (it != JObject.end())
    {
		auto tempOrientation = it->second.ToScalarArray<double>();
		if (tempOrientation.GetElementCount() != 9)
		{
			MDK_Error("Orientation size is wrong @ Load2DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Orientation = tempOrientation;
		Orientation.Reshape(3, 3);
    }
    else
    {
        MDK_Error("Couldn't get Orientation @ Load2DScalarImageFromJsonDataFile(...)")
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
		MDK_Error("Couldn't get PixelArray @ Load2DScalarImageFromJsonDataFile(...)")
		return false;
	}

	OutputImage.SetSize(Size);	// allocate memory
	OutputImage.SetSpacing(Spacing);
	OutputImage.SetOrigin(Origin);
	OutputImage.SetOrientation(Orientation);

	if (LoadScalarArrayFromDataFile(OutputImage.GetPixelPointer(), OutputImage.GetPixelCount(), DataFilePathAndName, PixelTypeInDataFile) == false)
	{
		OutputImage.Clear();
		return false;
	}
	return true;  
}


template<typename PixelType>
bool Load2DScalarImageFromSingleDICOMFile(DenseImage2D<PixelType>& OutputImage, const String& FilePathAndName)
{
    typedef itk::Image<PixelType, 3>  ITKImageType;
	typedef itk::ImageFileReader<ITKImageType> ITKImageReaderType;

    ITKImageReaderType::Pointer  ITKImageReader = ITKImageReaderType::New();

    ITKImageReader->SetFileName(FilePathAndName.StdString());

    try
    {
        ITKImageReader->Update();
    }
    catch (itk::ExceptionObject & err)
    {
        std::cerr << "ExceptionObject caught while reading the DICOM file @ Load2DScalarImageFromSingleDICOMFile(...)" << std::endl;
        std::cerr << err << std::endl;
        return false;
    }

	return ConvertITK3DScalarImageSliceToMDK2DScalarImage(ITKImageReader->GetOutput(), OutputImage);
}

}//namespace mdk
