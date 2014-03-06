#ifndef __mdkGlueMatrix_h
#define __mdkGlueMatrix_h

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

// ----------------------- Matrix or GlueMatrix {+ - * % /}  Matrix or GlueMatrix ------------------------------------------------//

// note: Matrix {+ -} Matrix returns GlueMatrix
// note: Matrix {* %} Matrix returns Matrix (defined in mdkMatrix.h and .hpp)
// note: Matrix / Matrix returns Matrix (defined in mdkMatrix.h and .hpp)

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// note: Matrix {+ -} GlueMatrix returns GlueMatrix
// note: Matrix {* %} GlueMatrix returns Matrix
// note: Matrix / GlueMatrix returns Matrix

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

// note: GlueMatrix {+ -} Matrix returns GlueMatrix
// note: GlueMatrix {* %} Matrix returns Matrix
// note: GlueMatrix / Matrix returns Matrix

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

// note: GlueMatrix {+ -} GlueMatrix returns GlueMatrix
// note: GlueMatrix {* %} GlueMatrix returns Matrix
// note: GlueMatrix / GlueMatrix returns Matrix

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

// ----------------------- GlueMatrix or ShadowMatrix {+ - * % /}  GlueMatrix or ShadowMatrix ------------------------------------//

// note: GlueMatrix {+ -} ShadowMatrix returns GlueMatrix
// note: GlueMatrix {* % /} ShadowMatrix returns Matrix

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

// note: ShadowMatrix {+ -} GlueMatrix returns GlueMatrix
// note: ShadowMatrix {* % /} GlueMatrix returns Matrix

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator+(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

// ----------------------- Element {+ - * /} Matrix or GlueMatrix ------------------------------------------------//

// note: Element {+ - *} Matrix returns GlueMatrix
// note: Element / Matrix returns Matrix  (defined in mdkMatrix.h and .hpp)

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator+(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator*(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

// note: Element {+ - *} GlueMatrix returns GlueMatrix
// note: Element / GlueMatrix returns Matrix

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator*(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix);

// ----------------------- Matrix or GlueMatrix {+ - * /}  Element ------------------------------------------------//

// note: Matrix {+ - * /}  Element returns GlueMatrix

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator+(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator*(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator/(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

// note: GlueMatrix {+ - * /}  Element returns GlueMatrix

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator*(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator/(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);
//--------------------------------------------------------------------------------------------------//

// 100 scalars and 100 matrix should be ok 
// reserve the capacity of std::vector
#define MDK_GlueMatrix_ReservedCapacity  100
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkGlueMatrix : public mdkObject
{
private:

	uint64 m_RowNumber;

	uint64 m_ColNumber;

    std::vector<std::shared_ptr<std::vector<ElementType>>> m_MatrixElementDataSharedPointerList;

    std::vector<ElementType> m_ElementList_Coef;

    ElementType m_IndependentElement;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkGlueMatrix();

public:
    inline ~mdkGlueMatrix();

private:
    inline mdkGlueMatrix(const mdkGlueMatrix<ElementType>& GlueMatrix);

    inline mdkGlueMatrix(mdkGlueMatrix<ElementType>&& GlueMatrix);

	//---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline uint64 GetRowNumber() const;

    inline uint64 GetColNumber() const;

    inline bool IsEmpty() const;

    inline mdkMatrix<ElementType> CreateMatrix() const;
	
    inline void CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const;

//--------------------------- deleted ----------------------------------------//

    void operator=(const mdkGlueMatrix<ElementType>& GlueMatrix) = delete;
    void operator=(mdkGlueMatrix<ElementType>&& GlueMatrix) = delete;

//--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename T>
    friend class mdkMatrix;

//--------------------------- friend function ----------------------------------------------------------------------------------//
    
    template <typename T>
    friend inline mdkGlueMatrix<T> operator+(const mdkMatrix<T>& MatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkGlueMatrix<ElementType> operator-(const mdkMatrix<T>& MatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkGlueMatrix<T>& operator+(const mdkMatrix<T>& MatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrix<T>& operator-(const mdkMatrix<T>& MatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(const mdkMatrix<T>& MatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(const mdkMatrix<T>& MatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(const mdkMatrix<T>& MatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrix<T>& operator+(mdkGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkGlueMatrix<T>& operator-(mdkGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkGlueMatrix<T>& GlueMatrixA, const mdkMatrix<T>& MatrixB);

    template <typename T>
    friend mdkGlueMatrix<T>& operator+(mdkGlueMatrix<T>& GlueMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrix<T>& operator-(mdkGlueMatrix<T>& GlueMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkGlueMatrix<T>& GlueMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkGlueMatrix<T>& GlueMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkGlueMatrix<T>& GlueMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrix<T> operator+(mdkGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkGlueMatrix<T> operator-(mdkGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkGlueMatrix<T>& GlueMatrixA, mdkShadowMatrix<T>& ShadowMatrixB);

    template <typename T>
    friend mdkGlueMatrix<T> operator+(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrix<T> operator-(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator*(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator%(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkMatrix<T> operator/(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrix<T>& GlueMatrixB);

    template <typename T>
    friend mdkGlueMatrix<T> operator+(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkGlueMatrix<T> operator-(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkGlueMatrix<T> operator*(const T& Element, const mdkMatrix<T>& Matrix);

    template <typename T>
    friend mdkGlueMatrix<T>& operator+(const T& Element, mdkGlueMatrix<T>& GlueMatrix);

    template <typename T>
    friend mdkGlueMatrix<T>& operator-(const T& Element, mdkGlueMatrix<T>& GlueMatrix);

    template <typename T>
    friend mdkGlueMatrix<T>& operator*(const T& Element, mdkGlueMatrix<T>& GlueMatrix);

    template <typename T>
    friend mdkMatrix<T> operator/(const T& Element, mdkGlueMatrix<T>& GlueMatrix);

    template <typename T>
    friend mdkGlueMatrix<T> operator+(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrix<T> operator-(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrix<T> operator*(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrix<T> operator/(const mdkMatrix<T>& Matrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrix<T>& operator+(mdkGlueMatrix<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrix<T>& operator-(mdkGlueMatrix<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrix<T>& operator*(mdkGlueMatrix<T>& GlueMatrix, const T& Element);

    template <typename T>
    friend mdkGlueMatrix<T>& operator/(mdkGlueMatrix<T>& GlueMatrix, const T& Element);
};

}//end namespace mdk

#include "mdkGlueMatrix.hpp"

#endif //defined MDK_ENABLE_GlueMatrix ======================================================================

#endif 