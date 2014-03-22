#ifndef __mdkDenseMatrix_h
#define __mdkDenseMatrix_h

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>
#include <cmath>
#include <algorithm>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
#include "mdkMatrix_Common.h"
#include "mdkDenseShadowMatrix.h"
#include "mdkDenseGlueMatrixForLinearCombination.h"
#include "mdkDenseGlueMatrixForMultiplication.h"
#include "mdkDenseMatrixOperator.h"
#include "mdkLinearAlgebra_Function_DenseMatrix.h"

namespace mdk
{

// 2D Dense Matrix Class Template, each entry/element is a scalar
// column major
//
// Compare to Matlab:
// mdkDenseMatrix API very similar to Matlab matrix
//
// Compare to Armadillo  (a linear algebra library, and it uses column major matrix)
// mdkDenseMatrix API better than Armadillo
//
//

// ----------------------------- mdkDenseMatrixData struct -------------------------------------------------------------//

template<typename ElementType>
struct mdkDenseMatrixData
{
    int64 RowNumber;  // RowNumber = the Number of Rows 
    int64 ColNumber;  // ColNumber = the Number of Columns

    std::vector<ElementType> DataArray;
//-------------------------------------------------------------
    mdkDenseMatrixData() 
    {
        RowNumber = 0;
        ColNumber = 0;
    };

    ~mdkDenseMatrixData() {};

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
    mdkDenseMatrixData(const mdkDenseMatrixData&) = delete;

    mdkDenseMatrixData(mdkDenseMatrixData&&) = delete;

    void operator=(const mdkDenseMatrixData&) = delete;

    void operator=(mdkDenseMatrixData&&) = delete;
};

//-----------------------------------------------------------------------------------------------------------------------------//

#define MDK_DenseMatrix_ColExpansionStep  100

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkDenseMatrix : public mdkObject
{

private:
     
    std::shared_ptr<mdkDenseMatrixData<ElementType>> m_MatrixData;

    ElementType* m_ElementPointer; // pointer to the first element, keep tracking m_MatrixData->DataArray.data()

    ElementType m_NaNElement;

	bool m_IsSizeFixed;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline mdkDenseMatrix();

    inline mdkDenseMatrix(int64 RowNumber, int64 ColNumber);

    inline mdkDenseMatrix(const ElementType& Element);

    // deep-copy or shared-copy constructor
    inline mdkDenseMatrix(const mdkDenseMatrix<ElementType>& InputMatrix, mdkObjectConstructionTypeEnum Method = mdkObjectConstructionTypeEnum::Copy);

    // move constructor
    inline mdkDenseMatrix(mdkDenseMatrix<ElementType>&& InputMatrix);

    inline mdkDenseMatrix(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkDenseMatrix(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline mdkDenseMatrix(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline mdkDenseMatrix(const ElementType* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

	inline ~mdkDenseMatrix();

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const mdkDenseMatrix<ElementType>& InputMatrix);

    // move assignment operator
    inline void operator=(mdkDenseMatrix<ElementType>&& InputMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& list);

    inline void operator=(const std::initializer_list<std::initializer_list<ElementType>>& list);

    inline void operator=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //----------------------  Copy From Matrix or Element  ----------------------------------------//

    // Copy can be used to convert a matrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool Copy(const mdkDenseMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const mdkDenseMatrix<ElementType_Input>* InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

    // Matrix A, MatrixB, A.Share(B)
    //
    // If B is a null matrix (B.IsNull() == true), then A.Share(B) set A to an empty matrix, and A != B
    // If B change the value of an element, A will know (i.e., the data in A is the same as the data in B)
    // If B change its size, A will know.
    //
    // The above rule also applies to A if A changes it self.
    //
    // A.Share(B) really means A and B share the same data forever
    // 
    // There are 3 situations I may use Share
    //
    // (1) A.Share(B), then use A as an observer of B, and do not modify B by using A, e.g., A(0,0)=1;
    // 
    // (2) A.Share(B), then forget B and use A,  e.g., A(0,0)=1;
    //
    // (2) A.Share(B), then use both A and B to operate on the same data 
    //                      in this case, using a pointer to B is better if B is a named objects
    //
    // Share vs ForceShare
    //
    // A.Share(B): if m_IsSizeFixed of A is true, and size of A does not match size of B, then return false
    //
    // A.ForceShare(B): Share B no matter what, it is used by GlueMatrix

    inline bool Share(mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool Share(mdkDenseMatrix<ElementType>* InputMatrix);

    inline void ForceShare(const mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool ForceShare(const mdkDenseMatrix<ElementType>* InputMatrix);

    // about const share: e.g., A.ConstShare(B), then A can only read (const functions) from B, and cant not write to B
    //
    // note1: if const share of one single object is needed
    //        just use const reference: 
    //        const mdkDenseMatrix<ElementType>& A = B
    //        const mdkDenseMatrix<ElementType>& A = InputFunction()(return a Matrix) 
    //
    // note2: if const share of many objects is needed
    //        there is no such thing as std::vector<const mdkDenseMatrix<ElementType>&>
    //
    //        you can create std::vector<const mdkDenseMatrix<ElementType>*> MatrixPtrListA = {&B1, &B2, ...};
    //        and const prevent changing B, e.g., (*MatrixPtrListA[0])(0,0) = 10; can not be compiled
    //
    //        However,std::vector<const mdkDenseMatrix<ElementType>> is equal to std::vector<mdkDenseMatrix<ElementType>>
    //
    //        std::vector<const mdkDenseMatrix<ElementType>> MatrixListA;        
    //        MatrixListA.emplace_back(B1, mdkObjectCopyConstructionTypeEnum::Share);
    //        But:
    //            MatrixListA[0](1,1) = 100; CAN be compiled !!!  (B1 is changed by this code)
    //
    //-----------------------------------------------------------------------------------------------------
    // conclusion: 
    // (1) An array of Share objects can be created from InputMatrix
    //     std::vector<mdkDenseMatrix<ElementType>> SharedMatrixArray(10);
    //     SharedMatrixArray[i].Share(InputMatrix);
    //     SharedMatrixArray[i].ForceShare(InputMatrix);
    //
    // (2) An array of const Share objects can be created by using std::vector, but const is lost
    //
    // (3) An array of const pointers to objects can be created from InputMatrix
    //     std::vector<const mdkDenseMatrix<ElementType>*> SharedMatrixPointerArray(10);
    //     SharedMatrixPointerArray[i] = &InputMatrix;
    //------------------------------------------------------------------------------------------------------

    //-------------------- Take -----------------------------------------------------------//

    //Take the the ownership (MatrixData) of the InputMatrix and Clear InputMatrix

    inline void Take(mdkDenseMatrix<ElementType>&& InputMatrix);

    inline bool Take(mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool Take(mdkDenseMatrix<ElementType>* InputMatrix);

    //Take the Matrix Created from ShadowMatrix or GlueMatrix

    inline bool Take(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline bool Take(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline bool Take(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //------------------------- Clear -------------------------------------------//

    // equal to Resize(0, 0)
    inline void Clear();

	//---------------------- Set/get Matrix Size, Shape ----------------------------------------//

    inline bool Reshape(int64 InputRowNumber, int64 InputColNumber);

    inline bool Resize(int64 InputRowNumber, int64 InputColNumber);

    inline void FixSize();
    
    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

    inline bool IsNull() const;

	inline mdkMatrixSize GetSize() const;

    inline int64 GetElementNumber() const;

	inline int64 GetColNumber() const;

	inline int64 GetRowNumber() const;

    inline bool IsNonemptyVector() const;

    inline bool IsNonemptyRowVector() const;

    inline bool IsNonemptyColVector() const;

    inline bool IsNonemptySquare() const;

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

    inline mdkDenseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList);

    inline const mdkDenseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList);

    inline const mdkDenseShadowMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkDenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    // at(): bound check -----------------

    inline mdkDenseShadowMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList);

    inline const mdkDenseShadowMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> at(const std::vector<int64>& LinearIndexList);

    inline const mdkDenseShadowMatrix<ElementType> at(const std::vector<int64>& LinearIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkDenseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    //---------------------- Get/Set SubMatrix by Matrix({}, {}) or Matrix.at({}, {})  or Matrix.SubMatrix(a, b, c, d) -------//

    // operator(): no bound check in release mode

    inline mdkDenseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                        std::initializer_list<int64>& ColIndexList);

    inline const mdkDenseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                              std::initializer_list<int64>& ColIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList, 
                                                        const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkDenseShadowMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList, 
                                                              const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkDenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                        const std::initializer_list<int64>& ColIndexList);

    inline const mdkDenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                              const std::initializer_list<int64>& ColIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                        const std::vector<int64>& ColIndexList);

    inline const mdkDenseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                              const std::vector<int64>& ColIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                        const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkDenseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                              const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkDenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                        const std::vector<int64>& ColIndexList);

    inline const mdkDenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                              const std::vector<int64>& ColIndexList) const;

    // at(): bound check -----------------

    inline mdkDenseShadowMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                                std::initializer_list<int64>& ColIndexList);

    inline const mdkDenseShadowMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                                      std::initializer_list<int64>& ColIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                                const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkDenseShadowMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                                      const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkDenseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                const std::initializer_list<int64>& ColIndexList);

    inline const mdkDenseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                      const std::initializer_list<int64>& ColIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                const std::vector<int64>& ColIndexList);

    inline const mdkDenseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                      const std::vector<int64>& ColIndexList) const;

    inline mdkDenseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkDenseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                      const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkDenseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                const std::vector<int64>& ColIndexList);

    inline const mdkDenseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                      const std::vector<int64>& ColIndexList) const;

    // return SubMatrix as Matrix -----------------------------------------------

    inline mdkDenseMatrix GetSubMatrix(const std::vector<int64>& RowIndexList, 
                                       const std::vector<int64>& ColIndexList) const;

    inline mdkDenseMatrix GetSubMatrix(const std::vector<int64>& RowIndexList, 
                                       const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkDenseMatrix GetSubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, 
                                       const std::vector<int64>& ColIndexList) const;

    inline bool GetSubMatrix(mdkDenseMatrix<ElementType> &OutputMatrix, 
                             const std::vector<int64>& RowIndexList, 
                             const std::vector<int64>& ColIndexList) const;

	//---------------------- Get/Set/Fill/Append/Delete/InsertCol Column ----------------------------------------//
	
    inline mdkDenseShadowMatrix<ElementType> Col(int64 ColIndex);

    // do not use const in Col(const std::initializer_list<int64>& ColIndexList); 
    // it leads to ambiguous call (vs2013), 
    // e.g., Col({0})  it can initialize Col(int64) or Col(std::vector);
    //
    // so: use std::initializer_list<int64> without const 
    //
    inline mdkDenseShadowMatrix<ElementType> Col(std::initializer_list<int64>& ColIndexList);

    inline mdkDenseShadowMatrix<ElementType> Col(const std::vector<int64>& ColIndexList);

    inline mdkDenseMatrix GetCol(int64 ColIndex) const;

    inline bool GetCol(int64 ColIndex, std::vector<ElementType>& ColData) const;

    inline bool GetCol(int64 ColIndex, ElementType* ColData) const;

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const mdkDenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);
	
    inline bool FillCol(int64 ColIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendCol(const mdkDenseMatrix<ElementType_Input>& ColData);

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
    inline bool InsertCol(int64 ColIndex, const mdkDenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);

	//---------------------- Get/Set/Fill/Append/Delete Row  ----------------------------------------//
	
    inline mdkDenseShadowMatrix<ElementType> Row(int64 RowIndex);

    inline mdkDenseShadowMatrix<ElementType> Row(std::initializer_list<int64>& RowIndexList);

    inline mdkDenseShadowMatrix<ElementType> Row(const std::vector<int64>& RowIndexList);

    inline mdkDenseMatrix GetRow(int64 RowIndex) const;

    inline bool GetRow(int64 RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(int64 RowIndex, ElementType* RowData) const;

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const mdkDenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

    inline bool FillRow(int64 RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendRow(const mdkDenseMatrix<ElementType_Input>& RowData);

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
    inline bool InsertRow(int64 RowIndex, const mdkDenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline mdkDenseShadowMatrix<ElementType> Diangonal();

    inline mdkDenseMatrix GetDiangonal() const;

    inline bool GetDiangonal(std::vector<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(ElementType* DiangonalData) const;

    template<typename ElementType_Input>
    inline bool SetDiangonal(const mdkDenseMatrix<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::vector<ElementType_Input>& DiangonalData);

	template<typename ElementType_Input>
	inline bool SetDiangonal(const ElementType_Input* DiangonalData);

    inline bool FillDiangonal(const ElementType& Element);

	//---------------------- Matrix {+= -= *= /=} Matrix ----------------------------------------//

	inline void operator+=(const mdkDenseMatrix<ElementType>& InputMatrix);

	inline void operator-=(const mdkDenseMatrix<ElementType>& InputMatrix);

	inline void operator*=(const mdkDenseMatrix<ElementType>& InputMatrix);

	inline void operator/=(const mdkDenseMatrix<ElementType>& InputMatrix);


    inline void operator+=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix);


    inline void operator+=(mdkDenseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator-=(mdkDenseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator*=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);


    inline void operator+=(mdkDenseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator-=(mdkDenseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator*=(mdkDenseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator/=(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

	// error:
	//template<typename ElementType_Input>
	//inline void operator+(ElementType_Input Element);

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //-------------------- element operation {^} -----------------------------------------------------------//

    inline mdkDenseMatrix operator^(const ElementType& Element);

    inline void operator^=(const ElementType& Element);

    //-------------------- special element operation : (.*) element multiply -----------------------------------------------------------//

    inline mdkDenseMatrix ElementMultiply(const mdkDenseMatrix<ElementType>& InputMatrix) const;

    inline mdkDenseMatrix ElementMultiply(const ElementType& Element) const;

    inline mdkDenseMatrix ElementMultiply(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix) const;

    inline mdkDenseMatrix ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const;

    inline mdkDenseMatrix ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const;

	//-------------------- general element operation : output a new matrix ------------------------------------------//

    inline mdkDenseMatrix ElementOperation(const char* OperationName) const;

    inline mdkDenseMatrix ElementOperation(const std::string& OperationName) const;

    inline mdkDenseMatrix ElementOperation(std::function<ElementType(const ElementType&)> Operation) const;

    inline mdkDenseMatrix ElementOperation(const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix) const;

    inline mdkDenseMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, 
                                           const mdkDenseMatrix<ElementType>& InputMatrix) const;

    inline mdkDenseMatrix ElementOperation(const char* OperationName, const ElementType& Element) const;

    inline mdkDenseMatrix ElementOperation(const std::string& OperationName, const ElementType& Element) const;

    inline mdkDenseMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element) const;

    //-------------------- general element operation in place : Object.ElementOperationInPlace modify the object itself ---------------//

    inline bool ElementOperationInPlace(const char* OperationName);

    inline bool ElementOperationInPlace(const std::string& OperationName);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&)> Operation);

    inline bool ElementOperationInPlace(const char* OperationName, const mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                        const mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const char* OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(const std::string& OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element);

	//-------------------- calculate sum mean min max ------------------------------------------//

    inline ElementType Mean() const;

    inline mdkDenseMatrix MeanToRow() const;

    inline mdkDenseMatrix MeanToCol() const;

    inline ElementType Sum() const;

    inline mdkDenseMatrix SumToRow() const;

    inline mdkDenseMatrix SumToCol() const;

    inline ElementType Max() const;

    inline mdkDenseMatrix MaxToRow() const;

    inline mdkDenseMatrix MaxToCol() const;

    inline ElementType Min() const;

    inline mdkDenseMatrix MinToRow() const;

    inline mdkDenseMatrix MinToCol() const;

	//----------------------------------- transpose -----------------------------------------//

    inline mdkDenseMatrix Transpose() const;

	//----------------------------------- Rank -----------------------------------------//

    inline int64 Rank() const;

	//----------------------------------- inverse -----------------------------------------//

    inline mdkDenseMatrix Inv() const;

    inline mdkDenseMatrix PseudoInv() const;

	//----------------------------------- SVD -----------------------------------------//

    inline mdkDenseMatrixSVDResult<ElementType> SVD() const;


	//---------------------------- private functions ---------------------------------------//

private:

};

//================================================== For Reference ===============================================================//
/*
// ------------------------------------ Matrix {+ - * /}  Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator+(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator-(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator*(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator/(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

// ------------------------------------ Matrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator+(mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator-(mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator*(mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator/(mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB);

// ------------------------------------- Element {+ - * /} Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator+(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator-(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator*(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator/(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

// ----------------------- Matrix = Element is defined by a constructor, and "=" operator --------------------------//

// ----------------------- Element = Matrix (if 1x1) is not allowed in c++ --------------------------------------------//
// not allowed in c++
//template<typename ElementType>
//inline void operator=(ElementType& Element, const mdkDenseMatrix<ElementType>& Matrix);
*/
//=====================================================================================================================//


}//end namespace mdk

#include "mdkDenseMatrix.hpp"

#endif
