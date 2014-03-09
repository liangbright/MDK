#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>
#include <functional>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
#include "mdkMatrixElement.h"
//#include "mdkShadowMatrix.h"


namespace mdk
{

// 2D Matrix Class Template, each entry/element is a scalar
// column major
//
// Compare to Matlab:
// mdkMatrix API very similar to Matlab matrix
//
// Compare to Armadillo  (a linear algebra library, and it uses column major matrix)
// mdkMatrix API better than Armadillo
//
//

//forward-declare ----------------//
template<typename ElementType>
class mdkMatrix;

template<typename ElementType>
class mdkShadowMatrix;

template<typename ElementType>
class mdkGlueMatrixForLinearCombination;

template<typename ElementType>
class mdkGlueMatrixForMultiplication;

template<typename ElementType>
struct mdkMatrixSVDResult;

// end of  forward-declare  ------//

// ----------------------------- mdkMatrixSize struct -------------------------------------------------------------//

struct mdkMatrixSize
{
	uint64 RowNumber;  // RowNumber = the Number of Rows 
	uint64 ColNumber;  // ColNumber = the Number of Columns
};

//============================================= For Reference ===================================================//
/*
// ------------------------------------ Matrix {+ - * /}  Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// ------------------------------------ Matrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

// ------------------------------------- Element {+ - * /} Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

// ----------------------- Element = Matrix (if 1x1) is not allowed in c++ --------------------------------------------//
// not allowed in c++
//template<typename ElementType>
//inline void operator=(ElementType& Element, const mdkMatrix<ElementType>& Matrix);
*/
//=====================================================================================================================//


//------------------------------------------- ALL Symbol --------------------------------------------------------------------------//

struct ALL_Symbol_For_mdkMatrix_Operator
{
    ALL_Symbol_For_mdkMatrix_Operator(const std::string& str)
    {
        if (str != "This_Is_ALL_Symbol_For_mdkMatrix_Operator")
        {
            mdkError << "ALL Symbol error @ ALL_Symbol_For_mdkMatrix_Operator" << '\n';
        }
    }

    ~ALL_Symbol_For_mdkMatrix_Operator() {}

// deleted:
    ALL_Symbol_For_mdkMatrix_Operator() = delete;
    ALL_Symbol_For_mdkMatrix_Operator(const ALL_Symbol_For_mdkMatrix_Operator&) = delete;
    void operator=(const ALL_Symbol_For_mdkMatrix_Operator&) = delete;
};

static ALL_Symbol_For_mdkMatrix_Operator This_Is_ALL_Symbol_For_mdkMatrix_Operator("This_Is_ALL_Symbol_For_mdkMatrix_Operator");

//refer to all the cols or rows, or all the elements
#define ALL This_Is_ALL_Symbol_For_mdkMatrix_Operator

//-----------------------------------------------------------------------------------------------------------------------------//

#define MDK_Matrix_ColExpansionStep  100

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkMatrix : public mdkObject
{

private:

	uint64 m_RowNumber;
	
	uint64 m_ColNumber;

    uint64 m_ElementNumber;

	std::shared_ptr<std::vector<ElementType>> m_ElementData;

    ElementType m_NaNElement;

	bool m_IsSizeFixed;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline mdkMatrix();

    inline mdkMatrix(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkMatrix<ElementType>& targetMatrix, bool IsSizeFixed = false);

    // move constructor
    inline mdkMatrix(mdkMatrix<ElementType>&& targetMatrix);

    inline mdkMatrix(const ElementType& Element);

    inline mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(ElementType* ElementPointer, uint64 RowNumber, uint64 ColNumber, bool IsInPlaceConstruction = false, bool IsSizeFixed = false);

	inline ~mdkMatrix();

    //----------------------  operator=  ----------------------------------------//

    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const mdkMatrix<ElementType>& targetMatrix);

    // move assignment operator
    inline void operator=(mdkMatrix<ElementType>&& targetMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& list);

    inline void operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //----------------------  Copy From Matrix or Element  ----------------------------------------//

    // Copy can be used to convert a matrix from double (ElementType_target) to float (ElementType), etc

    template<typename ElementType_target>  
    inline bool Copy(const mdkMatrix<ElementType_target>& targetMatrix);

    template<typename ElementType_target>
    inline bool Copy(const ElementType_target* ElementPointer, uint64 RowNumber, uint64 ColNumber);

    inline bool Fill(const ElementType& Element);

    // copy by share
    inline bool SharedCopy(const mdkMatrix<ElementType>& targetMatrix);

    //-------------------- Take : the the ownership of the input matrix ---------------------------//

    inline bool Take(mdkMatrix<ElementType>& targetMatrix);

    inline bool Take(mdkMatrix<ElementType>&& targetMatrix);

    inline bool Take(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline bool Take(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline bool Take(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //------------------------- Reset , Clear -------------------------------------------//
    
    // set the initial state, only use it in constructor
    inline void Reset();

    // clear memory if Self is not empty and m_IsSizeFixed is false
    inline bool Clear();

    // force-clear memory, set to be empty, only use it in move constructor and "=", and Take
    inline void ForceClear();

	//---------------------- Set/get Matrix Size, Shape ----------------------------------------//

    inline bool Reshape(uint64 targetRowNumber, uint64 targetColNumber);

    inline bool Resize(uint64 targetRowNumber, uint64 targetColNumber, bool IsSizeFixed = false);

    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

	inline mdkMatrixSize GetSize() const;

    inline void GetSize(uint64* RowNumber, uint64* ColNumber) const;

    inline uint64 GetElementNumber() const;

	inline uint64 GetColNumber() const;

	inline uint64 GetRowNumber() const;

    //------------------------ NaN Element -----------------------------//

    inline const ElementType& GetNaNElement() const;

    //------------------------ Element Type -----------------------------//

    inline mdkMatrixElementTypeEnum GetElementType() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline const std::shared_ptr<std::vector<ElementType>>& GetElementDataSharedPointer() const;

    inline ElementType* GetElementDataRawPointer();

    inline const ElementType* GetElementDataRawPointer() const;

	//----------- Get/Set Matrix(LinearIndex) -----------------------------------//

    // operator[]: no bound check

    inline ElementType& operator[](uint64 LinearIndex);

    inline const ElementType& operator[](uint64 LinearIndex) const;

	inline ElementType& operator()(uint64 LinearIndex);

    inline const ElementType& operator()(uint64 LinearIndex) const;
    
	inline ElementType& at(uint64 LinearIndex);

	inline const ElementType& at(uint64 LinearIndex) const;

	//----------- Get/Set Matrix(i,j)  ---------------------------------------------//

	inline ElementType& operator()(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& operator()(uint64 RowIndex, uint64 ColIndex) const;

	inline ElementType& at(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& at(uint64 RowIndex, uint64 ColIndex) const;

    //---------------------- Get/Set a set of elements ------------------------------------------------------//

    // operator[]: no bound check

    inline mdkShadowMatrix<ElementType> operator[](std::initializer_list<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator[](const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator[](const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix<ElementType> operator()(std::initializer_list<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);   

    //---------------------- Get/Set SubMatrix ------------------------------------------------------//

    inline mdkShadowMatrix<ElementType> operator()(std::initializer_list<uint64>& RowIndexList,
                                                   std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::initializer_list<uint64>& RowIndexList, 
                                                   const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                   const std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<uint64>& RowIndexList,
                                                   const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<uint64>& RowIndexList,
                                                   const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                   const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, 
                                                  uint64 ColIndex_start, uint64 ColIndex_end);

    inline ElementType& SubMatrix(uint64 RowIndex, uint64 ColIndex);

    inline mdkShadowMatrix<ElementType> SubMatrix(std::initializer_list<uint64>& RowIndexList,
                                                  std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::initializer_list<uint64>& RowIndexList,
                                                  const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix<ElementType> SubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                  const std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& RowIndexList,
                                                  const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& RowIndexList,
                                                  const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix<ElementType> SubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                  const std::vector<uint64>& ColIndexList);

	inline mdkMatrix GetSubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, uint64 ColIndex_start, uint64 ColIndex_end) const;

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList) const;

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkMatrix GetSubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList) const;

    inline bool GetSubMatrix(mdkMatrix<ElementType> &OutputMatrix, const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList) const;

	//---------------------- Get/Set/Fill/Append/Delete/InsertCol Column ----------------------------------------//
	
    inline mdkShadowMatrix<ElementType> Col(uint64 ColIndex);

    // do not use const in Col(const std::initializer_list<uint64>& ColIndexList); 
    // it leads to ambiguous call (vs2013), 
    // e.g., Col({0})  it can initialize Col(uint64) or Col(std::vector);
    //
    // so: use std::initializer_list<uint64> without const 
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

    template<typename ElementType_input>
    inline bool InsertCol(uint64 ColIndex, const mdkMatrix<ElementType_input>& ColData);

    template<typename ElementType_input>
    inline bool InsertCol(uint64 ColIndex, const std::initializer_list<ElementType_input>& ColData);

    template<typename ElementType_input>
    inline bool InsertCol(uint64 ColIndex, const std::vector<ElementType_input>& ColData);

    template<typename ElementType_input>
    inline bool InsertCol(uint64 ColIndex, const ElementType_input* ColData, uint64 Length);

	//---------------------- Get/Set/Fill/Append/Delete Row  ----------------------------------------//
	
    inline mdkShadowMatrix<ElementType> Row(uint64 RowIndex);

    inline mdkShadowMatrix<ElementType> Row(std::initializer_list<uint64>& RowIndexList);

    inline mdkShadowMatrix<ElementType> Row(const std::vector<uint64>& RowIndexList);

    inline mdkMatrix GetRow(uint64 RowIndex) const;

    inline bool GetRow(uint64 RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(uint64 RowIndex, ElementType* RowData) const;

    template<typename ElementType_input>
    inline bool SetRow(uint64 RowIndex, const mdkMatrix<ElementType_input>& RowData);

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

    template<typename ElementType_input>
    inline bool InsertRow(uint64 RowIndex, const mdkMatrix<ElementType_input>& RowData);

    template<typename ElementType_input>
    inline bool InsertRow(uint64 RowIndex, const std::initializer_list<ElementType_input>& RowData);

    template<typename ElementType_input>
    inline bool InsertRow(uint64 RowIndex, const std::vector<ElementType_input>& RowData);

    template<typename ElementType_input>
    inline bool InsertRow(uint64 RowIndex, const ElementType_input* RowData, uint64 Length);

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


    inline void operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix);


    inline void operator+=(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator-=(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator*=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);


    inline void operator+=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator-=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator*=(mdkGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator/=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

	// error:
	//template<typename ElementType_target>
	//inline void operator+(ElementType_target Element);

	inline void operator+=(ElementType Element);

	inline void operator-=(ElementType Element);

	inline void operator*=(ElementType Element);

	inline void operator/=(ElementType Element);

    //-------------------- element operation {^} -----------------------------------------------------------//

    inline mdkMatrix operator^(ElementType Element);

    inline void operator^=(ElementType Element);

    //-------------------- special element operation : (.*) element multiply -----------------------------------------------------------//

    inline mdkMatrix ElementMultiply(const mdkMatrix<ElementType>& targetMatrix);

    inline mdkMatrix ElementMultiply(const ElementType& Element);

    inline mdkMatrix ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkMatrix ElementMultiply(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline mdkMatrix ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

	//-------------------- general element operation : output a new matrix ------------------------------------------//

    inline mdkMatrix ElementOperation(const char* OperationName) const;

    inline mdkMatrix ElementOperation(const std::string& OperationName) const;

    inline mdkMatrix ElementOperation(std::function<ElementType(const ElementType&)> Operation) const;

    inline mdkMatrix ElementOperation(const std::string& OperationName, const mdkMatrix<ElementType>& targetMatrix) const;

    inline mdkMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, 
                                      const mdkMatrix<ElementType>& targetMatrix) const;

    inline mdkMatrix ElementOperation(const char* OperationName, const ElementType& Element) const;

    inline mdkMatrix ElementOperation(const std::string& OperationName, const ElementType& Element) const;

    inline mdkMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element) const;

    //-------------------- general element operation in place : Object.ElementOperationInPlace modify the object ------------------//

    inline bool ElementOperationInPlace(const char* OperationName);

    inline bool ElementOperationInPlace(const std::string& OperationName);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&)> Operation);

    inline bool ElementOperationInPlace(const char* OperationName, const mdkMatrix<ElementType>& targetMatrix);

    inline bool ElementOperationInPlace(const std::string& OperationName, const mdkMatrix<ElementType>& targetMatrix);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                        const mdkMatrix<ElementType>& targetMatrix);

    inline bool ElementOperationInPlace(const char* OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(const std::string& OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element);

	//-------------------- calculate sum mean min max ------------------------------------------//

    inline ElementType Mean() const;

    inline mdkMatrix MeanToRow() const;

    inline mdkMatrix MeanToCol() const;

    inline ElementType Sum() const;

    inline mdkMatrix SumToRow() const;

    inline mdkMatrix SumToCol() const;

    inline ElementType Max() const;

    inline mdkMatrix MaxToRow() const;

    inline mdkMatrix MaxToCol() const;

    inline ElementType Min() const;

    inline mdkMatrix MinToRow() const;

    inline mdkMatrix MinToCol() const;

	//----------------------------------- transpose -----------------------------------------//

    inline mdkMatrix Transpose() const;

	//----------------------------------- Rank -----------------------------------------//

    inline uint64 Rank() const;

	//----------------------------------- inverse -----------------------------------------//

    inline mdkMatrix Inv() const;

    inline mdkMatrix PseudoInv() const;

	//----------------------------------- SVD -----------------------------------------//

    inline mdkMatrixSVDResult<ElementType> SVD() const;


	//---------------------------- private functions ---------------------------------------//

private:

};

}//end namespace mdk

#include "mdkMatrix.hpp"

#endif
