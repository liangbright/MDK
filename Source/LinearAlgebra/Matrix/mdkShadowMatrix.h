#ifndef mdk_ShadowMatrix_h
#define mdk_ShadowMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkMatrixOperator.h"


namespace mdk
{

//------------------------------ forward-declare -----------//

template<typename ElementType>
class mdkMatrix;

template<typename ElementType>
class mdkLinearCombineGlueMatrix;

template<typename ElementType>
class mdkMultiplyGlueMatrix;

struct ALL_Symbol_For_mdkMatrix_Operator;

struct mdkMatrixSize;
//--------------------------- end of forward-declare -------//



//--------------------------------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkShadowMatrix : public mdkObject
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

    mdkMatrix<ElementType> m_SourceMatrixSharedCopy;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkShadowMatrix(); // empty ShadowMatrix

    // do not need non const versions of the following constructors
    // adding const to the member functions of mdkShadowMatrix is enough

    inline mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix, const std::vector<int64>& LinearIndexList);

    inline mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix, const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList);

    inline mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix, const std::vector<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix(const mdkMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList);

    inline mdkShadowMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix) = delete;

    inline mdkShadowMatrix(mdkShadowMatrix<ElementType>&& ShadowMatrix);

public:
	inline ~mdkShadowMatrix();

	//---------------------- mdkShadowMatrix = mdkShadowMatrix or Matrix or Element or GlueMatrix ----------------------------------------//
private:
    inline void operator=(mdkShadowMatrix<ElementType>&&) = delete;

public:
    inline void operator=(const mdkMatrix<ElementType>& Matrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

private:
	//---------------------- Get/Set mdkShadowMatrix(LinearIndex) ----------------------------------------//

    // operator[]: no bound check

    inline ElementType& operator[](int64 LinearIndex);

    inline const ElementType& operator[](int64 LinearIndex) const;

	inline ElementType& operator()(int64 LinearIndex);

	inline const ElementType& operator()(int64 LinearIndex) const;

	//---------------------- Get/Set mdkShadowMatrix(i,j) ----------------------------------------//

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

    inline const mdkMatrix<ElementType>& GetSourceMatrixSharedCopy() const;

    inline mdkMatrix<ElementType> CreateMatrix() const;

    inline bool CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const;

public:
    //--------------------------------------------------- ShadowMatrix {+= -= *= /=} Matrix ------------------------------------------------//

    inline void operator+=(const mdkMatrix<ElementType>& Matrix);

    inline void operator-=(const mdkMatrix<ElementType>& Matrix);

    inline void operator*=(const mdkMatrix<ElementType>& Matrix);

    inline void operator/=(const mdkMatrix<ElementType>& Matrix);

    //--------------------------------------------------ShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //----------------------------------------------- ShadowMatrix {+= -= *= /=} ShadowMatrix ---------------------------------------------//

    inline void operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    //----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

    inline void operator+=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator-=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator*=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    //------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------//

    inline void operator+=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator-=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator*=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator/=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- ElementMultiply is .* in matlab -----------------------------------------------------------------------//

    inline mdkMatrix<ElementType> ElementMultiply(const mdkMatrix<ElementType>& Matrix) const;

    inline mdkMatrix<ElementType> ElementMultiply(const ElementType& Element) const;

    inline mdkMatrix<ElementType> ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix) const;

    inline mdkMatrix<ElementType> ElementMultiply(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const;

    inline mdkMatrix<ElementType> ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix) const;

private:
//------------------------------- friend class ----------------------------------------------------------------------------//
    
    template<typename E_TYPE>
    friend class mdkMatrix;
    
// -------------------------------- friend  function ----------------------------------------------------------------------------//
    
    // ------------------------------------------ ShadowMatrix {+ - * /}  Matrix ----------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator+(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator-(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkMatrix<E_TYPE>& MatrixB);

    // ---------------------------------------------- Matrix {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator+(const mdkMatrix<E_TYPE>& MatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator-(const mdkMatrix<E_TYPE>& MatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkMatrix<E_TYPE>& MatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkMatrix<E_TYPE>& MatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator+(const mdkShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator-(const mdkShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator*(const mdkShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkShadowMatrix<E_TYPE>& ShadowMatrix, const E_TYPE& Element);

    // ----------------------------------------- Element {+ - * /} ShadowMatrix --------------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator+(const E_TYPE& Element, const mdkShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator-(const E_TYPE& Element, const mdkShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator*(const E_TYPE& Element, const mdkShadowMatrix<E_TYPE>& ShadowMatrix);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const E_TYPE& Element, const mdkShadowMatrix<E_TYPE>& ShadowMatrix);

    // ---------------------------------------- ShadowMatrix {+ - * /}  ShadowMatrix -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator+(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator-(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(mdkGlueMatrixForLinearCombination<E_TYPE> GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkGlueMatrixForLinearCombination<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, mdkGlueMatrixForMultiplication<E_TYPE> GlueMatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkShadowMatrix<E_TYPE>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixB);

    // -------------------------------------------- GlueMatrixForMultiplication {+ - * /}  ShadowMatrix -----------------------------------------------------//

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator+(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForLinearCombination<E_TYPE> operator-(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkGlueMatrixForMultiplication<E_TYPE> operator*(mdkGlueMatrixForMultiplication<E_TYPE> GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

    template<typename E_TYPE>
    friend mdkMatrix<E_TYPE> operator/(const mdkGlueMatrixForMultiplication<E_TYPE>& GlueMatrixA, const mdkShadowMatrix<E_TYPE>& ShadowMatrixB);

};

}//end namespace mdk

#include "mdkShadowMatrix.hpp"

#endif
