//#define MDK_DEBUG_INFO_ENABLE

#ifdef MDK_DEBUG_INFO_ENABLE
#define MDK_DEBUG_COUT(x)    std::cout << x
#else
#define MDK_DEBUG_COUT(x)
#endif

#define MDK_MANAGE_STACK_OBJECT

#ifdef MDK_MANAGE_STACK_OBJECT
#define MDK_CREATE_OBJECT_IN_STACK(ObjectClass, Object)  auto Object = ObjectClass(); \
	                                                     mdk::ObjectManager::WriteBasicInfoToStackObject(&Object); \
														 mdk::ObjectManager::RegisterStackObject(&Object);
#else
#define MDK_CREATE_OBJECT_IN_STACK(ObjectClass, Object)
#endif

#define MDK_MANAGE_HEAP_OBJECT

#ifdef MDK_MANAGE_HEAP_OBJECT
#define MDK_CREATE_OBJECT_IN_HEAP(ObjectClass, ObjectRawPtr)  auto ObjectRawPtr = mdk::Object<ObjectClass>::NewRaw();                                                        
#else
#define MDK_CREATE_OBJECT_IN_HEAP(ObjectClass, ObjectRawPtr)
#endif
