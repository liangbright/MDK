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

    inline mdkMatrix<ElementType> ElementMultiply(const ElementType& Element);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

private:
//--------------------------- deleted ----------------------------------------//

    void operator=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix) = delete;
    void operator=(mdkGlueMatrixForLinearCombination<ElementType>&& GlueMatrix) = delete;

//--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename E_TYPE>
    friend class mdkMatrix;

//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    // ----------------------------------------------------- Matrix  {+ -}  Matrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend inline mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkMatrix<E_TYPE>& MatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<E_TYPE>& MatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------------- Matrix  {+ -}  Element ------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkMatrix<E_TYPE>& MatrixA, const E_TYPE& ElementB);

    // --------------------------------------------------- Element {+ -} Matrix  ---------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementB, const mdkMatrix<E_TYPE>& MatrixA);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementB, const mdkMatrix<E_TYPE>& MatrixA);

    // --------------------------------------------- GlueMatrixForMultiplication {+ -}  Matrix ---------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ -} GlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkMatrix<E_TYPE>& MatrixA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkMatrix<E_TYPE>& MatrixA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForMultiplication  {+ -}  Element ------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const E_TYPE& ElementB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const E_TYPE& ElementB);

    // ---------------------------------------------- Element {+ -} GlueMatrixForMultiplication -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForMultiplication {+ -}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ -} GlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  Matrix ---------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    template <typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    // --------------------------------------------- Matrix or {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkMatrix<E_TYPE>& MatrixA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkMatrix<E_TYPE>& MatrixA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkMatrix<E_TYPE>& MatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkMatrix<E_TYPE>& MatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------------------- GlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator*(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator/(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const E_TYPE& ElementB);

    // ----------------------------------------------- Element {+ - * /} GlueMatrixForLinearCombination ----------------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const E_TYPE& ElementA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const E_TYPE& ElementA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator*(const E_TYPE& ElementA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const E_TYPE& ElementA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template <typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  GlueMatrixForLinearCombination ---------------------------------------//

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template <typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // ------------------------------------------ GlueMatrixForLinearCombination {+ - * /} GlueMatrixForMultiplication -------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA_L, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<E_TYPE> GlueMatrixB_M);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA_L, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB_M);

    // -------------------------------------------- GlueMatrixForMultiplication {+ - * /}  GlueMatrixForLinearCombination -----------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(mdkGlueMatrixForMultiplication<E_TYPE> GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB_L);

    //----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

    template<typename E_TYPE>
    friend void operator+=(mdkShadowMatrix<E_TYPE> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend void operator-=(mdkShadowMatrix<E_TYPE> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend void operator*=(mdkShadowMatrix<E_TYPE> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend void operator/=(mdkShadowMatrix<E_TYPE> ShadowMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);


    

};

}//end namespace mdk

#include "mdkGlueMatrixForLinearCombination.hpp"

#endif //defined MDK_Enable_GlueMatrix ======================================================================

#endif 
