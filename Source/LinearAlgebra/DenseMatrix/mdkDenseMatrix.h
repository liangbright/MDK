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
#include "mdkLinearAlgebra_Function_Common.h"

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
struct DenseMatrixData
{
    int_max RowNumber;  // RowNumber = the Number of Rows 
    int_max ColNumber;  // ColNumber = the Number of Columns

    ElementType* ElementPointer;

    std::vector<ElementType> DataArray;

    bool IsSizeFixed;

//-------------------------------------------------------------
    DenseMatrixData() 
    {
        RowNumber = 0;
        ColNumber = 0;
        ElementPointer = nullptr;
        IsSizeFixed = false;
    };

    ~DenseMatrixData() {};

    void CopyDataToInternalMemoryIfNecessary()
    {
        if (ElementPointer != DataArray.data())
        {
            auto ElementNumber = RowNumber*ColNumber;

            DataArray.resize(ElementNumber);

            for (int_max i = 0; i < ElementNumber; ++i)
            {
                DataArray[i] = ElementPointer[i];
            }

            ElementPointer = DataArray.data();
        }
    }

    ElementType& operator[](int_max LinearIndex)
    {
        return ElementPointer[LinearIndex];
    }

    const ElementType& operator[](int_max LinearIndex) const
    {
        return ElementPointer[LinearIndex];
    }

    ElementType& operator()(int_max LinearIndex)
    {
        return ElementPointer[LinearIndex];
    }

    const ElementType& operator()(int_max LinearIndex) const
    {
        return ElementPointer[LinearIndex];
    }

    ElementType& operator()(int_max RowIndex, int_max ColIndex)
    {
        return ElementPointer[ColIndex*RowNumber + RowIndex];
    }

    const ElementType& operator()(int_max RowIndex, int_max ColIndex) const
    {
        return ElementPointer[ColIndex*RowNumber + RowIndex];
    }

private:
//deleted: -------------------------------------------------
    DenseMatrixData(const DenseMatrixData&) = delete;

    DenseMatrixData(DenseMatrixData&&) = delete;

    void operator=(const DenseMatrixData&) = delete;

    void operator=(DenseMatrixData&&) = delete;
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class DenseMatrix : public Object
{

private:
     
    std::shared_ptr<DenseMatrixData<ElementType>> m_MatrixData;

    ElementType* m_ElementPointer; // pointer to the first element, keep tracking m_MatrixData->ElementPointer

    ElementType m_NaNElement;	

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline DenseMatrix();

    inline DenseMatrix(const Pure_Empty_Matrix_Symbol&);

    inline DenseMatrix(int_max RowNumber, int_max ColNumber);

    inline DenseMatrix(const std::initializer_list<ElementType>& InputList);

    inline DenseMatrix(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList);

    inline DenseMatrix(const ElementType& Element);

    // deep-copy or shared-copy constructor
    inline DenseMatrix(const DenseMatrix<ElementType>& InputMatrix, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy);

    // move constructor
    inline DenseMatrix(DenseMatrix<ElementType>&& InputMatrix) noexcept;

    inline DenseMatrix(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline DenseMatrix(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline DenseMatrix(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline DenseMatrix(ElementType* InputElementPointer, int_max InputRowNumber, int_max InputColNumber, bool IsSizeFixed = true); 
    // use existing data pointed by InputElementPointer
    // The data must be in heap
    // if the data is in stack of a function, then return a matrix will cause crash !

	inline ~DenseMatrix();

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const DenseMatrix<ElementType>& InputMatrix);

    // move assignment operator
    inline void operator=(DenseMatrix<ElementType>&& InputMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    inline void operator=(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList);

    inline void operator=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //----------------------  Copy From Matrix or Element  ----------------------------------------//

    // Copy can be used to convert a matrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool Copy(const DenseMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const DenseMatrix<ElementType_Input>* InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int_max InputRowNumber, int_max InputColNumber);

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
    // A.Share(B) really means A and B share the same data
    // until A.Share(C), then A == C, and the share-relation between A and B ends, B keeps its data.
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

    // m_MatrixData = InputMatrix.m_MatrixData;

    inline bool Share(DenseMatrix<ElementType>& InputMatrix);

    inline bool Share(DenseMatrix<ElementType>* InputMatrix);

    inline void ForceShare(const DenseMatrix<ElementType>& InputMatrix);

    inline bool ForceShare(const DenseMatrix<ElementType>* InputMatrix);

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

    //Take the the data of the InputMatrix and Clear InputMatrix

    // m_MatrixData->DataArray = std::move(InputMatrix.m_MatrixData->DataArray);

    inline void Take(DenseMatrix<ElementType>&& InputMatrix);

    inline bool Take(DenseMatrix<ElementType>& InputMatrix);

    inline bool Take(DenseMatrix<ElementType>* InputMatrix);

    //inline bool Take(std::vector<ElementType>& InputRowVector);

    //Take the Matrix Created from ShadowMatrix or GlueMatrix

    inline bool Take(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline bool Take(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline bool Take(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //------------------------- Swap shared_ptr m_MatrixData -------------------------------------------//

    // m_MatrixData.swap(InputMatrix.m_MatrixData)

    inline void SwapSmartPointer(DenseMatrix<ElementType>& InputMatrix);

    //------------------------- Clear -------------------------------------------//

    // clear memory, not equal to Resize(0, 0) which may not release memory
    inline void Clear();

	//---------------------- Set/get Matrix Size, Shape ----------------------------------------//

    inline bool Reshape(int_max InputRowNumber, int_max InputColNumber);

    inline bool Resize(int_max InputRowNumber, int_max InputColNumber); // try to keep the old data

    inline bool FastResize(int_max InputRowNumber, int_max InputColNumber); // do not care about old data

    inline bool Resize(int_max InputElementNumber); // try to keep the old data, can not use this to resize a m x n matrix (m>1 or n>1)

    inline bool FastResize(int_max InputElementNumber); // do not care about old data, can not use this to resize a m x n matrix (m>1 or n>1)

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current matrix size does not change

    inline void FixSize();
    
    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

    inline bool IsPureEmpty() const;

    inline bool IsShared() const;

	inline MatrixSize GetSize() const;

    inline int_max GetElementNumber() const;

	inline int_max GetColNumber() const;

	inline int_max GetRowNumber() const;

    inline bool IsVector() const;

    inline bool IsRowVector() const;

    inline bool IsColVector() const;

    inline bool IsSquare() const;

    //------------------------ NaN Element -----------------------------//

    inline const ElementType& GetNaNElement() const;

    //------------------------ Element Type -----------------------------//

    inline MatrixElementTypeEnum GetElementType() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

    inline ElementType* begin(); // the position of the first element

    inline const ElementType* begin() const;

    inline const ElementType* end() const; // the last position + 1

	//----------- Get/Set Matrix(LinearIndex) -----------------------------------//

    // operator[] or () : no bound check in release mode

    inline ElementType& operator[](int_max LinearIndex);

    inline const ElementType& operator[](int_max LinearIndex) const;

	inline ElementType& operator()(int_max LinearIndex);

    inline const ElementType& operator()(int_max LinearIndex) const;
    
    // at(): bound check

	inline ElementType& at(int_max LinearIndex);

	inline const ElementType& at(int_max LinearIndex) const;

	//----------- Get/Set Matrix(i,j)  ---------------------------------------------//

    // operator() : no bound check in release mode

	inline ElementType& operator()(int_max RowIndex, int_max ColIndex);

	inline const ElementType& operator()(int_max RowIndex, int_max ColIndex) const;

    // at(): bound check

	inline ElementType& at(int_max RowIndex, int_max ColIndex);

	inline const ElementType& at(int_max RowIndex, int_max ColIndex) const;

    //----------- Get/Set a set of elements by Matrix({}), Matrix.at({}) -------------------------------------//

    // operator(): no bound check in release mode
    //
    // note: operator[] is for single element access only, operator[{}] is not defined

    inline DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& LinearIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const std::vector<int_max>& LinearIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const std::vector<int_max>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& LinearIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    // at(): bound check -----------------

    inline DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& LinearIndexList);

    inline const DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const std::vector<int_max>& LinearIndexList);

    inline const DenseShadowMatrix<ElementType> at(const std::vector<int_max>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& LinearIndexList);

    inline const DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol); // convert matrix to col-vector

    inline const DenseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    //---------- Get/Set SubMatrix by Matrix({...},{...}), Matrix.at({...}, {...}), Matrix.Col({...}), Matrix.Row({...}), Matrix.Diagnal() -------//

    // operator(): no bound check in release mode

    inline DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList,
                                                     const std::initializer_list<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList,
                                                           const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList, 
                                                     const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList, 
                                                           const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                     const std::initializer_list<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                           const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const std::vector<int_max>& RowIndexList,
                                                     const std::vector<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const std::vector<int_max>& RowIndexList,
                                                           const std::vector<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const std::vector<int_max>& RowIndexList,
                                                     const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> operator()(const std::vector<int_max>& RowIndexList,
                                                           const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                     const std::vector<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                           const std::vector<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                     const DenseMatrix<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                           const DenseMatrix<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                     const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                           const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                     const DenseMatrix<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                           const DenseMatrix<int_max>& ColIndexList) const;

    // at(): bound check -----------------

    inline DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                             const std::initializer_list<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                                   const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                             const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                                   const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                             const std::initializer_list<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                   const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const std::vector<int_max>& RowIndexList,
                                             const std::vector<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const std::vector<int_max>& RowIndexList,
                                                   const std::vector<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const std::vector<int_max>& RowIndexList,
                                             const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> at(const std::vector<int_max>& RowIndexList,
                                                   const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                             const std::vector<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                   const std::vector<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                             const DenseMatrix<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                                   const DenseMatrix<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                             const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                                   const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                             const DenseMatrix<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                   const DenseMatrix<int_max>& ColIndexList) const;

    // Col(...) is just at(ALL, ...)

    inline DenseShadowMatrix<ElementType> Col(int_max ColIndex);

    // do not use const in Col(const std::initializer_list<int_max>& ColIndexList); 
    // it leads to ambiguous call (vs2013), 
    // e.g., Col({0})  it can initialize Col(int_max) or Col(std::vector);
    //
    // so: use std::initializer_list<int_max> without const 
    //
    inline DenseShadowMatrix<ElementType> Col(const std::initializer_list<int_max>& ColIndexList);

    inline DenseShadowMatrix<ElementType> Col(const std::vector<int_max>& ColIndexList);

    inline DenseShadowMatrix<ElementType> Col(const DenseMatrix<int_max>& ColIndexList);

    // Row(...) is just at(..., ALL)

    inline DenseShadowMatrix<ElementType> Row(int_max RowIndex);

    inline DenseShadowMatrix<ElementType> Row(const std::initializer_list<int_max>& RowIndexList);

    inline DenseShadowMatrix<ElementType> Row(const std::vector<int_max>& RowIndexList);

    inline DenseShadowMatrix<ElementType> Row(const DenseMatrix<int_max>& RowIndexList);

    //
    inline DenseShadowMatrix<ElementType> Diangonal();

    inline const DenseShadowMatrix<ElementType> Diangonal() const;

    // return SubMatrix as DenseMatrix ---------------------------------------------------------------

    inline DenseMatrix GetSubMatrix(const std::initializer_list<int_max>& RowIndexList,
                                    const std::initializer_list<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType> &OutputMatrix, 
                             const std::initializer_list<int_max>& RowIndexList, 
                             const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseMatrix GetSubMatrix(const std::initializer_list<int_max>& RowIndexList,
                                    const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseMatrix GetSubMatrix(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, 
                                    const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseMatrix GetSubMatrix(const std::vector<int_max>& RowIndexList,
                                    const std::vector<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType> &OutputMatrix,
                             const std::vector<int_max>& RowIndexList,
                             const std::vector<int_max>& ColIndexList) const;

    inline DenseMatrix GetSubMatrix(const std::vector<int_max>& RowIndexList,
                                    const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseMatrix GetSubMatrix(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, 
                                    const std::vector<int_max>& ColIndexList) const;

    inline DenseMatrix GetSubMatrix(const DenseMatrix<int_max>& RowIndexList,
                                    const DenseMatrix<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType> &OutputMatrix, 
                             const DenseMatrix<int_max>& RowIndexList, 
                             const DenseMatrix<int_max>& ColIndexList) const;

    inline DenseMatrix GetSubMatrix(const DenseMatrix<int_max>& RowIndexList,
                                    const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline DenseMatrix GetSubMatrix(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, 
                                    const DenseMatrix<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType> &OutputMatrix, 
                             const int_max* RowIndexList, int_max OutputRowNumber,
                             const int_max* ColIndexList, int_max OutputColNumber) const;

	//---------------------- Get/Set/Fill/Append/Insert A Single Column, Delete multi-Columns ----------------------------------------//
	
    inline DenseMatrix GetCol(int_max ColIndex) const;

    inline bool GetCol(int_max ColIndex, std::vector<ElementType>& ColData) const;

    inline bool GetCol(int_max ColIndex, DenseMatrix<ElementType>& ColData) const;

    inline bool GetCol(int_max ColIndex, ElementType* ColData) const;

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const ElementType_Input* ColData, int_max Length);
	
    inline bool FillCol(int_max ColIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendCol(const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool AppendCol(const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool AppendCol(const DenseMatrix<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool AppendCol(const ElementType_Input* ColData, int_max Length);
	
    inline bool DeleteCol(int_max ColIndex);

    inline bool DeleteCol(const std::initializer_list<int_max>& ColIndexList);

    inline bool DeleteCol(const std::vector<int_max>& ColIndexList);

    inline bool DeleteCol(const DenseMatrix<int_max>& ColIndexList);

    inline bool DeleteCol(const int_max* ColIndexList, int_max Length);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const ElementType_Input* ColData, int_max Length);

	//---------------------- Get/Set/Fill/Append A Single Row, Delete Multi-Rows  ----------------------------------------//
	
    inline DenseMatrix GetRow(int_max RowIndex) const;

    inline bool GetRow(int_max RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(int_max RowIndex, DenseMatrix<ElementType>& RowData) const;

    inline bool GetRow(int_max RowIndex, ElementType* RowData) const;

    template<typename ElementType_Input>
    inline bool SetRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int_max RowIndex, const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool SetRow(int_max RowIndex, const ElementType_Input* RowData, int_max Length);

    inline bool FillRow(int_max RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendRow(const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool AppendRow(const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool AppendRow(const DenseMatrix<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool AppendRow(const ElementType_Input* RowData, int_max Length);

    inline bool DeleteRow(int_max RowIndex);

    inline bool DeleteRow(const std::initializer_list<int_max>& RowIndexList);

    inline bool DeleteRow(const std::vector<int_max>& RowIndexList);

    inline bool DeleteRow(const DenseMatrix<int_max>& RowIndexList);

    inline bool DeleteRow(const int_max* RowIndexList, int_max Length);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const ElementType_Input* RowData, int_max Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline DenseMatrix GetDiangonal() const;

    inline bool GetDiangonal(std::vector<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(ElementType* DiangonalData) const;

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::vector<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const DenseMatrix<ElementType_Input>& DiangonalData);

	template<typename ElementType_Input>
	inline bool SetDiangonal(const ElementType_Input* DiangonalData);

    inline bool FillDiangonal(const ElementType& Element);

	//---------------------- Matrix {+= -= *= /=} Matrix ----------------------------------------//

	inline void operator+=(const DenseMatrix<ElementType>& InputMatrix);

	inline void operator-=(const DenseMatrix<ElementType>& InputMatrix);

	inline void operator*=(const DenseMatrix<ElementType>& InputMatrix);

	inline void operator/=(const DenseMatrix<ElementType>& InputMatrix);


    inline void operator+=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator/=(const DenseShadowMatrix<ElementType>& ShadowMatrix);


    inline void operator+=(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator-=(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator*=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator/=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);


    inline void operator+=(DenseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator-=(DenseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator*=(DenseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator/=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

	// error:
	//template<typename ElementType_Input>
	//inline void operator+(ElementType_Input Element);

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //-------------------- special element operation {^} -----------------------------------------------------------//

    inline DenseMatrix operator^(const ElementType& Element);

    inline void operator^=(const ElementType& Element);

    //-------------------- special element operation : (.*) element multiply -----------------------------------------------------------//

    inline DenseMatrix ElementMultiply(const ElementType& Element) const;

    inline DenseMatrix ElementMultiply(const DenseMatrix<ElementType>& InputMatrix) const;

    inline DenseMatrix ElementMultiply(DenseMatrix<ElementType>&& InputMatrix) const;

    inline DenseMatrix ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix) const;

    inline DenseMatrix ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const;

    inline DenseMatrix ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const;

	//-------------------- general element operation : output a new matrix with the same size ------------------------------------------//

    inline DenseMatrix ElementNamedOperation(const std::string& OperationName) const;

    template<typename OperationType>
    inline DenseMatrix ElementOperation(OperationType Operation) const;

    inline DenseMatrix ElementNamedOperation(const char OperationName, const DenseMatrix<ElementType>& InputMatrix) const;

    inline DenseMatrix ElementNamedOperation(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix) const;

    template<typename OperationType>
    inline DenseMatrix ElementOperation(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix) const;

    inline DenseMatrix ElementNamedOperation(const char OperationName, const ElementType& Element) const;

    inline DenseMatrix ElementNamedOperation(const std::string& OperationName, const ElementType& Element) const;

    template<typename OperationType>
    inline DenseMatrix ElementOperation(OperationType Operation, const ElementType& Element) const;

    //-------------------- general element operation in place : Object.ElementOperationInPlace modify the object itself ---------------//

    inline bool ElementNamedOperationInPlace(const std::string& OperationName);

    template<typename OperationType>
    inline bool ElementOperationInPlace(OperationType Operation);

    inline bool ElementNamedOperationInPlace(const char OperationName, const DenseMatrix<ElementType>& InputMatrix);

    inline bool ElementNamedOperationInPlace(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix);

    template<typename OperationType>
    inline bool ElementOperationInPlace(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix);

    inline bool ElementNamedOperationInPlace(const char OperationName, const ElementType& Element);

    inline bool ElementNamedOperationInPlace(const std::string& OperationName, const ElementType& Element);

    template<typename OperationType>
    inline bool ElementOperationInPlace(OperationType Operation, const ElementType& Element);

    //-------------------- general Col operation : output a new col-matrix ------------------------------------------//

    inline DenseMatrix ColNamedOperation(int_max ColIndex, const std::string& OperationName, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix ColOperation(int_max ColIndex, OperationType Operation, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColNamedOperation(int_max ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColNamedOperation(int_max ColIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;
    
    template<typename OperationType>
    inline DenseMatrix ColOperation(int_max ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColNamedOperation(int_max ColIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColNamedOperation(int_max ColIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix ColOperation(int_max ColIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true) const;

    //-------------------- general col operation in place : Object.ColOperationInPlace modify the object itself ---------------//

    inline bool ColNamedOperationInPlace(int_max ColIndex, const std::string& OperationName, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool ColOperationInPlace(int_max ColIndex, OperationType Operation, bool EnableBoundCheck = true);

    inline bool ColNamedOperationInPlace(int_max ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool ColNamedOperationInPlace(int_max ColIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool ColOperationInPlace(int_max ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool ColNamedOperationInPlace(int_max ColIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    inline bool ColNamedOperationInPlace(int_max ColIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool ColOperationInPlace(int_max ColIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true);

    //-------------------- general Row operation : output a new row-matrix ------------------------------------------//

    inline DenseMatrix RowNamedOperation(int_max RowIndex, const std::string& OperationName, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix RowOperation(int_max RowIndex, OperationType Operation, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowNamedOperation(int_max RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowNamedOperation(int_max RowIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix RowOperation(int_max RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowNamedOperation(int_max RowIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowNamedOperation(int_max RowIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix RowOperation(int_max RowIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true) const;

    //-------------------- general row operation in place : Object.RowOperationInPlace modify the object itself ---------------//

    inline bool RowNamedOperationInPlace(int_max RowIndex, const std::string& OperationName, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool RowOperationInPlace(int_max RowIndex, OperationType Operation, bool EnableBoundCheck = true);

    inline bool RowNamedOperationInPlace(int_max RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool RowNamedOperationInPlace(int_max RowIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool RowOperationInPlace(int_max RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool RowNamedOperationInPlace(int_max RowIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    inline bool RowNamedOperationInPlace(int_max RowIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool RowOperationInPlace(int_max RowIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true);

	//-------------------- calculate sum mean min max ------------------------------------------//

    inline ElementType Mean() const;

    inline DenseMatrix MeanToRow() const;

    inline DenseMatrix MeanToCol() const;

    inline ElementType Sum() const;

    inline DenseMatrix SumToRow() const;

    inline DenseMatrix SumToCol() const;

    inline ElementType Max() const;

    inline DenseMatrix MaxToRow() const;

    inline DenseMatrix MaxToCol() const;

    inline ElementType Min() const;

    inline DenseMatrix MinToRow() const;

    inline DenseMatrix MinToCol() const;

    //------------------------------------ norm ----------------------------------------------//

    inline ElementType L1Norm() const;

    inline ElementType L2Norm() const;

	//----------------------------------- transpose -----------------------------------------//

    inline DenseMatrix Transpose() const;

	//----------------------------------- Rank -----------------------------------------//

    inline int_max Rank() const;

	//----------------------------------- inverse -----------------------------------------//

    inline DenseMatrix Inv() const;

    inline DenseMatrix PseudoInv() const;

	//----------------------------------- SVD -----------------------------------------//

    inline DenseMatrixSVDResult<ElementType> SVD() const;


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
