#include <iostream>
#include <string>

#include <mdkObjectManager.h>

#include<mdkDebugMarco.h>

namespace mdk
{

std::unordered_map<ObjectBase*, ObjectBaseInfo*> ObjectManager::m_ObjectInfoMap;

std::unordered_map<ObjectBase*, ObjectBaseInfo*> ObjectManager::m_HeapObjectInfoMap;

std::unordered_map<ObjectBase*, ObjectBaseInfo*> ObjectManager::m_StackObjectInfoMap;

std::mutex  ObjectManager::m_Register_Mutex;

std::mutex  ObjectManager::m_UnRegister_Mutex;

std::mutex  ObjectManager::m_Heap_Register_Mutex;

std::mutex  ObjectManager::m_Heap_UnRegister_Mutex;

std::mutex  ObjectManager::m_Stack_Register_Mutex;

std::mutex  ObjectManager::m_Stack_UnRegister_Mutex;

std::mutex  ObjectManager::m_CollectHeapGarbage_Mutex;

std::mutex  ObjectManager::m_DeleteHeapObject_Mutex;


ObjectManager::ObjectManager()
{
	MDK_DEBUG_COUT("Welcome @ ObjectManager(): You should not call this fuction" << std::endl);
}

ObjectManager::~ObjectManager()
{
	MDK_DEBUG_COUT("Bye bye @ ~ObjectManager(): You should not call this fuction" << std::endl);
}

std::shared_ptr<ObjectBase> ObjectManager::CreateTheFirstSharedPointerToHeapObject(ObjectBase* Ptr_Base)
{
	std::shared_ptr<ObjectBase> sp(Ptr_Base);

	Ptr_Base->m_WeakPointerToSelf = sp;

	return sp;
}

bool ObjectManager::WriteBasicInfoToHeapObject(ObjectBase* Ptr_Base)
{
	Ptr_Base->m_IsSelfInHeap = true;

	Ptr_Base->m_IsCreatedByMdkObject = true;

	return true;
}

bool ObjectManager::WriteBasicInfoToStackObject(ObjectBase* Ptr_Base)
{
	Ptr_Base->m_IsSelfInHeap = false;

	Ptr_Base->m_IsCreatedByMdkObject = true;

	return true;
}

// this function is thread safe with mutex
bool ObjectManager::Register(ObjectBase* Ptr_Base)
{
	m_Register_Mutex.lock();

	MDK_DEBUG_COUT("Try to Register an object @ ObjectManager::Register()" << std::endl);

	auto it = m_ObjectInfoMap.find(Ptr_Base);

	if (it == m_ObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("Register the object " << Ptr_Base->GetClassName() << " @ ObjectManager::Register()" << std::endl);

		m_ObjectInfoMap[Ptr_Base] = new ObjectBaseInfo(Ptr_Base);

		m_Register_Mutex.unlock();

		return true;
	}
	
	MDK_DEBUG_COUT("Can not Register the object @ ObjectManager::Register()" << std::endl);

	m_Register_Mutex.unlock();
	
	return false;	
}

// this function is thread safe with mutex
bool ObjectManager::UnRegister(ObjectBase* Ptr_Base)
{
	m_UnRegister_Mutex.lock();

	MDK_DEBUG_COUT("Try to UnRegister an object @ ObjectManager::UnRegister()" << std::endl);

	auto it = m_ObjectInfoMap.find(Ptr_Base);

	if (it != m_ObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("UnRegister the object " << Ptr_Base->GetClassName() << " @ ObjectManager::UnRegister()" << std::endl);

		// delete the record
		delete it->second;
		m_ObjectInfoMap.erase(it);

		m_UnRegister_Mutex.unlock();

		return true;
	}

	MDK_DEBUG_COUT("Can not UnRegister the object @ ObjectManager::UnRegister()" << std::endl);

	m_UnRegister_Mutex.unlock();

	return false;
}

bool ObjectManager::UnRegisterUnusualObject(ObjectBase* Ptr_Base)
{
	return ObjectManager::UnRegister(Ptr_Base);
}

bool ObjectManager::IsObjectValid(ObjectBase* Ptr_Base)
{
	MDK_DEBUG_COUT("check if the object is valid (in record and alive)" << std::endl);

	auto temp1 = ObjectManager::IsObjectInStack(Ptr_Base);
	if (temp1 == true)
	{
		return true;
	}

	auto temp2 = ObjectManager::IsObjectInHeap(Ptr_Base);
	if (temp2 == true)
	{
		return true;
	}

	auto temp3 = ObjectManager::IsObjectInUnknowPlace(Ptr_Base);
	if (temp3 == true)
	{
		return true;
	}

	return false;
}

unsigned long long int  ObjectManager::GetObjectTotalNumber()
{
	return (unsigned long long int(m_ObjectInfoMap.size())) + ObjectManager::GetHeapObjectTotalNumber() + ObjectManager::GetStackObjectTotalNumber();
}

// this function is thread safe with mutex
bool ObjectManager::RegisterHeapObject(ObjectBase* Ptr_Base)
{	
	// the lock() should be here if an iterater will be invalid after a record is added or deleted.
	// e.g., right after get an iterater "it" using m_HeapObjectInfoMap.find in thread_A, a record is deleted in thread_B
	// then the iterater "it" is no longer valid.
	// from the  test, the iterater is still valid after adding or deleting record

	// An object is automaticly registered in ObjectBase, so it can only be  registered once.
	// do not use Register in other place

	// in conclusion, put the lock here just to be safe
	m_Heap_Register_Mutex.lock();

	MDK_DEBUG_COUT("Try to Register a heap object @ ObjectManager::RegisterHeapObject()" << std::endl);

	// if Ptr_Base has been registered, then erase its record
	//  

	auto it = m_HeapObjectInfoMap.find(Ptr_Base);

	if (it == m_HeapObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("Register the heap object " << Ptr_Base->GetClassName() << " @ ObjectManager::RegisterHeapObject()" << std::endl);

		ObjectManager::UnRegister(Ptr_Base);

		m_HeapObjectInfoMap[Ptr_Base] = new ObjectBaseInfo(Ptr_Base);

		m_Heap_Register_Mutex.unlock();

		return true;
	}

	MDK_DEBUG_COUT("Can not Register the heap object @ ObjectManager::RegisterHeapObject()" << std::endl);

	m_Heap_Register_Mutex.unlock();

	return false;
}

// this function is thread safe with mutex
bool ObjectManager::UnRegisterHeapObject(ObjectBase* Ptr_Base)
{		
	// The lock must be here.
	// e.g.: if two threads try to UnRegister the same object, they both can find the same iterator
	// then erase the same record twice and case error !
	// Can this happen ?  
	// yes, it can happen: if you use new and delete. After new an object, you may delete the object many times
	
	m_Heap_UnRegister_Mutex.lock();

	auto it = m_HeapObjectInfoMap.find(Ptr_Base);

	if (it != m_HeapObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("UnRegister a heap object " << it->second->m_ObjectName << " @ ObjectManager::UnRegisterHeapObject()" << std::endl);
		
		// delete the record
		delete it->second;
		m_HeapObjectInfoMap.erase(it);

		m_Heap_UnRegister_Mutex.unlock();

		return true;
	}

	m_Heap_UnRegister_Mutex.unlock();

	return false;
}

// this function is thread safe : only read
unsigned long long int ObjectManager::GetHeapObjectTotalNumber()
{		
	return unsigned long long int(m_HeapObjectInfoMap.size());
}

std::shared_ptr<ObjectBase> ObjectManager::CreateSharedPointerToHeapObjectInRecord(ObjectBase* Ptr_Base)
{
	MDK_DEBUG_COUT("try to Create a Shared Pointer to a heap object" << std::endl);

	auto it = m_HeapObjectInfoMap.find(Ptr_Base);

	if (it != m_HeapObjectInfoMap.end())
	{
		if (it->second->m_WeakPointerToObject.expired() == false) // Assume the object is created by using mdk::Object<T>::New()
		{
			return it->second->m_WeakPointerToObject.lock();
		}
		else // Assume the object is created by using mdk::Object<T>::NewRaw()
		{
			std::shared_ptr<ObjectBase> sp(Ptr_Base);

			it->first->m_WeakPointerToSelf = sp; // update the weak pointer stored in the object
			it->second->m_WeakPointerToObject = sp; // update the weak pointer stored in its ObjectBaseInfo

			return  sp;
		}
	}

	std::shared_ptr<ObjectBase> sp_empty;
	return sp_empty;
}

// this function is thread safe with mutex
bool ObjectManager::RegisterStackObject(ObjectBase* Ptr_Base)
{
	m_Stack_Register_Mutex.lock();

	MDK_DEBUG_COUT("Try to Register a stack object @ ObjectManager::RegisterStackObject()" << std::endl);

	auto it = m_StackObjectInfoMap.find(Ptr_Base);

	if (it == m_StackObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("Register the stack object " << Ptr_Base->GetClassName() << " @ ObjectManager::RegisterStackObject()" << std::endl);

		ObjectManager::UnRegister(Ptr_Base);

		m_StackObjectInfoMap[Ptr_Base] = new ObjectBaseInfo(Ptr_Base);

		m_Stack_Register_Mutex.unlock();

		return true;
	}

	MDK_DEBUG_COUT("Can not Register the stack object @ ObjectManager::RegisterStackObject()" << std::endl);

	m_Stack_Register_Mutex.unlock();

	return false;
}

// this function is thread safe with mutex
bool ObjectManager::UnRegisterStackObject(ObjectBase* Ptr_Base)
{
	m_Stack_UnRegister_Mutex.lock();

	auto it = m_StackObjectInfoMap.find(Ptr_Base);

	if (it != m_StackObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("UnRegister a stack object " << it->second->m_ObjectName << " @ ObjectManager::UnRegisterStackObject()" << std::endl);

		// delete the record
		delete it->second;
		m_StackObjectInfoMap.erase(it);

		m_Stack_UnRegister_Mutex.unlock();

		return true;
	}

	m_Stack_UnRegister_Mutex.unlock();

	return false;
}

// this function is thread safe : only read
unsigned long long int ObjectManager::GetStackObjectTotalNumber()
{
	return unsigned long long int(m_StackObjectInfoMap.size());
}

// delete the objects in heap that are no longer in use.
bool ObjectManager::CollectHeapGarbage()
{
	m_CollectHeapGarbage_Mutex.lock();

	MDK_DEBUG_COUT("delete and unregister the heap objects that are no longer in use @ ObjectManager::CollectGarbage()" << std::endl);

	std::vector<mdk::ObjectBase*>  BasePointerList;

	/*
	for (auto it = m_HeapObjectInfoMap.begin(); it != m_HeapObjectInfoMap.end(); it++)
	{
		if (it->second->m_WeakPointerToObject.expired())
		{
			BasePointerList.push_back(it->first);
		}
	}

	for (auto itp = BasePointerList.begin(); itp != BasePointerList.end(); itp++)
	{
		delete *itp;
	}
	*/

	auto it = m_HeapObjectInfoMap.begin();

	while (it != m_HeapObjectInfoMap.end())
	{
		// if the object is created by using mdk::Object<T>::NewRaw()
		// then it should be collected by calling CollectHeapGarbage (or DeleteHeapObject if you know the raw pointer)
		if (it->second->m_WeakPointerToObject.expired()) 
		{
			it->first->m_IsRecordBeingDeletedByOtherFunction = true; // no need to unregister again

			delete it->second;
			delete it->first;
			m_HeapObjectInfoMap.erase(it++);
		}
		else
		{
			++it;
		}		
	}

	m_CollectHeapGarbage_Mutex.unlock();

	return true;
}

bool ObjectManager::DeleteHeapObject(ObjectBase* Ptr_Base)
{
	MDK_DEBUG_COUT("try to delete and unregiser a heap object @ ObjectManager::DeleteHeapObject()" << std::endl);

	m_DeleteHeapObject_Mutex.lock();

	auto it = m_HeapObjectInfoMap.find(Ptr_Base);

	if (it != m_HeapObjectInfoMap.end())
	{
		// if the object is created by using mdk::Object<T>::NewRaw() and you know the raw pointer, i.e., Ptr_Base
		// then it can be deleted by calling DeleteHeapObject
		if (it->second->m_WeakPointerToObject.expired())
		{
			MDK_DEBUG_COUT("delete and unregiser the heap object " << Ptr_Base->GetClassName() << " @ ObjectManager::DeleteHeapObject()" << std::endl);

			it->first->m_IsRecordBeingDeletedByOtherFunction = true; // no need to unregister again

			delete it->second;
			delete it->first;
			m_HeapObjectInfoMap.erase(it);

			m_DeleteHeapObject_Mutex.unlock();

			return true;			
		}
		
		MDK_DEBUG_COUT("can not delete : the heap object " << Ptr_Base->GetClassName() << " is still in use @ ObjectManager::DeleteHeapObject()" << std::endl);
	}

	MDK_DEBUG_COUT("can not delete : Object is not in heap" << std::endl);

	m_DeleteHeapObject_Mutex.unlock();

	return false;
}

bool ObjectManager::IsObjectInHeap(ObjectBase * Ptr_Base)
{
	auto it = m_HeapObjectInfoMap.find(Ptr_Base);

	if (it != m_HeapObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("the object " << Ptr_Base->GetClassName() << " is in heap" << std::endl);

		return true;
	}

	return false;
}

bool ObjectManager::IsObjectInStack(ObjectBase * Ptr_Base)
{
	auto it = m_StackObjectInfoMap.find(Ptr_Base);

	if (it != m_StackObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("the object " << Ptr_Base->GetClassName() << " is in stack" << std::endl);

		return true;
	}

	return false;
}

bool ObjectManager::IsObjectInUnknowPlace(ObjectBase* Ptr_Base)
{
	auto it = m_ObjectInfoMap.find(Ptr_Base);

	if (it != m_ObjectInfoMap.end())
	{
		MDK_DEBUG_COUT("the object " << Ptr_Base->GetClassName() << " is in Unknow Place" << std::endl);

		return true;
	}

	return false;
}

bool ObjectManager::IsHeapObjectBeingShared(ObjectBase * Ptr_Base)
{
	auto it = m_HeapObjectInfoMap.find(Ptr_Base);

	if (it != m_HeapObjectInfoMap.end())
	{
		if (it->second->m_WeakPointerToObject.expired())
		{
			return false;
		}
		else
		{
			return true;
		}		
	}

	return false;
}

void ObjectManager::PrintSelf()
{
	std::cout << "The number of objects in heap = " << m_HeapObjectInfoMap.size() <<std::endl;
	std::cout << "The number of objects in stack = " << m_StackObjectInfoMap.size() << std::endl;
	std::cout << "The number of objects in unknow place = " << m_ObjectInfoMap.size() << std::endl;
}

}//end namespace mdk