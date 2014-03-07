#ifndef __mdkMatrix_h
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
class mdkLinearCombineGlueMatrix;

template<typename ElementType>
struct mdkMatrixSVDResult;

template<typename ElementType>
struct mdkMatrixPCAResult;

// end of  forward-declare  ------//

struct mdkMatrixSize
{
	uint64 RowNumber;  // RowNumber = the Number of Rows 
	uint64 ColNumber;  // ColNumber = the Number of Columns
};

// ------------------------------------ Matrix {+ - * % /}  Matrix ------------------------------------------------//

#if !defined MDK_Enable_GlueMatrix //---------------------------------------------

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

#endif //!defined MDK_Enable_GlueMatrix --------------------------------------------

//element-wise multiplication
template<typename ElementType>
inline mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// ------------------------------------ Matrix {+ - * /}  Element ------------------------------------------------//

#if !defined MDK_Enable_GlueMatrix //----------------------------------------

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

#endif // !defined MDK_Enable_GlueMatrix -------------------------------------

// ------------------------------------- Element {+ - * /} Matrix ------------------------------------------------//

#if !defined MDK_Enable_GlueMatrix  //----------------------

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

#endif // !defined MDK_Enable_GlueMatrix -------------------

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

// ----------------------- Element = Matrix (if 1x1) is not allowed in c++ --------------------------------------------//
// not allowed in c++
//template<typename ElementType>
//inline void operator=(ElementType& Element, const mdkMatrix<ElementType>& Matrix);


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

	std::shared_ptr<std::vector<ElementType>> m_ElementData;

	bool m_IsSizeFixed;

	mdkMatrixElementTypeEnum m_ElementType;

	ElementType  m_NaNElement; // NaN only valid for float and double 

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline mdkMatrix();

    inline mdkMatrix(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkMatrix<ElementType>& targetMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const ElementType* ElementPointer, uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

    inline mdkMatrix(std::vector<ElementType>* ElementDataPointer, uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

    // move constructor
    inline mdkMatrix(mdkMatrix<ElementType>&& targetMatrix, bool IsSizeFixed = false);

	inline ~mdkMatrix();

    //-----------------------------------------------------------------------------------//
    
    // set the initial state, only use it in constructor
    inline void Reset();

    // clear memory if Self is not empty and m_IsSizeFixed is false
    inline bool Clear();

    // force-clear memory, set to be empty, only use it in move constructor and "="
    inline void ForceClear();

    //-----------------------------------------------------------------------------//

    inline mdkMatrixElementTypeEnum GetElementType() const;

    inline ElementType GetNaNElement() const;

    inline void SetNaNElement(mdkMatrixElementTypeEnum ElementType);

    inline void SetNaNElement(ElementType NaNElement);

    inline const std::shared_ptr<std::vector<ElementType>>& GetElementDataSharedPointer() const;

	inline ElementType* GetElementDataRawPointer();

	inline const ElementType* GetElementDataRawPointer() const;

	//---------------------- Matrix Size ----------------------------------------//

    inline bool Reshape(uint64 targetRowNumber, uint64 targetColNumber);

    inline bool Resize(uint64 targetRowNumber, uint64 targetColNumber);

    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

	inline mdkMatrixSize GetSize() const;

    inline void GetSize(uint64* RowNumber, uint64* ColNumber) const;

    inline uint64 GetElementNumber() const;

	inline uint64 GetColNumber() const;

	inline uint64 GetRowNumber() const;

	//---------------------- Initialize Matrix ----------------------------------------//

    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const mdkMatrix<ElementType>& targetMatrix);

    // move assignment operator
    inline void operator=(mdkMatrix<ElementType>&& targetMatrix);

	inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& list);

    inline void operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

	template<typename ElementType_target>
    inline bool Copy(const mdkMatrix<ElementType_target>& targetMatrix);

	template<typename ElementType_target>
    inline bool Copy(const ElementType_target* ElementPointer, uint64 RowNumber, uint64 ColNumber);

	inline bool Fill(const ElementType& Element);

    //----------- Special Copy : share data ------------------------------------------------------------------------//

    // 1: for shadowMatrix
    // 2: replace operator "=" : example: C.SharedCopy(A*B) is faster than C = A*B because no data copy from temp matrix to C
    inline void Share(const mdkMatrix<ElementType>& targetMatrix);

    inline void Share(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void Share(const mdkLinearCombineGlueMatrix<ElementType>& mdkLinearCombineGlueMatrix);

	//----------- Get/Set Matrix(LinearIndex) : size can not be changed even if m_IsSizeFixed is false -----------------//

	inline ElementType& operator()(uint64 LinearIndex);

    inline const ElementType& operator()(uint64 LinearIndex) const;

    #if defined MDK_Matrix_Enable_Operator_Bracket //------------------------------------

	inline ElementType& operator[](uint64 LinearIndex);
    
	inline const ElementType& operator[](uint64 LinearIndex) const;
    
    #endif // #if defined MDK_Matrix_Enable_Operator_Bracket //---------------------------

	inline ElementType& at(uint64 LinearIndex);

	inline const ElementType& at(uint64 LinearIndex) const;

	//----------- Get/Set Matrix(i,j)  : size can not be changed even if m_IsSizeFixed is false -------------------//

	inline ElementType& operator()(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& operator()(uint64 RowIndex, uint64 ColIndex) const;

	inline ElementType& at(uint64 RowIndex, uint64 ColIndex);

	inline const ElementType& at(uint64 RowIndex, uint64 ColIndex) const;

    //---------------------- Get/Set SubMatrix ------------------------------------------------------//

    inline mdkShadowMatrix<ElementType> operator()(std::initializer_list<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    #if defined MDK_Matrix_Enable_Operator_Bracket //------------------------------------
    
    inline mdkShadowMatrix<ElementType> operator[](std::initializer_list<uint64>& LinearIndexList);
    
    inline mdkShadowMatrix<ElementType> operator[](const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> operator[](const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    #endif //#if defined MDK_Matrix_Enable_Operator_Bracket //----------------------------

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

    inline mdkShadowMatrix<ElementType> SubMatrix(std::initializer_list<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(std::initializer_list<uint64>& RowIndexList,
                                                  std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::initializer_list<uint64>& RowIndexList,
                                                  const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix<ElementType> SubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                  const std::initializer_list<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& LinearIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& RowIndexList,
                                                  const std::vector<uint64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> SubMatrix(const std::vector<uint64>& RowIndexList,
                                                  const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline mdkShadowMatrix<ElementType> SubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                  const std::vector<uint64>& ColIndexList);

	inline mdkMatrix GetSubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, uint64 ColIndex_start, uint64 ColIndex_end) const;

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList) const;

    inline void GetSubMatrix(mdkMatrix<ElementType> &OutputMatrix, const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList) const;

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkMatrix GetSubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList) const;

    inline mdkMatrix GetSubMatrix(const std::vector<uint64>& LinearIndexList) const;

	//---------------------- Get/Set/Fill/Append/Delete/InsertCol Column ----------------------------------------//
	
    inline mdkShadowMatrix<ElementType> Col(uint64 ColIndex);

    // do not use const as in Col(const std::initializer_list<uint64>& ColIndexList); 
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

	//---------------------- Matrix {+= -= *= /= %=} Matrix ----------------------------------------//

	inline void operator+=(const mdkMatrix<ElementType>& targetMatrix);

	inline void operator-=(const mdkMatrix<ElementType>& targetMatrix);

	inline void operator*=(const mdkMatrix<ElementType>& targetMatrix);

	inline void operator/=(const mdkMatrix<ElementType>& targetMatrix);

    inline void operator%=(const mdkMatrix<ElementType>& targetMatrix);


    inline void operator+=(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

    inline void operator-=(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

    inline void operator*=(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

    inline void operator/=(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

    inline void operator%=(mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);


    inline void operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator%=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    //---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

	// error:
	//template<typename ElementType_target>
	//inline void operator+(ElementType_target Element);

	inline void operator+=(ElementType Element);

	inline void operator-=(ElementType Element);

	inline void operator*=(ElementType Element);

	inline void operator/=(ElementType Element);

    //-------------------- element operation {^} -----------------------------------------------------------//

    inline void operator^(double value);

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

    //-------------------- element operation in place ------------------------------------------------------//

    inline bool ElementOperationInPlace(const char* FunctionName);

    inline bool ElementOperationInPlace(const std::string& FunctionName);

    template<typename FunctionType>
    inline bool ElementOperationInPlace(FunctionType Function);

    template<typename ElementType_target>
    inline bool ElementOperationInPlace(const char* FunctionName, const mdkMatrix<ElementType_target>& targetMatrix);

    template<typename ElementType_target>
    inline bool ElementOperationInPlace(const std::string& FunctionName, const mdkMatrix<ElementType_target>& targetMatrix);

    template<typename FunctionType, typename ElementType_target>
    inline bool ElementOperationInPlace(FunctionType Function, const mdkMatrix<ElementType_target>& targetMatrix);

    inline bool ElementOperationInPlace(const char* FunctionName, ElementType Element);

    inline bool ElementOperationInPlace(const std::string& FunctionName, ElementType Element);

    template<typename FunctionType>
    inline bool ElementOperationInPlace(FunctionType Function, ElementType Element);

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