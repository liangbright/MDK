#ifndef __mdkString_h
#define __mdkString_h

#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <ostream>

#include "mdkDebugConfig.h"
#include "mdkCommonType.h"
#include "mdkObject.h"
#include "mdkDenseVector_ForwardDeclare.h"

namespace mdk
{
// this class is based on std::string
// String can be shared in MDK
//  

//----------- forward declare -------------------//
template<typename ElementType>
class ObjectArray;

template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class BasicString;
//-----------------------------------------//
typedef BasicString<char> String; // assume UTF-8
//-----
typedef BasicString<char>     CharString;
typedef BasicString<wchar_t>  WCharString;
typedef BasicString<char16_t> Char16String;
typedef BasicString<char32_t> Char32String;
//-----------------------------------------//

template<typename ElementType>
inline void MDK_Check_ElementType_Of_String()
{
    MDK_Check_ElementType_Of_String(ElementType(0));
}

template<typename ElementType>
inline void MDK_Check_ElementType_Of_String(const ElementType& ReferenceElement)
{
    MDK_Error("Wrong ElementType for BasicString @ MDK_Check_ElementType_Of_String(...)")
}

inline void MDK_Check_ElementType_Of_String(const char&)
{
}

inline void MDK_Check_ElementType_Of_String(const wchar_t&)
{
}

inline void MDK_Check_ElementType_Of_String(const char16_t&)
{
}

inline void MDK_Check_ElementType_Of_String(const char32_t&)
{
}

//----------------------------------------------------------------------------------------------------------------------------//
template<typename Element_Type>
class BasicString : public Object
{
public:
	typedef Element_Type  ElementType;

private:     
    std::shared_ptr<std::basic_string<ElementType>> m_StringData;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline BasicString();

    inline BasicString(const ElementType& Element);

    inline BasicString(const std::initializer_list<ElementType>& InputString);

    inline BasicString(const ElementType* InputString);

    inline BasicString(const std::basic_string<ElementType>& InputString);

    // deep-copy or shared-copy constructor
    inline BasicString(const BasicString<ElementType>& InputString, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy);

    // move constructor
    inline BasicString(BasicString<ElementType>&& InputString) noexcept;

	inline ~BasicString();

    //----------------------  operator=  ----------------------------------------//

    inline void operator=(const BasicString<ElementType>& InputString);

    inline void operator=(BasicString<ElementType>&& InputString);

    inline void operator=(const ElementType& Element);

    inline void operator=(const std::initializer_list<ElementType>& InputString);

    inline void operator=(const ElementType* InputString);

    inline void operator=(const std::basic_string<ElementType>& InputString);

    //----------------------  Get/Set Std string  ----------------------------------------//

    inline std::basic_string<ElementType>& StdString();

    inline const std::basic_string<ElementType>& StdString() const;

    //----------------------  Copy  ----------------------------------------//

    inline void Copy(const BasicString<ElementType>& InputString);

    inline void Copy(const BasicString<ElementType>* InputString);

    inline void Copy(const ElementType* InputElementPointer);

    inline void Fill(const ElementType& Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

    inline void Share(BasicString<ElementType>& InputString);

    inline void Share(BasicString<ElementType>* InputString);

    inline void ForceShare(const BasicString<ElementType>& InputString);

    inline void ForceShare(const BasicString<ElementType>* InputString);

    //-------------------- Take -----------------------------------------------------------//

    inline void Take(BasicString<ElementType>&& InputString);

    inline void Take(BasicString<ElementType>& InputString);

    inline void Take(BasicString<ElementType>* InputString);

    //------------------------- Swap shared_ptr m_StringData -------------------------------------------//

    inline void SwapSmartPointer(BasicString<ElementType>& InputString);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

    inline bool Resize(int_max InputLength); // try to keep the old data

    inline bool FastResize(int_max InputLength); // do not care about old data

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current Length does not change

	inline void ReleaseUnusedCapacity();

    inline bool IsEmpty() const;

    inline bool IsShared() const;

    inline int_max GetElementNumber() const;

	inline int_max GetLength() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer(); //  the position of the first element

    inline const ElementType* GetElementPointer() const;

	inline ElementType* GetPointer(); //  the position of the first element

	inline const ElementType* GetPointer() const;

	inline ElementType* begin(); //  the position of the first element

	inline const ElementType* begin() const;

	inline ElementType* end(); //  the position of the first element

	inline const ElementType* end() const;

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

    inline void Append(const std::basic_string<ElementType>& InputString);

    inline void Append(const BasicString<ElementType>& InputString);

	inline void Append(const ElementType* InputString);

    inline void Delete(int_max Index);

    inline void Delete(const std::initializer_list<int_max>& IndexList);

    inline void Delete(const std::vector<int_max>& IndexList);

	inline void Delete(const DenseVector<int_max>& IndexList);

    inline void Delete(const DenseMatrix<int_max>& IndexList);

    inline void Delete(const ObjectArray<int_max>& IndexList);

    inline void Delete(const int_max* IndexList, int_max ListLength);

    inline void Delete(int_max Index_start, int_max Index_end);

    inline void Insert(int_max Index, const ElementType& Element);

    inline void Insert(int_max Index, const std::initializer_list<ElementType>& InputString);

    inline void Insert(int_max Index, const std::basic_string<ElementType>& InputString);

    inline void Insert(int_max Index, const ElementType* InputString);

	inline void Insert(int_max Index, const BasicString<ElementType>& InputString);

    //------------------------------------------------------------------

    inline void operator+=(const std::basic_string<ElementType>& InputString);

	inline void operator+=(const BasicString<ElementType>& InputString);

	inline void operator+=(const ElementType* InputString);

private:

};

//------------------------------- non-member function -----------------------------------------------------------//

template<typename ElementType>
inline
BasicString<ElementType> operator+(const BasicString<ElementType>& StringA, const BasicString<ElementType>& StringB);

template<typename ElementType>
inline
BasicString<ElementType> operator+(const BasicString<ElementType>& StringA, const std::basic_string<ElementType>& StringB);

template<typename ElementType>
inline
BasicString<ElementType> operator+(const std::basic_string<ElementType>& StringA, const BasicString<ElementType>& StringB);

template<typename ElementType>
inline
BasicString<ElementType> operator+(const BasicString<ElementType>& StringA, const ElementType* StringB);

template<typename ElementType>
inline
BasicString<ElementType> operator+(const ElementType* StringA, const BasicString<ElementType>& StringB);

template<typename ElementType>
inline
bool operator==(const BasicString<ElementType>& StringA, const BasicString<ElementType>& StringB);

template<typename ElementType>
inline
bool operator==(const BasicString<ElementType>& StringA, const std::basic_string<ElementType>& StringB);

template<typename ElementType>
inline
bool operator==(const std::basic_string<ElementType>& StringA, const BasicString<ElementType>& StringB);

template<typename ElementType>
inline
bool operator==(const BasicString<ElementType>& StringA, const ElementType* StringB);

template<typename ElementType>
inline
bool operator==(const ElementType* StringA, const BasicString<ElementType>& StringB);

template<typename ElementType>
inline
bool operator!=(const BasicString<ElementType>& StringA, const BasicString<ElementType>& StringB);

template<typename ElementType>
inline
bool operator!=(const BasicString<ElementType>& StringA, const std::basic_string<ElementType>& StringB);

template<typename ElementType>
inline
bool operator!=(const std::basic_string<ElementType>& StringA, const BasicString<ElementType>& StringB);

template<typename ElementType>
inline
bool operator!=(const BasicString<ElementType>& StringA, const ElementType* StringB);

template<typename ElementType>
inline
bool operator!=(const ElementType* StringA, const BasicString<ElementType>& StringB);

template <typename ElementType, typename Traits = std::char_traits<ElementType>>
inline
std::basic_ostream<ElementType, Traits>&  operator<<(std::basic_ostream<ElementType, Traits>& os, const BasicString<ElementType>& str);

}//end namespace mdk

#include "mdkString.hpp"

#endif
