#ifndef __mdkShadowMatrix_h
#define __mdkShadowMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"

namespace mdk
{

//------------------------------ forward-declare -----------//

template<typename ElementType>
class mdkMatrix;

template<typename ElementType>
class mdkShadowMatrix;

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

	uint64 m_RowNumber;

	uint64 m_ColNumber;

	uint64 m_ElementNumber;

    bool m_Flag_OutputVector;

	// info from the source Matrix -----------------------------------------------//

	std::vector<uint64> m_RowIndexList_source;

	std::vector<uint64> m_ColIndexList_source;

	std::vector<uint64> m_LinearIndexList_source;

    mdkMatrix<ElementType> m_SourceMatrixSharedCopy;

	//------------------- constructor and destructor ------------------------------------//
private:
    inline mdkShadowMatrix(); // only use for empty ShadowMatrix

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix(mdkMatrix<ElementType>& sourceMatrix, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix(const mdkShadowMatrix<ElementType>&) = delete;

    inline mdkShadowMatrix(mdkShadowMatrix<ElementType>&& ShadowMatrix);

public:
	inline ~mdkShadowMatrix();

	//---------------------- mdkShadowMatrix = mdkShadowMatrix or Matrix or Element ----------------------------------------//
private:
    inline void operator=(mdkShadowMatrix<ElementType>&&) = delete;

public:
    inline void operator=(const mdkMatrix<ElementType>& targetMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- ElementMultiply is .* in matlab -----------------------------------------------//

    inline mdkMatrix ElementMultiply(const mdkMatrix<ElementType>& targetMatrix);

    inline mdkMatrix ElementMultiply(const ElementType& Element);

    inline mdkMatrix ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkMatrix ElementMultiply(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline mdkMatrix ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

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
    
    // ---------------------------------------- ShadowMatrix {+ - *  % /}  ShadowMatrix -------------------------------------------------------//

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator+(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator-(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator*(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    //element-wise multiplication
    template<typename TYPE>
    friend mdkMatrix<TYPE> operator%(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    // ------------------------------------------ ShadowMatrix {+ - * % /}  Matrix ----------------------------------------------------------//

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator+(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator-(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator*(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    //element-wise multiplication
    template<typename TYPE>
    friend mdkMatrix<TYPE> operator%(const mdkMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    // ---------------------------------------------- Matrix {+ - * % /}  ShadowMatrix -----------------------------------------------------//

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator+(const mdkMatrix<TYPE>& MatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator-(const mdkMatrix<TYPE>& MatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator*(const mdkMatrix<TYPE>& MatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    //element-wise multiplication
    template<typename TYPE>
    friend mdkMatrix<TYPE> operator%(const mdkMatrix<TYPE>& MatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkMatrix<TYPE>& MatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    // ----------------------------------------- Element {+ - * /} ShadowMatrix --------------------------------------------------------------//

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator+(TYPE Element, const mdkShadowMatrix<TYPE>& ShadowMatrix);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator-(TYPE Element, const mdkShadowMatrix<TYPE>& ShadowMatrix);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator*(TYPE Element, const mdkShadowMatrix<TYPE>& ShadowMatrix);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(TYPE Element, const mdkShadowMatrix<TYPE>& ShadowMatrix);

    // ------------------------------------------ ShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator+(const mdkShadowMatrix<TYPE>& ShadowMatrix, TYPE Element);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator-(const mdkShadowMatrix<TYPE>& ShadowMatrix, TYPE Element);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator*(const mdkShadowMatrix<TYPE>& ShadowMatrix, TYPE Element);

    template<typename TYPE>
    friend mdkMatrix<TYPE> operator/(const mdkShadowMatrix<TYPE>& ShadowMatrix, TYPE Element);

    //----------------------------------------------- ShadowMatrix {+= -= *= %= /=} ShadowMatrix ---------------------------------------------//

    template<typename TYPE>
    friend void operator+=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend void operator-=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend void operator*=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend void operator%=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    template<typename TYPE>
    friend void operator/=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkShadowMatrix<TYPE>& ShadowMatrixB);

    //--------------------------------------------------- ShadowMatrix {+= -= *= %= /=} Matrix ------------------------------------------------//

    template<typename TYPE>
    friend void operator+=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend void operator-=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend void operator*=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend void operator%=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    template<typename TYPE>
    friend void operator/=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMatrix<TYPE>& MatrixB);

    //--------------------------------------------------ShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

    template<typename TYPE>
    friend void operator+=(mdkShadowMatrix<TYPE>& ShadowMatrix, const TYPE& Element);

    template<typename TYPE>
    friend void operator-=(EmdkShadowMatrix<TYPE>& ShadowMatrix, const TYPE& Element);

    template<typename TYPE>
    friend void operator*=(mdkShadowMatrix<TYPE>& ShadowMatrix, const TYPE& Element);

    template<typename TYPE>
    friend void operator/=(mdkShadowMatrix<TYPE>& ShadowMatrix, const TYPE& Element);

    //----------------------------------------------- ShadowMatrix {+= -= /= *= %= } LinearCombineGlueMatrix ----------------------------------------//

    template<typename TYPE>
    friend void operator+=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkLinearCombineGlueMatrix<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator-=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkLinearCombineGlueMatrix<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator*=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkLinearCombineGlueMatrix<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator%=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkLinearCombineGlueMatrix<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator/=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkLinearCombineGlueMatrix<TYPE>& GlueMatrixB);

    //------------------------------------------------ ShadowMatrix {+= -= /= *= %= } MultiplyGlueMatrix --------------------------------------------//

    template<typename TYPE>
    friend void operator+=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMultiplyGlueMatrix<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator-=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMultiplyGlueMatrix<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator*=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMultiplyGlueMatrix<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator%=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMultiplyGlueMatrix<TYPE>& GlueMatrixB);

    template<typename TYPE>
    friend void operator/=(mdkShadowMatrix<TYPE>& ShadowMatrixA, const mdkMultiplyGlueMatrix<TYPE>& GlueMatrixB);

};

}//end namespace mdk

#include "mdkShadowMatrix.hpp"

#endif