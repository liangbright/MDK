#ifndef mdk_DenseImage3D_hpp
#define mdk_DenseImage3D_hpp

namespace mdk
{

template<typename PixelType>
DenseImageData3D<PixelType>::DenseImageData3D()
{
    this->Clear();
}


template<typename PixelType>
DenseImageData3D<PixelType>::~DenseImageData3D()
{
}


template<typename PixelType>
inline
void DenseImageData3D<PixelType>::Clear()
{
    m_Size[0] = 0;
    m_Size[1] = 0;
    m_Size[2] = 0;

    m_PixelNumberPerZSlice = 0;

    m_LocalSys.Origin[0] = 0;
    m_LocalSys.Origin[1] = 0;
    m_LocalSys.Origin[2] = 0;

    m_LocalSys.Spacing[0] = 0;
    m_LocalSys.Spacing[1] = 0;
    m_LocalSys.Spacing[2] = 0;

	m_LocalSys.DirectionX[0] = 1;
	m_LocalSys.DirectionX[1] = 0;
	m_LocalSys.DirectionX[2] = 0;

	m_LocalSys.DirectionY[0] = 0;
	m_LocalSys.DirectionY[1] = 1;
	m_LocalSys.DirectionY[2] = 0;

	m_LocalSys.DirectionZ[0] = 0;
	m_LocalSys.DirectionZ[1] = 0;
	m_LocalSys.DirectionZ[2] = 1;

	m_Flag_Orientation_is_IdentityMatrix = true;

    m_DataArray.Clear();

	m_Pixel_OutsideImage = PixelType(0);
}


template<typename PixelType>
inline 
PixelType& DenseImageData3D<PixelType>::operator[](int_max LinearIndex)
{ 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData3D<PixelType>::operator[](int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
PixelType& DenseImageData3D<PixelType>::operator()(int_max LinearIndex)
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData3D<PixelType>::operator()(int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}
   

template<typename PixelType>
inline
PixelType& DenseImageData3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex)
{
    auto LinearIndex = zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;
 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    auto LinearIndex = zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;

    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
int_max DenseImageData3D<PixelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;    
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> DenseImageData3D<PixelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex) const
{
    auto divresult = std::div(LinearIndex, m_PixelNumberPerZSlice);

	auto zIndex = divresult.quot; // z
            
	divresult = std::div(divresult.rem, m_Size[0]);

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
DenseVector<ScalarType, 3> DenseImageData3D<PixelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const
{
	DenseVector<ScalarType, 3> Position;
	auto Index3D = this->TransformLinearIndexTo3DIndex(LinearIndex);
	Position[0] = ScalarType(m_LocalSys.Origin[0] + double(Index3D[0]) * m_LocalSys.Spacing[0]);
	Position[1] = ScalarType(m_LocalSys.Origin[1] + double(Index3D[1]) * m_LocalSys.Spacing[1]);
	Position[2] = ScalarType(m_LocalSys.Origin[2] + double(Index3D[2]) * m_LocalSys.Spacing[2]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3> DenseImageData3D<PixelType>::
Transform3DIndexTo3DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const
{
	DenseVector<ScalarType_Position, 3> Position;
	Position[0] = ScalarType_Position(m_LocalSys.Origin[0] + double(xIndex) * m_LocalSys.Spacing[0]);
	Position[1] = ScalarType_Position(m_LocalSys.Origin[1] + double(yIndex) * m_LocalSys.Spacing[1]);
	Position[2] = ScalarType_Position(m_LocalSys.Origin[2] + double(zIndex) * m_LocalSys.Spacing[2]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> DenseImageData3D<PixelType>::Transform3DPhysicalPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> Index3D;
	Index3D[0] = ScalarType((double(x) - m_LocalSys.Origin[0]) / m_LocalSys.Spacing[0]);
	Index3D[1] = ScalarType((double(y) - m_LocalSys.Origin[1]) / m_LocalSys.Spacing[1]);
	Index3D[2] = ScalarType((double(z) - m_LocalSys.Origin[2]) / m_LocalSys.Spacing[2]);
	return Index3D;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> DenseImageData3D<PixelType>::Transform3DPhysicalPositionFromLocalToGlobal(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> GlobalPosition;
	if (m_Flag_Orientation_is_IdentityMatrix == true)
	{
		GlobalPosition[0] = x;
		GlobalPosition[1] = y;
		GlobalPosition[2] = z;
	}
	else
	{ 
		GlobalPosition = ConvertPointCoordinateFromLocalToGlobal(x, y, z, m_LocalSys);
	}
	return GlobalPosition;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImageData3D<PixelType>::Transform3DPhysicalPositionFromGlobalToLocal(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> LocalPosition;
	if (m_Flag_Orientation_is_IdentityMatrix == true)
	{
		LocalPosition[0] = x;
		LocalPosition[1] = y;
		LocalPosition[2] = z;
	}
	else
	{
		LocalPosition = ConvertPointCoordinateFromGlobalToLocal(x, y, z, m_LocalSys);
	}
	return LocalPosition;
}

//========================================================== DenseImage ========================================================================//

template<typename PixelType>
DenseImage3D<PixelType>::DenseImage3D()
{
	m_ImageData = std::make_shared<DenseImageData3D<PixelType>>();
}


template<typename PixelType>
DenseImage3D<PixelType>::DenseImage3D(const DenseImage3D<PixelType>& InputImage)
{	
    this->Copy(InputImage);
}


template<typename PixelType>
DenseImage3D<PixelType>::DenseImage3D(DenseImage3D<PixelType>&& InputImage)
{
	m_ImageData = std::move(InputImage.m_ImageData);
}


template<typename PixelType>
DenseImage3D<PixelType>::~DenseImage3D()
{
}


template<typename PixelType>
void DenseImage3D<PixelType>::operator=(const DenseImage3D<PixelType>& InputImage)
{
    this->Copy(InputImage);
}


template<typename PixelType>
void DenseImage3D<PixelType>::operator=(DenseImage3D<PixelType>&& InputImage)
{
	this->Copy(std::move(InputImage));
}


template<typename PixelType>
template<typename PixelType_Input>
void DenseImage3D<PixelType>::Copy(const DenseImage3D<PixelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        MDK_Warning("try to Copy self @ 3DDenseImage::Copy(InputImage)")
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
		m_ImageData = std::make_shared<DenseImageData3D<PixelType>>();
	}

    this->SetSize(InputImage.GetSize());
    this->SetSpacing(InputImage.GetSpacing());
    this->SetOrigin(InputImage.GetOrigin());
    this->SetOrientation(InputImage.GetOrientation());
	this->CopyPixelData(InputImage.GetPixelPointer(), InputImage.GetPixelNumber());
}


template<typename PixelType>
template<typename PixelType_Input>
bool DenseImage3D<PixelType>::Copy(const DenseImage3D<PixelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DDenseImage::Copy(DenseImage* InputImage)")
        return false;
    }

    this->Copy(*InputImage);

    return true;
}


template<typename PixelType>
template<typename PixelType_Input>
bool DenseImage3D<PixelType>::CopyPixelData(const PixelType_Input* InputPixelPointer, int_max InputPixelNumber)
{
    if (InputPixelPointer == nullptr || InputPixelNumber <= 0)
	{
        //MDK_Error("Invalid input @ 3DDenseImage::CopyPixelData(...)")
		this->Clear();
		return true;
	}

    auto SelfPixelNumber = this->GetPixelNumber();

    if (SelfPixelNumber != InputPixelNumber)// must call SetSize() before this function
    {
        MDK_Error("Size does not match @ 3DDenseImage::CopyPixelData(...)")
        return false;
    }

    auto PixelPtr = this->GetPixelPointer();

    if (std::size_t(InputPixelPointer) == std::size_t(PixelPtr))
    {
        MDK_Warning("An DenseImage tries to Copy itself @ 3DDenseImage::CopyPixelData(...)")
        return true;
    }
  
    for (int_max i = 0; i < SelfPixelNumber; ++i)
	{
        PixelPtr[i] = PixelType(InputPixelPointer[i]);
	}

    return true;
}


template<typename PixelType>
void DenseImage3D<PixelType>::Copy(DenseImage3D<PixelType>&& InputImage)
{
	if (!m_ImageData)
	{
		m_ImageData = std::make_shared<DenseImageData3D<PixelType>>();
	}

	m_ImageData->m_Size[0] = InputImage.m_ImageData->m_Size[0];
	m_ImageData->m_Size[1] = InputImage.m_ImageData->m_Size[1];
	m_ImageData->m_Size[2] = InputImage.m_ImageData->m_Size[2];

	m_ImageData->m_PixelNumberPerZSlice = InputImage.m_ImageData->m_PixelNumberPerZSlice;

	m_ImageData->m_LocalSys = InputImage.m_ImageData->m_LocalSys;
	m_ImageData->m_Flag_Orientation_is_IdentityMatrix = InputImage.m_ImageData->m_Flag_Orientation_is_IdentityMatrix;

	m_ImageData->m_DataArray = std::move(InputImage.m_ImageData->m_DataArray);

	m_ImageData->m_Pixel_OutsideImage = InputImage.m_ImageData->m_Pixel_OutsideImage;

	InputImage.Clear();
}


template<typename PixelType>
bool DenseImage3D<PixelType>::Fill(const PixelType& Pixel)
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
bool DenseImage3D<PixelType>::Share(DenseImage3D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
    return true;
}


template<typename PixelType>
bool DenseImage3D<PixelType>::Share(DenseImage3D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DDenseImage::Share(DenseImage* InputImage)")
        return false;
    }

    return this->Share(*InputImage);
}


template<typename PixelType>
void DenseImage3D<PixelType>::ForceShare(const DenseImage3D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
}


template<typename PixelType>
bool DenseImage3D<PixelType>::ForceShare(const DenseImage3D<PixelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ 3DDenseImage::ForceShare(DenseImage* InputImage)")
        return false;
    }

    return this->ForceShare(*InputImage);
}


template<typename PixelType>
bool DenseImage3D<PixelType>::Share(PixelType* InputImage, const Image3DInfo& InputImageInfo)
{
	if (InputImage == nullptr)
	{
		MDK_Error("Input is nullptr @ DenseImage3D::Share(DenseImage* InputImage)")
		return false;
	}

	this->SetOrigin(InputImageInfo.Origin);
	this->SetSpacing(InputImageInfo.Spacing);
	this->SetOrientation(InputImageInfo.Orientation);
	//this->SetSize(XXX);
	m_ImageData->m_Size[0] = InputImageInfo.Size[0];
	m_ImageData->m_Size[1] = InputImageInfo.Size[1];
	m_ImageData->m_Size[2] = InputImageInfo.Size[2];
	m_ImageData->m_PixelNumberPerZSlice = InputImageInfo.Size[0] * InputImageInfo.Size[1];
	auto InputPixelNumber = InputImageInfo.Size[0] * InputImageInfo.Size[1] * InputImageInfo.Size[2];
	return m_ImageData->m_DataArray.Share(InputImage, InputPixelNumber, true);
}


template<typename PixelType>
bool DenseImage3D<PixelType>::ForceShare(const PixelType* InputImage, const Image3DInfo& InputImageInfo)
{
	return this->Share(const_cast<PixelType*>(InputImage), InputImageInfo);
}


template<typename PixelType>
void DenseImage3D<PixelType>::Clear()
{
	if (m_ImageData)
	{
		m_ImageData->Clear();
	}
}


template<typename PixelType>
inline
bool DenseImage3D<PixelType>::IsEmpty() const
{
	if (m_ImageData)
	{
		return (m_ImageData->m_DataArray.GetLength() == 0);
	}
	else
	{
		return true;
	}
}


template<typename PixelType>
inline
bool DenseImage3D<PixelType>::IsPureEmpty() const
{
	return (!m_ImageData);
}


template<typename PixelType>
inline
PixelType* DenseImage3D<PixelType>::GetPixelPointer()
{
	if (m_ImageData)
	{
		return m_ImageData->m_DataArray.GetPointer();
	}
	else
	{
		return nullptr;
	}
}


template<typename PixelType>
inline
const PixelType* DenseImage3D<PixelType>::GetPixelPointer() const
{
	if (m_ImageData)
	{
		return m_ImageData->m_DataArray.GetPointer();
	}
	else
	{
		return nullptr;
	}
}


template<typename PixelType>
inline
PixelType* DenseImage3D<PixelType>::GetPixelPointerOfZSlice(int_max ZSliceIndex)
{
	if (this->IsPureEmpty() == true)
	{
		return nullptr;
	}

	if (ZSliceIndex < 0 || ZSliceIndex >= m_ImageData->m_Size[2])
	{
		return nullptr;
	}

	auto PointerOfZSlice = this->GetPixelPointer() + ZSliceIndex*m_ImageData->m_PixelNumberPerZSlice;
	return PointerOfZSlice;
}


template<typename PixelType>
inline
const PixelType* DenseImage3D<PixelType>::GetPixelPointerOfZSlice(int_max ZSliceIndex) const
{
	if (this->IsPureEmpty() == true)
	{
		return nullptr;
	}

	if (ZSliceIndex < 0 || ZSliceIndex >= m_ImageData->m_Size[2])
	{
		return nullptr;
	}

	auto PointerOfZSlice = this->GetPixelPointer() + ZSliceIndex*m_ImageData->m_PixelNumberPerZSlice;
	return PointerOfZSlice;
}


template<typename PixelType>
inline
PixelType* DenseImage3D<PixelType>::begin()
{
	return this->GetPixelPointer();
}


template<typename PixelType>
inline
const PixelType* DenseImage3D<PixelType>::begin() const
{
	return this->GetPixelPointer();
}


template<typename PixelType>
inline
PixelType* DenseImage3D<PixelType>::end()
{
	auto BeginPtr = return this->GetPixelPointer();
	if (BeginPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto EndPtr = BeginPtr + this->GetPixelNumber();
		return EndPtr;
	}
}


template<typename PixelType>
inline
const PixelType* DenseImage3D<PixelType>::end() const
{
	auto BeginPtr = return this->GetPixelPointer();
	if (BeginPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto EndPtr = BeginPtr + this->GetPixelNumber();
		return EndPtr;
	}
}


template<typename PixelType>
inline 
Image3DInfo DenseImage3D<PixelType>::GetInfo() const
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
DenseVector<int_max, 3> DenseImage3D<PixelType>::GetSize() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_Size;
	}
	else
	{
		DenseVector<int_max, 3> EmptySize;
		EmptySize.Fill(0);
		return EmptySize;
	}
}


template<typename PixelType>
inline
void DenseImage3D<PixelType>::GetSize(int_max& Lx, int_max& Ly, int_max& Lz) const
{
	if (this->IsPureEmpty() == false)
	{
		Lx = m_ImageData->m_Size[0];
		Ly = m_ImageData->m_Size[1];
		Lz = m_ImageData->m_Size[2];
	}
	else
	{
		Lx = 0;
		Ly = 0;
		Lz = 0;
	}
}


template<typename PixelType>
inline
bool DenseImage3D<PixelType>::SetSize(const DenseVector<int_max, 3>& Size)
{
    return this->SetSize(Size[0], Size[1], Size[2]);
}


template<typename PixelType>
inline
bool DenseImage3D<PixelType>::SetSize(int_max Lx, int_max Ly, int_max Lz)
{
    if (Lx < 0 || Ly < 0 || Lz < 0)
    {
        MDK_Error("Ivalid input @ 3DDenseImage::SetSize(...)")
        return false;
    }

	if (this->IsPureEmpty() == true)
	{
		m_ImageData = std::make_shared<DenseImageData3D<PixelType>>();
	}

	if (Lx == m_ImageData->m_Size[0] && Ly == m_ImageData->m_Size[1] && Lz == m_ImageData->m_Size[2])
	{
		return true;
	}

    if (Lx == 0 || Ly == 0 || Lz == 0)
    {
        m_ImageData->m_DataArray.Clear();

        m_ImageData->m_Size[0] = 0;
        m_ImageData->m_Size[1] = 0;
        m_ImageData->m_Size[2] = 0;
        m_ImageData->m_PixelNumberPerZSlice = 0;

        return true;
    }

try
{
    m_ImageData->m_DataArray.Resize(Lx*Ly*Lz);
 
    m_ImageData->m_Size[0] = Lx;
    m_ImageData->m_Size[1] = Ly;
    m_ImageData->m_Size[2] = Lz;
    m_ImageData->m_PixelNumberPerZSlice = Ly*Lx;
}
catch (...)
{
    MDK_Error("Out Of Memory @ 3DDenseImage::SetSize(...)")

    this->Clear();
    return false;
}

    return true;
}


template<typename PixelType>
inline
DenseVector<double, 3> DenseImage3D<PixelType>::GetSpacing() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_LocalSys.Spacing;
	}
	else
	{
		DenseVector<double, 3> EmptySpacing;
		EmptySpacing.Fill(0);
		return EmptySpacing;
	}
}


template<typename PixelType>
inline
void DenseImage3D<PixelType>::GetSpacing(double& Spacing_x, double& Spacing_y, double& Spacing_z) const
{
	if (this->IsPureEmpty() == false)
	{
		Spacing_x = m_ImageData->m_LocalSys.Spacing[0];
		Spacing_y = m_ImageData->m_LocalSys.Spacing[1];
		Spacing_z = m_ImageData->m_LocalSys.Spacing[2];
	}
	else
	{
		Spacing_x = 0;
		Spacing_y = 0;
		Spacing_z = 0;
	}
}


template<typename PixelType>
inline
void DenseImage3D<PixelType>::SetSpacing(const DenseVector<double, 3>& Spacing)
{
    this->SetSpacing(Spacing[0], Spacing[1], Spacing[2]);
}


template<typename PixelType>
inline
void DenseImage3D<PixelType>::SetSpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
	auto Zero = std::numeric_limits<double>::epsilon();
	if (Spacing_x <= Zero || Spacing_y <= Zero || Spacing_z <= Zero)
    {
        MDK_Error("Invalid input (<= eps) @ 3DDenseImage::SetSpacing(...)")
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		m_ImageData = std::make_shared<DenseImageData3D<PixelType>>();
	}
    m_ImageData->m_LocalSys.Spacing[0] = Spacing_x;
    m_ImageData->m_LocalSys.Spacing[1] = Spacing_y;
    m_ImageData->m_LocalSys.Spacing[2] = Spacing_z;
}


template<typename PixelType>
inline
DenseVector<double, 3> DenseImage3D<PixelType>::GetOrigin() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_LocalSys.Origin;
	}
	else
	{
		DenseVector<double, 3> EmptyOrigin;
		EmptyOrigin.Fill(0);
		return EmptyOrigin;
	}
}


template<typename PixelType>
inline
void DenseImage3D<PixelType>::GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const
{
	if (this->IsPureEmpty() == false)
	{
		Origin_x = m_ImageData->m_LocalSys.Origin[0];
		Origin_y = m_ImageData->m_LocalSys.Origin[1];
		Origin_z = m_ImageData->m_LocalSys.Origin[2];
	}
	else
	{
		Origin_x = 0;
		Origin_y = 0;
		Origin_z = 0;
	}
}


template<typename PixelType>
inline
void DenseImage3D<PixelType>::SetOrigin(const DenseVector<double, 3>& Origin)
{
    this->SetOrigin(Origin[0], Origin[1], Origin[2]);
}


template<typename PixelType>
inline
void DenseImage3D<PixelType>::SetOrigin(double Origin_x, double Origin_y, double Origin_z)
{
	if (this->IsPureEmpty() == true)
	{
		m_ImageData = std::make_shared<DenseImageData3D<PixelType>>();
	}
    m_ImageData->m_LocalSys.Origin[0] = Origin_x;
    m_ImageData->m_LocalSys.Origin[1] = Origin_y;
    m_ImageData->m_LocalSys.Origin[2] = Origin_z;
}


template<typename PixelType>
inline 
DenseMatrix<double> DenseImage3D<PixelType>::GetOrientation() const
{
	DenseMatrix<double> Orientation(3, 3);
	if (this->IsPureEmpty() == false)
	{
		Orientation.SetCol(0, m_ImageData->m_LocalSys.DirectionX);
		Orientation.SetCol(1, m_ImageData->m_LocalSys.DirectionY);
		Orientation.SetCol(2, m_ImageData->m_LocalSys.DirectionZ);
	}
	else
	{
		Orientation.FillDiagonal(1);
	}
	return Orientation;
}


template<typename PixelType>
inline 
void DenseImage3D<PixelType>::SetOrientation(const DenseMatrix<double>& Orientation)
{
	if (Orientation.GetColNumber() != 3 || Orientation.GetRowNumber() != 3)
	{
		MDK_Error("Invalid input size @ 3DDenseImage::SetOrientation(...)")
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		m_ImageData = std::make_shared<DenseImageData3D<PixelType>>();
	}

	Orientation.GetCol(0, m_ImageData->m_LocalSys.DirectionX);
	Orientation.GetCol(1, m_ImageData->m_LocalSys.DirectionY);
	Orientation.GetCol(2, m_ImageData->m_LocalSys.DirectionZ);

	auto One = double(1);
	auto Zero = double(0);
	// test if exact match
	if (   m_ImageData->m_LocalSys.DirectionX[0] != One  || m_ImageData->m_LocalSys.DirectionX[1] != Zero || m_ImageData->m_LocalSys.DirectionX[2] != Zero
		|| m_ImageData->m_LocalSys.DirectionY[0] != Zero || m_ImageData->m_LocalSys.DirectionY[1] != One  || m_ImageData->m_LocalSys.DirectionY[2] != Zero
		|| m_ImageData->m_LocalSys.DirectionZ[0] != Zero || m_ImageData->m_LocalSys.DirectionZ[1] != Zero || m_ImageData->m_LocalSys.DirectionZ[2] != One)
	{
		m_ImageData->m_Flag_Orientation_is_IdentityMatrix = false;
	}
	else
	{
		m_ImageData->m_Flag_Orientation_is_IdentityMatrix = true;
	}
}


template<typename PixelType>
inline
DenseVector<double, 3> DenseImage3D<PixelType>::GetPhysicalSize() const
{
	DenseVector<double, 3> Size;
	if (this->IsPureEmpty() == false)
	{
		Size[0] = double(m_ImageData->m_Size[0]) * m_ImageData->m_LocalSys.Spacing[0];
		Size[1] = double(m_ImageData->m_Size[1]) * m_ImageData->m_LocalSys.Spacing[1];
		Size[2] = double(m_ImageData->m_Size[2]) * m_ImageData->m_LocalSys.Spacing[2];
	}
	else
	{
		Size[0] = 0;
		Size[1] = 0;
		Size[2] = 0;
	}
    return Size;
}


template<typename PixelType>
inline 
void DenseImage3D<PixelType>::GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y, double& PhysicalSize_z) const
{
	if (this->IsPureEmpty() == false)
	{
		PhysicalSize_x = double(m_ImageData->m_Size[0]) * m_ImageData->m_LocalSys.Spacing[0];
		PhysicalSize_y = double(m_ImageData->m_Size[1]) * m_ImageData->m_LocalSys.Spacing[1];
		PhysicalSize_z = double(m_ImageData->m_Size[2]) * m_ImageData->m_LocalSys.Spacing[2];
	}
	else
	{
		PhysicalSize_x = 0;
		PhysicalSize_y = 0;
		PhysicalSize_z = 0;
	}
}


template<typename PixelType>
inline
int_max DenseImage3D<PixelType>::GetPixelNumber() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_PixelNumberPerZSlice * m_ImageData->m_Size[2];
	}
	else
	{
		return 0;
	}
}
 

template<typename PixelType>
inline
int_max DenseImage3D<PixelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return m_ImageData->Transform3DIndexToLinearIndex(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
int_max DenseImage3D<PixelType>::Transform3DIndexToLinearIndex(const DenseVector<int_max, 3>& Index3D) const
{
	return m_ImageData->Transform3DIndexToLinearIndex(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImage3D<PixelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo3DIndex<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImage3D<PixelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo3DPhysicalPosition<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3> DenseImage3D<PixelType>::
Transform3DIndexTo3DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const
{
	return m_ImageData->Transform3DIndexTo3DPhysicalPosition<ScalarType_Position>(xIndex, yIndex, zIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3> DenseImage3D<PixelType>::Transform3DIndexTo3DPhysicalPosition(const DenseVector<ScalarType_Index, 3>& Index3D) const
{
	return m_ImageData->Transform3DIndexTo3DPhysicalPosition<ScalarType_Position>(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	return m_ImageData->Transform3DPhysicalPositionTo3DIndex(x, y, z);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionTo3DIndex(const DenseVector<ScalarType, 3>& Position) const
{
	return m_ImageData->Transform3DPhysicalPositionTo3DIndex(Position[0], Position[1], Position[2]);
}

template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionToNearest3DDiscreteIndex(ScalarType x, ScalarType y, ScalarType z) const
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
DenseVector<int_max, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionToNearest3DDiscreteIndex(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DPhysicalPositionToNearest3DDiscreteIndex(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<int_max, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionToNearest3DDiscreteIndexInsideImage(ScalarType x, ScalarType y, ScalarType z) const
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
DenseVector<int_max, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionToNearest3DDiscreteIndexInsideImage(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DPhysicalPositionToNearest3DDiscreteIndexInsideImage(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionFromLocalToGlobal(ScalarType x, ScalarType y, ScalarType z) const
{
	return m_ImageData->Transform3DPhysicalPositionFromLocalToGlobal(x, y, z);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionFromLocalToGlobal(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DPhysicalPositionFromLocalToGlobal(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionFromGlobalToLocal(ScalarType x, ScalarType y, ScalarType z) const
{
	return m_ImageData->Transform3DPhysicalPositionFromGlobalToLocal(x, y, z);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImage3D<PixelType>::Transform3DPhysicalPositionFromGlobalToLocal(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DPhysicalPositionFromGlobalToLocal(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
bool DenseImage3D<PixelType>::CheckIf3DIndexIsInsideImage(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const
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
bool DenseImage3D<PixelType>::CheckIf3DIndexIsInsideImage(const DenseVector<ScalarType, 3>& Index3D) const
{
	return this->CheckIf3DIndexIsInsideImage(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline 
bool DenseImage3D<PixelType>::CheckIf3DPhysicalPositionIsInsideImage(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index3D = this->Transform3DPhysicalPositionTo3DIndex(x, y, z);
	return this->CheckIf3DIndexIsInsideImage(Index3D);
}


template<typename PixelType>
template<typename ScalarType>
inline 
bool DenseImage3D<PixelType>::CheckIf3DPhysicalPositionIsInsideImage(const DenseVector<ScalarType, 3>& Position) const
{
	return this->CheckIf3DPhysicalPositionIsInsideImage(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
inline 
void DenseImage3D<PixelType>::SetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, PixelType Pixel)
{
	(*this)(xIndex, yIndex, zIndex) = std::move(Pixel);
}


template<typename PixelType>
inline 
void DenseImage3D<PixelType>::SetPixelAt3DIndex(DenseVector<int_max, 3> Index3D, PixelType Pixel)
{
	(*this)(Index3D[0], Index3D[1], Index3D[2]) = std::move(Pixel);
}


template<typename PixelType>
inline
void DenseImage3D<PixelType>::SetPixelAtLinearIndex(int_max LinearIndex, PixelType Pixel)
{
	(*this)(LinearIndex) = std::move(Pixel);
}


template<typename PixelType>
inline
const PixelType& DenseImage3D<PixelType>::GetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
	return (*this)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
const PixelType& DenseImage3D<PixelType>::GetPixelAt3DIndex(DenseVector<int_max, 3> Index3D) const
{
	return (*this)(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
inline 
const PixelType& DenseImage3D<PixelType>::GetPixelAtLinearIndex(int_max LinearIndex) const
{
	return (*this)(LinearIndex);
}


template<typename PixelType>
inline
PixelType& DenseImage3D<PixelType>::operator[](int_max LinearIndex)
{
#if defined MDK_DEBUG_DenseImage3D_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();
    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ DenseImage3D::operator(LinearIndex)")
        return m_ImageData->m_Pixel_OutsideImage;
    }

#endif //MDK_DEBUG_DenseImage3D_Operator_CheckBound

	return (*m_ImageData)[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImage3D<PixelType>::operator[](int_max LinearIndex) const
{
#if defined MDK_DEBUG_DenseImage3D_Operator_CheckBound

    auto PixelNumber = this->GetPixelNumber();
    if (LinearIndex >= PixelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ DenseImage3D::operator(LinearIndex)")
        return m_ImageData->m_Pixel_OutsideImage;
    }

#endif //MDK_DEBUG_DenseImage3D_Operator_CheckBound

	return (*m_ImageData)[LinearIndex];
}


template<typename PixelType>
inline
PixelType& DenseImage3D<PixelType>::operator()(int_max LinearIndex)
{
	return this->operator[](LinearIndex);
}


template<typename PixelType>
inline
const PixelType& DenseImage3D<PixelType>::operator()(int_max LinearIndex) const
{
	return this->operator[](LinearIndex);
}


template<typename PixelType>
inline
PixelType& DenseImage3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex)
{
#if defined MDK_DEBUG_DenseImage3D_Operator_CheckBound

    auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
	{
		MDK_Error("Invalid input @ DenseImage3D::operator(xIndex, yIndex, zIndex)")
		return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_DenseImage3D_Operator_CheckBound
	
    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
const PixelType& DenseImage3D<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
#if defined MDK_DEBUG_DenseImage3D_Operator_CheckBound

    auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0 || zIndex >= Size[2] || zIndex < 0)
	{
        MDK_Error("Invalid input @ DenseImage3D::operator(xIndex, yIndex, zIndex) const")
        return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_DenseImage3D_Operator_CheckBound

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename PixelType>
template<typename ScalarType>
const PixelType& DenseImage3D<PixelType>::GetPixelNearestTo3DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const
{
	if (this->IsEmpty() == true)
	{
		MDK_Error("Self is empty @ DenseImage3D:: GetPixelNearestTo3DIndex(...)")
		return m_ImageData->m_Pixel_OutsideImage;
	}

	auto Size = this->GetSize();

	auto x = int_max(std::round(xIndex));
	auto y = int_max(std::round(yIndex));
	auto z = int_max(std::round(zIndex));

	if (x < 0)
	{
		x = 0;
	}
	else if (x >= Size[0])
	{
		x = Size[0] - 1;
	}

	if (y < 0)
	{
		y = 0;
	}
	else if (y >= Size[1])
	{
		y = Size[1] - 1;
	}

	if (z < 0)
	{
		z = 0;
	}
	else if (z >= Size[2])
	{
		z = Size[2] - 1;
	}

	return (*m_ImageData)(x, y, z);
}


template<typename PixelType>
template<typename ScalarType>
const PixelType& DenseImage3D<PixelType>::GetPixelNearestTo3DIndex(const DenseVector<ScalarType, 3>& Index3D) const
{
	return this->GetPixelNearestTo3DIndex(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& DenseImage3D<PixelType>::GetPixelNearestTo3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index3D = this->Transform3DPhysicalPositionTo3DIndex(x, y, z);
	return this->GetPixelNearestTo3DIndex(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& DenseImage3D<PixelType>::GetPixelNearestTo3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position) const
{
	return this->GetPixelNearestTo3DPhysicalPosition(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage3D<PixelType>::
GetPixelAt3DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt3DContinuousIndex<OutputPixelType>(*this, xIndex, yIndex, zIndex, Option);
}


template<typename PixelType>
PixelType DenseImage3D<PixelType>::GetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt3DContinuousIndex_Nearest<PixelType>(*this, xIndex, yIndex, zIndex, Option);
}


template<typename PixelType>
PixelType DenseImage3D<PixelType>::GetPixelAt3DIndex(int xIndex, int yIndex, int zIndex, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(int_max(xIndex), int_max(yIndex), int_max(zIndex), Option);
}


template<typename PixelType>
PixelType DenseImage3D<PixelType>::GetPixelAt3DIndex(long xIndex, long yIndex, long zIndex, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(int_max(xIndex), int_max(yIndex), int_max(zIndex), Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage3D<PixelType>::
GetPixelAt3DIndex(const DenseVector<ScalarType, 3>& Index3D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex<OutputPixelType>(Index3D[0], Index3D[1], Index3D[2], Option);
}


template<typename PixelType>
PixelType DenseImage3D<PixelType>::
GetPixelAt3DIndex(const DenseVector<int_max, 3>& Index3D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(Index3D[0], Index3D[1], Index3D[2], Option);
}


template<typename PixelType>
PixelType DenseImage3D<PixelType>::
GetPixelAt3DIndex(const DenseVector<int, 3>& Index3D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(int_max(Index3D[0]), int_max(Index3D[1]), int_max(Index3D[2]), Option);
}


template<typename PixelType>
PixelType DenseImage3D<PixelType>::
GetPixelAt3DIndex(const DenseVector<long, 3>& Index3D, const InterpolationOptionType& Option) const
{
	return this->GetPixelAt3DIndex(int_max(Index3D[0]), int_max(Index3D[1]), int_max(Index3D[2]), Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage3D<PixelType>::
GetPixelAt3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt3DPhysicalPosition<OutputPixelType>(*this, x, y, z, Option);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage3D<PixelType>::
GetPixelAt3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position, const InterpolationOptionType& Option) const
{
	return InterpolateImageAt3DPhysicalPosition<OutputPixelType>(*this, Position[0], Position[1], Position[2], Option);
}


template<typename PixelType>
DenseVector<int_max>
DenseImage3D<PixelType>::GetLinearIndexListInRegion(const BoxRegionOf3DIndexInImage3D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	auto xIndex_s = int_max(RegionInfo.x_min);
	auto yIndex_s = int_max(RegionInfo.y_min);
	auto zIndex_s = int_max(RegionInfo.z_min);
	auto xIndex_e = int_max(RegionInfo.x_max);
	auto yIndex_e = int_max(RegionInfo.y_max);
	auto zIndex_e = int_max(RegionInfo.z_max);

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s || zIndex_e < zIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage3D::GetLinearIndexListInRegion(...)")
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}
	
	auto Size = this->GetSize();
	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	zIndex_s = (std::max)(zIndex_s, int_max(0)); zIndex_s = (std::min)(zIndex_s, Size[2] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);
	zIndex_e = (std::max)(zIndex_e, int_max(0)); zIndex_e = (std::min)(zIndex_e, Size[2] - 1);

	auto Region_Lx = xIndex_e - xIndex_s + 1;
	auto Region_Ly = yIndex_e - yIndex_s + 1;
	auto Region_Lz = zIndex_e - zIndex_s + 1;

	DenseVector<int_max>  List;
    List.Resize(Region_Lx*Region_Ly*Region_Lz);

    auto PixelNumberPerZSlice = m_ImageData->PixelNumberPerZSlice;

	int_max Counter = 0;

	for (int_max k = zIndex_s; k <= zIndex_e; ++k)
	{
        auto temp_k = k*PixelNumberPerZSlice;

		for (int_max j = yIndex_s; j <= yIndex_e; ++j)
		{
            auto temp_j = j*Size[0];

			for (int_max i = xIndex_s; i <= xIndex_e; ++i)
			{
                List[Counter] = temp_k + temp_j + i;
			    Counter += 1;
			}
		}
	}
    
	return List;
}


template<typename PixelType>
DenseVector<int_max>
DenseImage3D<PixelType>::GetLinearIndexListInRegion(const BoxRegionOf3DPhysicalPositionInImage3D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	auto Index3D_min = this->Transform3DPhysicalPositionTo3DIndex(RegionInfo.x_min, RegionInfo.y_min, RegionInfo.z_min);
	auto xIndex_s = int_max(Index3D_min[0]);
	auto yIndex_s = int_max(Index3D_min[1]);
	auto zIndex_s = int_max(Index3D_min[2]);
	auto Index3D_max = this->Transform3DPhysicalPositionTo3DIndex(RegionInfo.x_max, RegionInfo.y_max, RegionInfo.z_max);
	auto xIndex_e = int_max(Index3D_max[0]);
	auto yIndex_e = int_max(Index3D_max[1]);
	auto zIndex_e = int_max(Index3D_max[2]);

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s || zIndex_e < zIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage3D::GetLinearIndexListInRegion(...)")
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	auto Size = this->GetSize();

	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	zIndex_s = (std::max)(zIndex_s, int_max(0)); zIndex_s = (std::min)(zIndex_s, Size[2] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);
	zIndex_e = (std::max)(zIndex_e, int_max(0)); zIndex_e = (std::min)(zIndex_e, Size[2] - 1);

	BoxRegionOf3DIndexInImage3D RegionOf3DIndex;
	RegionOf3DIndex.x_min = xIndex_s;
	RegionOf3DIndex.y_min = yIndex_s;
	RegionOf3DIndex.z_min = zIndex_s;
	RegionOf3DIndex.x_max = xIndex_e;
	RegionOf3DIndex.y_max = yIndex_e;
	RegionOf3DIndex.z_max = zIndex_e;
	return this->GetLinearIndexListInRegion(RegionOf3DIndex);
}


template<typename PixelType>
DenseMatrix<int_max> DenseImage3D<PixelType>::Get3DIndexListInRegion(const BoxRegionOf3DIndexInImage3D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseMatrix<int_max> EmptyList;
		return EmptyList;
	}

	auto xIndex_s = int_max(RegionInfo.x_min);
	auto yIndex_s = int_max(RegionInfo.y_min);
	auto zIndex_s = int_max(RegionInfo.z_min);
	auto xIndex_e = int_max(RegionInfo.x_max);
	auto yIndex_e = int_max(RegionInfo.y_max);
	auto zIndex_e = int_max(RegionInfo.z_max);

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s || zIndex_e < zIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage3D::GetLinearIndexListInRegion(...)")
		DenseMatrix<int_max> EmptyList;
		return EmptyList;
	}

	auto Size = this->GetSize();
	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	zIndex_s = (std::max)(zIndex_s, int_max(0)); zIndex_s = (std::min)(zIndex_s, Size[2] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);
	zIndex_e = (std::max)(zIndex_e, int_max(0)); zIndex_e = (std::min)(zIndex_e, Size[2] - 1);

	DenseMatrix<int_max> List;
	List.ReserveCapacity(3, (zIndex_e - zIndex_s + 1)*(yIndex_e - yIndex_s + 1)*(xIndex_e - xIndex_s + 1));
	for (int_max k = zIndex_s; k <= zIndex_e; ++k)
	{
		for (int_max j = yIndex_s; j <= yIndex_e; ++j)
		{
			for (int_max i = xIndex_s; i <= xIndex_e; ++i)
			{
				List.AppendCol({i, j, k});
			}
		}
	}
    
	return List;
}


template<typename PixelType>
DenseMatrix<int_max> DenseImage3D<PixelType>::Get3DIndexListInRegion(const BoxRegionOf3DPhysicalPositionInImage3D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseMatrix<int_max> EmptyList;
		return EmptyList;
	}

	auto Index3D_min = this->Transform3DPhysicalPositionTo3DIndex(RegionInfo.x_min, RegionInfo.y_min, RegionInfo.z_min);
	auto xIndex_s = int_max(Index3D_min[0]);
	auto yIndex_s = int_max(Index3D_min[1]);
	auto zIndex_s = int_max(Index3D_min[2]);
	auto Index3D_max = this->Transform3DPhysicalPositionTo3DIndex(RegionInfo.x_max, RegionInfo.y_max, RegionInfo.z_max);
	auto xIndex_e = int_max(Index3D_max[0]);
	auto yIndex_e = int_max(Index3D_max[1]);
	auto zIndex_e = int_max(Index3D_max[2]);

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s || zIndex_e < zIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage3D::Get3DIndexListInRegion(...)")
		DenseMatrix<int_max> EmptyList;
		return EmptyList;
	}

	auto Size = this->GetSize();

	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	zIndex_s = (std::max)(zIndex_s, int_max(0)); zIndex_s = (std::min)(zIndex_s, Size[2] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);
	zIndex_e = (std::max)(zIndex_e, int_max(0)); zIndex_e = (std::min)(zIndex_e, Size[2] - 1);

	BoxRegionOf3DIndexInImage3D RegionOf3DIndex;
	RegionOf3DIndex.x_min = xIndex_s;
	RegionOf3DIndex.y_min = yIndex_s;
	RegionOf3DIndex.z_min = zIndex_s;
	RegionOf3DIndex.x_max = xIndex_e;
	RegionOf3DIndex.y_max = yIndex_e;
	RegionOf3DIndex.z_max = zIndex_e;
	return this->Get3DIndexListInRegion(RegionOf3DIndex);
}


template<typename PixelType>
DenseImage3D<PixelType>
DenseImage3D<PixelType>::GetSubImage(const BoxRegionOf3DIndexInImage3D& RegionInfo) const
{
    DenseImage3D<PixelType> SubImage; // empty SubImage

    if (this->IsEmpty() == true)
    {
        MDK_Warning("DenseImage is empty @ DenseImage3D::GetSubImage()")
        return SubImage;
    }

	auto xIndex_s = int_max(std::round(RegionInfo.x_min));
	auto yIndex_s = int_max(std::round(RegionInfo.y_min));
	auto zIndex_s = int_max(std::round(RegionInfo.z_min));
	auto xIndex_e = int_max(std::round(RegionInfo.x_max));
	auto yIndex_e = int_max(std::round(RegionInfo.y_max));
	auto zIndex_e = int_max(std::round(RegionInfo.z_max));

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s || zIndex_e < zIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage3D::GetSubImage(...)")
		return SubImage;
	}

	auto Size = this->GetSize();

	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	zIndex_s = (std::max)(zIndex_s, int_max(0)); zIndex_s = (std::min)(zIndex_s, Size[2] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);
	zIndex_e = (std::max)(zIndex_e, int_max(0)); zIndex_e = (std::min)(zIndex_e, Size[2] - 1);

    auto Lx = xIndex_e - xIndex_s + 1;
    auto Ly = yIndex_e - yIndex_s + 1;
    auto Lz = zIndex_e - zIndex_s + 1;
	
	auto Origin_new = this->Transform3DIndexTo3DPhysicalPosition(xIndex_s, yIndex_s, zIndex_s);

	SubImage.SetSize(Lx, Ly, Lz);
	SubImage.SetSpacing(this->GetSpacing());
	SubImage.SetOrigin(Origin_new);
	SubImage.SetOrientation(this->GetOrientation());

	auto SubPtr = SubImage.GetPixelPointer();
    auto RawPtr = this->GetPixelPointer();

    auto PixelNumberPerZSlice = m_ImageData->m_PixelNumberPerZSlice;

    for (int_max k = zIndex_s; k <= zIndex_e; ++k)
    {
        auto temp_k = k*PixelNumberPerZSlice;

        for (int_max j = yIndex_s; j <= yIndex_e; ++j)
        {
            auto temp_j = j*Size[0];

            for (int_max i = xIndex_s; i <= xIndex_e; ++i)
            {
				SubPtr[0] = RawPtr[temp_k + temp_j + i];
				++SubPtr;
            }
        }
    }

	return SubImage;
}


template<typename PixelType>
DenseImage3D<PixelType>
DenseImage3D<PixelType>::GetSubImage(const BoxRegionOf3DPhysicalPositionInImage3D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseImage3D<PixelType> EmptyImage;
		return EmptyImage;
	}

	auto Index3D_min = this->Transform3DPhysicalPositionTo3DIndex(RegionInfo.x_min, RegionInfo.y_min, RegionInfo.z_min);
	auto xIndex_s = int_max(Index3D_min[0]);
	auto yIndex_s = int_max(Index3D_min[1]);
	auto zIndex_s = int_max(Index3D_min[2]);
	auto Index3D_max = this->Transform3DPhysicalPositionTo3DIndex(RegionInfo.x_max, RegionInfo.y_max, RegionInfo.z_max);
	auto xIndex_e = int_max(Index3D_max[0]);
	auto yIndex_e = int_max(Index3D_max[1]);
	auto zIndex_e = int_max(Index3D_max[2]);

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s || zIndex_e < zIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage3D::GetSubImage(...)")
		DenseImage3D<PixelType> EmptyImage;
		return EmptyImage;
	}

	auto Size = this->GetSize();
	
	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	zIndex_s = (std::max)(zIndex_s, int_max(0)); zIndex_s = (std::min)(zIndex_s, Size[2] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);
	zIndex_e = (std::max)(zIndex_e, int_max(0)); zIndex_e = (std::min)(zIndex_e, Size[2] - 1);
	
	BoxRegionOf3DIndexInImage3D RegionOf3DIndex;
	RegionOf3DIndex.x_min = xIndex_s;
	RegionOf3DIndex.y_min = yIndex_s;
	RegionOf3DIndex.z_min = zIndex_s;
	RegionOf3DIndex.x_max = xIndex_e;
	RegionOf3DIndex.y_max = yIndex_e;
	RegionOf3DIndex.z_max = zIndex_e;
	return this->GetSubImage(RegionOf3DIndex);
}


template<typename PixelType>
DenseImage3D<PixelType>
DenseImage3D<PixelType>::Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
	DenseImage3D<PixelType> SubImage; // empty SubImage

    if (this->IsEmpty() == true)
    {
        MDK_Warning("Self is empty @ DenseImage3D::Pad")
        return SubImage;
    }

    if (Option != "replicate")
    {
        MDK_Error("Invalid Option @ DenseImage3D::Pad")
        return SubImage;
    }

    if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
    {
        MDK_Warning("Invalid Pad Size @ DenseImage3D::Pad")
		SubImage = (*this);
		return SubImage;
    }

    auto Size = this->GetSize();

    auto Lx = Size[0] + Pad_Lx;
    auto Ly = Size[1] + Pad_Ly;
    auto Lz = Size[2] + Pad_Lz;

	SubImage.SetSize(Lx, Ly, Lz);
	SubImage.SetSpacing(this->GetSpacing());
	SubImage.SetOrigin(this->GetOrigin());
	SubImage.SetOrientation(this->GetOrientation());

    if (Option == "zero")
    {
		SubImage.Fill(PixelType(0));

        for (int_max k = 0; k <= Lz; ++k)
        {
            for (int_max j = 0; j <= Ly; ++j)
            {
                for (int_max i = 0; i <= Lx; ++i)
                {
                    auto temp_i = i + Pad_Lx;
                    auto temp_j = j + Pad_Ly;
                    auto temp_k = k + Pad_Lz;
					SubImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
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
                    auto i = (std::min)((std::max)(temp_i - Pad_Lx, 0), Size[0] - 1);
                    auto j = (std::min)((std::max)(temp_j - Pad_Ly, 0), Size[1] - 1);
                    auto k = (std::min)((std::max)(temp_k - Pad_Lz, 0), Size[2] - 1);
					SubImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
                }
            }
        }
    }

	return SubImage;
}


template<typename PixelType>
DenseImage3D<PixelType>
DenseImage3D<PixelType>::Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
	DenseImage3D<PixelType> SubImage; // empty SubImage

    if (this->IsEmpty() == true)
    {
        MDK_Warning("DenseImage is empty @ DenseImage3D::Pad(...)")
		return SubImage;
    }

    if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
    {
        MDK_Warning("Zero Pad Size @ DenseImage3D::Pad(...)")
		SubImage = (*this);
		return SubImage;
    }

    auto Size = this->GetSize();

    auto Lx = Size[0] + Pad_Lx;
    auto Ly = Size[1] + Pad_Ly;
    auto Lz = Size[2] + Pad_Lz;

	SubImage.SetSize(Lx, Ly, Lz);
	SubImage.SetSpacing(this->GetSpacing());
	SubImage.SetOrigin(this->GetOrigin());
	SubImage.SetOrientation(this->GetOrientation());
	SubImage.Fill(Pixel);
    for (int_max k = 0; k <= Lz; ++k)
    {
        for (int_max j = 0; j <= Ly; ++j)
        {
            for (int_max i = 0; i <= Lx; ++i)
            {
                auto temp_i = i + Pad_Lx;
                auto temp_j = j + Pad_Ly;
                auto temp_k = k + Pad_Lz;
				SubImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
            }
        }
    }

	return SubImage;
}


template<typename PixelType>
DenseImage3D<PixelType>
DenseImage3D<PixelType>::UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
	DenseImage3D<PixelType> SubImage; // empty SubImage

    auto Size = this->GetSize();

    if (Pad_Lx > Size[0] || Pad_Lx  < 0 || Pad_Ly > Size[1] || Pad_Ly < 0 || Pad_Lz > Size[2] || Pad_Lz < 0)
    {
        MDK_Error("Invalid Pad Size @ DenseImage3D::UnPad(...)")
		return SubImage;
    }

    if (Pad_Lx == Size[0] || Pad_Ly == Size[1] || Pad_Lz == Size[2])
    {
        MDK_Warning("Output is empty @ DenseImage3D::UnPad(...)")
        return SubImage;
    }

    if (Pad_Lx == 0 && Pad_Ly == 0 && Pad_Lz == 0)
    {
        MDK_Warning("Input Pad Size is [0, 0, 0] @ DenseImage3D::UnPad(...)")
		SubImage = (*this);
		return SubImage;
    }

    return this->GetSubImage(Pad_Lx, Size[0] - 1 - Pad_Lx,
                             Pad_Ly, Size[1] - 1 - Pad_Ly,
                             Pad_Lz, Size[2] - 1 - Pad_Lz);
}

}//end namespace mdk

#endif