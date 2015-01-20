#ifndef mdk_DenseGlueMatrixForLinearCombination_h
#define mdk_DenseGlueMatrixForLinearCombination_h

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
class DenseMatrix;

template<typename ElementType>
class DenseShadowMatrix;

template<typename ElementType>
class DenseGlueMatrixForMultiplication;

struct MatrixSize;
// end of forward-declare //

//---------------------------------------------------------------------------------------------------------------------------------//

// reserve the capacity of m_SourceMatrixSharedCopyList
#define MDK_DenseGlueMatrixForLinearCombination_ReservedCapacity  10
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class DenseGlueMatrixForLinearCombination : public Object
{
private:

	int_max m_RowNumber;

	int_max m_ColNumber;

    std::vector<DenseMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    std::vector<ElementType> m_ElementList_Coef;

    ElementType m_IndependentElement;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline DenseGlueMatrixForLinearCombination();

public:
    inline ~DenseGlueMatrixForLinearCombination();

private:
    inline DenseGlueMatrixForLinearCombination(const DenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix) = delete;

public:
    inline DenseGlueMatrixForLinearCombination(DenseGlueMatrixForLinearCombination<ElementType>&& DenseGlueMatrix);

    //--------------------------- operator= deleted ----------------------------------------//
private:
    void operator=(const DenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix) = delete;
    void operator=(DenseGlueMatrixForLinearCombination<ElementType>&& DenseGlueMatrix) = delete;

	//---------------------- Other ----------------------------------------//
private:

    inline int_max GetRowNumber() const;

    inline int_max GetColNumber() const;

    inline MatrixSize GetSize() const;

    inline int_max GetMatrixNumber() const;

    inline bool IsEmpty() const;

    inline DenseMatrix<ElementType> CreateDenseMatrix() const;
	
    inline bool CreateDenseMatrix(DenseMatrix<ElementType>& OutputMatrix) const;

public:

    inline DenseMatrix<ElementType> ElementMultiply(const DenseMatrix<ElementType>& targetMatrix);

    inline DenseMatrix<ElementType> ElementMultiply(const ElementType& Element);

    inline DenseMatrix<ElementType> ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline DenseMatrix<ElementType> ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix);

    inline DenseMatrix<ElementType> ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix);

private:
//--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename E_TYPE>
    friend class DenseMatrix;

    template<typename E_TYPE>
    friend class DenseShadowMatrix;
//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    // ----------------------------------------------------- Matrix  {+ -}  Matrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseMatrix<E_TYPE>& MatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseMatrix<E_TYPE>& MatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------------- Matrix  {+ -}  Element ------------------------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    // --------------------------------------------------- Element {+ -} Matrix  ---------------------------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementB, const DenseMatrix<E_TYPE>& MatrixA);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementB, const DenseMatrix<E_TYPE>& MatrixA);

    // --------------------------------------------- DenseGlueMatrixForMultiplication {+ -}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ -} DenseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseMatrix<E_TYPE>& MatrixA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseMatrix<E_TYPE>& MatrixA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication  {+ -}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ---------------------------------------------- Element {+ -} DenseGlueMatrixForMultiplication -----------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication {+ -}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ -} DenseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // --------------------------------------------- DenseGlueMatrixForLinearCombination {+ - * /}  Matrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ - * /} DenseGlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseMatrix<E_TYPE>& MatrixA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseMatrix<E_TYPE>& MatrixA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseMatrix<E_TYPE>& MatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseMatrix<E_TYPE>& MatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------------------- DenseGlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator*(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator/(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ----------------------------------------------- Element {+ - * /} DenseGlueMatrixForLinearCombination ----------------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator*(const E_TYPE& ElementA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const E_TYPE& ElementA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} DenseGlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // --------------------------------------------- DenseGlueMatrixForLinearCombination {+ - * /}  DenseGlueMatrixForLinearCombination ---------------------------------------//

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------------ DenseGlueMatrixForLinearCombination {+ - * /} DenseGlueMatrixForMultiplication -------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, const DenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB_M);

    // -------------------------------------------- DenseGlueMatrixForMultiplication {+ - * /}  DenseGlueMatrixForLinearCombination -----------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA_M, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    // --------------------------------------------- DenseGlueMatrixForMultiplication {+ -}  DenseGlueMatrixForMultiplication ---------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);
    
};

}//end namespace mdk

#include "mdkDenseGlueMatrixForLinearCombination.hpp"

#endif 
