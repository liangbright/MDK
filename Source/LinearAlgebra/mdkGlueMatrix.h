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

// note: GlueMatrix {+ - * % /} ShadowMatrix returns Matrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator%(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

// note: ShadowMatrix {+ - * % /} GlueMatrix returns Matrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

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

public:

	uint64 m_RowNumber;

	uint64 m_ColNumber;

    std::vector<std::shared_ptr<std::vector<ElementType>>> m_MatrixElementDataSharedPointerList;

    std::vector<ElementType> m_ElementList_Coef;

    ElementType m_IndependentElement;

public:		
	
	//------------------- constructor and destructor ------------------------------------//

    inline mdkGlueMatrix();

    inline mdkGlueMatrix(const mdkGlueMatrix<ElementType>& GlueMatrix);

    inline ~mdkGlueMatrix();

	//---------------------- Other ----------------------------------------//

    inline void Reset();

    inline uint64 GetRowNumber() const;

    inline uint64 GetColNumber() const;

    inline bool IsEmpty() const;

    inline mdkMatrix<ElementType> CreateMatrix() const;
	
    inline void CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const;

private:
     void operator=(const mdkGlueMatrix<ElementType>& GlueMatrix); // Not implemented

};

}//end namespace mdk

#include "mdkGlueMatrix.hpp"

#endif //defined MDK_ENABLE_GlueMatrix ======================================================================

#endif 