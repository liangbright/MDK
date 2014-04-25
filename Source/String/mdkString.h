#ifndef __mdkString_h
#define __mdkString_h

#include <vector>
#include <string>

#include "mdkObject.h"

namespace mdk
{

// this class is based on std::string
// String can be shared in MDK
//  
// It is usually used to store Name
// e.g., Name of Dictionary, Name of File

//----------- forward declare -------------------//
template<typename ElementType>
class DataContainer;

template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class String;
//-----------------------------------------//

typedef String<char>     CharString;
typedef String<wchar_t>  WCharString;
typedef String<char16_t> U16CharString;
typedef String<char32_t> U32CharString;


template<typename ElementType>
void MDK_Check_ElementType_Of_String()
{
    MDK_Check_ElementType_Of_String(ElementType(0));
}

template<typename ElementType>
void MDK_Check_ElementType_Of_String(ElementType ReferenceElement)
{
    MDK_Error("Wrong ElementType for String @ MDK_Check_ElementType_Of_String(...)")
}

void MDK_Check_ElementType_Of_String(char)
{
}

void MDK_Check_ElementType_Of_String(wchar_t)
{
}

void MDK_Check_ElementType_Of_String(char16_t)
{
}

void MDK_Check_ElementType_Of_String(char32_t)
{
}

//----------------------------------------------------------------------------------------------------------------------------//
template<typename ElementType>
class String : public Object
{
private:
     
    std::shared_ptr<std::basic_string<ElementType>> m_StringData;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline String();

    inline String(const ElementType& Element);

    inline String(const std::initializer_list<ElementType>& InputString);

    inline String(const ElementType* InputString);

    inline String(const std::basic_string<ElementType>& InputString);

    // deep-copy or shared-copy constructor
    inline String(const String<ElementType>& InputString, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy);

    // move constructor
    inline String(String<ElementType>&& InputString) noexcept;

	inline ~String();

    //----------------------  operator=  ----------------------------------------//

    inline void operator=(const String<ElementType>& InputString);

    inline void operator=(String<ElementType>&& InputString);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& InputString);

    inline void operator=(const ElementType* InputString);

    inline void operator=(const std::basic_string<ElementType>& InputString);

    //----------------------  Get/Set Std string  ----------------------------------------//

    inline std::basic_string<ElementType>& StdString();

    inline const std::basic_string<ElementType>& StdString() const;

    //----------------------  Copy  ----------------------------------------//

    inline void Copy(const String<ElementType>& InputString);

    inline void Copy(const String<ElementType>* InputString);

    inline void Copy(const ElementType* InputElementPointer);

    inline void Fill(const ElementType& Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

    inline void Share(String<ElementType>& InputString);

    inline void Share(String<ElementType>* InputString);

    inline void ForceShare(const String<ElementType>& InputString);

    inline void ForceShare(const String<ElementType>* InputString);

    //-------------------- Take -----------------------------------------------------------//

    inline void Take(String<ElementType>&& InputString);

    inline void Take(String<ElementType>& InputString);

    inline void Take(String<ElementType>* InputString);

    //------------------------- Swap shared_ptr m_StringData -------------------------------------------//

    inline void SwapSmartPointer(String<ElementType>& InputString);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

    inline bool Resize(int_max InputLength); // try to keep the old data

    inline bool FastResize(int_max InputLength); // do not care about old data

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current Length does not change

    inline void Squeeze();

    inline bool IsEmpty() const;

    inline bool IsShared() const;

    inline int_max GetElementNumber() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer(); //  the position of the first element

    inline const ElementType* GetElementPointer() const;

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

    inline void Append(const ElementType& Element);

    inline void Append(const std::initializer_list<ElementType>& InputString);

    inline void Append(const std::vector<ElementType>& InputString);

    inline void Append(const DenseMatrix<ElementType>& InputString);

    inline void Append(const DataContainer<ElementType>& InputString);

    inline void Append(const ElementType* InputString);

    inline void Append(const std::basic_string<ElementType>& InputString);

    inline void Append(const String<ElementType>& InputString);

    inline void Delete(int_max Index);

    inline void Delete(const std::initializer_list<int_max>& IndexList);

    inline void Delete(const std::vector<int_max>& IndexList);

    inline void Delete(const DenseMatrix<int_max>& IndexList);

    inline void Delete(const DataContainer<int_max>& IndexList);

    inline void Delete(const int_max* IndexList, int_max ListLength);

    inline void Delete(int_max Index_start, int_max Index_end);

    inline void Insert(int_max Index, const ElementType& Element);

    inline void Insert(int_max Index, const std::initializer_list<ElementType>& InputString);

    inline void Insert(int_max Index, const std::vector<ElementType>& InputString);

    inline void Insert(int_max Index, const DenseMatrix<ElementType>& InputString);

    inline void Insert(int_max Index, const DataContainer<ElementType>& InputString);

    inline void Insert(int_max Index, const String<ElementType>& InputString);

    inline void Insert(int_max Index, const std::basic_string<ElementType>& InputString);

    inline void Insert(int_max Index, const ElementType* InputString);

    //------------------------------------------------------------------

    inline void operator+=(const String<ElementType>& InputString);

    inline void operator+=(const std::basic_string<ElementType>& InputString);

    inline void operator+=(const ElementType* InputString);

private:

};

//------------------------------- non-member function -----------------------------------------------------------//

template<typename ElementType>
inline
String<ElementType> operator+(const String<ElementType>& StringA, const String<ElementType>& StringB);

template<typename ElementType>
inline
String<ElementType> operator+(const String<ElementType>& StringA, const std::basic_string<ElementType>& StringB);

template<typename ElementType>
inline
String<ElementType> operator+(const std::basic_string<ElementType>& StringA, const String<ElementType>& StringB);

template<typename ElementType>
inline
String<ElementType> operator+(const String<ElementType>& StringA, const ElementType* StringB);

template<typename ElementType>
inline
String<ElementType> operator+(const ElementType* StringA, const String<ElementType>& StringB);

template<typename ElementType>
inline
bool operator==(const String<ElementType>& StringA, const String<ElementType>& StringB);

template<typename ElementType>
inline
bool operator==(const String<ElementType>& StringA, const std::basic_string<ElementType>& StringB);

template<typename ElementType>
inline
bool operator==(const std::basic_string<ElementType>& StringA, const String<ElementType>& StringB);

template<typename ElementType>
inline
bool operator==(const String<ElementType>& StringA, const ElementType* StringB);

template<typename ElementType>
inline
bool operator==(const ElementType* StringA, const String<ElementType>& StringB);

template<typename ElementType>
inline
bool operator!=(const String<ElementType>& StringA, const String<ElementType>& StringB);

template<typename ElementType>
inline
bool operator!=(const String<ElementType>& StringA, const std::basic_string<ElementType>& StringB);

template<typename ElementType>
inline
bool operator!=(const std::basic_string<ElementType>& StringA, const String<ElementType>& StringB);

template<typename ElementType>
inline
bool operator!=(const String<ElementType>& StringA, const ElementType* StringB);

template<typename ElementType>
inline
bool operator!=(const ElementType* StringA, const String<ElementType>& StringB);

template <typename ElementType, typename Traits = std::char_traits<ElementType>>
inline
std::basic_ostream<ElementType, Traits>&  operator<<(std::basic_ostream<ElementType, Traits>& os, const String<ElementType>& str);

}//end namespace mdk

#include "mdkString.hpp"

#endif
