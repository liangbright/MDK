#ifndef mdk_SparseVector_h
#define mdk_SparseVector_h

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>
#include <cmath>
#include <algorithm>

//#include "mdkObject.h"
#include "mdkMatrix_Common.h"
#include "mdkDenseVector.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// This is a memory efficient implementation of Sparse Vector
// It is used in sparse coding
// It can be used as Pixel in Image: some pixel can be pure empty to save memory

//forward declare --------------------------
//template<typename ElementType>
//class DenseMatrix;
//----------------------------------------

//-----------------------------------------------------------------------------------------------//
#if defined MDK_DEBUG_MODE
    #define  MDK_DEBUG_SparseVector_Operator_CheckBound
#endif
//-----------------------------------------------------------------------------------------------//

template<typename ElementType>
struct SparseVectorData
{
	int_max Length;                      // length of the vector

	DenseVector<int_max> IndexList;      // RowIndexList if it is a row vector
										 // ColIndexList if it is a col vector

	DenseVector<ElementType> ElementList;  // store element with the order in IndexList

	ElementType ZeroElement;

	SparseVectorData()
	{
		Length = 0;
		ZeroElement = ElementType(0);
	}

	~SparseVectorData() {}
};


template<typename Element_Type>
class SparseVector : public Object
{
public:
	typedef Element_Type ElementType;

private:
	std::unique_ptr<SparseVectorData<ElementType>> m_Data;

public:

    inline SparseVector();

	inline SparseVector(const MDK_Symbol_PureEmpty&);

	inline SparseVector(const ElementType& Element);

    inline SparseVector(const SparseVector& InputVector);
     
    inline SparseVector(SparseVector&& InputVector);

    inline ~SparseVector();

    inline void Initialize(int_max Length = 0); // all zero

    inline bool Initialize(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& ElementList, int_max Length);

    inline bool Initialize(const std::vector<int_max>& IndexList, const std::vector<ElementType>& ElementList, int_max Length);

    inline bool Initialize(const DenseVector<int_max>& IndexList, const DenseVector<ElementType>& ElementList, int_max Length);

    inline bool Initialize(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& ElementList, int_max Length);

	inline bool Initialize(const DenseVector<int_max>& IndexList, const DenseMatrix<ElementType>& ElementList, int_max Length);

	inline bool Initialize(const DenseMatrix<int_max>& IndexList, const DenseVector<ElementType>& ElementList, int_max Length);

    inline bool Initialize(const int_max* IndexList, const ElementType* ElementList, int_max RecordedElementNumber, int_max Length);

    inline void InitializeFromSortedData(std::vector<int_max> IndexList, std::vector<ElementType> ElementList, int_max Length);

	inline void InitializeFromSortedData(DenseVector<int_max> IndexList, DenseVector<ElementType> ElementList, int_max Length);

    inline void operator=(const SparseVector& InputVector);

    inline void operator=(SparseVector&& InputVector);

    //---------------------------------------------------------------------

    inline void Clear();

	inline void Clear(const MDK_Symbol_PureEmpty&); // m_Data.reset(), set m_Data to be empty

    inline void Resize(int_max InputLength);

    inline void FastResize(int_max InputLength);

	inline bool IsEmpty() const; // vector length is 0 or pure empty

	inline bool IsPureEmpty() const; // m_Data is empty

    //------------------------------------------

    inline const ElementType& operator[](int_max Index) const;

    inline const ElementType& operator()(int_max Index) const;

	inline const ElementType& GetElement(int_max Index) const;

	inline bool SetElement(int_max Index, const ElementType& Element);

    //-----------------------------------------

    inline void Copy(const SparseVector& InputVector);

    inline void Take(SparseVector& InputVector);

    //------------------------------------------

    inline int_max GetLength() const;

    inline int_max GetRecordedElementCount() const;

    inline ElementType* GetPointerOfBeginElement();

    inline const ElementType* GetPointerOfBeginElement() const;

	inline DenseVector<int_max>& IndexList();

	inline const DenseVector<int_max>& IndexList() const;

	inline DenseVector<ElementType>& ElementList();

	inline const DenseVector<ElementType>& ElementList() const;

    //------------------------------------------
	DenseMatrix<ElementType> ConvertToDenseMatrixAsColVector() const;
	DenseMatrix<ElementType> ConvertToDenseMatrixAsRowVector() const;
	DenseVector<ElementType> ConvertToDenseVector() const;
	//------------------------------------------

    inline ElementType Sum() const;

    inline ElementType L1Norm() const;

    inline ElementType L2Norm() const;
};


}//namespace mdk

#include "mdkSparseVector.hpp"

#endif
