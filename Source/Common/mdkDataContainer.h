#ifndef __mdkDataContainer_h
#define __mdkDataContainer_h

#include <vector>
#include <string>

#include "mdkObject.h"
#include "mdkConstant.h"

namespace mdk
{

#if defined MDK_DEBUG_MODE
    #define MDK_DEBUG_DataContainer_Operator_CheckBound
#endif

//------------------------------
template<typename ElementType>
class DenseMatrix;
//------------------------------

// ----------------------------- DataContainerData struct -------------------------------------------------------------//

template<typename ElementType>
struct DataContainerData
{
    int_max Length; 

    std::vector<ElementType> DataArray;

    ElementType NaNElement;

    bool IsSizeFixed;

//-------------------------------------------------------------
    DataContainerData() 
    {
        Length = 0;
        IsSizeFixed = false;
        NaNElement = GetNaNElement(NaNElement);
    };

    ~DataContainerData() {};

    ElementType& operator[](int_max Index)
    {
        return DataArray[Index];
    }

    const ElementType& operator[](int_max Index) const
    {
        return DataArray[Index];
    }

    ElementType& operator()(int_max Index)
    {
        return DataArray[Index];
    }

    const ElementType& operator()(int_max Index) const
    {
        return DataArray[Index];
    }

private:
//deleted: -------------------------------------------------
    DataContainerData(const DataContainerData&) = delete;

    DataContainerData(DataContainerData&&) = delete;

    void operator=(const DataContainerData&) = delete;

    void operator=(DataContainerData&&) = delete;
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class DataContainer : public Object
{
private:
     
    std::shared_ptr<DataContainerData<ElementType>> m_Data;

    ElementType* m_ElementPointer;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline DataContainer();

    inline DataContainer(const std::initializer_list<ElementType>& InputData);

    inline DataContainer(const ElementType& Element);

    // deep-copy or shared-copy constructor
    inline DataContainer(const DataContainer<ElementType>& InputData, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy);

    // move constructor
    inline DataContainer(DataContainer<ElementType>&& InputData) noexcept;

	inline ~DataContainer();

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const DataContainer<ElementType>& InputData);

    inline void operator=(DataContainer<ElementType>&& InputData);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    //----------------------  Copy  ----------------------------------------//

    template<typename ElementType_Input>  
    inline bool Copy(const DataContainer<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Copy(const DataContainer<ElementType_Input>* InputData);

    template<typename ElementType_Input>
    inline bool Copy(const ElementType_Input* InputElementPointer, int_max InputLength);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

    inline bool Share(DataContainer<ElementType>& InputData);

    inline bool Share(DataContainer<ElementType>* InputData);

    inline void ForceShare(const DataContainer<ElementType>& InputData);

    inline bool ForceShare(const DataContainer<ElementType>* InputData);

    //-------------------- Take -----------------------------------------------------------//

    inline void Take(DataContainer<ElementType>&& InputData);

    inline bool Take(DataContainer<ElementType>& InputData);

    inline bool Take(DataContainer<ElementType>* InputData);

    //------------------------- Swap shared_ptr m_Data -------------------------------------------//

    inline void SwapSmartPointer(DataContainer<ElementType>& InputData);

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

    inline int_max GetElementNumber() const; // the same as GetLength()

    //------------------------ Element Info -----------------------------//

    const ElementType& GetNaNElement()  const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer(); //  the position of the first element

    inline const ElementType* GetElementPointer() const;

    inline ElementType* begin();

    inline const ElementType* begin() const;

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

    inline bool Append(const ElementType& Element);

    template<typename ElementType_Input>
    inline bool Append(const std::initializer_list<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Append(const std::vector<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Append(const DenseMatrix<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Append(const DataContainer<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Append(const ElementType_Input* InputData, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const std::vector<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

    inline bool Delete(const DataContainer<int_max>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    template<typename ElementType_Input>
    inline bool Insert(int_max Index, const std::initializer_list<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Insert(int_max Index, const std::vector<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Insert(int_max Index, const DenseMatrix<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Insert(int_max Index, const DataContainer<ElementType_Input>& InputData);

    template<typename ElementType_Input>
    inline bool Insert(int_max Index, const ElementType_Input* InputData, int_max InputLength);

private:

};


}//end namespace mdk

#include "mdkDataContainer.hpp"

#endif
