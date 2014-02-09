#ifndef __mdk3DImageSequence_h
#define __mdk3DImageSequence_h

#include <vector>

#include "mdkObject.h"
#include "mdk3DImage.h"

namespace mdk
{

// 3D Image Sequence Class
// Voxel = a vector of scalars
// Lx: number of voxels in x-direction
// Ly: number of voxels in y-direction
// Lz: number of voxels in z-direction
// Lt: number of voxels in t-direction

template<typename VoxelType>
class mdk3DImageSequence : public mdkObject
{

private:

    std::vector<mdk3DImage<VoxelType>> m_ImageData;

	double VoxelPhysicalSize[3];

	// time interval between frames (unit: second)
	// zero if Lt = 0
	std::vector<double> m_TimeInterval;

	uint64 m_ImageSize[4]; // {Lx, Ly, Lz, Lt}  number of voxels in each direction

	double m_PhysicalOrigin[4]; // {x0, y0, z0, t0} in world coordinate system (unit: mm, second)

public:		
	
	mdk3DImageSequence();

	~mdk3DImageSequence();

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
		            double VoxelPhysicalSize_x, double VoxelPhysicalSize_y, double VoxelPhysicalSize_z,
                    const std::vector<double>& TimeInterval);

    bool Swallow(mdk3DImage<VoxelType>& targetImage);

private:
	mdk3DImageSequence(const mdk3DImageSequence&);        // Not implemented.
	void operator=(const mdk3DImageSequence&);            // Not implemented.
};

}//end namespace mdk

#endif