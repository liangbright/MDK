#include "mdkAlgorithmObject.h"

namespace mdk
{
		
mdkAlgorithmObject::mdkAlgorithmObject()
{
}

mdkAlgorithmObject::~mdkAlgorithmObject()
{
}

void mdkAlgorithmObject::Apply(const std::vector<mdkObject*>& InputList, const std::vector<mdkObject*>& OutputList)
{
	this->SetInputList(InputList);

	this->SetOutputList(OutputList);

	//Compute Output
	this->Run();

	// clear the two lists
	this->ClearInputAndOutputList();

	//clear internal data, not input or output
	this->Clear();

}

}//end namespace mdk