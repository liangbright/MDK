#ifndef __mdk3DImage_h
#define __mdk3DImage_h

#include <vector>
#include <memory>

#include "mdkObject.h"
#include "mdkMatrix.h"

namespace mdk
{
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
//
// note: 
// use std::array as VoxelType if voxel is a vector with known length, and do not use std::vector
// --------------------------------------------------------------------------------------------------------//

struct mdk3DImageSize
{
	uint64 Lx;
	uint64 Ly;
	uint64 Lz;
};


struct mdk3DImagePhysicalOrigin
{
    double x;
    double y;
    double z;
};


struct mdk3DImageVoxelPhysicalSize
{
    double Vx;
    double Vy;
    double Vz;
};


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

	VoxelType m_EmptyVoxel_temp;

	bool m_IsTemporaryImage;

public:		
	
	mdk3DImage();

	mdk3DImage(const mdk3DImage<VoxelType>& targetImage);

	void operator=(const mdk3DImage<VoxelType>& targetImage);

	~mdk3DImage();

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz = 1,
		            double PhysicalOrigin_x = 0.0,
		            double PhysicalOrigin_y = 0.0,
		            double PhysicalOrigin_z = 0.0,
		            double VoxelPhysicalSize_x = 1.0,
		            double VoxelPhysicalSize_y = 1.0,
		            double VoxelPhysicalSize_z = 1.0);

	void Clear();

	inline bool IsEmpty() const;

	void Copy(const mdk3DImage<VoxelType>& targetImage);

	void Copy(const VoxelType* VoxelPointer, uint64 Lx, uint64 Ly, uint64 Lz = 1);

    void Swap(mdk3DImage<VoxelType>& targetImage);

    bool Reshape(uint64 Lx, uint64 Ly, uint64 Lz = 1);

	void SetPhysicalOrigin(double PhysicalOrigin_x, double PhysicalOrigin_y, double PhysicalOrigin_z = 0.0);

	void SetVoxelPhysicalSize(double VoxelPhysicalSize_x, double VoxelPhysicalSize_y, double VoxelPhysicalSize_z = 1.0);

	bool Fill(VoxelType Voxel);

	inline void SetTobeTemporaryImage();

	inline std::vector<VoxelType>* GetVoxelDataArrayPointer();

	inline VoxelType* GetVoxelDataRawPointer();

	inline const VoxelType* GetVoxelDataRawPointer() const;

	inline std::shared_ptr<std::vector<VoxelType>> GetVoxelDataSharedPointer();

	inline mdk3DImageSize GetImageSize() const;

	template<typename ScalarType>
	inline void GetImageSize(ScalarType* Lx, ScalarType* Ly, ScalarType* Lz = nullptr) const;

    inline mdk3DImageVoxelPhysicalSize GetVoxelPhysicalSize() const;

	inline void GetVoxelPhysicalSize(double* VoxelPhysicalSize_x, double* VoxelPhysicalSize_y, double* VoxelPhysicalSize_z = nullptr) const;

    inline mdk3DImagePhysicalOrigin GetPhysicalOrigin() const;

	inline void GetPhysicalOrigin(double* PhysicalOrigin_x, double* PhysicalOrigin_y, double* PhysicalOrigin_z = nullptr) const;

	inline void GetLinearIndexBy3DIndex(uint64* LinearIndex, uint64 xIndex, uint64 yIndex, uint64 zIndex = 0) const;

	inline void Get3DIndexByLinearIndex(uint64 LinearIndex, uint64* xIndex, uint64* yIndex, uint64* zIndex = nullptr) const;

	//--------------------------- Get/Set EmptyVoxel (e.g., 0) ------------------------------//

	inline void SetEmptyVoxel(VoxelType EmptyVoxel);

	inline const VoxelType& GetEmptyVoxel();

	//--------------------------- Get/Set Voxel      ------------------------------//

	inline VoxelType& operator()(uint64 LinearIndex);

	inline const VoxelType& operator()(uint64 LinearIndex) const;

	inline VoxelType& operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0);

	inline const VoxelType& operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0) const;

	inline VoxelType& at(uint64 LinearIndex);

	inline const VoxelType& at(uint64 LinearIndex) const;

	inline VoxelType& at(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0);

	inline const VoxelType& at(uint64 xIndex, uint64 yIndex, uint64 zIndex =0) const;

	//-------------------------- Get SubImage -------------------------------//

	mdk3DImage SubImage(uint64 xIndex_s, uint64 xIndex_e, uint64 yIndex_s, uint64 yIndex_e, uint64 zIndex_s = 0, uint64 zIndex_e = 0);

	//-------------------------- Sum, Mean, Max, Min -------------------------------//

	VoxelType Sum();

	VoxelType Mean();

	VoxelType Max();

	VoxelType Min();

	//-------------------------- Interpolation -------------------------------//

	//inline VoxelType& operator()(double x, double y, double z);

	//inline VoxelType& at(double x, double y, double z);


	//-------------------------- Pad, UnPad -------------------------------//

	mdk3DImage  Pad(const char* Option, uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0);

	mdk3DImage  Pad(VoxelType Voxel, uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0);

	mdk3DImage  UnPad(uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0);

	//------------------------- Get LinearIndex In Region -------------------//

    mdkMatrix<uint64> GetLinearIndexArrayOfRegion(uint64 xIndex_s,     uint64 Region_Lx,
		                                          uint64 yIndex_s,     uint64 Region_Ly,
						  					      uint64 zIndex_s = 0, uint64 Region_Lz = 0);

};

}//end namespace mdk

#include "mdk3DImage.hpp"

#endif