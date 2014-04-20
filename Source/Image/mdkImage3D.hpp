#ifndef __mdkImage3D_hpp
#define __mdkImage3D_hpp


namespace mdk
{

template<typename PixelType>
Image3DData<PixelType>::Image3DData()
{
    this->Clear();
}


template<typename PixelType>
Image3DData<PixelType>::~Image3DData()
{
}


template<typename PixelType>
void Image3DData<PixelType>::Clear()
{
    m_Size[0] = 0;
    m_Size[1] = 0;
    m_Size[2] = 0;

    m_PixelNumberPerZSlice = 0;

    m_PhysicalOrigin[0] = 0;
    m_PhysicalOrigin[1] = 0;
    m_PhysicalOrigin[2] = 0;

    m_PixelSpacing[0] = 0;
    m_PixelSpacing[1] = 0;
    m_PixelSpacing[2] = 0;

    m_Orientation.FastResize(3, 3);
    m_Orientation.FixSize();
    m_Orientation.FillDiangonal(1.0);

    m_DataArray.clear();

    m_NaNPixel = GetNaNElement(m_NaNPixel);
}


template<typename PixelType>
inline 
PixelType& Image3DData<PixelType>::operator[](int_max LinearIndex)
{ 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image3DData<PixelType>::operator[](int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
PixelType& Image3DData<PixelType>::operator()(int_max LinearIndex)
{
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image3DData<PixelType>::operator()(int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}
   

template<typename PixelType>
inline
PixelType& Image3DData<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex)
{
    auto LinearIndex = zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;
 
    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
const PixelType& Image3DData<PixelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    auto LinearIndex = zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;

    return m_DataArray[LinearIndex];
}


template<typename PixelType>
inline
int_max Image3DData<PixelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return zIndex*m_PixelNumberPerZSlice + yIndex*m_Size[0] + xIndex;    
}


template<typename PixelType>
inline 
void Image3DData<PixelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex, int_max& zIndex) const
{
    std::lldiv_t divresult;

    divresult = div(LinearIndex, m_PixelNumberPerZSlice);

    zIndex = divresult.quot;
            
    divresult = div(divresult.rem, m_Size[0]);

    yIndex = divresult.quot;

    xIndex = divresult.rem;
}


template<typename PixelType>
inline 
void Image3DData<PixelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex, double& x, double& y, double& z) const
{       
    std::lldiv_t divresult;

    divresult = div(LinearIndex, m_PixelNumberPerZSlice);

    z = double(divresult.quot);

    divresult = div(divresult.rem, m_Size[0]);

    y = double(divresult.quot);
      
    x = double(divresult.rem);

    x = m_PhysicalOrigin[0] + x * m_PixelSpacing[0];

    y = m_PhysicalOrigin[1] + y * m_PixelSpacing[1];

    z = m_PhysicalOrigin[2] + z * m_PixelSpacing[2];
}


template<typename PixelType>
inline
void Image3DData<PixelType>::Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex, double& x, double& y, double& z) const
{
    x = m_PhysicalOrigin[0] + double(xIndex) * m_PixelSpacing[0];

    y = m_PhysicalOrigin[1] + double(yIndex) * m_PixelSpacing[1];
   
    z = m_PhysicalOrigin[2] + double(zIndex) * m_PixelSpacing[2];
}


template<typename PixelType>
inline 
void Image3DData<PixelType>::Transform3DPhysicalPositionToContinuous3DIndex(double x, double y, double z, double& xIndex, double& yIndex, double& zIndex) const
{
    xIndex = (x - m_PhysicalOrigin[0]) / m_PixelSpacing[0];

    yIndex = (y - m_PhysicalOrigin[1]) / m_PixelSpacing[1];

    zIndex = (z - m_PhysicalOrigin[2]) / m_PixelSpacing[2];
}

//========================================================== Image ========================================================================//

template<typename PixelType>
Image3D<PixelType>::Image3D()
{
    m_ImageData = std::make_shared<Image3DData<PixelType>>();

    this->Clear();
}


template<typename PixelType>
Image3D<PixelType>::~Image3D()
{
}


template<typename PixelType>
Image3D<PixelType>::Image3D(Image3D<PixelType>&& InputImage)
{
    m_ImageData = std::move(InputImage.m_ImageData);

    m_PixelPointer = m_ImageData->m_DataArray.data();
}


template<typename PixelType>
void Image3D<PixelType>::operator=(Image3D<PixelType>&& InputImage)
{
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

    this->CopyData(InputImage.GetPixelPointer(), InputImage.GetPixelNumber());
    this->SetSize(InputImage.GetSize());
    this->SetPixelSpacing(InputImage.GetPixelSpacing());
    this->SetPhysicalOrigin(InputImage.GetPhysicalOrigin);
    this->SetOrientation(InputImage.GetOrientation());
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
bool Image3D<PixelType>::CopyData(const PixelType_Input* InputPixelPointer, int_max InputPixelNumber)
{
    if (InputPixelPointer == nullptr || InputPixelNumber <= 0)
	{
        MDK_Error("Invalid input @ 3DImage::CopyData(...)")
		return false;
	}

    auto SelfPixelNumber = this->GetPixelNumber();

    if (SelfPixelNumber != InputPixelNumber)
    {
        MDK_Error("Size does not match @ 3DImage::CopyData(...)")
        return false;
    }

    auto PixelPtr = this->GetPixelPointer();

    if (std::size_t(InputPixelPointer) == std::size_t(PixelPtr))
    {
        MDK_Warning("An image tries to Copy itself @ 3DImage::CopyData(...)")
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
    
    m_ImageData->m_PixelSpacing[0] = InputImage.m_ImageData->m_PixelSpacing[0];
    m_ImageData->m_PixelSpacing[1] = InputImage.m_ImageData->m_PixelSpacing[1];
    m_ImageData->m_PixelSpacing[2] = InputImage.m_ImageData->m_PixelSpacing[2];

    m_ImageData->m_PhysicalOrigin[0] = InputImage.m_ImageData->m_PhysicalOrigin[0];
    m_ImageData->m_PhysicalOrigin[1] = InputImage.m_ImageData->m_PhysicalOrigin[1];
    m_ImageData->m_PhysicalOrigin[2] = InputImage.m_ImageData->m_PhysicalOrigin[2];
    
    m_ImageData->m_Orientation = std::move(InputImage.m_ImageData->m_Orientation);

    m_ImageData->m_DataArray = std::move(InputImage.m_ImageData->m_DataArray);
    
    m_PixelPointer = m_ImageData->m_DataArray.data();
    
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
    m_ImageData->Clear();

    m_PixelPointer = nullptr;
}


template<typename PixelType>
inline
bool Image3D<PixelType>::IsEmpty() const
{
    return (m_ImageData->m_DataArray.size() == 0);
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
Image3DSize Image3D<PixelType>::GetSize() const
{
    Image3DSize Size;

    Size.Lx = m_ImageData->m_Size[0];
    Size.Ly = m_ImageData->m_Size[1];
    Size.Lz = m_ImageData->m_Size[2];

    return Size;
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
bool Image3D<PixelType>::SetSize(const Image3DSize& Size)
{
    return this->SetSize(Size.Lx, Size.Ly, Size.Lz);
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
Image3DPixelSpacing Image3D<PixelType>::GetPixelSpacing() const
{
    Image3DPixelSpacing Size;

    Size.Sx = m_ImageData->m_PixelSpacing[0];
    Size.Sy = m_ImageData->m_PixelSpacing[1];
    Size.Sz = m_ImageData->m_PixelSpacing[2];

    return Size;
}


template<typename PixelType>
inline
void Image3D<PixelType>::GetPixelSpacing(double& Spacing_x, double& Spacing_y, double& Spacing_z) const
{
    Spacing_x = m_ImageData->m_PixelSpacing[0];
    Spacing_y = m_ImageData->m_PixelSpacing[1];
    Spacing_z = m_ImageData->m_PixelSpacing[2];
}


template<typename PixelType>
inline
void Image3D<PixelType>::SetPixelSpacing(const Image3DPixelSpacing& Spacing)
{
    this->SetPixelSpacing(Spacing.Sx, Spacing.Sy, Spacing.Sz);
}


template<typename PixelType>
inline
void Image3D<PixelType>::SetPixelSpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
    if (Spacing_x <= 0 || Spacing_y <= 0 || Spacing_z <= 0)
    {
        MDK_Error("Invalid input @ 3DImage::SetPixelSpacing(...)")
        return;
    }

    m_ImageData->m_PixelSpacing[0] = Spacing_x;
    m_ImageData->m_PixelSpacing[1] = Spacing_y;
    m_ImageData->m_PixelSpacing[2] = Spacing_z;
}


template<typename PixelType>
inline
Image3DPhysicalOrigin Image3D<PixelType>::GetPhysicalOrigin() const
{
    Image3DPhysicalOrigin Origin;

    Origin.x = m_ImageData->m_PhysicalOrigin[0];
    Origin.y = m_ImageData->m_PhysicalOrigin[1];
    Origin.z = m_ImageData->m_PhysicalOrigin[2];

    return Origin;
}


template<typename PixelType>
inline
void Image3D<PixelType>::GetPhysicalOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const
{
    Origin_x = m_ImageData->m_PhysicalOrigin[0];
    Origin_y = m_ImageData->m_PhysicalOrigin[1];
    Origin_z = m_ImageData->m_PhysicalOrigin[2];
}


template<typename PixelType>
inline
void Image3D<PixelType>::SetPhysicalOrigin(const Image3DPhysicalOrigin& Origin)
{
    this->SetPhysicalOrigin(Origin.x, Origin.y, Origin.z);
}


template<typename PixelType>
inline
void Image3D<PixelType>::SetPhysicalOrigin(double Origin_x, double Origin_y, double Origin_z)
{
    m_ImageData->m_PhysicalOrigin[0] = Origin_x;
    m_ImageData->m_PhysicalOrigin[1] = Origin_y;
    m_ImageData->m_PhysicalOrigin[2] = Origin_z;
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
Image3DPhysicalSize Image3D<PixelType>::GetPhysicalSize() const
{
    Image3DPhysicalSize Size;

    Size.Lx = m_ImageData->m_Size[0] * m_ImageData->m_PixelSpacing[0];

    Size.Ly = m_ImageData->m_Size[1] * m_ImageData->m_PixelSpacing[1];

    Size.Lz = m_ImageData->m_Size[2] * m_ImageData->m_PixelSpacing[2];

    return Size;
}


template<typename PixelType>
inline 
void Image3D<PixelType>::GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y, double& PhysicalSize_z) const
{
    PhysicalSize_x = m_ImageData->m_Size[0] * m_ImageData->m_PixelSpacing[0];

    PhysicalSize_y = m_ImageData->m_Size[1] * m_ImageData->m_PixelSpacing[1];

    PhysicalSize_z = m_ImageData->m_Size[2] * m_ImageData->m_PixelSpacing[2];
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
inline
void Image3D<PixelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex, int_max& zIndex) const
{
    m_ImageData->TransformLinearIndexTo3DIndex(LinearIndex, xIndex, yIndex, zIndex);
}


template<typename PixelType>
inline
void Image3D<PixelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex, double& x, double& y, double& z) const
{
    m_ImageData->TransformLinearIndexTo3DPhysicalPosition(LinearIndex, x, y, z);
}


template<typename PixelType>
inline
void Image3D<PixelType>::Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex, double& x, double& y, double& z) const
{
    m_ImageData->Transform3DIndexTo3DPhysicalPosition(xIndex, yIndex, zIndex, x, y, z);
}


template<typename PixelType>
inline 
void Image3D<PixelType>::Transform3DPhysicalPositionToContinuous3DIndex(double x, double y, double z, double& xIndex, double& yIndex, double& zIndex) const
{
    m_ImageData->Transform3DPhysicalPositionToContinuous3DIndex(x, y, z, xIndex, yIndex, zIndex);
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

	if (xIndex >= Size.Lx || xIndex < 0 || yIndex >= Size.Ly || yIndex < 0 || zIndex >= Size.Lz || zIndex < 0)
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

	if (xIndex >= Size.Lx || xIndex < 0 || yIndex >= Size.Ly || yIndex < 0 || zIndex >= Size.Lz || zIndex < 0)
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
Image3D<PixelType> Image3D<PixelType>::GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e, int_max zIndex_s, int_max zIndex_e) const
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

	Lx = xIndex_e - xIndex_s + 1;
	Ly = yIndex_e - yIndex_s + 1;
	Lz = zIndex_e - zIndex_s + 1;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());

    auto tempRawPtr = tempImage.GetPixelPointer();

    auto RawPtr = this->GetPixelPointer();

    PixelNumberPerZSlice = m_ImageData->PixelNumberPerZSlice;

	int_max temp_k   temp_k = zIndex_s;

	for (int_max k = zIndex_s; k <= zIndex_e; ++k)
	{
        auto temp_k = k*PixelNumberPerZSlice;

		for (int_max j = yIndex_s; j <= yIndex_e; ++j)
		{
            auto temp_j = j*Size.Lx;

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

    auto Lx = Size.Lx + Pad_Lx;
    auto Ly = Size.Ly + Pad_Ly;
    auto Lz = Size.Lz + Pad_Lz;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());


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
    Image3D<PixelType> tempImage; // empty image
    
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

    auto Lx = Size.Lx + Pad_Lx;
    auto Ly = Size.Ly + Pad_Ly;
    auto Lz = Size.Lz + Pad_Lz;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());

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

    if (Pad_Lx > Size.Lx || Pad_Lx  < 0 || Pad_Ly > Size.Ly || Pad_Ly < 0 || Pad_Lz > Size.Lz || Pad_Lz < 0)
	{
		MDK_Error("Invalid Pad Size @ 3DImage::UnPad")
		return tempImage;
	}

    if (Pad_Lx == Size.Lx || Pad_Ly == Size.Ly || Pad_Lz == Size.Lz)
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

    return this->GetSubImage(Pad_Lx, Size.Lx - 1 - Pad_Lx,
                             Pad_Ly, Size.Ly - 1 - Pad_Ly,
                             Pad_Lz, Size.Lz - 1 - Pad_Lz);
                             

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
            auto temp_j = j*Size.Lx;

			for (int_max i = xIndex_s; i < xIndex_s + Region_Lx; ++i)
			{
                List[Counter] = temp_k + temp_j + i;

			    Counter += 1;
			}
		}
	}
    
	return List;
}


}//end namespace mdk

#endif