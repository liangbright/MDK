#include "mdkProcessObject.h"

namespace mdk
{

ProcessObject::ProcessObject()
{
}
	

ProcessObject::~ProcessObject()
{
}


void ProcessObject::SetupPipelineOutput()
{
    // for example
    // ProcessOutputObjectPointer[0] = &m_Dictionary;
    // ProcessOutputObjectPointerList_SharedCopy[0] = &m_Dictionary_SharedCopy;
}


void ProcessObject::UpdatePipelineOutput()
{
    auto OutputNumber = int_max(ProcessOutputObjectPointerList.size());

    for (int_max i = 0; i < OutputNumber; ++i)
    {
        auto ObjectPointer_i = *ProcessOutputObjectPointerList[i];
        if (ObjectPointer_i != ProcessOutputObjectPointerList_SharedCopy[i])
        {
            ProcessOutputObjectPointerList_SharedCopy[i]->ShallowCopy(*ObjectPointer_i);
        }
    }
}


}//end namespace mdk
