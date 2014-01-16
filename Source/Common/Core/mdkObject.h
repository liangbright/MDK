#ifndef __mdkObject_h
#define __mdkObject_h

#include <vector>

#include "mdkType.h"

namespace mdk
{

class mdkObject
{

private:

	std::vector<mdkObject*>  m_InputList;

	std::vector<mdkObject*>  m_OutputList;

	uint64 m_MaximumInputNumber;

	uint64 m_MaximumOutputNumber;

	uint64 m_SelfUpdateCounter;

public:		
	mdkObject();
	~mdkObject();

	virtual void Run();

	virtual bool Update();

	virtual bool IsModified();

	virtual void Clear();

protected:

	void SetMaximumInputNumber(uint64 Number);

	void SetMaximumOutputNumber(uint64 Number);

	void SetInputList(const std::vector<mdkObject*>& InputList);

	void SetOutputList(const std::vector<mdkObject*>& OutputList);

	bool SetInputConnection(uint64 InputPort, mdkObject* Input);

	bool SetInputConnection(mdkObject* Input); // InputPort=0;

	bool SetOutputConnection(uint64 OutputPort, mdkObject* Output);

	bool SetOutputConnection(mdkObject* Output); // OutputPort=0;

	void ClearInputAndOutputList();

private:
	mdkObject(const mdkObject&);       // Not implemented.
	void operator=(const mdkObject&);  // Not implemented.
};

}//end namespace mdk

#endif