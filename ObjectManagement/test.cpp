#include <iostream>
#include <string>
#include <cstdlib>
#include <unordered_map>

#include <thread>

#include <mdkObject.h>
#include <mdkObjectManager.h>
#include <mdkObjectBase.h>
#include <mdkObjectBaseInfo.h>
#include <mdkImageFilter.h>

#include <mdkDebugMarco.h>


void test_Map_erase()
{

std::unordered_map<int, bool> tempMap; 

tempMap[1] = true;
tempMap[2] = false;
tempMap[3] = true;
tempMap[4] = false;
tempMap[5] = true;

auto it = tempMap.begin();

while (it != tempMap.end())
{
	if (it->second == true)
	{		
		tempMap.erase(it++);
	}
	else
	{
		++it;
	}	
}

it = tempMap.begin();

while (it != tempMap.end())
{
	std::cout << it->second << std::endl;
	++it;
}

}

void  Test_Marco_Create_Object()
{
	std::cout << "Test_Marco_Create_Object()" << std::endl;

	for (long n = 1; n <= 10; n++)
	{
		MDK_CREATE_OBJECT_IN_STACK(mdk::ImageFilter, temp_imfilter)

	    auto temp_Flag = mdk::Object<mdk::ImageFilter>::IsValid(&temp_imfilter);

		std::cout << temp_Flag << std::endl;

		temp_imfilter.SetInput(1);
	}

	std::cout << "ObjectManager::GetObjectNumber() = " << mdk::ObjectManager::GetObjectTotalNumber() << '\n';

	MDK_CREATE_OBJECT_IN_HEAP(mdk::ImageFilter, rpt_imfilter)

    rpt_imfilter->SetInput(121);

	auto sp = mdk::Object<mdk::ImageFilter>::New();


	auto rp = mdk::Object<mdk::ImageFilter>::NewRaw();

	std::cout << "ObjectManager::GetObjectNumber() = " << mdk::ObjectManager::GetObjectTotalNumber() << '\n';

	mdk::ObjectManager::CollectHeapGarbage();

	std::cout << "ObjectManager::GetObjectNumber() = " << mdk::ObjectManager::GetObjectTotalNumber() << '\n';
}

void  Test_weak_ptr()
{	
	auto rp = new mdk::ImageFilter();

	rp->SetInput(123);

	std::shared_ptr<mdk::ImageFilter> sp(rp);

	auto sp_base = std::static_pointer_cast<mdk::ObjectBase, mdk::ImageFilter>(sp);

	std::weak_ptr<mdk::ObjectBase> wp_base = sp_base;
	//=======================================================================

	auto sp1_base = wp_base.lock();

	auto temp_sp = std::static_pointer_cast<mdk::ImageFilter, mdk::ObjectBase>(sp1_base);

	std::weak_ptr<mdk::ImageFilter> temp_wp = temp_sp;

	//error
	//std::weak_ptr<mdk::ImageFilter> temp_wp = wp_base;
	//=======================================================================
	auto temp_rp = temp_sp.get();
			
	double num;
		
	temp_sp->GetOutput(&num);

	std::cout << num << std::endl;
}

void  Test_unordered_map()
{
	std::unordered_map<int, std::string> tempMap;

	tempMap[1] = "A";
	
	tempMap[2] = "B";

	auto it1 = tempMap.find(1);

	// mimic that in Thread_B , a record is deleted right after tempMap.find(1) in Thread_A
	auto it2 = tempMap.find(2);

	// mimic that in Thread_C , a record is added right after tempMap.find(1) in Thread_A
	tempMap[3] = "C";

	if (it2 != tempMap.end())
	{
		tempMap.erase(it2);

		// mimic that erase twice in two threads
		// error !
		//tempMap.erase(it2);
	}
	
	// it1 is still valid
	std::cout << it1->second << std::endl;

}

void GenerateNewObject()
{
	for (long i = 1; i <= 100; i++)
	{
		// sp_imfilter is a std::shared_ptr
		auto sp_imfilter = mdk::Object<mdk::ImageFilter>::New();

		std::cout << "ObjectManager::GetObjectNumber() = " << mdk::ObjectManager::GetObjectTotalNumber() << '\n';
	}


	auto temp = new  mdk::ImageFilter();
}

void  test_Register_Directly_in_ObjectBase()
{
	// add  ObjectManager::Register(this) in ObjectBase::ObjectBase()

	auto temp = new mdk::ImageFilter();

	std::cout << "ObjectManager::IsObjectInHeap(temp) = " << mdk::ObjectManager::IsObjectInStack(temp) << std::endl;

}



int main()
{	
	//=====================================================================================================
	//test_Register_Directly_in_ObjectBase();

	//test_Map_erase();

	//Test_unordered_map();

	//Test_Marco_Create_Object();

	//Test_weak_ptr();
	//=====================================================================================================
	// the unordered_map is big enough to hold the records of all the objects
	// max_num = 576460752303423487 ~ 2^59, machine = T420 + win7

	std::unordered_map<mdk::ObjectBase*, mdk::ObjectBaseInfo*> tempMap;
		
	auto max_num = tempMap.max_size();
	
	MDK_DEBUG_COUT("Maximum number of objects in std::unordered_map = " << max_num << '\n');
		
	std::cout << "ObjectManager::GetObjectNumber() = " << mdk::ObjectManager::GetObjectTotalNumber() << '\n';
	
	mdk::ObjectManager::PrintSelf();
	//=====================================================================================================
	std::cout << "=================== compare time cost =====================" << std::endl;
	// It only makes sense to compare time cost in release mode

	long LoopNumber = 100000000;

	std::cout << "LoopNumber = " << LoopNumber << std::endl;

	// use this whenever you can
	auto  StartTime = std::time(nullptr);
	for (long i = 1; i <= LoopNumber; i++)
	{
		// sp_imfilter is a std::shared_ptr
		auto sp_imfilter = mdk::Object<mdk::ImageFilter>::New();
	}
	auto  EndTime = std::time(nullptr);

	auto TimeCost1 = EndTime - StartTime;

	// use this only if you have to
	StartTime = std::time(nullptr);
	for (long i = 1; i <= LoopNumber; i++)
	{
		// sp_imfilter is a raw pointer to ImageFilter
		auto rp_imfilter = mdk::Object<mdk::ImageFilter>::NewRaw();
		// use Delete if NewRaw is used, and nothing bad will happen if New is used
		mdk::Object<mdk::ImageFilter>::Delete(rp_imfilter); 
	}
	EndTime = std::time(nullptr);

	auto TimeCost2 = EndTime - StartTime;

	// use this only if you have to
	StartTime = std::time(nullptr);
	for (long i = 1; i <= LoopNumber; i++)
	{
		MDK_CREATE_OBJECT_IN_STACK(mdk::ImageFilter, temp_imfilter)		
	}
	EndTime = std::time(nullptr);

	auto TimeCost3 = EndTime - StartTime;

	// if you use shared_ptr(new) or shared_ptr(raw_ptr)
	// then the mdk::Object will not work correctly
	StartTime = std::time(nullptr);
	for (long i = 1; i <= LoopNumber; i++)
	{
		std::shared_ptr<mdk::ImageFilter> sp(new mdk::ImageFilter());		
	}
	EndTime = std::time(nullptr);

	auto TimeCost4 = EndTime - StartTime;

	// if you use new and delete 
	// then mdk::Object will not work correctly
	StartTime = std::time(nullptr);
	for (long i = 1; i <= LoopNumber; i++)
	{
		auto temp = new mdk::ImageFilter();		
		delete temp;
	}
	EndTime = std::time(nullptr);

	auto TimeCost5 = EndTime - StartTime;
	
    // in stack without register
	StartTime = std::time(nullptr);
	for (long i = 1; i <= LoopNumber; i++)
	{
		auto tempObject = mdk::ImageFilter();
		tempObject.SetInput(6);
	}
	EndTime = std::time(nullptr);

	auto TimeCost6 = EndTime - StartTime;

	std::cout << "TimeCost1 = " << TimeCost1 << " : {New}" << '\n';
	std::cout << "TimeCost2 = " << TimeCost2 << " : {NewRaw and Delete}" << '\n';
	std::cout << "TimeCost3 = " << TimeCost3 << " : {MDK_Stack}" << '\n';		
	std::cout << "TimeCost4 = " << TimeCost4 << " : {shared_ptr(new)}" << '\n';
	std::cout << "TimeCost5 = " << TimeCost5 << " : new and delete" << '\n';
	std::cout << "TimeCost6 = " << TimeCost6 << " : {Stack}" << '\n';

	std::cout << "ObjectManager::GetObjectNumber() = " << mdk::ObjectManager::GetObjectTotalNumber() << '\n';
	
	mdk::ObjectManager::CollectHeapGarbage();

	std::cout << "ObjectManager::GetObjectNumber() = " << mdk::ObjectManager::GetObjectTotalNumber() << '\n';
	
	mdk::ObjectManager::PrintSelf();

	return 0;

	//=====================================================================================================		
	std::cout << "=================== test stack object =====================" << std::endl;

	mdk::ImageFilter imfilter;

	std::cout << "Create a Stack object imfilter = " << &imfilter << '\n';

	auto temp1 = mdk::Object<mdk::ImageFilter>::CreateSharedPointer(&imfilter);
	std::cout << "imfilter: temp1.use_count() = " << temp1.use_count() << '\n';

	auto temp2 = mdk::Object<mdk::ImageFilter>::CreateWeakPointer(&imfilter);
	std::cout << "imfilter temp2.use_count() = " << temp2.use_count() << '\n';

	std::cout << "delete imfilter : " << mdk::Object<mdk::ImageFilter>::Delete(&imfilter) << '\n';

	std::cout << "delete imfilter again : " << mdk::Object<mdk::ImageFilter>::Delete(&imfilter) << '\n';

	mdk::ObjectManager::PrintSelf();
	//=====================================================================================================
	std::cout << "=================== test NewRaw =====================" << std::endl;
	 
	auto rp1_imfilter = mdk::Object<mdk::ImageFilter>::NewRaw();

	std::cout << "rp1_imfilter->Delete() : " << mdk::Object<mdk::ImageFilter>::Delete(rp1_imfilter) << '\n';
	
	std::cout << "rp1_imfilter->Delete() again : " << mdk::Object<mdk::ImageFilter>::Delete(rp1_imfilter) << '\n';

	auto Flag_rp1_imfilter = mdk::Object<mdk::ImageFilter>::IsValid(rp1_imfilter);

	std::cout << "rp1_imfilter IsValid = " << Flag_rp1_imfilter << '\n';

	auto temp_imfilter = mdk::Object<mdk::ImageFilter>::CreateSharedPointer(rp1_imfilter);
	
	//------------------------------------------------------------------------------------------------
	auto rp2_imfilter = mdk::Object<mdk::ImageFilter>::NewRaw();

	{
		auto sp1_imfilter = mdk::Object<mdk::ImageFilter>::CreateSharedPointer(rp2_imfilter);
		std::cout << "mdk::Object::Delete(rp2_imfilter) " << mdk::Object<mdk::ImageFilter>::Delete(rp2_imfilter) << '\n';
	}

	//warnning: delete again !
	std::cout << "mdk::Object::Delete(rp2_imfilter)" << mdk::Object<mdk::ImageFilter>::Delete(rp2_imfilter) << '\n';

	mdk::ObjectManager::PrintSelf();
	//=====================================================================================================
	std::cout << "=================== test New =====================" << std::endl;

	auto sp_imfilter = mdk::Object<mdk::ImageFilter>::New();
	auto rp_imfilter = sp_imfilter.get();

	rp_imfilter->SetInput(7);

	{
		auto sp1 = mdk::Object<mdk::ImageFilter>::CreateSharedPointer(rp_imfilter);
	    std::cout << "sp1 " << sp1.use_count() << '\n';
    }

	{
		auto sp2 = mdk::Object<mdk::ImageFilter>::CreateSharedPointer(rp_imfilter);
		std::cout << "sp2 " << sp2.use_count() << '\n';
	}

	{
		auto wp1 = mdk::Object<mdk::ImageFilter>::CreateWeakPointer(rp_imfilter);
		std::cout << "wp1 " << wp1.use_count() << '\n';
	}

	{
		auto wp2 = mdk::Object<mdk::ImageFilter>::CreateWeakPointer(rp_imfilter);
		std::cout << "wp2 " << wp2.use_count() << '\n';
	}

	std::cout << sp_imfilter.use_count() << '\n';

	mdk::ObjectManager::PrintSelf();
	//=====================================================================================================
	// never do shared_ptr(raw_ptr), use CreateSharedPointer(raw_ptr)
	{
//		std::shared_ptr<mdk::ImageFilter> wrong_sp1(rp_imfilter);
//		std::cout << "wrong_sp1 " << wrong_sp1.use_count() << '\n';
	}
	//=====================================================================================================	
	std::cout << "=================== test mult-thread =====================" << std::endl;

	auto StartNumber = mdk::ObjectManager::GetObjectTotalNumber();

	std::cout << "Start : ObjectManager::GetObjectNumber() = " << StartNumber << '\n';
	
	std::thread t1(GenerateNewObject);
	std::thread t2(GenerateNewObject);
	std::thread t3(GenerateNewObject);
	std::thread t4(GenerateNewObject);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	std::cout << "End : ObjectManager::GetObjectNumber() = " << mdk::ObjectManager::GetObjectTotalNumber() << '\n';
	
	std::cout << "StartNum : = " << StartNumber << '\n';

	//=====================================================================================================
	mdk::ObjectManager::PrintSelf();
	
}