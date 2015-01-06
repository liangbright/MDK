#ifndef __mdkITKDataStructureConversion_hpp
#define __mdkITKDataStructureConversion_hpp


namespace mdk
{

template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(DenseImage3D<PixelType>& MDKImage, bool SharePixelData)
{
	if (MDKImage.IsEmpty() == true)
	{
		MDK_Warning("MDKImage is empty @ mdkFileIO ConvertMDK3DScalarImageToITK3DScalarImage(...)")
		return nullptr;
	}

    auto InputSize        = MDKImage.GetSize();
    auto InputOrigin      = MDKImage.GetOrigin();
    auto InputSpacing     = MDKImage.GetSpacing();
    auto InputPixelNumber = MDKImage.GetPixelNumber();

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
	auto ITKImage = itk::SmartPointer<itk::Image<PixelType, 3>>::New();
	ConvertMDK3DScalarImageToITK3DScalarImage(MDKImage, ITKImage.GetPointer());
	return ITKImage;
}


template<typename PixelType>
bool ConvertMDK3DScalarImageToITK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, itk::Image<PixelType, 3>& ITKImage)
{
	return ConvertMDK3DScalarImageToITK3DScalarImage(MDKImage, &ITKImage);
}


template<typename PixelType>
bool ConvertMDK3DScalarImageToITK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, itk::Image<PixelType, 3>* ITKImage)
{
	if (MDKImage.IsEmpty() == true)
	{
		MDK_Warning("MDKImage is empty @ mdkFileIO ConvertMDK3DScalarImageToITK3DScalarImage(...)")
		return nullptr;
	}

    bool SharePixelData = true;
    auto importFilter = ConvertMDK3DScalarImageToITK3DScalarImage(MDKImage, SharePixelData);

    typedef itk::Image<PixelType, 3>  ITKImageType;
    typedef itk::ImageDuplicator<ITKImageType> DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetMDKImage(importFilter->GetOutput());
    duplicator->Update();

    ITKImageType::Pointer ITKImage = duplicator->GetOutput();
    return ITKImage;
}


template<typename PixelType>
bool ConvertITK3DScalarImageToMDK3DScalarImage(const itk::Image<PixelType, 3>& ITKImage, DenseImage3D<PixelType>& MDKImage)
{
	return ConvertITK3DScalarImageToMDK3DScalarImage(&ITKImage, MDKImage);
}


template<typename PixelType>
bool ConvertITK3DScalarImageToMDK3DScalarImage(const itk::Image<PixelType, 3>* ITKImage, DenseImage3D<PixelType>& MDKImage)
{
    if (ITKImage == nullptr)
    {
        MDK_Error("Invalid input @ ConvertITK3DScalarImageToMDK3DScalarImage(...)")
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

	auto PtrOfMDKImage = MDKImage.GetPixelPointer();

    auto PtrOfITKImage = ITKImage->GetBufferPointer();

	for (int_max i = 0; i < MDKImage.GetPixelNumber(); ++i)
    {
		PtrOfMDKImage[i] = PtrOfITKImage[i];
    }

    return true;
}

}// namespace mdk


#endif