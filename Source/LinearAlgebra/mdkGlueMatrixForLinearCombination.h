#ifndef __mdkGlueMatrixForLinearCombination_h
#define __mdkGlueMatrixForLinearCombination_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
//#include "mdkMatrix.h"

#if defined MDK_Enable_GlueMatrix //=====================================================================================

namespace mdk
{

//forward-declare -----------//
template<typename ElementType>
class mdkMatrix;

template<typename ElementType>
class mdkShadowMatrix;

template<typename ElementType>
class GlueMatrixForMultiplication;

struct mdkMatrixSize;
// end of forward-declare //

//---------------------------------------------------------------------------------------------------------------------------------//

// 100 scalars and 100 matrix should be OK
// reserve the capacity of std::vector
#define MDK_GlueMatrixForLinearCombination_ReservedCapacity  100
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkGlueMatrixForLinearCombination : public mdkObject
{
private:

	uint64 m_RowNumber;

	uint64 m_ColNumber;

    std::vector<std::shared_ptr<std::vector<ElementType>>> m_MatrixElementDataSharedPointerList;

    std::vector<ElementType> m_ElementList_Coef;

    ElementType m_IndependentElement;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkGlueMatrixForLinearCombination();

public:
    inline ~mdkGlueMatrixForLinearCombination();

private:
    inline mdkGlueMatrixForLinearCombination(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix) = delete;

public:
    inline mdkGlueMatrixForLinearCombination(mdkGlueMatrixForLinearCombination<ElementType>&& GlueMatrix);

	//---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline uint64 GetRowNumber() const;

    inline uint64 GetColNumber() const;

    inline bool IsEmpty() const;

    inline mdkMatrix<ElementType> CreateMatrix() const;
	
    inline void CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const;

public:
    inline mdkMatrix<ElementType> ElementMultiply(const mdkMatrix<ElementType>& targetMatrix);

private:
//--------------------------- deleted ----------------------------------------//

    void operator=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix) = delete;
    void operator=(mdkGlueMatrixForLinearCombination<ElementType>&& GlueMatrix) = delete;

//--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename TYPE>
    friend class mdkMatrix;

//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    // ----------------------------------------------------- Matrix  {+ -}  Matrix ---------------------------------------------------------//

    template <typename TYPE>
    friend inline mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkMatrix<TYPE>& MatrixA, const mdkMatrix<TYPE>& MatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<TYPE>& MatrixA, const mdkMatrix<TYPE>& MatrixB);

    // ---------------------------------------------------- Matrix  {+ -}  Element ------------------------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkMatrix<TYPE>& MatrixA, const TYPE& ElementB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkMatrix<TYPE>& MatrixA, const TYPE& ElementB);

    // --------------------------------------------------- Element {+ -} Matrix  ---------------------------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const TYPE& ElementB, const mdkMatrix<TYPE>& MatrixA);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const TYPE& ElementB, const mdkMatrix<TYPE>& MatrixA);

    // --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  Matrix ---------------------------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template <typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA, const mdkMatrix<TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkMatrix<TYPE>& MatrixA, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkMatrix<TYPE>& MatrixA, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkMatrix<TYPE>& MatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    template <typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkMatrix<TYPE>& MatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    // ------------------------------------------------- GlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const TYPE& ElementB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const TYPE& ElementB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator*(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const TYPE& ElementB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator/(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const TYPE& ElementB);

    // ----------------------------------------------- Element {+ - * /} GlueMatrixForLinearCombination ----------------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const TYPE& ElementA, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const TYPE& ElementA, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator*(const TYPE& ElementA, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB);

    template <typename TYPE>
    friend mdkMatrix<TYPE> operator/(const TYPE& ElementA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template <typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkShadowMatrix<TYPE>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkShadowMatrix<TYPE>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    template <typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    // --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  GlueMatrixForLinearCombination ---------------------------------------//

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    template <typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    template <typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    // ------------------------------------------ GlueMatrixForLinearCombination {+ - * /} GlueMatrixForMultiplication -------------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB_M);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB_M);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<TYPE> GlueMatrixB_M);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixA_L, const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixB_M);

    // -------------------------------------------- GlueMatrixForMultiplication {+ - /}  GlueMatrixForLinearCombination -----------------------------------------//

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator+(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB_L);

    template<typename TYPE>
    friend mdkGlueMatrixForLinearCombination<TYPE> operator-(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<TYPE> GlueMatrixB_L);

    template<typename TYPE>
    friend mdkGlueMatrixForMultiplication<TYPE> operator*(mdkGlueMatrixForMultiplication<TYPE> GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB_L);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkGlueMatrixForMultiplication<TYPE>& GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB_L);

    //----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

    template<typename TYPE>
    friend void operator+=(mdkShadowMatrix<TYPE> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator-=(mdkShadowMatrix<TYPE> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator*=(mdkShadowMatrix<TYPE> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator/=(mdkShadowMatrix<TYPE> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<TYPE>& GlueMatrixB);

};

}//end namespace mdk

#include "mdkGlueMatrixForLinearCombination.hpp"

#endif //defined MDK_Enable_GlueMatrix ======================================================================

#endif 