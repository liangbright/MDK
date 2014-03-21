#ifndef __mdkSparseGlueMatrixForMultiplication_h
#define __mdkSparseGlueMatrixForMultiplication_h

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
//#include "mdkSparseMatrix.h"


namespace mdk
{

//------------------------------ forward-declare -----------//

template<typename ElementType>
class mdkSparseMatrix;

template<typename ElementType>
class mdkSparseShadowMatrix;

template<typename ElementType>
class mdkSparseGlueMatrixForLinearCombination;

struct mdkMatrixSize;
//--------------------------- end of forward-declare -------//

//---------------------------------------------------------------------------------------------------//
// 10 matrix should be OK
// reserve the capacity of std::vector<mdkSparseMatrix<ElementType>>
#define MDK_SparseGlueMatrixForMultiplication_ReservedCapacity  10
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkSparseGlueMatrixForMultiplication : public mdkObject
{
private:

    int64 m_RowNumber;

    int64 m_ColNumber;

    std::vector<mdkSparseMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    ElementType m_Element_Coef;

    bool m_Is_m_Element_Coef_Equal_to_One;

    //------------------- constructor and destructor ------------------------------------//
private:
    inline mdkSparseGlueMatrixForMultiplication();

public:
    inline ~mdkSparseGlueMatrixForMultiplication();

private:
    inline mdkSparseGlueMatrixForMultiplication(const mdkSparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix) = delete;

public:
    inline mdkSparseGlueMatrixForMultiplication(mdkSparseGlueMatrixForMultiplication<ElementType>&& SparseGlueMatrix);

    //---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline int64 GetRowNumber() const;

    inline int64 GetColNumber() const;

    inline int64 GetElementNumber() const;

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
    //--------------------------- deleted ----------------------------------------//

    void operator=(const mdkSparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix) = delete;
    void operator=(mdkSparseGlueMatrixForMultiplication<ElementType>&& SparseGlueMatrix) = delete;

    //--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename E_TYPE>
    friend class mdkSparseMatrix;

    template<typename E_TYPE>
    friend class mdkSparseShadowMatrix;

    //--------------------------- friend function ----------------------------------------------------------------------------------//

    // ----------------------------------------------------- Matrix  {*}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------------- Matrix  {* /}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator/(const mdkSparseMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    // --------------------------------------------------- Element {*} Matrix  ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const E_TYPE& ElementA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    // ------------------------------------------ Matrix {*}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {*}  Matrix ----------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------- ShadowMatrix {*}  ShadowMatrix -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ---------------------------------------- Matrix or {*} SparseGlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);


    // --------------------------------------- SparseGlueMatrixForLinearCombination {*}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);


    // -------------------------------------- ShadowMatrix {*} SparseGlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------- SparseGlueMatrixForLinearCombination {*}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------- SparseGlueMatrixForLinearCombination {*}  SparseGlueMatrixForLinearCombination ---------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // --------------------------------------------- SparseGlueMatrixForMultiplication {+ - * /}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ - * /} SparseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseMatrix<E_TYPE>& MatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseMatrix<E_TYPE>& MatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseMatrix<E_TYPE>& MatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    // -------------------------------------------- SparseGlueMatrixForMultiplication  {+ - * /}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator/(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ---------------------------------------------- Element {+ - * /} SparseGlueMatrixForMultiplication -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const E_TYPE& ElementA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const E_TYPE& ElementA, const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- SparseGlueMatrixForMultiplication {+ - /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} SparseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- SparseGlueMatrixForMultiplication {+ - * /}  SparseGlueMatrixForLinearCombination -----------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA_M, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    // ------------------------------------------ SparseGlueMatrixForLinearCombination {+ - * /} SparseGlueMatrixForMultiplication -------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB_M);

    // --------------------------------------------- SparseGlueMatrixForMultiplication {+ - * /}  SparseGlueMatrixForMultiplication ---------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

};


}// end namespace mdk

#include "mdkSparseGlueMatrixForMultiplication.hpp"

#endif
