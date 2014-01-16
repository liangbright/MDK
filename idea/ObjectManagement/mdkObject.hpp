#ifndef __mdkObject_hpp
#define __mdkObject_hpp

#include <iostream>
#include <string>
#include <memory>

#include <mdkObject.h>
#include <mdkObjectBase.h>
#include <mdkObjectManager.h>

#include <mdkDebugMarco.h>

namespace mdk
{

template <typename T>
Object<T>::Object()
{		
	MDK_DEBUG_COUT("Welcome @ Object(): You should not call Object()" << std::endl);
}

template <typename T>
Object<T>::~Object()
{	
	MDK_DEBUG_COUT("Bye bye @ ~Object(): You should not call ~Object()" << std::endl);
}

template<typename T>
std::shared_ptr<T> Object<T>::New()
{			
	MDK_DEBUG_COUT("Create a heap object and return its shared pointer" << std::endl);

	auto rp = new T();

	auto rp_ObjectBase = dynamic_cast<ObjectBase *>(rp);

	auto sp_ObjectBase = ObjectManager::CreateTheFirstSharedPointerToHeapObject(rp_ObjectBase);

	auto sp = std::dynamic_pointer_cast<T, ObjectBase>(sp_ObjectBase);

	ObjectManager::WriteBasicInfoToHeapObject(rp_ObjectBase);
		
	ObjectManager::RegisterHeapObject(rp_ObjectBase);

	return sp;
}

template<typename T>
T* Object<T>::NewRaw()
{
	MDK_DEBUG_COUT("Create a heap object and return its raw pointer " << std::endl);

	auto rp = new T();		

	auto rp_ObjectBase = dynamic_cast<ObjectBase *>(rp);

	ObjectManager::WriteBasicInfoToHeapObject(rp_ObjectBase);
	
	ObjectManager::RegisterHeapObject(rp_ObjectBase);

	return rp;
}

template<typename T>
bool Object<T>::Delete(T* TObject)
{	
	return ObjectManager::DeleteHeapObject(dynamic_cast<ObjectBase *>(TObject));
}

template<typename T>
bool Object<T>::CollectGarbage()
{
	return ObjectManager::CollectHeapGarbage();
}

template<typename T>
bool Object<T>::IsInHeap(T* TObject)
{
	return ObjectManager::IsObjectInHeap(dynamic_cast<ObjectBase *>(TObject));
}

template<typename T>
bool Object<T>::IsBeingShared(T* TObject)
{
	return ObjectManager::IsHeapObjectBeingShared(dynamic_cast<ObjectBase *>(TObject));
}

template<typename T>
bool Object<T>::IsValid(T* TObject)
{		
	return ObjectManager::IsObjectValid(dynamic_cast<ObjectBase *>(TObject));
}

template <typename T>
std::shared_ptr<T>  Object<T>::CreateSharedPointer(T* TObject)
{			
	auto sp_ObjectBase = ObjectManager::CreateSharedPointerToHeapObjectInRecord(dynamic_cast<ObjectBase *>(TObject));

	return std::dynamic_pointer_cast<T, ObjectBase>(sp_ObjectBase);
}

template <typename T>
std::weak_ptr<T>  Object<T>::CreateWeakPointer(T* TObject)
{	
	auto sp = Object<T>::CreateSharedPointer(TObject);

	std::weak_ptr<T> wp = sp;

	return wp;
}

}//end namespace mdk

#endif