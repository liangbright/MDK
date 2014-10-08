#ifndef __mdkImage3D_hpp
#define __mdkImage3D_hpp


namespace mdk
{

template<typename PixelType>
ImageData3D<PixelType>::ImageData3D()
{
    this->Clear();
}


template<typename PixelType>
ImageData3D<PixelType>::~ImageData3D()
{
}


template<typename PixelType>
inline
void ImageData3D<PixelType>::Clear()
{
    m_Size[0] = 0;
    m_Size[1] = 0;
    m_Size[2] = 0;

    m_PixelNumberPerZSlice = 0;

    m_Origin[0] = 0;
    m_Origin[1] = 0;
    m_Origin[2] = 0;

    m_Spacing[0] = 0;
    m_Spacing[1] = 0;
    m_Spacing[2] = 0;

    m_Orientation.FastResize(3, 3);
    m_Orientation.FixSize();
    m_Orientation.FillDiagonal(1.0);

    m_DataArray.clear();
    m_DataArray.shrink_to_fit();

    m_NaNPixel = GetNaNElement(m_NaNPixel);

	InterpolationOption.MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	InterpolationOption.BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Nearest;
	InterpolationOption.Pixel_OutsideImage = PixelType(0);
}


template<typename PixelType>
inline 
PixelType& ImageData3D<PixelType>::operator[](int_max LinearIndex)
{ 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& ImageData3D<PixelType>::operator[](int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
PixelType& ImageData3D<PixelType>::operator()(int_max LinearIndex)
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& ImageData3D<PixelType>::operator()(int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}
   

template<typename PixelType>
inline
PixelType& ImageData3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex)
{
    auto LinearIndex = zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;
 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& ImageData3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    auto LinearIndex = zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;

    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
int_max ImageData3D<PixelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;    
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageData3D<PixelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex) const
{
	DenseVector<ScalarType, 3> Index3D;

    std::lldiv_t divresult;

    divresult = div(LinearIndex, m_PixelNumberPerZSlice);

	Index3D[2] = divresult.quot; // z
            
    divresult = div(divresult.rem, m_Size[0]);

	Index3D[1] = divresult.quot; // y

	Index3D[0] = divresult.rem; // x

	return Index3D;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageData3D<PixelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const
{      
	DenseVector<ScalarType, 3> Position;

    std::lldiv_t divresult;

    divresult = div(LinearIndex, m_PixelNumberPerZSlice);

	Position[2] = ScalarType(divresult.quot);

    divresult = div(divresult.rem, m_Size[0]);

	Position[1] = ScalarType(divresult.quot);
      
	Position[0] = ScalarType(divresult.rem);

	Position[0] = m_Origin[0] + Position[0] * m_Spacing[0];
	Position[1] = m_Origin[1] + Position[1] * m_Spacing[1];
	Position[2] = m_Origin[2] + Position[2] * m_Spacing[2];

	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ImageData3D<PixelType>::Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex) const
{
	DenseVector<ScalarType, 3> Position;
	Position[0] = m_Origin[0] + double(xIndex) * m_Spacing[0];
	Position[1] = m_Origin[1] + double(yIndex) * m_Spacing[1];
	Position[2] = m_Origin[2] + double(zIndex) * m_Spacing[2];
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ImageData3D<PixelType>::Transform3DContinuousIndexTo3DPhysicalPosition(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const
{
	DenseVector<ScalarType, 3> Position;
	Position[0] = m_Origin[0] + xIndex * m_Spacing[0];
	Position[1] = m_Origin[1] + yIndex * m_Spacing[1];
	Position[2] = m_Origin[2] + zIndex * m_Spacing[2];
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageData3D<PixelType>::Transform3DPhysicalPositionTo3DContinuousIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> Index3D;
	Index3D[0] = (double(x) - m_Origin[0]) / m_Spacing[0];
	Index3D[1] = (double(y) - m_Origin[1]) / m_Spacing[1];
	Index3D[2] = (double(z) - m_Origin[2]) / m_Spacing[2];
	return Index3D;
}

//========================================================== Image ========================================================================//

template<typename PixelType>
Image3D<PixelType>::Image3D()
{
    m_ImageData = std::make_shared<ImageData3D<PixelType>>();
    this->Clear();
}


template<typename PixelType>
Image3D<PixelType>::Image3D(const Image3D<PixelType>& InputImage)
{
    m_ImageData = std::make_shared<ImageData3D<PixelType>>();    
    this->Clear();
    this->Copy(InputImage);
}


template<typename PixelType>
Image3D<PixelType>::Image3D(Image3D<PixelType>&& InputImage)
{
    m_ImageData = std::move(InputImage.m_ImageData);
    m_PixelPointer = m_ImageData->m_DataArray.data();

    // InputImage may not be destructed
	InputImage.m_PixelPointer = nullptr;
}


template<typename PixelType>
Image3D<PixelType>::~Image3D()
{
}


template<typename PixelType>
void Image3D<PixelType>::operator=(const Image3D<PixelType>& InputImage)
{
    this->Copy(InputImage);
}


template<typename PixelType>
void Image3D<PixelType>::operator=(Image3D<PixelType>&& InputImage)
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
void Image3D<PixelType>::Copy(const Image3D<PixelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        MDK_Warning("try to Copy self @ 3DImage::Copy(InputImage)")
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

	this->CopyPixelData(InputImage.GetPixelPointer(), InputImage.GetPixelNumber());
    this->SetSize(InputImage.GetSize());
    this->SetSpacing(InputImage.GetSpacing());
    this->SetOrigin(InputImage.GetOrigin);
    this->SetOrientation(InputImage.GetOrientation());
	this->SetInterpolationOption(InputImage.GetInterpolationOption());
}


template<typename PixelType>
template<typename PixelType_Input>
bool Image3D<PixelType>::Copy(const Image3D<PixelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DImage::Copy(Image* InputImage)")
        return false;
    }

    this->Copy(*InputImage);

    return true;
}


template<typename PixelType>
template<typename PixelType_Input>
bool Image3D<PixelType>::CopyPixelData(const PixelType_Input* InputPixelPointer, int_max InputPixelNumber)
{
    if (InputPixelPointer == nullptr || InputPixelNumber <= 0)
	{
        //MDK_Error("Invalid input @ 3DImage::CopyPixelData(...)")
		this->Clear();
		return true;
	}

    auto SelfPixelNumber = this->GetPixelNumber();

    if (SelfPixelNumber != InputPixelNumber)
    {
        MDK_Error("Size does not match @ 3DImage::CopyPixelData(...)")
        return false;
    }

    auto PixelPtr = this->GetPixelPointer();

    if (std::size_t(InputPixelPointer) == std::size_t(PixelPtr))
    {
        MDK_Warning("An image tries to Copy itself @ 3DImage::CopyPixelData(...)")
        return true;
    }
  
    for (int_max i = 0; i < SelfPixelNumber; ++i)
	{
        PixelPtr[i] = PixelType(InputPixelPointer[i]);
	}

    return true;
}


template<typename PixelType>
bool Image3D<PixelType>::Fill(const PixelType& Pixel)
{
	auto SelfPixelNumber = this->GetPixelNumber();
	if (SelfPixelNumber == 0)
    {
        return false;
    }

    auto BeginPtr = this->GetPixelPointer();

	for (auto Ptr = BeginPtr; Ptr < BeginPtr + SelfPixelNumber; ++Ptr)
    {
        Ptr[0] = Pixel;
    }

    return true;
}


template<typename PixelType>
bool Image3D<PixelType>::Share(Image3D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
    m_PixelPointer = m_ImageData->m_DataArray.data();

    return true;
}


template<typename PixelType>
bool Image3D<PixelType>::Share(Image3D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DImage::Share(Image* InputImage)")
        return false;
    }

    return this->Share(*InputImage);
}


template<typename PixelType>
void Image3D<PixelType>::ForceShare(const Image3D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
    m_PixelPointer = m_ImageData->m_DataArray.data();
}


template<typename PixelType>
bool Image3D<PixelType>::ForceShare(const Image3D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DImage::ForceShare(Image* InputImage)")
        return false;
    }

    return this->ForceShare(*InputImage);
}


template<typename PixelType>
void Image3D<PixelType>::Take(Image3D<PixelType>&& InputImage)
{
    this->Take(std::forward<Image3D<PixelType>&>(InputImage));
}


template<typename PixelType>
void Image3D<PixelType>::Take(Image3D<PixelType>& InputImage)
{
    m_ImageData->m_Size[0] = InputImage.m_ImageData->m_Size[0];
    m_ImageData->m_Size[1] = InputImage.m_ImageData->m_Size[1];
    m_ImageData->m_Size[2] = InputImage.m_ImageData->m_Size[2];    

    m_ImageData->m_PixelNumberPerZSlice = InputImage.m_ImageData->m_PixelNumberPerZSlice;
    
    m_ImageData->m_Spacing[0] = InputImage.m_ImageData->m_Spacing[0];
    m_ImageData->m_Spacing[1] = InputImage.m_ImageData->m_Spacing[1];
    m_ImageData->m_Spacing[2] = InputImage.m_ImageData->m_Spacing[2];

    m_ImageData->m_Origin[0] = InputImage.m_ImageData->m_Origin[0];
    m_ImageData->m_Origin[1] = InputImage.m_ImageData->m_Origin[1];
    m_ImageData->m_Origin[2] = InputImage.m_ImageData->m_Origin[2];
    
    m_ImageData->m_Orientation = std::move(InputImage.m_ImageData->m_Orientation);

    m_ImageData->m_DataArray = std::move(InputImage.m_ImageData->m_DataArray);    
    m_PixelPointer = m_ImageData->m_DataArray.data();
    
	m_ImageData->InterpolationOption = InputImage.m_ImageData->InterpolationOption;

    InputImage.Clear();
}


template<typename PixelType>
bool Image3D<PixelType>::Take(Image3D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Invalid input @ 3DImage::Take(Image3D<PixelType>*)")
        return false;
    }

    this->Take(*InputImage);

    return true;
}


template<typename PixelType>
void Image3D<PixelType>::Clear()
{
	if (m_ImageData)
	{
		m_ImageData->Clear();
		m_PixelPointer = nullptr;
	}
}


template<typename PixelType>
inline
bool Image3D<PixelType>::IsEmpty() const
{
	if (m_ImageData)
	{
		return (m_ImageData->m_DataArray.size() == 0);
	}
	{
		return true;
	}
}


template<typename PixelType>
inline
PixelType* Image3D<PixelType>::GetPixelPointer()
{
    return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
const PixelType* Image3D<PixelType>::GetPixelPointer() const
{
    return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
PixelType* Image3D<PixelType>::begin()
{
	return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
const PixelType* Image3D<PixelType>::begin() const
{
	return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
PixelType* Image3D<PixelType>::end()
{
	auto BeginPtr = m_ImageData->m_DataArray.data();
	if (BeginPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		return BeginPtr + this->GetPixelNumber();
	}
}


template<typename PixelType>
inline
const PixelType* Image3D<PixelType>::end() const
{
	auto BeginPtr = m_ImageData->m_DataArray.data();
	if (BeginPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		return BeginPtr + this->GetPixelNumber();
	}
}


template<typename PixelType>
inline
DenseVector<int_max, 3> Image3D<PixelType>::GetSize() const
{
	return m_ImageData->m_Size;
}


template<typename PixelType>
inline
void Image3D<PixelType>::GetSize(int_max& Lx, int_max& Ly, int_max& Lz) const
{
    Lx = m_ImageData->m_Size[0];
    Ly = m_ImageData->m_Size[1];
    Lz = m_ImageData->m_Size[2];
}


template<typename PixelType>
inline
bool Image3D<PixelType>::SetSize(const DenseVector<int_max, 3>& Size)
{
    return this->SetSize(Size[0], Size[1], Size[2]);
}


template<typename PixelType>
inline
bool Image3D<PixelType>::SetSize(int_max Lx, int_max Ly, int_max Lz)
{
    if (Lx < 0 || Ly < 0 || Lz < 0)
    {
        MDK_Error("Ivalid input @ 3DImage::SetSize(...)")
        return false;
    }

    if (Lx == 0 || Ly == 0 || Lz == 0)
    {
        m_ImageData->m_DataArray.clear();
        m_PixelPointer = nullptr;

        m_ImageData->m_Size[0] = 0;
        m_ImageData->m_Size[1] = 0;
        m_ImageData->m_Size[2] = 0;
        m_ImageData->m_PixelNumberPerZSlice = 0;

        return true;
    }

try
{
    m_ImageData->m_DataArray.resize(Lx*Ly*Lz);
    m_PixelPointer = m_ImageData->m_DataArray.data();

    m_ImageData->m_Size[0] = Lx;
    m_ImageData->m_Size[1] = Ly;
    m_ImageData->m_Size[2] = Lz;
    m_ImageData->m_PixelNumberPerZSlice = Ly*Lx;
}
catch (...)
{
    MDK_Error("Out Of Memory @ 3DImage::SetSize(...)")

    this->Clear();
    return false;
}

    return true;
}


template<typename PixelType>
inline
DenseVector<double, 3> Image3D<PixelType>::GetSpacing() const
{
	return m_ImageData->m_Spacing;
}


template<typename PixelType>
inline
void Image3D<PixelType>::GetSpacing(double& Spacing_x, double& Spacing_y, double& Spacing_z) const
{
    Spacing_x = m_ImageData->m_Spacing[0];
    Spacing_y = m_ImageData->m_Spacing[1];
    Spacing_z = m_ImageData->m_Spacing[2];
}


template<typename PixelType>
inline
void Image3D<PixelType>::SetSpacing(const DenseVector<double, 3>& Spacing)
{
    this->SetSpacing(Spacing[0], Spacing[1], Spacing[2]);
}


template<typename PixelType>
inline
void Image3D<PixelType>::SetSpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
    if (Spacing_x <= 0 || Spacing_y <= 0 || Spacing_z <= 0)
    {
        MDK_Error("Invalid input @ 3DImage::SetSpacing(...)")
        return;
    }

    m_ImageData->m_Spacing[0] = Spacing_x;
    m_ImageData->m_Spacing[1] = Spacing_y;
    m_ImageData->m_Spacing[2] = Spacing_z;
}


template<typename PixelType>
inline
DenseVector<double, 3> Image3D<PixelType>::GetOrigin() const
{
	return m_ImageData->m_Origin;
}


template<typename PixelType>
inline
void Image3D<PixelType>::GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const
{
    Origin_x = m_ImageData->m_Origin[0];
    Origin_y = m_ImageData->m_Origin[1];
    Origin_z = m_ImageData->m_Origin[2];
}


template<typename PixelType>
inline
void Image3D<PixelType>::SetOrigin(const DenseVector<double, 3>& Origin)
{
    this->SetOrigin(Origin[0], Origin[1], Origin[2]);
}


template<typename PixelType>
inline
void Image3D<PixelType>::SetOrigin(double Origin_x, double Origin_y, double Origin_z)
{
    m_ImageData->m_Origin[0] = Origin_x;
    m_ImageData->m_Origin[1] = Origin_y;
    m_ImageData->m_Origin[2] = Origin_z;
}


template<typename PixelType>
inline 
const DenseMatrix<double>& Image3D<PixelType>::GetOrientation() const
{
    return m_ImageData->m_Orientation;
}


template<typename PixelType>
inline 
void Image3D<PixelType>::SetOrientation(const DenseMatrix<double>& Orientation)
{
    if (Orientation.IsEmpty() == false)
    {
        if (Orientation.GetColNumber() == 3 && Orientation.GetRowNumber() == 3)
        {
            m_ImageData->m_Orientation = Orientation;
        }
        else
        {
            MDK_Error("Invalid input Orientation @ 3DImage::SetOrientation(...)")
        }
    }
}


template<typename PixelType>
inline
DenseVector<double, 3> Image3D<PixelType>::GetPhysicalSize() const
{
	DenseVector<double, 3> Size;
    Size[0] = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];
    Size[1] = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
    Size[2] = m_ImageData->m_Size[2] * m_ImageData->m_Spacing[2];
    return Size;
}


template<typename PixelType>
inline 
void Image3D<PixelType>::GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y, double& PhysicalSize_z) const
{
    PhysicalSize_x = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];
    PhysicalSize_y = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
    PhysicalSize_z = m_ImageData->m_Size[2] * m_ImageData->m_Spacing[2];
}


template<typename PixelType>
inline
int_max Image3D<PixelType>::GetPixelNumber() const
{
    return m_ImageData->m_Size[0] * m_ImageData->m_Size[1] * m_ImageData->m_Size[2];
}
 

template<typename PixelType>
inline
int_max Image3D<PixelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return m_ImageData->Transform3DIndexToLinearIndex(xIndex, yIndex, zIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> Image3D<PixelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo3DIndex(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> Image3D<PixelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo3DPhysicalPosition(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> Image3D<PixelType>::Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex) const
{
	return m_ImageData->Transform3DIndexTo3DPhysicalPosition(xIndex, yIndex, zIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> Image3D<PixelType>::Transform3DContinuousIndexTo3DPhysicalPosition(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const
{
	return m_ImageData->Transform3DContinuousIndexTo3DPhysicalPosition(xIndex, yIndex, zIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> Image3D<PixelType>::Transform3DPhysicalPositionTo3DContinuousIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	return m_ImageData->Transform3DPhysicalPositionTo3DContinuousIndex(x, y, z);
}


template<typename PixelType>
inline
PixelType& Image3D<PixelType>::operator[](int_max LinearIndex)
{
#if defined MDK_DEBUG_3DImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();

    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ 3DImage::operator(LinearIndex)")
        return m_ImageData->m_NaNPixel;
    }

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image3D<PixelType>::operator[](int_max LinearIndex) const
{
#if defined MDK_DEBUG_3DImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();

    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ 3DImage::operator(LinearIndex)")
        return m_ImageData->m_NaNPixel;
    }

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
PixelType& Image3D<PixelType>::operator()(int_max LinearIndex)
{
#if defined MDK_DEBUG_3DImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();

	if (LinearIndex >= PixelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ 3DImage::operator(LinearIndex)")
		return m_ImageData->m_NaNPixel;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image3D<PixelType>::operator()(int_max LinearIndex) const
{
#if defined MDK_DEBUG_3DImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();

	if (LinearIndex >= PixelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ 3DImage::operator(LinearIndex) const")
		return m_ImageData->m_NaNPixel;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
PixelType& Image3D<PixelType>::at(int_max LinearIndex)
{
    auto PixelNumber = this->GetPixelNumber();

    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ 3DImage::at(LinearIndex)")
        return m_ImageData->m_NaNPixel;
    }

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image3D<PixelType>::at(int_max LinearIndex) const
{
    auto PixelNumber = this->GetPixelNumber();

    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ 3DImage::at(LinearIndex)")
        return m_ImageData->m_NaNPixel;
    }

    return m_PixelPointer[LinearIndex];
}


template<typename PixelType>
inline
PixelType& Image3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex)
{
#if defined MDK_DEBUG_3DImage_Operator_CheckBound

    auto Size = this->GetSize();

	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
	{
		MDK_Error("Invalid input @ 3DImage::operator(xIndex, yIndex, zIndex)")
		return m_ImageData->m_NaNPixel;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound
	
    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
const PixelType& Image3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
#if defined MDK_DEBUG_3DImage_Operator_CheckBound

    auto Size = this->GetSize();

	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
	{
        MDK_Error("Invalid input @ 3DImage::operator(xIndex, yIndex, zIndex) const")
        return m_ImageData->m_NaNPixel;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
PixelType& Image3D<PixelType>::at(int_max xIndex, int_max yIndex, int_max zIndex)
{
    auto Size = this->GetSize();

	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
	{
		MDK_Error("Invalid input @ 3DImage::at(xIndex, yIndex, zIndex)")
		return m_ImageData->m_NaNPixel;
	}

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
const PixelType& Image3D<PixelType>::at(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    auto Size = this->GetSize();

    if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
    {
        MDK_Error("Invalid input @ 3DImage::at(xIndex, yIndex, zIndex) const")
        return m_ImageData->m_NaNPixel;
    }

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
const Option_Of_Image3DInterpolation<PixelType>& Image3D<PixelType>::GetInterpolationOption() const
{
	return m_ImageData->InterpolationOption;
}


template<typename PixelType>
void Image3D<PixelType>::SetInterpolationOption(const Option_Of_Image3DInterpolation<PixelType>& Option)
{
	m_ImageData->InterpolationOption = Option;
}


template<typename PixelType>
template<typename ScalarType>
PixelType Image3D<PixelType>::GetPixelAt3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z) const
{
	return InterpolateImageAt3DPhysicalPosition(*this, x, y, z, m_ImageData->InterpolationOption);
}


template<typename PixelType>
template<typename ScalarType>
PixelType Image3D<PixelType>::GetPixelAt3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z, const Option_Of_Image3DInterpolation<PixelType>& Option) const
{
	return InterpolateImageAt3DPhysicalPosition(*this, x, y, z, Option);
}


template<typename PixelType>
template<typename ScalarType>
PixelType Image3D<PixelType>::GetPixelAt3DContinuousIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const
{
	return InterpolateImageAt3DContinuousIndex(*this, xIndex, yIndex, zIndex, m_ImageData->InterpolationOption);
}


template<typename PixelType>
template<typename ScalarType>
PixelType Image3D<PixelType>::GetPixelAt3DContinuousIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex, const Option_Of_Image3DInterpolation<PixelType>& Option) const
{
	return InterpolateImageAt3DContinuousIndex(*this, xIndex, yIndex, zIndex, Option);
}


template<typename PixelType>
DenseMatrix<int_max>
Image3D<PixelType>::GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
                                                int_max yIndex_s,     int_max Region_Ly,
                                                int_max zIndex_s,     int_max Region_Lz) const
{
    DenseMatrix<int_max>  List;
    
    auto Size = this->GetSize();

    if (   xIndex_s >= Size[0] || xIndex_s < 0
        || yIndex_s >= Size[1] || yIndex_s < 0
        || zIndex_s >= Size[2] || zIndex_s < 0
        || Region_Lx > Size[0] - xIndex_s || Region_Lx < 0
        || Region_Ly > Size[1] - yIndex_s || Region_Ly < 0
        || Region_Lz > Size[2] - zIndex_s || Region_Lz < 0)
	{
		MDK_Error("Invalid input @ 3DImage::GetLinearIndexArrayOfRegion")
		return List;
	}

	if (Region_Lx == 0 || Region_Ly == 0 || Region_Lz == 0)
	{
		MDK_Warning("Empty input region @ 3DImage::GetLinearIndexArrayOfRegion")
		return List;
	}

    List.Resize(Region_Lx*Region_Ly*Region_Lz, 1);

    auto PixelNumberPerZSlice = m_ImageData->PixelNumberPerZSlice;

	int_max Counter = 0;

	for (int_max k = zIndex_s; k < zIndex_s + Region_Lz; ++k)
	{
        auto temp_k = k*PixelNumberPerZSlice;

		for (int_max j = yIndex_s; j < yIndex_s + Region_Ly; ++j)
		{
            auto temp_j = j*Size[0];

			for (int_max i = xIndex_s; i < xIndex_s + Region_Lx; ++i)
			{
                List[Counter] = temp_k + temp_j + i;

			    Counter += 1;
			}
		}
	}
    
	return List;
}


template<typename PixelType>
Image3D<PixelType>
Image3D<PixelType>::GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e, int_max zIndex_s, int_max zIndex_e) const
{
    Image3D<PixelType> tempImage; // empty image

    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 3DImage::GetSubImage()")
        return tempImage;
    }

    auto Size = this->GetSize();

    if (xIndex_s >= Size[0] || xIndex_s < 0
        || xIndex_e >= Size[0] || xIndex_e < 0
        || xIndex_s > xIndex_e
        || yIndex_s >= Size[1] || yIndex_s < 0
        || yIndex_e >= Size[1] || yIndex_e < 0
        || yIndex_s > yIndex_e
        || zIndex_s >= Size[2] || zIndex_s < 0
        || zIndex_e >= Size[2] || zIndex_e < 0
        || zIndex_s > zIndex_e)
    {
        MDK_Error("Invalid input @ 3DImage::GetSubImage() const")
            return tempImage;
    }

    auto Lx = xIndex_e - xIndex_s + 1;
    auto Ly = yIndex_e - yIndex_s + 1;
    auto Lz = zIndex_e - zIndex_s + 1;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetSpacing(this->GetSpacing());
    tempImage.SetOrigin(this->GetOrigin());

    auto tempRawPtr = tempImage.GetPixelPointer();

    auto RawPtr = this->GetPixelPointer();

    PixelNumberPerZSlice = m_ImageData->PixelNumberPerZSlice;

    int_max temp_k   temp_k = zIndex_s;

    for (int_max k = zIndex_s; k <= zIndex_e; ++k)
    {
        auto temp_k = k*PixelNumberPerZSlice;

        for (int_max j = yIndex_s; j <= yIndex_e; ++j)
        {
            auto temp_j = j*Size[0];

            for (int_max i = xIndex_s; i <= xIndex_e; ++i)
            {
                tempRawPtr[0] = RawPtr[temp_k + temp_j + i];

                ++tempRawPtr;
            }
        }
    }

    return tempImage;
}


template<typename PixelType>
Image3D<PixelType>
Image3D<PixelType>::Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    Image3D<PixelType> tempImage; // empty image

    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 3DImage::Pad")
        return tempImage;
    }

    if (Option != "replicate")
    {
        MDK_Error("Invalid Option @ 3DImage::Pad")
        return tempImage;
    }

    if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
    {
        MDK_Warning("Invalid Pad Size @ 3DImage::Pad")

        tempImage = (*this);

        return tempImage;
    }

    auto Size = this->GetSize();

    auto Lx = Size[0] + Pad_Lx;
    auto Ly = Size[1] + Pad_Ly;
    auto Lz = Size[2] + Pad_Lz;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetSpacing(this->GetSpacing());
    tempImage.SetOrigin(this->GetOrigin());

    if (Option == "zero")
    {
        tempImage.Fill(PixelType(0));

        for (int_max k = 0; k <= Lz; ++k)
        {
            for (int_max j = 0; j <= Ly; ++j)
            {
                for (int_max i = 0; i <= Lx; ++i)
                {
                    auto temp_i = i + Pad_Lx;
                    auto temp_j = j + Pad_Ly;
                    auto temp_k = k + Pad_Lz;

                    tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
                }
            }
        }
    }
    else if (Option == "replicate")
    {
        for (int_max temp_k = 0; temp_k <= Lz; ++temp_k)
        {
            for (int_max temp_j = 0; temp_j <= Ly; ++temp_j)
            {
                for (int_max temp_i = 0; temp_i <= Lx; ++temp_i)
                {
                    auto i = std::min(std::max(temp_i - Pad_Lx, 0), Size[0] - 1);

                    auto j = std::min(std::max(temp_j - Pad_Ly, 0), Size[1] - 1);

                    auto k = std::min(std::max(temp_k - Pad_Lz, 0), Size[2] - 1);

                    tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
                }
            }
        }
    }

    return tempImage;
}


template<typename PixelType>
Image3D<PixelType>
Image3D<PixelType>::Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    ScalarImage3D<PixelType> tempImage; // empty image

    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 3DImage::Pad")
        return tempImage;
    }

    if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
    {
        MDK_Warning("Invalid Pad Size @ 3DImage::Pad")

        tempImage = (*this);

        return tempImage;
    }

    auto Size = this->GetSize();

    auto Lx = Size[0] + Pad_Lx;
    auto Ly = Size[1] + Pad_Ly;
    auto Lz = Size[2] + Pad_Lz;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetSpacing(this->GetSpacing());
    tempImage.SetOrigin(this->GetOrigin());

    tempImage.Fill(Pixel);

    for (int_max k = 0; k <= Lz; ++k)
    {
        for (int_max j = 0; j <= Ly; ++j)
        {
            for (int_max i = 0; i <= Lx; ++i)
            {
                auto temp_i = i + Pad_Lx;
                auto temp_j = j + Pad_Ly;
                auto temp_k = k + Pad_Lz;

                tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
            }
        }
    }

    return tempImage;

}


template<typename PixelType>
Image3D<PixelType>
Image3D<PixelType>::UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    Image3D<PixelType> tempImage; // empty image

    auto Size = this->GetSize();

    if (Pad_Lx > Size[0] || Pad_Lx  < 0 || Pad_Ly > Size[1] || Pad_Ly < 0 || Pad_Lz > Size[2] || Pad_Lz < 0)
    {
        MDK_Error("Invalid Pad Size @ 3DImage::UnPad")
        return tempImage;
    }

    if (Pad_Lx == Size[0] || Pad_Ly == Size[1] || Pad_Lz == Size[2])
    {
        MDK_Warning("Output is empty @ 3DImage::UnPad")
        return tempImage;
    }

    if (Pad_Lx == 0 && Pad_Ly == 0 && Pad_Lz == 0)
    {
        MDK_Warning("Input Pad Size is [0, 0, 0] @ 3DImage::UnPad")

        tempImage = (*this);

        return tempImage;
    }

    return this->GetSubImage(Pad_Lx, Size[0] - 1 - Pad_Lx,
                             Pad_Ly, Size[1] - 1 - Pad_Ly,
                             Pad_Lz, Size[2] - 1 - Pad_Lz);


}

}//end namespace mdk

#endif