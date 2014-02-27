#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
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

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

//element-wise multiplication
template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// ----------------------- Element {+ - * /} Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(ElementType Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(ElementType Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(ElementType Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(ElementType Element, const mdkMatrix<ElementType>& Matrix);

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

#define MDK_Matrix_ColExpansionStep  100

#define MDK_Matrix_RowExpansionStep  100

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

	ElementType  m_EmptyElement; // may not be ZeroElement, but ZeroElement = m_EmptyElement - m_EmptyElement;

	bool m_IsTemporary; // a Matrix returned from a function is a temporary matrix

public:		
	
	//------------------- constructor and destructor ------------------------------------//

	inline mdkMatrix();

	inline mdkMatrix(const mdkMatrix<ElementType>& targetMatrix);

    inline mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix);

	inline mdkMatrix(uint64 RowNumber, uint64 ColNumber = 1, bool IsSizeFixed = false);

    inline mdkMatrix(ElementType* ElementPointer, uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = true);

	inline ~mdkMatrix();

    //-----------------------------------------------------------------------------------//

    inline void Clear();

    inline ElementType GetEmptyElement();

    inline std::vector<ElementType>* GetElementDataArrayPointer();

    inline std::shared_ptr<std::vector<ElementType>>& GetElementDataSharedPointer();

	inline const std::shared_ptr<std::vector<ElementType>>& GetElementDataSharedPointer() const;

	inline ElementType* GetElementDataRawPointer();

	inline const ElementType* GetElementDataRawPointer() const;

	inline mdkMatrixElementTypeEnum GetElementType() const;

	inline void SetTobeTemporary();

	inline bool IsTemporary() const;

	//---------------------- Matrix Size ----------------------------------------//

	inline bool SetSize(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

    inline void FixSize(bool Fix = true);

	inline bool Reshape(uint64 RowNumber, uint64 ColNumber);

	inline void GetSize(uint64* RowNumber, uint64* ColNumber) const;

	inline mdkMatrixSize GetSize() const;

    inline uint64 GetElementNumber() const;

	inline uint64 GetColNumber() const;

	inline uint64 GetRowNumber() const;

	inline bool IsSizeFixed() const;

	inline bool IsEmpty() const;

	//---------------------- Initilize Matrix ----------------------------------------//

	template<typename ElementType_target>
	inline void operator=(const mdkMatrix<ElementType_target>& targetMatrix);

	inline void operator=(const ElementType& Element);

	inline void operator=(const std::initializer_list<ElementType>& list);

    inline void operator=(const std::vector<ElementType>& ColVector);

    inline void operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

	template<typename ElementType_target>
	inline bool Copy(const mdkMatrix<ElementType_target>& targetMatrix);

	template<typename ElementType_target>
	inline bool Copy(const ElementType_target* ElementPointer, uint64 RowNumber, uint64 ColNumber);

	inline bool Fill(const ElementType& Element);

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

    //---------------------- Get/Set SubMatrix by using ShadowMatrix -----------------------------//

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<uint64>& RowIndexList,
                                                   const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<uint64>& LinearIndexList);

	//---------------------- Get SubMatrix -------------------------------------------------//

    inline mdkShadowMatrix<ElementType> SubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, uint64 ColIndex_start, uint64 ColIndex_end);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& RowIndexList,
                                                  const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& LinearIndexList);

	inline mdkMatrix GetSubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, uint64 ColIndex_start, uint64 ColIndex_end);

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList);

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& LinearIndexList);

	//---------------------- Get/Set a column ----------------------------------------//
	
    inline mdkShadowMatrix<ElementType> Col(uint64 ColIndex);

    inline mdkShadowMatrix<ElementType> Col(const std::vector<uint64>& ColIndexList);

    inline mdkMatrix GetCol(uint64 ColIndex);

	inline bool GetCol(uint64 ColIndex, ElementType* ColData);

	inline bool GetCol(uint64 ColIndex, std::vector<ElementType>& ColData);

	template<typename ElementType_input>
	inline bool SetCol(uint64 ColIndex, const mdkMatrix<ElementType_input>& ColData);

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
	
    inline mdkShadowMatrix<ElementType> Row(uint64 RowIndex);

    inline mdkShadowMatrix<ElementType> Row(const std::vector<uint64>& RowIndexList);

    inline mdkMatrix GetRow(uint64 RowIndex);

	inline bool GetRow(uint64 ColIndex, ElementType* RowData);

	inline bool GetRow(uint64 RowIndex, std::vector<ElementType>& RowData);

	template<typename ElementType_input>
	inline bool SetRow(uint64 ColIndex, const mdkMatrix<ElementType_input>& RowData);

	template<typename ElementType_input>
	inline bool SetRow(uint64 RowIndex, const ElementType_input* RowData);

	template<typename ElementType_input>
	inline bool SetRow(uint64 RowIndex, const std::vector<ElementType_input>& RowData);

	template<typename ElementType_input>
	inline bool AppendRow(const mdkMatrix<ElementType_input>& RowData);

	template<typename ElementType_input>
	inline bool AppendRow(const ElementType_input* RowData);

	template<typename ElementType_input>
	inline bool AppendRow(const std::vector<ElementType_input>& RowData);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline mdkShadowMatrix<ElementType> Diangonal();

    inline mdkMatrix GetDiangonal();

	inline bool GetDiangonal(ElementType* DiangonalData);

	inline bool GetDiangonal(std::vector<ElementType>& DiangonalData);

	inline bool SetDiangonal(ElementType Element);

	template<typename ElementType_input>
	inline bool SetDiangonal(const mdkMatrix<ElementType_input>& DiangonalData);

	template<typename ElementType_input>
	inline bool SetDiangonal(const ElementType_input* DiangonalData);

	template<typename ElementType_input>
	inline bool SetDiangonal(const std::vector<ElementType_input>& DiangonalData);

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

	//-------------------- element operation  ------------------------------------------//

	inline mdkMatrix ElementOperation(const char* FunctionName);

	inline mdkMatrix ElementOperation(const std::string& FunctionName);

	template<typename FunctionType>
	inline mdkMatrix ElementOperation(FunctionType Function);

	template<typename ElementType_target>
	inline mdkMatrix ElementOperation(const char* FunctionName, const mdkMatrix<ElementType_target>& targetMatrix);

	template<typename ElementType_target>
	inline mdkMatrix ElementOperation(const std::string& FunctionName, const mdkMatrix<ElementType_target>& targetMatrix);

	template<typename FunctionType, typename ElementType_target>
	inline mdkMatrix ElementOperation(FunctionType Function, const mdkMatrix<ElementType_target>& targetMatrix);

	inline mdkMatrix ElementOperation(const char* FunctionName, ElementType Element);

	inline mdkMatrix ElementOperation(const std::string& FunctionName, ElementType Element);

	template<typename FunctionType>
	inline mdkMatrix ElementOperation(FunctionType Function, ElementType Element);

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
	template<typename ElementType_target>
	inline mdkMatrixElementTypeEnum FindElementType(ElementType_target Element);

};

}//end namespace mdk

#include "mdkMatrix.hpp"

#endif