#include <iostream>

#include <mdkObjectBaseInfo.h>

#include<mdkDebugMarco.h>

namespace mdk
{

ObjectBaseInfo::ObjectBaseInfo(ObjectBase* Ptr_Base)
{
	m_ObjectBirthTime = std::time(nullptr);

	m_ObjectDeathTime = 0;

	m_WeakPointerToObject = Ptr_Base->m_WeakPointerToSelf;

	m_IsObjectInHeap = Ptr_Base->m_IsSelfInHeap;

	m_IsObjectAlive = true;

	m_IsObjectCreatedByMdkObject = Ptr_Base->m_IsCreatedByMdkObject;

	m_ObjectName = Ptr_Base->GetClassName();

	MDK_DEBUG_COUT("Welcome @ ObjectBaseInfo()" << std::endl);
}

ObjectBaseInfo::~ObjectBaseInfo()
{
	MDK_DEBUG_COUT("Bye bye @ ~ObjectBaseInfo()" << std::endl);
}

void ObjectBaseInfo::DeathMark()
{
	m_ObjectDeathTime = std::time(nullptr);

	m_IsObjectAlive = false;

	MDK_DEBUG_COUT("DeathMark() @ ObjectBaseInfo" << std::endl);
}

}//end namespace mdk