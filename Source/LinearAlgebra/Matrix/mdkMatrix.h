#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
#include "mdkMatrixCommon.h"
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
class mdkDenseMatrix;

template<typename ElementType>
class mdkDenseShadowMatrix;

template<typename ElementType>
class mdkSparseMatrix;

template<typename ElementType>
class mdkSparseShadowMatrix;

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

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkMatrix : public mdkObject
{

private:
     
    mdkDenseMatrix<ElementType> m_DenseMatrix;

    mdkSparseMatrix<ElementType> m_SparseMatrix;

    bool m_Is_DenseMatrix;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline mdkMatrix();

    inline mdkMatrix(int64 RowNumber, int64 ColNumber);

    inline mdkMatrix(const ElementType& Element);

    // deep-copy or shared-copy constructor
    inline mdkMatrix(const mdkMatrix<ElementType>& InputMatrix, mdkObjectCopyConstructionTypeEnum Method = mdkObjectCopyConstructionTypeEnum::DeepCopy);

    // move constructor
    inline mdkMatrix(mdkMatrix<ElementType>&& InputMatrix);

    inline mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkMatrix(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline mdkMatrix(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline mdkMatrix(const ElementType* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

	inline ~mdkMatrix();

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const mdkMatrix<ElementType>& InputMatrix);

    // move assignment operator
    inline void operator=(mdkMatrix<ElementType>&& InputMatrix);

    // other 
    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& list);

    inline void operator=(const std::initializer_list<std::initializer_list<ElementType>>& list);

    inline void operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //----------------------  DeepCopy From Matrix or Element  ----------------------------------------//

    // DeepCopy can be used to convert a matrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool DeepCopy(const mdkMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool DeepCopy(const mdkMatrix<ElementType_Input>* InputMatrix);

    template<typename ElementType_Input>
    inline bool DeepCopy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

    inline bool Fill(const ElementType& Element);

    //-------------------------- SharedCopy, ForceSharedCopy  ------------------------------------------ //
 
    // Why I use the name: SharedCopy instead of Share :
    // A.Share(B) literally means Matrix A and Matrix B share the same thing
    // but after B.Clear(), and B.Resize(10,10), A and B have different data pointed by different shared_ptr (m_MatrixData)
    // The "Share" is one-time thing, not share forever, maybe OneTimeShare is better than Share
    // ShallowCopy is a better name: it means one-time operation
    // I use the name SharedCopy: A.SharedCopy(B) means A is a copy of B by sharing

    // if m_IsSizeFixed is true, and size does not match, then return false
    //
    inline bool SharedCopy(mdkMatrix<ElementType>& InputMatrix);

    inline bool SharedCopy(mdkMatrix<ElementType>* InputMatrix);

    // it is used by GlueMatrix
    // Share the object (InputMatrix) no matter what, even if InputMatrix is const
    inline void ForceSharedCopy(const mdkMatrix<ElementType>& InputMatrix);

    inline bool ForceSharedCopy(const mdkMatrix<ElementType>* InputMatrix);

    // about const share: i.e., only read (const functions), not write
    //
    // note1: if const share of one single object is needed
    //        just use const reference: 
    //        const mdkMatrix<ElementType>& = InputMatrix
    //        const mdkMatrix<ElementType>& = InputFunction()(return InputMatrix) 
    //
    // note2: if const share of many objects is needed
    //        there is no such thing as std::vector<const mdkMatrix<ElementType>&>
    //        just create std::vector<const mdkMatrix<ElementType>*> MatrixPtrList = {&A}; from Matrix A
    //        then const prevent using (*MatrixPtrList[0])(0,0) = 10;
    //
    //        However,std::vector<const mdkMatrix<ElementType>> is equal to std::vector<mdkMatrix<ElementType>>
    //        std::vector<const mdkMatrix<ElementType>> MatrixList can be constructed from Matrix A by share
    //        
    //        MatrixList.emplace_back(A, mdkObjectConstructionTypeEnum::SHARE);
    //        But:
    //            MatrixList[0](1,1) = 100; CAN be compiled !!!  (A is changed by this code)
    //
    //-----------------------------------------------------------------------------------------------------
    // conclusion: 
    // (1) An array of shared objects can be created from InputMatrix
    //     std::vector<mdkMatrix<ElementType>> SharedMatrixArray(10);
    //     SharedMatrixArray[i].Share(InputMatrix);
    //     SharedMatrixArray[i].ForceShare(InputMatrix);
    //
    // (2) An array of const shared objects can be created by using std::vector, but const is lost
    //
    // (3) An array of const pointers to shared objects can be created from InputMatrix, NOT InputFunction()
    //     std::vector<const mdkMatrix<ElementType>*> SharedMatrixPointerArray(10);
    //     SharedMatrixPointerArray[i] = &InputMatrix;
    //------------------------------------------------------------------------------------------------------

    //-------------------- Take -----------------------------------------------------------//

    //Take the the ownership of the InputMatrix and Clear it

    inline bool Take(mdkMatrix<ElementType>* InputMatrix);

    inline bool Take(mdkMatrix<ElementType>& InputMatrix);

    inline bool Take(mdkMatrix<ElementType>&& InputMatrix);

    //Take the the ownership of the Matrix Created from ShadowMatrix or GlueMatrix

    inline bool Take(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline bool Take(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline bool Take(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //---------------- Get Data from DenseMatrix or SparseMatrix ----------------------------------------//

    template<typename ElementType_Input>
    inline bool DeepCopy(const mdkDenseMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool DeepCopy(const mdkDenseMatrix<ElementType_Input>* InputMatrix);

    // SharedCopy DenseMatrix
    inline bool SharedCopy(mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool SharedCopy(mdkDenseMatrix<ElementType>* InputMatrix);

    // ForceSharedCopy DenseMatrix
    inline void ForceSharedCopy(const mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool ForceSharedCopy(const mdkDenseMatrix<ElementType>* InputMatrix);

    // Take DenseMatrix
    inline bool Take(mdkDenseMatrix<ElementType>& InputMatrix);

    inline bool Take(mdkDenseMatrix<ElementType>* InputMatrix);

    // SharedCopy SparseMatrix
    inline bool SharedCopy(mdkSparseMatrix<ElementType>& InputMatrix);

    inline bool SharedCopy(mdkSparseMatrix<ElementType>* InputMatrix);

    // ForceSharedCopy SparseMatrix
    inline void ForceSharedCopy(const mdkSparseMatrix<ElementType>& InputMatrix);

    inline bool ForceSharedCopy(const mdkSparseMatrix<ElementType>* InputMatrix);

    // Take SparseMatrix
    inline bool Take(mdkSparseMatrix<ElementType>& InputMatrix);

    inline bool Take(mdkSparseMatrix<ElementType>* InputMatrix);

    //---------------- Output DenseMatrix or SparseMatrix -----------------------------------//

    inline mdkDenseMatrix<ElementType>& toDenseMatrix();

    inline mdkSparseMatrix<ElementType>& toSparseMatrix();

    //------------------------- Reset , Clear -------------------------------------------//
    
    // set the initial state, use it in constructor, do more things than Clear()
    inline void Reset();

    // clear memory no matter what, and set m_IsSizeFixed to be false
    inline void Clear();

	//---------------------- Set/get Matrix Size, Shape ----------------------------------------//

    inline bool Reshape(int64 InputRowNumber, int64 InputColNumber);

    inline bool Resize(int64 InputRowNumber, int64 InputColNumber);

    inline void FixSize();
    
    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

    inline bool IsNoneEmptyVector() const;

    inline bool IsNoneEmptyRowVector() const;

    inline bool IsNoneEmptyColVector() const;

	inline mdkMatrixSize GetSize() const;

    inline int64 GetElementNumber() const;

    inline int64 GetNonZeroElementNumber() const;

	inline int64 GetColNumber() const;

	inline int64 GetRowNumber() const;

    inline bool IsDenseMatrix() const;

    inline bool IsSparseMatrix() const;

    //------------------------ NaN Element -----------------------------//

    inline const ElementType& GetNaNElement() const;

    //------------------------ Element Type -----------------------------//

    inline mdkMatrixElementTypeEnum GetElementType() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

    //--------------------- Get Element -----------------------------//

    inline const ElementType& GetElement(int64 LinearIndex) const;

    inline const ElementType& GetElement(int64 RowIndex, int64 ColIndex) const;

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
