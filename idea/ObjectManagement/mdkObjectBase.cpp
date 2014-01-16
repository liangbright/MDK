#include <iostream>

#include <mdkObjectBase.h>
#include <mdkObjectManager.h>

#include<mdkDebugMarco.h>

namespace mdk
{

ObjectBase::ObjectBase()
{
	m_IsCreatedByMdkObject = false;

	m_IsSelfInHeap = false;  // value does not matter

	m_WeakPointerToSelf.reset();

	m_IsRecordBeingDeletedByOtherFunction = false;

	// Heap Objects are registered in mdk::ObjectManager when you use mdk::Object<T>::New() or NewRaw();
	// inside  mdk::Object<T>::New() and NewRaw(), new mdk::ImageFilter is used to generate a heap object
	// Stack objects are registered in mdk::ObjectManager when you use MDK_CREATE_OBJECT_ONSTACK(ObjectClass, Object)

	// Is it necessary to use ObjectManager::Register(this) here ?  Maybe not.

	// suppose you use ObjectManager::Register(this) here, and the current object is ImageFilter inherited from ObjectBase,	

	// reason-1
	// this function ObjectBase() is called after new mdk::ImageFilter() is called or  mdk::ImageFilter() is called
	// inside this function, we do not know if the object is created in heap by using new, or in stack

	// reason-2
	// we need a function to locate the record of the object in ObjectManager
	// and then update ObjectBaseInfo::m_WeakPointerToObject, make sure that it points to ImageFilter or is nullptr
	// and that is done in mdk::Object<T>::New() or NewRaw()

	// note: 
	// this->some_virtual_function_of_ImageFilter will not work inside this function if ImageFilter is inherited from ObjectBase
	// but it will work after the ImageFilter object is constructed.
	// e.g., inside this function, this->GetClassName() will call ObjectBase::GetClassName(), not ImageFilter::GetClassName()

	// Therefore:
    // Always use mdk::Object<T>::New(),  or use MDK_CREATE_OBJECT_ONSTACK(ObjectClass, Object)


	// Now I think we need to use ObjectManager::Register(this) here
	// because if you use new and forget to use delete, then ObjectManager will know
   
	// maybe it is better to put a switch here to control if new/delete should be recoreded or not 

	ObjectManager::Register(this);
	
	MDK_DEBUG_COUT("Welcome @ ObjectBase()  this = " << this << std::endl);
}

ObjectBase::~ObjectBase()
{
	if (m_IsCreatedByMdkObject == true)
	{
		if (m_IsRecordBeingDeletedByOtherFunction == false)
		{
			if (m_IsSelfInHeap == true)
			{
				ObjectManager::UnRegisterHeapObject(this);
			}
			else
			{
				ObjectManager::UnRegisterStackObject(this);
			}
		}
	}	
	else
	{
		ObjectManager::UnRegisterUnusualObject(this);
	}

	MDK_DEBUG_COUT("Bye ~ @ ~ObjectBase()  this = " << this << std::endl);
}

std::string ObjectBase::GetClassName()
{
	return std::string("ObjectBase");
}

std::shared_ptr<ObjectBase> ObjectBase::GetSharedPointerToObjectBase()
{
	return m_WeakPointerToSelf.lock();
}

}//end namespace mdk