#ifndef __mdkSparseShadowMatrix_h
#define __mdkSparseShadowMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkSparseMatrixOperator.h"


namespace mdk
{

//------------------------------ forward-declare -----------//

template<typename ElementType>
class mdkDenseMatrix;

template<typename ElementType>
class mdkSparseMatrix;

template<typename ElementType>
class mdkDenseGlueMatrixForLinearCombination;

template<typename ElementType>
class mdkDenseGlueMatrixForMultiplication;

struct ALL_Symbol_For_mdkMatrix_Operator;

struct mdkMatrixSize;
//--------------------------- end of forward-declare -------//



//--------------------------------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkSparseShadowMatrix : public mdkObject
{
private:

	int64 m_RowNumber;

	int64 m_ColNumber;

    bool m_Flag_OutputVector;

    ElementType m_NaNElement;

	// info of the source Matrix -----------------------------------------------//

    bool m_Flag_All_Row;

    bool m_Flag_All_Col;

	std::vector<int64> m_RowIndexList_source;

	std::vector<int64> m_ColIndexList_source;

	std::vector<int64> m_LinearIndexList_source;

    mdkSparseMatrix<ElementType> m_SourceMatrixSharedCopy;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkSparseShadowMatrix(); // empty ShadowMatrix

    // do not need non const versions of the following constructors
    // adding const to the member functions of mdkSparseShadowMatrix is enough

    inline mdkSparseShadowMatrix(const mdkSparseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& LinearIndexList);

    inline mdkSparseShadowMatrix(const mdkSparseMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkSparseShadowMatrix(const mdkSparseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList);

    inline mdkSparseShadowMatrix(const mdkSparseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkSparseShadowMatrix(const mdkSparseMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList);

    inline mdkSparseShadowMatrix(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix) = delete;

    inline mdkSparseShadowMatrix(mdkSparseShadowMatrix<ElementType>&& ShadowMatrix);

public:
	inline ~mdkSparseShadowMatrix();

	//---------------------- mdkSparseShadowMatrix = mdkSparseShadowMatrix or Matrix or Element or GlueMatrix ----------------------------------------//
private:
    inline void operator=(mdkSparseShadowMatrix<ElementType>&&) = delete;

public:
    inline void operator=(const mdkSparseMatrix<ElementType>& Matrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkSparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

private:
	//---------------------- Get/Set mdkSparseShadowMatrix(LinearIndex) ----------------------------------------//

    // operator[]: no bound check

    inline ElementType& operator[](int64 LinearIndex);

    inline const ElementType& operator[](int64 LinearIndex) const;

	inline ElementType& operator()(int64 LinearIndex);

	inline const ElementType& operator()(int64 LinearIndex) const;

	//---------------------- Get/Set mdkSparseShadowMatrix(i,j) ----------------------------------------//

	inline ElementType& operator()(int64 RowIndex, int64 ColIndex);

	inline const ElementType& operator()(int64 RowIndex, int64 ColIndex) const;

	//---------------------- Other ----------------------------------------//

    inline void Reset();

    inline bool IsLinearIndexListOnly() const;

    inline int64 GetRowNumber() const;

    inline int64 GetColNumber() const;

    inline int64 GetElementNumber() const;

    inline bool IsEmpty() const;

    inline mdkMatrixSize GetSize() const;

    inline const std::vector<int64>& GetRowIndexListOfSource() const;

    inline const std::vector<int64>& GetColIndexListOfSource() const;

    inline const std::vector<int64>& GetLinearIndexListOfSource() const;

    inline const mdkSparseMatrix<ElementType>& GetSourceMatrixSharedCopy() const;

    inline mdkSparseMatrix<ElementType> CreateSparseMatrix() const;

    inline mdkDenseMatrix<ElementType> CreateDenseMatrix() const;

    inline bool CreateSparseMatrix(mdkSparseMatrix<ElementType>& OutputMatrix) const;

    inline bool CreateDenseMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const;

public:
    //--------------------------------------------------- SparseShadowMatrix {+= -= *= /=} SparseMatrix ------------------------------------------------//

    inline void operator+=(const mdkSparseMatrix<ElementType>& Matrix);

    inline void operator-=(const mdkSparseMatrix<ElementType>& Matrix);

    inline void operator*=(const mdkSparseMatrix<ElementType>& Matrix);

    inline void operator/=(const mdkSparseMatrix<ElementType>& Matrix);

    //----------------------------------------------- SparseShadowMatrix {+= -= *= /=} SparseShadowMatrix ---------------------------------------------//

    inline void operator+=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    //----------------------------------------------- SparseShadowMatrix {+= -= *= /=} SparseGlueMatrixForLinearCombination ----------------------------------------//

    inline void operator+=(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator-=(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator*=(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    //------------------------------------------------ SparseShadowMatrix {+= -= *= /=} SparseGlueMatrixForMultiplication --------------------------------------//

    inline void operator+=(const mdkSparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator-=(const mdkSparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator*=(const mdkSparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator/=(const mdkSparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- ElementMultiply is .* in Matlab -----------------------------------------------------------------------//

    inline mdkSparseMatrix<ElementType> ElementMultiply(const ElementType& Element) const;

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkSparseMatrix<ElementType>& InputSparseMatrix) const;

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkSparseShadowMatrix<ElementType>& SparseShadowMatrix) const;

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkSparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix) const;

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkSparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix) const;

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkDenseMatrix<ElementType>& InputDenseMatrix) const;

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkDenseShadowMatrix<ElementType>& DenseShadowMatrix) const;

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix) const;

    inline mdkSparseMatrix<ElementType> ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix) const;

private:
//------------------------------- friend class ----------------------------------------------------------------------------//
    
    template<typename E_TYPE>
    friend class mdkSparseMatrix;
    
// -------------------------------- friend  function ----------------------------------------------------------------------------//
    
    // ------------------------------------------ SparseShadowMatrix {+ - * /}  SparseMatrix ----------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator+(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator-(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------- SparseMatrix {+ - * /}  SparseShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator+(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator-(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseMatrix<E_TYPE>& MatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ SparseShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator+(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator-(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    // ----------------------------------------- Element {+ - * /} SparseShadowMatrix --------------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator+(const E_TYPE& Element, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator-(const E_TYPE& Element, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator*(const E_TYPE& Element, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const E_TYPE& Element, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrix);

    // ---------------------------------------- SparseShadowMatrix {+ - * /}  SparseShadowMatrix -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator+(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator-(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ SparseShadowMatrix {+ - * /} DenseGlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForLinearCombination {+ - * /}  SparseShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(mdkSparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ SparseShadowMatrix {+ - * /} DenseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication {+ - * /}  SparseShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseGlueMatrixForMultiplication<E_TYPE> operator*(mdkSparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkSparseMatrix<E_TYPE> operator/(const mdkSparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkSparseShadowMatrix<E_TYPE>& ShadowMatrixB);

};

}//end namespace mdk

#include "mdkSparseShadowMatrix.hpp"

#endif
