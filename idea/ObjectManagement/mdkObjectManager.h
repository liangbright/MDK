#ifndef __mdkObjectManager_h
#define __mdkObjectManager_h

#include <unordered_map>
#include <mutex>

#include <mdkObjectBase.h>
#include <mdkObjectBaseInfo.h>

namespace mdk
{

// TODO: 
// test the thread safety of this class.

class ObjectManager
{

private:
	
	static std::unordered_map<ObjectBase*, ObjectBaseInfo*> m_ObjectInfoMap;

	static std::unordered_map<ObjectBase*, ObjectBaseInfo*> m_HeapObjectInfoMap;

	static std::unordered_map<ObjectBase*, ObjectBaseInfo*> m_StackObjectInfoMap;

	static std::mutex  m_Register_Mutex;

	static std::mutex  m_UnRegister_Mutex;

	static std::mutex  m_Heap_Register_Mutex;

	static std::mutex  m_Heap_UnRegister_Mutex;

	static std::mutex  m_Stack_Register_Mutex;

	static std::mutex  m_Stack_UnRegister_Mutex;

	static std::mutex  m_CollectHeapGarbage_Mutex;

	static std::mutex  m_DeleteHeapObject_Mutex;

public:
	
	static std::shared_ptr<ObjectBase> CreateTheFirstSharedPointerToHeapObject(ObjectBase*);

	static bool WriteBasicInfoToHeapObject(ObjectBase*);
	
	static bool WriteBasicInfoToStackObject(ObjectBase*);

	static bool Register(ObjectBase*);
	
	static bool UnRegister(ObjectBase*);

	static bool CollectHeapGarbage();

	static bool IsObjectValid(ObjectBase*);

	static unsigned long long int GetObjectTotalNumber();

	static bool RegisterHeapObject(ObjectBase*);
	
	static bool UnRegisterHeapObject(ObjectBase*);

	static bool DeleteHeapObject(ObjectBase*);
	
	static bool IsObjectInHeap(ObjectBase*);	

	static bool IsHeapObjectBeingShared(ObjectBase*);

	static std::shared_ptr<ObjectBase> CreateSharedPointerToHeapObjectInRecord(ObjectBase*);

	static unsigned long long int GetHeapObjectTotalNumber();

	static bool RegisterStackObject(ObjectBase*);

	static bool UnRegisterStackObject(ObjectBase*);
				
	static bool IsObjectInStack(ObjectBase*);
	
	static unsigned long long int GetStackObjectTotalNumber();

	static bool IsObjectInUnknowPlace(ObjectBase*);

	static bool RegisterUnusualObject(ObjectBase*);

	static bool UnRegisterUnusualObject(ObjectBase*);

	static void PrintSelf();

private:

	ObjectManager();
	~ObjectManager();
	
};

}//end namespace mdk

#endif