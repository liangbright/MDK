#ifndef __mdkObjectBase_h
#define __mdkObjectBase_h

#include <memory>
#include <string>

namespace mdk
{

class ObjectBase
{
friend class  ObjectManager;

friend class ObjectBaseInfo;

private:
	std::weak_ptr<ObjectBase> m_WeakPointerToSelf;

	bool m_IsRegisteredByObjectManager;

	bool  m_IsSelfInHeap;

	bool  m_IsCreatedByMdkObject;

	bool  m_IsRecordBeingDeletedByOtherFunction;

public:
	ObjectBase();

	virtual ~ObjectBase();

	virtual std::string GetClassName();

	std::shared_ptr<ObjectBase> GetSharedPointerToObjectBase();
};

}//end namespace mdk

#endif