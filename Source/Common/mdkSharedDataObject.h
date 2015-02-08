#ifndef MDK_SharedDataObject_h
#define MDK_SharedDataObject_h

#include <memory>

#include "mdkSymbol.h"
#include "mdkObject.h"

namespace mdk
{
//---------------- DataType -------------------------//
// DataType must have 11 member function:
// Copy
//     void operator=(const DataType& InputData); // return must be void 
//	   bool Copy(const DataType& InputData);    
//	   bool Copy(const MDK_Symbol_Empty&);    
// Take
//     void operator=(DataType&& InputData); // return must be void 
//     bool Take(DataType&& InputData);
//	   bool Take(const MDK_Symbol_Empty&);  
// Share
//     bool IsReadyToShare(const DataType& InputData) const;
//     bool IsReadyToShare(const MDK_Symbol_Empty&) const;
//     bool IsSharedWith(const DataType& InputData) const;
// Clear
//     void Clear();
// check if empty
//     bool IsEmpty() const;
//-------------------------------------------------//

//--------------------------------------------------- Shared, ForceShare  ------------------------------------------ //
// SharedDataObject A, B; A.Share(B) or A.ForceShare(B) is  m_Data (of A) = m_Data (of B);
// If B change the value of its Data, A will be changed (i.e., the data in A is the same as the data in B)
// If B change clear itself, A will be changed.
// If A change it self, B will be changed
//
// SharedDataObject A, B, C; 
// A.Share(B); B.Share(C); Now, A and C are NOT shared by each other.
// A.Share(B) really means A and B share the same data
// After B.Share(C), then B is C, and the share-relation between A and B is terminated, A keep its data.
// 
// 5 way to terminate the relation A.Share(B) or A.ForceShare(B)
// (1) SharedDataObject C;  C = std::move(A);
// (2) SharedDataObject C(std::move(A));  or DenseMatrix C = std::move(A); 
// After(1) or (2), A become a pure empty SharedDataObject; if A is to be used again, then A.ForceShare(MDK_Empty);
//
// (3) SharedDataObject C; A.ForceShare(C); A is C, and B keep the data
// (4) A.ForceShare(MDK_EMPTY);  A become a new SharedDataObject
// (5) SharedDataObject C; A.Swap(C); C become A, A become C;
// (6) A.Recreate(); A is a new SharedDataObject, and B keep the data
//
// Share vs ForceShare
// A.Share(B) return true, if A can share B
// A.ForceShare(B): Share B no matter what
//------------------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------//
// A Shared Data class, e.g., SomeDataObject, may not necessarily inherit from SharedDataObject, but
// if SomeDataObject has a member function that has the same name as a member function of SharedDataObject,
// then, this function must behave in the same way
//------------------------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------//
// derived class:
// ObjectArray
//------------------------------------------------------------------------------------------------------------------------//

template<typename Data_Type>
class SharedDataObject : public Object
{
public:
	typedef Data_Type DataType;

protected:
	std::shared_ptr<DataType> m_Data;

public:
	inline SharedDataObject();
	inline SharedDataObject(const MDK_Symbol_PureEmpty&);
	inline SharedDataObject(const SharedDataObject& InputObject);
	inline SharedDataObject(SharedDataObject&& InputObject);
	inline virtual ~SharedDataObject();

	inline void operator=(const SharedDataObject& InputObject);
	inline void operator=(SharedDataObject&& InputObject);

	inline bool Copy(const SharedDataObject& InputObject);

	inline bool Share(SharedDataObject& InputObject);
	inline void ForceShare(const SharedDataObject& InputObject);
	inline void ForceShare(const MDK_Symbol_Empty&);
	
	inline bool Take(SharedDataObject& InputObject);
	inline bool Take(SharedDataObject&& InputObject);

	inline void Swap(SharedDataObject& InputObject);

	inline void Clear();
	inline void Clear(const MDK_Symbol_PureEmpty&);

	inline void Recreate();

	inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;
	inline bool IsShared() const;
	inline bool IsSharedWith(const SharedDataObject& InputObject) const;
};

}//end namespace mdk

#include "mdkSharedDataObject.hpp"

#endif
