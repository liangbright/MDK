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
// reslice the input image using ITK/VTK, then feed it into this class.
//
// ok to store image with non-isotropic voxels
// but such image is not good for analysis
// if the size is 0.5/0.5/6, then it is really bad and useless for 3D analysis
//
// ITK can register images with non-isotropic voxels
//
// note: 
// use std::array as VoxelType if voxel is a vector with known length, and do not use std::vector
// --------------------------------------------------------------------------------------------------------//

struct mdk3DImageDimension
{
	uint64 Lx;
	uint64 Ly;
	uint64 Lz;
};

struct mdk3DImagePhysicalSize
{
    double Sx;
    double Sy;
    double Sz;
};

struct mdk3DImagePhysicalOrigin
{
    double x;
    double y;
    double z;
};

struct mdk3DImageVoxelPhysicalSize
{
    double Sx;
    double Sy;
    double Sz;
};

typedef enum
{
    NearestNeighbor,
    CubicLinear,
} mdk3DImageInterpolationMethodEnum;


template<typename VoxelType>
class mdk3DImage : public mdkObject
{

private:

	std::shared_ptr<std::vector<VoxelType>> m_VoxelData;

    uint64 m_ImageDimension[3]; // {Lx, Ly, Lz} number of voxels in each direction

	uint64 m_VoxelNumber;  // total number of voxels

	uint64 m_VoxelNumberPerZSlice; // total number of voxels in each z-slice  = m_ImageSize[2]*m_ImageSize[1]

	double m_PhysicalOrigin[3];    // i.e., Origin in ITK, VTK, {x0, y0, z0} in world coordinate system (x,y,z) (unit: mm)

	double m_VoxelPhysicalSize[3]; // i.e., Spacing in ITK, VTK

	VoxelType m_ZeroVoxel;

    VoxelType m_ZeroVoxel_Error_Output;

public:		
	
	mdk3DImage();

	mdk3DImage(const mdk3DImage<VoxelType>& targetImage) = delete;

    mdk3DImage(mdk3DImage<VoxelType>&& targetImage);

    mdk3DImage(uint64 Lx, uint64 Ly, uint64 Lz = 1,
               double PhysicalOrigin_x = 0.0,
               double PhysicalOrigin_y = 0.0,
               double PhysicalOrigin_z = 0.0,
               double VoxelPhysicalSize_x = 1.0,
               double VoxelPhysicalSize_y = 1.0,
               double VoxelPhysicalSize_z = 1.0);

	~mdk3DImage();

    void operator=(const mdk3DImage<VoxelType>& targetImage);

    void operator=(mdk3DImage<VoxelType>&& targetImage);

    //---------------------------------------------------------//

    void Copy(const mdk3DImage<VoxelType>& targetImage);

    void Copy(const VoxelType* VoxelPointer, uint64 Lx, uint64 Ly, uint64 Lz = 1);

    void Take(mdk3DImage<VoxelType>&& targetImage);

    void Take(mdk3DImage<VoxelType>& targetImage);

    //-----------------------------------------------------------------//

    void Reset();

    void ForceClear();

    //-----------------------------------------------------------------//

    bool ReInitialize(uint64 Lx, uint64 Ly, uint64 Lz = 1,
		              double PhysicalOrigin_x = 0.0,
		              double PhysicalOrigin_y = 0.0,
		              double PhysicalOrigin_z = 0.0,
		              double VoxelPhysicalSize_x = 1.0,
		              double VoxelPhysicalSize_y = 1.0,
		              double VoxelPhysicalSize_z = 1.0);

    inline bool Fill(const VoxelType& Voxel);

	inline bool IsEmpty() const;

    inline VoxelType* GetVoxelDataRawPointer();

    inline const VoxelType* GetVoxelDataRawPointer() const;

    inline const std::shared_ptr<std::vector<VoxelType>>& GetVoxelDataSharedPointer() const; 

    // ------------------------ Get/Set ImageInfo ------------------------------------------------------------------------//

    inline mdk3DImageDimension GetImageDimension() const;

    inline void GetImageDimension(uint64* Lx, uint64* Ly, uint64* Lz = nullptr) const;

    inline mdk3DImagePhysicalSize GetImagePhysicalSize() const;

    inline void GetImagePhysicalSize(double* PhysicalSize_x, double* PhysicalSize_y, double* PhysicalSize_z = nullptr) const;

	void SetPhysicalOrigin(double PhysicalOrigin_x, double PhysicalOrigin_y, double PhysicalOrigin_z = 0.0);

    inline mdk3DImagePhysicalOrigin GetPhysicalOrigin() const;

    inline void GetPhysicalOrigin(double* PhysicalOrigin_x, double* PhysicalOrigin_y, double* PhysicalOrigin_z = nullptr) const;

	void SetVoxelPhysicalSize(double VoxelPhysicalSize_x, double VoxelPhysicalSize_y, double VoxelPhysicalSize_z = 1.0);

    inline mdk3DImageVoxelPhysicalSize GetVoxelPhysicalSize() const;

    inline void GetVoxelPhysicalSize(double* VoxelPhysicalSize_x, double* VoxelPhysicalSize_y, double* VoxelPhysicalSize_z = nullptr) const;

    // ------------------------ Index and Position ------------------------------------------------------------------------//

	inline void GetLinearIndexBy3DIndex(uint64* LinearIndex, uint64 xIndex, uint64 yIndex, uint64 zIndex = 0) const;

	inline void Get3DIndexByLinearIndex(uint64 LinearIndex, uint64* xIndex, uint64* yIndex, uint64* zIndex = nullptr) const;

    inline void Get3DPositionByLinearIndex(uint64 LinearIndex, double* x, double* y, double* z = nullptr) const;

    inline void Get3DPositionBy3DIndex(uint64 xIndex, uint64 yIndex, uint64 zIndex, double* x, double* y, double* z = nullptr) const;

	//--------------------------- Get/Set ZeroVoxel ------------------------------//

    inline void SetZeroVoxel(VoxelType EmptyVoxel);

    inline const VoxelType& GetZeroVoxel() const;

	//--------------------------- Get/Set Voxel      ------------------------------//

    inline VoxelType& operator[](uint64 LinearIndex);

    inline const VoxelType& operator[](uint64 LinearIndex) const;

	inline VoxelType& operator()(uint64 LinearIndex);

	inline const VoxelType& operator()(uint64 LinearIndex) const;

	inline VoxelType& operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0);

	inline const VoxelType& operator()(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0) const;

	inline VoxelType& at(uint64 LinearIndex);

	inline const VoxelType& at(uint64 LinearIndex) const;

	inline VoxelType& at(uint64 xIndex, uint64 yIndex, uint64 zIndex = 0);

	inline const VoxelType& at(uint64 xIndex, uint64 yIndex, uint64 zIndex =0) const;

	//-------------------------- Get SubImage -------------------------------//

    mdk3DImage GetSubImage(uint64 xIndex_s, uint64 xIndex_e, uint64 yIndex_s, uint64 yIndex_e, uint64 zIndex_s = 0, uint64 zIndex_e = 0) const;

	//-------------------------- Pad, UnPad -------------------------------//

    mdk3DImage  Pad(const char* Option, uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0) const;

    mdk3DImage  Pad(VoxelType Voxel, uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0) const;

    mdk3DImage  UnPad(uint64 Pad_Lx, uint64 Pad_Ly, uint64 Pad_Lz = 0) const;

	//------------------------- Get LinearIndex In Region -------------------//

    mdkMatrix<uint64> GetLinearIndexListOfRegion(uint64 xIndex_s,     uint64 Region_Lx,
		                                         uint64 yIndex_s,     uint64 Region_Ly,
                                                 uint64 zIndex_s = 0, uint64 Region_Lz = 0) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    VoxelType Sum() const;

    VoxelType Mean() const;

    VoxelType Max() const;

    VoxelType Min() const;

    //-------------------------- Interpolation at 3D Position (x, y, z)-------------------------------//

    inline VoxelType At3DPosition(double x, double y, double z, 
                                  mdk3DImageInterpolationMethodEnum Method = mdk3DImageInterpolationMethodEnum::NearestNeighbor) const;

};


}//end namespace mdk

#include "mdk3DImage.hpp"

#endif