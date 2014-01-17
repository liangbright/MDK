#ifndef __mdkDataObject_h
#define __mdkDataObject_h

#include "mdkObject.h"

namespace mdk
{

class mdkDataObject : public mdkObject
{

public:		
	mdkDataObject(void);
	~mdkDataObject(void);

private:
	mdkDataObject(const mdkDataObject&);   // Not implemented.
	void operator=(const mdkDataObject&);  // Not implemented.

};

}//end namespace mdk

#endif