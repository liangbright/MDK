#ifndef __mdkAlgorithmObject_h
#define __mdkAlgorithmObject_h

#include <vector>

#include <mdkObject.h>

namespace mdk
{

class mdkAlgorithmObject : public mdkObject
{

public:		
	mdkAlgorithmObject();
	~mdkAlgorithmObject();

	void Apply(const std::vector<mdkObject*>& InputList, const std::vector<mdkObject*>& OutputList);

private:
	mdkAlgorithmObject(const mdkAlgorithmObject&);   // Not implemented.
	void operator=(const mdkAlgorithmObject&);       // Not implemented.

};

}//end namespace mdk

#endif