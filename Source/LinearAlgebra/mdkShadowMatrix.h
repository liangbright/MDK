#ifndef __mdkShadowMatrix_h
#define __mdkShadowMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
//#include "mdkMatrix.h"

namespace mdk
{

//forward-declare -----------//
template<typename ElementType>
class mdkShadowMatrix;

template<typename ElementType>
class mdkGlueMatrix;

template<typename ElementType>
class mdkMatrix;

struct ALL_Symbol_For_mdkMatrix_Operator;

struct mdkMatrixSize;
// end of forward-declare //

// ----------------------- ShadowMatrix {+ - * / %}  ShadowMatrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

//element-wise multiplication
template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

// ----------------------- Element {+ - * /} ShadowMatrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(ElementType Element, const mdkShadowMatrix<ElementType>& ShadowMatrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(ElementType Element, const mdkShadowMatrix<ElementType>& ShadowMatrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(ElementType Element, const mdkShadowMatrix<ElementType>& ShadowMatrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(ElementType Element, const mdkShadowMatrix<ElementType>& ShadowMatrix);

// ----------------------- ShadowMatrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrix, ElementType Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrix, ElementType Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrix, ElementType Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrix, ElementType Element);
//--------------------------------------------------------------------------------------------------//

// ----------------------- ShadowMatrix {+ - * / %}  Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

//element-wise multiplication
template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

// ----------------------- Matrix {+ - * / %}  ShadowMatrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

//element-wise multiplication
template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

//--------------------------------------------------------------------------------------------------//


template<typename ElementType>
class mdkShadowMatrix : public mdkObject
{
private:

	uint64 m_RowNumber;

	uint64 m_ColNumber;

	uint64 m_ElementNumber;

    ElementType m_NaNElement;

    bool m_IsLinearIndexListOnly;

	// info from the source Matrix -----------------------------------------------//

	std::vector<uint64> m_RowIndexList_source;

	std::vector<uint64> m_ColIndexList_source;

	std::vector<uint64> m_LinearIndexList_source;

    mdkMatrix<ElementType> m_SourceMatrixSharedCopy;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkShadowMatrix(); // only use for empty ShadowMatrix

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix(const mdkShadowMatrix<ElementType>&) = delete;

    inline mdkShadowMatrix(mdkShadowMatrix<ElementType>&& ShadowMatrix);

public:
	inline ~mdkShadowMatrix();

	//---------------------- mdkShadowMatrix = mdkShadowMatrix or Matrix or Element ----------------------------------------//

    inline void operator=(const mdkMatrix<ElementType>& targetMatrix);

    inline void operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

private:
    inline void operator=(mdkShadowMatrix<ElementType>&&) = delete;

public:
	inline void operator=(const ElementType& Element);

	//---------------------- mdkShadowMatrix {+= -= *= /=} Matrix or mdkShadowMatrix -----------------------------//

    inline void operator+=(const mdkMatrix<ElementType>& targetMatrix);

    inline void operator-=(const mdkMatrix<ElementType>& targetMatrix);

    inline void operator*=(const mdkMatrix<ElementType>& targetMatrix);

    inline void operator/=(const mdkMatrix<ElementType>& targetMatrix);

    inline void operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    //---------------------- mdkShadowMatrix {+= -= *= /=} Element ----------------------------------------//

	inline void operator+=(ElementType Element);

	inline void operator-=(ElementType Element);

	inline void operator*=(ElementType Element);

	inline void operator/=(ElementType Element);

	//---------------------- Get/Set mdkShadowMatrix(LinearIndex) ----------------------------------------//
private:
	inline ElementType& operator()(uint64 LinearIndex);

	inline const ElementType& operator()(uint64 LinearIndex) const;

	//---------------------- Get/Set mdkShadowMatrix(i,j) ----------------------------------------//

	inline ElementType& operator()(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& operator()(uint64 RowIndex, uint64 ColIndex) const;

	//---------------------- Other ----------------------------------------//

    inline void Reset();

    inline bool IsLinearIndexListOnly() const;

    inline uint64 GetRowNumber() const;

    inline uint64 GetColNumber() const;

    inline uint64 GetElementNumber() const;

    inline bool IsEmpty() const;

    inline mdkMatrixSize GetSize() const;

    inline const std::vector<uint64>& GetRowIndexListOfSource() const;

    inline const std::vector<uint64>& GetColIndexListOfSource() const;

    inline const std::vector<uint64>& GetLinearIndexListOfSource() const;

    inline const mdkMatrix<ElementType>& GetSourceMatrixSharedCopy() const;

    inline mdkMatrix<ElementType> CreateMatrix() const;

    inline void CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const;

//------------------------------- friend class ----------------------------------------------------------------------------//
    
    template<typename T>
    friend class mdkMatrix;
    
// -------------------------------- friend  function ----------------------------------------------------------------------------//
    
    template<typename T>
    friend mdkMatrix<T> operator+(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator-(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator*(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator/(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator%(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator+(T Element, const mdkShadowMatrix<T>& ShadowMatrix);

    template<typename T>
    friend mdkMatrix<T> operator-(T Element, const mdkShadowMatrix<T>& ShadowMatrix);

    template<typename T>
    friend mdkMatrix<T> operator*(T Element, const mdkShadowMatrix<T>& ShadowMatrix);

    template<typename T>
    friend mdkMatrix<T> operator/(T Element, const mdkShadowMatrix<T>& ShadowMatrix);

    template<typename T>
    friend mdkMatrix<T> operator+(const mdkShadowMatrix<T>& ShadowMatrix, T Element);

    template<typename T>
    friend mdkMatrix<T> operator-(const mdkShadowMatrix<T>& ShadowMatrix, T Element);

    template<typename T>
    friend mdkMatrix<T> operator*(const mdkShadowMatrix<T>& ShadowMatrix, T Element);

    template<typename T>
    friend mdkMatrix<T> operator/(const mdkShadowMatrix<T>& ShadowMatrix, T Element);
  
    template<typename T>
    friend mdkMatrix<T> operator+(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkMatrix<T>& MatrixB);

    template<typename T>
    friend mdkMatrix<T> operator-(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkMatrix<T>& MatrixB);

    template<typename T>
    friend mdkMatrix<T> operator*(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkMatrix<T>& MatrixB);

    template<typename T>
    friend mdkMatrix<T> operator/(const mdkShadowMatrix<T>& ShadowMatrixA, const mdkMatrix<T>& MatrixB);

    template<typename T>
    friend mdkMatrix<T> operator%(const mdkMatrix<T>& ShadowMatrixA, const mdkMatrix<T>& MatrixB);

    template<typename T>
    friend mdkMatrix<T> operator+(const mdkMatrix<T>& MatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator-(const mdkMatrix<T>& MatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator*(const mdkMatrix<T>& MatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator/(const mdkMatrix<T>& MatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    template<typename T>
    friend mdkMatrix<T> operator%(const mdkMatrix<T>& MatrixA, const mdkShadowMatrix<T>& ShadowMatrixB);

    //--------------------GlueMatrix realted function-----------------

    template<typename T>
    friend mdkMatrix<T> operator*(mdkShadowMatrix<T>& ShadowMatrixA, mdkGlueMatrix<T>& GlueMatrixB);
    
};

}//end namespace mdk

#include "mdkShadowMatrix.hpp"

#endif