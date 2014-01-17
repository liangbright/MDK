#ifndef __mdkImageTemplate_h
#define __mdkImageTemplate_h

#include "mdkDataArray.h"

namespace mdk
{

template<typename ScalarType>
class mdkImageTemplate : public mdkDataArrayTemplate<ScalarType>
{

public:

	double ElementDimension;

	double ElementSize;

	uint64 ImageSize[4]; // {Lx, Ly, Lz, Lt}

public:		
	// 3D+t
	// Lx: ImageTemplate length in x-direction
	// Ly: ImageTemplate length in x-direction
	// Lz: ImageTemplate length in z-direction
	// Lt: ImageTemplate length in t-direction
	//
	mdkImageTemplate();

	~mdkImageTemplate();

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt, 
		            uint64 ComponentNumber, double VoxelSize, double TimeInterval);

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
		            uint64 ComponentNumber, double VoxelSize, double* TimeIntervalList);

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
		            uint64 ComponentNumber, double* VoxelSizeList, double TimeInterval);

	bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt,
		            uint64 ComponentNumber, double* VoxelSizeList, double* TimeIntervalList);

private:
	mdkImageTemplate(const mdkImageTemplate&);        // Not implemented.
	void operator=(const mdkImageTemplate&);  // Not implemented.
};

}//end namespace mdk

#endif