#ifndef __mdkAbstractImage_h
#define __mdkAbstractImage_h

#include "mdkDataObject.h"

namespace mdk
{

class mdkAbstractImage : public mdkDataObject
{

public:		
	// 3D+t
	// Lx: AbstractImage length in x-direction
	// Ly: AbstractImage length in y-direction
	// Lz: AbstractImage length in z-direction
	// Lt: AbstractImage length in t-direction
	//
	mdkAbstractImage();

	~mdkAbstractImage();

	virtual bool Initialize(uint64 Lx, uint64 Ly, uint64 Lz, uint64 Lt, 
		                    uint64 ElementDimension, const double* ElementSize, 
							double Spacing, double TimeInterval) = 0;


private:
	mdkAbstractImage(const mdkAbstractImage&);        // Not implemented.
	void operator=(const mdkAbstractImage&);  // Not implemented.
};

}//end namespace mdk

#endif