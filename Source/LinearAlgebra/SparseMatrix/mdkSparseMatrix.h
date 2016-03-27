#pragma once

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>
#include <cmath>
#include <algorithm>

#include "mdkObjectArray.h"

#include "mdkLinearAlgebraConfig.h"
#include "mdkLinearAlgebra_Function_Common.h"
#include "mdkMatrix_Common.h"

//#include "mdkSparseShadowMatrix.h"
//#include "mdkSparseGlueMatrixForMultiplication.h"
//#include "mdkSparseGlueMatrixForLinearCombination.h"
//#include "mdkSparseMatrixOperator.h"
//#include "mdkLinearAlgebra_Function_SparseMatrix.h"


namespace mdk
{

//-----------------------------
template<typename ElementType>
class SparseVector;
//------------------------------

// 2D SparseMatrix Class Template
// Compressed sparse column (CSC)
//
// Compare to Matlab:
// SparseMatrix is the same as Matlab Sparse Matrix except index staring from 0 instead of 1 in Matlab
//

// ----------------------------- SparseMatrixDataInCSCFormat struct -------------------------------------------------------------//

template<typename ElementType>
struct SparseMatrixDataInCSCFormat
{
    int_max m_RowNumber;  // RowNumber = the Number of Rows 
    int_max m_ColNumber;  // ColNumber = the Number of Columns

    std::vector<int_max> m_RowIndexList;    // .size() is total number of elements

    std::vector<ElementType> m_ElementList; // .size() is total number of elements

    std::vector<int_max> m_ColBeginElementIndexInElementList; // .size() is ColNumber
	// Index of the first element in each Col
	// Index in m_RowIndexList, m_ColIndexList, m_ElementList
    // Index is -1 if col is empty    

	std::vector<int_max> m_RecordedElementNumberInEachCol; // .size() is ColNumber

    ElementType m_ZeroElement;

    ElementType m_NaNElement;

    bool m_IsSizeFixed;

//-------------------------------------------------------------------------------------------------
    inline SparseMatrixDataInCSCFormat();

    inline ~SparseMatrixDataInCSCFormat();

    inline void Construct(int_max InputRowNumber, int_max InputColNumber); // all zero

    inline void Construct(const int_max* InputRowIndexList,
                          const int_max* InputColIndexList,
                          const ElementType* InputElementList,
                          int_max RecordedElementNumber,
                          int_max InputRowNumber,
                          int_max InputColNumber,
                          int_max AdditionalReservedCapacity = 0);

    inline void ConstructColVector(const int_max* InputRowIndexList,
                                   const ElementType* InputElementList,
                                   int_max RecordedElementNumber,
                                   int_max InputRowNumber);

    inline void ConstructRowVector(const int_max* InputColIndexList,
                                   const ElementType* InputElementList,
                                   int_max RecordedElementNumber,
                                   int_max InputColNumber);

    inline void ConstructFromSortedData(std::vector<int_max> InputRowIndexList,
                                        std::vector<int_max> InputColIndexList,
                                        std::vector<ElementType> InputElementList,
                                        int_max InputRowNumber,
                                        int_max InputColNumber);

    inline void ConstructColVectorFromSortedData(std::vector<int_max> InputRowIndexList,
                                                 std::vector<ElementType> InputElementList,
                                                 int_max InputRowNumber);

    inline void ConstructRowVectorFromSortedData(std::vector<int_max> InputColIndexList,
                                                 std::vector<ElementType> InputElementList,
                                                 int_max InputColNumber);

    //---------------------------------------------------------------------

    inline void Clear();

    inline void Resize(int_max InputRowNumber, int_max InputColNumber);

    inline void FastResize(int_max InputRowNumber, int_max InputColNumber);

    //------------------------------------------
    // note 1: [] and () have no bound check in release mode
    // note 2: none cost operator [] and () willl created a new record at location [k], (k) or (RowIndex, ColIndex)
    //         if there is no record in that location

    inline ElementType& operator[](int_max LinearIndex); 

    inline const ElementType& operator[](int_max LinearIndex) const;

    inline ElementType& operator()(int_max LinearIndex);

    inline const ElementType& operator()(int_max LinearIndex) const;

    inline ElementType& operator()(int_max RowIndex, int_max ColIndex);

    inline const ElementType& operator()(int_max RowIndex, int_max ColIndex) const;

    //------------------------------------------
    // note: output -1 if no record exits in the input location

	inline int_max GetElementIndexInElementList(int_max LinearIndex) const;

	inline int_max GetElementIndexInElementList(int_max RowIndex, int_max ColIndex) const;

    //------------------------------------------
    // note: output ElementType(0) if no record exits in the input location

    inline const ElementType& GetElement(int_max LinearIndex) const;

    inline const ElementType& GetElement(int_max RowIndex, int_max ColIndex) const;

    inline ElementType& SetElement(int_max LinearIndex, const ElementType& InputElement);

    inline ElementType& SetElement(int_max RowIndex, int_max ColIndex, const ElementType& InputElement);

    //-----------------------------------------

    inline void Copy(const SparseMatrixDataInCSCFormat& InputData);

    inline void Take(SparseMatrixDataInCSCFormat& InputData);

    //------------------------------------------
    // get the length of m_ElementList

    inline int_max GetRecordedElementNumber() const;

    inline std::vector<int_max> GetColIndexList() const;

    //-----------------------------------------

    ElementType* GetPointerOfBeginElementInCol(int_max ColIndex);

    const ElementType* GetPointerOfBeginElementInCol(int_max ColIndex) const;

	int_max GetIndexInElementListOfBeginElementInCol(int_max ColIndex) const;

	int_max GetRowIndexOfBeginElementInCol(int_max ColIndex) const;

    int_max GetRecordedElementNumberInCol(int_max ColIndex) const;

    inline std::vector<int_max> GetRowIndexListInCol(int_max ColIndex) const;

private:
//deleted: -------------------------------------------------
    SparseMatrixDataInCSCFormat(const SparseMatrixDataInCSCFormat&) = delete;

    SparseMatrixDataInCSCFormat(SparseMatrixDataInCSCFormat&&) = delete;

    void operator=(const SparseMatrixDataInCSCFormat&) = delete;

    void operator=(SparseMatrixDataInCSCFormat&&) = delete;
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename Element_Type>
class SparseMatrix : public Object
{
public:
	typedef Element_Type  ElementType;

private:    
    std::shared_ptr<SparseMatrixDataInCSCFormat<ElementType>> m_MatrixData;    

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline SparseMatrix();

    inline SparseMatrix(int_max RowNumber, int_max ColNumber);

    inline SparseMatrix(const ElementType& Element);

    inline SparseMatrix(const std::initializer_list<ElementType>& InputList);

    inline SparseMatrix(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList);

    // copy constructor
	inline SparseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix);

    // move constructor
    inline SparseMatrix(SparseMatrix<ElementType>&& InputSparseMatrix);

    //inline SparseMatrix(const SparseShadowMatrix<ElementType>& SparseShadowMatrix);

    //inline SparseMatrix(const SparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix);

    //inline SparseMatrix(const SparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix);

	inline ~SparseMatrix();

    //------------------------------construction method -----------------------------//
    
    inline bool Construct(const std::initializer_list<int_max>& RowIndexList,
					      const std::initializer_list<int_max>& ColIndexList,
                          const std::initializer_list<ElementType>& ElementList,
                          int_max RowNumber,
                          int_max ColNumber,
                          int_max AdditionalReservedCapacity = 0);

    inline bool Construct(const std::vector<int_max>& RowIndexList,
                          const std::vector<int_max>& ColIndexList,
                          const std::vector<ElementType>& ElementList,
						  int_max RowNumber,
						  int_max ColNumber,
                          int_max AdditionalReservedCapacity = 0);

    inline bool Construct(const DenseMatrix<int_max>& RowIndexList,
                       const DenseMatrix<int_max>& ColIndexList,
                       const DenseMatrix<ElementType>& ElementList,
                       int_max RowNumber,
                       int_max ColNumber,
                       int_max AdditionalReservedCapacity = 0);

    inline bool Construct(const int_max* RowIndexList,
                          const int_max* ColIndexList,
                          const ElementType* ElementList,
                          int_max RecordedElementNumber,
                          int_max RowNumber,
                          int_max ColNumber,
                          int_max AdditionalReservedCapacity = 0);

    inline void ConstructFromSortedData(const std::vector<int_max>& RowIndexList,
                                        const std::vector<int_max>& ColIndexList,
                                        const std::vector<ElementType>& ElementList,
                                        int_max RowNumber,
                                        int_max ColNumber);

    inline bool ConstructColVector(const std::initializer_list<int_max>& RowIndexList,
                                   const std::vector<ElementType>& ElementList,
                                   int_max RowNumber);

    inline bool ConstructColVector(const std::vector<int_max>& RowIndexList,
                                   const std::vector<ElementType>& ElementList,
                                   int_max RowNumber);


    inline bool ConstructColVector(const DenseMatrix<int_max>& RowIndexList,
                                   const DenseMatrix<ElementType>& ElementList,
                                   int_max RowNumber);


    inline void ConstructColVectorFromSortedData(std::vector<int_max> RowIndexList,
                                                 std::vector<ElementType> ElementList,
                                                 int_max RowNumber);

    inline bool ConstructRowVector(const std::initializer_list<int_max>& ColIndexList,
                                   const std::vector<ElementType>& ElementList,
                                   int_max ColNumber);

    inline bool ConstructRowVector(const std::vector<int_max>& ColIndexList,
                                   const std::vector<ElementType>& ElementList,
                                   int_max ColNumber);


    inline bool ConstructRowVector(const DenseMatrix<int_max>& ColIndexList,
                                   const DenseMatrix<ElementType>& ElementList,
                                   int_max ColNumber);

    inline bool ConstructFromSparseColVectorSetInOrder(const std::vector<SparseVector<ElementType>>& SparseVectorSet);

	inline bool ConstructFromSparseColVectorSetInOrder(const ObjectArray<SparseVector<ElementType>>& SparseVectorSet);

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const SparseMatrix<ElementType>& InputMatrix);

    // move assignment operator
    inline void operator=(SparseMatrix<ElementType>&& InputMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    inline void operator=(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList);

    //inline void operator=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    //inline void operator=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    //inline void operator=(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //----------------------  Copy From SparseMatrix or Element  ----------------------------------------//

    // Copy can be used to convert a SparseMatrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool Copy(const SparseMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int_max InputRowNumber, int_max InputColNumber);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Share, ForceShare  ------------------------------------------ //
 
    // if m_IsSizeFixed is true, and size does not match, then return false
    //
    inline bool Share(SparseMatrix<ElementType>& InputMatrix);

    inline bool Share(SparseMatrix<ElementType>* InputMatrix);

    // Share the object (InputSparseMatrix) no matter what, even if InputSparseMatrix is const
    //
    inline void ForceShare(const SparseMatrix<ElementType>& InputMatrix);

    inline bool ForceShare(const SparseMatrix<ElementType>* InputMatrix);

    //-------------------- Take -----------------------------------------------------------//

    //Take the the ownership of the InputSparseMatrix and ForceClear it

    inline bool Take(SparseMatrix<ElementType>* InputMatrix);

    inline bool Take(SparseMatrix<ElementType>& InputMatrix);

    inline bool Take(SparseMatrix<ElementType>&& InputMatrix);

    //Take the the ownership of the SparseMatrix Constructd from ShadowSparseMatrix or SparseGlueMatrix

    //inline bool Take(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    //inline bool Take(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    //inline bool Take(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

	//----------------------------------------------------------------------------------------//
	// clear memory no matter what, and set m_IsSizeFixed to be false
    inline void Clear();

	//---------------------- Set/get SparseMatrix Size, Shape ----------------------------------------//

    inline bool Reshape(int_max InputRowNumber, int_max InputColNumber);

    inline bool Resize(int_max InputRowNumber, int_max InputColNumber);

    inline bool FastResize(int_max InputRowNumber, int_max InputColNumber);

    inline bool Resize(int_max InputElementNumber); // try to keep the old data, can not use this to resize a m x n matrix (m>1 or n>1)

    inline bool FastResize(int_max InputElementNumber); // do not care about old data, can not use this to resize a m x n matrix (m>1 or n>1)

    inline void FixSize();
    
    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

    inline bool IsPureEmpty() const;

	inline MatrixSize GetSize() const;

    inline int_max GetElementNumber() const;

    inline int_max GetRecordedElementNumber() const;

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

    //------------ Get Array ---------------------------------------------//

    inline const std::vector<int_max>& RowIndexList() const;

	inline std::vector<int_max> GetRowIndexList() const;

    inline std::vector<int_max> GetColIndexList() const;

    inline const std::vector<ElementType>& GetElmentList() const;

	inline const std::vector<int_max>& GetColBeginElementIndexInElmentList() const;

    std::vector<int_max> GetRowIndexListInCol(int_max ColIndex) const;

    //--------------------- Get Data Pointer -----------------------------//

	inline ElementType* GetPointerOfElementList();

	inline const ElementType* GetPointerOfElementList() const;

    inline int_max* GetPointerOfRowIndexList();

    inline const int_max* GetPointerOfRowIndexList() const;

	inline int_max* GetPointerOfColBeginElementIndexInElementList();

	inline const int_max* GetPointerOfColBeginElementIndexInElementList() const;

    inline ElementType* GetPointerOfBeginElementInCol(int_max ColIndex);

    inline const ElementType* GetPointerOfBeginElementInCol(int_max ColIndex) const;

	inline int_max GetIndexInElementListOfBeginElementInCol(int_max ColIndex) const;

    inline int_max GetRowIndexOfBeginElementInCol(int_max ColIndex) const;

    inline int_max GetRecordedElementNumberInCol(int_max ColIndex) const;

    //--------------------- Get/Set Element -----------------------------//

    inline const ElementType& GetElement(int_max LinearIndex) const;

    inline const ElementType& GetElement(int_max RowIndex, int_max ColIndex) const;

    inline bool SetElement(int_max LinearIndex, const ElementType& InputElement);

    inline bool SetElement(int_max RowIndex, int_max ColIndex, const ElementType& InputElement);

	//----------- Get/Set SparseMatrix(LinearIndex) -----------------------------------//

    // none-const operator [] and (), and at() will create a new record at location [k] or (k) 
    // if there is no record at that location 

    // operator[] or () : no bound check in release mode

    inline ElementType& operator[](int_max LinearIndex);

    inline const ElementType& operator[](int_max LinearIndex) const;

	inline ElementType& operator()(int_max LinearIndex);

    inline const ElementType& operator()(int_max LinearIndex) const;
    
    // at(): bound check

	inline ElementType& at(int_max LinearIndex);

	inline const ElementType& at(int_max LinearIndex) const;

	//----------- Get/Set SparseMatrix(i,j)  ---------------------------------------------//

    // none-const operator [] and (), and at() will create a new record at location (i,j) 
    // if there is no record at that location 

    // operator() : no bound check in release mode

	inline ElementType& operator()(int_max RowIndex, int_max ColIndex);

	inline const ElementType& operator()(int_max RowIndex, int_max ColIndex) const;

    // at(): bound check

	inline ElementType& at(int_max RowIndex, int_max ColIndex);

	inline const ElementType& at(int_max RowIndex, int_max ColIndex) const;

    //---------------------- Get/Set a set of elements by SparseMatrix({}) or SparseMatrix.at({}) ----------------------------//
    /*
    // operator(): no bound check in release mode
    //
    // note: operator[] is for single element access only, operator[{}] is not defined
    
    inline SparseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const std::vector<int_max>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const std::vector<int_max>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    // at(): bound check -----------------

    inline SparseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const std::vector<int_max>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> at(const std::vector<int_max>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    //---------------------- Get/Set SubSparseMatrix by SparseMatrix({}, {}) or SparseMatrix.at({}, {})  or SparseMatrix.SubSparseMatrix(a, b, c, d) -------//

    // operator(): no bound check in release mode

    inline SparseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList,
                                                      const std::initializer_list<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList,
                                                            const std::initializer_list<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList,
                                                      const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList,
                                                            const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                      const std::initializer_list<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                            const std::initializer_list<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const std::vector<int_max>& RowIndexList,
                                                      const std::vector<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const std::vector<int_max>& RowIndexList,
                                                            const std::vector<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const std::vector<int_max>& RowIndexList,
                                                      const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> operator()(const std::vector<int_max>& RowIndexList,
                                                            const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                      const std::vector<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                            const std::vector<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                      const DenseMatrix<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                            const DenseMatrix<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                      const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                            const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                      const DenseMatrix<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                            const DenseMatrix<int_max>& ColIndexList) const;

    // at(): bound check -----------------

    inline SparseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                              const std::initializer_list<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                                    const std::initializer_list<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                              const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                                    const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                              const std::initializer_list<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                    const std::initializer_list<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const std::vector<int_max>& RowIndexList,
                                              const std::vector<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const std::vector<int_max>& RowIndexList,
                                                    const std::vector<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const std::vector<int_max>& RowIndexList,
                                              const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> at(const std::vector<int_max>& RowIndexList,
                                                    const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                              const std::vector<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                    const std::vector<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                              const DenseMatrix<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                                    const DenseMatrix<int_max>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                              const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                                    const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                              const DenseMatrix<int_max>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                    const DenseMatrix<int_max>& ColIndexList) const;

    //

    inline SparseShadowMatrix<ElementType> Col(int_max ColIndex);

    inline SparseShadowMatrix<ElementType> Col(std::initializer_list<int_max>& ColIndexList);

    inline SparseShadowMatrix<ElementType> Col(const std::vector<int_max>& ColIndexList);

    inline SparseShadowMatrix<ElementType> Row(int_max RowIndex);

    inline SparseShadowMatrix<ElementType> Row(std::initializer_list<int_max>& RowIndexList);

    inline SparseShadowMatrix<ElementType> Row(const std::vector<int_max>& RowIndexList);

    inline SparseShadowMatrix<ElementType> Diangonal();

    //-------------------------------- special col reference ------------------------------

    inline SparseMatrix RefCol(int_max ColIndex);

    inline const SparseMatrix RefCol(int_max ColIndex) const;

    // return SubSparseMatrix as SparseMatrix -----------------------------------------------

    inline SparseMatrix GetSubMatrix(const std::vector<int_max>& RowIndexList, 
                                     const std::vector<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(SparseMatrix<ElementType> &OutputSparseMatrix, 
                             const std::vector<int_max>& RowIndexList, 
                             const std::vector<int_max>& ColIndexList) const;

    inline SparseMatrix GetSubMatrix(const std::vector<int_max>& RowIndexList, 
                                     const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseMatrix GetSubMatrix(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, 
                                     const std::vector<int_max>& ColIndexList) const;


	//---------------------- Get/Set/Fill/Append/Delete/InsertCol Column ----------------------------------------//

    inline SparseMatrix GetCol(int_max ColIndex) const;

    inline bool GetCol(int_max ColIndex, SparseMatrix<ElementType>& ColData) const;

    inline bool GetCol(int_max ColIndex, DenseMatrix<ElementType>& ColData) const;

    inline bool GetCol(int_max ColIndex, std::vector<ElementType>& ColData) const;

    inline bool GetCol(int_max ColIndex, ElementType* ColData) const;

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const SparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const ElementType_Input* ColData, int_max Length);
	
    inline bool FillCol(int_max RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendCol(const SparseMatrix<ElementType_Input>& ColData);

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

    inline bool DeleteCol(const int_max* ColIndexListPtr, int_max Length);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const SparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const ElementType_Input* ColData, int_max Length);

	//---------------------- Get/Set/Fill/Append/Delete Row  ----------------------------------------//

    inline SparseMatrix GetRow(int_max RowIndex) const;

    inline bool GetRow(int_max RowIndex, SparseMatrix<ElementType>& RowData) const;

    inline bool GetRow(int_max RowIndex, DenseMatrix<ElementType>& RowData) const;

    inline bool GetRow(int_max RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(int_max RowIndex, ElementType* RowData) const;

    template<typename ElementType_Input>
    inline bool SetRow(int_max RowIndex, const SparseMatrix<ElementType_Input>& RowData);

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
    inline bool AppendRow(const SparseMatrix<ElementType_Input>& RowData);

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

    inline bool DeleteRow(const int_max* RowIndexListPtr, int_max Length);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const SparseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const ElementType_Input* RowData, int_max Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline SparseMatrix GetDiangonal() const;

    inline bool GetDiangonal(SparseMatrix<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(DenseMatrix<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(std::vector<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(ElementType* DiangonalData) const;

    template<typename ElementType_Input>
    inline bool SetDiangonal(const SparseMatrix<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::vector<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const DenseMatrix<ElementType_Input>& DiangonalData);

	template<typename ElementType_Input>
	inline bool SetDiangonal(const ElementType_Input* DiangonalData);

    inline bool FillDiangonal(const ElementType& Element);

	//---------------------- SparseMatrix {+= -= *= /=} SparseMatrix ----------------------------------------//

	inline void operator+=(const SparseMatrix<ElementType>& InputMatrix);

    inline void operator-=(const SparseMatrix<ElementType>& InputMatrix);

    inline void operator*=(const SparseMatrix<ElementType>& InputMatrix);

    //inline void operator/=(const SparseMatrix<ElementType>& InputMatrix); // 0/0

    inline void operator+=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    //inline void operator/=(const SparseShadowMatrix<ElementType>& ShadowMatrix);  // 0/0

    inline void operator+=(SparseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator-=(SparseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator*=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    //inline void operator/=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);   // 0/0

    inline void operator+=(SparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator-=(SparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator*=(SparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    //inline void operator/=(SparseGlueMatrixForMultiplication<ElementType> GlueMatrix); // 0/0

    //---------------------- SparseMatrix {+= -= *= /=} Element ----------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //-------------------- special element operation {^} -----------------------------------------------------------//

    inline SparseMatrix operator^(const ElementType& Element);

    inline void operator^=(const ElementType& Element);

    //-------------------- special element operation {.*} element multiply -----------------------------------------------------------//

    inline SparseMatrix ElementMultiply(const ElementType& Element) const;

    inline SparseMatrix ElementMultiply(const SparseMatrix<ElementType>& InputSparseMatrix) const;

    inline SparseMatrix ElementMultiply(const SparseShadowMatrix<ElementType>& SparseShadowMatrix) const;

    inline SparseMatrix ElementMultiply(const SparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix) const;

    inline SparseMatrix ElementMultiply(const SparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix) const;

    //-------------------- general element operation : output a new matrix ------------------------------------------//

    inline SparseMatrix ElementOperation(const char* OperationName) const;

    inline SparseMatrix ElementOperation(const std::string& OperationName) const;

    inline SparseMatrix ElementOperation(std::function<ElementType(const ElementType&)> Operation) const;

    inline SparseMatrix ElementOperation(const std::string& OperationName, const SparseMatrix<ElementType>& InputMatrix) const;

    inline SparseMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                            const SparseMatrix<ElementType>& InputMatrix) const;

    inline SparseMatrix ElementOperation(const char* OperationName, const ElementType& Element) const;

    inline SparseMatrix ElementOperation(const std::string& OperationName, const ElementType& Element) const;

    inline SparseMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element) const;

    //-------------------- general element operation in place : Object.ElementOperationInPlace modify the object ------------------//

    inline bool ElementOperationInPlace(const char* OperationName);

    inline bool ElementOperationInPlace(const std::string& OperationName);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&)> Operation);

    inline bool ElementOperationInPlace(const char* OperationName, const SparseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const std::string& OperationName, const SparseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                        const SparseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const char* OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(const std::string& OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element);

	//-------------------- calculate sum mean min max ------------------------------------------//

    inline ElementType Mean() const;

    inline SparseMatrix MeanToRow() const;

    inline SparseMatrix MeanToCol() const;

    inline ElementType Sum() const;

    inline SparseMatrix SumToRow() const;

    inline SparseMatrix SumToCol() const;

    inline ElementType Max() const;

    inline SparseMatrix MaxToRow() const;

    inline SparseMatrix MaxToCol() const;

    inline ElementType Min() const;

    inline SparseMatrix MinToRow() const;

    inline SparseMatrix MinToCol() const;

	//----------------------------------- transpose -----------------------------------------//

    inline SparseMatrix Transpose() const;


    inline void TransposeInPlace();

	//----------------------------------- Rank -----------------------------------------//

    inline int_max Rank() const;

	//----------------------------------- inverse -----------------------------------------//

    inline DenseMatrix<ElementType> Inv() const;

    inline DenseMatrix<ElementType> PseudoInv() const;

	//----------------------------------- SVD -----------------------------------------//

    inline SparseMatrixSVDResult<ElementType> SVD() const;

    */

	//---------------------------- private functions ---------------------------------------//

private:

};

//============================================= For Reference ===================================================//
/*
// ------------------------------------ SparseMatrix {+ - * /}  SparseMatrix ------------------------------------------------//

template<typename ElementType>
inline SparseMatrix<ElementType> operator+(const SparseMatrix<ElementType>& SparseMatrixA, const SparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator-(const SparseMatrix<ElementType>& SparseMatrixA, const SparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator*(const SparseMatrix<ElementType>& SparseMatrixA, const SparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator/(const SparseMatrix<ElementType>& SparseMatrixA, const SparseMatrix<ElementType>& SparseMatrixB);

// ------------------------------------ SparseMatrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline SparseMatrix<ElementType> operator+(SparseMatrix<ElementType>& SparseMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator-(SparseMatrix<ElementType>& SparseMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator*(SparseMatrix<ElementType>& SparseMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator/(SparseMatrix<ElementType>& SparseMatrixA, const ElementType& ElementB);

// ------------------------------------- Element {+ - * /} SparseMatrix ------------------------------------------------//

template<typename ElementType>
inline SparseMatrix<ElementType> operator+(const ElementType& ElementA, const SparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator-(const ElementType& ElementA, const SparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator*(const ElementType& ElementA, const SparseMatrix<ElementType>& SparseMatrixB);

template<typename ElementType>
inline SparseMatrix<ElementType> operator/(const ElementType& ElementA, const SparseMatrix<ElementType>& SparseMatrixB);

// ----------------------- Element = SparseMatrix (if 1x1) is not allowed in c++ --------------------------------------------//
// not allowed in c++
//template<typename ElementType>
//inline void operator=(ElementType& Element, const SparseMatrix<ElementType>& SparseMatrix);
*/
//=====================================================================================================================//


}//end namespace mdk

#include "mdkSparseMatrix.hpp"
