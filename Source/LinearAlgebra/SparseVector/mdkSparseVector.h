#ifndef __mdkSparseVector_h
#define __mdkSparseVector_h

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>
#include <cmath>
#include <algorithm>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
#include "mdkMatrix_Common.h"


namespace mdk
{

// This is a memory efficient implementation of Sparse Vector
// It is used in sparse coding

template<typename ElementType>
class SparseVector : public Object
{
private:
    int_max m_Length;                      // length of the vector

    std::vector<int_max> m_IndexList;      // RowIndexList if it is a row vector
                                           // ColIndexList if it is a col vector

    std::vector<ElementType> m_DataArray;  // store element with the order in m_IndexList

    ElementType m_ZeroElement;              // represent all the zero elements

public:

    inline SparseVector();

    inline SparseVector(const SparseVector& InputVector);
     
    inline SparseVector(SparseVector&& InputVector);

    inline ~SparseVector();

    inline void Construct(int_max Length); // all zero

    inline bool Construct(const std::initializer_list<int_max>& IndexList,
                          const std::initializer_list<ElementType>& DataArray,
                          int_max Length);

    inline bool Construct(const std::vector<int_max>& IndexList,
                          const std::vector<ElementType>& DataArray,
                          int_max Length);

    inline bool Construct(const DenseMatrix<int_max>& IndexList,
                          const DenseMatrix<ElementType>& DataArray,
                          int_max Length);

    inline bool Construct(const int_max* IndexList,
                          const ElementType* DataArray,
                          int_max RecordedElementNumber,
                          int_max Length);

    inline void ConstructFromSortedData(std::vector<int_max> IndexList, std::vector<ElementType> DataArray, int_max Length);

    inline void operator=(const SparseVector& InputVector);

    inline void operator=(SparseVector&& InputVector);

    //---------------------------------------------------------------------

    inline void Clear();

    inline void Resize(int_max InputLength);

    inline void FastResize(int_max InputLength);

    //------------------------------------------
    // note 1: [] and () have no bound check in release mode
    // note 2: there is no none const operator [] and ()

    inline const ElementType& operator[](int_max Index) const;

    inline const ElementType& operator()(int_max Index) const;

    //------------------------------------------
    // note: output ElementType(0) if no record exits in the input location

    inline const ElementType& GetElement(int_max Index) const;

    inline bool SetElement(int_max Index, const ElementType& Element);

    //-----------------------------------------

    inline void Copy(const SparseVector& InputVector);

    inline void Take(SparseVector& InputVector);

    //------------------------------------------

    inline int_max GetLength() const;

    inline int_max GetRecordedElementNumber() const;

    inline ElementType* GetPointerOfBeginElement();

    inline const ElementType* GetPointerOfBeginElement() const;

    inline std::vector<int_max>& IndexList();

    inline const std::vector<int_max>& IndexList() const;

    inline std::vector<ElementType>& DataArray();

    inline const std::vector<ElementType>& DataArray() const;

    //------------------------------------------

    inline ElementType Sum() const;

    inline ElementType L1Norm() const;

    inline ElementType L2Norm() const;
};


}//namespace mdk

#include "mdkSparseVector.hpp"

#endif
