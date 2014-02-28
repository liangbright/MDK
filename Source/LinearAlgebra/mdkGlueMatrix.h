#ifndef __mdkGlueMatrix_h
#define __mdkGlueMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
//#include "mdkMatrix.h"

#if defined MDK_ENABLE_GlueMatrix //=====================================================================================

namespace mdk
{

//forward-declare -----------//
template<typename ElementType>
class mdkMatrix;

struct mdkMatrixSize;
// end of forward-declare //

// ----------------------- Matrix {+ - }  Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrixA, mdkGlueMatrix<ElementType>& GlueMatrixB);

// ----------------------- Element {+ - *} Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator+(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator*(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator*(const ElementType& Element, mdkGlueMatrix<ElementType>& GlueMatrix);

// ----------------------- Matrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator+(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator-(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator*(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType> operator/(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator+(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator-(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator*(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);

template<typename ElementType>
inline mdkGlueMatrix<ElementType>& operator/(mdkGlueMatrix<ElementType>& GlueMatrix, const ElementType& Element);
//--------------------------------------------------------------------------------------------------//

#define MDK_GlueMatrix_ReservedNumber  10
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

    inline void Clear();

    inline mdkMatrix<ElementType> CreateMatrix() const;
	
private:
     void operator=(const mdkGlueMatrix<ElementType>& GlueMatrix); // Not implemented

};

}//end namespace mdk

#include "mdkGlueMatrix.hpp"

#endif //defined MDK_ENABLE_GlueMatrix ======================================================================

#endif 