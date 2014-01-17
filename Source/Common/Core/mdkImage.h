#ifndef __mdkImage_h
#define __mdkImage_h

#include "mdkStructuredDataArray.h"

namespace mdk
{

// 3D+t Image Class
// Voxel = Element in mdkStructuredDataArray
// Voxel is the space occupied by element 
// Lx: image length in x-direction
// Ly: image length in y-direction
// Lz: image length in z-direction
// Lt: image length in t-direction

class mdkImage : public mdkStructuredDataArray
{

private:
	// VoxelPhysicalSize (unit: mm), e.g., 0.5mm x 0.5mm x 0.5mm,  same resolution in x, y, z directions 
	// same resolution => image filters do not need to be re-sampled with different voxel shape
	// reslicethe input image using itk/vtk, then feed it into this class.
	//
	// ok to store image with nonisotropic voxels
	// but such image is not good for analsyis
	// if the size is 0.5/0.5/6, then it is realy bad and useless for 3D analysis
	//
	// itk can register images with nonisotropic voxels

	double ElementPhysicalSize[3];

	// time interval between frames (unit: second)
	// zero if Lt = 0
	double m_TimeInterval;

	// time intervals between frames (unit: second)
	// if time intervals are not equal to each other
	// nullptr if Lt=0 or equal intervals
	double* m_TimeIntervalList;

	uint64 m_ImageSize[4]; // {Lx, Ly, Lz, Lt}  element/voxel number in each direction

	double m_PhysicalOrigin[4]; // {x0, y0, z0, t0} in world coordinate system (unit: mm)

public:		
	
	mdkImage();

	~mdkImage();

	bool Initialize(mdkScalarTypeEnum ScalarType, 
		            uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
					uint64 ElementDimension, double ElementSize[],
					double ElementPhysicalSize_x, double ElementPhysicalSize_y, double ElementPhysicalSize_z,
					double TimeInterval);

	bool Initialize(mdkScalarTypeEnum ScalarType, 
		            uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
					uint64 ElementDimension, double ElementSize[],
					double ElementPhysicalSize_x, double ElementPhysicalSize_y, double ElementPhysicalSize_z,
					double TimeIntervalList[]);

private:
	mdkImage(const mdkImage&);        // Not implemented.
	void operator=(const mdkImage&);  // Not implemented.
};

}//end namespace mdk

#endif