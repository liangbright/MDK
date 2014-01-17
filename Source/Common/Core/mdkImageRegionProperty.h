#ifndef __mdkImageRegionProperty_h
#define __mdkImageRegionProperty_h

#include "mdkObject.h"

namespace mdk
{

class mdkImageRegionProperty : public mdkObject
{

public:
	uint64 RegionNumber;

public:		
	mdkImageRegionProperty();
	~mdkImageRegionProperty();

};

}//end namespace mdk

#endif