#ifndef __mdkSparseGlueMatrixForLinearCombination_h
#define __mdkSparseGlueMatrixForLinearCombination_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
//#include "mdkSparseMatrix.h"

namespace mdk
{

//forward-declare -----------//
template<typename ElementType>
class mdkSparseMatrix;

template<typename ElementType>
class mdkSparseShadowMatrix;

template<typename ElementType>
class SparseGlueMatrixForMultiplication;

struct mdkMatrixSize;
// end of forward-declare //

//---------------------------------------------------------------------------------------------------------------------------------//

// 100 scalars and 100 matrix should be OK
// reserve the capacity of std::vector
#define MDK_SparseGlueMatrixForLinearCombination_ReservedCapacity  100
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkSparseGlueMatrixForLinearCombination : public mdkObject
{
private:

	int64 m_RowNumber;

	int64 m_ColNumber;

    std::vector<mdkSparseMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    std::vector<ElementType> m_ElementList_Coef;

    ElementType m_IndependentElement;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkSparseGlueMatrixForLinearCombination();

public:
    inline ~mdkSparseGlueMatrixForLinearCombination();

private:
    inline mdkSparseGlueMatrixForLinearCombination(const mdkSparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix) = delete;

public:
    inline mdkSparseGlueMatrixForLinearCombination(mdkSparseGlueMatrixForLinearCombination<ElementType>&& SparseGlueMatrix);

    //--------------------------- operator= deleted ----------------------------------------//
private:
    void operator=(const mdkSparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix) = delete;
    void operator=(mdkSparseGlueMatrixForLinearCombination<ElementType>&& SparseGlueMatrix) = delete;

	//---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline int64 GetRowNumber() const;

    inline int64 GetColNumber() const;

    inline mdkMatrixSize GetSize() const;

    inline int64 GetMatrixNumber() const;

    inline bool IsEmpty() const;

    inline mdkSparseMatrix<ElementType> CreateSparseMatrix() const;
	
    inline bool CreateSparseMatrix(mdkSparseMatrix<ElementType>& OutputMatrix) const;

    inline mdkDenseMatrix<ElementType> CreateDenseMatrix() const;

    inline bool CreateDenseMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const;

public:

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkSparseMatrix<ElementType>& targetMatrix);

    inline mdkSparseMatrix<ElementType> ElementMultiply(const ElementType& Element);

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkSparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix);

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkSparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix);

private:
//--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename E_TYPE>
    friend class mdkSparseMatrix;

    template<typename E_TYPE>
    friend class mdkSparseShadowMatrix;
//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    // ------------------------------------- SparseMatrix  {+ -}  SparseMatrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend inline mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<ElementType> operator-(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    // ------------------------------------------------ SparseMatrix  {+ -}  Element ------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator+(const mdkSparseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator-(const mdkSparseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    // ------------------------------------------------ Element {+ -} SparseMatrix  ---------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator+(const E_TYPE& ElementB, const mdkSparseMatrix<E_TYPE>& MatrixA);

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator-(const E_TYPE& ElementB, const mdkSparseMatrix<E_TYPE>& MatrixA);

    // ----------------------------- SparseGlueMatrixForMultiplication {+ -}  SparseMatrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    // ---------------------------- SparseMatrix or {+ -} SparseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseMatrix<E_TYPE>& MatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseMatrix<E_TYPE>& MatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // -------------------------------------------- SparseGlueMatrixForMultiplication  {+ -}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ---------------------------------------------- Element {+ -} SparseGlueMatrixForMultiplication -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // ---------------------------- SparseGlueMatrixForMultiplication {+ -}  SparseShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // -------------------------------- SparseShadowMatrix {+ -} SparseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // ------------------------------- SparseGlueMatrixForLinearCombination {+ - *}  SparseMatrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    // --------------------------------- SparseMatrix or {+ - *} SparseGlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseMatrix<E_TYPE>& MatrixA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseMatrix<E_TYPE>& MatrixA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------------------- SparseGlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator*(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator/(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ----------------------------------------------- Element {+ - *} SparseGlueMatrixForLinearCombination ----------------------------------------------//

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator*(const E_TYPE& ElementA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    // ------------------------------- SparseGlueMatrixForLinearCombination {+ - *}  SparseShadowMatrix -----------------------------------------------------//

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------ SparseShadowMatrix {+ - *} SparseGlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ---------------------------- SparseGlueMatrixForLinearCombination {+ - *}  SparseGlueMatrixForLinearCombination ---------------------------------------//

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // --------------------------- SparseGlueMatrixForLinearCombination {+ - *} SparseGlueMatrixForMultiplication -------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    // --------------------------- SparseGlueMatrixForMultiplication {+ - *}  SparseGlueMatrixForLinearCombination -----------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    // --------------------------------------------- SparseGlueMatrixForMultiplication {+ -}  SparseGlueMatrixForMultiplication ---------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);
    
};

}//end namespace mdk

#include "mdkSparseGlueMatrixForLinearCombination.hpp"

#endif 
