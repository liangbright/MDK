#ifndef mdk_DenseImage2D_hpp
#define mdk_DenseImage2D_hpp

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
	m_Info.Clear();
    m_PixelArray.Clear();
	m_Pixel_OutsideImage = GetZeroPixel<PixelType>();
}


template<typename PixelType>
inline 
PixelType& DenseImageData2D<PixelType>::operator[](int_max LinearIndex)
{ 
    return m_PixelArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData2D<PixelType>::operator[](int_max LinearIndex) const
{
    return m_PixelArray[LinearIndex];
}


template<typename PixelType>
inline
PixelType& DenseImageData2D<PixelType>::operator()(int_max LinearIndex)
{
    return m_PixelArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData2D<PixelType>::operator()(int_max LinearIndex) const
{
    return m_PixelArray[LinearIndex];
}
   

template<typename PixelType>
inline
PixelType& DenseImageData2D<PixelType>::operator()(int_max xIndex, int_max yIndex)
{
    auto LinearIndex = yIndex*m_Info.Size[0] + xIndex;
    return m_PixelArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImageData2D<PixelType>::operator()(int_max xIndex, int_max yIndex) const
{
	auto LinearIndex = yIndex*m_Info.Size[0] + xIndex;
    return m_PixelArray[LinearIndex];
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> DenseImageData2D<PixelType>::TransformLinearIndexTo2DIndex(int_max LinearIndex) const
{            
	auto divresult = std::div(LinearIndex m_Info.Size[0]);
	auto yIndex = divresult.quot;
	auto xIndex = divresult.rem;

	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = ScalarType(xIndex);
	Index2D[1] = ScalarType(yIndex);
	return Index2D;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImageData2D<PixelType>::TransformLinearIndexTo2DPosition(int_max LinearIndex) const
{
	auto Index2D = this->TransformLinearIndexTo2DIndex(LinearIndex);
	auto Position = this->Transform2DIndexTo2DPosition<ScalarType>(Index2D[0], Index2D[1]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> DenseImageData2D<PixelType>::TransformLinearIndexTo3DWorldPosition(int_max LinearIndex) const
{
	auto Index2D = this->TransformLinearIndexTo2DIndex(LinearIndex);
	auto Position = this->Transform2DIndexTo3DWorldPosition<ScalarType>(Index2D[0], Index2D[1]);
	return Position;
}


template<typename PixelType>
inline
int_max DenseImageData2D<PixelType>::Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const
{
	return yIndex*m_Info.Size[0] + xIndex;
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2>
DenseImageData2D<PixelType>::Transform2DIndexTo2DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const
{
	DenseVector<ScalarType_Position, 2> Position;
	Position[0] = ScalarType_Position(double(xIndex)*m_Info.Spacing[0]);
	Position[1] = ScalarType_Position(double(yIndex)*m_Info.Spacing[1]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline 
DenseVector<ScalarType_Position, 3> 
DenseImageData2D<PixelType>::Transform2DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const
{
	auto M = m_Info.TransformMatrix_2DIndexTo3DWorld.GetElementPointer();
	DenseVector<ScalarType_Position, 3> Position;
	Position[0] = ScalarType_Position(m_Info.Origin[0] + double(xIndex)*M[0] + double(yIndex)*M[3]);
	Position[1] = ScalarType_Position(m_Info.Origin[1] + double(xIndex)*M[1] + double(yIndex)*M[4]);
	Position[2] = ScalarType_Position(m_Info.Origin[2] + double(xIndex)*M[2] + double(yIndex)*M[5]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImageData2D<PixelType>::Transform2DPositionTo2DIndex(ScalarType x, ScalarType y) const
{
	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = ScalarType(double(x)/m_Info.Spacing[0]);
	Index2D[1] = ScalarType(double(y)/m_Info.Spacing[1]);
	return Index2D;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImageData2D<PixelType>::Transform2DPositionTo3DWorldPosition(ScalarType x, ScalarType y) const
{
	auto M = m_Info.Orientation.GetElementPointer();
	DenseVector<ScalarType, 3> Position;
	Position[0] = ScalarType(m_Info.Origin[0] + double(x)*M[0] + double(y)*M[3]);
	Position[1] = ScalarType(m_Info.Origin[1] + double(x)*M[1] + double(y)*M[4]);
	Position[2] = ScalarType(m_Info.Origin[2] + double(x)*M[2] + double(y)*M[5]);
	return Position;
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> DenseImageData2D<PixelType>::Transform3DWorldPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	auto temp_x = double(x) - m_Info.Origin[0];
	auto temp_y = double(y) - m_Info.Origin[1];
	auto temp_z = double(z) - m_Info.Origin[2];

	auto M = m_Info.TransformMatrix_3DWorldTo2DIndex.GetElementPointer();

	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = ScalarType(double(temp_x)*M[0] + double(temp_y)*M[2] + double(temp_z)*M[4]);
	Index2D[1] = ScalarType(double(temp_x)*M[1] + double(temp_y)*M[3] + double(temp_z)*M[5]);
	return Index2D;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImageData2D<PixelType>::Transform3DWorldPositionTo2DPosition(ScalarType x, ScalarType y, ScalarType z) const
{
	auto temp_x = double(x) - m_Info.Origin[0];
	auto temp_y = double(y) - m_Info.Origin[1];
	auto temp_z = double(z) - m_Info.Origin[2];
	auto R = m_Info.Orientation.GetElementPointer();
	DenseVector<ScalarType, 2> Position;
	Position[0] = ScalarType(double(temp_x)*R[0] + double(temp_y)*R[1] + double(temp_z)*R[2]);
	Position[1] = ScalarType(double(temp_x)*R[3] + double(temp_y)*R[4] + double(temp_z)*R[5]);
	//auto z = ScalarType(double(temp_x)*R[6] + double(temp_y)*R[7] + double(temp_z)*R[8]);// should be 0
	return Position;
}

//========================================================== DenseImage ========================================================================//

template<typename PixelType>
DenseImage2D<PixelType>::DenseImage2D()
{
	m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
}


template<typename PixelType>
DenseImage2D<PixelType>::DenseImage2D(const DenseImage2D<PixelType>& InputImage)
{	
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
	this->Copy(std::move(InputImage));
}


template<typename PixelType>
template<typename PixelType_Input>
void DenseImage2D<PixelType>::Copy(const DenseImage2D<PixelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        MDK_Warning("try to Copy self @ DenseImage2D::Copy(InputImage)")
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
		m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
	}

    this->SetSize(InputImage.GetSize());
    this->SetSpacing(InputImage.GetSpacing());
    this->SetOrigin(InputImage.GetOrigin());
    this->SetOrientation(InputImage.GetOrientation());
	this->CopyPixelData(InputImage.GetPixelPointer(), InputImage.GetPixelCount());
}


template<typename PixelType>
template<typename PixelType_Input>
void DenseImage2D<PixelType>::CopyPixelData(const PixelType_Input* InputPixelPointer, int_max InputPixelCount)
{
    if (InputPixelPointer == nullptr || InputPixelCount <= 0)
	{
        MDK_Warning("Input is nullptr, Clear self @ DenseImage2D::CopyPixelData(...)")
		this->Clear();
		return;
	}

    auto SelfPixelCount = this->GetPixelCount();

    if (SelfPixelCount != InputPixelCount)// must call SetSize() before this function
    {
        MDK_Error("Size does not match @ DenseImage2D::CopyPixelData(...)")
        return;
    }

    auto PixelPtr = this->GetPixelPointer();

    if (std::size_t(InputPixelPointer) == std::size_t(PixelPtr))
    {
        MDK_Warning("A DenseImage try to Copy itself @ DenseImage2D::CopyPixelData(...)")
        return;
    }
  
    for (int_max i = 0; i < SelfPixelCount; ++i)
	{
        PixelPtr[i] = PixelType(InputPixelPointer[i]);
	}
}


template<typename PixelType>
void DenseImage2D<PixelType>::Copy(DenseImage2D<PixelType>&& InputImage)
{
	if (InputImage.IsPureEmpty() == true)
	{
		return;
	}

	if (!m_ImageData)
	{
		m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
	}

	m_ImageData->m_Info  = std::move(InputImage.m_ImageData->m_Info);
	m_ImageData->m_PixelArray = std::move(InputImage.m_ImageData->m_PixelArray);
	m_ImageData->m_Pixel_OutsideImage = InputImage.m_ImageData->m_Pixel_OutsideImage;

	InputImage.Clear();
}


template<typename PixelType>
void DenseImage2D<PixelType>::Fill(const PixelType& Pixel)
{
	auto SelfPixelCount = this->GetPixelCount();
	if (SelfPixelCount == 0)
    {
		MDK_Error("Self is empty @ DenseImage2D::File(...)")
        return;
    }

    auto BeginPtr = this->GetPixelPointer();

	for (auto Ptr = BeginPtr; Ptr < BeginPtr + SelfPixelCount; ++Ptr)
    {
        Ptr[0] = Pixel;
    }
}


template<typename PixelType>
void DenseImage2D<PixelType>::Share(DenseImage2D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
}


template<typename PixelType>
void DenseImage2D<PixelType>::ForceShare(const DenseImage2D<PixelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary
}


template<typename PixelType>
void DenseImage2D<PixelType>::Share(PixelType* InputImage, const ImageInfo2D& InputImageInfo)
{
	if (InputImage == nullptr)
	{
		MDK_Error("Input is nullptr @ DenseImage2D::Share(DenseImage*, ImageInfo2D)")
		return;
	}

	this->SetOrigin(InputImageInfo.Origin);
	this->SetSpacing(InputImageInfo.Spacing);
	this->SetOrientation(InputImageInfo.Orientation);
	//this->SetSize(XXX); do not allocate memory for internal array
	m_ImageData->m_Info.Size[0] = InputImageInfo.Size[0];
	m_ImageData->m_Info.Size[1] = InputImageInfo.Size[1];
	auto InputPixelCount = InputImageInfo.Size[0] * InputImageInfo.Size[1];
	m_ImageData->m_PixelArray.Share(InputImage, InputPixelCount, true);
}


template<typename PixelType>
void DenseImage2D<PixelType>::ForceShare(const PixelType* InputImage, const ImageInfo2D& InputImageInfo)
{
	this->Share(const_cast<PixelType*>(InputImage), InputImageInfo);
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
		return (m_ImageData->m_PixelArray.GetLength() == 0);
	}
	else
	{
		return true;
	}
}


template<typename PixelType>
inline
bool DenseImage2D<PixelType>::IsPureEmpty() const
{
	return (!m_ImageData);
}


template<typename PixelType>
inline
bool DenseImage2D<PixelType>::IsPixelDataInInternalArray() const
{
	if (this->IsPureEmpty() == true)
	{
		return false;
	}

	return m_ImageData->m_PixelArray.IsDataInInternalArray();
}


template<typename PixelType>
inline
PixelType* DenseImage2D<PixelType>::GetPixelPointer()
{
	if (m_ImageData)
	{
		return m_ImageData->m_PixelArray.GetPointer();
	}
	else
	{
		return nullptr;
	}
}


template<typename PixelType>
inline
const PixelType* DenseImage2D<PixelType>::GetPixelPointer() const
{
	if (m_ImageData)
	{
		return m_ImageData->m_PixelArray.GetPointer();
	}
	else
	{
		return nullptr;
	}
}


template<typename PixelType>
inline
PixelType* DenseImage2D<PixelType>::begin()
{
	return this->GetPixelPointer();
}


template<typename PixelType>
inline
const PixelType* DenseImage2D<PixelType>::begin() const
{
	return this->GetPixelPointer();
}


template<typename PixelType>
inline
PixelType* DenseImage2D<PixelType>::end()
{
	auto BeginPtr = return this->GetPixelPointer();
	if (BeginPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto EndPtr = BeginPtr + this->GetPixelCount();
		return EndPtr;
	}
}


template<typename PixelType>
inline
const PixelType* DenseImage2D<PixelType>::end() const
{
	auto BeginPtr = return this->GetPixelPointer();
	if (BeginPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto EndPtr = BeginPtr + this->GetPixelCount();
		return EndPtr;
	}
}


template<typename PixelType>
inline 
ImageInfo2D DenseImage2D<PixelType>::GetInfo() const
{
	ImageInfo2D Info;
	if (this->IsPureEmpty() == false)
	{
		Info = m_ImageData->m_Info;
	}
	return Info;
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetInfo(const ImageInfo2D& Info, bool Flag_AllocateMemory)
{
	if (Flag_AllocateMemory == true)
	{
		this->SetSize(Info.Size); // allocate memory
	}
	if (this->IsPureEmpty() == true)
	{
		this->SetSize(0, 0);
	}
	m_ImageData->m_Info = Info;
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::AllocateMemory()
{
	this->SetSize(this->GetSize());
}


template<typename PixelType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::GetSize() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_Info.Size;
	}
	else
	{
		DenseVector<int_max, 2> EmptySize;
		EmptySize[0] = 0;
		EmptySize[1] = 0;
		return EmptySize;
	}
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::GetSize(int_max& Lx, int_max& Ly) const
{
	if (this->IsPureEmpty() == false)
	{
		Lx = m_ImageData->m_Info.Size[0];
		Ly = m_ImageData->m_Info.Size[1];
	}
	else
	{
		Lx = 0;
		Ly = 0;
	}
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetSize(const DenseVector<int_max, 2>& Size)
{
    this->SetSize(Size[0], Size[1]);
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetSize(int_max Lx, int_max Ly)
{
    if (Lx < 0 || Ly < 0)
    {
        MDK_Error("Ivalid input @ DenseImage2D::SetSize(...)")
        return;
    }

	if (!m_ImageData)
	{
		m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
	}

	if (Lx == m_ImageData->m_Info.Size[0] && Ly == m_ImageData->m_Info.Size[1])
	{
		return;
	}

    if (Lx == 0 || Ly == 0)
    {
        m_ImageData->m_PixelArray.Clear();
        m_ImageData->m_Info.Size[0] = 0;
        m_ImageData->m_Info.Size[1] = 0;
        return;
    }

try
{
    m_ImageData->m_PixelArray.Resize(Lx*Ly); 
    m_ImageData->m_Info.Size[0] = Lx;
    m_ImageData->m_Info.Size[1] = Ly;
}
catch (...)
{
    MDK_Error("Out Of Memory @ DenseImage2D::SetSize(...)")
    //this->Clear();
}
}


template<typename PixelType>
inline
DenseVector<double, 2> DenseImage2D<PixelType>::GetSpacing() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_Info.Spacing;
	}
	else
	{
		DenseVector<double, 2> EmptySpacing = { 1.0, 1.0};
		return EmptySpacing;
	}
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::GetSpacing(double& Spacing_x, double& Spacing_y) const
{
	if (this->IsPureEmpty() == false)
	{
		Spacing_x = m_ImageData->m_Info.Spacing[0];
		Spacing_y = m_ImageData->m_Info.Spacing[1];
	}
	else
	{
		Spacing_x = 1.0;
		Spacing_y = 1.0;
	}
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
	auto Zero = std::numeric_limits<double>::epsilon();
	if (Spacing_x <= Zero || Spacing_y <= Zero)
    {
        MDK_Error("Invalid input (<= eps) @ DenseImage2D::SetSpacing(...)")
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
	}
    m_ImageData->m_Info.Spacing[0] = Spacing_x;
    m_ImageData->m_Info.Spacing[1] = Spacing_y;

	m_ImageData->m_Info.UpdateTransformMatrix();
}


template<typename PixelType>
inline
DenseVector<double, 3> DenseImage2D<PixelType>::GetOrigin() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_Info.Origin;
	}
	else
	{
		DenseVector<double, 3> EmptyOrigin = { 0.0, 0.0, 0.0 };
		return EmptyOrigin;
	}
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const
{
	if (this->IsPureEmpty() == false)
	{
		Origin_x = m_ImageData->m_Info.Origin[0];
		Origin_y = m_ImageData->m_Info.Origin[1];
		Origin_z = m_ImageData->m_Info.Origin[2];
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
void DenseImage2D<PixelType>::SetOrigin(const DenseVector<double, 3>& Origin)
{
	this->SetOrigin(Origin[0], Origin[1], Origin[2]);
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetOrigin(double Origin_x, double Origin_y, double Origin_z)
{
	if (this->IsPureEmpty() == true)
	{
		m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
	}
    m_ImageData->m_Info.Origin[0] = Origin_x;
    m_ImageData->m_Info.Origin[1] = Origin_y;
	m_ImageData->m_Info.Origin[2] = Origin_z;
}


template<typename PixelType>
inline 
DenseMatrix<double> DenseImage2D<PixelType>::GetOrientation() const
{	
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_Info.Orientation;
	}
	else
	{
		DenseMatrix<double> Orientation(3, 3);
		Orientation.Fill(0.0);
		Orientation.FillDiagonal(1.0);
		return Orientation;
	}
}


template<typename PixelType>
inline 
void DenseImage2D<PixelType>::SetOrientation(const DenseMatrix<double>& Orientation)
{
	if (Orientation.GetColCount() != 3 || Orientation.GetRowCount() != 3)
	{
		MDK_Error("Invalid input size @ DenseImage2D::SetOrientation(...)")
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		m_ImageData = std::make_shared<DenseImageData2D<PixelType>>();
	}

	m_ImageData->m_Info.Orientation = Orientation;

	m_ImageData->m_Info.UpdateTransformMatrix();
}


template<typename PixelType>
inline 
DenseMatrix<double> DenseImage2D<PixelType>::GetTransformMatrix_2DIndexTo3DWorld() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_Info.TransformMatrix_2DIndexTo3DWorld;
	}
	else
	{
		DenseMatrix<double> TransformMatrix = { { 1.0, 0.0 },
		                                        { 0.0, 1.0 },
		                                        { 0.0, 0.0 } };
		return TransformMatrix;
	}
}


template<typename PixelType>
inline DenseMatrix<double> DenseImage2D<PixelType>::GetTransformMatrix_3DWorldTo2DIndex() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_Info.TransformMatrix_3DWorldTo2DIndex;
	}
	else
	{
		DenseMatrix<double> TransformMatrix = { { 1.0, 0.0, 0.0 },
		                                        { 0.0, 1.0, 0.0 } };
		return TransformMatrix;
	}
}


template<typename PixelType>
inline
DenseVector<double, 2> DenseImage2D<PixelType>::GetPhysicalSize() const
{
	DenseVector<double, 2> PhysicalSize;
	if (this->IsPureEmpty() == false)
	{
		PhysicalSize[0] = double(m_ImageData->m_Info.Size[0]) * m_ImageData->m_Info.Spacing[0];
		PhysicalSize[1] = double(m_ImageData->m_Info.Size[1]) * m_ImageData->m_Info.Spacing[1];
	}
	else
	{
		PhysicalSize[0] = 0;
		PhysicalSize[1] = 0;
	}
	return PhysicalSize;
}


template<typename PixelType>
inline 
void DenseImage2D<PixelType>::GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y) const
{
	if (this->IsPureEmpty() == false)
	{
		PhysicalSize_x = double(m_ImageData->m_Info.Size[0]) * m_ImageData->m_Info.Spacing[0];
		PhysicalSize_y = double(m_ImageData->m_Info.Size[1]) * m_ImageData->m_Info.Spacing[1];
	}
	else
	{
		PhysicalSize_x = 0;
		PhysicalSize_y = 0;
	}
}


template<typename PixelType>
inline
int_max DenseImage2D<PixelType>::GetPixelCount() const
{
	if (this->IsPureEmpty() == false)
	{
		return m_ImageData->m_Info.Size[0] * m_ImageData->m_Info.Size[1];
	}
	else
	{
		return 0;
	}
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
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::TransformLinearIndexTo2DPosition(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo2DPosition<ScalarType>(LinearIndex);
}


template<typename PixelType>
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> DenseImage2D<PixelType>::TransformLinearIndexTo3DWorldPosition(int_max LinearIndex) const
{
	return m_ImageData->TransformLinearIndexTo3DWorldPosition<ScalarType>(LinearIndex);
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
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2>
DenseImage2D<PixelType>::Transform2DIndexTo2DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const
{
	return m_ImageData->Transform2DIndexTo2DPosition<ScalarType_Position>(xIndex, yIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2> DenseImage2D<PixelType>::Transform2DIndexTo2DPosition(const DenseVector<ScalarType_Index, 2>& Index2D) const
{
	return m_ImageData->Transform2DIndexTo2DPosition<ScalarType_Position>(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3> 
DenseImage2D<PixelType>::Transform2DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const
{
	return m_ImageData->Transform2DIndexTo3DWorldPosition<ScalarType_Position>(xIndex, yIndex);
}


template<typename PixelType>
template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3> DenseImage2D<PixelType>::Transform2DIndexTo3DWorldPosition(const DenseVector<ScalarType_Index, 2>& Index2D) const
{
	return m_ImageData->Transform2DIndexTo3DWorldPosition<ScalarType_Position>(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::Transform2DPositionTo2DIndex(ScalarType x, ScalarType y) const
{
	return m_ImageData->Transform2DPositionTo2DIndex(x, y);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::Transform2DPositionTo2DIndex(const DenseVector<ScalarType, 2>& Position) const
{
	return this->Transform2DPositionTo2DIndex(Position[0], Position[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::Transform2DPositionToNearest2DDiscreteIndex(ScalarType x, ScalarType y) const
{
	auto Index2D = m_ImageData->Transform2DPositionTo2DIndex(x, y);
	auto x_Index = int_max(std::round(Index2D[0]));
	auto y_Index = int_max(std::round(Index2D[1]));
	DenseVector<int_max, 2> Index2D_Nearest;
	Index2D_Nearest[0] = x_Index;
	Index2D_Nearest[1] = y_Index;
	return Index2D_Nearest;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::Transform2DPositionToNearest2DDiscreteIndex(const DenseVector<ScalarType, 2>& Position) const
{
	return this->Transform2DPositionToNearest2DDiscreteIndex(Position[0], Position[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::Transform2DPositionToNearest2DDiscreteIndexInsideImage(ScalarType x, ScalarType y) const
{
	auto Index2D = m_ImageData->Transform2DPositionTo2DIndex(x, y);
	auto Size = this->GetSize();
	auto x_Index = int_max(std::round(Index2D[0]));
	auto y_Index = int_max(std::round(Index2D[1]));
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

	DenseVector<int_max, 2> Index2D_Inside;
	Index2D_Inside[0] = x_Index;
	Index2D_Inside[1] = y_Index;
	return Index2D_Inside;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::Transform2DPositionToNearest2DDiscreteIndexInsideImage(const DenseVector<ScalarType, 2>& Position) const
{
	return this->Transform2DPositionToNearest2DDiscreteIndexInsideImage(Position[0], Position[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImage2D<PixelType>::Transform2DPositionTo3DWorldPosition(ScalarType x, ScalarType y) const
{
	return m_ImageData->Transform2DPositionTo3DWorldPosition(x, y);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> DenseImage2D<PixelType>::Transform2DPositionTo3DWorldPosition(const DenseVector<ScalarType, 2>& Position) const
{
	return this->Transform2DPositionTo3DWorldPosition(Position[0], Position[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::Transform3DWorldPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	return m_ImageData->Transform3DWorldPositionTo2DIndex(x, y, z);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::Transform3DWorldPositionTo2DIndex(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DWorldPositionTo2DIndex(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::Transform3DWorldPositionToNearest2DDiscreteIndex(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index2D = m_ImageData->Transform3DWorldPositionTo2DIndex(x, y, z);
	auto x_Index = int_max(std::round(Index2D[0]));
	auto y_Index = int_max(std::round(Index2D[1]));
	DenseVector<int_max, 2> Index2D_Nearest;
	Index2D_Nearest[0] = x_Index;
	Index2D_Nearest[1] = y_Index;
	return Index2D_Nearest;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::Transform3DWorldPositionToNearest2DDiscreteIndex(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DWorldPositionToNearest2DDiscreteIndex(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::Transform3DWorldPositionToNearest2DDiscreteIndexInsideImage(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index2D = m_ImageData->Transform3DWorldPositionTo2DIndex(x, y, z);
	auto Size = this->GetSize();
	auto x_Index = int_max(std::round(Index2D[0]));
	auto y_Index = int_max(std::round(Index2D[1]));
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

	DenseVector<int_max, 2> Index2D_Inside;
	Index2D_Inside[0] = x_Index;
	Index2D_Inside[1] = y_Index;
	return Index2D_Inside;
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<int_max, 2> DenseImage2D<PixelType>::Transform3DWorldPositionToNearest2DDiscreteIndexInsideImage(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DWorldPositionToNearest2DDiscreteIndexInsideImage(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::Transform3DWorldPositionTo2DPosition(ScalarType x, ScalarType y, ScalarType z) const
{
	return m_ImageData->Transform3DWorldPositionTo2DPosition(x, y, z);
}


template<typename PixelType>
template<typename ScalarType>
inline
DenseVector<ScalarType, 2> DenseImage2D<PixelType>::Transform3DWorldPositionTo2DPosition(const DenseVector<ScalarType, 3>& Position) const
{
	return this->Transform3DWorldPositionTo2DPosition(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename ScalarType>
inline
bool DenseImage2D<PixelType>::CheckIf2DIndexIsInsideImage(ScalarType xIndex, ScalarType yIndex) const
{
	auto Size = this->GetSize();
	auto Lx = double(Size[0]);
	auto Ly = double(Size[1]);
	
	auto x = double(xIndex);
	auto y = double(yIndex);
	
	if (x < 0.0 || x >= Lx || y <= 0.0 || y >= Ly)
	{
		return false;
	}
	return true;
}


template<typename PixelType>
template<typename ScalarType>
inline
bool DenseImage2D<PixelType>::CheckIf2DIndexIsInsideImage(const DenseVector<ScalarType, 2>& Index2D) const
{
	return this->CheckIf2DIndexIsInsideImage(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
bool DenseImage2D<PixelType>::CheckIf2DPositionIsInsideImage(ScalarType x, ScalarType y) const
{
	auto Index2D = this->Transform2DPositionTo2DIndex(x, y);
	return this->CheckIf2DIndexIsInsideImage(Index2D);
}


template<typename PixelType>
template<typename ScalarType>
inline
bool DenseImage2D<PixelType>::CheckIf2DPositionIsInsideImage(const DenseVector<ScalarType, 2>& Position) const
{
	return this->CheckIf2DPositionIsInsideImage(Position[0], Position[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline 
bool DenseImage2D<PixelType>::CheckIf3DWorldPositionIsInsideImage(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index2D = this->Transform3DWorldPositionTo2DIndex(x, y, z);
	return this->CheckIf2DIndexIsInsideImage(Index2D);
}


template<typename PixelType>
template<typename ScalarType>
inline 
bool DenseImage2D<PixelType>::CheckIf3DWorldPositionIsInsideImage(const DenseVector<ScalarType, 3>& Position) const
{
	return this->CheckIf3DWorldPositionIsInsideImage(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
inline 
void DenseImage2D<PixelType>::SetPixelAt2DIndex(int_max xIndex, int_max yIndex, PixelType Pixel)
{
	(*this)(xIndex, yIndex) = std::move(Pixel);
}


template<typename PixelType>
inline 
void DenseImage2D<PixelType>::SetPixelAt2DIndex(DenseVector<int_max, 2> Index2D, PixelType Pixel)
{
	(*this)(Index2D[0], Index2D[1]) = std::move(Pixel);
}


template<typename PixelType>
inline
void DenseImage2D<PixelType>::SetPixelAtLinearIndex(int_max LinearIndex, PixelType Pixel)
{
	(*this)(LinearIndex) = std::move(Pixel);
}


template<typename PixelType>
inline
const PixelType& DenseImage2D<PixelType>::GetPixelAt2DIndex(int_max xIndex, int_max yIndex) const
{
	return (*this)(xIndex, yIndex);
}


template<typename PixelType>
inline
const PixelType& DenseImage2D<PixelType>::GetPixelAt2DIndex(DenseVector<int_max, 2> Index2D) const
{
	return (*this)(Index2D[0], Index2D[1]);
}


template<typename PixelType>
inline 
const PixelType& DenseImage2D<PixelType>::GetPixelAtLinearIndex(int_max LinearIndex) const
{
	return (*this)(LinearIndex);
}


template<typename PixelType>
inline
PixelType& DenseImage2D<PixelType>::operator[](int_max LinearIndex)
{
#if defined MDK_DEBUG_DenseImage2D

    auto PixelCount = this->GetPixelCount();
    if (LinearIndex >= PixelCount || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ DenseImage2D::operator(LinearIndex)")
        return m_ImageData->m_Pixel_OutsideImage;
    }

#endif //MDK_DEBUG_DenseImage2D

	return (*m_ImageData)[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& DenseImage2D<PixelType>::operator[](int_max LinearIndex) const
{
#if defined MDK_DEBUG_DenseImage2D

    auto PixelCount = this->GetPixelCount();
    if (LinearIndex >= PixelCount || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ DenseImage2D::operator(LinearIndex)")
        return m_ImageData->m_Pixel_OutsideImage;
    }

#endif //MDK_DEBUG_DenseImage2D

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
#if defined MDK_DEBUG_DenseImage2D

    auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0)
	{
		MDK_Error("Invalid input @ DenseImage2D::operator(xIndex, yIndex)")
		return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_DenseImage2D
	
    return (*m_ImageData)(xIndex, yIndex);
}


template<typename PixelType>
inline
const PixelType& DenseImage2D<PixelType>::operator()(int_max xIndex, int_max yIndex) const
{
#if defined MDK_DEBUG_DenseImage2D

    auto Size = this->GetSize();
	if (xIndex >= Size[0] || xIndex < 0 || yIndex >= Size[1] || yIndex < 0)
	{
        MDK_Error("Invalid input @ DenseImage2D::operator(xIndex, yIndex) const")
        return m_ImageData->m_Pixel_OutsideImage;
	}

#endif //MDK_DEBUG_DenseImage2D

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

	auto x = int_max(std::round(xIndex));
	auto y = int_max(std::round(yIndex));

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

	return (*m_ImageData)(x, y);
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
const PixelType& DenseImage2D<PixelType>::GetPixelNearestTo2DPosition(ScalarType x, ScalarType y) const
{
	auto Index2D = this->Transform2DPositionTo2DIndex(x, y);
	return this->GetPixelNearestTo2DIndex(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& DenseImage2D<PixelType>::GetPixelNearestTo2DPosition(const DenseVector<ScalarType, 2>& Position) const
{
	return this->GetPixelNearestTo2DPosition(Position[0], Position[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& DenseImage2D<PixelType>::GetPixelNearestTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z) const
{
	auto Index2D = this->Transform3DWorldPositionTo2DIndex(x, y, z);
	return this->GetPixelNearestTo2DIndex(Index2D[0], Index2D[1]);
}


template<typename PixelType>
template<typename ScalarType>
inline
const PixelType& DenseImage2D<PixelType>::GetPixelNearestTo3DWorldPosition(const DenseVector<ScalarType, 3>& Position) const
{
	return this->GetPixelNearestTo3DWorldPosition(Position[0], Position[1], Position[2]);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt2DIndex(ScalarType xIndex, ScalarType yIndex, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return InterpolateImageAt2DIndex<OutputPixelType>(*this, xIndex, yIndex, Option, EnableBoundCheck);
}


template<typename PixelType>
PixelType DenseImage2D<PixelType>::GetPixelAt2DIndex(int_max xIndex, int_max yIndex, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return InterpolateImageAt2DIndex_Nearest<PixelType>(*this, xIndex, yIndex, Option, EnableBoundCheck);
}


template<typename PixelType>
PixelType DenseImage2D<PixelType>::GetPixelAt2DIndex(int xIndex, int yIndex, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return this->GetPixelAt2DIndex(int_max(xIndex), int_max(yIndex), Option, EnableBoundCheck);
}


template<typename PixelType>
PixelType DenseImage2D<PixelType>::GetPixelAt2DIndex(long xIndex, long yIndex, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return this->GetPixelAt2DIndex(int_max(xIndex), int_max(yIndex), Option, EnableBoundCheck);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<ScalarType, 2>& Index2D, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return this->GetPixelAt2DIndex<OutputPixelType>(Index2D[0], Index2D[1], Option, EnableBoundCheck);
}


template<typename PixelType>
PixelType DenseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<int_max, 2>& Index2D, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return this->GetPixelAt2DIndex(Index2D[0], Index2D[1], Option, EnableBoundCheck);
}


template<typename PixelType>
PixelType DenseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<int, 2>& Index2D, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return this->GetPixelAt2DIndex(int_max(Index2D[0]), int_max(Index2D[1]), Option, EnableBoundCheck);
}


template<typename PixelType>
PixelType DenseImage2D<PixelType>::
GetPixelAt2DIndex(const DenseVector<long, 2>& Index2D, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return this->GetPixelAt2DIndex(int_max(Index2D[0]), int_max(Index2D[1]), Option, EnableBoundCheck);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt2DPosition(ScalarType x, ScalarType y, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return InterpolateImageAt2DPosition<OutputPixelType>(*this, x, y, Option, EnableBoundCheck);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt2DPosition(const DenseVector<ScalarType, 2>& Position, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return InterpolateImageAt2DPosition<OutputPixelType>(*this, Position[0], Position[1], Option, EnableBoundCheck);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt3DWorldPosition(ScalarType x, ScalarType y, ScalarType z, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return InterpolateImageAt3DWorldPosition<OutputPixelType>(*this, x, y, z, Option, EnableBoundCheck);
}


template<typename PixelType>
template<typename OutputPixelType, typename ScalarType>
OutputPixelType DenseImage2D<PixelType>::
GetPixelAt3DWorldPosition(const DenseVector<ScalarType, 3>& Position, const InterpolationOptionType& Option, bool EnableBoundCheck) const
{
	return InterpolateImageAt3DWorldPosition<OutputPixelType>(*this, Position[0], Position[1], Position[2], Option, EnableBoundCheck);
}


template<typename PixelType>
DenseVector<int_max>
DenseImage2D<PixelType>::GetLinearIndexListInRegion(const BoxRegionOf2DIndexInImage2D& RegionInfo) const
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

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage2D::GetLinearIndexListInRegion(...)")
			DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	auto Size = this->GetSize();
	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);

	auto Region_Lx = xIndex_e - xIndex_s + 1;
	auto Region_Ly = yIndex_e - yIndex_s + 1;

	DenseVector<int_max>  List;
	List.Resize(Region_Lx*Region_Ly);

	int_max Counter = 0;

	for (int_max j = yIndex_s; j <= yIndex_e; ++j)
	{
		auto temp_j = j*Size[0];

		for (int_max i = xIndex_s; i <= xIndex_e; ++i)
		{
			List[Counter] = temp_j + i;
			Counter += 1;
		}
	}

	return List;
}


template<typename PixelType>
DenseVector<int_max>
DenseImage2D<PixelType>::GetLinearIndexListInRegion(const BoxRegionOf3DWorldPositionInImage2D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	auto Index2D_min = this->Transform3DWorldPositionTo2DIndex(RegionInfo.x_min, RegionInfo.y_min, RegionInfo.z_min);
	auto xIndex_s = int_max(Index2D_min[0]);
	auto yIndex_s = int_max(Index2D_min[1]);

	auto Index2D_max = this->Transform3DWorldPositionTo2DIndex(RegionInfo.x_max, RegionInfo.y_max, RegionInfo.z_max);
	auto xIndex_e = int_max(Index2D_max[0]);
	auto yIndex_e = int_max(Index2D_max[1]);

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage2D::GetLinearIndexListInRegion(...)")
		DenseVector<int_max> EmptyList;
		return EmptyList;
	}

	auto Size = this->GetSize();

	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);

	BoxRegionOf2DIndexInImage2D RegionOf2DIndex;
	RegionOf2DIndex.x_min = xIndex_s;
	RegionOf2DIndex.y_min = yIndex_s;
	RegionOf2DIndex.x_max = xIndex_e;
	RegionOf2DIndex.y_max = yIndex_e;
	return this->GetLinearIndexListInRegion(RegionOf2DIndex);
}


template<typename PixelType>
DenseMatrix<int_max> DenseImage2D<PixelType>::Get2DIndexListInRegion(const BoxRegionOf2DIndexInImage2D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseMatrix<int_max> EmptyList;
		return EmptyList;
	}

	auto xIndex_s = int_max(RegionInfo.x_min);
	auto yIndex_s = int_max(RegionInfo.y_min);
	auto xIndex_e = int_max(RegionInfo.x_max);
	auto yIndex_e = int_max(RegionInfo.y_max);

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage2D::GetLinearIndexListInRegion(...)")
		DenseMatrix<int_max> EmptyList;
		return EmptyList;
	}

	auto Size = this->GetSize();
	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);

	DenseMatrix<int_max> List;
	List.ReserveCapacity(3, (yIndex_e - yIndex_s + 1)*(xIndex_e - xIndex_s + 1));
	for (int_max j = yIndex_s; j <= yIndex_e; ++j)
	{
		for (int_max i = xIndex_s; i <= xIndex_e; ++i)
		{
			List.AppendCol({ i, j});
		}
	}
    
	return List;
}


template<typename PixelType>
DenseMatrix<int_max> DenseImage2D<PixelType>::Get2DIndexListInRegion(const BoxRegionOf2DPositionInImage2D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseMatrix<int_max> EmptyList;
		return EmptyList;
	}

	auto Index2D_min = this->Transform2DPositionTo2DIndex(RegionInfo.x_min, RegionInfo.y_min);
	auto xIndex_s = int_max(Index2D_min[0]);
	auto yIndex_s = int_max(Index2D_min[1]);
	auto zIndex_s = int_max(Index2D_min[2]);
	auto Index2D_max = this->Transform2DPositionTo2DIndex(RegionInfo.x_max, RegionInfo.y_max);
	auto xIndex_e = int_max(Index2D_max[0]);
	auto yIndex_e = int_max(Index2D_max[1]);
	auto zIndex_e = int_max(Index2D_max[2]);

	auto Size = this->GetSize();

	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	zIndex_s = (std::max)(zIndex_s, int_max(0)); zIndex_s = (std::min)(zIndex_s, Size[2] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);
	zIndex_e = (std::max)(zIndex_e, int_max(0)); zIndex_e = (std::min)(zIndex_e, Size[2] - 1);

	BoxRegionOf2DIndexInImage2D RegionOf2DIndex;
	RegionOf2DIndex.x_min = xIndex_s;
	RegionOf2DIndex.y_min = yIndex_s;
	RegionOf2DIndex.z_min = zIndex_s;
	RegionOf2DIndex.x_max = xIndex_e;
	RegionOf2DIndex.y_max = yIndex_e;
	RegionOf2DIndex.z_max = zIndex_e;
	return this->Get2DIndexListInRegion(RegionOf2DIndex);
}


template<typename PixelType>
DenseMatrix<int_max> DenseImage2D<PixelType>::Get2DIndexListInRegion(const BoxRegionOf3DWorldPositionInImage2D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseMatrix<int_max> EmptyList;
		return EmptyList;
	}

	auto Index2D_min = this->Transform3DWorldPositionTo2DIndex(RegionInfo.x_min, RegionInfo.y_min, RegionInfo.z_min);// projection
	auto xIndex_s = int_max(Index2D_min[0]);
	auto yIndex_s = int_max(Index2D_min[1]);

	auto Index2D_max = this->Transform3DWorldPositionTo2DIndex(RegionInfo.x_max, RegionInfo.y_max, RegionInfo.z_max);// projection
	auto xIndex_e = int_max(Index2D_max[0]);
	auto yIndex_e = int_max(Index2D_max[1]);

	auto Size = this->GetSize();

	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);

	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);

	BoxRegionOf2DIndexInImage2D RegionOf2DIndex;
	RegionOf2DIndex.x_min = xIndex_s;
	RegionOf2DIndex.y_min = yIndex_s;
	RegionOf2DIndex.x_max = xIndex_e;
	RegionOf2DIndex.y_max = yIndex_e;
	return this->Get2DIndexListInRegion(RegionOf2DIndex);
}


template<typename PixelType>
DenseImage2D<PixelType>
DenseImage2D<PixelType>::GetSubImage(const BoxRegionOf2DIndexInImage2D& RegionInfo) const
{
	DenseImage2D<PixelType> SubImage; // empty SubImage

	if (this->IsEmpty() == true)
	{
		MDK_Warning("DenseImage is empty @ DenseImage2D::GetSubImage()")
			return SubImage;
	}

	auto xIndex_s = int_max(std::round(RegionInfo.x_min));
	auto yIndex_s = int_max(std::round(RegionInfo.y_min));
	auto xIndex_e = int_max(std::round(RegionInfo.x_max));
	auto yIndex_e = int_max(std::round(RegionInfo.y_max));

	if (xIndex_e < xIndex_s || yIndex_e < yIndex_s)
	{
		MDK_Error("Invalid input @ DenseImage2D::GetSubImage(...)")
			return SubImage;
	}

	auto Size = this->GetSize();

	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);

	auto Lx = xIndex_e - xIndex_s + 1;
	auto Ly = yIndex_e - yIndex_s + 1;

	auto Origin_new = this->Transform2DIndexTo3DWorldPosition(xIndex_s, yIndex_s);

	SubImage.SetSize(Lx, Ly);
	SubImage.SetSpacing(this->GetSpacing());
	SubImage.SetOrigin(Origin_new);
	SubImage.SetOrientation(this->GetOrientation());

	auto SubPtr = SubImage.GetPixelPointer();
	auto RawPtr = this->GetPixelPointer();

	auto PixelCountPerZSlice = m_ImageData->m_PixelCountPerZSlice;

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
DenseImage2D<PixelType>::GetSubImage(const BoxRegionOf2DPositionInImage2D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseImage2D<PixelType> EmptyImage;
		return EmptyImage;
	}

	auto Index2D_min = this->Transform2DPositionTo2DIndex(RegionInfo.x_min, RegionInfo.y_min);
	auto xIndex_s = int_max(Index2D_min[0]);
	auto yIndex_s = int_max(Index2D_min[1]);
	auto Index2D_max = this->Transform2DPositionTo2DIndex(RegionInfo.x_max, RegionInfo.y_max);
	auto xIndex_e = int_max(Index2D_max[0]);
	auto yIndex_e = int_max(Index2D_max[1]);

	auto Size = this->GetSize();
	
	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);
	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);
	
	BoxRegionOf2DIndexInImage2D RegionOf2DIndex;
	RegionOf2DIndex.x_min = double(xIndex_s);
	RegionOf2DIndex.y_min = double(yIndex_s);
	RegionOf2DIndex.x_max = double(xIndex_e);
	RegionOf2DIndex.y_max = double(yIndex_e);
	return this->GetSubImage(RegionOf2DIndex);
}


template<typename PixelType>
DenseImage2D<PixelType>
DenseImage2D<PixelType>::GetSubImage(const BoxRegionOf3DWorldPositionInImage2D& RegionInfo) const
{
	if (this->IsEmpty() == true)
	{
		DenseImage2D<PixelType> EmptyImage;
		return EmptyImage;
	}

	auto Index2D_min = this->Transform3DWorldPositionTo2DIndex(RegionInfo.x_min, RegionInfo.y_min, RegionInfo.z_min);
	auto xIndex_s = int_max(Index2D_min[0]);
	auto yIndex_s = int_max(Index2D_min[1]);

	auto Index2D_max = this->Transform3DWorldPositionTo2DIndex(RegionInfo.x_max, RegionInfo.y_max, RegionInfo.z_max);
	auto xIndex_e = int_max(Index2D_max[0]);
	auto yIndex_e = int_max(Index2D_max[1]);


	auto Size = this->GetSize();

	xIndex_s = (std::max)(xIndex_s, int_max(0)); xIndex_s = (std::min)(xIndex_s, Size[0] - 1);
	yIndex_s = (std::max)(yIndex_s, int_max(0)); yIndex_s = (std::min)(yIndex_s, Size[1] - 1);

	xIndex_e = (std::max)(xIndex_e, int_max(0)); xIndex_e = (std::min)(xIndex_e, Size[0] - 1);
	yIndex_e = (std::max)(yIndex_e, int_max(0)); yIndex_e = (std::min)(yIndex_e, Size[1] - 1);

	BoxRegionOf2DIndexInImage2D RegionOf2DIndex;
	RegionOf2DIndex.x_min = double(xIndex_s);
	RegionOf2DIndex.y_min = double(yIndex_s);
	RegionOf2DIndex.x_max = double(xIndex_e);
	RegionOf2DIndex.y_max = double(yIndex_e);
	return this->GetSubImage(RegionOf2DIndex);
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

    if (Pad_Lx <= 0 && Pad_Ly <= 0)
    {
        MDK_Warning("Invalid Pad Size @ DenseImage2D::Pad")
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
				auto i = (std::min)((std::max)(temp_i - Pad_Lx, 0), Size[0] - 1);
				auto j = (std::min)((std::max)(temp_j - Pad_Ly, 0), Size[1] - 1);
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

    if (Pad_Lx == Size[0] || Pad_Ly == Size[1])
    {
        MDK_Warning("Output is empty @ DenseImage2D::UnPad(...)")
        return SubImage;
    }

    if (Pad_Lx == 0 && Pad_Ly == 0)
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