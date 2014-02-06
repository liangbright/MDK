#ifndef __mdk3DImage_h
#define __mdk3DImage_h

#include <vector>
#include <memory>

#include "mdkObject.h"

namespace mdk
{

struct mdk3DImageSize
{
	uint64 Lx;
	uint64 Ly;
	uint64 Lz;
};

//-------------------------------------------------------------------------------------------------------//
// 3D Image Class
// Voxel = an array of scalars
// Lx: number of voxels in x-direction
// Ly: number of voxels in y-direction
// Lz: number of voxels in z-direction
//
// VoxelPhysicalSize (unit: mm), e.g., 0.5mm x 0.5mm x 0.5mm,  same resolution in x, y, z directions 
// same resolution => image filters do not need to be re-sampled with different voxel shape
// reslice the input image using itk/vtk, then feed it into this class.
//
// ok to store image with nonisotropic voxels
// but such image is not good for analsyis
// if the size is 0.5/0.5/6, then it is realy bad and useless for 3D analysis
//
// itk can register images with nonisotropic voxels
// --------------------------------------------------------------------------------------------------------//

template<typename VoxelType>
class mdk3DImage : public mdkObject
{

private:

	std::shared_ptr<std::vector<VoxelType>> m_VoxelData;

	uint64 m_ImageSize[3]; // {Lx, Ly, Lz} number of voxels in each direction

	uint64 m_VoxelNumber;  // total number of voxels

	uint64 m_VoxelNumberPerZSlice; // total number of voxels in each z-slice  = m_ImageSize[2]*m_ImageSize[1]

	double m_PhysicalOrigin[3];    // {x0, y0, z0} in world coordinate system (x,y,z) (unit: mm)

	double m_VoxelPhysicalSize[3];

	VoxelType m_EmptyVoxel;

	VoxelType m_EmptyVoxel_output;

	bool m_IsTemporaryImage;

public:		
	
	mdk3DImage();

	mdk3DImage(const mdk3DImage<VoxelType>& targetImage);

	void operator=(const mdk3DImage<VoxelType>& targetImage);

	~mdk3DImage();

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz,
		            double PhysicalOrigin_x = 0.0,
		            double PhysicalOrigin_y = 0.0,
		            double PhysicalOrigin_z = 0.0,
		            double VoxelPhysicalSize_x = 1.0,
		            double VoxelPhysicalSize_y = 1.0,
		            double VoxelPhysicalSize_z = 1.0);

	void Clear();

	bool IsEmpty() const;

	void Copy(const mdk3DImage<VoxelType>& targetImage);

	void Copy(const VoxelType* VoxelPointer, uint64 Lx, uint64 Ly, uint64 Lz);

	void SetPhysicalOrigin(double PhysicalOrigin_x, double PhysicalOrigin_y, double PhysicalOrigin_z);

	void SetVoxelPhysicalSize(double VoxelPhysicalSize_x, double VoxelPhysicalSize_y, double VoxelPhysicalSize_z);

	bool Fill(VoxelType Voxel);

	void SetTobeTemporaryImage();

	std::vector<VoxelType>* GetVoxelDataArrayPointer();

	VoxelType* GetVoxelDataRawPointer();

	const VoxelType* GetVoxelDataRawPointer() const;

	std::shared_ptr<std::vector<VoxelType>> GetVoxelDataSharedPointer();

	inline mdk3DImageSize GetImageSize() const;

	inline void GetImageSize(uint64* Lx, uint64* Ly, uint64* Lz) const;

	inline void GetImageSize(uint64* ImageSize) const;

	template<typename ScalarType>
	inline void GetImageSize(ScalarType* Lx, ScalarType* Ly, ScalarType* Lz) const;

	template<typename ScalarType>
	inline void GetImageSize(ScalarType* ImageSize) const;

	inline void GetVoxelPhysicalSize(uint64* VoxelPhysicalSize_x, uint64* VoxelPhysicalSize_y, uint64* VoxelPhysicalSize_z) const;

	inline void GetPhysicalOrigin(uint64* PhysicalOrigin_x, uint64* PhysicalOrigin_y, uint64* PhysicalOrigin_z) const;

	inline void GetLinearIndexBy3DIndex(uint64 xIndex, uint64 yIndex, uint64 zIndex, uint64* LinearIndex) const;

	inline void Get3DIndexByLinearIndex(uint64 LinearIndex, uint64* xIndex, uint64* yIndex, uint64* zIndex) const;

	//--------------------------- Get EmptyVoxel (e.g., 0) ------------------------------//

	inline const VoxelType& GetEmptyVoxel();

	//--------------------------- Get/Set Voxel      ------------------------------//

	inline VoxelType& operator()(uint64 LinearIndex);

	inline const VoxelType& operator()(uint64 LinearIndex) const;

	inline VoxelType& operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex);

	inline const VoxelType& operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex) const;

	inline VoxelType& at(uint64 LinearIndex);

	inline const VoxelType& at(uint64 LinearIndex) const;

	inline VoxelType& at(uint64 xIndex, uint64 yIndex, uint64 zIndex);

	inline const VoxelType& at(uint64 xIndex, uint64 yIndex, uint64 zIndex) const;

	//-------------------------- Get SubImage -------------------------------//

	mdk3DImage SubImage(uint64 xIndex_s, uint64 xIndex_e, uint64 yIndex_s, uint64 yIndex_e, uint64 zIndex_s, uint64 zIndex_e);

	//-------------------------- Sum, Mean, Max, Min -------------------------------//

	VoxelType Sum();

	VoxelType Mean();

	VoxelType Max();

	VoxelType Min();

	//-------------------------- Interpolation -------------------------------//

	//inline VoxelType& operator()(double x, double y, double z);

	//inline VoxelType& at(double x, double y, double z);

};

}//end namespace mdk

#include "mdk3DImage.hpp"

#endif