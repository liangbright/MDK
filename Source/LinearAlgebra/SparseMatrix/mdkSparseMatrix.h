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
// mdkSparseMatrix is the same as Matlab SparseMatrix except index staring from 0 instead of 1 in Matlab
//

//forward-declare ----------------//
template<typename ElementType>
class mdkMatrix;

template<typename ElementType>
class mdkSparseMatrix;

template<typename ElementType>
class mdkSparseShadowMatrix;

template<typename ElementType>
class mdkSparseGlueMatrixForLinearCombination;

template<typename ElementType>
class mdkSparseGlueMatrixForMultiplication;

// end of  forward-declare  ------//

// -----------------------//

#define MDK_SparseMatrix_AdditionalReservedCapacity 1000; 

// -----------------------//

// ----------------------------- mdkSparseMatrixData struct -------------------------------------------------------------//

template<typename ElementType>
struct mdkSparseMatrixData
{
    int64 m_RowNumber;  // RowNumber = the Number of Rows 
    int64 m_ColNumber;  // ColNumber = the Number of Columns

    std::vector<int64> m_RowIndexList;

    std::vector<int64> m_ColBeginElementRelativeIndexList; // Index in m_RowIndexList and m_DataArray

    std::vector<int64> m_NonZeroElementNumberInEachCol;

    std::vector<ElementType> m_DataArray;
//-------------------------------------------------------------
    inline mdkSparseMatrixData();

    inline mdkSparseMatrixData(int64 RowNumber, int64 ColNumber);

    inline void Construct(const int64* RowIndexList,
                          const int64* ColIndexList,
                          const ElementType* DataArray,
                          int64 NonZeroElementNumber,
                          int64 RowNumber,
                          int64 ColNumber,
                          int64 AdditionalReservedCapacity = MDK_SparseMatrix_AdditionalReservedCapacity);

    inline mdkSparseMatrixData(const mdkMatrix<int64>& RowIndexList,
                               const mdkMatrix<int64>& ColIndexList,
                               const mdkMatrix<ElementType>& DataArray,
                               int64 RowNumber,
                               int64 ColNumber,
                               int64 AdditionalReservedCapacity = MDK_SparseMatrix_AdditionalReservedCapacity);

    inline mdkSparseMatrixData(const std::vector<int64>& RowIndexList,
                               const std::vector<int64>& ColIndexList,
                               const std::vector<ElementType>& DataArray,
                               int64 RowNumber,
                               int64 ColNumber,
                               int64 AdditionalReservedCapacity = MDK_SparseMatrix_AdditionalReservedCapacity);

    inline ~mdkSparseMatrixData();

    inline ElementType& operator[](int64 LinearIndex);

    inline const ElementType& operator[](int64 LinearIndex) const;

    inline ElementType& operator()(int64 LinearIndex);

    inline const ElementType& operator()(int64 LinearIndex) const;

    inline ElementType& operator()(int64 RowIndex, int64 ColIndex);

    inline const ElementType& operator()(int64 RowIndex, int64 ColIndex) const;

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
     
    std::shared_ptr<mdkSparseMatrixData<ElementType>> m_MatrixData;

    ElementType m_NaNElement;

	bool m_IsSizeFixed;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline mdkSparseMatrix();

    inline mdkSparseMatrix(int64 RowNumber, int64 ColNumber);

    inline mdkSparseMatrix(const mdkMatrix<int64>& RowIndexList,
                           const mdkMatrix<int64>& ColIndexList,
                           const mdkMatrix<ElementType>& DataArray,
                           int64 RowNumber,
                           int64 ColNumber,
                           int64 AdditionalReservedCapacity = MDK_SparseMatrix_AdditionalReservedCapacity);

    inline mdkSparseMatrix(const std::vector<int64>& RowIndexList,
                           const std::vector<int64>& ColIndexList,
                           const std::vector<ElementType>& DataArray,
                           int64 RowNumber,
                           int64 ColNumber,
                           int64 AdditionalReservedCapacity = MDK_SparseMatrix_AdditionalReservedCapacity);

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


    //------------------------------construction method -----------------------------//

    inline bool Construct(const mdkMatrix<int64>& RowIndexList,
                          const mdkMatrix<int64>& ColIndexList,
                          const mdkMatrix<ElementType>& DataArray,
                          int64 RowNumber,
                          int64 ColNumber,
                          int64 AdditionalReservedCapacity = MDK_SparseMatrix_AdditionalReservedCapacity);

    inline bool Construct(const std::vector<int64>& RowIndexList,
                          const std::vector<int64>& ColIndexList,
                          const std::vector<ElementType>& DataArray,
                          int64 RowNumber,
                          int64 ColNumber,
                          int64 AdditionalReservedCapacity = MDK_SparseMatrix_AdditionalReservedCapacity);

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
    inline bool Copy(const mdkSparseMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Share, ForceShare  ------------------------------------------ //
 
    // if m_IsSizeFixed is true, and size does not match, then return false
    //
    inline bool Share(mdkSparseMatrix<ElementType>& InputMatrix);

    // it is used by SparseGlueMatrix
    // Share the object (InputSparseMatrix) no matter what, even if InputSparseMatrix is const
    inline void ForceShare(const mdkSparseMatrix<ElementType>& InputMatrix);

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

    inline int64 GetNonZeroElementNumber() const;

	inline int64 GetColNumber() const;

	inline int64 GetRowNumber() const;

    //------------------------ NaN Element -----------------------------//

    inline const ElementType& GetNaNElement() const;

    //------------------------ Element Type -----------------------------//

    inline mdkSparseMatrixElementTypeEnum GetElementType() const;

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

    //-------------------- special element operation {^} -----------------------------------------------------------//

    inline mdkSparseMatrix operator^(const ElementType& Element);

    inline void operator^=(const ElementType& Element);

    //-------------------- special element operation {.*} element multiply -----------------------------------------------------------//

    inline mdkSparseMatrix ElementMultiply(const ElementType& Element);

    inline mdkSparseMatrix ElementMultiply(const mdkSparseMatrix<ElementType>& InputSparseMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkSparseShadowMatrix<ElementType>& SparseShadowMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkSparseGlueMatrixForLinearCombination<ElementType>& SparseGlueMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkSparseGlueMatrixForMultiplication<ElementType>& SparseGlueMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkDenseMatrix<ElementType>& InputDenseMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkDenseShadowMatrix<ElementType>& DenseShadowMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& DenseGlueMatrix);

    inline mdkSparseMatrix ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& DenseGlueMatrix);

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

    inline mdkMatrix Inv() const;

    inline mdkMatrix PseudoInv() const;

	//----------------------------------- SVD -----------------------------------------//

    inline mdkMatrixSVDResult<ElementType> SVD() const;


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
