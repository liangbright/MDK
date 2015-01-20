#ifndef mdk_DenseShadowMatrix_h
#define mdk_DenseShadowMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>
#include <cmath>
#include <algorithm>

#include "mdkDenseMatrix.h"

namespace mdk
{

//------------------------------ forward-declare -----------//
//template<typename ElementType>
//class DenseMatrix;

template<typename ElementType>
class DenseGlueMatrixForLinearCombination;

template<typename ElementType>
class DenseGlueMatrixForMultiplication;

//struct MDK_Symbol_ALL;
//struct MatrixSize;
//--------------------------- end of forward-declare -------//



//--------------------------------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class DenseShadowMatrix : public Object
{
private:

	int_max m_RowNumber;

	int_max m_ColNumber;

	int_max m_ElementNumber;

    bool m_Flag_OutputVector;

    bool m_Flag_All_Row;

    bool m_Flag_All_Col;

    ElementType m_NaNElement;

	// info of the source Matrix -----------------------------------------------//

	std::vector<int_max> m_RowIndexList_source;

	std::vector<int_max> m_ColIndexList_source;

	std::vector<int_max> m_LinearIndexList_source;

    DenseMatrix<ElementType> m_SourceMatrixSharedCopy; // can not use a raw pointer to the source matrix, see operator=(const DenseShadowMatrix&)

	//------------------- constructor and destructor ------------------------------------//
private:
    inline DenseShadowMatrix(); // empty ShadowMatrix

    // do not need none const versions of the following constructors
    // adding const to the member functions of DenseShadowMatrix is enough

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, const std::initializer_list<int_max>& LinearIndexList);

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, const std::initializer_list<int_max>& RowIndexList, const std::initializer_list<int_max>& ColIndexList);

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, const std::initializer_list<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol);

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, const MDK_Symbol_ALL& ALL_Symbol, const std::initializer_list<int_max>& ColIndexList);

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, std::vector<int_max> LinearIndexList);

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, std::vector<int_max> RowIndexList, std::vector<int_max> ColIndexList);

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, std::vector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol);

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, const MDK_Symbol_ALL& ALL_Symbol, std::vector<int_max> ColIndexList);

    inline DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, const MDK_Symbol_ALL& ALL_Symbol);

    inline DenseShadowMatrix(const DenseShadowMatrix<ElementType>& InputShadowMatrix) = delete;

    // move every member of InputShadowMatrix to this ShadowMatrix
    // reset the smart pointer of InputShadowMatrix.m_SourceMatrixSharedCopy
    // do not make this function public:
    // for example,  auto B = A(ALL, {0}); B should be a copy of a colume, but this function is called 
    inline DenseShadowMatrix(DenseShadowMatrix<ElementType>&& InputShadowMatrix);

public:
	inline ~DenseShadowMatrix();

	//---------------------- DenseShadowMatrix = DenseShadowMatrix or Matrix or Element or GlueMatrix ----------------------------------------//
    
public:
    inline void operator=(const DenseMatrix<ElementType>& InputMatrix);

    template<int_max Length>
    inline void operator=(const DenseVector<ElementType, Length>& InputVector);

	inline void operator=(const std::initializer_list<ElementType>& InputVector);

    inline void operator=(const ElementType& Element);

    inline void operator=(const DenseShadowMatrix<ElementType>& InputShadowMatrix);

    // it is no possible to move only a part of a matrix
    // so just use the above operator to copy data
    //inline void operator=(DenseShadowMatrix<ElementType>&&);

    inline void operator=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

private:
	//---------------------- Get/Set mdkDenseShadowMatrix(LinearIndex) ----------------------------------------//

    // operator[]: no bound check

    inline ElementType& operator[](int_max LinearIndex);

    inline const ElementType& operator[](int_max LinearIndex) const;

	inline ElementType& operator()(int_max LinearIndex);

	inline const ElementType& operator()(int_max LinearIndex) const;

	//---------------------- Get/Set mdkDenseShadowMatrix(i,j) ----------------------------------------//

	inline ElementType& operator()(int_max RowIndex, int_max ColIndex);

	inline const ElementType& operator()(int_max RowIndex, int_max ColIndex) const;

	//---------------------- Other ----------------------------------------//

    inline bool IsLinearIndexListOnly() const;

    inline int_max GetRowNumber() const;

    inline int_max GetColNumber() const;

    inline int_max GetElementNumber() const;

    inline bool IsEmpty() const;

    inline MatrixSize GetSize() const;

    inline const std::vector<int_max>& GetRowIndexListOfSource() const;

    inline const std::vector<int_max>& GetColIndexListOfSource() const;

    inline const std::vector<int_max>& GetLinearIndexListOfSource() const;

    inline const DenseMatrix<ElementType>& GetSourceMatrixSharedCopy() const;

    inline DenseMatrix<ElementType> CreateDenseMatrix() const;

    inline bool CreateDenseMatrix(DenseMatrix<ElementType>& OutputMatrix) const;

public:
    //--------------------------------------------------- ShadowMatrix {+= -= *= /=} Matrix ------------------------------------------------//

    inline void operator+=(const DenseMatrix<ElementType>& InputMatrix);

    inline void operator-=(const DenseMatrix<ElementType>& InputMatrix);

    inline void operator*=(const DenseMatrix<ElementType>& InputMatrix);

    inline void operator/=(const DenseMatrix<ElementType>& InputMatrix);

    //--------------------------------------------------ShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //----------------------------------------------- ShadowMatrix {+= -= *= /=} ShadowMatrix ---------------------------------------------//

    inline void operator+=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    //----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

    inline void operator+=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator-=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator*=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    //------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------//

    inline void operator+=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator-=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator*=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator/=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- ElementMultiply is .* in matlab -----------------------------------------------------------------------//

    inline DenseMatrix<ElementType> ElementMultiply(const DenseMatrix<ElementType>& InputMatrix) const;

    inline DenseMatrix<ElementType> ElementMultiply(const ElementType& Element) const;

    inline DenseMatrix<ElementType> ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix) const;

    inline DenseMatrix<ElementType> ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const;

    inline DenseMatrix<ElementType> ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const;

private:
//------------------------------- friend class ----------------------------------------------------------------------------//
    
    template<typename E_TYPE>
    friend class DenseMatrix;
    
// -------------------------------- friend  function ----------------------------------------------------------------------------//
    
    // ------------------------------------------ ShadowMatrix {+ - * /}  Matrix ----------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator+(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator-(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------- Matrix {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator+(const DenseMatrix<E_TYPE>& MatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator-(const DenseMatrix<E_TYPE>& MatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseMatrix<E_TYPE>& MatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseMatrix<E_TYPE>& MatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator+(const DenseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator-(const DenseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator*(const DenseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    // ----------------------------------------- Element {+ - * /} ShadowMatrix --------------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator+(const E_TYPE& Element, const DenseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator-(const E_TYPE& Element, const DenseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator*(const E_TYPE& Element, const DenseShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const E_TYPE& Element, const DenseShadowMatrix<E_TYPE>& ShadowMatrix);

    // ---------------------------------------- ShadowMatrix {+ - * /}  ShadowMatrix -------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator+(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator-(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(DenseGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseShadowMatrix<E_TYPE>& ShadowMatrixA, const DenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForMultiplication {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator+(const DenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForLinearCombination<E_TYPE> operator-(const DenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseGlueMatrixForMultiplication<E_TYPE> operator*(DenseGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend DenseMatrix<E_TYPE> operator/(const DenseGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const DenseShadowMatrix<E_TYPE>& ShadowMatrixB);

};

}//end namespace mdk

#include "mdkDenseShadowMatrix.hpp"

#endif
