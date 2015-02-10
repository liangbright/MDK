#ifndef MDK_UniqueDataObjectExample_h
#define MDK_UniqueDataObjectExample_h

#include <memory>

#include "mdkSymbol.h"
#include "mdkObject.h"

namespace mdk
{
//-------------------------------------------UniqueDataObjectExample Class is a reference design ----------------------------------//
// A Unique Data class, e.g., SomeDataObject, should NOT necessarily inherit from UniqueDataObjectExample, but
// if this Unique Data class has a member function that has the same name as a member function of UniqueDataObjectExample,
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
class UniqueDataObjectExample : public Object
{
public:
	typedef Data_Type DataType;

protected:
	std::unique_ptr<DataType> m_Data;

public:
	inline UniqueDataObjectExample();
	inline UniqueDataObjectExample(const MDK_Symbol_PureEmpty&);
	inline UniqueDataObjectExample(const UniqueDataObjectExample& InputObject);
	inline UniqueDataObjectExample(UniqueDataObjectExample&& InputObject);
	inline virtual ~UniqueDataObjectExample();

	inline void operator=(const UniqueDataObjectExample& InputObject);
	inline void operator=(UniqueDataObjectExample&& InputObject);// same as Move

	inline bool Copy(const UniqueDataObjectExample& InputObject);	
	inline bool Copy(UniqueDataObjectExample&& InputObject);// same as Move

	inline void Swap(UniqueDataObjectExample& InputObject);

	// Move may not be needed because A.Move(B) can be replaced by A.Swap(B); B.Clear();
	inline void Move(UniqueDataObjectExample& InputObject);
	inline void Move(UniqueDataObjectExample&& InputObject);

	inline void Clear();
	inline void Clear(const MDK_Symbol_PureEmpty&);// this may not be needed

	inline void Recreate();// this may not be needed

	inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;
};

}//end namespace mdk

#include "mdkUniqueDataObjectExample.hpp"

#endif
