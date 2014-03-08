#ifndef __mdkGlueMatrixForMultiplication_h
#define __mdkGlueMatrixForMultiplication_h

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
//#include "mdkMatrix.h"

#if defined MDK_Enable_GlueMatrix //=====================================================================================

namespace mdk
{

//------------------------------ forward-declare -----------//

template<typename ElementType>
class mdkMatrix;

template<typename ElementType>
class mdkShadowMatrix;

template<typename ElementType>
class mdkGlueMatrixForLinearCombination;

struct mdkMatrixSize;
//--------------------------- end of forward-declare -------//

//---------------------------------------------------------------------------------------------------//
// 10 matrix should be OK
// reserve the capacity of std::vector<mdkMatrix<ElementType>>
#define MDK_GlueMatrixForMultiplication_ReservedCapacity  10
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkGlueMatrixForMultiplication : public mdkObject
{
private:

    uint64 m_RowNumber;

    uint64 m_ColNumber;

    std::vector<mdkMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    ElementType m_Element_Coef;

    //------------------- constructor and destructor ------------------------------------//
private:
    inline mdkGlueMatrixForMultiplication();

public:
    inline ~mdkGlueMatrixForMultiplication();

private:
    inline mdkGlueMatrixForMultiplication(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix) = delete;

public:
    inline mdkGlueMatrixForMultiplication(mdkGlueMatrixForMultiplication<ElementType>&& GlueMatrix);

    //---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline uint64 GetRowNumber() const;

    inline uint64 GetColNumber() const;

    inline uint64 GetElementNumber() const;

    inline mdkMatrixSize GetSize() const;

    inline bool IsEmpty() const;

    inline mdkMatrix<ElementType> CreateMatrix() const;

    inline void CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const;

public:

    inline mdkMatrix<ElementType> ElementMultiply(const mdkMatrix<ElementType>& targetMatrix);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

private:
    //--------------------------- deleted ----------------------------------------//

    void operator=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix) = delete;
    void operator=(mdkGlueMatrixForMultiplication<ElementType>&& GlueMatrix) = delete;

    //--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename T>
    friend class mdkMatrix;

    //--------------------------- friend function ----------------------------------------------------------------------------------//

    // ----------------------------------------------------- Matrix  {*}  Matrix ---------------------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkMatrix<TYPE>& MatrixA, const mdkMatrix<TYPE>& MatrixB);

    // --------------------------------------------- GlueMatrixForMultiplication {+ - * /}  Matrix ---------------------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(mdkGlueMatrixForMultiplication<TYPE> GlueMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkMatrix<TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkMatrix<TYPE>& MatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkMatrix<TYPE>& MatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkMatrix<TYPE>& MatrixA, mdkGlueMatrixForMultiplication<TYPE> GlueMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkMatrix<TYPE>& MatrixA, const mdkGlueMatrixForMultiplication<TYPE> GlueMatrixB);

    // -------------------------------------------- GlueMatrixForMultiplication  {+ - * /}  Element ------------------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const TYPE& ElementB);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const TYPE& ElementB);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(mdkGlueMatrixForMultiplication<TYPE> GlueMatrixA, const TYPE& ElementB);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator/(mdkGlueMatrixForMultiplication<TYPE> GlueMatrixA, const TYPE& ElementB);

    // ---------------------------------------------- Element {+ - * /} GlueMatrixForMultiplication -----------------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const TYPE& ElementA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const TYPE& ElementA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const TYPE& ElementA, mdkGlueMatrixForMultiplication<TYPE> GlueMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const TYPE& ElementA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForMultiplication {+ - /}  ShadowMatrix -----------------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(mdkGlueMatrixForMultiplication<TYPE> GlueMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkShadowMatrix<TYPE>& ShadowMatrixA, mdkGlueMatrixForMultiplication<TYPE> GlueMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForMultiplication {+ - /}  GlueMatrixForLinearCombination -----------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB_L);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB_L);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(mdkGlueMatrixForMultiplication<TYPE> GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB_L);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB_L);

    // ------------------------------------------ GlueMatrixForLinearCombination {+ - * /} GlueMatrixForMultiplication -------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB_M);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB_M);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<TYPE> GlueMatrixB_M);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA_L, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB_M);

    // --------------------------------------------- GlueMatrixForMultiplication {+ - * /}  GlueMatrixForMultiplication ---------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(mdkGlueMatrixForMultiplication<TYPE> GlueMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    //------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------------//

    template<typename TYPE>
    friend void operator+=(mdkShadowMatrix<TYPE> ShadowMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator-=(mdkShadowMatrix<TYPE> ShadowMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator*=(mdkShadowMatrix<TYPE> ShadowMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator/=(mdkShadowMatrix<TYPE> ShadowMatrixA, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB);

};


}// end namespace mdk

#include "mdkGlueMatrixForMultiplication.hpp"

#endif // MDK_Enable_GlueMatrix =====================================================================================

#endif