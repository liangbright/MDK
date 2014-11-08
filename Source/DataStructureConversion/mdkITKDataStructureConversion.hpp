#ifndef __mdkITKDataStructureConversion_hpp
#define __mdkITKDataStructureConversion_hpp


namespace mdk
{

template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(DenseImage3D<PixelType>& InputImage, bool SharePixelData)
{
    auto InputSize        = InputImage.GetSize();
    auto InputOrigin      = InputImage.GetOrigin();
    auto InputSpacing     = InputImage.GetSpacing();
    auto InputPixelNumber = InputImage.GetPixelNumber();

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
    importFilter->SetRegion(region);

    const itk::SpacePrecisionType origin[3] = { InputOrigin[0], InputOrigin[1], InputOrigin[2] };
    importFilter->SetOrigin(origin);

    const itk::SpacePrecisionType spacing[3] = { InputSpacing[0], InputSpacing[1], InputSpacing[2] };
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
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(DenseImage3D<PixelType>& InputImage)
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