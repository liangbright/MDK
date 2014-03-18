#ifndef __mdkSparseMatrix_h
#define __mdkSparseMatrix_h

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>

#include "mdkLinearAlgebraConfig.h"
#include "mdkMatrix.h"


namespace mdk
{

// 2D SparseMatrix Class Template, each entry/element is a scalar
// column major
//
// Compare to Matlab:
// mdkSparseMatrix API very similar to Matlab SparseMatrix
//
//
//

//forward-declare ----------------//
template<typename ElementType>
class mdkSparseMatrix;

template<typename ElementType>
class mdkSparseShadowMatrix;

template<typename ElementType>
class mdkSparseGlueMatrixForLinearCombination;

template<typename ElementType>
class mdkSparseGlueMatrixForMultiplication;

// end of  forward-declare  ------//


// ----------------------------- mdkSparseMatrixData struct -------------------------------------------------------------//

template<typename ElementType>
struct mdkSparseMatrixData
{
    int64 RowNumber;  // RowNumber = the Number of Rows 
    int64 ColNumber;  // ColNumber = the Number of Columns

    std::vector<ElementType> DataArray;
//-------------------------------------------------------------
    mdkSparseMatrixData() 
    {
        RowNumber = 0;
        ColNumber = 0;
    };

    ~mdkSparseMatrixData() {};

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
    mdkSparseMatrixData(const mdkSparseMatrixData&) = delete;

    mdkSparseMatrixData(mdkSparseMatrixData&&) = delete;

    void operator=(const mdkSparseMatrixData&) = delete;

    void operator=(mdkSparseMatrixData&&) = delete;
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkSparseMatrix : public mdkObject
{

private:
     
    std::shared_ptr<mdkSparseMatrixData<ElementType>> m_SparseMatrixData;

    ElementType m_NaNElement;

	bool m_IsSizeFixed;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline mdkSparseMatrix();

    inline mdkSparseMatrix(int64 RowNumber, int64 ColNumber);

    // copy constructor (DeepCopy)
    inline mdkSparseMatrix(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

    inline mdkSparseMatrix(const ElementType& Element);

    // copy constructor (DeepCopy or ShallowCopy)
    inline mdkSparseMatrix(mdkSparseMatrix<ElementType>& InputSparseMatrix, mdkObjectConstructionTypeEnum Method = mdkObjectConstructionTypeEnum::DeepCopy);

    // move constructor
    inline mdkSparseMatrix(mdkSparseMatrix<ElementType>&& InputSparseMatrix);

    inline mdkSparseMatrix(const mdkSparseShadowMatrix<ElementType>& SparseShadowMatrix);

    inline mdkSparseMatrix(const mdkSparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix);

    inline mdkSparseMatrix(const mdkSparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix);

	inline ~mdkSparseMatrix();

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

    // move assignment operator
    inline void operator=(mdkSparseMatrix<ElementType>&& InputSparseMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& list);

    inline void operator=(const std::initializer_list<std::initializer_list<ElementType>>& list);

    inline void operator=(const mdkShadowSparseMatrix<ElementType>& ShadowSparseMatrix);

    inline void operator=(const mdkGlueSparseMatrixForLinearCombination<ElementType>& GlueSparseMatrix);

    inline void operator=(const mdkGlueSparseMatrixForMultiplication<ElementType>& GlueSparseMatrix);

    //----------------------  Copy From SparseMatrix or Element  ----------------------------------------//

    // Copy can be used to convert a SparseMatrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool Copy(const mdkSparseMatrix<ElementType_Input>& InputSparseMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Share, ForceShare  ------------------------------------------ //
 
    // if m_IsSizeFixed is true, and size does not match, then return false
    //
    inline bool Share(mdkSparseMatrix<ElementType>& InputSparseMatrix);

    // it is used by GlueSparseMatrix
    // Share the object (InputSparseMatrix) no matter what, even if InputSparseMatrix is const
    inline void ForceShare(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

    // about const share: i.e., only read (const functions), not write
    //
    // note1: if const share of one single object is needed
    //        just use const reference: 
    //        const mdkSparseMatrix<ElementType>& = InputSparseMatrix
    //        const mdkSparseMatrix<ElementType>& = InputFunction()(return InputSparseMatrix) 
    //
    // note2: if const share of many objects is needed
    //        there is no such thing as std::vector<const mdkSparseMatrix<ElementType>&>
    //        just create std::vector<const mdkSparseMatrix<ElementType>*> SparseMatrixPtrList = {&A}; from SparseMatrix A
    //        then const prevent using (*SparseMatrixPtrList[0])(0,0) = 10;
    //
    //        However,std::vector<const mdkSparseMatrix<ElementType>> is equal to std::vector<mdkSparseMatrix<ElementType>>
    //        std::vector<const mdkSparseMatrix<ElementType>> SparseMatrixList can be constructed from SparseMatrix A by share
    //        
    //        SparseMatrixList.emplace_back(A, mdkObjectConstructionTypeEnum::SHARE);
    //        But:
    //            SparseMatrixList[0](1,1) = 100; CAN be compiled !!!  (A is changed by this code)
    //
    //-----------------------------------------------------------------------------------------------------
    // conclusion: 
    // (1) An array of shared objects can be created from InputSparseMatrix
    //     std::vector<mdkSparseMatrix<ElementType>> SharedSparseMatrixArray(10);
    //     SharedSparseMatrixArray[i].Share(InputSparseMatrix);
    //     SharedSparseMatrixArray[i].ForceShare(InputSparseMatrix);
    //
    // (2) An array of const shared objects can be created by using std::vector, but const is lost
    //
    // (3) An array of const pointers to shared objects can be created from InputSparseMatrix, NOT InputFunction()
    //     std::vector<const mdkSparseMatrix<ElementType>*> SharedSparseMatrixPointerArray(10);
    //     SharedSparseMatrixPointerArray[i] = &InputSparseMatrix;
    //------------------------------------------------------------------------------------------------------

    //-------------------- Take -----------------------------------------------------------//

    //Take the the ownership of the InputSparseMatrix and ForceClear it

    inline bool Take(mdkSparseMatrix<ElementType>& InputSparseMatrix);

    inline bool Take(mdkSparseMatrix<ElementType>&& InputSparseMatrix);

    //Take the the ownership of the SparseMatrix Created from ShadowSparseMatrix or GlueSparseMatrix

    inline bool Take(const mdkShadowSparseMatrix<ElementType>& ShadowSparseMatrix);

    inline bool Take(const mdkGlueSparseMatrixForLinearCombination<ElementType>& GlueSparseMatrix);

    inline bool Take(const mdkGlueSparseMatrixForMultiplication<ElementType>& GlueSparseMatrix);

    //------------------------- Reset , Clear -------------------------------------------//
    
    // set the initial state, use it in constructor, do more things than Clear()
    inline void Reset();

    // clear memory no matter what, and set m_IsSizeFixed to be false
    inline void Clear();

	//---------------------- Set/get SparseMatrix Size, Shape ----------------------------------------//

    inline bool Reshape(int64 InputRowNumber, int64 InputColNumber);

    inline bool Resize(int64 InputRowNumber, int64 InputColNumber);

    inline void FixSize();
    
    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

	inline mdkSparseMatrixSize GetSize() const;

    inline int64 GetElementNumber() const;

	inline int64 GetColNumber() const;

	inline int64 GetRowNumber() const;

    //------------------------ NaN Element -----------------------------//

    inline const ElementType& GetNaNElement() const;

    //------------------------ Element Type -----------------------------//

    inline mdkSparseMatrixElementTypeEnum GetElementType() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

	//----------- Get/Set SparseMatrix(LinearIndex) -----------------------------------//

    // operator[] or () : no bound check in release mode

    inline ElementType& operator[](int64 LinearIndex);

    inline const ElementType& operator[](int64 LinearIndex) const;

	inline ElementType& operator()(int64 LinearIndex);

    inline const ElementType& operator()(int64 LinearIndex) const;
    
    // at(): bound check

	inline ElementType& at(int64 LinearIndex);

	inline const ElementType& at(int64 LinearIndex) const;

	//----------- Get/Set SparseMatrix(i,j)  ---------------------------------------------//

    // operator() : no bound check in release mode

	inline ElementType& operator()(int64 RowIndex, int64 ColIndex);

	inline const ElementType& operator()(int64 RowIndex, int64 ColIndex) const;

    // at(): bound check

	inline ElementType& at(int64 RowIndex, int64 ColIndex);

	inline const ElementType& at(int64 RowIndex, int64 ColIndex) const;

    //---------------------- Get/Set a set of elements by SparseMatrix({}) or SparseMatrix.at({}) ----------------------------//

    // operator(): no bound check in release mode
    //
    // note: operator[] is for single element access only, operator[{}] is not defined

    inline mdkShadowSparseMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList);

    inline const mdkShadowSparseMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList);

    inline const mdkShadowSparseMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> operator()(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol);   

    inline const mdkShadowSparseMatrix<ElementType> operator()(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol) const;

    // at(): bound check -----------------

    inline mdkShadowSparseMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList);

    inline const mdkShadowSparseMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> at(const std::vector<int64>& LinearIndexList);

    inline const mdkShadowSparseMatrix<ElementType> at(const std::vector<int64>& LinearIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> at(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol);

    inline const mdkShadowSparseMatrix<ElementType> at(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol) const;

    //---------------------- Get/Set SubSparseMatrix by SparseMatrix({}, {}) or SparseMatrix.at({}, {})  or SparseMatrix.SubSparseMatrix(a, b, c, d) -------//

    // operator(): no bound check in release mode

    inline mdkShadowSparseMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                   std::initializer_list<int64>& ColIndexList);

    inline const mdkShadowSparseMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                         std::initializer_list<int64>& ColIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList, 
                                                   const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol);

    inline const mdkShadowSparseMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList, 
                                                         const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol) const;

    inline mdkShadowSparseMatrix<ElementType> operator()(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol,
                                                   const std::initializer_list<int64>& ColIndexList);

    inline const mdkShadowSparseMatrix<ElementType> operator()(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol,
                                                         const std::initializer_list<int64>& ColIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                   const std::vector<int64>& ColIndexList);

    inline const mdkShadowSparseMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                         const std::vector<int64>& ColIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                   const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol);

    inline const mdkShadowSparseMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                         const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol) const;

    inline mdkShadowSparseMatrix<ElementType> operator()(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol,
                                                   const std::vector<int64>& ColIndexList);

    inline const mdkShadowSparseMatrix<ElementType> operator()(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol,
                                                         const std::vector<int64>& ColIndexList) const;

    // at(): bound check -----------------

    inline mdkShadowSparseMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                           std::initializer_list<int64>& ColIndexList);

    inline const mdkShadowSparseMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                                 std::initializer_list<int64>& ColIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                           const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol);

    inline const mdkShadowSparseMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                                 const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol) const;

    inline mdkShadowSparseMatrix<ElementType> at(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol,
                                           const std::initializer_list<int64>& ColIndexList);

    inline const mdkShadowSparseMatrix<ElementType> at(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol,
                                                 const std::initializer_list<int64>& ColIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                           const std::vector<int64>& ColIndexList);

    inline const mdkShadowSparseMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                 const std::vector<int64>& ColIndexList) const;

    inline mdkShadowSparseMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                           const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol);

    inline const mdkShadowSparseMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                 const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol) const;

    inline mdkShadowSparseMatrix<ElementType> at(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol,
                                           const std::vector<int64>& ColIndexList);

    inline const mdkShadowSparseMatrix<ElementType> at(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol,
                                                 const std::vector<int64>& ColIndexList) const;

    // return SubSparseMatrix as SparseMatrix -----------------------------------------------

    inline mdkSparseMatrix GetSubSparseMatrix(const std::vector<int64>& RowIndexList, 
                                  const std::vector<int64>& ColIndexList) const;

    inline mdkSparseMatrix GetSubSparseMatrix(const std::vector<int64>& RowIndexList, 
                                  const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol) const;

    inline mdkSparseMatrix GetSubSparseMatrix(const ALL_Symbol_For_mdkSparseMatrix_Operator& ALL_Symbol, 
                                  const std::vector<int64>& ColIndexList) const;

    inline bool GetSubSparseMatrix(mdkSparseMatrix<ElementType> &OutputSparseMatrix, 
                             const std::vector<int64>& RowIndexList, 
                             const std::vector<int64>& ColIndexList) const;

	//---------------------- Get/Set/Fill/Append/Delete/InsertCol Column ----------------------------------------//
	
    inline mdkShadowSparseMatrix<ElementType> Col(int64 ColIndex);

    // do not use const in Col(const std::initializer_list<int64>& ColIndexList); 
    // it leads to ambiguous call (vs2013), 
    // e.g., Col({0})  it can initialize Col(int64) or Col(std::vector);
    //
    // so: use std::initializer_list<int64> without const 
    //
    inline mdkShadowSparseMatrix<ElementType> Col(std::initializer_list<int64>& ColIndexList);

    inline mdkShadowSparseMatrix<ElementType> Col(const std::vector<int64>& ColIndexList);

    inline mdkSparseMatrix GetCol(int64 ColIndex) const;

    inline bool GetCol(int64 ColIndex, std::vector<ElementType>& ColData) const;

    inline bool GetCol(int64 ColIndex, ElementType* ColData) const;

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const mdkSparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);
	
    inline bool FillCol(int64 ColIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendCol(const mdkSparseMatrix<ElementType_Input>& ColData);

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
    inline bool InsertCol(int64 ColIndex, const mdkSparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);

	//---------------------- Get/Set/Fill/Append/Delete Row  ----------------------------------------//
	
    inline mdkShadowSparseMatrix<ElementType> Row(int64 RowIndex);

    inline mdkShadowSparseMatrix<ElementType> Row(std::initializer_list<int64>& RowIndexList);

    inline mdkShadowSparseMatrix<ElementType> Row(const std::vector<int64>& RowIndexList);

    inline mdkSparseMatrix GetRow(int64 RowIndex) const;

    inline bool GetRow(int64 RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(int64 RowIndex, ElementType* RowData) const;

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const mdkSparseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

    inline bool FillRow(int64 RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendRow(const mdkSparseMatrix<ElementType_Input>& RowData);

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
    inline bool InsertRow(int64 RowIndex, const mdkSparseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline mdkShadowSparseMatrix<ElementType> Diangonal();

    inline mdkSparseMatrix GetDiangonal() const;

    inline bool GetDiangonal(std::vector<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(ElementType* DiangonalData) const;

    template<typename ElementType_Input>
    inline bool SetDiangonal(const mdkSparseMatrix<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::vector<ElementType_Input>& DiangonalData);

	template<typename ElementType_Input>
	inline bool SetDiangonal(const ElementType_Input* DiangonalData);

    inline bool FillDiangonal(const ElementType& Element);

	//---------------------- SparseMatrix {+= -= *= /=} SparseMatrix ----------------------------------------//

	inline void operator+=(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

	inline void operator-=(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

	inline void operator*=(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

	inline void operator/=(const mdkSparseMatrix<ElementType>& InputSparseMatrix);


    inline void operator+=(const mdkShadowSparseMatrix<ElementType>& ShadowSparseMatrix);

    inline void operator-=(const mdkShadowSparseMatrix<ElementType>& ShadowSparseMatrix);

    inline void operator*=(const mdkShadowSparseMatrix<ElementType>& ShadowSparseMatrix);

    inline void operator/=(const mdkShadowSparseMatrix<ElementType>& ShadowSparseMatrix);


    inline void operator+=(mdkGlueSparseMatrixForLinearCombination<ElementType> GlueSparseMatrix);

    inline void operator-=(mdkGlueSparseMatrixForLinearCombination<ElementType> GlueSparseMatrix);

    inline void operator*=(const mdkGlueSparseMatrixForLinearCombination<ElementType>& GlueSparseMatrix);

    inline void operator/=(const mdkGlueSparseMatrixForLinearCombination<ElementType>& GlueSparseMatrix);


    inline void operator+=(mdkGlueSparseMatrixForMultiplication<ElementType> GlueSparseMatrix);

    inline void operator-=(mdkGlueSparseMatrixForMultiplication<ElementType> GlueSparseMatrix);

    inline void operator*=(mdkGlueSparseMatrixForMultiplication<ElementType> GlueSparseMatrix);

    inline void operator/=(const mdkGlueSparseMatrixForMultiplication<ElementType>& GlueSparseMatrix);

    //---------------------- SparseMatrix {+= -= *= /=} Element ----------------------------------------//

	// error:
	//template<typename ElementType_Input>
	//inline void operator+(ElementType_Input Element);

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //-------------------- element operation {^} -----------------------------------------------------------//

    inline mdkSparseMatrix operator^(const ElementType& Element);

    inline void operator^=(const ElementType& Element);

    //-------------------- special element operation : (.*) element multiply -----------------------------------------------------------//

    inline mdkSparseMatrix ElementMultiply(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

    inline mdkSparseMatrix ElementMultiply(const ElementType& Element);

    inline mdkSparseMatrix ElementMultiply(const mdkShadowSparseMatrix<ElementType>& ShadowSparseMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkGlueSparseMatrixForLinearCombination<ElementType>& GlueSparseMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkGlueSparseMatrixForMultiplication<ElementType>& GlueSparseMatrix);

	//-------------------- general element operation : output a new SparseMatrix ------------------------------------------//

    inline mdkSparseMatrix ElementOperation(const char* OperationName) const;

    inline mdkSparseMatrix ElementOperation(const std::string& OperationName) const;

    inline mdkSparseMatrix ElementOperation(std::function<ElementType(const ElementType&)> Operation) const;

    inline mdkSparseMatrix ElementOperation(const std::string& OperationName, const mdkSparseMatrix<ElementType>& InputSparseMatrix) const;

    inline mdkSparseMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, 
                                      const mdkSparseMatrix<ElementType>& InputSparseMatrix) const;

    inline mdkSparseMatrix ElementOperation(const char* OperationName, const ElementType& Element) const;

    inline mdkSparseMatrix ElementOperation(const std::string& OperationName, const ElementType& Element) const;

    inline mdkSparseMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element) const;

    //-------------------- general element operation in place : Object.ElementOperationInPlace modify the object ------------------//

    inline bool ElementOperationInPlace(const char* OperationName);

    inline bool ElementOperationInPlace(const std::string& OperationName);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&)> Operation);

    inline bool ElementOperationInPlace(const char* OperationName, const mdkSparseMatrix<ElementType>& InputSparseMatrix);

    inline bool ElementOperationInPlace(const std::string& OperationName, const mdkSparseMatrix<ElementType>& InputSparseMatrix);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                        const mdkSparseMatrix<ElementType>& InputSparseMatrix);

    inline bool ElementOperationInPlace(const char* OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(const std::string& OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element);

	//-------------------- calculate sum mean min max ------------------------------------------//

    inline ElementType Mean() const;

    inline mdkSparseMatrix MeanToRow() const;

    inline mdkSparseMatrix MeanToCol() const;

    inline ElementType Sum() const;

    inline mdkSparseMatrix SumToRow() const;

    inline mdkSparseMatrix SumToCol() const;

    inline ElementType Max() const;

    inline mdkSparseMatrix MaxToRow() const;

    inline mdkSparseMatrix MaxToCol() const;

    inline ElementType Min() const;

    inline mdkSparseMatrix MinToRow() const;

    inline mdkSparseMatrix MinToCol() const;

	//----------------------------------- transpose -----------------------------------------//

    inline mdkSparseMatrix Transpose() const;

	//----------------------------------- Rank -----------------------------------------//

    inline int64 Rank() const;

	//----------------------------------- inverse -----------------------------------------//

    inline mdkSparseMatrix Inv() const;

    inline mdkSparseMatrix PseudoInv() const;

	//----------------------------------- SVD -----------------------------------------//

    inline mdkSparseMatrixSVDResult<ElementType> SVD() const;


	//---------------------------- private functions ---------------------------------------//

private:

};

//============================================= For Reference ===================================================//
/*
// ------------------------------------ SparseMatrix {+ - * /}  SparseMatrix ------------------------------------------------//

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator+(const mdkSparseMatrix<ElementType>& SparseMatrixA, const mdkSparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator-(const mdkSparseMatrix<ElementType>& SparseMatrixA, const mdkSparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator*(const mdkSparseMatrix<ElementType>& SparseMatrixA, const mdkSparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator/(const mdkSparseMatrix<ElementType>& SparseMatrixA, const mdkSparseMatrix<ElementType>& SparseMatrixB);

// ------------------------------------ SparseMatrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator+(mdkSparseMatrix<ElementType>& SparseMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator-(mdkSparseMatrix<ElementType>& SparseMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator*(mdkSparseMatrix<ElementType>& SparseMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator/(mdkSparseMatrix<ElementType>& SparseMatrixA, const ElementType& ElementB);

// ------------------------------------- Element {+ - * /} SparseMatrix ------------------------------------------------//

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator+(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator-(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator*(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> operator/(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& SparseMatrixB);

// ----------------------- Element = SparseMatrix (if 1x1) is not allowed in c++ --------------------------------------------//
// not allowed in c++
//template<typename ElementType>
//inline void operator=(ElementType& Element, const mdkSparseMatrix<ElementType>& SparseMatrix);
*/
//=====================================================================================================================//


}//end namespace mdk

#include "mdkSparseMatrix.hpp"

#endif
