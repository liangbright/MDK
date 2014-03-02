﻿#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
#include "mdkMatrixElement.h"
//#include "mdkShadowMatrix.h"


namespace mdk
{

// 2D Matrix Class Template, each entry/element is a scalar
// column major
//
// If heavy linear algebra is required, then convert mdkMatrix to Armadillo matrix, do something, and convert back
// Armadillo is a linear algebra library, and it uses column major matrix
//
// The functions that are not supported in Armadillo, are provided in mdkLinearAlgebra.h/cpp

//forward-declare ----------------//
template<typename ElementType>
class mdkMatrix;

template<typename ElementType>
class mdkShadowMatrix;

template<typename ElementType>
class mdkGlueMatrix;

// end of  forward-declare  //

struct mdkMatrixSize
{
	uint64 RowNumber;  // RowNumber = the Number of Rows 
	uint64 ColNumber;  // ColNumber = the Number of Columns
};


template<typename ElementType>
struct mdkMatrixSVDResult
{
// Matrix = U*S*V;
	mdkMatrix<ElementType> U;  // matrix
	mdkMatrix<ElementType> S;  // matrix  : change to vector?
	mdkMatrix<ElementType> V;  // matrix
};


// ----------------------- Matrix {+ - * / %}  Matrix ------------------------------------------------//

#if !defined MDK_ENABLE_GlueMatrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

#endif // !defined MDK_ENABLE_GlueMatrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

//element-wise multiplication
template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// ----------------------- Element {+ - * /} Matrix ------------------------------------------------//

#if !defined MDK_ENABLE_GlueMatrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

#endif // !defined MDK_ENABLE_GlueMatrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

// ----------------------- Matrix {+ - * /}  Element ------------------------------------------------//

#if !defined MDK_ENABLE_GlueMatrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkMatrix<ElementType>& Matrix, const ElementType& Element);

#endif // !defined MDK_ENABLE_GlueMatrix
//--------------------------------------------------------------------------------------------------//

#define MDK_Matrix_ColExpansionStep  100

//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkMatrix : public mdkObject
{

private:

	uint64 m_RowNumber;
	
	uint64 m_ColNumber;

	std::shared_ptr<std::vector<ElementType>> m_ElementData;

	bool m_IsSizeFixed;

	mdkMatrixElementTypeEnum m_ElementType;

	ElementType  m_NaNElement; // may not be ZeroElement, but ZeroElement != m_NaNElement - m_NaNElement;

	bool m_IsTemporary; // a Matrix returned from a function is a temporary matrix

public:		
	
	//------------------- constructor and destructor ------------------------------------//

	inline mdkMatrix();

    inline mdkMatrix(const mdkMatrix<ElementType>& targetMatrix, bool IsSizeFixed = true);

    inline mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix, bool IsSizeFixed = true);

    inline mdkMatrix(const mdkGlueMatrix<ElementType>& GlueMatrix, bool IsSizeFixed = true);

    inline mdkMatrix(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = true);

    inline mdkMatrix(const ElementType* ElementPointer, uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = true);

	inline ~mdkMatrix();

    //-----------------------------------------------------------------------------------//
    inline void Clear();

    inline void SetTobeTemporary();

    inline bool IsTemporary() const;

    inline mdkMatrixElementTypeEnum GetElementType() const;

    inline ElementType GetNaNElement() const;

    inline void SetNaNElement(mdkMatrixElementTypeEnum ElementType);

    inline void SetNaNElement(ElementType NaNElement);

    inline const std::shared_ptr<std::vector<ElementType>>& GetElementDataSharedPointer() const;

	inline ElementType* GetElementDataRawPointer();

	inline const ElementType* GetElementDataRawPointer() const;

	//---------------------- Matrix Size ----------------------------------------//

	inline bool SetSize(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

    inline bool Reshape(uint64 RowNumber, uint64 ColNumber);

    inline void FixSize(bool Fix = true);

    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

	inline mdkMatrixSize GetSize() const;

    inline void GetSize(uint64* RowNumber, uint64* ColNumber) const;

    inline uint64 GetElementNumber() const;

	inline uint64 GetColNumber() const;

	inline uint64 GetRowNumber() const;

	//---------------------- Initilize Matrix ----------------------------------------//

	template<typename ElementType_target>
	inline void operator=(const mdkMatrix<ElementType_target>& targetMatrix);

	inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& list);

    template<typename ElementType_target>
    inline void operator=(const mdkShadowMatrix<ElementType_target>& ShadowMatrix);

    template<typename ElementType_target>
    inline void operator=(const mdkGlueMatrix<ElementType_target>& GlueMatrix);

	template<typename ElementType_target>
	inline bool Copy(const mdkMatrix<ElementType_target>& targetMatrix);

	template<typename ElementType_target>
	inline bool Copy(const ElementType_target* ElementPointer, uint64 RowNumber, uint64 ColNumber);

	inline bool Fill(const ElementType& Element);

    //----------- Special Copy : share data ------------------------------------------------------------------------//

    inline void SharedCopy(const mdkMatrix<ElementType>& targetMatrix);

	//----------- Get/Set Matrix(LinearIndex) : size can not be changed even if m_IsSizeFixed is false -----------------//

	inline ElementType& operator()(uint64 LinearIndex);

	inline const ElementType& operator()(uint64 LinearIndex) const;

	inline ElementType& operator[](uint64 LinearIndex);

	inline const ElementType& operator[](uint64 LinearIndex) const;

	inline ElementType& at(uint64 LinearIndex);

	inline const ElementType& at(uint64 LinearIndex) const;

	//----------- Get/Set Matrix(i,j)  : size can not be changed even if m_IsSizeFixed is false -------------------//

	inline ElementType& operator()(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& operator()(uint64 RowIndex, uint64 ColIndex) const;

	inline ElementType& at(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& at(uint64 RowIndex, uint64 ColIndex) const;

    //---------------------- Get/Set SubMatrix ------------------------------------------------------//

    inline mdkShadowMatrix<ElementType> operator()(const std::initializer_list<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::initializer_list<uint64>& RowIndexList,
                                                   const std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<uint64>& RowIndexList,
                                                   const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, 
                                                  uint64 ColIndex_start, uint64 ColIndex_end);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::initializer_list<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::initializer_list<uint64>& RowIndexList,
                                                  const std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& RowIndexList,
                                                  const std::vector<uint64>& ColIndexList);

	inline mdkMatrix GetSubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, uint64 ColIndex_start, uint64 ColIndex_end) const;

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList) const;

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& LinearIndexList) const;

	//---------------------- Get/Set/Fill/Append/Delete Column ----------------------------------------//
	
    inline mdkShadowMatrix<ElementType> Col(uint64 ColIndex);

    // do not use Col(const std::initializer_list<uint64>& ColIndexList); 
    // it leads to ambiguous call (vs2013), 
    // e.g., Col({0})  it can initialize Col(uint64) or Col(std::vector);
    //
    inline mdkShadowMatrix<ElementType> Col(std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> Col(const std::vector<uint64>& ColIndexList);

    inline mdkMatrix GetCol(uint64 ColIndex) const;

    inline bool GetCol(uint64 ColIndex, std::vector<ElementType>& ColData) const;

    inline bool GetCol(uint64 ColIndex, ElementType* ColData) const;

    template<typename ElementType_input>
    inline bool SetCol(uint64 ColIndex, const mdkMatrix<ElementType_input>& ColData);

    template<typename ElementType_input>
    inline bool SetCol(uint64 ColIndex, const std::initializer_list<ElementType_input>& ColData);

    template<typename ElementType_input>
    inline bool SetCol(uint64 ColIndex, const std::vector<ElementType_input>& ColData);

	template<typename ElementType_input>
    inline bool SetCol(uint64 ColIndex, const ElementType_input* ColData, uint64 Length);
	
    inline bool FillCol(uint64 ColIndex, const ElementType& Element);

    template<typename ElementType_input>
    inline bool AppendCol(const mdkMatrix<ElementType_input>& ColData);

    template<typename ElementType_input>
    inline bool AppendCol(const std::initializer_list<ElementType_input>& ColData);

    template<typename ElementType_input>
    inline bool AppendCol(const std::vector<ElementType_input>& ColData);

	template<typename ElementType_input>
    inline bool AppendCol(const ElementType_input* ColData, uint64 Length);
	
    inline bool DeleteCol(uint64 ColIndex);

    // no const before std::initializer_list
    inline bool DeleteCol(std::initializer_list<uint64>& ColIndexList);

    inline bool DeleteCol(const std::vector<uint64>& ColIndexList);

    inline bool DeleteCol(const uint64* ColIndexPtr, uint64 Length);

	//---------------------- Get/Set/Fill/Append/Delete Row  ----------------------------------------//
	
    inline mdkShadowMatrix<ElementType> Row(uint64 RowIndex);

    inline mdkShadowMatrix<ElementType> Row(std::initializer_list<uint64>& RowIndexList);

    inline mdkShadowMatrix<ElementType> Row(const std::vector<uint64>& RowIndexList);

    inline mdkMatrix GetRow(uint64 RowIndex) const;

    inline bool GetRow(uint64 RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(uint64 ColIndex, ElementType* RowData) const;

    template<typename ElementType_input>
    inline bool SetRow(uint64 ColIndex, const mdkMatrix<ElementType_input>& RowData);

    template<typename ElementType_input>
    inline bool SetRow(uint64 RowIndex, const std::initializer_list<ElementType_input>& RowData);

    template<typename ElementType_input>
    inline bool SetRow(uint64 RowIndex, const std::vector<ElementType_input>& RowData);

	template<typename ElementType_input>
    inline bool SetRow(uint64 RowIndex, const ElementType_input* RowData, uint64 Length);

    inline bool FillRow(uint64 RowIndex, const ElementType& Element);

    template<typename ElementType_input>
    inline bool AppendRow(const mdkMatrix<ElementType_input>& RowData);

    template<typename ElementType_input>
    inline bool AppendRow(const std::initializer_list<ElementType_input>& RowData);

    template<typename ElementType_input>
    inline bool AppendRow(const std::vector<ElementType_input>& RowData);

	template<typename ElementType_input>
    inline bool AppendRow(const ElementType_input* RowData, uint64 Length);

    inline bool DeleteRow(uint64 RowIndex);

    // no const before std::initializer_list
    inline bool DeleteRow(std::initializer_list<uint64>& RowIndexList);

    inline bool DeleteRow(const std::vector<uint64>& RowIndexList);

    inline bool DeleteRow(const uint64* RowIndexPtr, uint64 Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline mdkShadowMatrix<ElementType> Diangonal();

    inline mdkMatrix GetDiangonal() const;

    inline bool GetDiangonal(std::vector<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(ElementType* DiangonalData) const;

    template<typename ElementType_input>
    inline bool SetDiangonal(const mdkMatrix<ElementType_input>& DiangonalData);

    template<typename ElementType_input>
    inline bool SetDiangonal(const std::initializer_list<ElementType_input>& DiangonalData);

    template<typename ElementType_input>
    inline bool SetDiangonal(const std::vector<ElementType_input>& DiangonalData);

	template<typename ElementType_input>
	inline bool SetDiangonal(const ElementType_input* DiangonalData);

    inline bool FillDiangonal(const ElementType& Element);
	//---------------------- Matrix {+= -= *= /=} Matrix ----------------------------------------//

	inline void operator+=(const mdkMatrix<ElementType>& targetMatrix);

	inline void operator-=(const mdkMatrix<ElementType>& targetMatrix);

	inline void operator*=(const mdkMatrix<ElementType>& targetMatrix);

	inline void operator/=(const mdkMatrix<ElementType>& targetMatrix);

    //---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

	// error:
	//template<typename ElementType_target>
	//inline void operator+(ElementType_target Element);

	inline void operator+=(ElementType Element);

	inline void operator-=(ElementType Element);

	inline void operator*=(ElementType Element);

	inline void operator/=(ElementType Element);

	//-------------------- element operation : output a new matrix ------------------------------------------//

    inline mdkMatrix ElementOperation(const char* FunctionName) const;

    inline mdkMatrix ElementOperation(const std::string& FunctionName) const;

	template<typename FunctionType>
    inline mdkMatrix ElementOperation(FunctionType Function) const;

	template<typename ElementType_target>
    inline mdkMatrix ElementOperation(const char* FunctionName, const mdkMatrix<ElementType_target>& targetMatrix) const;

	template<typename ElementType_target>
    inline mdkMatrix ElementOperation(const std::string& FunctionName, const mdkMatrix<ElementType_target>& targetMatrix) const;

	template<typename FunctionType, typename ElementType_target>
    inline mdkMatrix ElementOperation(FunctionType Function, const mdkMatrix<ElementType_target>& targetMatrix) const;

	inline mdkMatrix ElementOperation(const char* FunctionName, ElementType Element) const;

    inline mdkMatrix ElementOperation(const std::string& FunctionName, ElementType Element) const;

	template<typename FunctionType>
    inline mdkMatrix ElementOperation(FunctionType Function, ElementType Element) const;

    //-------------------- element operation on self ------------------------------------------------------//

    inline bool ElementOperationOnSelf(const char* FunctionName);

    inline bool ElementOperationOnSelf(const std::string& FunctionName);

    template<typename FunctionType>
    inline bool ElementOperationOnSelf(FunctionType Function);

    template<typename ElementType_target>
    inline bool ElementOperationOnSelf(const char* FunctionName, const mdkMatrix<ElementType_target>& targetMatrix);

    template<typename ElementType_target>
    inline bool ElementOperationOnSelf(const std::string& FunctionName, const mdkMatrix<ElementType_target>& targetMatrix);

    template<typename FunctionType, typename ElementType_target>
    inline bool ElementOperationOnSelf(FunctionType Function, const mdkMatrix<ElementType_target>& targetMatrix);

    inline bool ElementOperationOnSelf(const char* FunctionName, ElementType Element);

    inline bool ElementOperationOnSelf(const std::string& FunctionName, ElementType Element);

    template<typename FunctionType>
    inline bool ElementOperationOnSelf(FunctionType Function, ElementType Element);

	//-------------------- calculate sum mean min max ------------------------------------------//

	inline ElementType Mean();

	inline mdkMatrix MeanToRow();

    inline mdkMatrix MeanToCol();

	inline ElementType Sum();

    inline mdkMatrix SumToRow();

    inline mdkMatrix SumToCol();

	inline ElementType Max();

    inline mdkMatrix MaxToRow();

    inline mdkMatrix MaxToCol();

	inline ElementType Min();

    inline mdkMatrix MinToRow();

    inline mdkMatrix MinToCol();

	//----------------------------------- transpose -----------------------------------------//

	inline mdkMatrix GetTranspose();

    inline mdkMatrix Tran();

	//----------------------------------- Rank -----------------------------------------//

	inline uint64 Rank();

	//----------------------------------- inverse -----------------------------------------//

	inline mdkMatrix Inv();

	inline mdkMatrix PseudoInv();

	//----------------------------------- SVD -----------------------------------------//

	inline mdkMatrixSVDResult<ElementType> SVD();	


	//---------------------------- private functions ---------------------------------------//

private:

};

}//end namespace mdk

#include "mdkMatrix.hpp"

#endif