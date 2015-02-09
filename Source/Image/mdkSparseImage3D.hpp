#ifndef mdk_SparseImage3D_hpp
#define mdk_SparseImage3D_hpp


namespace mdk
{

template<typename PixelType>
SparseImageData3D<PixelType>::SparseImageData3D()
{
    this->Clear();
}


template<typename PixelType>
SparseImageData3D<PixelType>::~SparseImageData3D()
{
}


template<typename PixelType>
inline
void SparseImageData3D<PixelType>::Clear()
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

	m_Orientation.Clear();
    m_Orientation.Resize(3, 3);
    m_Orientation.FixSize();
    m_Orientation.FillDiagonal(1.0);

    m_DataMap.clear();
  
	m_Pixel_OutsideImage = PixelType(0);
}


template<typename PixelType>
inline
int_max SparseImageData3D<PixelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;    
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> SparseImageData3D<PixelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex) const
{
	auto divresult = div(LinearIndex, m_PixelNumberPerZSlice);

	auto zIndex = divresult.quot; // z

	divresult = div(divresult.rem, m_Size[0]);

	auto yIndex = divresult.quot; // y
	auto xIndex = divresult.rem; // x

	DenseVector<ScalarType, 3> Index3D;
	Index3D[0] = ScalarType(xIndex);
	Index3D[1] = ScalarType(yIndex);
	Index3D[2] = ScalarType(zIndex);
	return Index3D;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> SparseImageData3D<PixelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const
{      
	DenseVector<ScalarType, 3> Position;
	auto Index3D = this->TransformLinearIndexTo3DIndex(LinearIndex);
	Position[0] = ScalarType(m_Origin[0] + double(Index3D[0]) * m_Spacing[0]);
	Position[1] = ScalarType(m_Origin[1] + double(Index3D[1]) * m_Spacing[1]);
	Position[2] = ScalarType(m_Origin[2] + double(Index3D[2]) * m_Spacing[2]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3> SparseImageData3D<PixelType>::
Transform3DIndexTo3DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const
{
	DenseVector<ScalarType_Position, 3> Position;
	Position[0] = m_Origin[0] + double(xIndex) * m_Spacing[0];
	Position[1] = m_Origin[1] + double(yIndex) * m_Spacing[1];
	Position[2] = m_Origin[2] + double(zIndex) * m_Spacing[2];
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> SparseImageData3D<PixelType>::Transform3DPhysicalPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> Index3D;
	Index3D[0] = (double(x) - m_Origin[0]) / m_Spacing[0];
	Index3D[1] = (double(y) - m_Origin[1]) / m_Spacing[1];
	Index3D[2] = (double(z) - m_Origin[2]) / m_Spacing[2];
	return Index3D;
}

//========================================================== SparseImage ========================================================================//

template<typename PixelType>
SparseImage3D<PixelType>::SparseImage3D()
{
    m_ImageData = std::make_shared<SparseImageData3D<PixelType>>();
}


template<typename PixelType>
SparseImage3D<PixelType>::SparseImage3D(const SparseImage3D<PixelType>& InputSparseImage)
{
    this->Copy(InputSparseImage);
}


template<typename PixelType>
SparseImage3D<PixelType>::SparseImage3D(SparseImage3D<PixelType>&& InputSparseImage)
{
	m_ImageData = std::move(InputSparseImage.m_ImageData);
}


template<typename PixelType>
SparseImage3D<PixelType>::~SparseImage3D()
{
}


template<typename PixelType>
void SparseImage3D<PixelType>::operator=(const SparseImage3D<PixelType>& InputImage)
{
    this->Copy(InputImage);
}


template<typename PixelType>
void SparseImage3D<PixelType>::operator=(SparseImage3D<PixelType>&& InputImage)
{
	this->Copy(std::move(InputImage));
}


template<typename PixelType>
template<typename PixelType_Input>
void SparseImage3D<PixelType>::Copy(const SparseImage3D<PixelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        MDK_Warning("try to Copy self @ 3DSparseImage::Copy(InputSparseImage)")
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
		m_ImageData = std::make_shared<SparseImageData3D<PixelType>>();
	}

    this->SetSize(InputSparseImage.GetSize());
    this->SetSpacing(InputSparseImage.GetSpacing());
    this->SetOrigin(InputSparseImage.GetOrigin);
    this->SetOrientation(InputSparseImage.GetOrientation());
	this->CopyPixelData(InputSparseImage);
}


template<typename PixelType>
template<typename PixelType_Input>
bool SparseImage3D<PixelType>::Copy(const SparseImage3D<PixelType_Input>* InputSparseImage)
{
    if (InputSparseImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DSparseImage::Copy(SparseImage* InputSparseImage)")
        return false;
    }

    this->Copy(*InputSparseImage);

    return true;
}


template<typename PixelType>
template<typename PixelType_Input>
void SparseImage3D<PixelType>::CopyPixelData(const SparseImage3D<PixelType_Input>& InputImage)
{
	for (auto it = InputImage.m_DataMap.begin(); it != InputImage.m_DataMap.end(); ++it)
	{
		m_DataMap[it->first] = PixelType(it->second);
	}
}


template<typename PixelType>
void SparseImage3D<PixelType>::Copy(SparseImage3D<PixelType>&& InputSparseImage)
{
	if (!m_ImageData)
	{
		m_ImageData = std::make_shared<SparseImageData3D<PixelType>>();
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

	m_ImageData->m_DataMap = std::move(InputSparseImage.m_ImageData->m_DataMap);

	m_ImageData->m_Pixel_OutsideImage = InputSparseImage.m_ImageData->m_Pixel_OutsideImage;

	InputSparseImage.Clear();
}


template<typename PixelType>
bool SparseImage3D<PixelType>::Share(SparseImage3D<PixelType>& InputSparseImage)
{
    m_ImageData = InputSparseImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
    return true;
}


template<typename PixelType>
bool SparseImage3D<PixelType>::Share(SparseImage3D<PixelType>* InputSparseImage)
{
    if (InputSparseImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DSparseImage::Share(SparseImage* InputSparseImage)")
        return false;
    }

    return this->Share(*InputSparseImage);
}


template<typename PixelType>
void SparseImage3D<PixelType>::ForceShare(const SparseImage3D<PixelType>& InputSparseImage)
{
    m_ImageData = InputSparseImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
}


template<typename PixelType>
bool SparseImage3D<PixelType>::ForceShare(const SparseImage3D<PixelType>* InputSparseImage)
{
    if (InputSparseImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DSparseImage::ForceShare(SparseImage* InputSparseImage)")
        return false;
    }

    return this->ForceShare(*InputSparseImage);
}


template<typename PixelType>
void SparseImage3D<PixelType>::Clear()
{
	if (m_ImageData)
	{
		m_ImageData->Clear();
	}
}


template<typename PixelType>
inline
bool SparseImage3D<PixelType>::IsEmpty() const
{
	if (m_ImageData)
	{
		return (m_ImageData->m_DataMap.size() == 0);
	}
	{
		return true;
	}
}


template<typename PixelType>
inline
Image3DInfo SparseImage3D<PixelType>::GetInfo() const
{
	Image3DInfo Info;
	Info.Origin = this->GetOrigin();
	Info.Spacing = this->GetSpacing();
	Info.Size = this->GetSize();
	Info.Orientation = this->GetOrientation();
	return Info;
}


template<typename PixelType>
inline
DenseVector<int_max, 3> SparseImage3D<PixelType>::GetSize() const
{
	return m_ImageData->m_Size;
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::GetSize(int_max& Lx, int_max& Ly, int_max& Lz) const
{
    Lx = m_ImageData->m_Size[0];
    Ly = m_ImageData->m_Size[1];
    Lz = m_ImageData->m_Size[2];
}


template<typename PixelType>
inline
bool SparseImage3D<PixelType>::SetSize(const DenseVector<int_max, 3>& Size)
{
    return this->SetSize(Size[0], Size[1], Size[2]);
}


template<typename PixelType>
inline
bool SparseImage3D<PixelType>::SetSize(int_max Lx, int_max Ly, int_max Lz)
{
    if (Lx < 0 || Ly < 0 || Lz < 0)
    {
        MDK_Error("Ivalid input @ 3DSparseImage::SetSize(...)")
        return false;
    }

	if (Lx == m_ImageData->m_Size[0] && Ly == m_ImageData->m_Size[1] && Lz == m_ImageData->m_Size[2])
	{
		return true;
	}

    if (Lx == 0 || Ly == 0 || Lz == 0)
    {
        m_ImageData->m_DataMap.clear();
        m_ImageData->m_Size[0] = 0;
        m_ImageData->m_Size[1] = 0;
        m_ImageData->m_Size[2] = 0;
        m_ImageData->m_PixelNumberPerZSlice = 0;
        return true;
    }

    m_ImageData->m_Size[0] = Lx;
    m_ImageData->m_Size[1] = Ly;
    m_ImageData->m_Size[2] = Lz;
    m_ImageData->m_PixelNumberPerZSlice = Ly*Lx;
    return true;
}


template<typename PixelType>
inline
DenseVector<double, 3> SparseImage3D<PixelType>::GetSpacing() const
{
	return m_ImageData->m_Spacing;
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::GetSpacing(double& Spacing_x, double& Spacing_y, double& Spacing_z) const
{
    Spacing_x = m_ImageData->m_Spacing[0];
    Spacing_y = m_ImageData->m_Spacing[1];
    Spacing_z = m_ImageData->m_Spacing[2];
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::SetSpacing(const DenseVector<double, 3>& Spacing)
{
    this->SetSpacing(Spacing[0], Spacing[1], Spacing[2]);
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::SetSpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
	auto Zero = std::numeric_limits<double>::epsilon();
	if (Spacing_x <= Zero || Spacing_y <= Zero || Spacing_z <= Zero)
    {
        MDK_Error("Invalid input (<= eps) @ 3DSparseImage::SetSpacing(...)")
        return;
    }

    m_ImageData->m_Spacing[0] = Spacing_x;
    m_ImageData->m_Spacing[1] = Spacing_y;
    m_ImageData->m_Spacing[2] = Spacing_z;
}


template<typename PixelType>
inline
DenseVector<double, 3> SparseImage3D<PixelType>::GetOrigin() const
{
	return m_ImageData->m_Origin;
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const
{
    Origin_x = m_ImageData->m_Origin[0];
    Origin_y = m_ImageData->m_Origin[1];
    Origin_z = m_ImageData->m_Origin[2];
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::SetOrigin(const DenseVector<double, 3>& Origin)
{
    this->SetOrigin(Origin[0], Origin[1], Origin[2]);
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::SetOrigin(double Origin_x, double Origin_y, double Origin_z)
{
    m_ImageData->m_Origin[0] = Origin_x;
    m_ImageData->m_Origin[1] = Origin_y;
    m_ImageData->m_Origin[2] = Origin_z;
}


template<typename PixelType>
inline 
const DenseMatrix<double>& SparseImage3D<PixelType>::GetOrientation() const
{
    return m_ImageData->m_Orientation;
}


template<typename PixelType>
inline 
void SparseImage3D<PixelType>::SetOrientation(const DenseMatrix<double>& Orientation)
{
    if (Orientation.IsEmpty() == false)
    {
        if (Orientation.GetColNumber() == 3 && Orientation.GetRowNumber() == 3)
        {
            m_ImageData->m_Orientation = Orientation;
        }
        else
        {
            MDK_Error("Invalid input Orientation @ 3DSparseImage::SetOrientation(...)")
        }
    }
}


template<typename PixelType>
inline
DenseVector<double, 3> SparseImage3D<PixelType>::GetPhysicalSize() const
{
	DenseVector<double, 3> Size;
    Size[0] = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];
    Size[1] = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
    Size[2] = m_ImageData->m_Size[2] * m_ImageData->m_Spacing[2];
    return Size;
}


template<typename PixelType>
inline 
void SparseImage3D<PixelType>::GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y, double& PhysicalSize_z) const
{
    PhysicalSize_x = m_ImageData->m_Size[0] * m_ImageData->m_Spacing[0];
    PhysicalSize_y = m_ImageData->m_Size[1] * m_ImageData->m_Spacing[1];
    PhysicalSize_z = m_ImageData->m_Size[2] * m_ImageData->m_Spacing[2];
}


template<typename PixelType>
inline
int_max SparseImage3D<PixelType>::GetRecordedPixelNumber() const
{
	return int_max(m_ImageData->m_DataMap.size());
}
 

template<typename PixelType>
inline
int_max SparseImage3D<PixelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return m_ImageData->Transform3DIndexToLinearIndex(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
int_max SparseImage3D<PixelType>::Transform3DIndexToLinearIndex(const DenseVector<int_max, 3>& Index3D) const
{
	return m_ImageData->Transform3DIndexToLinearIndex(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> SparseImage3D<PixelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo3DIndex<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> SparseImage3D<PixelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo3DPhysicalPosition<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3> SparseImage3D<PixelType>::
Transform3DIndexTo3DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const
{
	return m_ImageData->Transform3DIndexTo3DPhysicalPosition<ScalarType_Position>(xIndex, yIndex, zIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3> SparseImage3D<PixelType>::Transform3DIndexTo3DPhysicalPosition(const DenseVector<ScalarType_Index, 3>& Index3D) const
{
	return m_ImageData->Transform3DIndexTo3DPhysicalPosition<ScalarType_Position>(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> SparseImage3D<PixelType>::Transform3DPhysicalPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	return m_ImageData->Transform3DPhysicalPositionTo3DIndex(x, y, z);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> SparseImage3D<PixelType>::Transform3DPhysicalPositionTo3DIndex(const DenseVector<ScalarType, 3>& Position) const
{
	return m_ImageData->Transform3DPhysicalPositionTo3DIndex(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 3> SparseImage3D<PixelType>::Transform3DPhysicalPositionToNearest3DDiscreteIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index3D = m_ImageData->Transform3DPhysicalPositionTo3DIndex(x, y, z);
	auto x_Index = int_max(std::round(Index3D[0]));
	auto y_Index = int_max(std::round(Index3D[1]));
	auto z_Index = int_max(std::round(Index3D[2]));
	DenseVector<int_max, 3> Index3D_Nearest;
	Index3D_Nearest[0] = x_Index;
	Index3D_Nearest[1] = y_Index;
	Index3D_Nearest[2] = z_Index;
	return Index3D_Nearest;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 3> SparseImage3D<PixelType>::Transform3DPhysicalPositionToNearest3DDiscreteIndex(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DPhysicalPositionToNearest3DDiscreteIndex(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 3> SparseImage3D<PixelType>::Transform3DPhysicalPositionToNearest3DDiscreteIndexInsideImage(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index3D = m_ImageData->Transform3DPhysicalPositionTo3DIndex(x, y, z);
	auto Size = this->GetSize();
	auto x_Index = int_max(std::round(Index3D[0]));
	auto y_Index = int_max(std::round(Index3D[1]));
	auto z_Index = int_max(std::round(Index3D[2]));
	if (x_Index < 0)
	{
		x_Index = 0;
	}
	else if (x_Index >= Size[0])
	{
		x_Index = Size[0] - 1;
	}

	if (y_Index < 0)
	{
		y_Index = 0;
	}
	else if (y_Index >= Size[1])
	{
		y_Index = Size[1] - 1;
	}

	if (z_Index < 0)
	{
		z_Index = 0;
	}
	else if (z_Index >= Size[2])
	{
		z_Index = Size[2] - 1;
	}

	DenseVector<int_max, 3> Index3D_Inside;
	Index3D_Inside[0] = x_Index;
	Index3D_Inside[1] = y_Index;
	Index3D_Inside[2] = z_Index;
	return Index3D_Inside;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 3> SparseImage3D<PixelType>::Transform3DPhysicalPositionToNearest3DDiscreteIndexInsideImage(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DPhysicalPositionToNearest3DIndexInsideImage(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
bool SparseImage3D<PixelType>::CheckIf3DIndexIsInsideImage(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const
{
	auto Size = this->GetSize();
	auto Lx = double(Size[0]);
	auto Ly = double(Size[1]);
	auto Lz = double(Size[2]);
	auto x = double(xIndex);
	auto y = double(yIndex);
	auto z = double(zIndex);
	if (x < 0.0 || x >= Lx || y <= 0.0 || y >= Ly || z <= 0.0 || z >= Lz)
	{
		return false;
	}
	return true;
}


template<typename PixelType>
template<typename ScalarType>
inline
bool SparseImage3D<PixelType>::CheckIf3DIndexIsInsideImage(const DenseVector<ScalarType, 3>& Index3D) const
{
	return this->CheckIf3DIndexIsInsideImage(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
bool SparseImage3D<PixelType>::CheckIf3DPhysicalPositionIsInsideImage(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index3D = this->Transform3DPhysicalPositionTo3DIndex(x, y, z);
	return this->CheckIf3DIndexIsInsideImage(Index3D);
}


template<typename PixelType>
template<typename ScalarType>
inline
bool SparseImage3D<PixelType>::CheckIf3DPhysicalPositionIsInsideImage(const DenseVector<ScalarType, 3>& Position) const
{
	return this->CheckIf3DPhysicalPositionIsInsideImage(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
inline
bool SparseImage3D<PixelType>::CheckIfPixelIsStoredAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{// true, the pixel does exist at (xIndex, yIndex, zIndex)
	auto LinearIndex = this->Transform3DIndexToLinearIndex(xIndex, yIndex, zIndex);
	return this->CheckIfPixelIsStoredAtLinearIndex(LinearIndex);
}


template<typename PixelType>
inline
bool SparseImage3D<PixelType>::CheckIfPixelIsStoredAt3DIndex(const DenseVector<int_max, 3>& Index3D) const
{// true, the pixel does exist at (xIndex, yIndex, zIndex)
	return this->CheckIfPixelIsStoredAt3DIndex(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
inline 
bool SparseImage3D<PixelType>::CheckIfPixelIsStoredAtLinearIndex(int_max LinearIndex) const
{// true, the pixel does exist at (LinearIndex)
	auto it = m_ImageData->m_DataMap.find(LinearIndex);
	return (it != m_ImageData->m_DataMap.end());
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::SetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, PixelType Pixel)
{
#if defined MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
	{
		MDK_Error("Invalid input @ SparseImage3D::SetPixelAt3DIndex(...)")
		return;
	}

#endif //MDK_DEBUG_3DSparseImage_Operator_CheckBound
	
	auto LinearIndex = zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;
	m_ImageData->m_DataMap[LinearIndex] = std::move(Pixel);
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::SetPixelAt3DIndex(DenseVector<int_max, 3> Index3D, PixelType Pixel)
{
#if defined MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto Size = this->GetSize();
	if (Index3D[0] >= Size[0] || Index3D[0] < 0 || Index3D[1] >= Size[1] || Index3D[1] < 0 || Index3D[2] >= Size[2] || Index3D[2] < 0)
	{
		MDK_Error("Invalid input @ SparseImage3D::SetPixelAt3DIndex(...)")
		return;
	}

#endif //MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto LinearIndex = Index3D[2] * m_PixelNumberPerZSlice + Index3D[1] * m_Size[0] + Index3D[0];
	m_ImageData->m_DataMap[LinearIndex] = std::move(Pixel);
}


template<typename PixelType>
inline
void SparseImage3D<PixelType>::SetPixelAtLinearIndex(int_max LinearIndex, PixelType Pixel)
{
#if defined MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto Size = this->GetSize();
	auto PixelNumber = Size[0] * Size[1] * Size[2];
	if (LinearIndex >= PixelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ SparseImage3D::SetPixelAtLinearIndex(...)")
		return false;
	}

#endif //MDK_DEBUG_3DSparseImage_Operator_CheckBound

	m_ImageData->m_DataMap[LinearIndex] = std::move(Pixel);
}


template<typename PixelType>
inline
const PixelType& SparseImage3D<PixelType>::GetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
#if defined MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
	{
		MDK_Error("Invalid input @ SparseImage3D::operator(xIndex, yIndex, zIndex) const")
		return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto LinearIndex = zIndex*m_ImageData->m_PixelNumberPerZSlice + yIndex*m_ImageData->m_Size[0] + xIndex;
	return m_ImageData->m_DataMap[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& SparseImage3D<PixelType>::GetPixelAt3DIndex(DenseVector<int_max, 3> Index3D) const
{
	return this->GetPixelAt3DIndex(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
inline
const PixelType& SparseImage3D<PixelType>::GetPixelAtLinearIndex(int_max LinearIndex) const
{
#if defined MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto Size = this->GetSize();
	auto PixelNumber = Size[0] * Size[1] * Size[2];
	if (LinearIndex >= PixelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ SparseImage3D::GetPixelAtLinearIndex(...)")
		return false;
	}

#endif //MDK_DEBUG_3DSparseImage_Operator_CheckBound

	return m_ImageData->m_DataMap[LinearIndex];
}


template<typename PixelType>
inline
PixelType& SparseImage3D<PixelType>::operator[](int_max LinearIndex)
{
#if defined MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto Size = this->GetSize();
	auto PixelNumber = Size[0] * Size[1] * Size[2];
	if (LinearIndex >= PixelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ SparseImage3D::operator[](...)")
		return false;
	}

#endif //MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto it = m_ImageData->m_DataMap.find(LinearIndex);
	if (it == m_ImageData->m_DataMap.end())
	{
		m_ImageData->m_DataMap[LinearIndex] = m_ImageData->m_Pixel_OutsideImage;
	}
	return m_ImageData->m_DataMap[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& SparseImage3D<PixelType>::operator[](int_max LinearIndex) const
{
	return this->GetPixelAtLinearIndex(LinearIndex);
}


template<typename PixelType>
inline
PixelType& SparseImage3D<PixelType>::operator()(int_max LinearIndex)
{
	return this->operator[](LinearIndex);
}


template<typename PixelType>
inline
const PixelType& SparseImage3D<PixelType>::operator()(int_max LinearIndex) const
{
	return this->GetPixelAtLinearIndex(LinearIndex);
}


template<typename PixelType>
inline
PixelType& SparseImage3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex)
{
#if defined MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
	{
		MDK_Error("Invalid input @ SparseImage3D::operator(xIndex, yIndex, zIndex)")
		return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_3DSparseImage_Operator_CheckBound

	auto LinearIndex = zIndex*m_ImageData->m_PixelNumberPerZSlice + yIndex*m_ImageData->m_Size[0] + xIndex;
	auto it = m_ImageData->m_DataMap.find(LinearIndex);
	if (it == m_ImageData->m_DataMap.end())
	{
		m_ImageData->m_DataMap[LinearIndex] = m_ImageData->m_Pixel_OutsideImage;
	}
	return m_ImageData->m_DataMap[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& SparseImage3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
	return this->GetPixelAt3DIndex(xIndex, yIndex, zIndex);
}


template<typename PixelType>
template<typename ScalarType>
const PixelType& SparseImage3D<PixelType>::GetPixelNearestTo3DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const
{
	if (this->IsEmpty() == true)
	{
		MDK_Error("Self is empty @ SparseImage3D::GetPixelNearestTo3DIndex(...)")
		return m_ImageData->m_Pixel_OutsideImage;
	}

	auto Size = this->GetSize();

	auto x0 = int_max(std::round(xIndex));
	auto y0 = int_max(std::round(yIndex));
	auto z0 = int_max(std::round(zIndex));

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

	if (z0 < 0)
	{
		z0 = 0;
	}
	else if (z0 >= Size[2])
	{
		z0 = Size[2] - 1;
	}

	return m_ImageData->GetPixel(x0, y0, z0);
}


template<typename PixelType>
template<typename ScalarType>
const PixelType& SparseImage3D<PixelType>::GetPixelNearestTo3DIndex(const DenseVector<ScalarType, 3>& Index3D) const
{
	return this->GetPixelNearestTo3DIndex(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& SparseImage3D<PixelType>::GetPixelNearestTo3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index3D = this->Transform3DPhysicalPositionTo3DIndex(x, y, z);
	return this->GetPixelNearestTo3DIndex(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& SparseImage3D<PixelType>::GetPixelNearestTo3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position) const
{
	return this->GetPixelNearestTo3DPhysicalPosition(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType SparseImage3D<PixelType>::
GetPixelAt3DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt3DContinuousIndex<OutputPixelType>(*this, xIndex, yIndex, zIndex, Option);
}


template<typename PixelType>
const PixelType& SparseImage3D<PixelType>::GetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt3DContinuousIndex_Nearest<PixelType>(*this, xIndex, yIndex, zIndex, Option);
}


template<typename PixelType>
const PixelType& SparseImage3D<PixelType>::GetPixelAt3DIndex(int xIndex, int yIndex, int zIndex, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(int_max(xIndex), int_max(yIndex), int_max(zIndex), Option);
}


template<typename PixelType>
const PixelType& SparseImage3D<PixelType>::GetPixelAt3DIndex(long xIndex, long yIndex, long zIndex, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(int_max(xIndex), int_max(yIndex), int_max(zIndex), Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType SparseImage3D<PixelType>::
GetPixelAt3DIndex(const DenseVector<ScalarType, 3>& Index3D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex<OutputPixelType>(Index3D[0], Index3D[1], Index3D[2], Option);
}


template<typename PixelType>
const PixelType& SparseImage3D<PixelType>::
GetPixelAt3DIndex(const DenseVector<int_max, 3>& Index3D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(Index3D[0], Index3D[1], Index3D[2], Option);
}


template<typename PixelType>
const PixelType& SparseImage3D<PixelType>::
GetPixelAt3DIndex(const DenseVector<int, 3>& Index3D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(int_max(Index3D[0]), int_max(Index3D[1]), int_max(Index3D[2]), Option);
}


template<typename PixelType>
const PixelType& SparseImage3D<PixelType>::
GetPixelAt3DIndex(const DenseVector<long, 3>& Index3D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(int_max(Index3D[0]), int_max(Index3D[1]), int_max(Index3D[2]), Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType SparseImage3D<PixelType>::
GetPixelAt3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt3DPhysicalPosition<OutputPixelType>(*this, x, y, z, Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType SparseImage3D<PixelType>::
GetPixelAt3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt3DPhysicalPosition<OutputPixelType>(*this, Position[0], Position[1], Position[2], Option);
}


}//end namespace mdk

#endif