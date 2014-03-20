#ifndef __mdkDenseGlueMatrixForMultiplication_h
#define __mdkDenseGlueMatrixForMultiplication_h

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
//#include "mdkDenseMatrix.h"


namespace mdk
{

//------------------------------ forward-declare -----------//

template<typename ElementType>
class mdkDenseMatrix;

template<typename ElementType>
class mdkDenseShadowMatrix;

template<typename ElementType>
class mdkDenseGlueMatrixForLinearCombination;

struct mdkMatrixSize;
//--------------------------- end of forward-declare -------//

//---------------------------------------------------------------------------------------------------//
// 10 matrix should be OK
// reserve the capacity of std::vector<mdkDenseMatrix<ElementType>>
#define MDK_DenseGlueMatrixForMultiplication_ReservedCapacity  10
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkDenseGlueMatrixForMultiplication : public mdkObject
{
private:

    int64 m_RowNumber;

    int64 m_ColNumber;

    std::vector<mdkDenseMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    ElementType m_Element_Coef;

    bool m_Is_m_Element_Coef_Equal_to_One;

    //------------------- constructor and destructor ------------------------------------//
private:
    inline mdkDenseGlueMatrixForMultiplication();

public:
    inline ~mdkDenseGlueMatrixForMultiplication();

private:
    inline mdkDenseGlueMatrixForMultiplication(const mdkDenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix) = delete;

public:
    inline mdkDenseGlueMatrixForMultiplication(mdkDenseGlueMatrixForMultiplication<ElementType>&& DenseGlueMatrix);

    //---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline int64 GetRowNumber() const;

    inline int64 GetColNumber() const;

    inline int64 GetElementNumber() const;

    inline mdkMatrixSize GetSize() const;

    inline int64 GetMatrixNumber() const;

    inline bool IsEmpty() const;

    inline mdkDenseMatrix<ElementType> CreateMatrix() const;

    inline bool CreateMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const;

public:

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseMatrix<ElementType>& targetMatrix);

    inline mdkDenseMatrix<ElementType> ElementMultiply(const ElementType& Element);

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix);

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix);

private:
    //--------------------------- deleted ----------------------------------------//

    void operator=(const mdkDenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix) = delete;
    void operator=(mdkDenseGlueMatrixForMultiplication<ElementType>&& DenseGlueMatrix) = delete;

    //--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename E_TYPE>
    friend class mdkDenseMatrix;

    template<typename E_TYPE>
    friend class mdkDenseShadowMatrix;

    //--------------------------- friend function ----------------------------------------------------------------------------------//

    // ----------------------------------------------------- Matrix  {*}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------------- Matrix  {* /}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator/(const mdkDenseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    // --------------------------------------------------- Element {*} Matrix  ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const E_TYPE& ElementA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    // ------------------------------------------ Matrix {*}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {*}  Matrix ----------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------- ShadowMatrix {*}  ShadowMatrix -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ---------------------------------------- Matrix or {*} DenseGlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);


    // --------------------------------------- DenseGlueMatrixForLinearCombination {*}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);


    // -------------------------------------- ShadowMatrix {*} DenseGlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------- DenseGlueMatrixForLinearCombination {*}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------- DenseGlueMatrixForLinearCombination {*}  DenseGlueMatrixForLinearCombination ---------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // --------------------------------------------- DenseGlueMatrixForMultiplication {+ - * /}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ - * /} DenseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseMatrix<E_TYPE>& MatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseMatrix<E_TYPE>& MatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseMatrix<E_TYPE>& MatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication  {+ - * /}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator/(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ---------------------------------------------- Element {+ - * /} DenseGlueMatrixForMultiplication -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const E_TYPE& ElementA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const E_TYPE& ElementA, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication {+ - /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} DenseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication {+ - * /}  DenseGlueMatrixForLinearCombination -----------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA_M, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    // ------------------------------------------ DenseGlueMatrixForLinearCombination {+ - * /} DenseGlueMatrixForMultiplication -------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB_M);

    // --------------------------------------------- DenseGlueMatrixForMultiplication {+ - * /}  DenseGlueMatrixForMultiplication ---------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

};


}// end namespace mdk

#include "mdkDenseGlueMatrixForMultiplication.hpp"

#endif
