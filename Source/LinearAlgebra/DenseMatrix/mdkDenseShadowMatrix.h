#ifndef __mdkDenseShadowMatrix_h
#define __mdkDenseShadowMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>
#include <string>
#include <cmath>
#include <algorithm>

#include "mdkDebugConfig.h"

#include "mdkDenseMatrix.h"

namespace mdk
{

//------------------------------ forward-declare -----------//

template<typename ElementType>
class mdkDenseMatrix;

template<typename ElementType>
class mdkDenseGlueMatrixForLinearCombination;

template<typename ElementType>
class mdkDenseGlueMatrixForMultiplication;

struct ALL_Symbol_For_mdkMatrix_Operator;

struct mdkMatrixSize;
//--------------------------- end of forward-declare -------//



//--------------------------------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkDenseShadowMatrix : public mdkObject
{
private:

	int64 m_RowNumber;

	int64 m_ColNumber;

	int64 m_ElementNumber;

    bool m_Flag_OutputVector;

    ElementType m_NaNElement;

	// info of the source Matrix -----------------------------------------------//

    bool m_Flag_All_Row;

    bool m_Flag_All_Col;

	std::vector<int64> m_RowIndexList_source;

	std::vector<int64> m_ColIndexList_source;

	std::vector<int64> m_LinearIndexList_source;

    mdkDenseMatrix<ElementType> m_SourceMatrixSharedCopy;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkDenseShadowMatrix(); // empty ShadowMatrix

    // do not need non const versions of the following constructors
    // adding const to the member functions of mdkDenseShadowMatrix is enough

    inline mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& LinearIndexList);

    inline mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList);

    inline mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkDenseShadowMatrix(const mdkDenseMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList);

    inline mdkDenseShadowMatrix(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix) = delete;

    inline mdkDenseShadowMatrix(mdkDenseShadowMatrix<ElementType>&& ShadowMatrix);

public:
	inline ~mdkDenseShadowMatrix();

	//---------------------- mdkDenseShadowMatrix = mdkDenseShadowMatrix or Matrix or Element or GlueMatrix ----------------------------------------//
private:
    inline void operator=(mdkDenseShadowMatrix<ElementType>&&) = delete;

public:
    inline void operator=(const mdkDenseMatrix<ElementType>& Matrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

private:
	//---------------------- Get/Set mdkDenseShadowMatrix(LinearIndex) ----------------------------------------//

    // operator[]: no bound check

    inline ElementType& operator[](int64 LinearIndex);

    inline const ElementType& operator[](int64 LinearIndex) const;

	inline ElementType& operator()(int64 LinearIndex);

	inline const ElementType& operator()(int64 LinearIndex) const;

	//---------------------- Get/Set mdkDenseShadowMatrix(i,j) ----------------------------------------//

	inline ElementType& operator()(int64 RowIndex, int64 ColIndex);

	inline const ElementType& operator()(int64 RowIndex, int64 ColIndex) const;

	//---------------------- Other ----------------------------------------//

    inline void Clear();

    inline bool IsLinearIndexListOnly() const;

    inline int64 GetRowNumber() const;

    inline int64 GetColNumber() const;

    inline int64 GetElementNumber() const;

    inline bool IsEmpty() const;

    inline mdkMatrixSize GetSize() const;

    inline const std::vector<int64>& GetRowIndexListOfSource() const;

    inline const std::vector<int64>& GetColIndexListOfSource() const;

    inline const std::vector<int64>& GetLinearIndexListOfSource() const;

    inline const mdkDenseMatrix<ElementType>& GetSourceMatrixSharedCopy() const;

    inline mdkDenseMatrix<ElementType> CreateDenseMatrix() const;

    inline bool CreateDenseMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const;

public:
    //--------------------------------------------------- ShadowMatrix {+= -= *= /=} Matrix ------------------------------------------------//

    inline void operator+=(const mdkDenseMatrix<ElementType>& Matrix);

    inline void operator-=(const mdkDenseMatrix<ElementType>& Matrix);

    inline void operator*=(const mdkDenseMatrix<ElementType>& Matrix);

    inline void operator/=(const mdkDenseMatrix<ElementType>& Matrix);

    //--------------------------------------------------ShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //----------------------------------------------- ShadowMatrix {+= -= *= /=} ShadowMatrix ---------------------------------------------//

    inline void operator+=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    //----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

    inline void operator+=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator-=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator*=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    //------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------//

    inline void operator+=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator-=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator*=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator/=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- ElementMultiply is .* in matlab -----------------------------------------------------------------------//

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseMatrix<ElementType>& Matrix) const;

    inline mdkDenseMatrix<ElementType> ElementMultiply(const ElementType& Element) const;

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix) const;

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const;

    inline mdkDenseMatrix<ElementType> ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const;

private:
//------------------------------- friend class ----------------------------------------------------------------------------//
    
    template<typename E_TYPE>
    friend class mdkDenseMatrix;
    
// -------------------------------- friend  function ----------------------------------------------------------------------------//
    
    // ------------------------------------------ ShadowMatrix {+ - * /}  Matrix ----------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator+(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator-(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------- Matrix {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator+(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator-(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseMatrix<E_TYPE>& MatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator+(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator-(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    // ----------------------------------------- Element {+ - * /} ShadowMatrix --------------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator+(const E_TYPE& Element, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator-(const E_TYPE& Element, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator*(const E_TYPE& Element, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const E_TYPE& Element, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrix);

    // ---------------------------------------- ShadowMatrix {+ - * /}  ShadowMatrix -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator+(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator-(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkDenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForMultiplication {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseGlueMatrixForMultiplication<E_TYPE> operator*(mdkDenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkDenseMatrix<E_TYPE> operator/(const mdkDenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkDenseShadowMatrix<E_TYPE>& ShadowMatrixB);

};

}//end namespace mdk

#include "mdkDenseShadowMatrix.hpp"

#endif
