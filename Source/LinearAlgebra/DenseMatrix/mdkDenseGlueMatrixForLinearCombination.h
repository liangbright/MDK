#ifndef __mdkDenseGlueMatrixForLinearCombination_h
#define __mdkDenseGlueMatrixForLinearCombination_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
//#include "mdkDenseMatrix.h"

namespace mdk
{

//forward-declare -----------//
template<typename ElementType>
class mdkDenseMatrix;

template<typename ElementType>
class mdkDenseShadowMatrix;

template<typename ElementType>
class DenseGlueMatrixForMultiplication;

struct mdkMatrixSize;
// end of forward-declare //

//---------------------------------------------------------------------------------------------------------------------------------//

// reserve the capacity of m_SourceMatrixSharedCopyList
#define MDK_DenseGlueMatrixForLinearCombination_ReservedCapacity  20
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkDenseGlueMatrixForLinearCombination : public mdkObject
{
private:

	int64 m_RowNumber;

	int64 m_ColNumber;

    std::vector<mdkDenseMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    std::vector<ElementType> m_ElementList_Coef;

    ElementType m_IndependentElement;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkDenseGlueMatrixForLinearCombination();

public:
    inline ~mdkDenseGlueMatrixForLinearCombination();

private:
    inline mdkDenseGlueMatrixForLinearCombination(const mdkDenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix) = delete;

public:
    inline mdkDenseGlueMatrixForLinearCombination(mdkDenseGlueMatrixForLinearCombination<ElementType>&& DenseGlueMatrix);

    //--------------------------- operator= deleted ----------------------------------------//
private:
    void operator=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix) = delete;
    void operator=(mdkDenseGlueMatrixForLinearCombination<ElementType>&& DenseGlueMatrix) = delete;

	//---------------------- Other ----------------------------------------//
private:

    inline int64 GetRowNumber() const;

    inline int64 GetColNumber() const;

    inline mdkMatrixSize GetSize() const;

    inline int64 GetMatrixNumber() const;

    inline bool IsEmpty() const;

    inline mdkDenseMatrix<ElementType> CreateDenseMatrix() const;
	
    inline bool CreateDenseMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const;

public:

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseMatrix<ElementType>& targetMatrix);

    inline mdkDenseMatrix<ElementType> ElementMultiply(const ElementType& Element);

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix);

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix);

private:
//--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename E_TYPE>
    friend class mdkDenseMatrix;

    template<typename E_TYPE>
    friend class mdkDenseShadowMatrix;
//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    // ----------------------------------------------------- Matrix  {+ -}  Matrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend inline mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<ElementType> operator-(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------------- Matrix  {+ -}  Element ------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    // --------------------------------------------------- Element {+ -} Matrix  ---------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementB, const mdkDenseMatrix<E_TYPE>& MatrixA);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementB, const mdkDenseMatrix<E_TYPE>& MatrixA);

    // --------------------------------------------- DenseGlueMatrixForMultiplication {+ -}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ -} DenseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseMatrix<E_TYPE>& MatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseMatrix<E_TYPE>& MatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication  {+ -}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ---------------------------------------------- Element {+ -} DenseGlueMatrixForMultiplication -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication {+ -}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ -} DenseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // --------------------------------------------- DenseGlueMatrixForLinearCombination {+ - * /}  Matrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ - * /} DenseGlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseMatrix<E_TYPE>& MatrixA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseMatrix<E_TYPE>& MatrixA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------------------- DenseGlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator*(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator/(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ----------------------------------------------- Element {+ - * /} DenseGlueMatrixForLinearCombination ----------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator*(const E_TYPE& ElementA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const E_TYPE& ElementA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} DenseGlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // --------------------------------------------- DenseGlueMatrixForLinearCombination {+ - * /}  DenseGlueMatrixForLinearCombination ---------------------------------------//

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------------ DenseGlueMatrixForLinearCombination {+ - * /} DenseGlueMatrixForMultiplication -------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB_M);

    // -------------------------------------------- DenseGlueMatrixForMultiplication {+ - * /}  DenseGlueMatrixForLinearCombination -----------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA_M, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    // --------------------------------------------- DenseGlueMatrixForMultiplication {+ -}  DenseGlueMatrixForMultiplication ---------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);
    
};

}//end namespace mdk

#include "mdkDenseGlueMatrixForLinearCombination.hpp"

#endif 
