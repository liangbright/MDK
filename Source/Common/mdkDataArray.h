#ifndef __mdkDataArray_h
#define __mdkDataArray_h

#include <vector>
#include <string>
#include <memory>

#include "mdkObject.h"
#include "mdkConstant.h"

namespace mdk
{

#if defined MDK_DEBUG_MODE
    #define MDK_DEBUG_DataArray_Operator_CheckBound
#endif

//------------------------------
template<typename ElementType>
class DenseMatrix;
//------------------------------

// ----------------------------- DataArrayData struct -------------------------------------------------------------//

template<typename ElementType>
struct DataArrayData
{
    std::vector<ElementType> DataArray;

    int_max Length;

    ElementType* ElementPointer;

    ElementType ErrorElement;

    bool IsSizeFixed;

//-------------------------------------------------------------
    DataArrayData() 
    {
        Length = 0;
        ElementPointer = nullptr;
        IsSizeFixed = false;
        ErrorElement = GetNaNElement(ErrorElement);
    };

    ~DataArrayData() {};

    void CopyDataToInternalDataArrayIfNecessary()
    {
        if (ElementPointer != DataArray.data())
        {
            if (ElementPointer == nullptr)
            {
                MDK_Error("ElementPointer is nullptr @ DataArrayData::CopyDataToInternalDataArrayIfNecessary()")
                return;
            }

            DataArray.resize(Length);

            for (int_max i = 0; i < Length; ++i)
            {
                DataArray[i] = ElementPointer[i];
            }

            ElementPointer = DataArray.data();
        }
    }

    ElementType& operator[](int_max Index)
    {
        return ElementPointer[Index];
    }

    const ElementType& operator[](int_max Index) const
    {
        return ElementPointer[Index];
    }

    ElementType& operator()(int_max Index)
    {
        return ElementPointer[Index];
    }

    const ElementType& operator()(int_max Index) const
    {
        return ElementPointer[Index];
    }

private:
//deleted: -------------------------------------------------
    DataArrayData(const DataArrayData&) = delete;

    DataArrayData(DataArrayData&&) = delete;

    void operator=(const DataArrayData&) = delete;

    void operator=(DataArrayData&&) = delete;
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class DataArray : public Object
{
private:
     
    std::shared_ptr<DataArrayData<ElementType>> m_Data;

    ElementType* m_ElementPointer;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline DataArray();

    inline DataArray(const std::initializer_list<ElementType>& InputData);

    inline DataArray(const std::vector<ElementType>& InputData);

    // deep-copy or shared-copy constructor
    inline DataArray(const DataArray<ElementType>& InputData, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy);

    // move constructor
    inline DataArray(DataArray<ElementType>&& InputData) noexcept;

	inline ~DataArray();

    //-------------------- get/set std vector -----------------------------------//

    inline std::vector<ElementType>& StdVector();

    inline const std::vector<ElementType>& StdVector() const;

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const DataArray<ElementType>& InputData);

    inline void operator=(DataArray<ElementType>&& InputData);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    inline void operator=(const std::vector<ElementType>& InputList);

    //----------------------  Copy  ----------------------------------------//

    inline bool Copy(const DataArray<ElementType>& InputData);

    inline bool Copy(const DataArray<ElementType>* InputData);

    inline bool Copy(const ElementType* InputElementPointer, int_max InputLength);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

    inline bool Share(DataArray<ElementType>& InputData);

    inline bool Share(DataArray<ElementType>* InputData);

    inline void ForceShare(const DataArray<ElementType>& InputData);

    inline bool ForceShare(const DataArray<ElementType>* InputData);

    //-------------------------- special Share  ---------------------------------------------- //

    inline bool Share(ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed = true);

    inline bool ForceShare(const ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed = true);

    //-------------------- Take -----------------------------------------------------------//

    inline void Take(DataArray<ElementType>&& InputData);

    inline bool Take(DataArray<ElementType>& InputData);

    inline bool Take(DataArray<ElementType>* InputData);

    //------------------------- Swap shared_ptr m_Data -------------------------------------------//

    inline void SwapSmartPointer(DataArray<ElementType>& InputData);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

    inline bool Resize(int_max InputLength); // try to keep the old data

    inline bool FastResize(int_max InputLength); // do not care about old data

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current Length does not change

    inline void Squeeze();

    inline void FixSize();

    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;

    inline bool IsShared() const;

    inline int_max GetLength() const;

    inline int_max GetElementNumber() const; // the same as GetLength();

    //------------------------ Error Element -----------------------------//

    const ElementType& GetErrorElement()  const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer(); //  the pointer of the first element
    inline const ElementType* GetElementPointer() const;

    inline ElementType* GetPointer(); //  the pointer of the first element
    inline const ElementType* GetPointer() const;

    inline ElementType* begin();
    inline const ElementType* begin() const;

    inline ElementType* end(); // 1 + pointer of the last element
    inline const ElementType* end() const; // 1 + pointer of the last element

	//----------- Get/Set by Index -----------------------------------//

    // operator[] or () : no bound check in release mode

    inline ElementType& operator[](int_max Index);
    inline const ElementType& operator[](int_max Index) const;

    inline ElementType& operator()(int_max Index);
    inline const ElementType& operator()(int_max Index) const;
    
    // at(): bound check

    inline ElementType& at(int_max Index);
    inline const ElementType& at(int_max Index) const;

    //-------------------------------------------------------------------------------

    inline bool Append(ElementType Element);

    //inline bool Append(const std::initializer_list<ElementType>& InputData);

    //inline bool Append(const std::vector<ElementType>& InputData);

    // error if ElementType is std::vector
    //inline bool Append(const DenseMatrix<ElementType>& InputData);

    //inline bool Append(const DataArray<ElementType>& InputData);

    inline bool Append(const ElementType* InputData, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const std::vector<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

    inline bool Delete(const DataArray<int_max>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    inline bool Insert(int_max Index, const std::initializer_list<ElementType>& InputData);

    inline bool Insert(int_max Index, const std::vector<ElementType>& InputData);

    inline bool Insert(int_max Index, const DenseMatrix<ElementType>& InputData);

    inline bool Insert(int_max Index, const DataArray<ElementType>& InputData);

    inline bool Insert(int_max Index, const ElementType* InputData, int_max InputLength);

    //------------- use DataArray as a stack ----------------------------//

    inline bool Push(ElementType Element);

    inline ElementType Pop();

    //----------------------- Get a subset ------------------------------//

    inline DataArray<ElementType> GetSubSet(int_max Index_start, int_max Index_end);

    inline DataArray<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList);

    inline DataArray<ElementType> GetSubSet(const std::vector<int_max>& IndexList);

    inline DataArray<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList);

    inline DataArray<ElementType> GetSubSet(const DataArray<int_max>& IndexList);

    inline DataArray<ElementType> GetSubSet(const int_max* IndexList, int_max ListLength);

    //-------------------- find ---------------------------------------//

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> Find(MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline DenseMatrix<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction);

    //--------------------- sort ---------------------------------------//

    template<typename CompareFunctionType>
    inline DenseMatrix<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline DenseMatrix<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

private:

};


}//end namespace mdk

#include "mdkDataArray.hpp"

#endif
