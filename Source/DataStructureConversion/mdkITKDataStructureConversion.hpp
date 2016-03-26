#pragma once


namespace mdk
{
//------------------------------------------------------------- MDK-3D to ITK-3D -----------------------------------------------------------//
template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DScalarImageByITKImportImageFilter(DenseImage3D<PixelType>& MDKImage, bool SharePixelData)
{
	if (MDKImage.IsEmpty() == true)
	{
		MDK_Warning("MDKImage is empty @ ConvertMDK3DScalarImageByITKImportImageFilter(...)")
		return nullptr;
	}

    auto InputSize        = MDKImage.GetSize();
    auto InputOrigin      = MDKImage.GetOrigin();
    auto InputSpacing     = MDKImage.GetSpacing();
    auto InputPixelNumber = MDKImage.GetPixelCount();

    typedef itk::Image<PixelType, 3> ITKImageType;
    typedef itk::ImportImageFilter<PixelType, 3> ITKImportFilterType;

    ITKImportFilterType::Pointer importFilter = ITKImportFilterType::New();

    ITKImportFilterType::SizeType size;
    size[0] = InputSize[0];
    size[1] = InputSize[1];
    size[2] = InputSize[2];

    ITKImportFilterType::IndexType start;
    start.Fill(0);
    ITKImportFilterType::RegionType region;
    region.SetIndex(start);
    region.SetSize(size);
    importFilter->SetRegions(region);

    const itk::SpacePrecisionType origin[3] = { InputOrigin[0], InputOrigin[1], InputOrigin[2] };
    importFilter->SetOrigin(origin);

    const itk::SpacePrecisionType spacing[3] = { InputSpacing[0], InputSpacing[1], InputSpacing[2] };
    importFilter->SetSpacing(spacing);

    ITKImportFilterType::DirectionType Direction;
    typedef itk::SpacePrecisionType SpacePrecisionType;

    const DenseMatrix<double>& Orientation = MDKImage.GetOrientation();
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
        importFilter->SetImportPointer(MDKImage.GetPixelPointer(), InputPixelNumber, ITKImageWillOwnPixelDataArray);
    }
    else
    {
        PixelType* PixelDataArray = new PixelType[InputPixelNumber];
        for (int_max i = 0; i < InputPixelNumber; ++i)
        {
            PixelDataArray[i] = MDKImage[i];
        }

        bool ITKImageWillOwnPixelDataArray = true;
        importFilter->SetImportPointer(PixelDataArray, InputPixelNumber, ITKImageWillOwnPixelDataArray);
    }

    importFilter->Update();

    return importFilter;
}


template<typename PixelType>
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(DenseImage3D<PixelType>& MDKImage)
{
	typedef itk::Image<PixelType, 3> ITKImageType;
	itk::SmartPointer<ITKImageType> ITKImage = ITKImageType::New();
	ITKImageType * Ptr = ITKImage.GetPointer();
	ConvertMDK3DScalarImageToITK3DScalarImage(MDKImage, Ptr);
	return ITKImage;
}


template<typename PixelType>
bool ConvertMDK3DScalarImageToITK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, itk::Image<PixelType, 3>* ITKImage)
{
	if (ITKImage == nullptr)
	{
		MDK_Error("ITKImage is nullptr @ ConvertITK3DScalarImageToMDK3DScalarImage(...)")
		return false;
	}

	if (MDKImage.IsEmpty() == true)
	{
		MDK_Warning("MDKImage is empty @ ConvertMDK3DScalarImageToITK3DScalarImage(...)")		
	}

	auto InputSize = MDKImage.GetSize();
	auto InputOrigin = MDKImage.GetOrigin();
	auto InputSpacing = MDKImage.GetSpacing();

	typedef itk::Image<PixelType, 3> ITKImageType;

	ITKImageType::SizeType size;
	size[0] = InputSize[0];
	size[1] = InputSize[1];
	size[2] = InputSize[2];

	ITKImageType::IndexType start;
	start.Fill(0);
	ITKImageType::RegionType region;
	region.SetIndex(start);
	region.SetSize(size);
	ITKImage->SetRegions(region);

	const itk::SpacePrecisionType origin[3] = { InputOrigin[0], InputOrigin[1], InputOrigin[2] };
	ITKImage->SetOrigin(origin);

	const itk::SpacePrecisionType spacing[3] = { InputSpacing[0], InputSpacing[1], InputSpacing[2] };
	ITKImage->SetSpacing(spacing);

	ITKImageType::DirectionType Direction;
	typedef itk::SpacePrecisionType SpacePrecisionType;

	const DenseMatrix<double>& Orientation = MDKImage.GetOrientation();
	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			Direction(i, j) = SpacePrecisionType(Orientation(i, j));
		}
	}
	ITKImage->SetDirection(Direction);

	ITKImage->Allocate();

	auto Ptr_MDKImage = MDKImage.GetPixelPointer();

	auto Ptr_ITKImage = ITKImage->GetBufferPointer();

	for (int_max i = 0; i < MDKImage.GetPixelCount(); ++i)
	{
		Ptr_ITKImage[i] = Ptr_MDKImage[i];
	}

	return true;
}

//------------------------------------------------- MDK-2D to ITK-3D -------------------------------------------------//

template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK2DScalarImageByITKImportImageFilter(DenseImage2D<PixelType>& MDKImage, bool SharePixelData)
{
	if (MDKImage.IsEmpty() == true)
	{
		MDK_Warning("MDKImage is empty @ ConvertMDK2DScalarImageByITKImportImageFilter(...)")
		return nullptr;
	}

	auto InputSize = MDKImage.GetSize();
	auto InputOrigin = MDKImage.GetOrigin();
	auto InputSpacing = MDKImage.GetSpacing();
	auto InputPixelNumber = MDKImage.GetPixelCount();

	typedef itk::Image<PixelType, 3> ITKImageType;
	typedef itk::ImportImageFilter<PixelType, 3> ITKImportFilterType;

	ITKImportFilterType::Pointer importFilter = ITKImportFilterType::New();

	ITKImportFilterType::SizeType size;
	size[0] = InputSize[0];
	size[1] = InputSize[1];
	size[2] = 1;

	ITKImportFilterType::IndexType start;
	start.Fill(0);
	ITKImportFilterType::RegionType region;
	region.SetIndex(start);
	region.SetSize(size);
	importFilter->SetRegions(region);

	const itk::SpacePrecisionType origin[3] = { InputOrigin[0], InputOrigin[1], InputOrigin[2] };
	importFilter->SetOrigin(origin);

	const itk::SpacePrecisionType spacing[3] = { InputSpacing[0], InputSpacing[1], 1.0};
	importFilter->SetSpacing(spacing);

	ITKImportFilterType::DirectionType Direction;
	typedef itk::SpacePrecisionType SpacePrecisionType;

	const DenseMatrix<double>& Orientation = MDKImage.GetOrientation();
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
		importFilter->SetImportPointer(MDKImage.GetPixelPointer(), InputPixelNumber, ITKImageWillOwnPixelDataArray);
	}
	else
	{
		PixelType* PixelDataArray = new PixelType[InputPixelNumber];
		for (int_max i = 0; i < InputPixelNumber; ++i)
		{
			PixelDataArray[i] = MDKImage[i];
		}

		bool ITKImageWillOwnPixelDataArray = true;
		importFilter->SetImportPointer(PixelDataArray, InputPixelNumber, ITKImageWillOwnPixelDataArray);
	}

	importFilter->Update();

	return importFilter;
}


template<typename PixelType>
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK2DScalarImageToITK3DScalarImage(DenseImage2D<PixelType>& MDKImage)
{
	typedef itk::Image<PixelType, 3> ITKImageType;
	itk::SmartPointer<ITKImageType> ITKImage = ITKImageType::New();
	ITKImageType * Ptr = ITKImage.GetPointer();
	ConvertMDK2DScalarImageToITK3DScalarImage(MDKImage, Ptr);
	return ITKImage;
}


template<typename PixelType>
bool ConvertMDK2DScalarImageToITK3DScalarImage(const DenseImage2D<PixelType>& MDKImage, itk::Image<PixelType, 3>* ITKImage)
{
	if (ITKImage == nullptr)
	{
		MDK_Error("ITKImage is nullptr @ ConvertMDK2DScalarImageToITK2DScalarImage(...)")
		return false;
	}

	if (MDKImage.IsEmpty() == true)
	{
		MDK_Warning("MDKImage is empty @ ConvertMDK2DScalarImageToITK2DScalarImage(...)")
	}

	auto InputSize = MDKImage.GetSize();
	auto InputOrigin = MDKImage.GetOrigin();
	auto InputSpacing = MDKImage.GetSpacing();

	typedef itk::Image<PixelType, 3> ITKImageType;

	ITKImageType::SizeType size;
	size[0] = InputSize[0];
	size[1] = InputSize[1];
	size[2] = 1;

	ITKImageType::IndexType start;
	start.Fill(0);
	ITKImageType::RegionType region;
	region.SetIndex(start);
	region.SetSize(size);
	ITKImage->SetRegions(region);

	const itk::SpacePrecisionType origin[3] = { InputOrigin[0], InputOrigin[1], InputOrigin[2] };
	ITKImage->SetOrigin(origin);

	const itk::SpacePrecisionType spacing[3] = { InputSpacing[0], InputSpacing[1], 1.0};
	ITKImage->SetSpacing(spacing);

	ITKImageType::DirectionType Direction;
	typedef itk::SpacePrecisionType SpacePrecisionType;

	const DenseMatrix<double>& Orientation = MDKImage.GetOrientation();
	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			Direction(i, j) = SpacePrecisionType(Orientation(i, j));
		}
	}
	ITKImage->SetDirection(Direction);

	ITKImage->Allocate();

	auto Ptr_MDKImage = MDKImage.GetPixelPointer();

	auto Ptr_ITKImage = ITKImage->GetBufferPointer();

	for (int_max i = 0; i < MDKImage.GetPixelCount(); ++i)
	{
		Ptr_ITKImage[i] = Ptr_MDKImage[i];
	}

	return true;
}

//--------------------------------------------- ITK-3D to MDK-3D -----------------------------------------------------------------------//
template<typename PixelType>
bool ConvertITK3DScalarImageToMDK3DScalarImage(itk::Image<PixelType, 3>* ITKImage, DenseImage3D<PixelType>& MDKImage, bool SharePixelData)
{
	if (ITKImage == nullptr)
	{
		MDK_Error("ITKImage is nullptr @ ConvertITK3DScalarImageToMDK3DScalarImage(...)")
		return false;
	}

	if (SharePixelData == false)
	{
		return ConvertITK3DScalarImageToMDK3DScalarImage(ITKImage, MDKImage);
	}

	auto Size = ITKImage->GetBufferedRegion().GetSize();
	auto Spacing = ITKImage->GetSpacing();
	auto Origin = ITKImage->GetOrigin();
	auto Direction = ITKImage->GetDirection();

	ImageInfo3D Info;
	Info.Size[0] = Size[0];
	Info.Size[1] = Size[1];
	Info.Size[2] = Size[2];
	Info.Spacing[0] = Spacing[0];
	Info.Spacing[1] = Spacing[1];
	Info.Spacing[2] = Spacing[2];
	Info.Origin[0] = Origin[0];
	Info.Origin[1] = Origin[1];
	Info.Origin[2] = Origin[2];
	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			Info.Orientation(i, j) = double(Direction(i, j));
		}
	}	
	MDKImage.Share(ITKImage->GetBufferPointer(), Info);
}


template<typename PixelType>
bool ConvertITK3DScalarImageToMDK3DScalarImage(const itk::Image<PixelType, 3>* ITKImage, DenseImage3D<PixelType>& MDKImage)
{
    if (ITKImage == nullptr)
    {
        MDK_Error("ITKImage is nullptr @ ConvertITK3DScalarImageToMDK3DScalarImage(...)")
        return false;
    }

    auto Size = ITKImage->GetBufferedRegion().GetSize();
    auto Spacing = ITKImage->GetSpacing();
    auto Origin = ITKImage->GetOrigin();
    auto Direction = ITKImage->GetDirection();

	MDKImage.SetSize(Size[0], Size[1], Size[2]);
	MDKImage.SetSpacing(Spacing[0], Spacing[1], Spacing[2]);
	MDKImage.SetOrigin(Origin[0], Origin[1], Origin[2]);

    DenseMatrix<double> Orientation(3, 3);
    for (int j = 0; j < 3; ++j)
    {
        for (int i = 0; i < 3; ++i)
        {
            Orientation(i, j) = double(Direction(i, j));
        }
    }
	MDKImage.SetOrientation(Orientation);

	auto Ptr_MDKImage = MDKImage.GetPixelPointer();

	auto Ptr_ITKImage = ITKImage->GetBufferPointer();

	for (int_max i = 0; i < MDKImage.GetPixelCount(); ++i)
    {
		Ptr_MDKImage[i] = Ptr_ITKImage[i];
    }

    return true;
}

//------------------------------------------------- ITK-3D to MDK-2D ---------------------------------------------------------------------//
// ITKImage should be a Slice
template<typename PixelType>
bool ConvertITK3DScalarImageSliceToMDK2DScalarImage(itk::Image<PixelType, 3>* ITKImage, DenseImage2D<PixelType>& MDKImage, bool SharePixelData)
{
	if (ITKImage == nullptr)
	{
		MDK_Error("ITKImage is nullptr @ ConvertITK3DScalarImageToMDK2DScalarImage(...)")
		return false;
	}

	if (SharePixelData == false)
	{
		return ConvertITK3DScalarImageSliceToMDK2DScalarImage(ITKImage, MDKImage);
	}

	auto Size = ITKImage->GetBufferedRegion().GetSize();
	auto Spacing = ITKImage->GetSpacing();
	auto Origin = ITKImage->GetOrigin();
	auto Direction = ITKImage->GetDirection();

	if (Size[2] > 1)
	{
		MDK_Error("ITKImage is NOT a Slice in 3D Space @ ConvertITK3DScalarImageToMDK2DScalarImage(...)")
		return false;
	}

	ImageInfo2D Info;
	Info.Size[0] = Size[0];
	Info.Size[1] = Size[1];
	Info.Spacing[0] = Spacing[0];
	Info.Spacing[1] = Spacing[1];
	Info.Origin[0] = Origin[0];
	Info.Origin[1] = Origin[1];
	Info.Origin[2] = Origin[2];
	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			Info.Orientation(i, j) = double(Direction(i, j));
		}
	}
	MDKImage.Share(ITKImage->GetBufferPointer(), Info);
}


template<typename PixelType>
bool ConvertITK3DScalarImageSliceToMDK2DScalarImage(const itk::Image<PixelType, 3>* ITKImage, DenseImage2D<PixelType>& MDKImage)
{
	if (ITKImage == nullptr)
	{
		MDK_Error("ITKImage is nullptr @ ConvertITK3DScalarImageToMDK2DScalarImage(...)")
		return false;
	}

	auto Size = ITKImage->GetBufferedRegion().GetSize();
	auto Spacing = ITKImage->GetSpacing();
	auto Origin = ITKImage->GetOrigin();
	auto Direction = ITKImage->GetDirection();

	if (Size[2] > 1)
	{
		MDK_Error("ITKImage is NOT a Slice in 3D Space @ ConvertITK3DScalarImageToMDK2DScalarImage(...)")
		return false;
	}

	MDKImage.SetSize(Size[0], Size[1]);
	MDKImage.SetSpacing(Spacing[0], Spacing[1]);
	MDKImage.SetOrigin(Origin[0], Origin[1], Origin[2]);

	DenseMatrix<double> Orientation(3, 3);
	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			Orientation(i, j) = double(Direction(i, j));
		}
	}
	MDKImage.SetOrientation(Orientation);

	auto Ptr_MDKImage = MDKImage.GetPixelPointer();

	auto Ptr_ITKImage = ITKImage->GetBufferPointer();

	for (int_max i = 0; i < MDKImage.GetPixelCount(); ++i)
	{
		Ptr_MDKImage[i] = Ptr_ITKImage[i];
	}

	return true;
}

}// namespace mdk
