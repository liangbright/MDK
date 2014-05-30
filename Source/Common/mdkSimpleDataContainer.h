#ifndef __mdkSimpleDataContainer_h
#define __mdkSimpleDataContainer_h

#include <vector>
#include <string>
#include <memory>

#include "mdkObject.h"

namespace mdk
{

//------------------------------
template<typename ElementType>
class DenseMatrix;
//------------------------------

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class SimpleDataContainer : public Object
{     
    std::vector<ElementType> m_DataArray;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline SimpleDataContainer();

    inline SimpleDataContainer(const std::initializer_list<ElementType>& InputData);

    inline SimpleDataContainer(const std::vector<ElementType>& InputData);

    // deep-copy or shared-copy constructor
    inline SimpleDataContainer(const SimpleDataContainer<ElementType>& InputData);

    // move constructor
    inline SimpleDataContainer(SimpleDataContainer<ElementType>&& InputData) noexcept;

	inline ~SimpleDataContainer();

    //-------------------- get/set std vector -----------------------------------//

    inline std::vector<ElementType>& StdVector();

    inline const std::vector<ElementType>& StdVector() const;

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const SimpleDataContainer<ElementType>& InputData);

    inline void operator=(SimpleDataContainer<ElementType>&& InputData);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    inline void operator=(const std::vector<ElementType>& InputList);

    //----------------------  Copy  ----------------------------------------//

    inline bool Copy(const SimpleDataContainer<ElementType>& InputData);

    inline bool Copy(const SimpleDataContainer<ElementType>* InputData);

    inline bool Copy(const ElementType* InputElementPointer, int_max InputLength);

    inline bool Fill(const ElementType& Element);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

    inline bool Resize(int_max InputLength); // try to keep the old data

    inline bool FastResize(int_max InputLength); // do not care about old data

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current Length does not change

    inline void Squeeze();

    inline bool IsEmpty() const;

    inline int_max GetLength() const;

    inline int_max GetElementNumber() const; // the same as GetLength();

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer(); //  the pointer of the first element

    inline const ElementType* GetElementPointer() const;

    inline ElementType* begin();

    inline const ElementType* begin() const;

    inline ElementType* end(); // 1 + pointer of the last element

    inline const ElementType* end() const; // 1 + pointer of the last element

	//----------- Get/Set by Index -----------------------------------//

    // operator[] or () : no bound check

    inline ElementType& operator[](int_max Index);

    inline const ElementType& operator[](int_max Index) const;

    inline ElementType& operator()(int_max Index);

    inline const ElementType& operator()(int_max Index) const;
    
    // at(): bound check

    inline ElementType& at(int_max Index);

    inline const ElementType& at(int_max Index) const;

    //-------------------------------------------------------------------------------

    inline bool Append(ElementType Element);

    inline bool Append(const ElementType* InputData, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const std::vector<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

    inline bool Delete(const SimpleDataContainer<int_max>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    inline bool Insert(int_max Index, const std::initializer_list<ElementType>& InputData);

    inline bool Insert(int_max Index, const std::vector<ElementType>& InputData);

    inline bool Insert(int_max Index, const DenseMatrix<ElementType>& InputData);

    inline bool Insert(int_max Index, const SimpleDataContainer<ElementType>& InputData);

    inline bool Insert(int_max Index, const ElementType* InputData, int_max InputLength);

    //------------- use SimpleDataContainer as a stack ----------------------------//

    inline bool Push(ElementType Element);

    inline ElementType Pop();

    //----------------------- Get a subset ------------------------------//

    inline SimpleDataContainer<ElementType> GetSubSet(int_max Index_start, int_max Index_end);

    inline SimpleDataContainer<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList);

    inline SimpleDataContainer<ElementType> GetSubSet(const std::vector<int_max>& IndexList);

    inline SimpleDataContainer<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList);

    inline SimpleDataContainer<ElementType> GetSubSet(const SimpleDataContainer<int_max>& IndexList);

    inline SimpleDataContainer<ElementType> GetSubSet(const int_max* IndexList, int_max ListLength);

    //-------------------- find ---------------------------------------//

    template<typename MatchFunctionType>
    inline SimpleDataContainer<int_max> Find(MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline SimpleDataContainer<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline SimpleDataContainer<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction);

    //--------------------- sort ---------------------------------------//

    template<typename CompareFunctionType>
    inline SimpleDataContainer<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline SimpleDataContainer<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

private:

};


}//end namespace mdk

#include "mdkSimpleDataContainer.hpp"

#endif
