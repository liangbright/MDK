#ifndef __mdkObjectBaseInfo_h
#define __mdkObjectBaseInfo_h

#include <ctime>
#include <memory>

#include <mdkObjectBase.h>

namespace mdk
{

class ObjectBaseInfo
{
public:
	
	std::weak_ptr<ObjectBase> m_WeakPointerToObject;

	bool  m_IsObjectInHeap;
	
	bool  m_IsObjectAlive;

	bool m_IsObjectCreatedByMdkObject;

	std::time_t m_ObjectBirthTime;
	
	std::time_t m_ObjectDeathTime;

	std::string m_ObjectName;

public:
	ObjectBaseInfo(ObjectBase*);
	virtual ~ObjectBaseInfo(void);	

	void DeathMark(void);	
};

}//end namespace mdk

#endif