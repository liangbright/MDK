#ifndef MDK_UniqueDataObject_h
#define MDK_UniqueDataObject_h

#include <memory>

#include "mdkSymbol.h"
#include "mdkObject.h"

namespace mdk
{
//-------------------------------------------UniqueDataObject Class is a reference design ----------------------------------//
// A Unique Data class, e.g., SomeDataObject, may not necessarily inherit from UniqueDataObject, but
// if this Unique Data class has a member function that has the same name as a member function of UniqueDataObject,
// then, this function must behave in the same way
//------------------------------------------------------------------------------------------------------------------------//

//---------------- DataType -------------------------//
// DataType must have 5 member function:
// Copy
//     void operator=(const DataType& InputData); 
//	   bool Copy(const DataType& InputData);    
//	   bool Copy(const MDK_Symbol_Empty&);    
// Clear
//     void Clear();
// check if empty
//     bool IsEmpty() const;
//-------------------------------------------------//

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

	inline void operator=(const UniqueDataObject& InputObject);
	inline void operator=(UniqueDataObject&& InputObject);// same as Move

	inline bool Copy(const UniqueDataObject& InputObject);	
	inline bool Copy(UniqueDataObject&& InputObject);// same as Move

	inline void Swap(UniqueDataObject& InputObject);

	// Move may not be needed because A.Move(B) can be replaced by A.Swap(B); B.Clear();
	inline void Move(UniqueDataObject& InputObject);
	inline void Move(UniqueDataObject&& InputObject);

	inline void Clear();
	inline void Clear(const MDK_Symbol_PureEmpty&);// this may not be needed

	inline void Recreate();// this may not be needed

	inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;
};

}//end namespace mdk

#include "mdkUniqueDataObject.hpp"

#endif
