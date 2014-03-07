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

// --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  GlueMatrixForLinearCombination ---------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator+(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator-(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

// ----------------------------------------------- Matrix  {+ -}  Matrix ----------------------------------------------------------------------------//

// note: Matrix {+ -} Matrix return GlueMatrixForLinearCombination
// note: Matrix {*}   Matrix return MultiplyGlueMatrix (definition in mdkMultiplyGlueMatrix.h)
// note: Matrix {/}   Matrix return Matrix (definition in mdkMatrix.h)

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// --------------------------------------------- Matrix or {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

// note: Matrix {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: Matrix {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication (definition in mdkGlueMatrixForMultiplication.h)
// note: Matrix {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

// --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  Matrix ---------------------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} Matrix return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   Matrix return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   Matrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator+(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator-(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

// ---------------------------------------------------- Matrix  {+ - * /}  Element ------------------------------------------------------//

// note: Matrix {+ - * /}  Element return GlueMatrixForLinearCombination

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator*(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator/(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

// --------------------------------------------------- Element {+ - *} Matrix  ---------------------------------------------------------//

// note: Element {+ - *} Matrix return GlueMatrixForLinearCombination
// note: Element {/}     Matrix return Matrix  (definition in mdkMatrix.h)

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator*(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

// ------------------------------------------------- GlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

// note: GlueMatrixForLinearCombination {+ - * /}  Element return GlueMatrixForLinearCombination

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator+(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator-(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator*(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator/(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix, const ElementType& Element);

// ---------------------------------------------------- Element {+ - * /} GlueMatrixForLinearCombination ----------------------------------------------//

// note: Element {+ - *} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: Element {/}     GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator+(const ElementType& Element, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator-(const ElementType& Element, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator*(const ElementType& Element, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& Element, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

// -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} ShadowMatrix return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

// ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

// note: ShadowMatrix {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: ShadowMatrix {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: ShadowMatrix {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

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

    template<typename T>
    friend class mdkMatrix;

//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    template <typename T>
    friend inline mdkGlueMatrixForLinearCombination<T> operator+(const mdkMatrix<T>& MatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<T>& MatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator+(const mdkMatrix<T>& MatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator-(const mdkMatrix<T>& MatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(const mdkMatrix<T>& MatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(const mdkMatrix<T>& MatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator+(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator-(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator+(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator-(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator+(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator-(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkGlueMatrixForLinearCombination<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator+(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator-(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator+(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator-(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator*(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator+(const T& Element, mdkGlueMatrixForLinearCombination<T>& GlueMatrix);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator-(const T& Element, mdkGlueMatrixForLinearCombination<T>& GlueMatrix);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator*(const T& Element, mdkGlueMatrixForLinearCombination<T>& GlueMatrix);

    template <typename T>
    friend mdkMatrix<T> operator/(const T& Element, mdkGlueMatrixForLinearCombination<T>& GlueMatrix);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator+(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator-(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator*(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T> operator/(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator+(mdkGlueMatrixForLinearCombination<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator-(mdkGlueMatrixForLinearCombination<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator*(mdkGlueMatrixForLinearCombination<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrixForLinearCombination<T>& operator/(mdkGlueMatrixForLinearCombination<T>& GlueMatrix, const T& Element);
};

}//end namespace mdk

#include "mdkGlueMatrixForLinearCombination.hpp"

#endif //defined MDK_Enable_GlueMatrix ======================================================================

#endif 