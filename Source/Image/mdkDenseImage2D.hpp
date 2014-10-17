#ifndef __mdkDenseImage2D_hpp
#define __mdkDenseImage2D_hpp


namespace mdk
{

template<typename PixelType>
DenseImageData2D<PixelType>::DenseImageData2D()
{
    this->Clear();
}


template<typename PixelType>
DenseImageData2D<PixelType>::~DenseImageData2D()
{
}


template<typename PixelType>
inline
void DenseImageData2D<PixelType>::Clear()
{
    m_Size[0] = 0;
    m_Size[1] = 0;

    m_Origin[0] = 0;
    m_Origin[1] = 0;

    m_Spacing[0] = 0;
    m_Spacing[1] = 0;

	m_Orientation.Clear();
    m_Orientation.Resize(2, 2);
    m_Orientation.FixSize();
    m_Orientation.FillDiagonal(1.0);

    m_DataArray.clear();
    m_DataArray.shrink_to_fit();

	m_Pixel_OutsideImage = PixelType(0);
}


template<typename PixelType>
inline 
PixelType& DenseImageData2D<PixelType>::operator[](int_max LinearIndex)
{ 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData2D<PixelType>::operator[](int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
PixelType& DenseImageData2D<PixelType>::operator()(int_max LinearIndex)
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData2D<PixelType>::operator()(int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}
   

template<typename PixelType>
inline
PixelType& DenseImageData2D<PixelType>::operator()(int_max xIndex, int_max yIndex)
{
    auto LinearIndex = yIndex*m_Size[0] + xIndex;
 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData2D<PixelType>::operator()(int_max xIndex, int_max yIndex) const
{
    auto LinearIndex = yIndex*m_Size[0] + xIndex;

    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
int_max DenseImageData2D<PixelType>::Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const
{
    return yIndex*m_Size[0] + xIndex;    
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> DenseImageData2D<PixelType>::TransformLinearIndexTo2DIndex(int_max LinearIndex) const
{
	auto divresult = std::div(LinearIndex, m_Size[0]);
	auto yIndex = divresult.quot; // y
	auto xIndex = divresult.rem;  // x

	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = ScalarType(xIndex);
	Index2D[1] = ScalarType(yIndex);
	return Index2D;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> DenseImageData2D<PixelType>::TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex) const
{
	DenseVector<ScalarType, 2> Position;
	auto Index2D = this->TransformLinearIndexTo2DIndex(LinearIndex);
	Position[0] = ScalarType(m_Origin[0] + double(Index2D[0]) * m_Spacing[0]);
	Position[1] = ScalarType(m_Origin[1] + double(Index2D[1]) * m_Spacing[1]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2> DenseImageData2D<PixelType>::
Transform2DIndexTo2DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const
{
	DenseVector<ScalarType_Position, 2> Position;
	Position[0] = ScalarType_Position(m_Origin[0] + double(xIndex) * m_Spacing[0]);
	Position[1] = ScalarType_Position(m_Origin[1] + double(yIndex) * m_Spacing[1]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> DenseImageData2D<PixelType>::Transform2DPhysicalPositionTo2DIndex(ScalarType x, ScalarType y) const
{
	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = ScalarType((double(x) - m_Origin[0]) / m_Spacing[0]);
	Index2D[1] = ScalarType((double(y) - m_Origin[1]) / m_Spacing[1]);
	return Index2D;
}

//========================================================== DenseImage ========================================================================//

template<typename PixelType>
DenseImage2D<PixelType>::DenseImage2D()
{
    m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
    this->Clear();
}


template<typename PixelType>
DenseImage2D<PixelType>::DenseImage2D(const DenseImage2D<PixelType>& InputImage)
{
    m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();    
    this->Clear();
    this->Copy(InputImage);
}


template<typename PixelType>
DenseImage2D<PixelType>::DenseImage2D(DenseImage2D<PixelType>&& InputImage)
{
    m_ImageData = std::move(InputImage.m_ImageData);
}


template<typename PixelType>
DenseImage2D<PixelType>::~DenseImage2D()
{
}


template<typename PixelType>
void DenseImage2D<PixelType>::operator=(const DenseImage2D<PixelType>& InputImage)
{
    this->Copy(InputImage);
}


template<typename PixelType>
void DenseImage2D<PixelType>::operator=(DenseImage2D<PixelType>&& InputImage)
{
    if (!m_ImageData)
    {
        m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
        this->Clear();
    }

    this->Take(InputImage);

	InputImage.Clear();
}


template<typename PixelType>
template<typename PixelType_Input>
void DenseImage2D<PixelType>::Copy(const DenseImage2D<PixelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        MDK_Warning("try to Copy self @ 2DDenseImage::Copy(InputImage)")
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
}


template<typename PixelType>
template<typename PixelType_Input>
bool DenseImage2D<PixelType>::Copy(const DenseImage2D<PixelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DDenseImage::Copy(DenseImage* InputImage)")
        return false;
    }

    this->Copy(*InputImage);

    return true;
}


template<typename PixelType>
template<typename PixelType_Input>
bool DenseImage2D<PixelType>::CopyPixelData(const PixelType_Input* InputPixelPointer, int_max InputPixelNumber)
{
    if (InputPixelPointer == nullptr || InputPixelNumber <= 0)
	{
        //MDK_Error("Invalid input @ 2DDenseImage::CopyPixelData(...)")
		this->Clear();
		return true;
	}

    auto SelfPixelNumber = this->GetPixelNumber();

    if (SelfPixelNumber != InputPixelNumber)
    {
        MDK_Error("Size does not match @ 2DDenseImage::CopyPixelData(...)")
        return false;
    }

    auto PixelPtr = this->GetPixelPointer();

    if (std::size_t(InputPixelPointer) == std::size_t(PixelPtr))
    {
        MDK_Warning("An DenseImage tries to Copy itself @ 2DDenseImage::CopyPixelData(...)")
        return true;
    }
  
    for (int_max i = 0; i < SelfPixelNumber; ++i)
	{
        PixelPtr[i] = PixelType(InputPixelPointer[i]);
	}

    return true;
}


template<typename PixelType>
bool DenseImage2D<PixelType>::Fill(const PixelType& Pixel)
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
bool DenseImage2D<PixelType>::Share(DenseImage2D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
    return true;
}


template<typename PixelType>
bool DenseImage2D<PixelType>::Share(DenseImage2D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DDenseImage::Share(DenseImage* InputImage)")
        return false;
    }

    return this->Share(*InputImage);
}


template<typename PixelType>
void DenseImage2D<PixelType>::ForceShare(const DenseImage2D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
}


template<typename PixelType>
bool DenseImage2D<PixelType>::ForceShare(const DenseImage2D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DDenseImage::ForceShare(DenseImage* InputImage)")
        return false;
    }

    return this->ForceShare(*InputImage);
}


template<typename PixelType>
void DenseImage2D<PixelType>::Take(DenseImage2D<PixelType>&& InputImage)
{
    this->Take(std::forward<DenseImage2D<PixelType>&>(InputImage));
}


template<typename PixelType>
void DenseImage2D<PixelType>::Take(DenseImage2D<PixelType>& InputImage)
{
    m_ImageData->m_Size[0] = InputImage.m_ImageData->m_Size[0];
    m_ImageData->m_Size[1] = InputImage.m_ImageData->m_Size[1];
    
    m_ImageData->m_Spacing[0] = InputImage.m_ImageData->m_Spacing[0];
    m_ImageData->m_Spacing[1] = InputImage.m_ImageData->m_Spacing[1];

    m_ImageData->m_Origin[0] = InputImage.m_ImageData->m_Origin[0];
    m_ImageData->m_Origin[1] = InputImage.m_ImageData->m_Origin[1];
    
    m_ImageData->m_Orientation = std::move(InputImage.m_ImageData->m_Orientation);

    m_ImageData->m_DataArray = std::move(InputImage.m_ImageData->m_DataArray);    
    
	m_ImageData->m_Pixel_OutsideImage = InputImage.m_ImageData->m_Pixel_OutsideImage;

    InputImage.Clear();
}


template<typename PixelType>
bool DenseImage2D<PixelType>::Take(DenseImage2D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Invalid input @ 2DDenseImage::Take(DenseImage2D<PixelType>*)")
        return false;
    }

    this->Take(*InputImage);

    return true;
}


template<typename PixelType>
void DenseImage2D<PixelType>::Clear()
{
	if (m_ImageData)
	{
		m_ImageData->Clear();
	}
}


template<typename PixelType>
inline
bool DenseImage2D<PixelType>::IsEmpty() const
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
PixelType* DenseImage2D<PixelType>::GetPixelPointer()
{
    return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
const PixelType* DenseImage2D<PixelType>::GetPixelPointer() const
{
    return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
PixelType* DenseImage2D<PixelType>::begin()
{
	return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
const PixelType* DenseImage2D<PixelType>::begin() const
{
	return m_ImageData->m_DataArray.data();
}


template<typename PixelType>
inline
PixelType* DenseImage2D<PixelType>::end()
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
const PixelType* DenseImage2D<PixelType>::end() const
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
Image2DInfo DenseImage2D<PixelType>::GetInfo() const
{
	Image2DInfo Info;
	Info.Origin = this->GetOrigin();
	Info.Spacing = this->GetSpacing();
	Info.Size = this->GetSize();
	Info.Orientation = this->GetOrientation();
	return Info;
}


template<typename PixelType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::GetSize() const
{
	return m_ImageData->m_Size;
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::GetSize(int_max& Lx, int_max& Ly) const
{
    Lx = m_ImageData->m_Size[0];
    Ly = m_ImageData->m_Size[1];
}


template<typename PixelType>
inline
bool DenseImage2D<PixelType>::SetSize(const DenseVector<int_max, 2>& Size)
{
    return this->SetSize(Size[0], Size[1]);
}


template<typename PixelType>
inline
bool DenseImage2D<PixelType>::SetSize(int_max Lx, int_max Ly)
{
    if (Lx < 0 || Ly < 0)
    {
        MDK_Error("Ivalid input @ 2DDenseImage::SetSize(...)")
        return false;
    }

    if (Lx == 0 || Ly == 0)
    {
        m_ImageData->m_DataArray.clear();
        m_ImageData->m_Size[0] = 0;
        m_ImageData->m_Size[1] = 0;
        return true;
    }

try
{
    m_ImageData->m_DataArray.resize(Lx*Ly); 
    m_ImageData->m_Size[0] = Lx;
    m_ImageData->m_Size[1] = Ly;
}
catch (...)
{
    MDK_Error("Out Of Memory @ 2DDenseImage::SetSize(...)")

    this->Clear();
    return false;
}

    return true;
}


template<typename PixelType>
inline
DenseVector<double, 2> DenseImage2D<PixelType>::GetSpacing() const
{
	return m_ImageData->m_Spacing;
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::GetSpacing(double& Spacing_x, double& Spacing_y) const
{
    Spacing_x = m_ImageData->m_Spacing[0];
    Spacing_y = m_ImageData->m_Spacing[1];
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetSpacing(const DenseVector<double, 2>& Spacing)
{
    this->SetSpacing(Spacing[0], Spacing[1]);
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetSpacing(double Spacing_x, double Spacing_y)
{
    if (Spacing_x <= 0 || Spacing_y <= 0)
    {
        MDK_Error("Invalid input @ 2DDenseImage::SetSpacing(...)")
        return;
    }
    m_ImageData->m_Spacing[0] = Spacing_x;
    m_ImageData->m_Spacing[1] = Spacing_y;
}


template<typename PixelType>
inline
DenseVector<double, 2> DenseImage2D<PixelType>::GetOrigin() const
{
	return m_ImageData->m_Origin;
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::GetOrigin(double& Origin_x, double& Origin_y) const
{
    Origin_x = m_ImageData->m_Origin[0];
    Origin_y = m_ImageData->m_Origin[1];
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetOrigin(const DenseVector<double, 2>& Origin)
{
    this->SetOrigin(Origin[0], Origin[1]);
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetOrigin(double Origin_x, double Origin_y)
{
    m_ImageData->m_Origin[0] = Origin_x;
    m_ImageData->m_Origin[1] = Origin_y;
}


template<typename PixelType>
inline 
const DenseMatrix<double>& DenseImage2D<PixelType>::GetOrientation() const
{
    return m_ImageData->m_Orientation;
}


template<typename PixelType>
inline 
void DenseImage2D<PixelType>::SetOrientation(const DenseMatrix<double>& Orientation)
{
    if (Orientation.IsEmpty() == false)
    {
        if (Orientation.GetColNumber() == 2 && Orientation.GetRowNumber() == 2)
        {
            m_ImageData->m_Orientation = Orientation;
        }
        else
        {
            MDK_Error("Invalid input Orientation @ 2DDenseImage::SetOrientation(...)")
        }
    }
}


template<typename PixelType>
inline
DenseVector<double, 2> DenseImage2D<PixelType>::GetPhysicalSize() const
{
	DenseVector<double, 2> Size;
    Size[0] = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];
    Size[1] = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
    return Size;
}


template<typename PixelType>
inline 
void DenseImage2D<PixelType>::GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y) const
{
    PhysicalSize_x = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];
    PhysicalSize_y = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
}


template<typename PixelType>
inline
int_max DenseImage2D<PixelType>::GetPixelNumber() const
{
	return m_ImageData->m_Size[0] * m_ImageData->m_Size[1];
}
 

template<typename PixelType>
inline
int_max DenseImage2D<PixelType>::Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const
{
    return m_ImageData->Transform2DIndexToLinearIndex(xIndex, yIndex);
}


template<typename PixelType>
inline
int_max DenseImage2D<PixelType>::Transform2DIndexToLinearIndex(const DenseVector<int_max, 2>& Index2D) const
{
	return m_ImageData->Transform2DIndexToLinearIndex(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::TransformLinearIndexTo2DIndex(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo2DIndex<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo2DPhysicalPosition<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2> DenseImage2D<PixelType>::
Transform2DIndexTo2DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const
{
	return m_ImageData->Transform2DIndexTo2DPhysicalPosition<ScalarType_Position>(xIndex, yIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2> DenseImage2D<PixelType>::Transform2DIndexTo2DPhysicalPosition(const DenseVector<ScalarType_Index, 2>& Index2D) const
{
	return m_ImageData->Transform2DIndexTo2DPhysicalPosition<ScalarType_Position>(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::Transform2DPhysicalPositionTo2DIndex(ScalarType x, ScalarType y) const
{
	return m_ImageData->Transform2DPhysicalPositionTo2DIndex(x, y);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::Transform2DPhysicalPositionTo2DIndex(const DenseVector<ScalarType, 2>& Position) const
{
	return m_ImageData->Transform2DPhysicalPositionTo2DIndex(Position[0], Position[1]);
}


template<typename PixelType>
inline
PixelType& DenseImage2D<PixelType>::operator[](int_max LinearIndex)
{
#if defined MDK_DEBUG_2DDenseImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();
    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ DenseImage2D::operator(LinearIndex)")
        return m_ImageData->m_Pixel_OutsideImage;
    }

#endif //MDK_DEBUG_2DDenseImage_Operator_CheckBound

	return (*m_ImageData)[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImage2D<PixelType>::operator[](int_max LinearIndex) const
{
#if defined MDK_DEBUG_2DDenseImage_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();
    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ DenseImage2D::operator(LinearIndex)")
        return m_ImageData->m_Pixel_OutsideImage;
    }

#endif //MDK_DEBUG_2DDenseImage_Operator_CheckBound

	return (*m_ImageData)[LinearIndex];
}


template<typename PixelType>
inline
PixelType& DenseImage2D<PixelType>::operator()(int_max LinearIndex)
{
	return this->operator[](LinearIndex);
}


template<typename PixelType>
inline
const PixelType& DenseImage2D<PixelType>::operator()(int_max LinearIndex) const
{
	return this->operator[](LinearIndex);
}


template<typename PixelType>
inline
PixelType& DenseImage2D<PixelType>::operator()(int_max xIndex, int_max yIndex)
{
#if defined MDK_DEBUG_2DDenseImage_Operator_CheckBound

    auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0)
	{
		MDK_Error("Invalid input @ DenseImage2D::operator(xIndex, yIndex)")
		return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_2DDenseImage_Operator_CheckBound
	
    return (*m_ImageData)(xIndex, yIndex);
}


template<typename PixelType>
inline
const PixelType& DenseImage2D<PixelType>::operator()(int_max xIndex, int_max yIndex) const
{
#if defined MDK_DEBUG_2DDenseImage_Operator_CheckBound

    auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0)
	{
        MDK_Error("Invalid input @ DenseImage2D::operator(xIndex, yIndex) const")
        return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_2DDenseImage_Operator_CheckBound

    return (*m_ImageData)(xIndex, yIndex);
}


template<typename PixelType>
template<typename ScalarType>
const PixelType& DenseImage2D<PixelType>::GetPixelNearestTo2DIndex(ScalarType xIndex, ScalarType yIndex) const
{
	if (this->IsEmpty() == true)
	{
		MDK_Error("Self is empty @ DenseImage2D:: GetPixelNearestTo2DIndex(...)")
		return m_ImageData->m_Pixel_OutsideImage;
	}

	auto Size = this->GetSize();

	auto x0 = int_max(xIndex);
	auto y0 = int_max(yIndex);

	if (x0 < 0)
	{
		x0 = 0;
	}
	else if (x0 >= Size[0])
	{
		x0 = Size[0] - 1;
	}

	if (y0 < 0)
	{
		y0 = 0;
	}
	else if (y0 >= Size[1])
	{
		y0 = Size[1] - 1;
	}

	return (*m_ImageData)(x0, y0);
}


template<typename PixelType>
template<typename ScalarType>
const PixelType& DenseImage2D<PixelType>::GetPixelNearestTo2DIndex(const DenseVector<ScalarType, 2>& Index2D) const
{
	return this->GetPixelNearestTo2DIndex(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& DenseImage2D<PixelType>::GetPixelNearestTo2DPhysicalPosition(ScalarType x, ScalarType y) const
{
	auto Index2D = this->Transform2DPhysicalPositionTo2DIndex(x, y);
	return this->GetPixelNearestTo2DIndex(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& DenseImage2D<PixelType>::GetPixelNearestTo2DPhysicalPosition(const DenseVector<ScalarType, 2>& Position) const
{
	return this->GetPixelNearestTo2DPhysicalPosition(Position[0], Position[1]);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt2DIndex(ScalarType xIndex, ScalarType yIndex, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt2DContinuousIndex<OutputPixelType>(*this, xIndex, yIndex, Option);
}


template<typename PixelType>
const PixelType& DenseImage2D<PixelType>::GetPixelAt2DIndex(int_max xIndex, int_max yIndex, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt2DContinuousIndex_Nearest<PixelType>(*this, xIndex, yIndex, Option);
}


template<typename PixelType>
const PixelType& DenseImage2D<PixelType>::GetPixelAt2DIndex(int xIndex, int yIndex, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(int_max(xIndex), int_max(yIndex), Option);
}


template<typename PixelType>
const PixelType& DenseImage2D<PixelType>::GetPixelAt2DIndex(long xIndex, long yIndex, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(int_max(xIndex), int_max(yIndex), Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<ScalarType, 2>& Index2D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex<OutputPixelType>(Index2D[0], Index2D[1], Option);
}


template<typename PixelType>
const PixelType& DenseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<int_max, 2>& Index2D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(Index2D[0], Index2D[1], Option);
}


template<typename PixelType>
const PixelType& DenseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<int, 2>& Index2D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(int_max(Index2D[0]), int_max(Index2D[1]), Option);
}


template<typename PixelType>
const PixelType& DenseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<long, 2>& Index2D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(int_max(Index2D[0]), int_max(Index2D[1]), Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt2DPhysicalPosition(ScalarType x, ScalarType y, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt2DPhysicalPosition<OutputPixelType>(*this, x, y, Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt2DPhysicalPosition(const DenseVector<ScalarType, 2>& Position, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt2DPhysicalPosition<OutputPixelType>(*this, Position[0], Position[1], Option);
}


template<typename PixelType>
DenseMatrix<int_max>
DenseImage2D<PixelType>::GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
                                                    int_max yIndex_s,     int_max Region_Ly) const
{
    DenseMatrix<int_max>  List;
    
    auto Size = this->GetSize();

    if (   xIndex_s >= Size[0] || xIndex_s < 0
        || yIndex_s >= Size[1] || yIndex_s < 0
        || Region_Lx > Size[0] - xIndex_s || Region_Lx < 0
        || Region_Ly > Size[1] - yIndex_s || Region_Ly < 0)
	{
		MDK_Error("Invalid input @ 2DDenseImage::GetLinearIndexArrayOfRegion")
		return List;
	}

	if (Region_Lx == 0 || Region_Ly == 0)
	{
		MDK_Warning("Empty input region @ 2DDenseImage::GetLinearIndexArrayOfRegion")
		return List;
	}

    List.Resize(Region_Lx*Region_Ly, 1);

	int_max Counter = 0;

	for (int_max j = yIndex_s; j < yIndex_s + Region_Ly; ++j)
	{
		auto temp_j = j*Size[0];

		for (int_max i = xIndex_s; i < xIndex_s + Region_Lx; ++i)
		{
			List[Counter] = temp_j + i;
			Counter += 1;
		}
	}
    
	return List;
}


template<typename PixelType>
DenseImage2D<PixelType>
DenseImage2D<PixelType>::GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e) const
{
	DenseImage2D<PixelType> SubImage; // empty SubImage

	if (this->IsEmpty() == true)
	{
		MDK_Warning("Self is empty @ DenseImage2D::GetSubImage()")
			return SubImage;
	}

	auto Size = this->GetSize();

	if (   xIndex_s >= Size[0] || xIndex_s < 0
		|| xIndex_e >= Size[0] || xIndex_e < 0
		|| xIndex_s > xIndex_e
		|| yIndex_s >= Size[1] || yIndex_s < 0
		|| yIndex_e >= Size[1] || yIndex_e < 0
		|| yIndex_s > yIndex_e)
	{
		MDK_Error("Invalid input @ DenseImage2D::GetSubImage()")
			return SubImage;
	}

	auto Lx = xIndex_e - xIndex_s + 1;
	auto Ly = yIndex_e - yIndex_s + 1;

	SubImage.SetSize(Lx, Ly, Lz);
	SubImage.SetSpacing(this->GetSpacing());
	SubImage.SetOrigin(this->GetOrigin());
	SubImage.SetOrientation(this->GetOrientation());
	auto SubPtr = SubImage.GetPixelPointer();
	auto RawPtr = this->GetPixelPointer();
	for (int_max j = yIndex_s; j <= yIndex_e; ++j)
	{
		auto temp_j = j*Size[0];
		for (int_max i = xIndex_s; i <= xIndex_e; ++i)
		{
			SubPtr[0] = RawPtr[temp_j + i];
			++SubPtr;
		}
	}

	return SubImage;
}


template<typename PixelType>
DenseImage2D<PixelType>
DenseImage2D<PixelType>::Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly) const
{
	DenseImage2D<PixelType> SubImage; // empty SubImage

    if (this->IsEmpty() == true)
    {
        MDK_Warning("Self is empty @ DenseImage2D::Pad")
        return SubImage;
    }

    if (Option != "replicate")
    {
        MDK_Error("Invalid Option @ DenseImage2D::Pad")
        return SubImage;
    }

    if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
    {
        MDK_Warning("Zero Pad Size @ DenseImage2D::Pad")
		SubImage = (*this);
		return SubImage;
    }

    auto Size = this->GetSize();

    auto Lx = Size[0] + Pad_Lx;
    auto Ly = Size[1] + Pad_Ly;

	SubImage.SetSize(Lx, Ly);
	SubImage.SetSpacing(this->GetSpacing());
	SubImage.SetOrigin(this->GetOrigin());
	SubImage.SetOrientation(this->GetOrientation());
    if (Option == "zero")
    {
		SubImage.Fill(PixelType(0));

		for (int_max j = 0; j <= Ly; ++j)
		{
			for (int_max i = 0; i <= Lx; ++i)
			{
				auto temp_i = i + Pad_Lx;
				auto temp_j = j + Pad_Ly;
				SubImage(temp_i, temp_j) = (*this)(i, j);
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
				SubImage(temp_i, temp_j) = (*this)(i, j);
			}
		}
	}

	return SubImage;
}


template<typename PixelType>
DenseImage2D<PixelType>
DenseImage2D<PixelType>::Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly) const
{
	DenseImage2D<PixelType> SubImage; // empty SubImage

	if (this->IsEmpty() == true)
	{
		MDK_Warning("DenseImage is empty @ DenseImage2D::Pad(...)")
		return SubImage;
	}

	if (Pad_Lx <= 0 && Pad_Ly <= 0)
	{
		MDK_Warning("Zero Pad Size @ DenseImage2D::Pad(...)")
		SubImage = (*this);
		return SubImage;
	}

	auto Size = this->GetSize();

	auto Lx = Size[0] + Pad_Lx;
	auto Ly = Size[1] + Pad_Ly;

	SubImage.SetSize(Lx, Ly);
	SubImage.SetSpacing(this->GetSpacing());
	SubImage.SetOrigin(this->GetOrigin());
	SubImage.SetOrientation(this->GetOrientation());
	SubImage.Fill(Pixel);
	for (int_max j = 0; j <= Ly; ++j)
	{
		for (int_max i = 0; i <= Lx; ++i)
		{
			auto temp_i = i + Pad_Lx;
			auto temp_j = j + Pad_Ly;
			SubImage(temp_i, temp_j) = (*this)(i, j);
		}
	}

	return SubImage;
}


template<typename PixelType>
DenseImage2D<PixelType>
DenseImage2D<PixelType>::UnPad(int_max Pad_Lx, int_max Pad_Ly) const
{
	DenseImage2D<PixelType> SubImage; // empty SubImage

    auto Size = this->GetSize();

    if (Pad_Lx > Size[0] || Pad_Lx  < 0 || Pad_Ly > Size[1] || Pad_Ly < 0)
    {
        MDK_Error("Invalid Pad Size @ DenseImage2D::UnPad(...)")
        return SubImage;
    }

    if (Pad_Lx == Size[0] || Pad_Ly == Size[1] || Pad_Lz == Size[2])
    {
        MDK_Warning("Output is empty @ DenseImage2D::UnPad(...)")
        return SubImage;
    }

    if (Pad_Lx == 0 && Pad_Ly == 0 && Pad_Lz == 0)
    {
        MDK_Warning("Input Pad Size is [0, 0, 0] @ DenseImage2D::UnPad(...)")
		SubImage = (*this);
		return SubImage;
    }

    return this->GetSubImage(Pad_Lx, Size[0] - 1 - Pad_Lx,
                             Pad_Ly, Size[1] - 1 - Pad_Ly);
}

}//end namespace mdk

#endif