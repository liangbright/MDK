#ifndef __mdkImage_hpp
#define __mdkImage_hpp

//#include "mdkImage.h"

namespace mdk
{

template<typename VoxelType>
ImageData<VoxelType>::ImageData()
{
    this->Clear();
}


template<typename VoxelType>
ImageData<VoxelType>::~ImageData()
{
}


template<typename VoxelType>
void ImageData<VoxelType>::Clear()
{
    m_Size[0] = 0;
    m_Size[1] = 0;
    m_Size[2] = 0;

    m_VoxelNumberPerZSlice = 0;

    m_PhysicalOrigin[0] = 0;
    m_PhysicalOrigin[1] = 0;
    m_PhysicalOrigin[2] = 0;

    m_VoxelSpacing[0] = 0;
    m_VoxelSpacing[1] = 0;
    m_VoxelSpacing[2] = 0;

    m_Orientation.FastResize(3, 3);
    m_Orientation.FixSize();
    m_Orientation.FillDiagnal(1.0);

    m_DataArray.clear();

    m_NaNVoxel = GetNaNElement(m_NaNVoxel);
}


template<typename VoxelType>
inline 
VoxelType& ImageData<VoxelType>::operator[](int_max LinearIndex)
{ 
    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& ImageData<VoxelType>::operator[](int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
VoxelType& ImageData<VoxelType>::operator()(int_max LinearIndex)
{
    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& ImageData<VoxelType>::operator()(int_max LinearIndex) const
{
    return m_DataArray[LinearIndex];
}
   

template<typename VoxelType>
inline
VoxelType& ImageData<VoxelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex)
{
    auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_Size[0] + xIndex;
 
    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& ImageData<VoxelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_Size[0] + xIndex;

    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
int_max ImageData<VoxelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return zIndex*m_VoxelNumberPerZSlice + yIndex*m_Size[0] + xIndex;    
}


template<typename VoxelType>
inline 
void ImageData<VoxelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex, int_max& zIndex) const
{
    std::lldiv_t divresult;

    divresult = div(LinearIndex, m_VoxelNumberPerZSlice);

    zIndex = divresult.quot;
            
    divresult = div(divresult.rem, m_Size[0]);

    yIndex = divresult.quot;

    xIndex = divresult.rem;
}


template<typename VoxelType>
inline 
void ImageData<VoxelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex, double& x, double& y, double& z) const
{       
    std::lldiv_t divresult;

    divresult = div(LinearIndex, m_VoxelNumberPerZSlice);

    z = divresult.quot;

    divresult = div(divresult.rem, m_Size[0]);

    y = divresult.quot;
      
    x = divresult.rem;

    x = m_PhysicalOrigin[0] + x * m_VoxelSpacing[0];

    y = m_PhysicalOrigin[1] + y * m_VoxelSpacing[1];

    z = m_PhysicalOrigin[2] + z * m_VoxelSpacing[2];
}


template<typename VoxelType>
inline
void ImageData<VoxelType>::Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex, double& x, double& y, double& z) const
{
    x = m_PhysicalOrigin[0] + double(xIndex) * m_VoxelSpacing[0];

    y = m_PhysicalOrigin[1] + double(yIndex) * m_VoxelSpacing[1];
   
    z = m_PhysicalOrigin[2] + double(zIndex) * m_VoxelSpacing[2];
}


template<typename VoxelType>
inline 
void ImageData<VoxelType>::Transform3DPhysicalPositionToContinuous3DIndex(double x, double y, double z, double& xIndex, double& yIndex, double& zIndex) const
{
    xIndex = (x - m_PhysicalOrigin[0]) / m_VoxelSpacing[0];

    yIndex = (y - m_PhysicalOrigin[1]) / m_VoxelSpacing[1];

    zIndex = (z - m_PhysicalOrigin[2]) / m_VoxelSpacing[2];
}

//========================================================== Image ========================================================================//

template<typename VoxelType>
Image<VoxelType>::Image()
{
    m_ImageData = std::make_shared<ImageData<VoxelType>>();

    this->Clear();
}


template<typename VoxelType>
Image<VoxelType>::~Image()
{
}


template<typename VoxelType>
Image<VoxelType>::Image(Image<VoxelType>&& InputImage)
{
    m_ImageData = std::move(InputImage.m_ImageData);

    m_VoxelPointer = m_ImageData->m_DataArray.data();

    m_ZeroVoxel = InputImage.m_ZeroVoxel;

    m_ZeroVoxel_Error_Output = m_ZeroVoxel;
}


template<typename VoxelType>
void Image<VoxelType>::operator=(Image<VoxelType>&& InputImage)
{
    this->Take(InputImage);

    InputImage.Clear();
}


template<typename VoxelType>
template<typename VoxelType_Input>
void Image<VoxelType>::Copy(const Image<VoxelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        MDK_Warning("try to Copy self @ Image::Copy(InputImage)")
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

    this->CopyData(InputImage.GetVoxelPointer(), InputImage.GetVoxelNumber());
    this->SetSize(InputImage.GetSize());
    this->SetVoxelSpacing(InputImage.GetVoxelSpacing());
    this->SetPhysicalOrigin(InputImage.GetPhysicalOrigin);
    this->SetOrientation(InputImage.GetOrientation());
}


template<typename VoxelType>
template<typename VoxelType_Input>
bool Image<VoxelType>::Copy(const Image<VoxelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ Image::Copy(Image* InputImage)")
        return false;
    }

    this->Copy(*InputImage);

    return true;
}


template<typename VoxelType>
template<typename VoxelType_Input>
bool Image<VoxelType>::CopyData(const VoxelType_Input* InputVoxelPointer, int_max InputVoxelNumber)
{
    if (InputVoxelPointer == nullptr || InputVoxelNumber <= 0)
	{
        MDK_Error("Invalid input @ Image::CopyData(...)")
		return false;
	}

    auto SelfVoxelNumber = this->GetVoxelNumber();

    if (SelfVoxelNumber != InputVoxelNumber)
    {
        MDK_Error("Size does not match @ Image::CopyData(...)")
        return false;
    }

    auto VoxelPtr = this->GetVoxelPointer();

    if (std::size_t(InputVoxelPointer) == std::size_t(VoxelPtr))
    {
        MDK_Warning("An image tries to Copy itself @ Image::CopyData(...)")
        return true;
    }
  
    for (int_max i = 0; i < SelfVoxelNumber; ++i)
	{
        VoxelPtr[i] = VoxelType(InputVoxelPointer[i]);
	}

    return true;
}


template<typename VoxelType>
bool Image<VoxelType>::Fill(const VoxelType& Voxel)
{
    auto VoxelNumber = this->GetVoxelNumber();

    if (VoxelNumber == 0)
    {
        return false;
    }

    auto BeginPtr = this->GetVoxelPointer();

    for (auto Ptr = BeginPtr; Ptr < BeginPtr + VoxelNumber; ++Ptr)
    {
        Ptr[0] = Voxel;
    }

    return true;
}


template<typename VoxelType>
bool Image<VoxelType>::Share(Image<VoxelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary

    m_VoxelPointer = m_ImageData->m_DataArray.data();

    m_ZeroVoxel = InputImage.m_ZeroVoxel;

    m_ZeroVoxel_Error_Output = m_ZeroVoxel;

    return true;
}


template<typename VoxelType>
bool Image<VoxelType>::Share(Image<VoxelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ Image::Share(Image* InputImage)")
        return false;
    }

    return this->Share(*InputImage);
}


template<typename VoxelType>
void Image<VoxelType>::ForceShare(const Image<VoxelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary

    m_VoxelPointer = m_ImageData->m_DataArray.data();
}


template<typename VoxelType>
bool Image<VoxelType>::ForceShare(const Image<VoxelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ Image::ForceShare(Image* InputImage)")
        return false;
    }

    return this->ForceShare(*InputImage);
}


template<typename VoxelType>
void Image<VoxelType>::Take(Image<VoxelType>&& InputImage)
{
    this->Take(std::forward<Image<VoxelType>&>(InputImage));
}


template<typename VoxelType>
void Image<VoxelType>::Take(Image<VoxelType>& InputImage)
{
    m_ImageData->m_Size[0] = InputImage.m_ImageData->m_Size[0];
    m_ImageData->m_Size[1] = InputImage.m_ImageData->m_Size[1];
    m_ImageData->m_Size[2] = InputImage.m_ImageData->m_Size[2];
    
    m_ImageData->m_VoxelNumberPerZSlice = InputImage.m_ImageData->m_VoxelNumberPerZSlice;
    
    m_ImageData->m_VoxelSpacing[0] = InputImage.m_ImageData->m_VoxelSpacing[0];
    m_ImageData->m_VoxelSpacing[1] = InputImage.m_ImageData->m_VoxelSpacing[1];
    m_ImageData->m_VoxelSpacing[2] = InputImage.m_ImageData->m_VoxelSpacing[2];

    m_ImageData->m_PhysicalOrigin[0] = InputImage.m_ImageData->m_PhysicalOrigin[0];
    m_ImageData->m_PhysicalOrigin[1] = InputImage.m_ImageData->m_PhysicalOrigin[1];
    m_ImageData->m_PhysicalOrigin[2] = InputImage.m_ImageData->m_PhysicalOrigin[2];
    
    m_ImageData->m_Orientation = std::move(InputImage.m_ImageData->m_Orientation);

    m_ImageData->m_DataArray = std::move(InputImage.m_ImageData->m_DataArray);
    
    m_VoxelPointer = m_ImageData->m_DataArray.data();
    
    InputImage.Clear();
}


template<typename VoxelType>
void Image<VoxelType>::Clear()
{
    m_ImageData->Clear();

    m_VoxelPointer = nullptr;
}


template<typename VoxelType>
bool Image<VoxelType>::Initialize(int_max Lx, int_max Ly, int_max Lz,                                  
                                  double VoxelSpacing_x,   double VoxelSpacing_y,   double VoxelSpacing_z,
                                  double PhysicalOrigin_x, double PhysicalOrigin_y, double PhysicalOrigin_z,
                                  const DenseMatrix<double>& Orientation)
{
    if (Lx < 0 || Ly < 0 || Lz < 0 || VoxelSpacing_x < 0.0 || VoxelSpacing_y < 0.0 || VoxelSpacing_z < 0.0)
    {
        MDK_Error("Invalid input @ Image::Initialize")
        return false;
    }

    m_ImageData->m_Size[0] = Lx;
    m_ImageData->m_Size[1] = Ly;
    m_ImageData->m_Size[2] = Lz;

    m_ImageData->m_VoxelNumberPerZSlice = Ly*Lx;

    m_ImageData->m_VoxelSpacing[0] = VoxelSpacing_x;
    m_ImageData->m_VoxelSpacing[1] = VoxelSpacing_y;
    m_ImageData->m_VoxelSpacing[2] = VoxelSpacing_z;

    m_ImageData->m_PhysicalOrigin[0] = PhysicalOrigin_x;
    m_ImageData->m_PhysicalOrigin[1] = PhysicalOrigin_y;
    m_ImageData->m_PhysicalOrigin[2] = PhysicalOrigin_z;


    if (Orientation.IsEmpty() == false)
    {
        if (Orientation.GetColNumber() == 3 && Orientation.GetRowNumber() == 3)
        {
            m_ImageData->m_Orientation = Orientation;
        }
        else
        {
            MDK_Error("Invalid input Orientation @ Image::Initialize(...)")
        }
    }

    return this->SetSize(Lx, Ly, Lz);
}


template<typename VoxelType>
bool Image<VoxelType>::Initialize(const ImageSize& Size, const ImagePhysicalOrigin& Origin, const ImageVoxelSpacing& Spacing, const DenseMatrix<double>& Orientation)
{
    return this->Initialize(Size.Lx, Size.Ly, Size.Lz, Spacing.Sx, Spacing.Sy, Spacing.Sz, Origin.x, Origin.y, Origin.z, Orientation);
}


template<typename VoxelType>
inline
bool Image<VoxelType>::IsEmpty() const
{
    return (m_ImageData->m_DataArray.size() == 0);
}


template<typename VoxelType>
inline
VoxelType* Image<VoxelType>::GetVoxelPointer()
{
    return m_ImageData->m_DataArray.data();
}


template<typename VoxelType>
inline
const VoxelType* Image<VoxelType>::GetVoxelPointer() const
{
    return m_ImageData->m_DataArray.data();
}


template<typename VoxelType>
inline
ImageSize Image<VoxelType>::GetSize() const
{
    ImageSize Size;

    Size.Lx = m_ImageData->m_Size[0];
    Size.Ly = m_ImageData->m_Size[1];
    Size.Lz = m_ImageData->m_Size[2];

    return Size;
}


template<typename VoxelType>
inline
bool Image<VoxelType>::SetSize(const ImageSize& Size)
{
    return this->SetSize(Size.Lx, Size.Ly, Size.Lz);
}


template<typename VoxelType>
inline
bool Image<VoxelType>::SetSize(int_max Lx, int_max Ly, int_max Lz)
{
try
{
    m_ImageData->m_DataArray.resize(Lx*Ly*Lz);
    m_VoxelPointer = m_ImageData->m_DataArray.data();
}
catch (...)
{
    MDK_Error("Out Of Memory @ Image::SetSize(...)")

    this->Clear();
    return false;
}

    return true;
}


template<typename VoxelType>
inline
ImageVoxelSpacing Image<VoxelType>::GetVoxelSpacing() const
{
    ImageVoxelSpacing Size;

    Size.Sx = m_ImageData->m_VoxelSpacing[0];
    Size.Sy = m_ImageData->m_VoxelSpacing[1];
    Size.Sz = m_ImageData->m_VoxelSpacing[2];

    return Size;
}


template<typename VoxelType>
inline
void Image<VoxelType>::SetVoxelSpacing(const ImageVoxelSpacing& Spacing)
{
    this->SetVoxelSpacing(Spacing.Sx, Spacing.Sy, Spacing.Sz);
}


template<typename VoxelType>
inline
void Image<VoxelType>::SetVoxelSpacing(double VoxelSpacing_x, double VoxelSpacing_y, double VoxelSpacing_z)
{
    if (VoxelSpacing_x <= 0 || VoxelSpacing_y <= 0 || VoxelSpacing_z <= 0)
    {
        MDK_Error("Invalid input @ Image::SetVoxelSpacing(...)")
        return;
    }

    m_ImageData->m_VoxelSpacing[0] = VoxelSpacing_x;
    m_ImageData->m_VoxelSpacing[1] = VoxelSpacing_y;
    m_ImageData->m_VoxelSpacing[2] = VoxelSpacing_z;
}


template<typename VoxelType>
inline
ImagePhysicalOrigin Image<VoxelType>::GetPhysicalOrigin() const
{
    ImagePhysicalOrigin Origin;

    Origin.x = m_ImageData->m_PhysicalOrigin[0];
    Origin.y = m_ImageData->m_PhysicalOrigin[1];
    Origin.z = m_ImageData->m_PhysicalOrigin[2];

    return Origin;
}


template<typename VoxelType>
inline
void Image<VoxelType>::SetPhysicalOrigin(const ImagePhysicalOrigin& Origin)
{
    this->SetPhysicalOrigin(Origin.x, Origin.y, Origin.z);
}


template<typename VoxelType>
inline
void Image<VoxelType>::SetPhysicalOrigin(double Origin_x, double Origin_y, double Origin_z)
{
    m_ImageData->m_PhysicalOrigin[0] = Origin_x;
    m_ImageData->m_PhysicalOrigin[1] = Origin_y;
    m_ImageData->m_PhysicalOrigin[2] = Origin_z;
}


template<typename VoxelType>
inline 
const DenseMatrix<double>& Image<VoxelType>::GetOrientation() const
{
    return m_ImageData->m_Orientation;
}


template<typename VoxelType>
inline 
void Image<VoxelType>::SetOrientation(const DenseMatrix<double>& Orientation)
{
    if (Orientation.IsEmpty() == false)
    {
        if (Orientation.GetColNumber() == 3 && Orientation.GetRowNumber() == 3)
        {
            m_ImageData->m_Orientation = Orientation;
        }
        else
        {
            MDK_Error("Invalid input Orientation @ Image::SetOrientation(...)")
        }
    }
}


template<typename VoxelType>
inline 
ImagePhysicalSize Image<VoxelType>::GetPhysicalSize() const
{
    ImagePhysicalSize Size;

    Size.Sx = m_ImageData->m_Size[0] * m_ImageData->m_VoxelSpacing[0];

    Size.Sy = m_ImageData->m_Size[1] * m_ImageData->m_VoxelSpacing[1];

    Size.Sz = m_ImageData->m_Size[2] * m_ImageData->m_VoxelSpacing[2];

    return Size;
}


template<typename VoxelType>
inline
int_max Image<VoxelType>::GetVoxelNumber() const
{
    return m_ImageData->m_VoxelNumberPerZSlice * m_ImageData->m_Size[0];
}
 

template<typename VoxelType>
inline
int_max Image<VoxelType>::Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    return m_ImageData->Transform3DIndexToLinearIndex(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
void Image<VoxelType>::TransformLinearIndexTo3DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex, int_max& zIndex) const
{
    m_ImageData->TransformLinearIndexTo3DIndex(LinearIndex, xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
void Image<VoxelType>::TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex, double& x, double& y, double& z) const
{
    m_ImageData->TransformLinearIndexTo3DPhysicalPosition(LinearIndex, x, y, z);
}


template<typename VoxelType>
inline
void Image<VoxelType>::Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex, double& x, double& y, double& z) const
{
    m_ImageData->Transform3DIndexTo3DPhysicalPosition(xIndex, yIndex, zIndex, x, y, z);
}


template<typename VoxelType>
inline 
void Image<VoxelType>::Transform3DPhysicalPositionToContinuous3DIndex(double x, double y, double z, double& xIndex, double& yIndex, double& zIndex) const
{
    m_ImageData->Transform3DPhysicalPositionToContinuous3DIndex(x, y, z, xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
VoxelType& Image<VoxelType>::operator[](int_max LinearIndex)
{
#if defined(MDK_DEBUG_Image_Operator_CheckBound)

    auto VoxelNumber = this->GetVoxelNumber();

    if (LinearIndex >= VoxelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ Image::operator(LinearIndex)")
        return m_ImageData->m_NaNVoxel;
    }

#endif //MDK_DEBUG_Image_Operator_CheckBound

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& Image<VoxelType>::operator[](int_max LinearIndex) const
{
#if defined(MDK_DEBUG_Image_Operator_CheckBound)

    auto VoxelNumber = this->GetVoxelNumber();

    if (LinearIndex >= VoxelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ Image::operator(LinearIndex)")
        return m_ImageData->m_NaNVoxel;
    }

#endif //MDK_DEBUG_Image_Operator_CheckBound

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
VoxelType& Image<VoxelType>::operator()(int_max LinearIndex)
{
#if defined(MDK_DEBUG_Image_Operator_CheckBound)

    auto VoxelNumber = this->GetVoxelNumber();

	if (LinearIndex >= VoxelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ Image::operator(LinearIndex)")
		return m_ImageData->m_NaNVoxel;
	}

#endif //MDK_DEBUG_Image_Operator_CheckBound

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& Image<VoxelType>::operator()(int_max LinearIndex) const
{
#if defined(MDK_DEBUG_Image_Operator_CheckBound)

    auto VoxelNumber = this->GetVoxelNumber();

	if (LinearIndex >= VoxelNumber || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ Image::operator(LinearIndex) const")
		return m_ImageData->m_NaNVoxel;
	}

#endif //MDK_DEBUG_Image_Operator_CheckBound

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
VoxelType& Image<VoxelType>::at(int_max LinearIndex)
{
    auto VoxelNumber = this->GetVoxelNumber();

    if (LinearIndex >= VoxelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ Image::at(LinearIndex)")
        return m_ImageData->m_NaNVoxel;
    }

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& Image<VoxelType>::at(int_max LinearIndex) const
{
    auto VoxelNumber = this->GetVoxelNumber();

    if (LinearIndex >= VoxelNumber || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ Image::at(LinearIndex)")
        return m_ImageData->m_NaNVoxel;
    }

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
VoxelType& Image<VoxelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex)
{
#if defined(MDK_DEBUG_Image_Operator_CheckBound)

    auto ImageDimension = this->GetDimension();

	if (xIndex >= ImageDimension.Lx || xIndex < 0 || yIndex >= ImageDimension.Ly || yIndex < 0 || zIndex >= ImageDimension.Lz || zIndex < 0)
	{
		MDK_Error("Invalid input @ Image::operator(xIndex, yIndex, zIndex)")
		return m_ImageData->m_NaNVoxel;
	}

#endif //MDK_DEBUG_Image_Operator_CheckBound
	
    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
const VoxelType& Image<VoxelType>::operator()(int_max xIndex, int_max yIndex, int_max zIndex) const
{
#if defined(MDK_DEBUG_Image_Operator_CheckBound)

    auto ImageDimension = this->GetDimension();

	if (xIndex >= ImageDimension.Lx || xIndex < 0 || yIndex >= ImageDimension.Ly || yIndex < 0 || zIndex >= ImageDimension.Lz || zIndex < 0)
	{
        MDK_Error("Invalid input @ Image::operator(xIndex, yIndex, zIndex) const")
        return m_ImageData->m_NaNVoxel;
	}

#endif //MDK_DEBUG_Image_Operator_CheckBound

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
VoxelType& Image<VoxelType>::at(int_max xIndex, int_max yIndex, int_max zIndex)
{
    auto ImageDimension = this->GetDimension();

	if (xIndex >= ImageDimension[0] || xIndex < 0 || yIndex >= ImageDimension[1] || yIndex < 0 || zIndex >= ImageDimension[2] || zIndex < 0)
	{
		MDK_Error("Invalid input @ Image::at(xIndex, yIndex, zIndex)")
		return m_ImageData->m_NaNVoxel;
	}

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
const VoxelType& Image<VoxelType>::at(int_max xIndex, int_max yIndex, int_max zIndex) const
{
    auto ImageDimension = this->GetDimension();

    if (xIndex >= ImageDimension[0] || xIndex < 0 || yIndex >= ImageDimension[1] || yIndex < 0 || zIndex >= ImageDimension[2] || zIndex < 0)
    {
        MDK_Error("Invalid input @ Image::at(xIndex, yIndex, zIndex) const")
        return m_ImageData->m_NaNVoxel;
    }

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
Image<VoxelType> Image<VoxelType>::GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e, int_max zIndex_s = 0, int_max zIndex_e) const
{
    Image<VoxelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ Image::GetSubImage()")
        return tempImage;
    }

    auto ImageDimension = this->GetDimension();

    if (xIndex_s >= ImageDimension[0] || xIndex_s < 0
        || xIndex_e >= ImageDimension[0] || xIndex_e < 0
        || xIndex_s > xIndex_e
        || yIndex_s >= ImageDimension[1] || yIndex_s < 0
        || yIndex_e >= ImageDimension[1] || yIndex_e < 0
        || yIndex_s > yIndex_e
        || zIndex_s >= ImageDimension[2] || zIndex_s < 0
        || zIndex_e >= ImageDimension[2] || zIndex_e < 0
        || zIndex_s > zIndex_e)
	{
        MDK_Error("Invalid input @ Image::GetSubImage() const")
		return tempImage;
	}

	Lx = xIndex_e - xIndex_s + 1;
	Ly = yIndex_e - yIndex_s + 1;
	Lz = zIndex_e - zIndex_s + 1;

	tempImage.ReInitialize(Lx, Ly, Lz,
  		                 m_PhysicalOrigin[0],  m_PhysicalOrigin[1], m_PhysicalOrigin[2],
		                 m_VoxelSpacing[0],    m_VoxelSpacing[1],   m_VoxelSpacing[2]);

	tempRawPtr = SubImage.GetVoxelPointer();

    RawPtr = this->GetVoxelPointer();

    VoxelNumberPerZSlice = m_ImageData->VoxelNumberPerZSlice;

	int_max Index_k = 0;

	int_max Index_j = 0;

	Index_k = zIndex_s;

	for (int_max k = zIndex_s; k <= zIndex_e; ++k)
	{
		Index_j = yIndex_s;

		for (int_max j = yIndex_s; j <= yIndex_e; ++j)
		{
			for (int_max i = xIndex_s; i <= xIndex_e; ++i)
			{
				tempRawPtr[0] = RawPtr[Index_k + Index_j + i];

				++tempRawPtr;
			}

			Index_j += Lx;
		}

		Index_k += VoxelNumberPerZSlice;
	}
    
	return tempImage;
}


template<typename VoxelType>
Image<VoxelType> 
Image<VoxelType>::
Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    Image<VoxelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ Image::Pad")
        return tempImage;
    }

    if (Option != "replicate")
	{
		MDK_Error("Invalid Option @ Image::Pad")
		return tempImage;
	}

    if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
	{
		MDK_Warning("Invalid Pad Size @ Image::Pad")

		tempImage = (*this);

		return tempImage;
	}

    auto ImageDimension = this->GetDimension();

    auto PhysicalOrigin = this->GetPhysicalOrigin();

    auto VoxelSpacing = this->GetVoxelSpacing();

	auto Lx = ImageDimension[0] + Pad_Lx;
    auto Ly = ImageDimension[1] + Pad_Ly;
    auto Lz = ImageDimension[2] + Pad_Lz;

	tempImage.ReInitialize(Lx, Ly, Lz,
                           PhysicalOrigin[0],   PhysicalOrigin[1],    PhysicalOrigin[2],
                           VoxelSpacing[0],     VoxelSpacing[1],      VoxelSpacing[2]);


	if (OptionStr == "zero")
	{
        tempImage.Fill(VoxelType(0));

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
					auto i = std::min(std::max(temp_i - Pad_Lx, 0), ImageDimension[0] - 1);

					auto j = std::min(std::max(temp_j - Pad_Ly, 0), ImageDimension[1] - 1);

					auto k = std::min(std::max(temp_k - Pad_Lz, 0), ImageDimension[2] - 1);

					tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
				}
			}
		}
	}
    

    return tempImage;
}


template<typename VoxelType>
Image<VoxelType>
Image<VoxelType>::
Pad(VoxelType Voxel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    Image<VoxelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ Image::Pad")
        return tempImage;
    }

	if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
	{
		MDK_Warning("Invalid Pad Size @ Image::Pad")

		tempImage = (*this);

		return tempImage;
	}

    auto ImageDimension = this->GetDimension();

    auto PhysicalOrigin = this->GetPhysicalOrigin();

    auto VoxelSpacing = this->GetVoxelSpacing();

	auto Lx = ImageDimension[0] + Pad_Lx;
    auto Ly = ImageDimension[1] + Pad_Ly;
    auto Lz = ImageDimension[2] + Pad_Lz;

	tempImage.ReInitialize(Lx, Ly, Lz,
                           PhysicalOrigin[0], PhysicalOrigin[1], PhysicalOrigin[2],
                           VoxelSpacing[0],   VoxelSpacing[1],   VoxelSpacing[2]);

    tempImage.Fill(Voxel);

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


template<typename VoxelType>
Image<VoxelType> 
Image<VoxelType>::
UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    Image<VoxelType> tempImage; // empty image
   
    auto ImageDimension = this->GetDimension();

    if (Pad_Lx > ImageDimension[0] || Pad_Lx  < 0 || Pad_Ly > ImageDimension[1] || Pad_Ly < 0 || Pad_Lz > ImageDimension[2] || Pad_Lz < 0)
	{
		MDK_Error("Invalid Pad Size @ Image::UnPad")
		return tempImage;
	}

	if (Pad_Lx == ImageDimension[0] || Pad_Ly == ImageDimension[1] || Pad_Lz == ImageDimension[2])
	{
		MDK_Warning("Output is empty @ Image::UnPad")
		return tempImage;
	}

	if (Pad_Lx == 0 && Pad_Ly == 0 && Pad_Lz == 0)
	{
		MDK_Warning("Input Pad Size is [0, 0, 0] @ Image::UnPad")

		tempImage = (*this);

		return tempImage;
	}

	return this->GetSubImage(Pad_Lx, ImageDimension[0] - 1 - Pad_Lx,
		                     Pad_Ly, ImageDimension[1] - 1 - Pad_Ly,
		                     Pad_Lz, ImageDimension[2] - 1 - Pad_Lz);
                             

}


template<typename VoxelType>
DenseMatrix<int_max>
Image<VoxelType>::
GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
                           int_max yIndex_s,     int_max Region_Ly,
                           int_max zIndex_s,     int_max Region_Lz) const
{
    DenseMatrix<int_max>  List;
    
    auto ImageDimension = this->GetDimension();

    if (   xIndex_s >= ImageDimension[0] || xIndex_s < 0
        || yIndex_s >= ImageDimension[1] || yIndex_s < 0
        || zIndex_s >= ImageDimension[2] || zIndex_s < 0
        || Region_Lx > ImageDimension[0] - xIndex_s || Region_Lx < 0
        || Region_Ly > ImageDimension[1] - yIndex_s || Region_Ly < 0
        || Region_Lz > ImageDimension[2] - zIndex_s || Region_Lz < 0)
	{
		MDK_Error("Invalid input @ Image::GetLinearIndexArrayOfRegion")
		return List;
	}

	if (Region_Lx == 0 || Region_Ly == 0 || Region_Lz == 0)
	{
		MDK_Warning("Empty input region @ Image::GetLinearIndexArrayOfRegion")
		return List;
	}

    List.Resize(Region_Lx*Region_Ly*Region_Lz, 1);

    auto VoxelNumberPerZSlice = m_ImageData->VoxelNumberPerZSlice;

	int_max Counter = 0;

	for (int_max k = zIndex_s; k < zIndex_s + Region_Lz; ++k)
	{
		for (int_max j = yIndex_s; j < yIndex_s + Region_Ly; ++j)
		{
			for (int_max i = xIndex_s; i < xIndex_s + Region_Lx; ++i)
			{
				List[Counter] = k*VoxelNumberPerZSlice + j*ImageDimension[0] + i;

			    Counter += 1;
			}
		}
	}
    
	return List;
}


}//end namespace mdk

#endif