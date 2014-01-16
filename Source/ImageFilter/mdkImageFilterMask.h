#ifndef __mdkImageFilterMask_h
#define __mdkImageFilterMask_h

#include <vector>

#include "mdkAlgorithmObject.h"
#include "mdkMatrix.h"

namespace mdk
{

class mdkImageFilterMask : public  mdkAlgorithmObject
{

private:


public:		
	mdkImageFilterMask();
	~mdkImageFilterMask();
  

	void Apply(mdkMatrix* Input, mdkMatrix* Output);

private:
	mdkImageFilterMask(const mdkImageFilterMask&); // Not implemented.
	void operator=(const mdkImageFilterMask&);     // Not implemented.


};
}//end namespace mdk

#endif