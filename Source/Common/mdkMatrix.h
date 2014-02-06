#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>
#include <functional>

#include "mdkObject.h"

namespace mdk
{

// 2D Matrix Class Template, each entry/element is a scalar
// column major
//
// If heavy linear algebra is required, then convert mdkMatrix to Armadillo matrix, do something, and convert back
// Armadillo is a linear algebra library, and it uses column major matrix
//
// The functions that are not supported in Armadillo, are provided in mdkLinearAlgebra.h/cpp

//forward-declare the template class
template<typename ElementType>
class mdkMatrix;

struct MatrixSize
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


// ----------------------- Matrix {+ - * /}  Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(mdkMatrix<ElementType>& MatrixA, mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(mdkMatrix<ElementType>& MatrixA, mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkMatrix<ElementType>& MatrixA, mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkMatrix<ElementType>& MatrixA, mdkMatrix<ElementType>& MatrixB);

// ----------------------- Element {+ - * /} Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(ElementType Element, mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(ElementType Element, mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(ElementType Element, mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(ElementType Element, mdkMatrix<ElementType>& Matrix);

// ----------------------- Matrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(mdkMatrix<ElementType>& Matrix, ElementType Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(mdkMatrix<ElementType>& Matrix, ElementType Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkMatrix<ElementType>& Matrix, ElementType Element);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkMatrix<ElementType>& Matrix, ElementType Element);
//--------------------------------------------------------------------------------------------------//


template<typename ElementType>
class mdkMatrix : public mdkObject
{

private:

	uint64 m_RowNumber;
	
	uint64 m_ColNumber;

	uint64 m_ElementNumber;  // total number of Elements

	std::shared_ptr<std::vector<ElementType>> m_ElementData;

	bool m_IsSizeFixed;

	mdkMatrixElementTypeEnum m_ElementType;

	uint64 m_ColExpansionStep;

	uint64 m_RowExpansionStep;

	ElementType  m_EmptyElement;

	bool m_IsTemporaryMatrix;

public:		
	
	//------------------- constructor and destructor ------------------------------------//

	inline mdkMatrix();

	inline mdkMatrix(const mdkMatrix<ElementType>& targetMatrix);

	inline void operator=(const mdk::mdkMatrix<double>& targetMatrix);

	inline mdkMatrix(uint64 RowNumber, uint64 ColNumber = 1, bool IsSizeFixed = false);

	inline ~mdkMatrix();

	//-----------------------------------------------------------------------------------//

	inline void Clear();

	inline std::shared_ptr<std::vector<ElementType>> GetElementDataSharedPointer() const;

	inline ElementType* GetElementDataRawPointer();

	inline const ElementType* GetElementDataRawPointer() const;

	inline mdkMatrixElementTypeEnum GetElementType() const;

	inline void SetTobeTemporaryMatrix();

	inline bool IsTemporaryMatrix() const;

	//---------------------- Matrix Size ----------------------------------------//

	inline bool SetSize(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

	inline void FixSize(bool Fix);

	inline bool ReSize(uint64 RowNumber, uint64 ColNumber);

	inline void GetSize(uint64* RowNumber, uint64* ColNumber) const;

	inline MatrixSize GetSize() const;

	inline uint64 GetColNumber() const;

	inline uint64 GetRowNumber() const;

	inline bool IsSizeFixed() const;

	inline bool IsEmpty() const;

	//---------------------- Initilize Matrix ----------------------------------------//

	template<typename ElementType_target>
	inline void operator=(const mdkMatrix<ElementType_target>& targetMatrix);

	inline void operator=(ElementType Element);

	inline void operator=(const std::initializer_list<ElementType>& list);

	template<typename ElementType_target>
	inline bool Copy(const mdkMatrix<ElementType_target>& targetMatrix);

	template<typename ElementType_target>
	inline bool Copy(const ElementType_target* ElementPointer, uint64 RowNumber, uint64 ColNumber);

	inline bool Fill(ElementType Element);

	//---------------------- Get/Set Matrix(LinearIndex) ----------------------------------------//

	inline ElementType& operator()(uint64 LinearIndex);

	inline const ElementType& operator()(uint64 LinearIndex) const;

	inline ElementType& operator[](uint64 LinearIndex);

	inline const ElementType& operator[](uint64 LinearIndex) const;

	inline ElementType& at(uint64 LinearIndex);

	inline const ElementType& at(uint64 LinearIndex) const;

	//---------------------- Get/Set Matrix(i,j) ----------------------------------------//

	inline ElementType& operator()(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& operator()(uint64 RowIndex, uint64 ColIndex) const;

	inline ElementType& at(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& at(uint64 RowIndex, uint64 ColIndex) const;

	//---------------------- Get Matrix(i_s to i_e, j_s to j_e) ----------------------------------------//

	inline mdkMatrix SubMatrix(uint64 RowIndex_s, uint64 RowIndex_e, uint64 ColIndex_s, uint64 ColIndex_e);

	//---------------------- Get/Set a column ----------------------------------------//
	
	inline mdkMatrix Col(uint64 ColIndex);

	inline bool GetCol(uint64 ColIndex, ElementType* ColData);

	inline bool GetCol(uint64 ColIndex, std::vector<ElementType>& ColData);

	template<typename ElementType_input>
	inline bool SetCol(uint64 ColIndex, mdkMatrix<ElementType_input>& ColData);

	template<typename ElementType_input>
	inline bool SetCol(uint64 ColIndex, const ElementType_input* ColData);

	template<typename ElementType_input>
	inline bool SetCol(uint64 ColIndex, const std::vector<ElementType_input>& ColData);

	template<typename ElementType_input>
	inline bool AppendCol(const mdkMatrix<ElementType_input>& ColData);

	template<typename ElementType_input>
	inline bool AppendCol(const ElementType_input* ColData);
	
	template<typename ElementType_input>
	inline bool AppendCol(const std::vector<ElementType_input>& ColData);

	//---------------------- Get/Set a row  ----------------------------------------//
	
	inline mdkMatrix Row(uint64 RowIndex);

	inline bool GetRow(uint64 ColIndex, ElementType* RowData);

	inline bool GetRow(uint64 RowIndex, std::vector<ElementType>& RowData);

	template<typename ElementType_input>
	inline bool SetRow(uint64 ColIndex, mdkMatrix<ElementType_input>& RowData);

	template<typename ElementType_input>
	inline bool SetRow(uint64 RowIndex, const ElementType_input* RowData);

	template<typename ElementType_input>
	inline bool SetRow(uint64 RowIndex, const std::vector<ElementType_input>& RowData);

	template<typename ElementType_input>
	inline bool AppendRow(mdkMatrix<ElementType_input>& RowData);

	template<typename ElementType_input>
	inline bool AppendRow(const ElementType_input* RowData);

	template<typename ElementType_input>
	inline bool AppendRow(const std::vector<ElementType_input>& RowData);

	//---------------------- Get/Set the diagonal ----------------------------------------//

	inline mdkMatrix Diangonal();

	inline bool GetDiangonal(ElementType* DiangonalData);

	inline bool GetDiangonal(std::vector<ElementType>& DiangonalData);

	inline bool SetDiangonal(ElementType Element);

	template<typename ElementType_input>
	inline bool SetDiangonal(mdkMatrix<ElementType_input>& DiangonalData);

	template<typename ElementType_input>
	inline bool SetDiangonal(const ElementType_input* DiangonalData);

	template<typename ElementType_input>
	inline bool SetDiangonal(const std::vector<ElementType_input>& DiangonalData);

	//---------------------- Matrix {+= -= *= /=} Matrix ----------------------------------------//

	inline void operator+=(mdkMatrix<ElementType>& targetMatrix);

	inline void operator-=(mdkMatrix<ElementType>& targetMatrix);

	inline void operator*=(mdkMatrix<ElementType>& targetMatrix);

	inline void operator/=(mdkMatrix<ElementType>& targetMatrix);

    //---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

	// error:
	//template<typename ElementType_target>
	//inline void operator+(ElementType_target Element);

	inline void operator+=(ElementType Element);

	inline void operator-=(ElementType Element);

	inline void operator*=(ElementType Element);

	inline void operator/=(ElementType Element);

	//-------------------- element operation  ------------------------------------------//

	inline mdkMatrix ElementOperation(const char* FunctionName);

	inline mdkMatrix ElementOperation(std::string FunctionName);

	inline mdkMatrix ElementOperation(std::function<ElementType(ElementType)> Function);

	template<typename ElementType_target>
	inline mdkMatrix ElementOperation(const char* FunctionName, mdkMatrix<ElementType_target>& targetMatrix);

	template<typename ElementType_target>
	inline mdkMatrix ElementOperation(std::string FunctionName, mdkMatrix<ElementType_target>& targetMatrix);

	template<typename ElementType_target>
	inline mdkMatrix ElementOperation(std::function<ElementType(ElementType, ElementType)> Function, mdkMatrix<ElementType_target>& targetMatrix);

	inline mdkMatrix ElementOperation(const char* FunctionName, ElementType Element);

	inline mdkMatrix ElementOperation(std::string FunctionName, ElementType Element);

	inline mdkMatrix ElementOperation(std::function<ElementType(ElementType, ElementType)> Function, ElementType Element);

	//-------------------- calculate sum mean min max ------------------------------------------//

	inline ElementType Mean();

	inline mdkMatrix MeanAlongRow();

	inline mdkMatrix MeanAlongCol();

	inline ElementType Sum();

	inline mdkMatrix SumAlongRow();

	inline mdkMatrix SumAlongCol();

	inline ElementType Max();

	inline mdkMatrix MaxAlongRow();

	inline mdkMatrix MaxAlongCol();

	inline ElementType Min();

	inline mdkMatrix MinAlongRow();

	inline mdkMatrix MinAlongCol();

	//----------------------------------- transpose -----------------------------------------//

	inline mdkMatrix GetTranspose();

	//----------------------------------- Rank -----------------------------------------//

	inline uint64 Rank();

	//----------------------------------- inverse -----------------------------------------//

	inline mdkMatrix Inv();

	inline mdkMatrix PseudoInv();

	//----------------------------------- SVD -----------------------------------------//

	inline mdkMatrixSVDResult<ElementType> SVD();	

	//---------------------------- private functions ---------------------------------------//

private:
	template<typename ElementType_target>
	inline mdkMatrixElementTypeEnum FindElementType(ElementType_target Element);

	template<typename ElementType_target>
	inline uint64 ByteNumberOfElement(ElementType_target Element);
};

}//end namespace mdk

#include "mdkMatrix.hpp"

#endif