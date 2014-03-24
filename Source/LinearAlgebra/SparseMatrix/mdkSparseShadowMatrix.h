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

struct ALL_Symbol_For_Matrix_Operator;

struct MatrixSize;

template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class DenseGlueMatrixForLinearCombination;

template<typename ElementType>
class DenseGlueMatrixForMultiplication;

template<typename ElementType>
class SparseMatrix;

template<typename ElementType>
class SparseGlueMatrixForLinearCombination;

template<typename ElementType>
class SparseGlueMatrixForMultiplication;

//--------------------------- end of forward-declare -------//



//--------------------------------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class SparseShadowMatrix : public Object
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

    SparseMatrix<ElementType> m_SourceMatrixSharedCopy;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline SparseShadowMatrix(); // empty ShadowMatrix

    // do not need non const versions of the following constructors
    // adding const to the member functions of SparseShadowMatrix is enough

    inline SparseShadowMatrix(const SparseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& LinearIndexList);

    inline SparseShadowMatrix(const SparseMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline SparseShadowMatrix(const SparseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList);

    inline SparseShadowMatrix(const SparseMatrix<ElementType>& sourceMatrix, const std::vector<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline SparseShadowMatrix(const SparseMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList);

    inline SparseShadowMatrix(const SparseShadowMatrix<ElementType>& ShadowMatrix) = delete;

    inline SparseShadowMatrix(SparseShadowMatrix<ElementType>&& ShadowMatrix);

public:
	inline ~SparseShadowMatrix();

	//---------------------- SparseShadowMatrix = SparseShadowMatrix or Matrix or Element or GlueMatrix ----------------------------------------//
private:
    inline void operator=(SparseShadowMatrix<ElementType>&&) = delete;

public:
    inline void operator=(const SparseMatrix<ElementType>& Matrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

private:
	//---------------------- Get/Set SparseShadowMatrix(LinearIndex) ----------------------------------------//

    // operator[]: no bound check

    inline ElementType& operator[](int64 LinearIndex);

    inline const ElementType& operator[](int64 LinearIndex) const;

	inline ElementType& operator()(int64 LinearIndex);

	inline const ElementType& operator()(int64 LinearIndex) const;

	//---------------------- Get/Set SparseShadowMatrix(i,j) ----------------------------------------//

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

    inline const SparseMatrix<ElementType>& GetSourceMatrixSharedCopy() const;

    inline SparseMatrix<ElementType> CreateSparseMatrix() const;

    inline mdkDenseMatrix<ElementType> CreateDenseMatrix() const;

    inline bool CreateSparseMatrix(SparseMatrix<ElementType>& OutputMatrix) const;

    inline bool CreateDenseMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const;

public:
    //--------------------------------------------------- SparseShadowMatrix {+= -= *= /=} SparseMatrix ------------------------------------------------//

    inline void operator+=(const SparseMatrix<ElementType>& Matrix);

    inline void operator-=(const SparseMatrix<ElementType>& Matrix);

    inline void operator*=(const SparseMatrix<ElementType>& Matrix);

    inline void operator/=(const SparseMatrix<ElementType>& Matrix);

    //----------------------------------------------- SparseShadowMatrix {+= -= *= /=} SparseShadowMatrix ---------------------------------------------//

    inline void operator+=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    //----------------------------------------------- SparseShadowMatrix {+= -= *= /=} SparseGlueMatrixForLinearCombination ----------------------------------------//

    inline void operator+=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator-=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator*=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    //------------------------------------------------ SparseShadowMatrix {+= -= *= /=} SparseGlueMatrixForMultiplication --------------------------------------//

    inline void operator+=(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator-=(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator*=(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator/=(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- ElementMultiply is .* in Matlab -----------------------------------------------------------------------//

    inline SparseMatrix<ElementType> ElementMultiply(const ElementType& Element) const;

    inline SparseMatrix<ElementType> ElementMultiply(const SparseMatrix<ElementType>& InputSparseMatrix) const;

    inline SparseMatrix<ElementType> ElementMultiply(const SparseShadowMatrix<ElementType>& SparseShadowMatrix) const;

    inline SparseMatrix<ElementType> ElementMultiply(const SparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix) const;

    inline SparseMatrix<ElementType> ElementMultiply(const SparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix) const;

    inline SparseMatrix<ElementType> ElementMultiply(const DenseMatrix<ElementType>& InputDenseMatrix) const;

    inline SparseMatrix<ElementType> ElementMultiply(const DenseShadowMatrix<ElementType>& DenseShadowMatrix) const;

    inline SparseMatrix<ElementType> ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix) const;

    inline SparseMatrix<ElementType> ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix) const;

private:
//------------------------------- friend class ----------------------------------------------------------------------------//
    
    template<typename E_TYPE>
    friend class SparseMatrix;
    
// -------------------------------- friend  function ----------------------------------------------------------------------------//
    
    // ------------------------------------------ SparseShadowMatrix {+ - * /}  SparseMatrix ----------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator+(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator-(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------- SparseMatrix {+ - * /}  SparseShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator+(const SparseMatrix<E_TYPE>& MatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator-(const SparseMatrix<E_TYPE>& MatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseMatrix<E_TYPE>& MatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const SparseMatrix<E_TYPE>& MatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ SparseShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator+(const SparseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator-(const SparseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator*(const SparseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const SparseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    // ----------------------------------------- Element {+ - * /} SparseShadowMatrix --------------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator+(const E_TYPE& Element, const SparseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator-(const E_TYPE& Element, const SparseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator*(const E_TYPE& Element, const SparseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const E_TYPE& Element, const SparseShadowMatrix<E_TYPE>& ShadowMatrix);

    // ---------------------------------------- SparseShadowMatrix {+ - * /}  SparseShadowMatrix -------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator+(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator-(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ SparseShadowMatrix {+ - * /} DenseGlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForLinearCombination {+ - * /}  SparseShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(SparseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const SparseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ SparseShadowMatrix {+ - * /} DenseGlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const SparseShadowMatrix<E_TYPE>& ShadowMatrixA, const SparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- DenseGlueMatrixForMultiplication {+ - * /}  SparseShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator+(const SparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForLinearCombination<E_TYPE> operator-(const SparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseGlueMatrixForMultiplication<E_TYPE> operator*(SparseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend SparseMatrix<E_TYPE> operator/(const SparseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const SparseShadowMatrix<E_TYPE>& ShadowMatrixB);

};

}//end namespace mdk

#include "mdkSparseShadowMatrix.hpp"

#endif
