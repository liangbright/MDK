#ifndef mdk_SparseGlueMatrixForLinearCombination_h
#define mdk_SparseGlueMatrixForLinearCombination_h

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
class SparseMatrix;

template<typename ElementType>
class SparseShadowMatrix;

template<typename ElementType>
class SparseGlueMatrixForMultiplication;

struct MatrixSize;
// end of forward-declare //

//---------------------------------------------------------------------------------------------------------------------------------//

// 100 scalars and 100 matrix should be OK
// reserve the capacity of std::vector
#define MDK_SparseGlueMatrixForLinearCombination_ReservedCapacity  100
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class SparseGlueMatrixForLinearCombination : public Object
{
private:

	int64 m_RowNumber;

	int64 m_ColNumber;

    std::vector<SparseMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    std::vector<ElementType> m_ElementList_Coef;

    ElementType m_IndependentElement;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline SparseGlueMatrixForLinearCombination();

public:
    inline ~SparseGlueMatrixForLinearCombination();

private:
    inline SparseGlueMatrixForLinearCombination(const SparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix) = delete;

public:
    inline SparseGlueMatrixForLinearCombination(SparseGlueMatrixForLinearCombination<ElementType>&& SparseGlueMatrix);

    //--------------------------- operator= deleted ----------------------------------------//
private:
    void operator=(const SparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix) = delete;
    void operator=(SparseGlueMatrixForLinearCombination<ElementType>&& SparseGlueMatrix) = delete;

	//---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline int64 GetRowNumber() const;

    inline int64 GetColNumber() const;

    inline MatrixSize GetSize() const;

    inline int64 GetMatrixNumber() const;

    inline bool IsEmpty() const;

    inline SparseMatrix<ElementType> CreateSparseMatrix() const;
	
    inline bool CreateSparseMatrix(SparseMatrix<ElementType>& OutputMatrix) const;

    inline DenseMatrix<ElementType> CreateDenseMatrix() const;

    inline bool CreateDenseMatrix(DenseMatrix<ElementType>& OutputMatrix) const;

public:

    inline SparseMatrix<ElementType> ElementMultiply(const SparseMatrix<ElementType>& targetMatrix);

    inline SparseMatrix<ElementType> ElementMultiply(const ElementType& Element);

    inline SparseMatrix<ElementType> ElementMultiply(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline SparseMatrix<ElementType> ElementMultiply(const SparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix);

    inline SparseMatrix<ElementType> ElementMultiply(const SparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix);

private:
//--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename E_TYPE>
    friend class SparseMatrix;

    template<typename E_TYPE>
    friend class SparseShadowMatrix;
//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    // ------------------------------------- SparseMatrix  {+ -}  SparseMatrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend inline SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const SparseMatrix<E_TYPE>& MatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<ElementType> operator-(const SparseMatrix<E_TYPE>& MatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    // ------------------------------------------------ SparseMatrix  {+ -}  Element ------------------------------------------------------//

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator+(const SparseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator-(const SparseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    // ------------------------------------------------ Element {+ -} SparseMatrix  ---------------------------------------------------------//

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator+(const E_TYPE& ElementB, const SparseMatrix<E_TYPE>& MatrixA);

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator-(const E_TYPE& ElementB, const SparseMatrix<E_TYPE>& MatrixA);

    // ----------------------------- SparseGlueMatrixForMultiplication {+ -}  SparseMatrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    // ---------------------------- SparseMatrix or {+ -} SparseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const SparseMatrix<E_TYPE>& MatrixA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const SparseMatrix<E_TYPE>& MatrixA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // -------------------------------------------- SparseGlueMatrixForMultiplication  {+ -}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ---------------------------------------------- Element {+ -} SparseGlueMatrixForMultiplication -----------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // ---------------------------- SparseGlueMatrixForMultiplication {+ -}  SparseShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // -------------------------------- SparseShadowMatrix {+ -} SparseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // ------------------------------- SparseGlueMatrixForLinearCombination {+ - *}  SparseMatrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    // --------------------------------- SparseMatrix or {+ - *} SparseGlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const SparseMatrix<E_TYPE>& MatrixA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const SparseMatrix<E_TYPE>& MatrixA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseMatrix<E_TYPE>& MatrixA, const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------------------- SparseGlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator*(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator/(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ----------------------------------------------- Element {+ - *} SparseGlueMatrixForLinearCombination ----------------------------------------------//

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator*(const E_TYPE& ElementA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    // ------------------------------- SparseGlueMatrixForLinearCombination {+ - *}  SparseShadowMatrix -----------------------------------------------------//

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------ SparseShadowMatrix {+ - *} SparseGlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ---------------------------- SparseGlueMatrixForLinearCombination {+ - *}  SparseGlueMatrixForLinearCombination ---------------------------------------//

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // --------------------------- SparseGlueMatrixForLinearCombination {+ - *} SparseGlueMatrixForMultiplication -------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    // --------------------------- SparseGlueMatrixForMultiplication {+ - *}  SparseGlueMatrixForLinearCombination -----------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    // --------------------------------------------- SparseGlueMatrixForMultiplication {+ -}  SparseGlueMatrixForMultiplication ---------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);
    
};

}//end namespace mdk

#include "mdkSparseGlueMatrixForLinearCombination.hpp"

#endif 
