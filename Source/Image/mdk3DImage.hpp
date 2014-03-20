#ifndef __mdk3DImage_hpp
#define __mdk3DImage_hpp

#include <cstdlib>

#include "mdk3DImage.h"
#include "mdk3DImageVoxel.h"

namespace mdk
{

template<typename VoxelType>
mdk3DImageData<VoxelType>::mdk3DImageData()
{
    this->Reset();
}


template<typename VoxelType>
mdk3DImageData<VoxelType>::~mdk3DImageData()
{
}


template<typename VoxelType>
void mdk3DImageData<VoxelType>::Reset()
{
    m_ImageDimension[0] = 0;
    m_ImageDimension[1] = 0;
    m_ImageDimension[2] = 0;

    m_VoxelNumberPerZSlice = 0;

    m_PhysicalOrigin[0] = 0;
    m_PhysicalOrigin[1] = 0;
    m_PhysicalOrigin[2] = 0;

    m_VoxelPhysicalSize[0] = 0;
    m_VoxelPhysicalSize[1] = 0;
    m_VoxelPhysicalSize[2] = 0;

    m_DataArray.clear();
}


template<typename VoxelType>
inline 
VoxelType& mdk3DImageData<VoxelType>::operator[](int64 LinearIndex)
{ 
    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImageData<VoxelType>::operator[](int64 LinearIndex) const
{
    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
VoxelType& mdk3DImageData<VoxelType>::operator()(int64 LinearIndex)
{
    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImageData<VoxelType>::operator()(int64 LinearIndex) const
{
    return m_DataArray[LinearIndex];
}
   

template<typename VoxelType>
inline
VoxelType& mdk3DImageData<VoxelType>::operator()(int64 xIndex, int64 yIndex, int64 zIndex)
{
    auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageDimension[0] + xIndex;
 
    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImageData<VoxelType>::operator()(int64 xIndex, int64 yIndex, int64 zIndex) const
{
    auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageDimension[0] + xIndex;

    return m_DataArray[LinearIndex];
}


template<typename VoxelType>
inline
int64 mdk3DImageData<VoxelType>::GetLinearIndexBy3DIndex(int64 xIndex, int64 yIndex, int64 zIndex) const
{
    return zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageDimension[0] + xIndex;    
}


template<typename VoxelType>
inline 
void mdk3DImageData<VoxelType>::Get3DIndexByLinearIndex(int64 LinearIndex, int64* xIndex, int64* yIndex, int64* zIndex) const
{
    std::lldiv_t divresult;

    divresult = div(LinearIndex, m_VoxelNumberPerZSlice);

    zIndex[0] = divresult.quot;
            
    divresult = div(divresult.rem, m_ImageDimension[0]);

    yIndex[0] = divresult.quot;

    xIndex[0] = divresult.rem;
}


template<typename VoxelType>
inline 
void mdk3DImageData<VoxelType>::Get3DPositionByLinearIndex(int64 LinearIndex, double* x, double* y, double* z) const
{       
    std::lldiv_t divresult;

    divresult = div(LinearIndex, m_VoxelNumberPerZSlice);

    z[0] = divresult.quot;

    divresult = div(divresult.rem, m_ImageDimension[0]);

    y[0] = divresult.quot;
      
    x[0] = divresult.rem;

    x[0] = m_PhysicalOrigin[0] + x[0] * m_VoxelPhysicalSize[0];

    y[0] = m_PhysicalOrigin[0] + y[0] * m_VoxelPhysicalSize[1];

    z[0] = m_PhysicalOrigin[0] + z[0] * m_VoxelPhysicalSize[2];
}


template<typename VoxelType>
inline
void mdk3DImageData<VoxelType>::Get3DPositionBy3DIndex(int64 xIndex, int64 yIndex, int64 zIndex, double* x, double* y, double* z) const
{
    x[0] = m_PhysicalOrigin[0] + double(xIndex) * m_VoxelPhysicalSize[0];

    y[0] = m_PhysicalOrigin[0] + double(yIndex) * m_VoxelPhysicalSize[1];

    z[0] = m_PhysicalOrigin[0] + double(zIndex) * m_VoxelPhysicalSize[2];
}

//============================================================================================================================//
template<typename VoxelType>
mdk3DImage<VoxelType>::mdk3DImage()
{
	this->Reset();
}


template<typename VoxelType>
mdk3DImage<VoxelType>::~mdk3DImage()
{
}


template<typename VoxelType>
mdk3DImage<VoxelType>::mdk3DImage(mdk3DImage<VoxelType>&& InputImage)
{
    this->Reset();

    m_ImageData = std::move(InputImage.m_ImageData);

    if (m_ImageData)
    {
        m_VoxelPointer = m_ImageData->m_DataArray.data();
    }
    else
    {
        m_VoxelPointer = nullptr;
    }

    InputImage.Clear();
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::operator=(const mdk3DImage<VoxelType>& InputImage)
{
    this->Copy(InputImage);
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::operator=(mdk3DImage<VoxelType>&& InputImage)
{
    this->Take(InputImage);

    InputImage.clear();
}


template<typename VoxelType>
template<typename VoxelType_Input>
bool mdk3DImage<VoxelType>::DeepCopy(const mdk3DImage<VoxelType_Input>& InputImage)
{
    if (this == &InputImage)
    {
        mdkWarning << "try to DeepCopy self @ mdk3DImage::DeepCopy(InputImage)" << '\n';
        return true;
    }

	if (InputImage.IsEmpty() == true)
	{
        if (this->IsEmpty() == false)
        {
            mdkError << "Self is not emtpy but InputImage is empty @ mdk3DImage::DeepCopy(InputImage)" << '\n';
            return false;
        }
        else
        {
            return true;
        }
	}

    auto InputPtr = InputImage.GetVoxelPointer();

    auto InputDimension = InputImage.GetImageDimension();
    
    auto InputSpacing = InputImage.GetVoxelPhysicalSize();

    auto InputOrigin = InputImage.GetPhysicalOrigin();

    if (this->IsEmpty() == true)
    {
        this->Initialize(InputDimension.Lx, InputDimension.Ly, InputDimension.Lz,
                         InputOrigin.x, InputOrigin.y, InputOrigin.z,
                         InputSpacing.Sx, InputSpacing.Sy, InputSpacing.Sz);
    }

    return this->CopyData(InputPtr, InputDimension.Lx, InputDimension.Ly, InputDimension.Lz);

}


template<typename VoxelType>
template<typename VoxelType_Input>
bool mdk3DImage<VoxelType>::DeepCopy(const mdk3DImage<VoxelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        mdkError << "Input is nullptr @ mdk3DImage::DeepCopy(mdk3DImage* InputImage)" << '\n';
        return false;
    }

    return this->DeepCopy(*InputImage);
}


template<typename VoxelType>
template<typename VoxelType_Input>
bool mdk3DImage<VoxelType>::CopyData(const VoxelType_Input* InputVoxelPointer, int64 Lx, int64 Ly, int64 Lz = 1)
{
    if (this->IsEmpty == false)
	{
        mdkError << "Self is empty @ mdk3DImage::CopyData" << '\n';
		return false;
	}

    auto SelfDimension = this->GetImageDimension();

    if (SelfDimension.Lx != Lx || SelfDimension.Ly != Ly || SelfDimension.Lz != Lz)
    {
        mdkError << "Dimension does not match @ mdk3DImage::CopyData" << '\n';
        return false;
    }

    auto VoxelPtr = this->GetVoxelPointer();

    if (std::size_t(InputVoxelPointer) == std::size_t(VoxelPtr))
    {
        mdkWarning << "An image tries to Copy itself @ mdk3DImage::CopyData" << '\n';
        return true;
    }
    

	for (int64 i = 0; i < m_VoxelNumber; ++i)
	{
        VoxelPtr[i] = VoxelType(InputVoxelPointer[i]);
	}

    return true;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::Fill(const VoxelType& Voxel)
{
    if (this->IsEmpty() == true)
    {
        return false;
    }

    auto BeginPtr = this->GetVoxelPointer();

    auto ImageDimension = this->GetImageDimension();

    auto VoxelNumber = ImageDimension.Lx *ImageDimension.Ly *ImageDimension.Lz;

    for (auto Ptr = BeginPtr; Ptr < BeginPtr + VoxelNumber; ++Ptr)
    {
        Ptr[0] = Voxel;
    }

    return true;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::SharedCopy(mdk3DImage<VoxelType>& InputImage)
{
    if (this->IsEmpty() == false)
    {
        mdkError << "Self is not empty @ mdk3DImage::SharedCopy(const mdk3DImage& InputImage)" << '\n';
        return false;
    }

    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary

    if (m_ImageData)
    {
        m_VoxelPointer = m_ImageData->m_DataArray.data();
    }
    else
    {
        m_VoxelPointer = nullptr;
    }

    return true;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::SharedCopy(mdk3DImage<VoxelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        mdkError << "Input is nullptr @ mdk3DImage::SharedCopy(mdk3DImage* InputImage)" << '\n';
        return false;
    }

    return this->SharedCopy(*InputImage);
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::ForceSharedCopy(const mdk3DImage<VoxelType>& InputImage)
{
    m_ImageData = InputImage.m_ImageData; // std::Shared_ptr, self assignment test is not necessary

    if (m_ImageData)
    {
        m_VoxelPointer = m_ImageData->m_DataArray.data();
    }
    else
    {
        m_VoxelPointer = nullptr;
    }
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::ForceSharedCopy(const mdk3DImage<VoxelType>* InputImage)
{
    if (InputImage == nullptr)
    {
        mdkError << "Input is nullptr @ mdk3DImage::ForceSharedCopy(mdk3DImage* InputImage)" << '\n';
        return false;
    }

    return this->ForceSharedCopy(*InputImage);
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Take(mdk3DImage<VoxelType>&& InputImage)
{
    mdk3DImage<VoxelType>& tempImage = InputImage;

    this->Take(tempImage);
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Take(mdk3DImage<VoxelType>& InputImage)
{
    m_ImageData = std::move(InputImage.m_ImageData);

    if (m_ImageData)
    {
        m_VoxelPointer = m_ImageData->m_DataArray.data();
    }
    else
    {
        m_VoxelPointer = nullptr;
    }

    InputImage.clear();
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Reset()
{
    m_ImageData.reset(); // reset shared_ptr

    m_VoxelPointer = nullptr;

    m_ZeroVoxel = m_ZeroVoxel - m_ZeroVoxel;

    m_ZeroVoxel_Error_Output = m_ZeroVoxel;
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Clear()
{
    m_ImageData.reset(); // reset shared_ptr

    m_VoxelPointer = nullptr;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::Initialize(int64 Lx, int64 Ly, int64 Lz = 1,
                                       double PhysicalOrigin_x = 0.0,
                                       double PhysicalOrigin_y = 0.0,
                                       double PhysicalOrigin_z = 0.0,
                                       double VoxelPhysicalSize_x = 1.0,
                                       double VoxelPhysicalSize_y = 1.0,
                                       double VoxelPhysicalSize_z = 1.0)
{
    if (m_ImageData)
    {
        mdkError << "Can not Initialize: Image is not empty @ mdk3DImage::Initialize" << '\n';
        return false;
    }

    if (Lx <= 0 || Ly <= 0 || Lz <= 0)
    {
        mdkError << "Invalid input @ mdk3DImage::Initialize" << '\n';
        return false;
    }

    m_ImageData = std::make_shared<mdk3DImageData<VoxelType>>();

    m_ImageData->m_ImageDimension[0] = Lx;
    m_ImageData->m_ImageDimension[1] = Ly;
    m_ImageData->m_ImageDimension[2] = Lz;

    m_ImageData->m_VoxelNumberPerZSlice = Ly*Lx;

    m_ImageData->m_PhysicalOrigin[0] = PhysicalOrigin_x;
    m_ImageData->m_PhysicalOrigin[1] = PhysicalOrigin_y;
    m_ImageData->m_PhysicalOrigin[2] = PhysicalOrigin_z;

    m_ImageData->m_VoxelPhysicalSize[0] = VoxelPhysicalSize_x;
    m_ImageData->m_VoxelPhysicalSize[1] = VoxelPhysicalSize_y;
    m_ImageData->m_VoxelPhysicalSize[2] = VoxelPhysicalSize_z;

    m_ImageData->m_DataArray.resize(Lx*Ly*Lz);


    m_VoxelPointer = m_ImageData->m_DataArray.data();

    return true;
}


template<typename VoxelType>
inline
bool mdk3DImage<VoxelType>::IsEmpty() const
{
    return !(m_ImageData);
}


template<typename VoxelType>
inline
VoxelType* mdk3DImage<VoxelType>::GetVoxelPointer()
{
    if (m_ImageData)
    {
        return m_ImageData->m_DataArray.data();
    }

    return nullptr;
}


template<typename VoxelType>
inline
const VoxelType* mdk3DImage<VoxelType>::GetVoxelPointer() const
{
    if (m_ImageData)
    {
        return m_ImageData->m_DataArray.data();
    }

    return nullptr;
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::SetZeroVoxel(VoxelType ZeroVoxel)
{
    m_ZeroVoxel = ZeroVoxel;

    m_ZeroVoxel_Error_Output = ZeroVoxel;
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::GetZeroVoxel() const
{
    return m_ZeroVoxel;
}


template<typename VoxelType>
inline
mdk3DImageDimension mdk3DImage<VoxelType>::GetImageDimension() const
{
    mdk3DImageDimension Dimension;

    if (m_ImageData)
    {
        Dimension.Lx = m_ImageData->m_ImageDimension[0];
        Dimension.Ly = m_ImageData->m_ImageDimension[1];
        Dimension.Lz = m_ImageData->m_ImageDimension[2];
    }
    else
    {
        Dimension.Lx = 0;
        Dimension.Ly = 0;
        Dimension.Lz = 0;
    }

    return Dimension;
}


template<typename VoxelType>
inline 
mdk3DImagePhysicalSize mdk3DImage<VoxelType>::GetImagePhysicalSize() const
{
    mdk3DImagePhysicalSize Size;

    if (m_ImageData)
    {
        Size.Sx = m_ImageData->m_ImageDimension[0] * m_ImageData->m_VoxelPhysicalSize[0];

        Size.Sy = m_ImageData->m_ImageDimension[1] * m_ImageData->m_VoxelPhysicalSize[1];

        Size.Sz = m_ImageData->m_ImageDimension[2] * m_ImageData->m_VoxelPhysicalSize[2];
    }
    else
    {
        Size.Sx = 0;
        Size.Sy = 0;
        Size.Sz = 0;
    }

    return Size;
}


template<typename VoxelType>
inline
mdk3DImageVoxelPhysicalSize mdk3DImage<VoxelType>::GetVoxelPhysicalSize() const
{
    mdk3DImageVoxelPhysicalSize Size;

    if (m_ImageData)
    {
        Size.Sx = m_ImageData->m_VoxelPhysicalSize[0];
        Size.Sy = m_ImageData->m_VoxelPhysicalSize[1];
        Size.Sz = m_ImageData->m_VoxelPhysicalSize[2];
    }
    else
    {
        Size.Sx = 0;
        Size.Sy = 0;
        Size.Sz = 0;
    }

    return Size;
}


template<typename VoxelType>
inline
mdk3DImagePhysicalOrigin mdk3DImage<VoxelType>::GetPhysicalOrigin() const
{
    mdk3DImagePhysicalOrigin Origin;

    if (m_ImageData)
    {
        Origin.x = m_ImageData->m_PhysicalOrigin[0];
        Origin.y = m_ImageData->m_PhysicalOrigin[1];
        Origin.z = m_ImageData->m_PhysicalOrigin[2];
    }
    else
    {
        Origin.x = 0;
        Origin.y = 0;
        Origin.z = 0;
    }

    return Origin;
}


template<typename VoxelType>
inline
int64 mdk3DImage<VoxelType>::GetVoxelNumber() const
{
    if (m_ImageData)
    {
        return m_ImageData->m_VoxelNumberPerZSlice * m_ImageData->m_ImageDimension[0]
    }
    else
    {
        return 0;
    }
}
 

template<typename VoxelType>
inline
int64 mdk3DImage<VoxelType>::GetLinearIndexBy3DIndex(int64 xIndex, int64 yIndex, int64 zIndex = 0) const
{
    if (m_ImageData)
    {
        return m_ImageData->GetLinearIndexBy3DIndex(xIndex, yIndex, zIndex);
    }
    else
    {
        mdkError << "Image is empty @ mdk3DImage::GetLinearIndexBy3DIndex()" << '\n';
        return 0;
    }
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::Get3DIndexByLinearIndex(int64 LinearIndex, int64* xIndex, int64* yIndex, int64* zIndex) const
{
    if (m_ImageData)
    {
        m_ImageData->Get3DIndexByLinearIndex(LinearIndex, xIndex, yIndex, zIndex);
    }
    else
    {
        mdkError << "Image is empty @ mdk3DImage::GetLinearIndexBy3DIndex()" << '\n';

        xIndex[0] = 0;
        yIndex[0] = 0;
        zIndex[0] = 0;
    }
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::Get3DPositionByLinearIndex(int64 LinearIndex, double* x, double* y, double* z) const
{
    if (m_ImageData)
    {
        m_ImageData->Get3DPositionByLinearIndex(LinearIndex, x, y, z);
    }
    else
    {
        mdkError << "Image is empty @ mdk3DImage::Get3DPositionByLinearIndex()" << '\n';

        x[0] = 0;
        y[0] = 0;
        z[0] = 0;
    }
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::Get3DPositionBy3DIndex(int64 xIndex, int64 yIndex, int64 zIndex, double* x, double* y, double* z = nullptr) const
{
    if (m_ImageData)
    {
        m_ImageData->Get3DPositionBy3DIndex(xIndex, yIndex, zIndex, x, y, z);
    }
    else
    {
        mdkError << "Image is empty @ mdk3DImage::Get3DPositionBy3DIndex()" << '\n';

        x[0] = 0;
        y[0] = 0;
        z[0] = 0;
    }
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::operator[](int64 LinearIndex)
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

    auto VoxelNumber = this->GetVoxelNumber();

    if (LinearIndex >= VoxelNumber || LinearIndex < 0)
    {
        mdkError << "Invalid input @ mkd3DImage::operator(LinearIndex)" << '\n';
        m_ZeroVoxel_Error_Output = m_ZeroVoxel;
        return m_ZeroVoxel_Error_Output;
    }

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::operator[](int64 LinearIndex) const
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

    auto VoxelNumber = this->GetVoxelNumber();

    if (LinearIndex >= VoxelNumber || LinearIndex < 0)
    {
        mdkError << "Invalid input @ mkd3DImage::operator(LinearIndex)" << '\n';
        return m_ZeroVoxel;
    }

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::operator()(int64 LinearIndex)
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

    auto VoxelNumber = this->GetVoxelNumber();

	if (LinearIndex >= VoxelNumber || LinearIndex < 0)
	{
		mdkError << "Invalid input @ mkd3DImage::operator(LinearIndex)" << '\n';
		m_ZeroVoxel_Error_Output = m_ZeroVoxel;
		return m_ZeroVoxel_Error_Output;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::operator()(int64 LinearIndex) const
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

    auto VoxelNumber = this->GetVoxelNumber();

	if (LinearIndex >= VoxelNumber || LinearIndex < 0)
	{
		mdkError << "Invalid input @ mkd3DImage::operator(LinearIndex) const" << '\n';
		return m_ZeroVoxel;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::at(int64 LinearIndex)
{
    auto VoxelNumber = this->GetVoxelNumber();

    if (LinearIndex >= VoxelNumber || LinearIndex < 0)
    {
        mdkError << "Invalid input @ mkd3DImage::at(LinearIndex)" << '\n';
        m_ZeroVoxel_Error_Output = m_ZeroVoxel;
        return m_ZeroVoxel_Error_Output;
    }

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::at(int64 LinearIndex) const
{
    auto VoxelNumber = this->GetVoxelNumber();

    if (LinearIndex >= VoxelNumber || LinearIndex < 0)
    {
        mdkError << "Invalid input @ mkd3DImage::at(LinearIndex)" << '\n';
        return m_ZeroVoxel;
    }

    return m_VoxelPointer[LinearIndex];
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::operator()(int64 xIndex, int64 yIndex, int64 zIndex = 0)
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

    auto ImageDimension = this->GetImageDimension();

	if (xIndex >= ImageDimension.Lx || xIndex < 0 || yIndex >= ImageDimension.Ly || yIndex < 0 || zIndex >= ImageDimension.Lz || zIndex < 0)
	{
		mdkError << "Invalid input @ mkd3DImage::operator(xIndex, yIndex, zIndex)" << '\n';
		m_ZeroVoxel_Error_Output = m_ZeroVoxel;
		return m_ZeroVoxel_Error_Output;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound
	
    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::operator()(int64 xIndex, int64 yIndex, int64 zIndex = 0) const
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

    auto ImageDimension = this->GetImageDimension();

	if (xIndex >= ImageDimension.Lx || xIndex < 0 || yIndex >= ImageDimension.Ly || yIndex < 0 || zIndex >= ImageDimension.Lz || zIndex < 0)
	{
        mdkError << "Invalid input @ mkd3DImage::operator(xIndex, yIndex, zIndex) const" << '\n';
        return m_ZeroVoxel;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::at(int64 xIndex, int64 yIndex, int64 zIndex = 0)
{
    auto ImageDimension = this->GetImageDimension();

	if (xIndex >= ImageDimension[0] || xIndex < 0 || yIndex >= ImageDimension[1] || yIndex < 0 || zIndex >= ImageDimension[2] || zIndex < 0)
	{
		mdkError << "Invalid input @ mkd3DImage::at(xIndex, yIndex, zIndex)" << '\n';
		m_ZeroVoxel_Error_Output = m_ZeroVoxel;
		return m_ZeroVoxel_Error_Output;
	}

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::at(int64 xIndex, int64 yIndex, int64 zIndex = 0) const
{
    auto ImageDimension = this->GetImageDimension();

    if (xIndex >= ImageDimension[0] || xIndex < 0 || yIndex >= ImageDimension[1] || yIndex < 0 || zIndex >= ImageDimension[2] || zIndex < 0)
    {
        mdkError << "Invalid input @ mkd3DImage::at(xIndex, yIndex, zIndex) const" << '\n';
        m_ZeroVoxel_Error_Output = m_ZeroVoxel;
        return m_ZeroVoxel_Error_Output;
    }

    return (*m_ImageData)(xIndex, yIndex, zIndex);
}


template<typename VoxelType>
mdk3DImage<VoxelType> mdk3DImage<VoxelType>::GetSubImage(int64 xIndex_s, int64 xIndex_e, int64 yIndex_s, int64 yIndex_e, int64 zIndex_s = 0, int64 zIndex_e = 0) const
{
    mdk3DImage<VoxelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        mdkWarning << "Image is empty @ mdk3DImage::GetSubImage()" << '\n';
        return tempImage;
    }

    auto ImageDimension = this->GetImageDimension();

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
        mdkError << "Invalid input @ mkd3DImage::GetSubImage() const" << '\n';
		return tempImage;
	}

	Lx = xIndex_e - xIndex_s + 1;
	Ly = yIndex_e - yIndex_s + 1;
	Lz = zIndex_e - zIndex_s + 1;

	tempImage.Initialize(Lx, Ly, Lz,
  		                 m_PhysicalOrigin[0],    m_PhysicalOrigin[1],    m_PhysicalOrigin[2],
		                 m_VoxelPhysicalSize[0], m_VoxelPhysicalSize[1], m_VoxelPhysicalSize[2]);

	tempRawPtr = SubImage.GetVoxelPointer();

    RawPtr = this->GetVoxelPointer();

    VoxelNumberPerZSlice = m_ImageData->VoxelNumberPerZSlice;

	int64 Index_k = 0;

	int64 Index_j = 0;

	Index_k = zIndex_s;

	for (int64 k = zIndex_s; k <= zIndex_e; ++k)
	{
		Index_j = yIndex_s;

		for (int64 j = yIndex_s; j <= yIndex_e; ++j)
		{
			for (int64 i = xIndex_s; i <= xIndex_e; ++i)
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
mdk3DImage<VoxelType> 
mdk3DImage<VoxelType>::
Pad(const std::string& Option, int64 Pad_Lx, int64 Pad_Ly, int64 Pad_Lz = 0) const
{
    mdk3DImage<VoxelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        mdkWarning << "Image is empty @ mdk3DImage::Pad" << '\n';
        return tempImage;
    }

    if (Option != "replicate")
	{
		mdkError << "Invalid Option @ mdk3DImage::Pad" << '\n';
		return tempImage;
	}

    if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
	{
		mdkWarning << "Invalid Pad Size @ mdk3DImage::Pad" << '\n';

		tempImage = (*this);

		return tempImage;
	}

    auto ImageDimension = this->GetImageDimension();

    auto PhysicalOrigin = this->GetPhysicalOrigin();

    auto VoxelPhysicalSize = this->GetVoxelPhysicalSize();

	auto Lx = ImageDimension[0] + Pad_Lx;
    auto Ly = ImageDimension[1] + Pad_Ly;
    auto Lz = ImageDimension[2] + Pad_Lz;

	tempImage.Initialize(Lx, Ly, Lz,
                         PhysicalOrigin[0], PhysicalOrigin[1], PhysicalOrigin[2],
                         VoxelPhysicalSize[0], VoxelPhysicalSize[1], VoxelPhysicalSize[2]);


	if (OptionStr == "zero")
	{
        tempImage.Fill(VoxelType(0));

		for (int64 k = 0; k <= Lz; ++k)
		{
			for (int64 j = 0; j <= Ly; ++j)
			{
				for (int64 i = 0; i <= Lx; ++i)
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
		for (int64 temp_k = 0; temp_k <= Lz; ++temp_k)
		{
			for (int64 temp_j = 0; temp_j <= Ly; ++temp_j)
			{
				for (int64 temp_i = 0; temp_i <= Lx; ++temp_i)
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
mdk3DImage<VoxelType>
mdk3DImage<VoxelType>::
Pad(VoxelType Voxel, int64 Pad_Lx, int64 Pad_Ly, int64 Pad_Lz = 0) const
{
    mdk3DImage<VoxelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        mdkWarning << "Image is empty @ mdk3DImage::Pad" << '\n';
        return tempImage;
    }

	if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
	{
		mdkWarning << "Invalid Pad Size @ mdk3DImage::Pad" << '\n';

		tempImage = (*this);

		return tempImage;
	}

    auto ImageDimension = this->GetImageDimension();

    auto PhysicalOrigin = this->GetPhysicalOrigin();

    auto VoxelPhysicalSize = this->GetVoxelPhysicalSize();

	auto Lx = ImageDimension[0] + Pad_Lx;
    auto Ly = ImageDimension[1] + Pad_Ly;
    auto Lz = ImageDimension[2] + Pad_Lz;

	tempImage.Initialize(Lx, Ly, Lz,
                         PhysicalOrigin[0], PhysicalOrigin[1], PhysicalOrigin[2],
                         VoxelPhysicalSize[0], VoxelPhysicalSize[1], VoxelPhysicalSize[2]);

    tempImage.Fill(Voxel);

    for (int64 k = 0; k <= Lz; ++k)
    {
        for (int64 j = 0; j <= Ly; ++j)
        {
            for (int64 i = 0; i <= Lx; ++i)
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
mdk3DImage<VoxelType> 
mdk3DImage<VoxelType>::
UnPad(int64 Pad_Lx, int64 Pad_Ly, int64 Pad_Lz = 0) const
{
    mdk3DImage<VoxelType> tempImage; // empty image
   
    auto ImageDimension = this->GetImageDimension();

    if (Pad_Lx > ImageDimension[0] || Pad_Lx  < 0 || Pad_Ly > ImageDimension[1] || Pad_Ly < 0 || Pad_Lz > ImageDimension[2] || Pad_Lz < 0)
	{
		mdkError << "Invalid Pad Size @ mdk3DImage::UnPad" << '\n';
		return tempImage;
	}

	if (Pad_Lx == ImageDimension[0] || Pad_Ly == ImageDimension[1] || Pad_Lz == ImageDimension[2])
	{
		mdkWarning << "Output is empty @ mdk3DImage::UnPad" << '\n';
		return tempImage;
	}

	if (Pad_Lx == 0 && Pad_Ly == 0 && Pad_Lz == 0)
	{
		mdkWarning << "Input Pad Size is [0, 0, 0] @ mdk3DImage::UnPad" << '\n';

		tempImage = (*this);

		return tempImage;
	}

	return this->GetSubImage(Pad_Lx, ImageDimension[0] - 1 - Pad_Lx,
		                     Pad_Ly, ImageDimension[1] - 1 - Pad_Ly,
		                     Pad_Lz, ImageDimension[2] - 1 - Pad_Lz);
                             

}


template<typename VoxelType>
mdkMatrix<int64>
mdk3DImage<VoxelType>::
GetLinearIndexListOfRegion(int64 xIndex_s,     int64 Region_Lx,
                            int64 yIndex_s,     int64 Region_Ly,
                            int64 zIndex_s = 0, int64 Region_Lz = 0) const
{
    mdkMatrix<int64>  List;
    
    auto ImageDimension = this->GetImageDimension();

    if (   xIndex_s >= ImageDimension[0] || xIndex_s < 0
        || yIndex_s >= ImageDimension[1] || yIndex_s < 0
        || zIndex_s >= ImageDimension[2] || zIndex_s < 0
        || Region_Lx > ImageDimension[0] - xIndex_s || Region_Lx < 0
        || Region_Ly > ImageDimension[1] - yIndex_s || Region_Ly < 0
        || Region_Lz > ImageDimension[2] - zIndex_s || Region_Lz < 0)
	{
		mdkError << "Invalid input @ mdk3DImage::GetLinearIndexArrayOfRegion" << '\n';
		return List;
	}

	if (Region_Lx == 0 || Region_Ly == 0 || Region_Lz == 0)
	{
		mdkWarning << "Empty input region @ mdk3DImage::GetLinearIndexArrayOfRegion" << '\n';
		return List;
	}

    List.Resize(Region_Lx*Region_Ly*Region_Lz, 1);

    auto VoxelNumberPerZSlice = m_ImageData->VoxelNumberPerZSlice;

	int64 Counter = 0;

	for (int64 k = zIndex_s; k < zIndex_s + Region_Lz; ++k)
	{
		for (int64 j = yIndex_s; j < yIndex_s + Region_Ly; ++j)
		{
			for (int64 i = xIndex_s; i < xIndex_s + Region_Lx; ++i)
			{
				List[Counter] = k*VoxelNumberPerZSlice + j*ImageDimension[0] + i;

			    Counter += 1;
			}
		}
	}
    
	return List;
}


template<typename VoxelType>
inline
VoxelType mdk3DImage<VoxelType>::InterpolateAt3DPosition(double x, double y, double z,
                                                          mdk3DImageInterpolationMethodEnum Method = mdk3DImageInterpolationMethodEnum::NearestNeighbor) const
{
    return VoxelType(0);
}


}//end namespace mdk

#endif