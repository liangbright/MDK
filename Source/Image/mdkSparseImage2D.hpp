#ifndef mdk_SparseImage2D_hpp
#define mdk_SparseImage2D_hpp

namespace mdk
{

template<typename PixelType>
SparseImageData2D<PixelType>::SparseImageData2D()
{
    this->Clear();
}


template<typename PixelType>
SparseImageData2D<PixelType>::~SparseImageData2D()
{
}


template<typename PixelType>
inline
void SparseImageData2D<PixelType>::Clear()
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

    m_PixelMap.clear();
  
	m_Pixel_OutsideImage = PixelType(0);
}


template<typename PixelType>
inline
const PixelType& SparseImageData2D<PixelType>::GetPixel(int_max LinearIndex) const
{
    return m_PixelMap[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& SparseImageData2D<PixelType>::GetPixel(int_max xIndex, int_max yIndex) const
{
    auto LinearIndex = yIndex*m_Size[0] + xIndex;
	return m_PixelMap[LinearIndex];
}


template<typename PixelType>
inline
void SparseImageData2D<PixelType>::SetPixel(int_max LinearIndex, const PixelType& Pixel)
{
	m_PixelMap[LinearIndex] = Pixel;
}


template<typename PixelType>
inline
void SparseImageData2D<PixelType>::SetPixel(int_max xIndex, int_max yIndex, const PixelType& Pixel)
{
	auto LinearIndex = yIndex*m_Size[0] + xIndex;
	m_PixelMap[LinearIndex] = Pixel;
}


template<typename PixelType>
inline
int_max SparseImageData2D<PixelType>::Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const
{
    return yIndex*m_Size[0] + xIndex;    
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> SparseImageData2D<PixelType>::TransformLinearIndexTo2DIndex(int_max LinearIndex) const
{
	auto divresult = div(LinearIndex, m_Size[0]);
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
DenseVector<ScalarType, 2> SparseImageData2D<PixelType>::TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex) const
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
DenseVector<ScalarType_Position, 2> SparseImageData2D<PixelType>::
Transform2DIndexTo2DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const
{
	DenseVector<ScalarType_Position, 2> Position;
	Position[0] = m_Origin[0] + double(xIndex) * m_Spacing[0];
	Position[1] = m_Origin[1] + double(yIndex) * m_Spacing[1];
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> SparseImageData2D<PixelType>::Transform2DPhysicalPositionTo2DIndex(ScalarType x, ScalarType y) const
{
	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = (double(x) - m_Origin[0]) / m_Spacing[0];
	Index2D[1] = (double(y) - m_Origin[1]) / m_Spacing[1];
	return Index2D;
}

//========================================================== SparseImage ========================================================================//

template<typename PixelType>
SparseImage2D<PixelType>::SparseImage2D()
{
    m_ImageData = std::make_shared<SparseImageData2D<PixelType>>();
}


template<typename PixelType>
SparseImage2D<PixelType>::SparseImage2D(const SparseImage2D<PixelType>& InputSparseImage)
{
    this->Copy(InputSparseImage);
}


template<typename PixelType>
SparseImage2D<PixelType>::SparseImage2D(SparseImage2D<PixelType>&& InputSparseImage)
{
	m_ImageData = std::move(InputSparseImage.m_ImageData);
}


template<typename PixelType>
SparseImage2D<PixelType>::~SparseImage2D()
{
}


template<typename PixelType>
void SparseImage2D<PixelType>::operator=(const SparseImage2D<PixelType>& InputImage)
{
    this->Copy(InputImage);
}


template<typename PixelType>
void SparseImage2D<PixelType>::operator=(SparseImage2D<PixelType>&& InputImage)
{
	this->Copy(std::move(InputImage));
}


template<typename PixelType>
template<typename PixelType_Input>
void SparseImage2D<PixelType>::Copy(const SparseImage2D<PixelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        MDK_Warning("try to Copy self @ 2DSparseImage::Copy(InputSparseImage)")
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

	if (!m_ImageData)
	{
		m_ImageData = std::make_shared<SparseImageData2D<PixelType>>();
	}

    this->SetSize(InputSparseImage.GetSize());
    this->SetSpacing(InputSparseImage.GetSpacing());
    this->SetOrigin(InputSparseImage.GetOrigin);
    this->SetOrientation(InputSparseImage.GetOrientation());
	this->CopyPixelData(InputSparseImage);

}


template<typename PixelType>
template<typename PixelType_Input>
bool SparseImage2D<PixelType>::Copy(const SparseImage2D<PixelType_Input>* InputSparseImage)
{
    if (InputSparseImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DSparseImage::Copy(SparseImage* InputSparseImage)")
        return false;
    }
    this->Copy(*InputSparseImage);
    return true;
}


template<typename PixelType>
template<typename PixelType_Input>
void SparseImage2D<PixelType>::CopyPixelData(const SparseImage2D<PixelType_Input>& InputImage)
{
	for (auto it = InputImage.m_PixelMap.begin(); it != InputImage.m_PixelMap.end(); ++it)
	{
		m_PixelMap[it->first] = PixelType(it->second);
	}
}


template<typename PixelType>
void SparseImage2D<PixelType>::Copy(SparseImage2D<PixelType>&& InputSparseImage)
{
	if (!m_ImageData)
	{
		m_ImageData = std::make_shared<SparseImageData2D<PixelType>>();
	}

	m_ImageData->m_Size[0] = InputSparseImage.m_ImageData->m_Size[0];
	m_ImageData->m_Size[1] = InputSparseImage.m_ImageData->m_Size[1];
	m_ImageData->m_Size[2] = InputSparseImage.m_ImageData->m_Size[2];

	m_ImageData->m_PixelNumberPerZSlice = InputSparseImage.m_ImageData->m_PixelNumberPerZSlice;

	m_ImageData->m_Spacing[0] = InputSparseImage.m_ImageData->m_Spacing[0];
	m_ImageData->m_Spacing[1] = InputSparseImage.m_ImageData->m_Spacing[1];
	m_ImageData->m_Spacing[2] = InputSparseImage.m_ImageData->m_Spacing[2];

	m_ImageData->m_Origin[0] = InputSparseImage.m_ImageData->m_Origin[0];
	m_ImageData->m_Origin[1] = InputSparseImage.m_ImageData->m_Origin[1];
	m_ImageData->m_Origin[2] = InputSparseImage.m_ImageData->m_Origin[2];

	m_ImageData->m_Orientation = std::move(InputSparseImage.m_ImageData->m_Orientation);

	m_ImageData->m_PixelMap = std::move(InputSparseImage.m_ImageData->m_PixelMap);

	m_ImageData->m_Pixel_OutsideImage = InputSparseImage.m_ImageData->m_Pixel_OutsideImage;

	InputSparseImage.Clear();
}


template<typename PixelType>
bool SparseImage2D<PixelType>::Share(SparseImage2D<PixelType>& InputSparseImage)
{
    m_ImageData = InputSparseImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
    return true;
}


template<typename PixelType>
bool SparseImage2D<PixelType>::Share(SparseImage2D<PixelType>* InputSparseImage)
{
    if (InputSparseImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DSparseImage::Share(SparseImage* InputSparseImage)")
        return false;
    }

    return this->Share(*InputSparseImage);
}


template<typename PixelType>
void SparseImage2D<PixelType>::ForceShare(const SparseImage2D<PixelType>& InputSparseImage)
{
    m_ImageData = InputSparseImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
}


template<typename PixelType>
bool SparseImage2D<PixelType>::ForceShare(const SparseImage2D<PixelType>* InputSparseImage)
{
    if (InputSparseImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 2DSparseImage::ForceShare(SparseImage* InputSparseImage)")
        return false;
    }

    return this->ForceShare(*InputSparseImage);
}


template<typename PixelType>
void SparseImage2D<PixelType>::Clear()
{
	if (m_ImageData)
	{
		m_ImageData->Clear();
	}
}


template<typename PixelType>
inline
bool SparseImage2D<PixelType>::IsEmpty() const
{
	if (m_ImageData)
	{
		return (m_ImageData->m_PixelMap.size() == 0);
	}
	{
		return true;
	}
}


template<typename PixelType>
inline
Image2DInfo SparseImage2D<PixelType>::GetInfo() const
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
DenseVector<int_max, 2> SparseImage2D<PixelType>::GetSize() const
{
	return m_ImageData->m_Size;
}


template<typename PixelType>
inline
void SparseImage2D<PixelType>::GetSize(int_max& Lx, int_max& Ly) const
{
    Lx = m_ImageData->m_Size[0];
    Ly = m_ImageData->m_Size[1];
}


template<typename PixelType>
inline
bool SparseImage2D<PixelType>::SetSize(const DenseVector<int_max, 2>& Size)
{
    return this->SetSize(Size[0], Size[1]);
}


template<typename PixelType>
inline
bool SparseImage2D<PixelType>::SetSize(int_max Lx, int_max Ly)
{
    if (Lx < 0 || Ly < 0)
    {
        MDK_Error("Ivalid input @ 2DSparseImage::SetSize(...)")
        return false;
    }

	if (Lx == m_ImageData->m_Size[0] && Ly == m_ImageData->m_Size[1])
	{
		return;
	}

	if (Lx == 0 || Ly == 0)
	{
		m_ImageData->m_PixelMap.clear();
		m_ImageData->m_Size[0] = 0;
		m_ImageData->m_Size[1] = 0;
		return true;
	}

	m_ImageData->m_PixelMap.clear();
	m_ImageData->m_Size[0] = Lx;
	m_ImageData->m_Size[1] = Ly;
    return true;
}


template<typename PixelType>
inline
DenseVector<double, 2> SparseImage2D<PixelType>::GetSpacing() const
{
	return m_ImageData->m_Spacing;
}


template<typename PixelType>
inline
void SparseImage2D<PixelType>::GetSpacing(double& Spacing_x, double& Spacing_y) const
{
    Spacing_x = m_ImageData->m_Spacing[0];
    Spacing_y = m_ImageData->m_Spacing[1];
}


template<typename PixelType>
inline
void SparseImage2D<PixelType>::SetSpacing(const DenseVector<double, 2>& Spacing)
{
    this->SetSpacing(Spacing[0], Spacing[1]);
}


template<typename PixelType>
inline
void SparseImage2D<PixelType>::SetSpacing(double Spacing_x, double Spacing_y)
{
	auto Zero = std::numeric_limits<double>::epsilon();
	if (Spacing_x <= Zero || Spacing_y <= Zero)
    {
        MDK_Error("Invalid input (<= eps) @ 2DSparseImage::SetSpacing(...)")
        return;
    }

    m_ImageData->m_Spacing[0] = Spacing_x;
    m_ImageData->m_Spacing[1] = Spacing_y;
}


template<typename PixelType>
inline
DenseVector<double, 2> SparseImage2D<PixelType>::GetOrigin() const
{
	return m_ImageData->m_Origin;
}


template<typename PixelType>
inline
void SparseImage2D<PixelType>::GetOrigin(double& Origin_x, double& Origin_y) const
{
    Origin_x = m_ImageData->m_Origin[0];
    Origin_y = m_ImageData->m_Origin[1];
}


template<typename PixelType>
inline
void SparseImage2D<PixelType>::SetOrigin(const DenseVector<double, 2>& Origin)
{
    this->SetOrigin(Origin[0], Origin[1]);
}


template<typename PixelType>
inline
void SparseImage2D<PixelType>::SetOrigin(double Origin_x, double Origin_y)
{
    m_ImageData->m_Origin[0] = Origin_x;
    m_ImageData->m_Origin[1] = Origin_y;
}


template<typename PixelType>
inline 
const DenseMatrix<double>& SparseImage2D<PixelType>::GetOrientation() const
{
    return m_ImageData->m_Orientation;
}


template<typename PixelType>
inline 
void SparseImage2D<PixelType>::SetOrientation(const DenseMatrix<double>& Orientation)
{
    if (Orientation.IsEmpty() == false)
    {
        if (Orientation.GetColNumber() == 2 && Orientation.GetRowNumber() == 2)
        {
            m_ImageData->m_Orientation = Orientation;
        }
        else
        {
            MDK_Error("Invalid input Orientation @ 2DSparseImage::SetOrientation(...)")
        }
    }
}


template<typename PixelType>
inline
DenseVector<double, 2> SparseImage2D<PixelType>::GetPhysicalSize() const
{
	DenseVector<double, 2> Size;
    Size[0] = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];
    Size[1] = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
    return Size;
}


template<typename PixelType>
inline 
void SparseImage2D<PixelType>::GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y) const
{
    PhysicalSize_x = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];
    PhysicalSize_y = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
}


template<typename PixelType>
inline
int_max SparseImage2D<PixelType>::GetRecordedPixelNumber() const
{
	return int_max(m_ImageData->m_PixelMap.size());
}
 

template<typename PixelType>
inline
int_max SparseImage2D<PixelType>::Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const
{
    return m_ImageData->Transform2DIndexToLinearIndex(xIndex, yIndex);
}


template<typename PixelType>
inline
int_max SparseImage2D<PixelType>::Transform2DIndexToLinearIndex(const DenseVector<int_max, 2>& Index2D) const
{
	return m_ImageData->Transform2DIndexToLinearIndex(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> SparseImage2D<PixelType>::TransformLinearIndexTo2DIndex(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo2DIndex<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> SparseImage2D<PixelType>::TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo2DPhysicalPosition<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2> SparseImage2D<PixelType>::
Transform2DIndexTo2DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const
{
	return m_ImageData->Transform2DIndexTo2DPhysicalPosition<ScalarType_Position>(xIndex, yIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2> SparseImage2D<PixelType>::Transform2DIndexTo2DPhysicalPosition(const DenseVector<ScalarType_Index, 2>& Index2D) const
{
	return m_ImageData->Transform2DIndexTo2DPhysicalPosition<ScalarType_Position>(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> SparseImage2D<PixelType>::Transform2DPhysicalPositionTo2DIndex(ScalarType x, ScalarType y) const
{
	return m_ImageData->Transform2DPhysicalPositionTo2DIndex(x, y);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> SparseImage2D<PixelType>::Transform2DPhysicalPositionTo2DIndex(const DenseVector<ScalarType, 2>& Position) const
{
	return m_ImageData->Transform2DPhysicalPositionTo2DIndex(Position[0], Position[1]);
}


template<typename PixelType>
inline
const PixelType& SparseImage2D<PixelType>::operator[](int_max LinearIndex) const
{
#if defined MDK_DEBUG_2DSparseImage_Operator_CheckBound
	
	auto PixelNumber = m_ImageData->m_Size[0] * m_ImageData->m_Size[1];
    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ SparseImage2D::operator(LinearIndex)")
        return m_ImageData->m_Pixel_OutsideImage;
    }

#endif //MDK_DEBUG_2DSparseImage_Operator_CheckBound

	return m_ImageData->GetPixel(LinearIndex);
}


template<typename PixelType>
inline
const PixelType& SparseImage2D<PixelType>::operator()(int_max LinearIndex) const
{
	return this->operator[](LinearIndex);
}


template<typename PixelType>
inline
const PixelType& SparseImage2D<PixelType>::operator()(int_max xIndex, int_max yIndex) const
{
#if defined MDK_DEBUG_2DSparseImage_Operator_CheckBound

    auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0)
	{
        MDK_Error("Invalid input @ SparseImage2D::operator(xIndex, yIndex) const")
        return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_2DSparseImage_Operator_CheckBound

	return return m_ImageData->GetPixel(xIndex, yIndex, zIndex);
}


template<typename PixelType>
template<typename ScalarType>
const PixelType& SparseImage2D<PixelType>::GetPixelNearestTo2DIndex(ScalarType xIndex, ScalarType yIndex) const
{
	if (this->IsEmpty() == true)
	{
		MDK_Error("Self is empty @ SparseImage2D::GetPixelNearestTo2DIndex(...)")
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

	return m_ImageData->GetPixel(x0, y0);
}


template<typename PixelType>
template<typename ScalarType>
const PixelType& SparseImage2D<PixelType>::GetPixelNearestTo2DIndex(const DenseVector<ScalarType, 2>& Index2D) const
{
	return this->GetPixelNearestTo2DIndex(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& SparseImage2D<PixelType>::GetPixelNearestTo2DPhysicalPosition(ScalarType x, ScalarType y) const
{
	auto Index2D = this->Transform2DPhysicalPositionTo2DIndex(x, y);
	return this->GetPixelNearestTo2DIndex(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& SparseImage2D<PixelType>::GetPixelNearestTo2DPhysicalPosition(const DenseVector<ScalarType, 2>& Position) const
{
	return this->GetPixelNearestTo2DPhysicalPosition(Position[0], Position[1]);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType SparseImage2D<PixelType>::
GetPixelAt2DIndex(ScalarType xIndex, ScalarType yIndex, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt2DContinuousIndex<OutputPixelType>(*this, xIndex, yIndex, Option);
}


template<typename PixelType>
const PixelType& SparseImage2D<PixelType>::GetPixelAt2DIndex(int_max xIndex, int_max yIndex, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt2DContinuousIndex_Nearest<PixelType>(*this, xIndex, yIndex, Option);
}


template<typename PixelType>
const PixelType& SparseImage2D<PixelType>::GetPixelAt2DIndex(int xIndex, int yIndex, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(int_max(xIndex), int_max(yIndex), Option);
}


template<typename PixelType>
const PixelType& SparseImage2D<PixelType>::GetPixelAt2DIndex(long xIndex, long yIndex, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(int_max(xIndex), int_max(yIndex), Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType SparseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<ScalarType, 2>& Index2D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex<OutputPixelType>(Index2D[0], Index2D[1], Option);
}


template<typename PixelType>
const PixelType& SparseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<int_max, 2>& Index2D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(Index2D[0], Index2D[1], Option);
}


template<typename PixelType>
const PixelType& SparseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<int, 2>& Index2D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(int_max(Index2D[0]), int_max(Index2D[1]), Option);
}


template<typename PixelType>
const PixelType& SparseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<long, 2>& Index2D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt2DIndex(int_max(Index2D[0]), int_max(Index2D[1]), Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType SparseImage2D<PixelType>::
GetPixelAt2DPhysicalPosition(ScalarType x, ScalarType y, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt2DPhysicalPosition<OutputPixelType>(*this, x, y, Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType SparseImage2D<PixelType>::
GetPixelAt2DPhysicalPosition(const DenseVector<ScalarType, 2>& Position, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt2DPhysicalPosition<OutputPixelType>(*this, Position[0], Position[1], Option);
}


template<typename PixelType>
inline 
bool SparseImage2D<PixelType>::SetPixelAtLinearIndex(int_max LinearIndex, const PixelType& Pixel)
{
	auto Size = this->GetSize();
	auto PixelNumber = Size[0] * Size[1];
	if (LinearIndex >= PixelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ SparseImage2D::SetPixelAtLinearIndex(...)")
		return false;
	}
	m_ImageData->SetPixel(LinearIndex, Pixel);
	return true;
}


template<typename PixelType>
inline bool SparseImage2D<PixelType>::SetPixelAt2DIndex(int_max xIndex, int_max yIndex, const PixelType& Pixel)
{
	auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0)
	{
		MDK_Error("Invalid input @ SparseImage2D::SetPixelAt2DIndex(...)")
		return false;
	}
	m_ImageData->SetPixel(xIndex, yIndex, Pixel);
	return true;
}

}//end namespace mdk

#endif