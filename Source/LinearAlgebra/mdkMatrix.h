﻿#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include <vector>
#include <memory>
#include <string>
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
	int64 RowNumber;  // RowNumber = the Number of Rows 
	int64 ColNumber;  // ColNumber = the Number of Columns
};

// ----------------------------- mdkMatrixData struct -------------------------------------------------------------//

template<typename ElementType>
struct mdkMatrixData
{
    int64 RowNumber;  // RowNumber = the Number of Rows 
    int64 ColNumber;  // ColNumber = the Number of Columns

    std::vector<ElementType> DataArray;
//-------------------------------------------------------------
    mdkMatrixData() 
    {
        RowNumber = 0;
        ColNumber = 0;
    };

    ~mdkMatrixData() {};

    ElementType& operator[](int64 LinearIndex)
    {
        return DataArray[LinearIndex];
    }

    const ElementType& operator[](int64 LinearIndex) const
    {
        return DataArray[LinearIndex];
    }

    ElementType& operator()(int64 LinearIndex)
    {
        return DataArray[LinearIndex];
    }

    const ElementType& operator()(int64 LinearIndex) const
    {
        return DataArray[LinearIndex];
    }

    ElementType& operator()(int64 RowIndex, int64 ColIndex)
    {
        return DataArray[ColIndex*RowNumber + RowIndex];
    }

    const ElementType& operator()(int64 RowIndex, int64 ColIndex) const
    {
        return DataArray[ColIndex*RowNumber + RowIndex];
    }

private:
//deleted: -------------------------------------------------
    mdkMatrixData(const mdkMatrixData&) = delete;

    mdkMatrixData(mdkMatrixData&&) = delete;

    void operator=(const mdkMatrixData&) = delete;

    void operator=(mdkMatrixData&&) = delete;
};

//------------------------------------------- ALL Symbol --------------------------------------------------------------------------//

struct ALL_Symbol_InputStruct_For_ALL_Symbol_For_mdkMatrix_Operator
{
    std::string Name = "This_Is_For_ALL_Symbol_For_mdkMatrix_Operator";
};

struct ALL_Symbol_For_mdkMatrix_Operator
{
    ALL_Symbol_For_mdkMatrix_Operator(const ALL_Symbol_InputStruct_For_ALL_Symbol_For_mdkMatrix_Operator& InputStruct)
    {
        if (InputStruct.Name != "This_Is_For_ALL_Symbol_For_mdkMatrix_Operator")
        {
            mdkError << "ALL Symbol error @ ALL_Symbol_For_mdkMatrix_Operator" << '\n';
        }
    }

    ~ALL_Symbol_For_mdkMatrix_Operator() {}

// deleted:
    ALL_Symbol_For_mdkMatrix_Operator() = delete;
    ALL_Symbol_For_mdkMatrix_Operator(const ALL_Symbol_For_mdkMatrix_Operator&) = delete;
    ALL_Symbol_For_mdkMatrix_Operator(ALL_Symbol_For_mdkMatrix_Operator&&) = delete;
    void operator=(const ALL_Symbol_For_mdkMatrix_Operator&) = delete;
    void operator=(ALL_Symbol_For_mdkMatrix_Operator&&) = delete;
};

static ALL_Symbol_InputStruct_For_ALL_Symbol_For_mdkMatrix_Operator This_Is_ALL_Symbol_InputStruct_For_ALL_Symbol_For_mdkMatrix_Operator;

static ALL_Symbol_For_mdkMatrix_Operator This_Is_ALL_Symbol_For_mdkMatrix_Operator(This_Is_ALL_Symbol_InputStruct_For_ALL_Symbol_For_mdkMatrix_Operator);

//refer to all the cols or rows, or all the elements
#define ALL This_Is_ALL_Symbol_For_mdkMatrix_Operator

//-----------------------------------span: e.g., span(1,10) is 1:10 in Matlab, or span(1, 2, 10) is 1:2:10 in Matlab -----------------//
// note: definition in mdkMatrix.cpp

std::vector<int64> span(int64 Index_A, int64 Index_B);
std::vector<int64> span(int64 Index_A, int64 Step, int64 Index_B);

//-----------------------------------------------------------------------------------------------------------------------------//


#define MDK_Matrix_ColExpansionStep  100

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkMatrix : public mdkObject
{

private:
     
    std::shared_ptr<mdkMatrixData<ElementType>> m_MatrixData;

    ElementType* m_ElementPointer; // pointer to the first element, keep tracking m_MatrixData->DataArray.data()

    ElementType m_NaNElement;

	bool m_IsSizeFixed;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline mdkMatrix();

    inline mdkMatrix(int64 RowNumber, int64 ColNumber, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkMatrix<ElementType>& InputMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const ElementType& Element);

    // move constructor
    inline mdkMatrix(mdkMatrix<ElementType>&& InputMatrix);

    inline mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix, bool IsSizeFixed = false);

    inline mdkMatrix(const ElementType* InputElementPointer, int64 InputRowNumber, int64 InputColNumber, bool IsSizeFixed = false);

	inline ~mdkMatrix();

    //----------------------  operator=  ----------------------------------------//

    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const mdkMatrix<ElementType>& InputMatrix);

    // move assignment operator
    inline void operator=(mdkMatrix<ElementType>&& InputMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& list);

    inline void operator=(const std::initializer_list<std::initializer_list<ElementType>>& list);

    inline void operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //----------------------  Copy From Matrix or Element  ----------------------------------------//

    // Copy can be used to convert a matrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool Copy(const mdkMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Share, ForceShare  ------------------------------------------ //
 
    // if m_IsSizeFixed is true, and size does not match, then return false
    
    inline bool Share(mdkMatrix<ElementType>& InputMatrix);

    inline bool Share(mdkMatrix<ElementType>&& InputMatrix);

    // it is used by GlueMatrix
    // Share the object (InputMatrix) no matter what, even if InputMatrix is const
    inline void ForceShare(const mdkMatrix<ElementType>& InputMatrix);
  
    // about const share: i.e., only read (const functions), not write
    //
    // note1: if const share of one single object is needed
    //        just use const reference: 
    //        const mdkMatrix<ElementType>& = InputMatrix
    //        const mdkMatrix<ElementType>& = InputFunction()(return InputMatrix) 
    //
    // note2: if const share of many objects is needed
    //        there is no such thing as std::vector<const mdkMatrix<ElementType>&>
    //        but you can create std::vector<const mdkMatrix<ElementType>*>    
    //        However,std::vector<const mdkMatrix<ElementType>> may lead to trouble
    //        std::vector<const mdkMatrix<ElementType>> ConstMatrixArray(1);
    //        ConstMatrixArray[0].ForceShare(InputMatrix) is illegal (can not be compiled)
    //        ConstMatrixArray[0] = InputMatrix will call Copy() which is is illegal again
    //-----------------------------------------------------------------------------------------------------
    // conclusion: 
    // (1) An array of shared objects can be created from InputMatrix or InputFunction()
    //     std::vector<mdkMatrix<ElementType>> SharedMatrixArray(10);
    //     SharedMatrixArray[i].ForceShare(InputMatrix);
    //     SharedMatrixArray[i].ForceShare(InputFunction());
    //
    // (2) An array of const shared objects can not be created
    //     std::vector<const mdkMatrix<ElementType>> ConstMatrixArray(10); is useless
    //
    // (3) An array of const pointers to shared objects can be created from InputMatrix, NOT InputFunction()
    //     std::vector<const mdkMatrix<ElementType>*> SharedMatrixPointerArray(10);
    //     SharedMatrixPointerArray[i] = &InputMatrix;
    //------------------------------------------------------------------------------------------------------

    //-------------------- Take -----------------------------------------------------------//

    //Take the the ownership of the InputMatrix and ForceClear it

    inline bool Take(mdkMatrix<ElementType>& InputMatrix);

    inline bool Take(mdkMatrix<ElementType>&& InputMatrix);

    //Take the the ownership of the Matrix Created from ShadowMatrix or GlueMatrix

    inline bool Take(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline bool Take(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline bool Take(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //------------------------- Reset , Clear -------------------------------------------//
    
    // set the initial state, use it in constructor, do more things than Clear()
    inline void Reset();

    // clear memory no matter what, and set m_IsSizeFixed to be false
    inline void Clear();

	//---------------------- Set/get Matrix Size, Shape ----------------------------------------//

    inline bool Reshape(int64 InputRowNumber, int64 InputColNumber);

    inline bool Resize(int64 InputRowNumber, int64 InputColNumber, bool IsSizeFixed = false);

    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

	inline mdkMatrixSize GetSize() const;

    inline int64 GetElementNumber() const;

	inline int64 GetColNumber() const;

	inline int64 GetRowNumber() const;

    //------------------------ NaN Element -----------------------------//

    inline const ElementType& GetNaNElement() const;

    //------------------------ Element Type -----------------------------//

    inline mdkMatrixElementTypeEnum GetElementType() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

	//----------- Get/Set Matrix(LinearIndex) -----------------------------------//

    // operator[] or () : no bound check in release mode

    inline ElementType& operator[](int64 LinearIndex);

    inline const ElementType& operator[](int64 LinearIndex) const;

	inline ElementType& operator()(int64 LinearIndex);

    inline const ElementType& operator()(int64 LinearIndex) const;
    
    // at(): bound check

	inline ElementType& at(int64 LinearIndex);

	inline const ElementType& at(int64 LinearIndex) const;

	//----------- Get/Set Matrix(i,j)  ---------------------------------------------//

    // operator() : no bound check in release mode

	inline ElementType& operator()(int64 RowIndex, int64 ColIndex);

	inline const ElementType& operator()(int64 RowIndex, int64 ColIndex) const;

    // at(): bound check

	inline ElementType& at(int64 RowIndex, int64 ColIndex);

	inline const ElementType& at(int64 RowIndex, int64 ColIndex) const;

    //---------------------- Get/Set a set of elements by Matrix({}) or Matrix.at({}) ----------------------------//

    // operator(): no bound check in release mode
    //
    // note: operator[] is for single element access only, operator[{}] is not defined

    inline mdkShadowMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList);

    inline const mdkShadowMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList) const;

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList);

    inline const mdkShadowMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList) const;

    inline mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);   

    inline const mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    // at(): bound check -----------------

    inline mdkShadowMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList);

    inline const mdkShadowMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList) const;

    inline mdkShadowMatrix<ElementType> at(const std::vector<int64>& LinearIndexList);

    inline const mdkShadowMatrix<ElementType> at(const std::vector<int64>& LinearIndexList) const;

    inline mdkShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    //---------------------- Get/Set SubMatrix by Matrix({}, {}) or Matrix.at({}, {})  or Matrix.SubMatrix(a, b, c, d) -------//

    // operator(): no bound check in release mode

    inline mdkShadowMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                   std::initializer_list<int64>& ColIndexList);

    inline const mdkShadowMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                         std::initializer_list<int64>& ColIndexList) const;

    inline mdkShadowMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList, 
                                                   const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkShadowMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList, 
                                                         const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                   const std::initializer_list<int64>& ColIndexList);

    inline const mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                         const std::initializer_list<int64>& ColIndexList) const;

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                   const std::vector<int64>& ColIndexList);

    inline const mdkShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                         const std::vector<int64>& ColIndexList) const;

    inline mdkShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                   const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                         const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                   const std::vector<int64>& ColIndexList);

    inline const mdkShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                         const std::vector<int64>& ColIndexList) const;

    // at(): bound check -----------------

    inline mdkShadowMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                           std::initializer_list<int64>& ColIndexList);

    inline const mdkShadowMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                                 std::initializer_list<int64>& ColIndexList) const;

    inline mdkShadowMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                           const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkShadowMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                                 const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                           const std::initializer_list<int64>& ColIndexList);

    inline const mdkShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                 const std::initializer_list<int64>& ColIndexList) const;

    inline mdkShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                           const std::vector<int64>& ColIndexList);

    inline const mdkShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                 const std::vector<int64>& ColIndexList) const;

    inline mdkShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                           const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                 const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                           const std::vector<int64>& ColIndexList);

    inline const mdkShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                 const std::vector<int64>& ColIndexList) const;

    // return SubMatrix as Matrix -----------------------------------------------

    inline mdkMatrix GetSubMatrix(const std::vector<int64>& RowIndexList, 
                                  const std::vector<int64>& ColIndexList) const;

    inline mdkMatrix GetSubMatrix(const std::vector<int64>& RowIndexList, 
                                  const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkMatrix GetSubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, 
                                  const std::vector<int64>& ColIndexList) const;

    inline bool GetSubMatrix(mdkMatrix<ElementType> &OutputMatrix, 
                             const std::vector<int64>& RowIndexList, 
                             const std::vector<int64>& ColIndexList) const;

	//---------------------- Get/Set/Fill/Append/Delete/InsertCol Column ----------------------------------------//
	
    inline mdkShadowMatrix<ElementType> Col(int64 ColIndex);

    // do not use const in Col(const std::initializer_list<int64>& ColIndexList); 
    // it leads to ambiguous call (vs2013), 
    // e.g., Col({0})  it can initialize Col(int64) or Col(std::vector);
    //
    // so: use std::initializer_list<int64> without const 
    //
    inline mdkShadowMatrix<ElementType> Col(std::initializer_list<int64>& ColIndexList);

    inline mdkShadowMatrix<ElementType> Col(const std::vector<int64>& ColIndexList);

    inline mdkMatrix GetCol(int64 ColIndex) const;

    inline bool GetCol(int64 ColIndex, std::vector<ElementType>& ColData) const;

    inline bool GetCol(int64 ColIndex, ElementType* ColData) const;

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const mdkMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);
	
    inline bool FillCol(int64 ColIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendCol(const mdkMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool AppendCol(const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool AppendCol(const std::vector<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool AppendCol(const ElementType_Input* ColData, int64 Length);
	
    inline bool DeleteCol(int64 ColIndex);

    // no const before std::initializer_list
    inline bool DeleteCol(std::initializer_list<int64>& ColIndexList);

    inline bool DeleteCol(const std::vector<int64>& ColIndexList);

    inline bool DeleteCol(const int64* ColIndexListPtr, int64 Length);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const mdkMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);

	//---------------------- Get/Set/Fill/Append/Delete Row  ----------------------------------------//
	
    inline mdkShadowMatrix<ElementType> Row(int64 RowIndex);

    inline mdkShadowMatrix<ElementType> Row(std::initializer_list<int64>& RowIndexList);

    inline mdkShadowMatrix<ElementType> Row(const std::vector<int64>& RowIndexList);

    inline mdkMatrix GetRow(int64 RowIndex) const;

    inline bool GetRow(int64 RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(int64 RowIndex, ElementType* RowData) const;

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const mdkMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

    inline bool FillRow(int64 RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendRow(const mdkMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool AppendRow(const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool AppendRow(const std::vector<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool AppendRow(const ElementType_Input* RowData, int64 Length);

    inline bool DeleteRow(int64 RowIndex);

    // no const before std::initializer_list
    inline bool DeleteRow(std::initializer_list<int64>& RowIndexList);

    inline bool DeleteRow(const std::vector<int64>& RowIndexList);

    inline bool DeleteRow(const int64* RowIndexListPtr, int64 Length);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const mdkMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline mdkShadowMatrix<ElementType> Diangonal();

    inline mdkMatrix GetDiangonal() const;

    inline bool GetDiangonal(std::vector<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(ElementType* DiangonalData) const;

    template<typename ElementType_Input>
    inline bool SetDiangonal(const mdkMatrix<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::vector<ElementType_Input>& DiangonalData);

	template<typename ElementType_Input>
	inline bool SetDiangonal(const ElementType_Input* DiangonalData);

    inline bool FillDiangonal(const ElementType& Element);

	//---------------------- Matrix {+= -= *= /=} Matrix ----------------------------------------//

	inline void operator+=(const mdkMatrix<ElementType>& InputMatrix);

	inline void operator-=(const mdkMatrix<ElementType>& InputMatrix);

	inline void operator*=(const mdkMatrix<ElementType>& InputMatrix);

	inline void operator/=(const mdkMatrix<ElementType>& InputMatrix);


    inline void operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix);


    inline void operator+=(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator-=(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator*=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);


    inline void operator+=(mdkGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator-=(mdkGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator*=(mdkGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator/=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

	// error:
	//template<typename ElementType_Input>
	//inline void operator+(ElementType_Input Element);

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //-------------------- element operation {^} -----------------------------------------------------------//

    inline mdkMatrix operator^(const ElementType& Element);

    inline void operator^=(const ElementType& Element);

    //-------------------- special element operation : (.*) element multiply -----------------------------------------------------------//

    inline mdkMatrix ElementMultiply(const mdkMatrix<ElementType>& InputMatrix);

    inline mdkMatrix ElementMultiply(const ElementType& Element);

    inline mdkMatrix ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkMatrix ElementMultiply(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline mdkMatrix ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

	//-------------------- general element operation : output a new matrix ------------------------------------------//

    inline mdkMatrix ElementOperation(const char* OperationName) const;

    inline mdkMatrix ElementOperation(const std::string& OperationName) const;

    inline mdkMatrix ElementOperation(std::function<ElementType(const ElementType&)> Operation) const;

    inline mdkMatrix ElementOperation(const std::string& OperationName, const mdkMatrix<ElementType>& InputMatrix) const;

    inline mdkMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, 
                                      const mdkMatrix<ElementType>& InputMatrix) const;

    inline mdkMatrix ElementOperation(const char* OperationName, const ElementType& Element) const;

    inline mdkMatrix ElementOperation(const std::string& OperationName, const ElementType& Element) const;

    inline mdkMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element) const;

    //-------------------- general element operation in place : Object.ElementOperationInPlace modify the object ------------------//

    inline bool ElementOperationInPlace(const char* OperationName);

    inline bool ElementOperationInPlace(const std::string& OperationName);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&)> Operation);

    inline bool ElementOperationInPlace(const char* OperationName, const mdkMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const std::string& OperationName, const mdkMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                        const mdkMatrix<ElementType>& InputMatrix);

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

    inline int64 Rank() const;

	//----------------------------------- inverse -----------------------------------------//

    inline mdkMatrix Inv() const;

    inline mdkMatrix PseudoInv() const;

	//----------------------------------- SVD -----------------------------------------//

    inline mdkMatrixSVDResult<ElementType> SVD() const;


	//---------------------------- private functions ---------------------------------------//

private:

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


}//end namespace mdk

#include "mdkMatrix.hpp"

#endif
