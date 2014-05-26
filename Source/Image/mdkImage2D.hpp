#ifndef __mdkImage2D_hpp
#define __mdkImage2D_hpp


namespace mdk
{

template<typename PixelType>
ImageData2D<PixelType>::ImageData2D()
{
    this->Clear();
}


template<typename PixelType>
ImageData2D<PixelType>::~ImageData2D()
{
}


template<typename PixelType>
void ImageData2D<PixelType>::Clear()
{
    m_Size[0] = 0;
    m_Size[1] = 0;

    m_Origin[0] = 0;
    m_Origin[1] = 0;

    m_Spacing[0] = 0;
    m_Spacing[1] = 0;

    m_Orientation.FastResize(2, 2);
    m_Orientation.FixSize();
    m_Orientation.FillDiangonal(1.0);

    m_DataArray.clear();
    m_DataArray.shrink_to_fit();

    m_NaNPixel = GetNaNElement(m_NaNPixel);
}


template<typename PixelType>
inline 
PixelType& ImageData2D<PixelType>::operator[](int_max LinearIndex)
{ 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& ImageData2D<PixelType>::operator[](int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
PixelType& ImageData2D<PixelType>::operator()(int_max LinearIndex)
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& ImageData2D<PixelType>::operator()(int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}
   

template<typename PixelType>
inline
PixelType& ImageData2D<PixelType>::operator()(int_max xIndex, int_max yIndex)
{
    auto LinearIndex = yIndex*m_Size[0] + xIndex;
 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& ImageData2D<PixelType>::operator()(int_max xIndex, int_max yIndex) const
{
    auto LinearIndex = yIndex*m_Size[0] + xIndex;

    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
int_max ImageData2D<PixelType>::Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const
{
    return yIndex*m_Size[0] + xIndex;    
}


template<typename PixelType>
inline 
void ImageData2D<PixelType>::TransformLinearIndexTo2DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex) const
{
    std::lldiv_t divresult = div(LinearIndex, m_Size[0]);

    yIndex = divresult.quot;

    xIndex = divresult.rem;
}


template<typename PixelType>
inline 
void ImageData2D<PixelType>::TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex, double& x, double& y) const
{       
    std::lldiv_t divresult = div(LinearIndex, m_Size[0]);

    y = double(divresult.quot);

    x = double(divresult.rem);

    x = m_Origin[0] + x * m_Spacing[0];

    y = m_Origin[1] + y * m_Spacing[1];
}


template<typename PixelType>
inline
void ImageData2D<PixelType>::Transform2DIndexTo2DPhysicalPosition(int_max xIndex, int_max yIndex, double& x, double& y) const
{
    x = m_Origin[0] + double(xIndex) * m_Spacing[0];

    y = m_Origin[1] + double(yIndex) * m_Spacing[1];
}


template<typename PixelType>
inline 
void ImageData2D<PixelType>::Transform2DPhysicalPositionToContinuous2DIndex(double x, double y, double& xIndex, double& yIndex) const
{
    xIndex = (x - m_Origin[0]) / m_Spacing[0];

    yIndex = (y - m_Origin[1]) / m_Spacing[1];
}

//========================================================== Image ========================================================================//

template<typename PixelType>
Image2D<PixelType>::Image2D()
{
    m_ImageData = std::make_shared<ImageData2D<PixelType>>();

    this->Clear();
}


template<typename PixelType>
Image2D<PixelType>::Image2D(const Image2D& InputImage)
{
    m_ImageData = std::make_shared<ImageData2D<PixelType>>();

    this->Clear();

    this->Copy(InputImage);
}


template<typename PixelType>
Image2D<PixelType>::Image2D(Image2D<PixelType>&& InputImage)
{
    m_ImageData = std::move(InputImage.m_ImageData);

    m_PixelPointer = m_ImageData->m_DataArray.data();

    // InputImage may not be destructed
    InputImage.m_PixelPointer = nullptr;
}


template<typename PixelType>
Image2D<PixelType>::~Image2D()
{
}


template<typename PixelType>
void Image2D<PixelType>::operator=(const Image2D<PixelType>& InputImage)
{
    this->Copy(InputImage);
}


template<typename PixelType>
void Image2D<PixelType>::operator=(Image2D<PixelType>&& InputImage)
{
    if (!m_ImageData)
    {
        m_ImageData = std::make_shared<ImageData3D<PixelType>>();

        this->Clear();
    }

    this->Take(InputImage);

    InputImage.Clear();
}


template<typename PixelType>
template<typename PixelType_Input>
void Image2D<PixelType>::Copy(const Image2D<PixelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        MDK_Warning("try to Copy self @ 2DImage::Copy(InputImage)")
        return;
    }

	if (InputImage.IsEmpty() == true)
	{
        if (this->IsEmpty() == true)
        {
            return;
        }
        else
        {
            this->Clear();
            return;
        }
	}

    this->CopyData(InputImage.GetPixelPointer(), InputImage.GetPixelNumber());
    this->SetSize(InputImage.GetSize());
    this->SetSpacing(InputImage.GetSpacing());
    this->SetOrigin(InputImage.GetOrigin);
    this->SetOrientation(InputImage.GetOrientation());
}


template<typename PixelType>
template<typename PixelType_Input>
bool Image2D<PixelType>::Copy(const Image2D<PixelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DImage::Copy(Image* InputImage)")
        return false;
    }

    this->Copy(*InputImage);

    return true;
}


template<typename PixelType>
template<typename PixelType_Input>
bool Image2D<PixelType>::CopyData(const PixelType_Input* InputPixelPointer, int_max InputPixelNumber)
{
    if (InputPixelPointer == nullptr || InputPixelNumber <= 0)
	{
        MDK_Error("Invalid input @ 2DImage::CopyData(...)")
		return false;
	}

    auto SelfPixelNumber = this->GetPixelNumber();

    if (SelfPixelNumber != InputPixelNumber)
    {
        MDK_Error("Size does not match @ 2DImage::CopyData(...)")
        return false;
    }

    auto PixelPtr = this->GetPixelPointer();

    if (std::size_t(InputPixelPointer) == std::size_t(PixelPtr))
    {
        MDK_Warning("An image tries to Copy itself @ 2DImage::CopyData(...)")
        return true;
    }
  
    for (int_max i = 0; i < SelfPixelNumber; ++i)
	{
        PixelPtr[i] = PixelType(InputPixelPointer[i]);
	}

    return true;
}


template<typename PixelType>
bool Image2D<PixelType>::Fill(const PixelType& Pixel)
{
    auto PixelNumber = this->GetPixelNumber();

    if (PixelNumber == 0)
    {
        return false;
    }

    auto BeginPtr = this->GetPixelPointer();

    for (auto Ptr = BeginPtr; Ptr < BeginPtr + PixelNumber; ++Ptr)
    {
        Ptr[0] = Pixel;
    }

    return true;
}


template<typename PixelType>
bool Image2D<PixelType>::Share(Image2D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary

    m_PixelPointer = m_ImageData->m_DataArray.data();

    return true;
}


template<typename PixelType>
bool Image2D<PixelType>::Share(Image2D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DImage::Share(Image* InputImage)")
        return false;
    }

    return this->Share(*InputImage);
}


template<typename PixelType>
void Image2D<PixelType>::ForceShare(const Image2D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary

    m_PixelPointer = m_ImageData->m_DataArray.data();
}


template<typename PixelType>
bool Image2D<PixelType>::ForceShare(const Image2D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DImage::ForceShare(Image* InputImage)")
        return false;
    }

    return this->ForceShare(*InputImage);
}


template<typename PixelType>
void Image2D<PixelType>::Take(Image2D<PixelType>&& InputImage)
{
    this->Take(std::forward<Image2D<PixelType>&>(InputImage));
}


template<typename PixelType>
void Image2D<PixelType>::Take(Image2D<PixelType>& InputImage)
{
    m_ImageData->m_Size[0] = InputImage.m_ImageData->m_Size[0];
    m_ImageData->m_Size[1] = InputImage.m_ImageData->m_Size[1];
    
    m_ImageData->m_Spacing[0] = InputImage.m_ImageData->m_Spacing[0];
    m_ImageData->m_Spacing[1] = InputImage.m_ImageData->m_Spacing[1];
  
    m_ImageData->m_Origin[0] = InputImage.m_ImageData->m_Origin[0];
    m_ImageData->m_Origin[1] = InputImage.m_ImageData->m_Origin[1];
    
    m_ImageData->m_Orientation = std::move(InputImage.m_ImageData->m_Orientation);

    m_ImageData->m_DataArray = std::move(InputImage.m_ImageData->m_DataArray);
    
    m_PixelPointer = m_ImageData->m_DataArray.data();
    
    InputImage.Clear();
}


template<typename PixelType>
bool Image2D<PixelType>::Take(Image2D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Invalid input @ 2DImage::Take(Image2D<PixelType>*)")
        return false;
    }

    this->Take(*InputImage);

    return true;
}


template<typename PixelType>
void Image2D<PixelType>::Clear()
{
    m_ImageData->Clear();

    m_PixelPointer = nullptr;
}


template<typename PixelType>
inline
bool Image2D<PixelType>::IsEmpty() const
{
    return (m_ImageData->m_DataArray.size() == 0);
}


template<typename PixelType>
inline
PixelType* Image2D<PixelType>::GetPixelPointer()
{
    return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
const PixelType* Image2D<PixelType>::GetPixelPointer() const
{
    return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
ImageSize2D Image2D<PixelType>::GetSize() const
{
    ImageSize2D Size;

    Size.Lx = m_ImageData->m_Size[0];
    Size.Ly = m_ImageData->m_Size[1];

    return Size;
}


template<typename PixelType>
inline
void Image2D<PixelType>::GetSize(int_max& Lx, int_max& Ly, int_max& Lz) const
{
    Lx = m_ImageData->m_Size[0];
    Ly = m_ImageData->m_Size[1];
}


template<typename PixelType>
inline
bool Image2D<PixelType>::SetSize(const Image2DSize& Size)
{
    return this->SetSize(Size.Lx, Size.Ly);
}


template<typename PixelType>
inline
bool Image2D<PixelType>::SetSize(int_max Lx, int_max Ly)
{
    if (Lx < 0 || Ly < 0)
    {
        MDK_Error("Ivalid input @ 2DImage::SetSize(...)")
        return false;
    }

    if (Lx == 0 || Ly == 0)
    {
        m_ImageData->m_DataArray.clear();
        m_PixelPointer = nullptr;

        m_ImageData->m_Size[0] = 0;
        m_ImageData->m_Size[1] = 0;

        return true;
    }

try
{
    m_ImageData->m_DataArray.resize(Lx*Ly);
    m_PixelPointer = m_ImageData->m_DataArray.data();

    m_ImageData->m_Size[0] = Lx;
    m_ImageData->m_Size[1] = Ly;
}
catch (...)
{
    MDK_Error("Out Of Memory @ 2DImage::SetSize(...)")

    this->Clear();
    return false;
}

    return true;
}


template<typename PixelType>
inline
ImageSpacing2D Image2D<PixelType>::GetSpacing() const
{
    ImageSpacing2D Size;

    Size.Sx = m_ImageData->m_Spacing[0];
    Size.Sy = m_ImageData->m_Spacing[1];

    return Size;
}


template<typename PixelType>
inline
void Image2D<PixelType>::GetSpacing(double& Spacing_x, double& Spacing_y) const
{
    Spacing_x = m_ImageData->m_Spacing[0];
    Spacing_y = m_ImageData->m_Spacing[1];
}


template<typename PixelType>
inline
void Image2D<PixelType>::SetSpacing(const Image2DSpacing& Spacing)
{
    this->SetSpacing(Spacing.Sx, Spacing.Sy);
}


template<typename PixelType>
inline
void Image2D<PixelType>::SetSpacing(double Spacing_x, double Spacing_y)
{
    if (Spacing_x <= 0 || Spacing_y <= 0)
    {
        MDK_Error("Invalid input @ 2DImage::SetSpacing(...)")
        return;
    }

    m_ImageData->m_Spacing[0] = Spacing_x;
    m_ImageData->m_Spacing[1] = Spacing_y;
}


template<typename PixelType>
inline
ImageOrigin2D Image2D<PixelType>::GetOrigin() const
{
    ImageOrigin2D Origin;

    Origin.x = m_ImageData->m_Origin[0];
    Origin.y = m_ImageData->m_Origin[1];

    return Origin;
}


template<typename PixelType>
inline
void Image2D<PixelType>::GetOrigin(double& Origin_x, double& Origin_y) const
{
    Origin_x = m_ImageData->m_Origin[0];
    Origin_y = m_ImageData->m_Origin[1];
}


template<typename PixelType>
inline
void Image2D<PixelType>::SetOrigin(const Image2DOrigin& Origin)
{
    this->SetOrigin(Origin.x, Origin.y);
}


template<typename PixelType>
inline
void Image2D<PixelType>::SetOrigin(double Origin_x, double Origin_y)
{
    m_ImageData->m_Origin[0] = Origin_x;
    m_ImageData->m_Origin[1] = Origin_y;
}


template<typename PixelType>
inline 
const DenseMatrix<double>& Image2D<PixelType>::GetOrientation() const
{
    return m_ImageData->m_Orientation;
}


template<typename PixelType>
inline 
void Image2D<PixelType>::SetOrientation(const DenseMatrix<double>& Orientation)
{
    if (Orientation.IsEmpty() == false)
    {
        if (Orientation.GetColNumber() == 2 && Orientation.GetRowNumber() == 2)
        {
            m_ImageData->m_Orientation = Orientation;
        }
        else
        {
            MDK_Error("Invalid input Orientation @ 2DImage::SetOrientation(...)")
        }
    }
}


template<typename PixelType>
inline
ImagePhysicalSize2D Image2D<PixelType>::GetPhysicalSize() const
{
    ImagePhysicalSize2D Size;

    Size.Lx = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];

    Size.Ly = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];

    return Size;
}


template<typename PixelType>
inline 
void Image2D<PixelType>::GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y) const
{
    PhysicalSize_x = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];

    PhysicalSize_y = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
}


template<typename PixelType>
inline
int_max Image2D<PixelType>::GetPixelNumber() const
{
    return m_ImageData->m_Size[0] * m_ImageData->m_Size[1];
}
 

template<typename PixelType>
inline
int_max Image2D<PixelType>::Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const
{
    return m_ImageData->Transform2DIndexToLinearIndex(xIndex, yIndex);
}


template<typename PixelType>
inline
void Image2D<PixelType>::TransformLinearIndexTo2DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex) const
{
    m_ImageData->TransformLinearIndexTo2DIndex(LinearIndex, xIndex, yIndex);
}


template<typename PixelType>
inline
void Image2D<PixelType>::TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex, double& x, double& y) const
{
    m_ImageData->TransformLinearIndexTo2DPhysicalPosition(LinearIndex, x, y);
}


template<typename PixelType>
inline
void Image2D<PixelType>::Transform2DIndexTo2DPhysicalPosition(int_max xIndex, int_max yIndex, double& x, double& y) const
{
    m_ImageData->Transform2DIndexTo2DPhysicalPosition(xIndex, yIndex, x, y);
}


template<typename PixelType>
inline 
void Image2D<PixelType>::Transform2DPhysicalPositionToContinuous2DIndex(double x, double y, double& xIndex, double& yIndex) const
{
    m_ImageData->Transform2DPhysicalPositionToContinuous2DIndex(x, y, xIndex, yIndex);
}


template<typename PixelType>
inline
PixelType& Image2D<PixelType>::operator[](int_max LinearIndex)
{
#if defined MDK_DEBUG_2DImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();

    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ 2DImage::operator(LinearIndex)")
        return m_ImageData->m_NaNPixel;
    }

#endif //MDK_DEBUG_2DImage_Operator_CheckBound

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image2D<PixelType>::operator[](int_max LinearIndex) const
{
#if defined MDK_DEBUG_2DImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();

    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ 2DImage::operator(LinearIndex)")
        return m_ImageData->m_NaNPixel;
    }

#endif //MDK_DEBUG_2DImage_Operator_CheckBound

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
PixelType& Image2D<PixelType>::operator()(int_max LinearIndex)
{
#if defined MDK_DEBUG_2DImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();

	if (LinearIndex >= PixelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ 2DImage::operator(LinearIndex)")
		return m_ImageData->m_NaNPixel;
	}

#endif //MDK_DEBUG_2DImage_Operator_CheckBound

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image2D<PixelType>::operator()(int_max LinearIndex) const
{
#if defined MDK_DEBUG_2DImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();

	if (LinearIndex >= PixelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ 2DImage::operator(LinearIndex) const")
		return m_ImageData->m_NaNPixel;
	}

#endif //MDK_DEBUG_2DImage_Operator_CheckBound

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
PixelType& Image2D<PixelType>::at(int_max LinearIndex)
{
    auto PixelNumber = this->GetPixelNumber();

    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ 2DImage::at(LinearIndex)")
        return m_ImageData->m_NaNPixel;
    }

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image2D<PixelType>::at(int_max LinearIndex) const
{
    auto PixelNumber = this->GetPixelNumber();

    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ 2DImage::at(LinearIndex)")
        return m_ImageData->m_NaNPixel;
    }

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
PixelType& Image2D<PixelType>::operator()(int_max xIndex, int_max yIndex)
{
#if defined MDK_DEBUG_2DImage_Operator_CheckBound

    auto Size = this->GetSize();

    if (xIndex >= Size.Lx || xIndex < 0 || yIndex >= Size.Ly || yIndex < 0)
	{
		MDK_Error("Invalid input @ 2DImage::operator(xIndex, yIndex)")
		return m_ImageData->m_NaNPixel;
	}

#endif //MDK_DEBUG_2DImage_Operator_CheckBound
	
    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
const PixelType& Image2D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
#if defined MDK_DEBUG_2DImage_Operator_CheckBound

    auto Size = this->GetSize();

	if (xIndex >= Size.Lx || xIndex < 0 || yIndex >= Size.Ly || yIndex < 0)
	{
        MDK_Error("Invalid input @ 2DImage::operator(xIndex, yIndex) const")
        return m_ImageData->m_NaNPixel;
	}

#endif //MDK_DEBUG_2DImage_Operator_CheckBound

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
PixelType& Image2D<PixelType>::at(int_max xIndex, int_max yIndex)
{
    auto Size = this->GetSize();

	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0)
	{
		MDK_Error("Invalid input @ 2DImage::at(xIndex, yIndex)")
		return m_ImageData->m_NaNPixel;
	}

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
const PixelType& Image2D<PixelType>::at(int_max xIndex, int_max yIndex) const
{
    auto Size = this->GetSize();

    if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0)
    {
        MDK_Error("Invalid input @ 2DImage::at(xIndex, yIndex) const")
        return m_ImageData->m_NaNPixel;
    }

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
Image2D<PixelType> Image2D<PixelType>::GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e) const
{
    Image2D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 2DImage::GetSubImage()")
        return tempImage;
    }

    auto Size = this->GetSize();

    if (xIndex_s >= Size[0] || xIndex_s < 0 || xIndex_e >= Size[0] || xIndex_e < 0  || xIndex_s > xIndex_e
        || yIndex_s >= Size[1] || yIndex_s < 0 || yIndex_e >= Size[1] || yIndex_e < 0  || yIndex_s > yIndex_e)
	{
        MDK_Error("Invalid input @ 2DImage::GetSubImage() const")
		return tempImage;
	}

	auto Lx = xIndex_e - xIndex_s + 1;
	auto Ly = yIndex_e - yIndex_s + 1;

    tempImage.SetSize(Lx, Ly);
    tempImage.SetSpacing(this->GetSpacing());
    tempImage.SetOrigin(this->GetOrigin());

    auto tempRawPtr = tempImage.GetPixelPointer();

    auto RawPtr = this->GetPixelPointer();

	int_max Index_j = yIndex_s;

	for (int_max j = yIndex_s; j <= yIndex_e; ++j)
	{
        auto temp = j*Size.Lx;

		for (int_max i = xIndex_s; i <= xIndex_e; ++i)
		{
            tempRawPtr[0] = RawPtr[temp + i];
            
            ++tempRawPtr;
		}
	}
    
	return tempImage;
}


template<typename PixelType>
Image2D<PixelType> 
Image2D<PixelType>::Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly) const
{
    Image2D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 2DImage::Pad")
        return tempImage;
    }

    if (Option != "replicate")
	{
		MDK_Error("Invalid Option @ 2DImage::Pad")
		return tempImage;
	}

    if (Pad_Lx <= 0 && Pad_Ly <= 0)
	{
		MDK_Warning("Invalid Pad Size @ 2DImage::Pad")

		tempImage = (*this);

		return tempImage;
	}

    auto Size = this->GetSize();

    auto Lx = Size.Lx + Pad_Lx;
    auto Ly = Size.Ly + Pad_Ly;

    tempImage.SetSize(Lx, Ly);
    tempImage.SetSpacing(this->GetSpacing());
    tempImage.SetOrigin(this->GetOrigin());

	if (Option == "zero")
	{
        tempImage.Fill(PixelType(0));

		for (int_max j = 0; j <= Ly; ++j)
		{
			for (int_max i = 0; i <= Lx; ++i)
			{
				auto temp_i = i + Pad_Lx;
				auto temp_j = j + Pad_Ly;
		
				tempImage(temp_i, temp_j) = (*this)(i, j, k);
			}
		}
	}
	else if (Option == "replicate")
	{
		for (int_max temp_j = 0; temp_j <= Ly; ++temp_j)
		{
			for (int_max temp_i = 0; temp_i <= Lx; ++temp_i)
			{
				auto i = std::min(std::max(temp_i - Pad_Lx, 0), Size[0] - 1);

				auto j = std::min(std::max(temp_j - Pad_Ly, 0), Size[1] - 1);
		
				tempImage(temp_i, temp_j) = (*this)(i, j);
			}
		}		
	}    

    return tempImage;
}


template<typename PixelType>
Image2D<PixelType>
Image2D<PixelType>::Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly) const
{
    Image2D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 2DImage::Pad")
        return tempImage;
    }

	if (Pad_Lx <= 0 && Pad_Ly <= 0)
	{
		MDK_Warning("Invalid Pad Size @ 2DImage::Pad")

		tempImage = (*this);

		return tempImage;
	}

    auto Size = this->GetSize();

    auto Lx = Size.Lx + Pad_Lx;
    auto Ly = Size.Ly + Pad_Ly;

    tempImage.SetSize(Lx, Ly);
    tempImage.SetSpacing(this->GetSpacing());
    tempImage.SetOrigin(this->GetOrigin());

    tempImage.Fill(Pixel);

    for (int_max j = 0; j <= Ly; ++j)
    {
        for (int_max i = 0; i <= Lx; ++i)
        {
            auto temp_i = i + Pad_Lx;
            auto temp_j = j + Pad_Ly;

            tempImage(temp_i, temp_j) = (*this)(i, j);
        }
    }
    
	return tempImage;
}


template<typename PixelType>
Image2D<PixelType> 
Image2D<PixelType>::UnPad(int_max Pad_Lx, int_max Pad_Ly) const
{
    Image2D<PixelType> tempImage; // empty image
   
    auto Size = this->GetSize();

    if (Pad_Lx > Size.Lx || Pad_Lx  < 0 || Pad_Ly > Size.Ly || Pad_Ly < 0)
	{
		MDK_Error("Invalid Pad Size @ 2DImage::UnPad")
		return tempImage;
	}

    if (Pad_Lx == Size.Lx || Pad_Ly == Size.Ly)
	{
		MDK_Warning("Output is empty @ 2DImage::UnPad")
		return tempImage;
	}

	if (Pad_Lx == 0 && Pad_Ly == 0)
	{
		MDK_Warning("Input Pad Size is [0, 0] @ 2DImage::UnPad")

		tempImage = (*this);

		return tempImage;
	}

    return this->GetSubImage(Pad_Lx, Size.Lx - 1 - Pad_Lx,
                             Pad_Ly, Size.Ly - 1 - Pad_Ly);
                             

}


template<typename PixelType>
DenseMatrix<int_max>
Image2D<PixelType>::GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
                                                int_max yIndex_s,     int_max Region_Ly) const
{
    DenseMatrix<int_max>  List;
    
    auto Size = this->GetSize();

    if (   xIndex_s >= Size[0] || xIndex_s < 0
        || yIndex_s >= Size[1] || yIndex_s < 0
        || Region_Lx > Size[0] - xIndex_s || Region_Lx < 0
        || Region_Ly > Size[1] - yIndex_s || Region_Ly < 0)
	{
		MDK_Error("Invalid input @ 2DImage::GetLinearIndexArrayOfRegion")
		return List;
	}

	if (Region_Lx == 0 || Region_Ly == 0)
	{
		MDK_Warning("Empty input region @ 2DImage::GetLinearIndexArrayOfRegion")
		return List;
	}

    List.Resize(Region_Lx*Region_Ly, 1);

    auto PixelNumberPerZSlice = m_ImageData->PixelNumberPerZSlice;

	int_max Counter = 0;

	for (int_max j = yIndex_s; j < yIndex_s + Region_Ly; ++j)
	{
        auto temp = j*Size.Lx;

		for (int_max i = xIndex_s; i < xIndex_s + Region_Lx; ++i)
		{
            List[Counter] = temp + i;

		    Counter += 1;
		}
	}	
    
	return List;
}


}//end namespace mdk

#endif