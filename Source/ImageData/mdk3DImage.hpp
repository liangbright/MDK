#ifndef __mdk3DImage_hpp
#define __mdk3DImage_hpp

#include <cstdlib>

#include "mdk3DImage.h"
#include "mdk3DImageVoxel.h"

namespace mdk
{

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
mdk3DImage<VoxelType>::mdk3DImage(const mdk3DImage<VoxelType>& targetImage)
{
    this->Reset();

	(*this) = targetImage;
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::operator=(const mdk3DImage<VoxelType>& targetImage)
{
	// Image = Image
	if (this == &targetImage)
	{
		return;
	}

    if (targetImage.IsTemporary() == true)
	{
        m_VoxelData = targetImage.GetVoxelDataSharedPointer();

		targetImage.GetImageDimension(&m_ImageDimension[0], &m_ImageDimension[1], &m_ImageDimension[2]);

		m_VoxelNumber = m_ImageDimension[0] * m_ImageDimension[1] * m_ImageDimension[2];

		m_VoxelNumberPerZSlice = m_ImageDimension[0] * m_ImageDimension[1];

		targetImage.GetVoxelPhysicalSize(&m_VoxelPhysicalSize[0], &m_VoxelPhysicalSize[1], &m_VoxelPhysicalSize[2]);

		targetImage.GetPhysicalOrigin(&m_PhysicalOrigin[0], &m_PhysicalOrigin[1], &m_PhysicalOrigin[2]);
	}
	else
	{
		this->Copy(targetImage);
	}
}


template<typename VoxelType>
inline 
void  mdk3DImage<VoxelType>::SetTobeTemporary()
{
    m_IsTemporary = true;
}


template<typename VoxelType>
inline 
bool mdk3DImage<VoxelType>::IsTemporary() const
{
    return m_IsTemporary;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::Initialize(uint64 Lx, uint64 Ly, uint64 Lz = 1,
	                                   double PhysicalOrigin_x = 0.0,    
									   double PhysicalOrigin_y = 0.0, 
									   double PhysicalOrigin_z = 0.0,
	                                   double VoxelPhysicalSize_x = 1.0,
									   double VoxelPhysicalSize_y = 1.0, 
									   double VoxelPhysicalSize_z = 1.0)
{
	if (m_VoxelNumber > 0)
	{
		mdkWarning << "re-call Initialize @ mdk3DImage::Initialize" << '\n';
		this->Reset();
	}

	m_ImageDimension[0] = Lx;
	m_ImageDimension[1] = Ly;
	m_ImageDimension[2] = Lz;

	m_VoxelNumber = Lx*Ly*Lz;

	m_VoxelNumberPerZSlice = Ly*Lx;

	m_PhysicalOrigin[0] = PhysicalOrigin_x;
	m_PhysicalOrigin[1] = PhysicalOrigin_y;
	m_PhysicalOrigin[2] = PhysicalOrigin_z;

	m_VoxelPhysicalSize[0] = VoxelPhysicalSize_x;
	m_VoxelPhysicalSize[1] = VoxelPhysicalSize_y;
	m_VoxelPhysicalSize[2] = VoxelPhysicalSize_z;

	m_VoxelData->resize(m_VoxelNumber);

	return true;
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Reset()
{
    m_VoxelData = std::make_shared<std::vector<VoxelType>>();

	m_ImageDimension[0] = 0;
	m_ImageDimension[1] = 0;
	m_ImageDimension[2] = 0;

	m_PhysicalOrigin[0] = 0;
	m_PhysicalOrigin[1] = 0;
	m_PhysicalOrigin[2] = 0;

	m_VoxelPhysicalSize[0] = 0;
	m_VoxelPhysicalSize[1] = 0;
	m_VoxelPhysicalSize[2] = 0;

	m_VoxelNumber = 0;

	m_VoxelNumberPerZSlice = 0;

    m_EmptyVoxel = m_EmptyVoxel - m_EmptyVoxel;

    m_EmptyVoxel_temp = m_EmptyVoxel;

	m_IsTemporary = false;
}


template<typename VoxelType>
inline
bool mdk3DImage<VoxelType>::IsEmpty() const
{
	if (m_VoxelNumber == 0)
	{
		return true;
	}

	return false;
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Copy(const mdk3DImage<VoxelType>& targetImage)
{
	if (targetImage.IsEmpty() == true)
	{
		mdkWarning << "targetImage is empty @ mdk3DImage::Copy" << '\n';
        this->Reset();
		return;
	}

	auto targetSize = targetImage.GetImageDimension();

	auto targetRawPtr = GetVoxelDataRawPointer();

	this->Copy(targetRawPtr, targetSize.Lx, targetSize.Ly, targetSize.Lz);

	targetImage.GetVoxelPhysicalSize(&m_VoxelPhysicalSize[0], &m_VoxelPhysicalSize[1], &m_VoxelPhysicalSize[2]);

	targetImage.GetPhysicalOrigin(&m_PhysicalOrigin[0], &m_PhysicalOrigin[1], &m_PhysicalOrigin[2]);
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Copy(const VoxelType* VoxelPointer, uint64 Lx, uint64 Ly, uint64 Lz = 1)
{
	if (VoxelPointer == nullptr)
	{
		return;
	}

	this->Reset();

	m_ImageDimension[0] = Lx;
	m_ImageDimension[1] = Ly;
	m_ImageDimension[2] = Lz;

	m_VoxelNumber = Lx*Ly*Lz;

	m_VoxelNumberPerZSlice = Ly*Lx;

	m_VoxelData->resize(m_VoxelNumber);

	auto RawPtr = m_VoxelData->data();

	for (uint64 i = 0; i < m_VoxelNumber; ++i)
	{
		RawPtr[i] = VoxelPointer[i];
	}
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Swap(mdk3DImage<VoxelType>& targetImage)
{
    auto ImageSize = this->GetImageDimension();

    auto PhysicalOrigin = this->GetPhysicalOrigin();

    auto VoxelPhysicalSize = this->GetVoxelPhysicalSize();

    auto tarGetImageDimension = targetImage->GetImageDimension();

    auto targetPhysicalOrigin = targetImage->GetPhysicalOrigin();

    auto targetVoxelPhysicalSize = targetImage->GetVoxelPhysicalSize();

    //

    m_VoxelData.swap(targetImage.GetVoxelDataSharedPointer());

    //

    this->Reshape(tarGetImageDimension.Lx, tarGetImageDimension.Ly, tarGetImageDimension.Lz);

    this->SetPhysicalOrigin(targetPhysicalOrigin.x, targetPhysicalOrigin.y, targetPhysicalOrigin.z);

    this->SetVoxelPhysicalSize(targetVoxelPhysicalSize.Vx, targetVoxelPhysicalSize.Vy, targetVoxelPhysicalSize.Vz);

    //

    targetImage->Reshape(ImageSize.Lx, ImageSize.Ly, ImageSize.Lz);

    targetImage->SetPhysicalOrigin(PhysicalOrigin.x, PhysicalOrigin.y, PhysicalOrigin.z);

    targetImage->SetVoxelPhysicalSize(VoxelPhysicalSize.Vx, VoxelPhysicalSize.Vy, VoxelPhysicalSize.Vz);
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::Reshape(uint64 Lx, uint64 Ly, uint64 Lz = 1)
{
    if (Lx*Ly*Lz != uint64(m_VoxelData->size()))
    {
        return false;
    }

    m_ImageDimension[0] = Lx;
    m_ImageDimension[1] = Ly;
    m_ImageDimension[2] = Lz;

    m_VoxelNumber = Lx*Ly*Lz;

    m_VoxelNumberPerZSlice = Lx*Ly;

    return true;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::SetImageDimension(uint64 Lx, uint64 Ly, uint64 Lz = 1)
{
    if (m_VoxelNumber > 0)
    {
        mdkError << "This is not an empty image, call Clear and then change size @ mdk3DImage::SetImageDimension" << '\n';
        return false;
    }

    m_ImageDimension[0] = Lx;
    m_ImageDimension[1] = Ly;
    m_ImageDimension[2] = Lz;

    m_VoxelNumber = Lx*Ly*Lz;

    m_VoxelNumberPerZSlice = Ly*Lx;

    m_VoxelData->resize(m_VoxelNumber);

    return true;
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::SetPhysicalOrigin(double PhysicalOrigin_x, double PhysicalOrigin_y, double PhysicalOrigin_z = 0.0)
{
	m_PhysicalOrigin[0] = PhysicalOrigin_x;
	m_PhysicalOrigin[1] = PhysicalOrigin_y;
	m_PhysicalOrigin[2] = PhysicalOrigin_z;
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::SetVoxelPhysicalSize(double VoxelPhysicalSize_x, double VoxelPhysicalSize_y, double VoxelPhysicalSize_z = 1.0)
{
	m_VoxelPhysicalSize[0] = VoxelPhysicalSize_x;
	m_VoxelPhysicalSize[1] = VoxelPhysicalSize_y;
	m_VoxelPhysicalSize[2] = VoxelPhysicalSize_z;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::Fill(const VoxelType& Voxel)
{
	if (m_VoxelNumber == 0)
	{
		return false;
	}

	auto RawPtr = m_VoxelData->data();

	for (uint64 i = 0; i < m_VoxelNumber; ++i)
	{
		RawPtr[i] = Voxel;
	}

	return true;
}


// specialize a member function of a class template (use inline to prevent error LNK2005)
//
// note: if Clear is a virtual function in mdkObject
// then, this specialized function must co-exist with Clear
// and  m_EmptyVoxel -= m_EmptyVoxel may cause compiler error 
// when VoxelType is std::vector becuase -= is not defined for std::vector 
//
bool mdk3DImage<std::vector<double>>::Fill(const std::vector<double>& Voxel)
{
	if (m_VoxelNumber == 0)
	{
		return false;
	}

	auto RawPtr = m_VoxelData->data();

	for (uint64 i = 0; i < m_VoxelNumber; ++i)
	{
        RawPtr[i] = Voxel;
	}
 
	return true;
}


template<typename VoxelType>
inline
std::vector<VoxelType>* mdk3DImage<VoxelType>::GetVoxelDataArrayPointer()
{
	return m_VoxelData.get();
}


template<typename VoxelType>
inline
VoxelType* mdk3DImage<VoxelType>::GetVoxelDataRawPointer()
{
    return m_VoxelData->data();
}


template<typename VoxelType>
inline
const VoxelType* mdk3DImage<VoxelType>::GetVoxelDataRawPointer() const
{
    return m_VoxelData->data();
}


template<typename VoxelType>
inline
std::shared_ptr<std::vector<VoxelType>>& mdk3DImage<VoxelType>::GetVoxelDataSharedPointer()
{
    return m_VoxelData;
}


template<typename VoxelType>
inline
const std::shared_ptr<std::vector<VoxelType>>& mdk3DImage<VoxelType>::GetVoxelDataSharedPointer() const
{
	return m_VoxelData;
}


template<typename VoxelType>
inline 
void mdk3DImage<VoxelType>::SetEmptyVoxel(VoxelType EmptyVoxel)
{
	m_EmptyVoxel = EmptyVoxel;

	m_EmptyVoxel_temp = EmptyVoxel;
}


template<typename VoxelType>
inline 
const VoxelType& mdk3DImage<VoxelType>::GetEmptyVoxel() const
{
	return m_EmptyVoxel;
}


template<typename VoxelType>
inline
mdk3DImageDimension mdk3DImage<VoxelType>::GetImageDimension() const
{
    mdk3DImageDimension Dimension;

    Dimension.Lx = m_ImageDimension[0];
    Dimension.Ly = m_ImageDimension[1];
    Dimension.Lz = m_ImageDimension[2];

    return Dimension;
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetImageDimension(uint64* Lx, uint64* Ly, uint64* Lz = nullptr) const
{
	Lx[0] = m_ImageDimension[0];
	Ly[0] = m_ImageDimension[1];

	if (Lz != nullptr)
	{
		Lz[0] = m_ImageDimension[2];
	}
}


template<typename VoxelType>
inline 
mdk3DImagePhysicalSize mdk3DImage<VoxelType>::GetImagePhysicalSize() const
{
    mdk3DImagePhysicalSize Size;

    Size.Sx = m_ImageDimension[0] * m_VoxelPhysicalSize[0];

    Size.Sy = m_ImageDimension[1] * m_VoxelPhysicalSize[1];

    Size.Sz = m_ImageDimension[2] * m_VoxelPhysicalSize[2];

    return Size;
}


template<typename VoxelType>
inline 
void mdk3DImage<VoxelType>::GetImagePhysicalSize(double* PhysicalSize_x, double* PhysicalSize_y, double* PhysicalSize_z = nullptr) const
{
    PhysicalSize_x[0] = m_ImageDimension[0] * m_VoxelPhysicalSize[0];

    PhysicalSize_y[0] = m_ImageDimension[1] * m_VoxelPhysicalSize[1];

    if (PhysicalSize_z != nullptr)
    {
        PhysicalSize_z[0] = m_ImageDimension[2] * m_VoxelPhysicalSize[2];
    }
}


template<typename VoxelType>
inline
mdk3DImageVoxelPhysicalSize mdk3DImage<VoxelType>::GetVoxelPhysicalSize() const
{
    mdk3DImageVoxelPhysicalSize Size;

    Size.Vx = m_VoxelPhysicalSize[0];
    Size.Vy = m_VoxelPhysicalSize[1];
    Size.Vz = m_VoxelPhysicalSize[2];

    return Size;
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetVoxelPhysicalSize(double* VoxelPhysicalSize_x, double* VoxelPhysicalSize_y, double* VoxelPhysicalSize_z = nullptr) const
{
	VoxelPhysicalSize_x[0] = m_VoxelPhysicalSize[0];
	VoxelPhysicalSize_y[0] = m_VoxelPhysicalSize[1];

	if (VoxelPhysicalSize_z != nullptr)
	{
		VoxelPhysicalSize_z[0] = m_VoxelPhysicalSize[2];
	}
}


template<typename VoxelType>
inline
mdk3DImagePhysicalOrigin mdk3DImage<VoxelType>::GetPhysicalOrigin() const
{
    mdk3DImagePhysicalOrigin Origin;

    Origin.x = m_PhysicalOrigin[0];
    Origin.y = m_PhysicalOrigin[1];
    Origin.z = m_PhysicalOrigin[2];

    return Origin;
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetPhysicalOrigin(double* PhysicalOrigin_x, double* PhysicalOrigin_y, double* PhysicalOrigin_z = nullptr) const
{
	PhysicalOrigin_x[0] = m_PhysicalOrigin[0];
	PhysicalOrigin_y[0] = m_PhysicalOrigin[1];

	if (PhysicalOrigin_z != nullptr)
	{
		PhysicalOrigin_z[0] = m_PhysicalOrigin[2];
	}
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetLinearIndexBy3DIndex(uint64* LinearIndex, uint64 xIndex, uint64 yIndex, uint64 zIndex = 0) const
{
	LinearIndex[0] = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageDimension[0] + xIndex;
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::Get3DIndexByLinearIndex(uint64 LinearIndex, uint64* xIndex, uint64* yIndex, uint64* zIndex = nullptr) const
{
	std::lldiv_t divresult;
	divresult.rem = LinearIndex;

	if (zIndex != nullptr)
	{
		divresult = div(divresult.rem, m_VoxelNumberPerZSlice);
		zIndex[0] = divresult.quot;
	}

	divresult = div(divresult.rem, m_ImageDimension[0]);

	yIndex[0] = divresult.quot;

	xIndex[0] = divresult.rem;
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::Get3DPositionByLinearIndex(uint64 LinearIndex, double* x, double* y, double* z = nullptr) const
{
    std::lldiv_t divresult;
    divresult.rem = LinearIndex;

    if (z != nullptr)
    {
        divresult = div(divresult.rem, m_VoxelNumberPerZSlice);
        z[0] = divresult.quot;

        z[0] = m_PhysicalOrigin[0] + z[0] * m_VoxelPhysicalSize[2];
    }

    divresult = div(divresult.rem, m_ImageDimension[0]);

    y[0] = divresult.quot;

    y[0] = m_PhysicalOrigin[0] + y[0] * m_VoxelPhysicalSize[1];

    x[0] = divresult.rem;

    x[0] = m_PhysicalOrigin[0] + x[0] * m_VoxelPhysicalSize[0];    
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::Get3DPositionBy3DIndex(uint64 xIndex, uint64 yIndex, uint64 zIndex, double* x, double* y, double* z = nullptr) const
{
    x[0] = m_PhysicalOrigin[0] + double(xIndex) * m_VoxelPhysicalSize[0];

    y[0] = m_PhysicalOrigin[0] + double(yIndex) * m_VoxelPhysicalSize[1];

    if (z != nullptr)
    {
        z[0] = m_PhysicalOrigin[0] + double(zIndex) * m_VoxelPhysicalSize[2];
    }
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::operator[](uint64 LinearIndex)
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

    if (LinearIndex >= m_VoxelNumber)
    {
        mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::operator(LinearIndex)" << '\n';
        m_EmptyVoxel_temp = m_EmptyVoxel;
        return m_EmptyVoxel_temp;
    }

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::operator[](uint64 LinearIndex) const
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

    if (LinearIndex >= m_VoxelNumber)
    {
        mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::operator(LinearIndex)" << '\n';
        return m_EmptyVoxel;
    }

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

    return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::operator()(uint64 LinearIndex)
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

	if (LinearIndex >= m_VoxelNumber)
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::operator(LinearIndex)" << '\n';
		m_EmptyVoxel_temp = m_EmptyVoxel;
		return m_EmptyVoxel_temp;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::operator()(uint64 LinearIndex) const
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

	if (LinearIndex >= m_VoxelNumber)
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::operator(LinearIndex) const" << '\n';
		return m_EmptyVoxel;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0)
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

	if (xIndex >= m_ImageDimension[0] || yIndex >= m_ImageDimension[1] || zIndex >= m_ImageDimension[2])
	{
		mdkError << "xIndex >= m_ImageDimension[0] || yIndex >= m_ImageDimension[1] || zIndex >= m_ImageDimension[2] @ mkd3DImage::operator(xIndex, yIndex, zIndex)" << '\n';
		m_EmptyVoxel_temp = m_EmptyVoxel;
		return m_EmptyVoxel_temp;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

	auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageDimension[0] + xIndex;
	
	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0) const
{
#if defined(MDK_DEBUG_3DImage_Operator_CheckBound)

	if (xIndex >= m_ImageDimension[0] || yIndex >= m_ImageDimension[1] || zIndex >= m_ImageDimension[2])
	{
        mdkError << "xIndex >= m_ImageDimension[0] || yIndex >= m_ImageDimension[1] || zIndex >= m_ImageDimension[2] @ mkd3DImage::operator(xIndex, yIndex, zIndex)" << '\n';
        return m_EmptyVoxel;
	}

#endif //MDK_DEBUG_3DImage_Operator_CheckBound

	auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageDimension[0] + xIndex;

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::at(uint64 LinearIndex)
{
	if (LinearIndex >= m_VoxelNumber)
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::at(LinearIndex)" << '\n';
		m_EmptyVoxel_temp = m_EmptyVoxel;
		return m_EmptyVoxel_temp;
	}

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::at(uint64 LinearIndex) const
{
	if (LinearIndex >= m_VoxelNumber)
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::at(LinearIndex)" << '\n';
		return m_EmptyVoxel;
	}

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::at(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0)
{
	if (xIndex >= m_ImageDimension[0] || yIndex >= m_ImageDimension[1] || zIndex >= m_ImageDimension[2])
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::at(xIndex, yIndex, zIndex)" << '\n';
		m_EmptyVoxel_temp = m_EmptyVoxel;
		return m_EmptyVoxel_temp;
	}

	auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageDimension[0] + xIndex;

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::at(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0) const
{
	if (xIndex >= m_ImageDimension[0] || yIndex >= m_ImageDimension[1] || zIndex >= m_ImageDimension[2])
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::at(xIndex, yIndex, zIndex)" << '\n';
		return m_EmptyVoxel;
	}

	auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageDimension[0] + xIndex;

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
mdk3DImage<VoxelType> mdk3DImage<VoxelType>::GetSubImage(uint64 xIndex_s, uint64 xIndex_e, uint64 yIndex_s, uint64 yIndex_e, uint64 zIndex_s = 0, uint64 zIndex_e = 0) const
{
    mdk3DImage<VoxelType> tempImage; // empty image

	tempImage.SetTobeTemporary();

	if (xIndex_s >= ImageSize[0] || xIndex_e >= ImageSize[0] || xIndex_s > xIndex_e
		|| yIndex_s >= ImageSize[1] || yIndex_e >= ImageSize[1] || yIndex_s > yIndex_e
		|| zIndex_s >= ImageSize[2] || zIndex_e >= ImageSize[2] || zIndex_s > zIndex_e)
	{
		return tempImage;
	}

	Lx = xIndex_e - xIndex_s + 1;
	Ly = yIndex_e - yIndex_s + 1;
	Lz = zIndex_e - zIndex_s + 1;

	tempImage.Initialize(Lx, Ly, Lz,
  		                 m_PhysicalOrigin[0],    m_PhysicalOrigin[1],    m_PhysicalOrigin[2],
		                 m_VoxelPhysicalSize[0], m_VoxelPhysicalSize[1], m_VoxelPhysicalSize[2]);

	tempRawPtr = SubImage.GetVoxelDataRawPointer();

	RawPtr = m_VoxelData->data();

	uint64 Index_k = 0;

	uint64 Index_j = 0;

	Index_k = zIndex_s;

	for (uint64 k = zIndex_s; k <= zIndex_e; ++k)
	{
		Index_j = yIndex_s;

		for (uint64 j = yIndex_s; j <= yIndex_e; ++j)
		{
			for (uint64 i = xIndex_s; i <= xIndex_e; ++i)
			{
				tempRawPtr[0] = RawPtr[Index_k + Index_j + i];

				++tempRawPtr;
			}

			Index_j += Lx;
		}

		Index_k += m_VoxelNumberPerZSlice;
	}

	return tempImage;
}


template<typename VoxelType>
mdk3DImage<VoxelType> 
mdk3DImage<VoxelType>::
Pad(const char* Option, uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0) const
{
    mdk3DImage<VoxelType> tempImage; // empty image

	tempImage.SetTobeTemporary();

	std::string OptionStr(Option);

	if (OptionStr != "replicate")
	{
		mdkError << "Invalid Option @ mdk3DImage::Pad" << '\n';
		return tempImage;
	}

	if (Pad_Lx == 0 && Pad_Ly == 0 && Pad_Lz == 0)
	{
		mdkWarning << "Input Pad Size is [0, 0, 0] @ mdk3DImage::Pad" << '\n';

		tempImage = (*this);

		return tempImage;
	}

	auto Lx = m_ImageDimension[0] + Pad_Lx;
	auto Ly = m_ImageDimension[1] + Pad_Ly;
	auto Lz = m_ImageDimension[2] + Pad_Lz;

	tempImage.Initialize(Lx, Ly, Lz,
		                 m_PhysicalOrigin[0],     m_PhysicalOrigin[1],     m_PhysicalOrigin[2], 
  	                     m_VoxelPhysicalSize[0],  m_VoxelPhysicalSize[1],  m_VoxelPhysicalSize[2])


	if (OptionStr == "zero")
	{
		// tempImage has been filled with 0 by calling Initialize

		for (uint64 k = 0; k <= Lz; ++k)
		{
			for (uint64 j = 0; j <= Ly; ++j)
			{
				for (uint64 i = 0; i <= Lx; ++i)
				{
					auto temp_i = i + Pad_Lx;
					auto temp_j = j + Pad_Ly;
					auto temp_k = k + Pad_Lz;

					tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
				}
			}
		}
	}
	else if (OptionStr == "replicate")
	{
		for (uint64 temp_k = 0; temp_k <= Lz; ++temp_k)
		{
			for (uint64 temp_j = 0; temp_j <= Ly; ++temp_j)
			{
				for (uint64 temp_i = 0; temp_i <= Lx; ++temp_i)
				{
					auto i = std::min(std::max(temp_i - Pad_Lx, 0), m_ImageDimension[0] - 1);

					auto j = std::min(std::max(temp_j - Pad_Ly, 0), m_ImageDimension[1] - 1);

					auto k = std::min(std::max(temp_k - Pad_Lz, 0), m_ImageDimension[2] - 1);

					tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
				}
			}
		}
	}

}


template<typename VoxelType>
mdk3DImage<VoxelType>
mdk3DImage<VoxelType>::
Pad(VoxelType Voxel, uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0) const
{
    mdk3DImage<VoxelType> tempImage; // empty image

	tempImage.SetTobeTemporary();

	if (Pad_Lx == 0 && Pad_Ly == 0 && Pad_Lz == 0)
	{
		mdkWarning << "Input Pad Size is [0, 0, 0] @ mdk3DImage::Pad" << '\n';

		tempImage = (*this);

		return tempImage;
	}

	auto Lx = m_ImageDimension[0] + Pad_Lx;
	auto Ly = m_ImageDimension[1] + Pad_Ly;
	auto Lz = m_ImageDimension[2] + Pad_Lz;

	tempImage.Initialize(Lx, Ly, Lz,
	 	                 m_PhysicalOrigin[0], m_PhysicalOrigin[1], m_PhysicalOrigin[2],
		                 m_VoxelPhysicalSize[0], m_VoxelPhysicalSize[1], m_VoxelPhysicalSize[2])

	for (uint64 temp_k = 0; temp_k <= Lz; ++temp_k)
	{
		for (uint64 temp_j = 0; temp_j <= Ly; ++temp_j)
		{
			for (uint64 temp_i = 0; temp_i <= Lx; ++temp_i)
			{
				if (temp_i < Pad_Lx || temp_i >= m_ImageDimension[0] + Pad_Lx
					|| temp_j < Pad_Lx || temp_j >= m_ImageDimension[1] + Pad_Ly
					|| temp_k < Pad_Lx || temp_k >= m_ImageDimension[2] + Pad_Lz)
				{
					tempImage(temp_i, temp_j, temp_k) = Voxel;
				}
				else
				{
					auto i = temp_i - Pad_Lx;
					auto j = temp_j - Pad_Ly;
					auto k = temp_k - Pad_Lz;

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
UnPad(uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0) const
{
    mdk3DImage<VoxelType> tempImage; // empty image

	tempImage.SetTobeTemporary();

	if (Pad_Lx > m_ImageDimension[0] || Pad_Ly > m_ImageDimension[1] && Pad_Lz > m_ImageDimension[2])
	{
		mdkError << "Invalid Pad Size @ mdk3DImage::UnPad" << '\n';
		return tempImage;
	}

	if (Pad_Lx == m_ImageDimension[0] || Pad_Ly == m_ImageDimension[1] && Pad_Lz == m_ImageDimension[2])
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

	return this->SubImage(Pad_Lx, m_ImageDimension[0] - 1 - Pad_Lx,
		                  Pad_Ly, m_ImageDimension[1] - 1 - Pad_Ly,
		                  Pad_Lz, m_ImageDimension[2] - 1 - Pad_Lz);
}


template<typename VoxelType>
mdkMatrix<uint64>
mdk3DImage<VoxelType>::
GetLinearIndexArrayOfRegion(uint64 xIndex_s,     uint64 Region_Lx,
                            uint64 yIndex_s,     uint64 Region_Ly,
                            uint64 zIndex_s = 0, uint64 Region_Lz = 0) const
{
    mdkMatrix<uint64>  List;

    List.SetTobeTemporaryMatrix();

	if (xIndex_s >= m_ImageDimension[0] || yIndex_s >= m_ImageDimension[1] || zIndex_s >= m_ImageDimension[2] 
		|| Region_Lx > m_ImageDimension[0] - xIndex_s 
		|| Region_Ly > m_ImageDimension[1] - yIndex_s 
		|| Region_Lz > m_ImageDimension[2] - zIndex_s)
	{
		mdkError << "Invalid input @ mdk3DImage::GetLinearIndexArrayOfRegion" << '\n';
		return List;
	}

	if (Region_Lx == 0 || Region_Ly == 0 || Region_Lz == 0)
	{
		mdkWarning << "Empty input region @ mdk3DImage::GetLinearIndexArrayOfRegion" << '\n';
		return List;
	}

    List.SetSize(Region_Lx*Region_Ly*Region_Lz, 1);

	uint64 Counter = 0;

	for (uint64 k = zIndex_s; k < zIndex_s + Region_Lz; ++k)
	{
		for (uint64 j = yIndex_s; j < yIndex_s + Region_Ly; ++j)
		{
			for (uint64 i = xIndex_s; i < xIndex_s + Region_Lx; ++i)
			{
				List[Counter] = k*m_VoxelNumberPerZSlice + j*m_ImageDimension[0] + i;

			    Counter += 1;
			}
		}
	}

	return List;
}


template<typename VoxelType>
inline 
VoxelType mdk3DImage<VoxelType>::At3DIndex(double x, double y, double z,
                                           mdk3DImageInterpolationMethodEnum Method = mdk3DImageInterpolationMethodEnum::NearestNeighbor) const
{
    return m_EmptyVoxel;
}


template<typename VoxelType>
inline
VoxelType mdk3DImage<VoxelType>::operator()(double x, double y, double z,
                                            mdk3DImageInterpolationMethodEnum Method = mdk3DImageInterpolationMethodEnum::NearestNeighbor) const
{
    return m_EmptyVoxel;
}


template<typename VoxelType>
inline
VoxelType mdk3DImage<VoxelType>::At3DPosition(double x, double y, double z,
                                              mdk3DImageInterpolationMethodEnum Method = mdk3DImageInterpolationMethodEnum::NearestNeighbor) const
{
    return m_EmptyVoxel;
}


}//end namespace mdk

#endif