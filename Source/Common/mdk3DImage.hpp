#ifndef __mdk3DImage_hpp
#define __mdk3DImage_hpp

#include <cstdlib>

#include "mdk3DImage.h"

namespace mdk
{

template<typename VoxelType>
mdk3DImage<VoxelType>::mdk3DImage()
{
	this->Clear();
}


template<typename VoxelType>
mdk3DImage<VoxelType>::~mdk3DImage()
{
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::Initialize(uint64 Lx, uint64 Ly, uint64 Lz,
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
		this->Clear();
	}

	m_ImageSize[0] = Lx;
	m_ImageSize[1] = Ly;
	m_ImageSize[2] = Lz;

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
void mdk3DImage<VoxelType>::Clear()
{
	m_VoxelData.reset(new std::vector<VoxelType>);

	m_ImageSize[0] = 0;
	m_ImageSize[1] = 0;
	m_ImageSize[2] = 0;

	m_PhysicalOrigin[0] = 0;
	m_PhysicalOrigin[1] = 0;
	m_PhysicalOrigin[2] = 0;

	m_VoxelPhysicalSize[0] = 0;
	m_VoxelPhysicalSize[1] = 0;
	m_VoxelPhysicalSize[2] = 0;

	m_VoxelNumber = 0;

	m_VoxelNumberPerZSlice = 0;

	//m_EmptyVoxel = 0;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::IsEmpty()
{
	if (m_VoxelNumber == 0)
	{
		return true;
	}

	return false;
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Copy(mdk3DImage<VoxelType> targetImage)
{
	if (targetImage.IsEmpty() == true)
	{
		mdkWarning << "targetImage is empty @ mdk3DImage::Copy" << '\n';
		this->Clear();
		return;
	}

	auto targetSize = targetImage.GetImageSize();

	auto targetRawPtr = GetVoxelDataRawPointer();

	this->Copy(targetRawPtr, targetSize.Lx, targetSize.Ly, targetSize.Lz);

	targetImage.GetVoxelPhysicalSize(&m_VoxelPhysicalSize[0], &m_VoxelPhysicalSize[1], &m_VoxelPhysicalSize[2]);

	targetImage.GetPhysicalOrigin(&m_PhysicalOrigin[0], &m_PhysicalOrigin[1], &m_PhysicalOrigin[2]);
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::Copy(VoxelType* VoxelPointer, uint64 Lx, uint64 Ly, uint64 Lz)
{
	if (VoxelPointer == nullptr)
	{
		return;
	}

	this->Clear();

	m_ImageSize[0] = Lx;
	m_ImageSize[1] = Ly;
	m_ImageSize[2] = Lz;

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
void mdk3DImage<VoxelType>::SetPhysicalOrigin(double PhysicalOrigin_x, double PhysicalOrigin_y, double PhysicalOrigin_z)
{
	m_PhysicalOrigin[0] = PhysicalOrigin_x;
	m_PhysicalOrigin[1] = PhysicalOrigin_y;
	m_PhysicalOrigin[2] = PhysicalOrigin_z;
}


template<typename VoxelType>
void mdk3DImage<VoxelType>::SetVoxelPhysicalSize(double VoxelPhysicalSize_x, double VoxelPhysicalSize_y, double VoxelPhysicalSize_z)
{
	m_VoxelPhysicalSize[0] = VoxelPhysicalSize_x;
	m_VoxelPhysicalSize[1] = VoxelPhysicalSize_y;
	m_VoxelPhysicalSize[2] = VoxelPhysicalSize_z;
}


template<typename VoxelType>
bool mdk3DImage<VoxelType>::Fill(VoxelType Voxel)
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



bool mdk3DImage<std::vector<double>>::Fill(std::vector<double> Voxel)
{
	if (m_VoxelNumber == 0)
	{
		return false;
	}

	auto RawPtr = m_VoxelData->data();

	for (uint64 i = 0; i < m_VoxelNumber; ++i)
	{
		RawPtr[i] = Voxel;

		RawPtr[i].shrink_to_fit();
	}

	return true;
}


template<typename VoxelType>
std::vector<VoxelType>* mdk3DImage<VoxelType>::GetVoxelDataArrayPointer()
{
	return m_VoxelData.get();
}


template<typename VoxelType>
VoxelType* mdk3DImage<VoxelType>::GetVoxelDataRawPointer()
{
	return m_VoxelData->data();
}


template<typename VoxelType>
std::vector<VoxelType>* mdk3DImage<VoxelType>::ReleaseVoxelDataArrayOwnership()
{
	return m_VoxelData.release();
}


template<typename VoxelType>
VoxelType* mdk3DImage<VoxelType>::ReleaseVoxelDataOwnership()
{
	return m_VoxelData.release().data();
}


template<typename VoxelType>
inline
mdk3DImageSize mdk3DImage<VoxelType>::GetImageSize()
{
	mdk3DImageSize Size;

	Size.Lx = m_ImageSize[0];
	Size.Ly = m_ImageSize[1];
	Size.Lz = m_ImageSize[2];

	return Size;
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetImageSize(uint64* Lx, uint64* Ly, uint64* Lz)
{
	Lx[0] = m_ImageSize[0];
	Ly[0] = m_ImageSize[1];
	Lz[0] = m_ImageSize[2];
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetImageSize(uint64* ImageSize)
{
	ImageSize[0] = m_ImageSize[0];
	ImageSize[1] = m_ImageSize[1];
	ImageSize[2] = m_ImageSize[2];
}


template<typename VoxelType>
template<typename ScalarType>
inline
void mdk3DImage<VoxelType>::GetImageSize(ScalarType* Lx, ScalarType* Ly, ScalarType* Lz)
{
	Lx[0] = ScalarType(m_ImageSize[0]);
	Ly[0] = ScalarType(m_ImageSize[1]);
	Lz[0] = ScalarType(m_ImageSize[2]);
}


template<typename VoxelType>
template<typename ScalarType>
inline
void mdk3DImage<VoxelType>::GetImageSize(ScalarType* ImageSize)
{
	ImageSize[0] = ScalarType(m_ImageSize[0]);
	ImageSize[1] = ScalarType(m_ImageSize[1]);
	ImageSize[2] = ScalarType(m_ImageSize[2]);
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetVoxelPhysicalSize(uint64* VoxelPhysicalSize_x, uint64* VoxelPhysicalSize_y, uint64* VoxelPhysicalSize_z)
{
	VoxelPhysicalSize_x[0] = m_VoxelPhysicalSize[0];
	VoxelPhysicalSize_y[0] = m_VoxelPhysicalSize[1];
	VoxelPhysicalSize_z[0] = m_VoxelPhysicalSize[2];
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetPhysicalOrigin(uint64* PhysicalOrigin_x, uint64* PhysicalOrigin_y, uint64* PhysicalOrigin_z)
{
	PhysicalOrigin_x[0] = m_PhysicalOrigin[0];
	PhysicalOrigin_y[0] = m_PhysicalOrigin[1];
	PhysicalOrigin_z[0] = m_PhysicalOrigin[2];
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::GetLinearIndexBy3DIndex(uint64 xIndex, uint64 yIndex, uint64 zIndex, uint64* LinearIndex)
{
	LinearIndex[0] = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageSize[0] + xIndex;
}


template<typename VoxelType>
inline
void mdk3DImage<VoxelType>::Get3DIndexByLinearIndex(uint64 LinearIndex, uint64* xIndex, uint64* yIndex, uint64* zIndex)
{
	std::lldiv_t divresult;
	divresult.rem = LinearIndex;

	divresult = div(divresult.rem, m_VoxelNumberPerZSlice);

	zIndex[0] = divresult.quot;

	divresult = div(divresult.rem, m_ImageSize[0]);

	yIndex[0] = divresult.quot;

	xIndex[0] = divresult.rem;
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::operator()(uint64 LinearIndex)
{
	if (LinearIndex >= m_VoxelNumber)
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::operator(LinearIndex)" << '\n';
		return m_EmptyVoxel;
	}

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::operator()(uint64 LinearIndex) const
{
	if (LinearIndex >= m_VoxelNumber)
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::operator(LinearIndex)" << '\n';
		return m_EmptyVoxel;
	}

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
VoxelType& mdk3DImage<VoxelType>::operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex)
{
	if (xIndex >= m_ImageSize[0] || yIndex >= m_ImageSize[1] || zIndex >= m_ImageSize[2])
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::operator(xIndex, yIndex, zIndex)" << '\n';
		return m_EmptyVoxel;
	}

	auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageSize[0] + xIndex;
	
	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex) const
{
	if (xIndex >= m_ImageSize[0] || yIndex >= m_ImageSize[1] || zIndex >= m_ImageSize[2])
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::operator(xIndex, yIndex, zIndex)" << '\n';
		return m_EmptyVoxel;
	}

	auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageSize[0] + xIndex;

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
VoxelType& mdk3DImage<VoxelType>::at(uint64 LinearIndex)
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
VoxelType& mdk3DImage<VoxelType>::at(uint64 xIndex, uint64 yIndex, uint64 zIndex)
{
	if (xIndex >= m_ImageSize[0] || yIndex >= m_ImageSize[1] || zIndex >= m_ImageSize[2])
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::at(xIndex, yIndex, zIndex)" << '\n';
		return m_EmptyVoxel;
	}

	auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageSize[0] + xIndex;

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
inline
const VoxelType& mdk3DImage<VoxelType>::at(uint64 xIndex, uint64 yIndex, uint64 zIndex) const
{
	if (xIndex >= m_ImageSize[0] || yIndex >= m_ImageSize[1] || zIndex >= m_ImageSize[2])
	{
		mdkError << "LinearIndex >= m_VoxelNumber @ mkd3DImage::at(xIndex, yIndex, zIndex)" << '\n';
		return m_EmptyVoxel;
	}

	auto LinearIndex = zIndex*m_VoxelNumberPerZSlice + yIndex*m_ImageSize[0] + xIndex;

	return m_VoxelData->operator[](LinearIndex);
}


template<typename VoxelType>
mdk3DImage<VoxelType> mdk3DImage<VoxelType>::SubImage(uint64 xIndex_s, uint64 xIndex_e, uint64 yIndex_s, uint64 yIndex_e, uint64 zIndex_s, uint64 zIndex_e)
{
	mdk3DImage<ScalarType> tempImage; // empty image

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
  		                 m_PhysicalOrigin[0], m_PhysicalOrigin[1], m_PhysicalOrigin[2],
		                 m_VoxelPhysicalSize[0], m_VoxelPhysicalSize[1], m_VoxelPhysicalSize[2]);

	tempRawPtr = SubImage.GetVoxelDataRawPointer();

	RawPtr = m_VoxelData->data();

	uint64 Index_k = 0;

	uint64 Index_j = 0;

	Index_k = zIndex_s;

	for (k = zIndex_s; k <= zIndex_e; k++)
	{
		Index_j = yIndex_s;

		for (j = yIndex_s; j <= yIndex_e; j++)
		{
			for (i = xIndex_s; i <= xIndex_e; i++)
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

}//end namespace mdk

#endif