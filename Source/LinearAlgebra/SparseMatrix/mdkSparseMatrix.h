#ifndef __mdkSparseMatrix_h
#define __mdkSparseMatrix_h

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>
#include <cmath>
#include <algorithm>

#include "mdkLinearAlgebraConfig.h"

#include "mdkType.h"
#include "mdkMatrix_Common.h"
#include "mdkSparseShadowMatrix.h"
#include "mdkSparseGlueMatrixForMultiplication.h"
#include "mdkSparseGlueMatrixForLinearCombination.h"
#include "mdkSparseMatrixOperator.h"
#include "mdkLinearAlgebra_Function_SparseMatrix.h"


namespace mdk
{

// 2D SparseMatrix Class Template
// Compressed sparse column (CSC)
//
// Compare to Matlab:
// mdkSparseMatrix is the same as Matlab Sparse Matrix except index staring from 0 instead of 1 in Matlab
//

// ----------------------------- mdkSparseMatrixDataInCSCFormat struct -------------------------------------------------------------//

template<typename ElementType>
struct SparseMatrixDataInCSCFormat
{
    int64 m_RowNumber;  // RowNumber = the Number of Rows 
    int64 m_ColNumber;  // ColNumber = the Number of Columns

    std::vector<int64> m_RowIndexList;

    std::vector<int64> m_ColIndexList;

    std::vector<ElementType> m_DataArray;

    std::vector<int64> m_ColBeginElementLinearIndexInDataArray; // Index in m_RowIndexList m_ColIndexList m_DataArray

    std::vector<int64> m_RecordedElementNumberInEachCol;

    ElementType m_ZeroElement;

    bool m_IsSizeFixed;

//-------------------------------------------------------------------------------------------------
    inline SparseMatrixDataInCSCFormat();

    inline ~SparseMatrixDataInCSCFormat();

    inline void Construct(int64 InputRowNumber, int64 InputColNumber); // all zero

    inline void Construct(const int64* InputRowIndexList,
                          const int64* InputColIndexList,
                          const ElementType* InputDataArray,
                          int64 RecordedElementNumber,
                          int64 InputRowNumber,
                          int64 InputColNumber,
                          int64 AdditionalReservedCapacity = 0);

    //------------------------------------------

    inline void Clear();

    inline void Resize(int64 InputRowNumber, int64 InputColNumber);

    inline void FastResize(int64 InputRowNumber, int64 InputColNumber);

    //------------------------------------------
    // note 1: [] and () have no bound check in release mode
    // note 2: none cost operator [] and () willl created a new record at location [k], (k) or (RowIndex, ColIndex)
    //         if there is no record in that location

    inline ElementType& operator[](int64 LinearIndex); 

    inline const ElementType& operator[](int64 LinearIndex) const;

    inline ElementType& operator()(int64 LinearIndex);

    inline const ElementType& operator()(int64 LinearIndex) const;

    inline ElementType& operator()(int64 RowIndex, int64 ColIndex);

    inline const ElementType& operator()(int64 RowIndex, int64 ColIndex) const;

    //------------------------------------------
    // note: output -1 if no record exits in the input location

    inline int64 GetLinearIndexInDataArray(int64 LinearIndex) const;

    inline int64 GetLinearIndexInDataArray(int64 RowIndex, int64 ColIndex) const;

    //------------------------------------------
    // note: output ElementType(0) if no record exits in the input location

    inline const ElementType& GetElement(int64 LinearIndex) const;

    inline const ElementType& GetElement(int64 RowIndex, int64 ColIndex) const;

    inline ElementType& SetElement(int64 LinearIndex, const ElementType& InputElement);

    inline ElementType& SetElement(int64 RowIndex, int64 ColIndex, const ElementType& InputElement);

    //------------------------------------------
    // get the length of m_DataArray

    inline int64 GetRecordedElementNumber() const;

    //-----------------------------------------

    inline void Copy(const SparseMatrixDataInCSCFormat& InputData);

    inline void Take(SparseMatrixDataInCSCFormat& InputData);

private:
//deleted: -------------------------------------------------
    SparseMatrixDataInCSCFormat(const SparseMatrixDataInCSCFormat&) = delete;

    SparseMatrixDataInCSCFormat(SparseMatrixDataInCSCFormat&&) = delete;

    void operator=(const SparseMatrixDataInCSCFormat&) = delete;

    void operator=(SparseMatrixDataInCSCFormat&&) = delete;
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class SparseMatrix : public Object
{

private:
     
    std::shared_ptr<SparseMatrixDataInCSCFormat<ElementType>> m_MatrixData;

    ElementType m_NaNElement;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline SparseMatrix();

    inline SparseMatrix(int64 RowNumber, int64 ColNumber);

    inline SparseMatrix(const ElementType& Element);

    inline SparseMatrix(const std::initializer_list<ElementType>& InputList);

    inline SparseMatrix(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList);

    // copy constructor (Copy or Share)
    inline SparseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy);

    // move constructor
    inline SparseMatrix(SparseMatrix<ElementType>&& InputSparseMatrix);

    inline SparseMatrix(const SparseShadowMatrix<ElementType>& SparseShadowMatrix);

    inline SparseMatrix(const SparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix);

    inline SparseMatrix(const SparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix);

	inline ~SparseMatrix();

    //------------------------------construction method -----------------------------//

    inline bool Construct(const DenseMatrix<int64>& RowIndexList,
                          const DenseMatrix<int64>& ColIndexList,
                          const DenseMatrix<ElementType>& DataArray,
                          int64 RowNumber,
                          int64 ColNumber,
                          int64 AdditionalReservedCapacity = 0);

    inline bool Construct(const std::vector<int64>& RowIndexList,
                          const std::vector<int64>& ColIndexList,
                          const std::vector<ElementType>& DataArray,
                          int64 RowNumber,
                          int64 ColNumber,
                          int64 AdditionalReservedCapacity = 0);

    inline bool Construct(const std::initializer_list<int64>& RowIndexList,
                          const std::initializer_list<int64>& ColIndexList,
                          const std::initializer_list<ElementType>& DataArray,
                          int64 RowNumber,
                          int64 ColNumber,
                          int64 AdditionalReservedCapacity = 0);

    inline bool Construct(const int64* RowIndexList,
                          const int64* ColIndexList,
                          const ElementType* DataArray,
                          int64 RecordedElementNumber,
                          int64 RowNumber,
                          int64 ColNumber,
                          int64 AdditionalReservedCapacity = 0);

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

    inline void operator=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //----------------------  Copy From SparseMatrix or Element  ----------------------------------------//

    // Copy can be used to convert a SparseMatrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool Copy(const SparseMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

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

    //Take the the ownership of the SparseMatrix Created from ShadowSparseMatrix or SparseGlueMatrix

    inline bool Take(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline bool Take(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline bool Take(const SparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

public:
    // clear memory no matter what, and set m_IsSizeFixed to be false
    inline void Clear();

	//---------------------- Set/get SparseMatrix Size, Shape ----------------------------------------//

    inline bool Reshape(int64 InputRowNumber, int64 InputColNumber);

    inline bool Resize(int64 InputRowNumber, int64 InputColNumber);

    inline bool FastResize(int64 InputRowNumber, int64 InputColNumber);

    inline void FixSize();
    
    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

    inline bool IsPureEmpty() const;

	inline MatrixSize GetSize() const;

    inline int64 GetElementNumber() const;

    inline int64 GetRecordedElementNumber() const;

	inline int64 GetColNumber() const;

	inline int64 GetRowNumber() const;

    //------------------------ NaN Element -----------------------------//

    inline const ElementType& GetNaNElement() const;

    //------------------------ Element Type -----------------------------//

    inline MatrixElementTypeEnum GetElementType() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetRecordedElementPointer();

    inline const ElementType* GetRecordedElementPointer() const;

    //--------------------- Get/Set Element -----------------------------//

    inline const ElementType& GetElement(int64 LinearIndex) const;

    inline const ElementType& GetElement(int64 RowIndex, int64 ColIndex) const;

    inline bool SetElement(int64 LinearIndex, const ElementType& InputElement);

    inline bool SetElement(int64 RowIndex, int64 ColIndex, const ElementType& InputElement);

	//----------- Get/Set SparseMatrix(LinearIndex) -----------------------------------//

    // none-const operator [] and (), and at() will create a new record at location [k] or (k) 
    // if there is no record at that location 

    // operator[] or () : no bound check in release mode

    inline ElementType& operator[](int64 LinearIndex);

    inline const ElementType& operator[](int64 LinearIndex) const;

	inline ElementType& operator()(int64 LinearIndex);

    inline const ElementType& operator()(int64 LinearIndex) const;
    
    // at(): bound check

	inline ElementType& at(int64 LinearIndex);

	inline const ElementType& at(int64 LinearIndex) const;

	//----------- Get/Set SparseMatrix(i,j)  ---------------------------------------------//

    // none-const operator [] and (), and at() will create a new record at location (i,j) 
    // if there is no record at that location 

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

    inline SparseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    // at(): bound check -----------------

    inline SparseShadowMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const std::vector<int64>& LinearIndexList);

    inline const SparseShadowMatrix<ElementType> at(const std::vector<int64>& LinearIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    //---------------------- Get/Set SubSparseMatrix by SparseMatrix({}, {}) or SparseMatrix.at({}, {})  or SparseMatrix.SubSparseMatrix(a, b, c, d) -------//

    // operator(): no bound check in release mode

    inline SparseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                         std::initializer_list<int64>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                               std::initializer_list<int64>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList,
                                                         const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList,
                                                               const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                         const std::initializer_list<int64>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                               const std::initializer_list<int64>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                         const std::vector<int64>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                               const std::vector<int64>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                         const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                               const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                         const std::vector<int64>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                               const std::vector<int64>& ColIndexList) const;

    // at(): bound check -----------------

    inline SparseShadowMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                                 std::initializer_list<int64>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                                       std::initializer_list<int64>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                                 const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                                       const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                 const std::initializer_list<int64>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                       const std::initializer_list<int64>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                 const std::vector<int64>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                       const std::vector<int64>& ColIndexList) const;

    inline SparseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                 const ALL_Symbol_For_Matrix_Operator& ALL_Symbol);

    inline const SparseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                       const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                 const std::vector<int64>& ColIndexList);

    inline const SparseShadowMatrix<ElementType> at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol,
                                                       const std::vector<int64>& ColIndexList) const;

    // return SubSparseMatrix as SparseMatrix -----------------------------------------------

    inline SparseMatrix GetSubMatrix(const std::vector<int64>& RowIndexList, 
                                        const std::vector<int64>& ColIndexList) const;

    inline SparseMatrix GetSubMatrix(const std::vector<int64>& RowIndexList, 
                                        const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const;

    inline SparseMatrix GetSubMatrix(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, 
                                        const std::vector<int64>& ColIndexList) const;

    inline bool GetSubMatrix(SparseMatrix<ElementType> &OutputSparseMatrix, 
                             const std::vector<int64>& RowIndexList, 
                             const std::vector<int64>& ColIndexList) const;

	//---------------------- Get/Set/Fill/Append/Delete/InsertCol Column ----------------------------------------//
	
    inline SparseShadowMatrix<ElementType> Col(int64 ColIndex);

    inline SparseShadowMatrix<ElementType> Col(std::initializer_list<int64>& ColIndexList);

    inline SparseShadowMatrix<ElementType> Col(const std::vector<int64>& ColIndexList);

    inline SparseMatrix GetCol(int64 ColIndex) const;

    inline bool GetCol(int64 ColIndex, DenseMatrix<ElementType>& ColData) const;

    inline bool GetCol(int64 ColIndex, std::vector<ElementType>& ColData) const;

    inline bool GetCol(int64 ColIndex, ElementType* ColData) const;

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const SparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const DenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);
	
    inline bool FillCol(int64 RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendCol(const SparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool AppendCol(const DenseMatrix<ElementType_Input>& ColData);

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
    inline bool InsertCol(int64 ColIndex, const SparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const DenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);

	//---------------------- Get/Set/Fill/Append/Delete Row  ----------------------------------------//
	
    inline SparseShadowMatrix<ElementType> Row(int64 RowIndex);

    inline SparseShadowMatrix<ElementType> Row(std::initializer_list<int64>& RowIndexList);

    inline SparseShadowMatrix<ElementType> Row(const std::vector<int64>& RowIndexList);

    inline SparseMatrix GetRow(int64 RowIndex) const;

    inline bool GetRow(int64 RowIndex, DenseMatrix<ElementType>& RowData) const;

    inline bool GetRow(int64 RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(int64 RowIndex, ElementType* RowData) const;

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const SparseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const DenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

    inline bool FillRow(int64 RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendRow(const DenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool AppendRow(const SparseMatrix<ElementType_Input>& RowData);

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
    inline bool InsertRow(int64 RowIndex, const SparseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const DenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline SparseShadowMatrix<ElementType> Diangonal();

    inline SparseMatrix GetDiangonal() const;

    inline bool GetDiangonal(DenseMatrix<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(std::vector<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(ElementType* DiangonalData) const;

    template<typename ElementType_Input>
    inline bool SetDiangonal(const SparseMatrix<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const DenseMatrix<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::vector<ElementType_Input>& DiangonalData);

	template<typename ElementType_Input>
	inline bool SetDiangonal(const ElementType_Input* DiangonalData);

    inline bool FillDiangonal(const ElementType& Element);

	//---------------------- SparseMatrix {+= -= *= /=} SparseMatrix ----------------------------------------//

	inline void operator+=(const SparseMatrix<ElementType>& InputMatrix);

    inline void operator-=(const SparseMatrix<ElementType>& InputMatrix);

    inline void operator*=(const SparseMatrix<ElementType>& InputMatrix);


    inline void operator+=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const SparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const SparseShadowMatrix<ElementType>& ShadowMatrix);


    inline void operator+=(SparseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator-=(SparseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator*=(const SparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);


    inline void operator+=(SparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator-=(SparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator*=(SparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

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

    inline SparseMatrix ElementMultiply(const DenseMatrix<ElementType>& InputDenseMatrix) const;

    inline SparseMatrix ElementMultiply(const DenseShadowMatrix<ElementType>& DenseShadowMatrix) const;

    inline SparseMatrix ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix) const;

    inline SparseMatrix ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix) const;

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

	//----------------------------------- Rank -----------------------------------------//

    inline int64 Rank() const;

	//----------------------------------- inverse -----------------------------------------//

    inline DenseMatrix<ElementType> Inv() const;

    inline DenseMatrix<ElementType> PseudoInv() const;

	//----------------------------------- SVD -----------------------------------------//

    inline SparseMatrixSVDResult<ElementType> SVD() const;


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

#endif
