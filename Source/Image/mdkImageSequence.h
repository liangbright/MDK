#ifndef __mdkImageSequence_h
#define __mdkImageSequence_h

#include <vector>

#include "mdkObject.h"
#include "mdkImage.h"

namespace mdk
{

//  Image Sequence Class
// Voxel = a vector of scalars
// Lx: number of voxels in x-direction
// Ly: number of voxels in y-direction
// Lz: number of voxels in z-direction
// Lt: number of voxels in t-direction

template<typename VoxelType>
class ImageSequence : public Object
{

private:

    std::vector<Image<VoxelType>> m_ImageData;

	double VoxelPhysicalSize[3];

	// time interval between frames (unit: second)
	// zero if Lt = 0
	std::vector<double> m_TimeInterval;

	uint64 m_ImageSize[4]; // {Lx, Ly, Lz, Lt}  number of voxels in each direction

	double m_PhysicalOrigin[4]; // {x0, y0, z0, t0} in world coordinate system (unit: mm, second)

public:		
	
	ImageSequence();

	~ImageSequence();

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
		            double VoxelPhysicalSize_x, double VoxelPhysicalSize_y, double VoxelPhysicalSize_z,
                    const std::vector<double>& TimeInterval);

    bool Swallow(Image<VoxelType>& targetImage);

private:
	ImageSequence(const ImageSequence&);        // Not implemented.
	void operator=(const ImageSequence&);            // Not implemented.
};

}//end namespace mdk

#endif