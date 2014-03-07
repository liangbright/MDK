#ifndef __mdkLinearCombineGlueMatrix_h
#define __mdkLinearCombineGlueMatrix_h

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

struct mdkMatrixSize;
// end of forward-declare //

// ----------------------------------------------- Matrix  {+ -}  Matrix ----------------------------------------------------------------------------//

// note: Matrix {+ -} Matrix returns LinearCombineGlueMatrix
// note: Matrix {*} Matrix returns MultiplyGlueMatrix (defined in mdkMultiplyGlueMatrix.h)
// note: Matrix {% /} Matrix returns Matrix (defined in mdkMatrix.h)

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// --------------------------------------------- Matrix or {+ - * % /} LinearCombineGlueMatrix -------------------------------------------------------//

// note: Matrix {+ -} LinearCombineGlueMatrix returns LinearCombineGlueMatrix
// note: Matrix {* %} LinearCombineGlueMatrix returns Matrix
// note: Matrix / LinearCombineGlueMatrix returns Matrix

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator+(const mdkMatrix<ElementType>& MatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator-(const mdkMatrix<ElementType>& MatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

// --------------------------------------------- LinearCombineGlueMatrix or {+ - % /}  Matrix -------------------------------------------------------//

// note: LinearCombineGlueMatrix {+ -} Matrix returns LinearCombineGlueMatrix
// note: LinearCombineGlueMatrix {*} Matrix returns MultiplyGlueMatrix (defined in mdkMultiplyGlueMatrix.h)
// note: LinearCombineGlueMatrix {%} Matrix returns Matrix
// note: LinearCombineGlueMatrix / Matrix returns Matrix

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator+(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator-(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

// --------------------------------------------- LinearCombineGlueMatrix {+ - * % /}  LinearCombineGlueMatrix ---------------------------------------//

// note: LinearCombineGlueMatrix {+ -} LinearCombineGlueMatrix returns LinearCombineGlueMatrix
// note: LinearCombineGlueMatrix {* %} LinearCombineGlueMatrix returns Matrix
// note: LinearCombineGlueMatrix / LinearCombineGlueMatrix returns Matrix

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator+(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator-(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

// -------------------------------------------- LinearCombineGlueMatrix {+ - * % /}  ShadowMatrix -----------------------------------------------------//

// note: LinearCombineGlueMatrix {+ -} ShadowMatrix returns LinearCombineGlueMatrix
// note: LinearCombineGlueMatrix {* % /} ShadowMatrix returns Matrix

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator+(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator-(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

// ------------------------------------------ ShadowMatrix {+ - * % /} LinearCombineGlueMatrix -------------------------------------------------------//

// note: ShadowMatrix {+ -} LinearCombineGlueMatrix returns LinearCombineGlueMatrix
// note: ShadowMatrix {* % /} LinearCombineGlueMatrix returns Matrix

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator+(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator-(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrixB);

// ----------------------------------------- Element {+ - * /} Matrix or GlueMatrix -----------------------------------------------------//

// note: Element {+ - *} Matrix returns GlueMatrix
// note: Element / Matrix returns Matrix  (defined in mdkMatrix.h and .hpp)

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator+(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator-(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator*(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

// note: Element {+ - *} GlueMatrix returns GlueMatrix
// note: Element / GlueMatrix returns Matrix

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator+(const ElementType& Element, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator-(const ElementType& Element, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator*(const ElementType& Element, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& Element, mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

// ---------------------------------------- Matrix or GlueMatrix {+ - * /}  Element ------------------------------------------------------//

// note: Matrix {+ - * /}  Element returns GlueMatrix

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator+(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator-(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator*(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType> operator/(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

// note: GlueMatrix {+ - * /}  Element returns GlueMatrix

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator+(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator-(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator*(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkLinearCombineGlueMatrix<ElementType>& operator/(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------------------------//

// 100 scalars and 100 matrix should be OK
// reserve the capacity of std::vector
#define MDK_LinearCombineGlueMatrix_ReservedCapacity  100
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkLinearCombineGlueMatrix : public mdkObject
{
private:

	uint64 m_RowNumber;

	uint64 m_ColNumber;

    std::vector<std::shared_ptr<std::vector<ElementType>>> m_MatrixElementDataSharedPointerList;

    std::vector<ElementType> m_ElementList_Coef;

    ElementType m_IndependentElement;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkLinearCombineGlueMatrix();

public:
    inline ~mdkLinearCombineGlueMatrix();

private:
    inline mdkLinearCombineGlueMatrix(const mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix) = delete;

    inline mdkLinearCombineGlueMatrix(mdkLinearCombineGlueMatrix<ElementType>&& GlueMatrix);

	//---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline uint64 GetRowNumber() const;

    inline uint64 GetColNumber() const;

    inline bool IsEmpty() const;

    inline mdkMatrix<ElementType> CreateMatrix() const;
	
    inline void CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const;

//--------------------------- deleted ----------------------------------------//

    void operator=(const mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix) = delete;
    void operator=(mdkLinearCombineGlueMatrix<ElementType>&& GlueMatrix) = delete;

//--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename T>
    friend class mdkMatrix;

//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    template <typename T>
    friend inline mdkLinearCombineGlueMatrix<T> operator+(const mdkMatrix<T>& MatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<ElementType> operator-(const mdkMatrix<T>& MatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator+(const mdkMatrix<T>& MatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator-(const mdkMatrix<T>& MatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(const mdkMatrix<T>& MatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(const mdkMatrix<T>& MatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(const mdkMatrix<T>& MatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator+(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator-(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator+(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator-(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator+(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator-(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkLinearCombineGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator+(mdkShadowMatrix<T>& ShadowMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator-(mdkShadowMatrix<T>& ShadowMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkShadowMatrix<T>& ShadowMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkShadowMatrix<T>& ShadowMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkShadowMatrix<T>& ShadowMatrixA, mdkLinearCombineGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator+(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator-(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator*(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator+(const T& Element, mdkLinearCombineGlueMatrix<T>& GlueMatrix);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator-(const T& Element, mdkLinearCombineGlueMatrix<T>& GlueMatrix);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator*(const T& Element, mdkLinearCombineGlueMatrix<T>& GlueMatrix);

    template <typename T>
    friend mdkMatrix<T> operator/(const T& Element, mdkLinearCombineGlueMatrix<T>& GlueMatrix);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator+(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator-(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator*(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T> operator/(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator+(mdkLinearCombineGlueMatrix<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator-(mdkLinearCombineGlueMatrix<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator*(mdkLinearCombineGlueMatrix<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkLinearCombineGlueMatrix<T>& operator/(mdkLinearCombineGlueMatrix<T>& GlueMatrix, const T& Element);
};

}//end namespace mdk

#include "mdkLinearCombineGlueMatrix.hpp"

#endif //defined MDK_Enable_GlueMatrix ======================================================================

#endif 