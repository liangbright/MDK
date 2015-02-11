#ifndef mdk_ITKDataObject_FileIO_hpp
#define mdk_ITKDataObject_FileIO_hpp

namespace mdk
{

template<typename PixelType>
bool LoadITK3DScalarImageFromJsonDataFile(itk::Image<PixelType, 3>& OutputITKImage, const String& FilePathAndName)
{
	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ LoadITK3DScalarImageFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		MDK_Warning("Json file is empty @ LoadITK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//---------------------------------------------
	auto it = JObject.find("ObjectType");
	if (it != JObject.end())
	{
		auto ObjectType = it->second.GetString();
		if (ObjectType != "DenseImage3D")
		{
			MDK_Error("ObjectType is not DenseImage3D @ LoadITK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
	}
	else
	{
		MDK_Error("Couldn't get ObjectType @ LoadITK3DScalarImageFromJsonDataFile(...)")
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
		MDK_Error("Couldn't get PixelType @ LoadITK3DScalarImageFromJsonDataFile(...)")
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
			MDK_Error("Size vector is wrong @ LoadITK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Size[0] = tempArray[0];
		Size[1] = tempArray[1];
		Size[2] = tempArray[2];
	}
	else
	{
		MDK_Error("Couldn't get Size @ LoadITK3DScalarImageFromJsonDataFile(...)")
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
			MDK_Error("Spacing vector is wrong @ LoadITK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Spacing[0] = tempArray[0];
		Spacing[1] = tempArray[1];
		Spacing[2] = tempArray[2];
	}
	else
	{
		MDK_Error("Couldn't get Spacing @ LoadITK3DScalarImageFromJsonDataFile(...)")
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
			MDK_Error("Origin vector is wrong @ LoadITK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Origin[0] = tempArray[0];
		Origin[1] = tempArray[1];
		Origin[2] = tempArray[2];
	}
	else
	{
		MDK_Error("Couldn't get Origin @ LoadITK3DScalarImageFromJsonDataFile(...)")
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
			MDK_Error("Orientation size is wrong @ LoadITK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Orientation = tempOrientation;
		Orientation.Reshape(3, 3);
	}
	else
	{
		MDK_Error("Couldn't get Orientation @ LoadITK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//------------------------------------
	String DataFilePathAndName = ExtractFilePath(FilePathAndName);
	it = JObject.find("PixelArray");
	if (it != JObject.end())
	{
		DataFilePathAndName += it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get PixelArray @ LoadITK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//--------------------------------------------------
	typedef itk::Image<PixelType, 3> ITKImageType;

	ITKImageType::SizeType Size_itk;
	Size_itk[0] = Size[0];
	Size_itk[1] = Size[1];
	Size_itk[2] = Size[2];

	ITKImageType::IndexType start;
	start.Fill(0);

	ITKImageType::RegionType region;
	region.SetIndex(start);
	region.SetSize(Size_itk);

	OutputITKImage.SetOrigin(Origin.GetElementPointer());
	OutputITKImage.SetSpacing(Spacing.GetElementPointer());
	OutputITKImage.SetRegions(region);
	OutputITKImage.Allocate();

	int_max OutputPixelNumber = Size[0] * Size[1] * Size[2];

	auto PtrOfITKImage = OutputITKImage.GetBufferPointer();
	if (LoadScalarArrayFromDataFile(PtrOfITKImage, OutputPixelNumber, DataFilePathAndName, PixelTypeInDataFile) == false)
	{
		OutputITKImage.Delete();
	}
	return true;
}

template<typename PixelType>
bool LoadITK3DScalarImageFromJsonDataFile(itk::Image<PixelType, 3>* OutputITKImage, const String& FilePathAndName)
{
	if (OutputITKImage == nullptr)
	{
		MDK_Error("invlaid input: nullptr @ LoadITK3DScalarImageFromJsonDataFile(...)")
		return false;
	}

	return LoadITK3DScalarImageFromJsonDataFile(*OutputITKImage, FilePathAndName);
}

}// namespace mdk


#endif