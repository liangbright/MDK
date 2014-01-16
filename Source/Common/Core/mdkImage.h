#ifndef __mdkImage_h
#define __mdkImage_h

#include "mdkDataArray.h"

namespace mdk
{

template<typename ScalarType>
class mdkImage : public mdkDataArray<ScalarType>
{

public:
	// VoxelSize unit: mm, e.g., 0.5mm x 0.5mm x 0.5mm,  same resolution in x, y, z directions 
	// same resolution => image filters do not need to be re-sampled with different voxel shape
	// reslicethe input image using itk/vtk, then feed it into this class.
	double VoxelSize; 

	// ok to store image with nonisotropic voxels
	// but such image is not good for analsyis
	// if the size is 0.5/0.5/6, then it is realy bad and useless for 3D analysis
	//
	// itk can register images with nonisotropic voxels
	// 
	double* VoxelSizeList;

	// a pixel can be a vector with many components
	//uint64 ComponentNumber = ElementLength;

	double TimeInterval; // time interval between frames unit: second

	double* TimeIntervalList; // if time intervals are not equal to each other

	uint64 ImageSize[4]; // {Lx, Ly, Lz, Lt}

public:		
	// 3D+t
	// Lx: image length in x-direction
	// Ly: image length in x-direction
	// Lz: image length in z-direction
	// Lt: image length in t-direction
	//
	mdkImage();

	~mdkImage();

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt, 
		            uint64 ComponentNumber, double VoxelSize, double TimeInterval);

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
		            uint64 ComponentNumber, double VoxelSize, double* TimeIntervalList);

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
		            uint64 ComponentNumber, double* VoxelSizeList, double TimeInterval);

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
		            uint64 ComponentNumber, double* VoxelSizeList, double* TimeIntervalList);

private:
	mdkImage(const mdkImage&);        // Not implemented.
	void operator=(const mdkImage&);  // Not implemented.
};

}//end namespace mdk

#endif