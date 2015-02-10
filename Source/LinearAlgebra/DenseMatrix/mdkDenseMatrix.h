#ifndef mdk_DenseMatrix_h
#define mdk_DenseMatrix_h

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include <cmath>
#include <algorithm>
#include <limits> 

#include "mdkString.h"
#include "mdkLinearAlgebraConfig.h"
#include "mdkMatrix_Common.h"
#include "mdkStdObjectVector.h"
#include "mdkDenseVector.h"
#include "mdkDenseShadowMatrix.h"
#include "mdkDenseGlueMatrixForLinearCombination.h"
#include "mdkDenseGlueMatrixForMultiplication.h"
#include "mdkDenseMatrixOperator.h"
#include "mdkLinearAlgebra_Function_DenseMatrix.h"
#include "mdkLinearAlgebra_Function_Common.h"
#include "mdkDisplayMatrix.h"

namespace mdk
{

// Dense Matrix Template Class, each entry/element is a scalar
// column major
//
// DenseMatrix is similar to SharedDataObject
//
// Compare to Matlab:
// DenseMatrix API very similar to Matlab
//
// Compare to Armadillo  (a linear algebra library, and it uses column major matrix)
// DenseMatrix API better than Armadillo
//

// ----------------------------- DenseMatrixData struct -------------------------------------------------------------//

template<typename Element_Type>
struct DenseMatrixData
{
	typedef Element_Type  ElementType;

	bool IsSizeFixed;
    int_max RowNumber;  // RowNumber = the Number of Rows 
    int_max ColNumber;  // ColNumber = the Number of Columns

	ElementType* ElementPointer; // point to InternalArray/StdVector or external array 

	std::vector<ElementType> StdVector; // InternalArray

    ElementType ErrorElement;
//-------------------------------------------------------------
    DenseMatrixData() 
    {
		IsSizeFixed = false;
        RowNumber = 0;
        ColNumber = 0;
        ElementPointer = nullptr;
		ErrorElement = GetNaNElement(ErrorElement); // zero if int        
    };

    ~DenseMatrixData() {};

	void Clear()
	{
		IsSizeFixed = false;
		RowNumber = 0;
		ColNumber = 0;
		ElementPointer = nullptr;
		StdVector.clear();         // change size
		StdVector.shrink_to_fit(); // release memory
		ErrorElement = GetNaNElement(ErrorElement); // zero if int     
	}

    void CopyDataToInternalArrayIfNecessary()
    {
		if (ElementPointer != StdVector.data())
        {
            if (ElementPointer == nullptr)
            {
				if (RowNumber != 0 || ColNumber != 0 || StdVector.data() != nullptr || StdVector.size() != 0)
                {
                    MDK_Error("ElementPointer is nullptr but Self is not empty matrix @ DenseMatrixData::CopyDataToInternalArrayIfNecessary()")
                }
                return;
            }

            auto ElementNumber = RowNumber*ColNumber;

			StdVector.resize(ElementNumber);

            for (int_max i = 0; i < ElementNumber; ++i)
            {
				StdVector[i] = ElementPointer[i];
            }

			ElementPointer = StdVector.data();
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

template<typename Element_Type>
class DenseMatrix : public Object
{
public:
	typedef Element_Type  ElementType;

private:     
    std::shared_ptr<DenseMatrixData<ElementType>> m_MatrixData;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline DenseMatrix();

    inline DenseMatrix(const MDK_Symbol_PureEmpty&);

    inline DenseMatrix(const MDK_Symbol_Empty&);

    inline DenseMatrix(int_max RowNumber, int_max ColNumber);

	inline DenseMatrix(MatrixSize Size);

    inline DenseMatrix(const ElementType& Element);

    inline DenseMatrix(const std::initializer_list<ElementType>& InputList);

    inline DenseMatrix(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList);

    inline DenseMatrix(const std::initializer_list<const DenseMatrix<ElementType>*>& InputList);

    inline DenseMatrix(const std::initializer_list<std::initializer_list<const DenseMatrix<ElementType>*>>& InputListInList);

	inline DenseMatrix(std::vector<ElementType> InputColVector);

	inline DenseMatrix(StdObjectVector<ElementType> InputColVector);

	inline DenseMatrix(const ObjectArray<ElementType>& InputColVector);

	template<int_max VectorFixedLength>
	inline DenseMatrix(const DenseVector<ElementType, VectorFixedLength>& InputColVector);

	inline DenseMatrix(DenseVector<ElementType> InputColVector);

    // copy constructor
    inline DenseMatrix(const DenseMatrix<ElementType>& InputMatrix);

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

    // move assignment operator (this->m_MatrixData = std::move(m_MatrixData of InputMatrix))
    inline void operator=(DenseMatrix<ElementType>&& InputMatrix);

	inline void operator=(const DenseShadowMatrix<ElementType>& ShadowMatrix);

	inline void operator=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

	inline void operator=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    inline void operator=(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList);

    inline void operator=(const std::initializer_list<const DenseMatrix<ElementType>*>& InputList);

    inline void operator=(const std::initializer_list<std::initializer_list<const DenseMatrix<ElementType>*>>& InputListInList);

	inline void operator=(const std::vector<ElementType>& InputColVector);
	inline void operator=(std::vector<ElementType>&& InputColVector);

	inline void operator=(const StdObjectVector<ElementType>& InputColVector);
	inline void operator=(StdObjectVector<ElementType>&& InputColVector);

	inline void operator=(const ObjectArray<ElementType>& InputColVector);

	template<int_max TemplateLength>
	inline void operator=(const DenseVector<ElementType, TemplateLength>& InputColVector);

	inline void operator=(DenseVector<ElementType>&& InputColVector);

    //-----------------------------------------  Copy ---------------------------------------------//

    // Copy can be used to convert a matrix from double (ElementType_Input) to float (ElementType), etc

    template<typename ElementType_Input>  
    inline bool Copy(const DenseMatrix<ElementType_Input>& InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const DenseMatrix<ElementType_Input>* InputMatrix);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int_max InputRowNumber, int_max InputColNumber);

	inline bool Copy(DenseMatrix<ElementType>&& InputMatrix);

	//Take the Matrix Created from ShadowMatrix or GlueMatrix

	inline bool Copy(const DenseShadowMatrix<ElementType>& ShadowMatrix);

	inline bool Copy(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix);

	inline bool Copy(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix);

	inline bool Copy(const std::vector<ElementType>& InputColVector);
	inline bool Copy(std::vector<ElementType>&& InputColVector);

	inline bool Copy(const StdObjectVector<ElementType>& InputDataArray);
	inline bool Copy(StdObjectVector<ElementType>&& InputDataArray);

	inline bool Copy(const ObjectArray<ElementType>& InputDataArray);

	template<int_max TemplateLength>
	inline bool Copy(const DenseVector<ElementType, TemplateLength>& InputColVector);

	inline bool Copy(DenseVector<ElementType>&& InputColVector);

	inline bool Copy(const MDK_Symbol_Empty&);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

	// DenseMatrix A, B; A.Share(B), A.ForceShare(B) <=>  m_MatrixData (of A) = m_MatrixData (of B);
    // If B change the value of an element, A will be changed (i.e., the data in A is the same as the data in B)
    // If B change its size or clear itself, A will be changed.
    // If A change it self, B will be changed
    //
	// DenseMatrix A, B, C; 
	// A.Share(B); B.Share(C); Now, A and C are NOT shared by each other.
    // A.Share(B) really means A and B share the same data
    // After B.Share(C), then B is C, and the share-relation between A and B is terminated, A keep its data.
	// 
	// 4 way to terminate the relation A.Share(B) or A.ForceShare(B)
	// (1) DenseMatrix C(std::move(A));  or DenseMatrix C = std::move(A); 
	// (2) DenseMatrix C; A.Swap(C);
	// (3) DenseMatrix C; A.ForceShare(C) or A.Share(C);
	// (4) A.Recreate();
	//
    // There are 3 situation we may use A.Share(B) or A.ForceShare(B)
    // (1) use A as an observer of B, and do not modify B by using A, e.g., A(0,0)=1;
    // (2) forget B and use A,  e.g., A(0,0)=1;
    // (2) use both A and B to operate on the same data; in this case, using a pointer to B is better if B is a named object
    //
    // Share vs ForceShare
    // A.Share(B): if m_IsSizeFixed of A is true, and size of A does not match size of B, then return false (B is not shared by A)
    // A.ForceShare(B): Share B no matter what, it is used in GlueMatrix operation

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
    //        there is no such thing as std::vector<const DenseMatrix<ElementType>&>
    //
    //        you can create std::vector<const DenseMatrix<ElementType>*> MatrixPtrListA = {&B1, &B2, ...};
    //        and const prevent changing B, e.g., (*MatrixPtrListA[0])(0,0) = 10; can not be compiled
    //
    //        However,std::vector<const DenseMatrix<ElementType>> is equal to std::vector<DenseMatrix<ElementType>>
    //
    //        std::vector<const mdkDenseMatrix<ElementType>> MatrixListA;        
    //        MatrixListA.emplace_back(B1, ObjectCopyConstructionTypeEnum::Share);
    //        But:
    //            MatrixListA[0](1,1) = 100; CAN be compiled !!!  (B1 is changed by this code)
    //
    //-----------------------------------------------------------------------------------------------------
    // conclusion: 
    // (1) An array of Share objects can be created from InputMatrix
    //     std::vector<DenseMatrix<ElementType>> SharedMatrixArray(10);
    //     SharedMatrixArray[i].Share(InputMatrix);
    //     SharedMatrixArray[i].ForceShare(InputMatrix);
    //
    // (2) An array of const Share objects can be created by using std::vector, but const is lost
    //
    // (3) An array of const pointers to objects can be created from InputMatrix
    //     std::vector<const DenseMatrix<ElementType>*> SharedMatrixPointerArray(10);
    //     SharedMatrixPointerArray[i] = &InputMatrix;
    //------------------------------------------------------------------------------------------------------

    //---------------------- Special Share: share the Matrix of another library (e.g., eigen) ----------------------------------------//
    // It can be used to share a Matrix in eigen library
    // It can be used to share a col of a MDK Matrix (RefCol(...) is better)
    // do not use this Share() to share a MDK Matrix

    inline bool Share(ElementType* InputElementPointer, int_max InputRowNumber, int_max InputColNumber, bool IsSizeFixed = true);

    inline bool ForceShare(const ElementType* InputElementPointer, int_max InputRowNumber, int_max InputColNumber, bool IsSizeFixed = true);

    //------------------------- Swap shared_ptr m_MatrixData -------------------------------------------//
    // m_MatrixData.swap(InputMatrix.m_MatrixData)
	// faster than A.Take(B)
	// side effect : invalidate Share()
	// if A.Share(B); A.Swap(C); then A will no longer share B, but C will share B; A become C, C become A; 
	// if A.Share(B); A.Take(C); then A still share B, and only its content is changed; C will not share B
    inline void Swap(DenseMatrix<ElementType>& InputMatrix);

	//--------------------------- Recreate ---------------------------------------//
	inline void Recreate();

    //------------------------- Clear -------------------------------------------//
    // clear memory, not equal to Resize(0, 0) which may not release memory
	// if MatrixData exist, it will not be deleted (i.e., m_MatrixData.reset() is not used)
    inline void Clear();

	//---------------------- Set/get Matrix Size, Shape ----------------------------------------//

    inline bool Reshape(int_max InputRowNumber, int_max InputColNumber);

	inline bool Reshape(MatrixSize InputSize);

    inline bool Resize(int_max InputRowNumber, int_max InputColNumber); // try to keep the old data

	inline bool Resize(MatrixSize InputSize);

    inline bool FastResize(int_max InputRowNumber, int_max InputColNumber); // do not care about old data

	inline bool FastResize(MatrixSize InputSize);

    inline bool Resize(int_max InputElementNumber); // if matrix is vector, try to keep the old data, can not use it to resize a m x n matrix (m>1 or n>1)

    inline bool FastResize(int_max InputElementNumber); // if matrix is vector, do not care about old data, can not use it to resize a m x n matrix (m>1 or n>1)

    inline bool ReserveCapacity(int_max InputRowNumber, int_max InputColNumber); // reserve memory, current matrix size does not change

	inline bool ReserveCapacity(MatrixSize InputSize);

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current matrix size does not change

    inline void ReleaseUnusedCapacity();

    inline void FixSize();
    
    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

    inline bool IsPureEmpty() const;

    inline bool IsShared() const;

    inline bool IsSharedWith(const DenseMatrix& InputMatrix) const;

	inline bool IsDataInInternalArray() const;

	inline MatrixSize GetSize() const;

    inline int_max GetElementNumber() const;

	inline int_max GetColNumber() const;

	inline int_max GetRowNumber() const;

	inline bool IsScalar() const; // 1 by 1 matrix

    inline bool IsVector() const;

    inline bool IsRowVector() const;

    inline bool IsColVector() const;

    inline bool IsSquare() const;

    inline bool IsIdentityMatrix(ElementType Threshold = ElementType(0)) const;

    //------------------------ ErrorElement -----------------------------//

	inline void SetErrorElement(const ElementType& Element);

	inline ElementType GetErrorElement() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer(); //  the position of the first element
    inline const ElementType* GetElementPointer() const;

    inline ElementType* GetPointer(); //  the position of the first element
    inline const ElementType* GetPointer() const;

    inline ElementType* begin(); // the position of the first element
    inline const ElementType* begin() const;

    inline ElementType* end(); // the last position + 1
    inline const ElementType* end() const; // the last position + 1

    inline ElementType* GetElementPointerOfCol(int_max ColIndex); //  the position of the first element in Col # ColIndex
    inline const ElementType* GetElementPointerOfCol(int_max ColIndex) const;

    inline ElementType* GetPointerOfCol(int_max ColIndex); //  the position of the first element in Col # ColIndex
    inline const ElementType* GetPointerOfCol(int_max ColIndex) const;

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

	inline DenseShadowMatrix<ElementType> operator()(DenseVector<int_max> LinearIndexList);
	inline const DenseShadowMatrix<ElementType> operator()(DenseVector<int_max> LinearIndexList) const;

	template<int_max VectorFixedLength>
	inline DenseShadowMatrix<ElementType> operator()(const DenseVector<int_max, VectorFixedLength>& LinearIndexList);
	template<int_max VectorFixedLength>
	inline const DenseShadowMatrix<ElementType> operator()(const DenseVector<int_max, VectorFixedLength>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& LinearIndexList);
    inline const DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol);
    inline const DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol) const;

    // at(): bound check -----------------

    inline DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& LinearIndexList);
    inline const DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& LinearIndexList) const;

	inline DenseShadowMatrix<ElementType> at(DenseVector<int_max> LinearIndexList);
	inline const DenseShadowMatrix<ElementType> at(DenseVector<int_max> LinearIndexList) const;

	template<int_max VectorFixedLength>
	inline DenseShadowMatrix<ElementType> at(const DenseVector<int_max, VectorFixedLength>& LinearIndexList);
	template<int_max VectorFixedLength>
	inline const DenseShadowMatrix<ElementType> at(const DenseVector<int_max, VectorFixedLength>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& LinearIndexList);
    inline const DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& LinearIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol); // convert matrix to col-vector
    inline const DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol) const;

    //---------- Get/Set SubMatrix by Matrix({...},{...}), Matrix.at({...}, {...}), Matrix.Col({...}), Matrix.Row({...}), Matrix.Diagnal() -------//

    // operator(): no bound check in release mode

    inline DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList,
                                                     const std::initializer_list<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList,
                                                           const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList, 
                                                     const MDK_Symbol_ALL& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> operator()(const std::initializer_list<int_max>& RowIndexList, 
                                                           const MDK_Symbol_ALL& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol,
                                                     const std::initializer_list<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol,
                                                           const std::initializer_list<int_max>& ColIndexList) const;

    //-----------------------------------

	inline DenseShadowMatrix<ElementType> operator()(DenseVector<int_max> RowIndexList,
                                                     DenseVector<int_max> ColIndexList);

	inline const DenseShadowMatrix<ElementType> operator()(DenseVector<int_max> RowIndexList,
                                                           DenseVector<int_max> ColIndexList) const;

	inline DenseShadowMatrix<ElementType> operator()(DenseVector<int_max> RowIndexList,
                                                     const MDK_Symbol_ALL& ALL_Symbol);

	inline const DenseShadowMatrix<ElementType> operator()(DenseVector<int_max> RowIndexList,
                                                           const MDK_Symbol_ALL& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol,
                                                     DenseVector<int_max> ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol,
                                                           DenseVector<int_max> ColIndexList) const;

    //-----------------------------------
	template<int_max VectorFixedLengthA, int_max VectorFixedLengthB>
	inline DenseShadowMatrix<ElementType> operator()(const DenseVector<int_max, VectorFixedLengthA>& RowIndexList,
                                                     const DenseVector<int_max, VectorFixedLengthB>& ColIndexList);
	template<int_max VectorFixedLengthA, int_max VectorFixedLengthB>
	inline const DenseShadowMatrix<ElementType> operator()(const DenseVector<int_max, VectorFixedLengthA>& RowIndexList,
                                                           const DenseVector<int_max, VectorFixedLengthB>& ColIndexList) const;

	template<int_max VectorFixedLength>
	inline DenseShadowMatrix<ElementType> operator()(const DenseVector<int_max, VectorFixedLength>& RowIndexList,
                                                     const MDK_Symbol_ALL& ALL_Symbol);
	template<int_max VectorFixedLength>
	inline const DenseShadowMatrix<ElementType> operator()(const DenseVector<int_max, VectorFixedLength>& RowIndexList,
                                                           const MDK_Symbol_ALL& ALL_Symbol) const;

	template<int_max VectorFixedLength>
    inline DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol,
                                                     const DenseVector<int_max, VectorFixedLength>& ColIndexList);
	template<int_max VectorFixedLength>
    inline const DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol,
                                                           const DenseVector<int_max, VectorFixedLength>& ColIndexList) const;
    //-----------------------------------

    inline DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                     const DenseMatrix<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                           const DenseMatrix<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                     const MDK_Symbol_ALL& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> operator()(const DenseMatrix<int_max>& RowIndexList,
                                                           const MDK_Symbol_ALL& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol,
                                                     const DenseMatrix<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> operator()(const MDK_Symbol_ALL& ALL_Symbol,
                                                           const DenseMatrix<int_max>& ColIndexList) const;

    // at(): bound check -----------------

    inline DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                             const std::initializer_list<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                                   const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                             const MDK_Symbol_ALL& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> at(const std::initializer_list<int_max>& RowIndexList,
                                                   const MDK_Symbol_ALL& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol,
                                             const std::initializer_list<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol,
                                                   const std::initializer_list<int_max>& ColIndexList) const;

    //----------------------------------

    inline DenseShadowMatrix<ElementType> at(DenseVector<int_max> RowIndexList,
                                             DenseVector<int_max> ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(DenseVector<int_max> RowIndexList,
                                                   DenseVector<int_max> ColIndexList) const;

    inline DenseShadowMatrix<ElementType> at(DenseVector<int_max> RowIndexList,
                                             const MDK_Symbol_ALL& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> at(DenseVector<int_max> RowIndexList,
                                                   const MDK_Symbol_ALL& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol,
                                             DenseVector<int_max> ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol,
                                                   DenseVector<int_max> ColIndexList) const;
    //----------------------------------
	template<int_max VectorFixedLengthA, int_max VectorFixedLengthB>
	inline DenseShadowMatrix<ElementType> at(const DenseVector<int_max, VectorFixedLengthA>& RowIndexList,
                                             const DenseVector<int_max, VectorFixedLengthB>& ColIndexList);
	template<int_max VectorFixedLengthA, int_max VectorFixedLengthB>
	inline const DenseShadowMatrix<ElementType> at(const DenseVector<int_max, VectorFixedLengthA>& RowIndexList,
                                                   const DenseVector<int_max, VectorFixedLengthB>& ColIndexList) const;

	template<int_max VectorFixedLength>
	inline DenseShadowMatrix<ElementType> at(const DenseVector<int_max, VectorFixedLength>& RowIndexList,
                                             const MDK_Symbol_ALL& ALL_Symbol);
	template<int_max VectorFixedLength>
	inline const DenseShadowMatrix<ElementType> at(const DenseVector<int_max, VectorFixedLength>& RowIndexList,
                                                   const MDK_Symbol_ALL& ALL_Symbol) const;

	template<int_max VectorFixedLength>
    inline DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol,
                                             const DenseVector<int_max, VectorFixedLength>& ColIndexList);
	template<int_max VectorFixedLength>
    inline const DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol,
                                                   const DenseVector<int_max, VectorFixedLength>& ColIndexList) const;

    //----------------------------------

    inline DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                             const DenseMatrix<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                                   const DenseMatrix<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                             const MDK_Symbol_ALL& ALL_Symbol);

    inline const DenseShadowMatrix<ElementType> at(const DenseMatrix<int_max>& RowIndexList,
                                                   const MDK_Symbol_ALL& ALL_Symbol) const;

    inline DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol,
                                             const DenseMatrix<int_max>& ColIndexList);

    inline const DenseShadowMatrix<ElementType> at(const MDK_Symbol_ALL& ALL_Symbol,
                                                   const DenseMatrix<int_max>& ColIndexList) const;

    // Col(...) is just operator()(ALL, ...)

    inline DenseShadowMatrix<ElementType> Col(int_max ColIndex);
    inline const DenseShadowMatrix<ElementType> Col(int_max ColIndex) const;

    inline DenseShadowMatrix<ElementType> Col(const std::initializer_list<int_max>& ColIndexList);
    inline const DenseShadowMatrix<ElementType> Col(const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> Col(DenseVector<int_max> ColIndexList);
    inline const DenseShadowMatrix<ElementType> Col(DenseVector<int_max> ColIndexList) const;

	template<int_max VectorFixedLength>
	inline DenseShadowMatrix<ElementType> Col(const DenseVector<int_max, VectorFixedLength>& ColIndexList);
	template<int_max VectorFixedLength>
	inline const DenseShadowMatrix<ElementType> Col(const DenseVector<int_max, VectorFixedLength>& ColIndexList) const;

    inline DenseShadowMatrix<ElementType> Col(const DenseMatrix<int_max>& ColIndexList);
    inline const DenseShadowMatrix<ElementType> Col(const DenseMatrix<int_max>& ColIndexList) const;

    // Row(...) is just operator()(..., ALL)

    inline DenseShadowMatrix<ElementType> Row(int_max RowIndex);
    inline const DenseShadowMatrix<ElementType> Row(int_max RowIndex) const;

    inline DenseShadowMatrix<ElementType> Row(const std::initializer_list<int_max>& RowIndexList);
    inline const DenseShadowMatrix<ElementType> Row(const std::initializer_list<int_max>& RowIndexList) const;

    inline DenseShadowMatrix<ElementType> Row(DenseVector<int_max> RowIndexList);
    inline const DenseShadowMatrix<ElementType> Row(DenseVector<int_max> RowIndexList) const;

	template<int_max VectorFixedLength>
	inline DenseShadowMatrix<ElementType> Row(const DenseVector<int_max, VectorFixedLength>& RowIndexList);
	template<int_max VectorFixedLength>
	inline const DenseShadowMatrix<ElementType> Row(const DenseVector<int_max, VectorFixedLength>& RowIndexList) const;

    inline DenseShadowMatrix<ElementType> Row(const DenseMatrix<int_max>& RowIndexList);
    inline const DenseShadowMatrix<ElementType> Row(const DenseMatrix<int_max>& RowIndexList) const;

    //
    inline DenseShadowMatrix<ElementType> Diagonal();
    inline const DenseShadowMatrix<ElementType> Diagonal() const;

    // -------------------------- special col reference ---------------------------------------------
    // faster than Col(int_max ColIndex)
    // side effect: A.RefCol(k) = constant => error (fixed size Matrix = constant ), but A.Col(k) = constant is good
    // use A.RefCol(k).Fill(constant);

    inline DenseMatrix RefCol(int_max ColIndex);
    inline const DenseMatrix RefCol(int_max ColIndex) const;

    // ----------------------- Get SubMatrix as DenseMatrix --------------------------------------------

    // GetSubMatrix(LinearInedxList)
    // if matrix is row vector, then return row vector
    // if matrix is col vector, then return col vector
    // else, return row vector

    inline DenseMatrix GetSubMatrix(const std::initializer_list<int_max>& LinearIndexList) const;

	template<int_max TemplateVectorLength>
	inline DenseMatrix GetSubMatrix(const DenseVector<int_max, TemplateVectorLength>& LinearIndexList) const;

    inline DenseMatrix GetSubMatrix(const DenseMatrix<int_max>& LinearIndexList) const;

    inline DenseMatrix GetSubMatrix(const int_max* LinearIndexList, int_max ListLength) const;

    //-----------------------------

    inline DenseMatrix GetSubMatrix(const std::initializer_list<int_max>& RowIndexList,
                                    const std::initializer_list<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType> &OutputMatrix, 
                             const std::initializer_list<int_max>& RowIndexList, 
                             const std::initializer_list<int_max>& ColIndexList) const;

    inline DenseMatrix GetSubMatrix(const std::initializer_list<int_max>& RowIndexList,
                                    const MDK_Symbol_ALL& ALL_Symbol) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                             const std::initializer_list<int_max>& RowIndexList,
                             const MDK_Symbol_ALL& ALL_Symbol) const;

    inline DenseMatrix GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                    const std::initializer_list<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                             const MDK_Symbol_ALL& ALL_Symbol,
                             const std::initializer_list<int_max>& ColIndexList) const;

    //-----------------------------
	template<int_max TemplateVectorLengthA, int_max TemplateVectorLengthB>
	inline DenseMatrix GetSubMatrix(const DenseVector<int_max, TemplateVectorLengthA>& RowIndexList,
		                            const DenseVector<int_max, TemplateVectorLengthB>& ColIndexList) const;
	template<int_max TemplateVectorLengthA, int_max TemplateVectorLengthB>
    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                             const DenseVector<int_max, TemplateVectorLengthA>& RowIndexList, 
							 const DenseVector<int_max, TemplateVectorLengthB>& ColIndexList) const;

	template<int_max TemplateVectorLength>
	inline DenseMatrix GetSubMatrix(const DenseVector<int_max, TemplateVectorLength>& RowIndexList,
                                    const MDK_Symbol_ALL& ALL_Symbol) const;
	template<int_max TemplateVectorLength>
    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                             const DenseVector<int_max, TemplateVectorLength>& RowIndexList,
                             const MDK_Symbol_ALL& ALL_Symbol) const;

	template<int_max TemplateVectorLength>
    inline DenseMatrix GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                    const DenseVector<int_max, TemplateVectorLength>& ColIndexList) const;
	template<int_max TemplateVectorLength>
    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                             const MDK_Symbol_ALL& ALL_Symbol,
							 const DenseVector<int_max, TemplateVectorLength>& ColIndexList) const;

    //----------------------

    inline DenseMatrix GetSubMatrix(const DenseMatrix<int_max>& RowIndexList,
                                    const DenseMatrix<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                             const DenseMatrix<int_max>& RowIndexList, 
                             const DenseMatrix<int_max>& ColIndexList) const;

    inline DenseMatrix GetSubMatrix(const DenseMatrix<int_max>& RowIndexList,
                                    const MDK_Symbol_ALL& ALL_Symbol) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                             const DenseMatrix<int_max>& RowIndexList,
                             const MDK_Symbol_ALL& ALL_Symbol) const;

    inline DenseMatrix GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                    const DenseMatrix<int_max>& ColIndexList) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                             const MDK_Symbol_ALL& ALL_Symbol,
                             const DenseMatrix<int_max>& ColIndexList) const;

    //----------------------

    inline DenseMatrix GetSubMatrix(const int_max* RowIndexList, int_max OutputRowNumber,
                                    const int_max* ColIndexList, int_max OutputColNumber) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType> &OutputMatrix, 
                             const int_max* RowIndexList, int_max OutputRowNumber,
                             const int_max* ColIndexList, int_max OutputColNumber) const;

    inline DenseMatrix GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                    const int_max* ColIndexList, int_max OutputColNumber) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                             const MDK_Symbol_ALL& ALL_Symbol, 
                             const int_max* ColIndexList, int_max OutputColNumber) const;

    inline DenseMatrix GetSubMatrix(const int_max* RowIndexList, int_max OutputRowNumber,
                                    const MDK_Symbol_ALL& ALL_Symbol) const;

    inline bool GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                             const int_max* RowIndexList, int_max OutputRowNumber,
                             const MDK_Symbol_ALL& ALL_Symbol) const;

	//---------------------- Get/Set/Fill/Append/Insert A Single Column, Delete multi-Columns ----------------------------------------//
	
	template<typename ElementType_Output = ElementType>
	inline DenseMatrix<ElementType_Output> GetCol(int_max ColIndex) const;

	template<typename ElementType_Output, int_max VectorFixedLength>
	inline bool GetCol(int_max ColIndex, DenseVector<ElementType_Output, VectorFixedLength>& ColData) const;

	// must use different GetCol for variable and fixed length DenseVector
	template<typename ElementType_Output>
	inline bool GetCol(int_max ColIndex, DenseVector<ElementType_Output>& ColData) const;

	template<typename ElementType_Output>
	inline bool GetCol(int_max ColIndex, DenseMatrix<ElementType_Output>& ColData) const;

	template<typename ElementType_Output>
	inline bool GetCol(int_max ColIndex, ElementType_Output* ColData) const;

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColData);

	template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool SetCol(int_max ColIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& ColData);

    template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool SetCol(int_max ColIndex, const ElementType_Input* ColData);
	
    inline bool FillCol(int_max ColIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendCol(const std::initializer_list<ElementType_Input>& ColData);

    template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool AppendCol(const DenseVector<ElementType_Input, TemplateVectorLength>& ColData);

    template<typename ElementType_Input>
    inline bool AppendCol(const DenseMatrix<ElementType_Input>& ColData);

	template<typename ElementType_Input>
    inline bool AppendCol(const ElementType_Input* ColData, int_max Length);

    template<typename ElementType_Input>
    inline bool AppendCol(const ElementType_Input* ColData);

    inline bool DeleteCol(int_max ColIndex);

    inline bool DeleteCol(const std::initializer_list<int_max>& ColIndexList);

	template<int_max TemplateVectorLength>
	inline bool DeleteCol(const DenseVector<int_max, TemplateVectorLength>& ColIndexList);

    inline bool DeleteCol(const DenseMatrix<int_max>& ColIndexList);

    inline bool DeleteCol(const int_max* ColIndexList, int_max ListLength);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColData);

	template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool InsertCol(int_max ColIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColData);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const ElementType_Input* ColData, int_max Length);

    template<typename ElementType_Input>
    inline bool InsertCol(int_max ColIndex, const ElementType_Input* ColData);

	//---------------------- Get/Set/Fill/Append A Single Row, Delete Multi-Rows  ----------------------------------------//
	
	template<typename ElementType_Output = ElementType>
	inline DenseMatrix<ElementType_Output> GetRow(int_max RowIndex) const;

	template<typename ElementType_Output, int_max VectorFixedLength>
	inline bool GetRow(int_max RowIndex, DenseVector<ElementType_Output, VectorFixedLength>& RowData) const;

	// muse use different GetRow
	template<typename ElementType_Output>
	inline bool GetRow(int_max RowIndex, DenseVector<ElementType_Output>& RowData) const;

	template<typename ElementType_Output>
	inline bool GetRow(int_max RowIndex, DenseMatrix<ElementType_Output>& RowData) const;

	template<typename ElementType_Output>
	inline bool GetRow(int_max RowIndex, ElementType_Output* RowData) const;

    template<typename ElementType_Input>
    inline bool SetRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowData);

	template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool SetRow(int_max RowIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& RowData);

    template<typename ElementType_Input>
    inline bool SetRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool SetRow(int_max RowIndex, const ElementType_Input* RowData);

    inline bool FillRow(int_max RowIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool AppendRow(const std::initializer_list<ElementType_Input>& RowData);

	template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool AppendRow(const DenseVector<ElementType_Input, TemplateVectorLength>& RowData);

    template<typename ElementType_Input>
    inline bool AppendRow(const DenseMatrix<ElementType_Input>& RowData);

	template<typename ElementType_Input>
    inline bool AppendRow(const ElementType_Input* RowData, int_max Length);

    template<typename ElementType_Input>
    inline bool AppendRow(const ElementType_Input* RowData);

    inline bool DeleteRow(int_max RowIndex);

    inline bool DeleteRow(const std::initializer_list<int_max>& RowIndexList);

	template<int_max TemplateVectorLength>
	inline bool DeleteRow(const DenseVector<int_max, TemplateVectorLength>& RowIndexList);

    inline bool DeleteRow(const DenseMatrix<int_max>& RowIndexList);

    inline bool DeleteRow(const int_max* RowIndexList, int_max ListLength);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowData);

	template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool InsertRow(int_max RowIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowData);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const ElementType_Input* RowData, int_max Length);

    template<typename ElementType_Input>
    inline bool InsertRow(int_max RowIndex, const ElementType_Input* RowData);

    //---------------------- Append, delete, insert element when matrix is vector -----------------//
    // if matrix is empty or has one element, then it will become row vector
    // if matrix is not vector, then MDK_Error

    inline bool Append(ElementType Element);

    template<typename ElementType_Input>
    inline bool Append(const std::initializer_list<ElementType_Input>& ElementData);

	template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool Append(const DenseVector<ElementType_Input, TemplateVectorLength>& ElementData);

    template<typename ElementType_Input>
    inline bool Append(const DenseMatrix<ElementType_Input>& ElementData);

    template<typename ElementType_Input>
    inline bool Append(const ElementType_Input* ElementData, int_max Length);

    inline bool Delete(int_max LinearIndex);

    inline bool Delete(const std::initializer_list<int_max>& LinearIndexList);

	template<int_max TemplateVectorLength>
	inline bool Delete(const DenseVector<int_max, TemplateVectorLength>& LinearIndexList);

    inline bool Delete(const DenseMatrix<int_max>& LinearIndexList);

    inline bool Delete(const int_max* LinearIndexList, int_max ListLength);

    inline bool Insert(int_max LinearIndex, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool Insert(int_max LinearIndex, const std::initializer_list<ElementType_Input>& ElementData);

	template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool Insert(int_max LinearIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& ElementData);

    template<typename ElementType_Input>
    inline bool Insert(int_max LinearIndex, const DenseMatrix<ElementType_Input>& ElementData);

    template<typename ElementType_Input>
    inline bool Insert(int_max LinearIndex, const ElementType_Input* ElementData, int_max Length);

	//---------------------- Get/Set the diagonal ----------------------------------------//

	template<typename ElementType_Output = ElementType>
	inline DenseMatrix<ElementType_Output> GetDiagonal() const;

	template<typename ElementType_Output, int_max VectorFixedLength>
	inline bool GetDiagonal(DenseVector<ElementType_Output, VectorFixedLength>& RowData) const;

	// muse use different GetDiagonal
	template<typename ElementType_Output>
	inline bool GetDiagonal(DenseVector<ElementType_Output>& RowData) const;

	template<typename ElementType_Output>
	inline bool GetDiagonal(DenseMatrix<ElementType_Output>& DiagonalData) const;

	template<typename ElementType_Output>
	inline bool GetDiagonal(ElementType_Output* DiagonalData) const;

    template<typename ElementType_Input>
    inline bool SetDiagonal(const std::initializer_list<ElementType_Input>& DiagonalData);

	template<typename ElementType_Input, int_max TemplateVectorLength>
	inline bool SetDiagonal(const DenseVector<ElementType_Input, TemplateVectorLength>& DiagonalData);

    template<typename ElementType_Input>
    inline bool SetDiagonal(const DenseMatrix<ElementType_Input>& DiagonalData);

	template<typename ElementType_Input>
	inline bool SetDiagonal(const ElementType_Input* DiagonalData);

    inline bool FillDiagonal(const ElementType& Element);

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

    inline DenseMatrix ElementOperation(const char* OperationName) const;

    inline DenseMatrix ElementOperation(const String& OperationName) const;

    template<typename OperationType>
    inline DenseMatrix ElementOperation(OperationType Operation) const;

    inline DenseMatrix ElementOperation(const char OperationName, const DenseMatrix<ElementType>& InputMatrix) const;

    inline DenseMatrix ElementOperation(const char* OperationName, const DenseMatrix<ElementType>& InputMatrix) const;

    inline DenseMatrix ElementOperation(const String& OperationName, const DenseMatrix<ElementType>& InputMatrix) const;

    template<typename OperationType>
    inline DenseMatrix ElementOperation(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix) const;

    inline DenseMatrix ElementOperation(const char OperationName, const ElementType& Element) const;

    inline DenseMatrix ElementOperation(const char* OperationName, const ElementType& Element) const;

    inline DenseMatrix ElementOperation(const String& OperationName, const ElementType& Element) const;

    template<typename OperationType>
    inline DenseMatrix ElementOperation(OperationType Operation, const ElementType& Element) const;

    //-------------------- general element operation in place : Object.ElementOperationInPlace modify the object itself ---------------//

    inline bool ElementOperationInPlace(const char* OperationName);

    inline bool ElementOperationInPlace(const String& OperationName);

    template<typename OperationType>
    inline bool ElementOperationInPlace(OperationType Operation);

    inline bool ElementOperationInPlace(const char OperationName, const DenseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const char* OperationName, const DenseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const String& OperationName, const DenseMatrix<ElementType>& InputMatrix);

    template<typename OperationType>
    inline bool ElementOperationInPlace(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix);

    inline bool ElementOperationInPlace(const char OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(const char* OperationName, const ElementType& Element);

    inline bool ElementOperationInPlace(const String& OperationName, const ElementType& Element);

    template<typename OperationType>
    inline bool ElementOperationInPlace(OperationType Operation, const ElementType& Element);

    //-------------------- general Col operation : output a new col-matrix ------------------------------------------//

    inline DenseMatrix ColOperation(int_max ColIndex, const char* OperationName, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColOperation(int_max ColIndex, const String& OperationName, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix ColOperation(int_max ColIndex, OperationType Operation, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColOperation(int_max ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColOperation(int_max ColIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColOperation(int_max ColIndex, const String& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;
    
    template<typename OperationType>
    inline DenseMatrix ColOperation(int_max ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColOperation(int_max ColIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColOperation(int_max ColIndex, const char* OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    inline DenseMatrix ColOperation(int_max ColIndex, const String& OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix ColOperation(int_max ColIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true) const;

    //-------------------- general col operation in place : Object.ColOperationInPlace modify the object itself ---------------//

    inline bool ColOperationInPlace(int_max ColIndex, const char* OperationName, bool EnableBoundCheck = true);

    inline bool ColOperationInPlace(int_max ColIndex, const String& OperationName, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool ColOperationInPlace(int_max ColIndex, OperationType Operation, bool EnableBoundCheck = true);

    inline bool ColOperationInPlace(int_max ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool ColOperationInPlace(int_max ColIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool ColOperationInPlace(int_max ColIndex, const String& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool ColOperationInPlace(int_max ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool ColOperationInPlace(int_max ColIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    inline bool ColOperationInPlace(int_max ColIndex, const char* OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    inline bool ColOperationInPlace(int_max ColIndex, const String& OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool ColOperationInPlace(int_max ColIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true);

    //-------------------- general Row operation : output a new row-matrix ------------------------------------------//

    inline DenseMatrix RowOperation(int_max RowIndex, const char* OperationName, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowOperation(int_max RowIndex, const String& OperationName, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix RowOperation(int_max RowIndex, OperationType Operation, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowOperation(int_max RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowOperation(int_max RowIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowOperation(int_max RowIndex, const String& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix RowOperation(int_max RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowOperation(int_max RowIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowOperation(int_max RowIndex, const char* OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    inline DenseMatrix RowOperation(int_max RowIndex, const String& OperationName, const ElementType& Element, bool EnableBoundCheck = true) const;

    template<typename OperationType>
    inline DenseMatrix RowOperation(int_max RowIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true) const;

    //-------------------- general row operation in place : Object.RowOperationInPlace modify the object itself ---------------//

    inline bool RowOperationInPlace(int_max RowIndex, const char* OperationName, bool EnableBoundCheck = true);

    inline bool RowOperationInPlace(int_max RowIndex, const String& OperationName, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool RowOperationInPlace(int_max RowIndex, OperationType Operation, bool EnableBoundCheck = true);

    inline bool RowOperationInPlace(int_max RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool RowOperationInPlace(int_max RowIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool RowOperationInPlace(int_max RowIndex, const String& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool RowOperationInPlace(int_max RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true);

    inline bool RowOperationInPlace(int_max RowIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    inline bool RowOperationInPlace(int_max RowIndex, const char* OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    inline bool RowOperationInPlace(int_max RowIndex, const String& OperationName, const ElementType& Element, bool EnableBoundCheck = true);

    template<typename OperationType>
    inline bool RowOperationInPlace(int_max RowIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true);

    //------------------------ find element : return linear index ----------------------------//

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> Find(MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> Find(int_max MaxOutputNumber, int_max LinearIndex_start, int_max LinearIndex_end, MatchFunctionType MatchFunction) const;

	// find the first/last that match the condition
	template<typename MatchFunctionType>
	inline int_max Find(const String& first_or_last, MatchFunctionType MatchFunction) const;

	//-------------------- Match element (use == operater) : return linear index -----------------------------------//

	inline DenseMatrix<int_max> ExactMatch(const ElementType& InputElement) const;

	inline int_max ExactMatch(const String& first_or_last, const ElementType& InputElement) const;

    //------------------------ find col : return col index list ------------------------------------//

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> FindCol(MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> FindCol(int_max MaxOutputColNumber, MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> FindCol(int_max MaxOutputColNumber, int_max ColIndex_start, int_max ColIndex_end, MatchFunctionType MatchFunction) const;

	template<typename MatchFunctionType>
	inline int_max FindCol(const String& first_or_last, MatchFunctionType MatchFunction) const;

    //----------------------- sort element : return sorted linear index list ----------//

    template<typename CompareFunctionType>
    inline DenseMatrix<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline DenseMatrix<int_max> Sort(int_max LinearIndex_start, int_max LinearIndex_end, CompareFunctionType CompareFunction) const;

	inline DenseMatrix<int_max> Sort(const char* ascend_or_descend) const;

	inline DenseMatrix<int_max> Sort(const String& ascend_or_descend) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max LinearIndex_start, int_max LinearIndex_end, CompareFunctionType CompareFunction);

	inline void SortInPlace(const char* ascend_or_descend);

	inline void SortInPlace(const String& ascend_or_descend);

    //----------------------- sort col : return sorted col index list ----------------------------------//

    template<typename CompareFunctionType>
    inline DenseMatrix<int_max> SortCol(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline DenseMatrix<int_max> SortCol(int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline void SortColInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortColInPlace(int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction);

    //------------------------- unique : return linear index list of unique element ------------------------------------//

    inline DenseMatrix<int_max> FindUnique() const;

    //SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
    template<typename SpecialCompareFunctionType>
    inline DenseMatrix<int_max> FindUnique(SpecialCompareFunctionType SpecialCompareFunction) const;

    //------------------------- unique col : return col index list of unique col ---------------------------//

    inline DenseMatrix<int_max> FindUniqueCol() const;

    //SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
    template<typename SpecialCompareFunctionType>
    inline DenseMatrix<int_max> FindUniqueCol(SpecialCompareFunctionType SpecialCompareFunction) const;

	//--------------------- LinearIndex to/from 2DIndex [RowIndex, ColIndex] ------------------------------//
	inline DenseVector<int_max, 2> TransformLinearIndexTo2DIndex(int_max LinearIndex) const;

	inline int_max Transform2DIndexToLinearIndex(const DenseVector<int_max, 2>& Index2D) const;

	inline int_max Transform2DIndexToLinearIndex(int_max RowIndex, int_max ColIndex) const;

	//-------------------- calculate sum mean min max ------------------------------------------//

    inline ElementType Mean() const;

	inline DenseMatrix MeanOfEachCol() const;

	inline DenseMatrix MeanOfEachRow() const;

	inline ElementType Std() const;

    inline ElementType Sum() const;

    inline DenseMatrix SumOfEachCol() const;

	inline DenseMatrix SumOfEachRow() const;

    inline int_max IndexOfMax() const;

	inline DenseVector<int_max, 2> RowIndexAndColIndexOfMax() const;

    inline ElementType Max() const;

    inline DenseMatrix MaxOfEachCol() const;

	inline DenseMatrix MaxOfEachRow() const;

    inline int_max IndexOfMin() const;

	inline DenseVector<int_max, 2> RowIndexAndColIndexOfMin() const;

    inline ElementType Min() const;

    inline DenseMatrix MinOfEachCol() const;

	inline DenseMatrix MinOfEachRow() const;

    //------------------------------------ norm ----------------------------------------------//

    inline ElementType L1Norm() const;

    inline ElementType L2Norm() const;

	//----------------------------------- transpose -----------------------------------------//

    inline DenseMatrix Transpose() const;

    inline void TransposeInPlace();

	//----------------------------------- Rank -----------------------------------------//

    inline int_max Rank() const;

	//----------------------------------- determinant --------------------------------//
	
	inline ElementType Det() const;

	//----------------------------------- inverse -----------------------------------------//

    inline DenseMatrix Inv() const;

    inline DenseMatrix PInv() const; // Pseudo Inverse

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
