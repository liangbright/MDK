#ifndef MDK_SharedDataObjectExample_h
#define MDK_SharedDataObjectExample_h

#include <vector>
#include <memory>

#include "mdkSymbol.h"
#include "mdkObject.h"

namespace mdk
{
//------------------------------------------ SharedDataObjectExample Class is a reference design --------------------------------//
// A Shared Data class, e.g., DenseMatrix, should NOT inherit from SharedDataObjectExample, but
// if this Shared Data class has a member function that has the same name as a member function of SharedDataObjectExample,
// then, this function must behave in the same way
//------------------------------------------------------------------------------------------------------------------------//

//---------------- DataType -------------------------//
// DataType must have 5 member function:
// Copy
//	   void Copy(const DataType& InputData); // same as void operator=(const DataType& InputData);   
//	   void Copy(DataType&& InputData);      // same as void operator=(DataType&& InputData);   
//	   void Copy(const MDK_Symbol_Empty&);   // same as void operator=(const MDK_Symbol_Empty&);   Copy(MDK_EMPTY_OBJECT)
// Clear
//     void Clear();
// check if empty
//     bool IsEmpty() const;
//-------------------------------------------------//

//--------------------------------------------------- Shared and ForceShare  ------------------------------------------ //
// SharedDataObjectExample A, B; A.Share(B) or A.ForceShare(B) is  m_Data (of A) = m_Data (of B);
// If B change the value of its Data, A will be changed (i.e., the data in A is the same as the data in B)
// If B change clear itself, A will be changed.
// If A change it self, B will be changed
//
// SharedDataObjectExample A, B, C; 
// A.Share(B); B.Share(C); Now, A and C are NOT shared by each other.
// A.Share(B) really means A and B share the same data
// After B.Share(C), then B is C, and the share-relation between A and B is terminated, A keep its data.
// 
// 5 way to terminate the relation A.Share(B) or A.ForceShare(B)
// (1) SharedDataObjectExample C; C.Move(A); data is taken by C, A and B become pure empty
// (2) SharedDataObjectExample C; A.Swap(C); data is take by C;
// (3) SharedDataObjectExample C; A.ForceShare(C); A is C, and B keep the data
// (4) SharedDataObjectExample C; A.Move(C); A is C, and B keep the data
// (5) A.Recreate(); A is a new SharedDataObjectExample, and B keep the data
//
// Share vs ForceShare
// A.Share(B) return true, if A can share B
// A.ForceShare(B): Share B no matter what
//------------------------------------------------------------------------------------------------------------------------//

struct Data_Of_SharedDataObjectExample
{
	std::vector<double> ScalarArray;
//--------------------------------------------	
	Data_Of_SharedDataObjectExample() {}
	~Data_Of_SharedDataObjectExample() {}

	void operator=(const Data_Of_SharedDataObjectExample& InputData)
	{
		this->Copy(InputData);
	}

	void operator=(Data_Of_SharedDataObjectExample&& InputData)
	{
		this->Copy(std::move(InputData));
	}

	void operator=(const MDK_Symbol_Empty&)
	{
		this->Copy(MDK_EMPTY_OBJECT);
	}

	void Copy(const Data_Of_SharedDataObjectExample& InputData)
	{
		ScalarArray = InputData.ScalarArray;
	}

	void Copy(Data_Of_SharedDataObjectExample&& InputData)
	{
		ScalarArray = std::move(InputData.ScalarArray);
	}

	void Copy(const MDK_Symbol_Empty&)
	{
		this->Clear();
	}

	void Clear()
	{
		ScalarArray.clear();
		ScalarArray.shrink_to_fit();
	}

	bool IsEmpty()
	{
		return ScalarArray.size() == 0;
	}
};
//========================================================================================================//

template<typename Data_Type = Data_Of_SharedDataObjectExample>
class SharedDataObjectExample : public Object
{
public:
	typedef Data_Type DataType;

protected:
	std::shared_ptr<DataType> m_Data;

public:
	inline SharedDataObjectExample();
	inline SharedDataObjectExample(const MDK_Symbol_PureEmpty&);
	inline SharedDataObjectExample(const SharedDataObjectExample& InputObject);
	inline SharedDataObjectExample(SharedDataObjectExample&& InputObject);
	inline virtual ~SharedDataObjectExample();

	inline void operator=(const SharedDataObjectExample& InputObject);
	inline void operator=(SharedDataObjectExample&& InputObject);

	inline void Copy(const SharedDataObjectExample& InputObject);
	inline void Copy(SharedDataObjectExample&& InputObject);

	inline void Share(SharedDataObjectExample& InputObject);
	inline void ForceShare(const SharedDataObjectExample& InputObject);

	inline void Swap(SharedDataObjectExample& InputObject);

	// Move may not be needed because A.Move(B) can be replaced by A.Swap(B); B.Clear();
	inline void Move(SharedDataObjectExample& InputObject);
	inline void Move(SharedDataObjectExample&& InputObject);

	inline void Clear();
	inline void Clear(const MDK_Symbol_PureEmpty&);// this may not be needed

	inline void Recreate();// this may not be needed

	inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;
	inline bool IsShared() const;
	inline bool IsSharedWith(const SharedDataObjectExample& InputObject) const;
};

}//end namespace mdk

#include "mdkSharedDataObjectExample.hpp"

#endif
