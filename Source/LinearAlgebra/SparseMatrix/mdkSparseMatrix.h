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
struct mdkSparseMatrixDataInCSCFormat
{
    int64 m_RowNumber;  // RowNumber = the Number of Rows 
    int64 m_ColNumber;  // ColNumber = the Number of Columns

    std::vector<int64> m_RowIndexList;

    std::vector<int64> m_ColIndexList;

    std::vector<ElementType> m_DataArray;

    std::vector<int64> m_ColBeginElementLinearIndexInDataArray; // Index in m_RowIndexList m_ColIndexList m_DataArray

    std::vector<int64> m_RecordedElementNumberInEachCol;

    ElementType m_ZeroElement;

//-------------------------------------------------------------------------------------------------
    inline mdkSparseMatrixDataInCSCFormat();

    inline ~mdkSparseMatrixDataInCSCFormat();

    inline void Construct(int64 InputRowNumber, int64 InputColNumber);

    inline void Construct(const int64* InputRowIndexList,
                          const int64* InputColIndexList,
                          const ElementType* InputDataArray,
                          int64 RecordedElementNumber,
                          int64 InputRowNumber,
                          int64 InputColNumber,
                          int64 AdditionalReservedCapacity = 0);

    //------------------------------------------

    inline void Reset();

    inline void Resize(int64 InputRowNumber, int64 InputColNumber);

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

    inline void DeepCopy(const mdkSparseMatrixDataInCSCFormat& InputData);

private:
//deleted: -------------------------------------------------
    mdkSparseMatrixDataInCSCFormat(const mdkSparseMatrixDataInCSCFormat&) = delete;

    mdkSparseMatrixDataInCSCFormat(mdkSparseMatrixDataInCSCFormat&&) = delete;

    void operator=(const mdkSparseMatrixDataInCSCFormat&) = delete;

    void operator=(mdkSparseMatrixDataInCSCFormat&&) = delete;
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkSparseMatrix : public mdkObject
{

private:
     
    std::shared_ptr<mdkSparseMatrixDataInCSCFormat<ElementType>> m_MatrixData;

    ElementType m_NaNElement;

	bool m_IsSizeFixed;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline mdkSparseMatrix();

    inline mdkSparseMatrix(int64 RowNumber, int64 ColNumber);

    inline mdkSparseMatrix(const ElementType& Element);

    // copy constructor (DeepCopy or SharedCopy)
    inline mdkSparseMatrix(const mdkSparseMatrix<ElementType>& InputSparseMatrix, mdkObjectCopyConstructionTypeEnum Method = mdkObjectCopyConstructionTypeEnum::DeepCopy);

    // move constructor
    inline mdkSparseMatrix(mdkSparseMatrix<ElementType>&& InputSparseMatrix);

    inline mdkSparseMatrix(const mdkSparseShadowMatrix<ElementType>& SparseShadowMatrix);

    inline mdkSparseMatrix(const mdkSparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix);

    inline mdkSparseMatrix(const mdkSparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix);

	inline ~mdkSparseMatrix();

    //------------------------------construction method -----------------------------//

    inline bool Construct(const mdkDenseMatrix<int64>& RowIndexList,
                          const mdkDenseMatrix<int64>& ColIndexList,
                          const mdkDenseMatrix<ElementType>& DataArray,
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
    inline void operator=(const mdkSparseMatrix<ElementType>& InputMatrix);

    // move assignment operator
    inline void operator=(mdkSparseMatrix<ElementType>&& InputMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& list);

    inline void operator=(const std::initializer_list<std::initializer_list<ElementType>>& list);

    inline void operator=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator=(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline void operator=(const mdkSparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    //----------------------  Copy From SparseMatrix or Element  ----------------------------------------//

    // Copy can be used to convert a SparseMatrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool DeepCopy(const mdkSparseMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool DeepCopy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Share, ForceShare  ------------------------------------------ //
 
    // if m_IsSizeFixed is true, and size does not match, then return false
    //
    inline bool SharedCopy(mdkSparseMatrix<ElementType>& InputMatrix);

    // it is used by SparseGlueMatrix
    // Share the object (InputSparseMatrix) no matter what, even if InputSparseMatrix is const
    inline void ForceSharedCopy(const mdkSparseMatrix<ElementType>& InputMatrix);

    //-------------------- Take -----------------------------------------------------------//

    //Take the the ownership of the InputSparseMatrix and ForceClear it

    inline bool Take(mdkSparseMatrix<ElementType>& InputMatrix);

    inline bool Take(mdkSparseMatrix<ElementType>&& InputMatrix);

    //Take the the ownership of the SparseMatrix Created from ShadowSparseMatrix or SparseGlueMatrix

    inline bool Take(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline bool Take(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

    inline bool Take(const mdkSparseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

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

	inline mdkMatrixSize GetSize() const;

    inline int64 GetElementNumber() const;

    inline int64 GetRecordedElementNumber() const;

	inline int64 GetColNumber() const;

	inline int64 GetRowNumber() const;

    //------------------------ NaN Element -----------------------------//

    inline const ElementType& GetNaNElement() const;

    //------------------------ Element Type -----------------------------//

    inline mdkMatrixElementTypeEnum GetElementType() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

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

    inline mdkSparseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList);

    inline const mdkSparseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& LinearIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList);

    inline const mdkSparseShadowMatrix<ElementType> operator()(const std::vector<int64>& LinearIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkSparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    // at(): bound check -----------------

    inline mdkSparseShadowMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList);

    inline const mdkSparseShadowMatrix<ElementType> at(std::initializer_list<int64>& LinearIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> at(const std::vector<int64>& LinearIndexList);

    inline const mdkSparseShadowMatrix<ElementType> at(const std::vector<int64>& LinearIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkSparseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    //---------------------- Get/Set SubSparseMatrix by SparseMatrix({}, {}) or SparseMatrix.at({}, {})  or SparseMatrix.SubSparseMatrix(a, b, c, d) -------//

    // operator(): no bound check in release mode

    inline mdkSparseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                         std::initializer_list<int64>& ColIndexList);

    inline const mdkSparseShadowMatrix<ElementType> operator()(std::initializer_list<int64>& RowIndexList,
                                                               std::initializer_list<int64>& ColIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList,
                                                         const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkSparseShadowMatrix<ElementType> operator()(const std::initializer_list<int64>& RowIndexList,
                                                               const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkSparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                         const std::initializer_list<int64>& ColIndexList);

    inline const mdkSparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                               const std::initializer_list<int64>& ColIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                         const std::vector<int64>& ColIndexList);

    inline const mdkSparseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                               const std::vector<int64>& ColIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                         const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkSparseShadowMatrix<ElementType> operator()(const std::vector<int64>& RowIndexList,
                                                               const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkSparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                         const std::vector<int64>& ColIndexList);

    inline const mdkSparseShadowMatrix<ElementType> operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                               const std::vector<int64>& ColIndexList) const;

    // at(): bound check -----------------

    inline mdkSparseShadowMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                                 std::initializer_list<int64>& ColIndexList);

    inline const mdkSparseShadowMatrix<ElementType> at(std::initializer_list<int64>& RowIndexList,
                                                       std::initializer_list<int64>& ColIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                                 const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkSparseShadowMatrix<ElementType> at(const std::initializer_list<int64>& RowIndexList,
                                                       const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkSparseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                 const std::initializer_list<int64>& ColIndexList);

    inline const mdkSparseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                       const std::initializer_list<int64>& ColIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                 const std::vector<int64>& ColIndexList);

    inline const mdkSparseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                       const std::vector<int64>& ColIndexList) const;

    inline mdkSparseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                 const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol);

    inline const mdkSparseShadowMatrix<ElementType> at(const std::vector<int64>& RowIndexList,
                                                       const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkSparseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                 const std::vector<int64>& ColIndexList);

    inline const mdkSparseShadowMatrix<ElementType> at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                                       const std::vector<int64>& ColIndexList) const;

    // return SubSparseMatrix as SparseMatrix -----------------------------------------------

    inline mdkSparseMatrix GetSubMatrix(const std::vector<int64>& RowIndexList, 
                                        const std::vector<int64>& ColIndexList) const;

    inline mdkSparseMatrix GetSubMatrix(const std::vector<int64>& RowIndexList, 
                                        const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const;

    inline mdkSparseMatrix GetSubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, 
                                        const std::vector<int64>& ColIndexList) const;

    inline bool GetSubMatrix(mdkSparseMatrix<ElementType> &OutputSparseMatrix, 
                             const std::vector<int64>& RowIndexList, 
                             const std::vector<int64>& ColIndexList) const;

	//---------------------- Get/Set/Fill/Append/Delete/InsertCol Column ----------------------------------------//
	
    inline mdkSparseShadowMatrix<ElementType> Col(int64 ColIndex);

    inline mdkSparseShadowMatrix<ElementType> Col(std::initializer_list<int64>& ColIndexList);

    inline mdkSparseShadowMatrix<ElementType> Col(const std::vector<int64>& ColIndexList);

    inline mdkSparseMatrix GetCol(int64 ColIndex) const;

    inline bool GetCol(int64 ColIndex, mdkDenseMatrix<ElementType>& ColData) const;

    inline bool GetCol(int64 ColIndex, std::vector<ElementType>& ColData) const;

    inline bool GetCol(int64 ColIndex, ElementType* ColData) const;

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const mdkSparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const mdkDenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool SetCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);
	
    inline bool FillCol(int64 RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendCol(const mdkSparseMatrix<ElementType_Input>& ColData);

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
    inline bool InsertCol(int64 ColIndex, const mdkSparseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const mdkDenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length);

	//---------------------- Get/Set/Fill/Append/Delete Row  ----------------------------------------//
	
    inline mdkSparseShadowMatrix<ElementType> Row(int64 RowIndex);

    inline mdkSparseShadowMatrix<ElementType> Row(std::initializer_list<int64>& RowIndexList);

    inline mdkSparseShadowMatrix<ElementType> Row(const std::vector<int64>& RowIndexList);

    inline mdkSparseMatrix GetRow(int64 RowIndex) const;

    inline bool GetRow(int64 RowIndex, mdkDenseMatrix<ElementType>& RowData) const;

    inline bool GetRow(int64 RowIndex, std::vector<ElementType>& RowData) const;

    inline bool GetRow(int64 RowIndex, ElementType* RowData) const;

    template<typename ElementType_Input>
    inline bool SetRow(int64 RowIndex, const mdkSparseMatrix<ElementType_Input>& RowData);

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
    inline bool InsertRow(int64 RowIndex, const mdkDenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

    inline mdkSparseShadowMatrix<ElementType> Diangonal();

    inline mdkSparseMatrix GetDiangonal() const;

    inline bool GetDiangonal(mdkDenseMatrix<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(std::vector<ElementType>& DiangonalData) const;

    inline bool GetDiangonal(ElementType* DiangonalData) const;

    template<typename ElementType_Input>
    inline bool SetDiangonal(const mdkSparseMatrix<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const mdkDenseMatrix<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData);

    template<typename ElementType_Input>
    inline bool SetDiangonal(const std::vector<ElementType_Input>& DiangonalData);

	template<typename ElementType_Input>
	inline bool SetDiangonal(const ElementType_Input* DiangonalData);

    inline bool FillDiangonal(const ElementType& Element);

	//---------------------- SparseMatrix {+= -= *= /=} SparseMatrix ----------------------------------------//

	inline void operator+=(const mdkSparseMatrix<ElementType>& InputMatrix);

    inline void operator-=(const mdkSparseMatrix<ElementType>& InputMatrix);

    inline void operator*=(const mdkSparseMatrix<ElementType>& InputMatrix);


    inline void operator+=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator-=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);

    inline void operator*=(const mdkSparseShadowMatrix<ElementType>& ShadowMatrix);


    inline void operator+=(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator-=(mdkSparseGlueMatrixForLinearCombination<ElementType> GlueMatrix);

    inline void operator*=(const mdkSparseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);


    inline void operator+=(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator-=(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    inline void operator*=(mdkSparseGlueMatrixForMultiplication<ElementType> GlueMatrix);

    //---------------------- SparseMatrix {+= -= *= /=} Element ----------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //-------------------- special element operation {^} -----------------------------------------------------------//

    inline mdkSparseMatrix operator^(const ElementType& Element);

    inline void operator^=(const ElementType& Element);

    //-------------------- special element operation {.*} element multiply -----------------------------------------------------------//

    inline mdkSparseMatrix ElementMultiply(const ElementType& Element) const;

    inline mdkSparseMatrix ElementMultiply(const mdkSparseMatrix<ElementType>& InputSparseMatrix) const;

    inline mdkSparseMatrix ElementMultiply(const mdkSparseShadowMatrix<ElementType>& SparseShadowMatrix) const;

    inline mdkSparseMatrix ElementMultiply(const mdkSparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix) const;

    inline mdkSparseMatrix ElementMultiply(const mdkSparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix) const;

    inline mdkSparseMatrix ElementMultiply(const mdkDenseMatrix<ElementType>& InputDenseMatrix) const;

    inline mdkSparseMatrix ElementMultiply(const mdkDenseShadowMatrix<ElementType>& DenseShadowMatrix) const;

    inline mdkSparseMatrix ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix) const;

    inline mdkSparseMatrix ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix) const;

    //-------------------- general element operation : output a new matrix ------------------------------------------//

    inline mdkSparseMatrix ElementOperation(const char* OperationName) const;

    inline mdkSparseMatrix ElementOperation(const std::string& OperationName) const;

    inline mdkSparseMatrix ElementOperation(std::function<ElementType(const ElementType&)> Operation) const;

    inline mdkSparseMatrix ElementOperation(const std::string& OperationName, const mdkSparseMatrix<ElementType>& InputMatrix) const;

    inline mdkSparseMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                            const mdkSparseMatrix<ElementType>& InputMatrix) const;

    inline mdkSparseMatrix ElementOperation(const char* OperationName, const ElementType& Element) const;

    inline mdkSparseMatrix ElementOperation(const std::string& OperationName, const ElementType& Element) const;

    inline mdkSparseMatrix ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element) const;

    //-------------------- general element operation in place : Object.ElementOperationInPlace modify the object ------------------//

    inline bool ElementOperationInPlace(const char* OperationName);

    inline bool ElementOperationInPlace(const std::string& OperationName);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&)> Operation);

    inline bool ElementOperationInPlace(const char* OperationName, const mdkSparseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const std::string& OperationName, const mdkSparseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                        const mdkSparseMatrix<ElementType>& InputMatrix);

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

    inline mdkDenseMatrix<ElementType> Inv() const;

    inline mdkDenseMatrix<ElementType> PseudoInv() const;

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
