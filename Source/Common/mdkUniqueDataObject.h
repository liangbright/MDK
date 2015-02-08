#ifndef MDK_UniqueDataObject_h
#define MDK_UniqueDataObject_h

#include <memory>

#include "mdkSymbol.h"
#include "mdkObject.h"

namespace mdk
{
//---------------- DataType -------------------------//
// DataType must have 8 member function:
// Copy
//     void operator=(const DataType& InputData); 
//	   bool Copy(const DataType& InputData);    
//	   bool Copy(const MDK_Symbol_Empty&);    
// Take
//     void operator=(DataType&& InputData);  
//     bool Take(DataType&& InputData);
//	   bool Take(const MDK_Symbol_Empty&);  
// Clear
//     void Clear();
// check if empty
//     bool IsEmpty() const;
//-------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------//
// A Unique Data class, e.g., SomeDataObject, may not necessarily inherit from UniqueDataObject, but
// if SomeDataObject has a member function that has the same name as a member function of UniqueDataObject,
// then, this function must behave in the same way
//------------------------------------------------------------------------------------------------------------------------//

template<typename Data_Type>
class UniqueDataObject : public Object
{
public:
	typedef Data_Type DataType;

protected:
	std::unique_ptr<DataType> m_Data;

public:
	inline UniqueDataObject();
	inline UniqueDataObject(const MDK_Symbol_PureEmpty&);
	inline UniqueDataObject(const UniqueDataObject& InputObject);
	inline UniqueDataObject(UniqueDataObject&& InputObject);
	inline virtual ~UniqueDataObject();

	inline void operator=(const UniqueDataObject& InputObject);// copy
	inline void operator=(UniqueDataObject&& InputObject);// not copy, not take

	inline bool Copy(const UniqueDataObject& InputObject);
	
	inline bool Take(UniqueDataObject& InputObject);
	inline bool Take(UniqueDataObject&& InputObject);

	inline void Swap(UniqueDataObject& InputObject);

	inline void Clear();
	inline void Clear(const MDK_Symbol_PureEmpty&);

	inline void Create();

	inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;
};

}//end namespace mdk

#include "mdkUniqueDataObject.hpp"

#endif
