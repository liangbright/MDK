#ifndef __mdkString_hpp
#define __mdkString_hpp


namespace mdk
{
 
template<typename ElementType>
inline
String<ElementType>::String()
{
    MDK_Check_ElementType_Of_String<ElementType>();

    this->Resize(0);
}



template<typename ElementType>
inline
String<ElementType>::String(const ElementType& Element)
{
    MDK_Check_ElementType_Of_String<ElementType>();

    this->Resize(1);

    (*this)(0) = Element;
}


template<typename ElementType>
inline
String<ElementType>::String(const std::initializer_list<ElementType>& InputString)
{
    this->Resize(0);

    (*m_StringData) = InputString;
}


template<typename ElementType>
inline
String<ElementType>::String(const ElementType* InputString)
{
    this->Resize(0);

    (*m_StringData) = InputString;
}


template<typename ElementType>
inline String<ElementType>::String(const std::basic_string<ElementType>& InputString)
{
    this->Resize(0);

    (*m_StringData) = InputString;
}


template<typename ElementType>
inline
String<ElementType>::String(const String<ElementType>& InputString, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy)
{
    MDK_Check_ElementType_Of_String<ElementType>();

    if (Method == ObjectConstructionTypeEnum::Copy)
    {
        this->Resize(0);

        this->Copy(InputString);
    }
    else
    {
        this->ForceShare(InputString);
    }
}


// move constructor
template<typename ElementType>
inline
String<ElementType>::String(String<ElementType>&& InputString) noexcept
{
    m_StringData = std::move(InputString.m_StringData);
}


template<typename ElementType>
inline
String<ElementType>::~String()
{
}


template<typename ElementType>
inline
void String<ElementType>::operator=(const String<ElementType>& InputString)
{
    this->Copy(InputString);
}


// move assignment operator
template<typename ElementType>
inline
void String<ElementType>::operator=(String<ElementType>&& InputString)
{
    this->Take(std::forward<String<ElementType>&>(InputString));
}


template<typename ElementType>
inline
void String<ElementType>::operator=(const ElementType& Element)
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0)
    {
        this->Resize(1);

        (*this)[0] = Element;
    }
    else if (ElementNumber == 1)
    {
        (*this)[0] = Element;
    }
    else
    {
        this->Clear();

        this->Resize(1);

        (*this)[0] = Element;
    }
}


template<typename ElementType>
inline
void String<ElementType>::operator=(const std::initializer_list<ElementType>& InputString)
{
    (*m_StringData) = InputString;
}


template<typename ElementType>
inline 
void String<ElementType>::operator=(const ElementType* InputString)
{
    (*m_StringData) = InputString;
}


template<typename ElementType>
inline
void String<ElementType>::operator=(const std::basic_string<ElementType>& InputString)
{
    (*m_StringData) = InputString;
}


template<typename ElementType>
inline
void String<ElementType>::Copy(const String<ElementType>& InputString)
{
    if (this == &InputString)
    {
        MDK_Warning("A String tries to Copy itself @ String::Copy(InputString)")
        return;
    }

    if (InputString.IsEmpty() == true)
    {
        this->Clear();
        return;
    }

    // copy data
    this->Copy(InputString.GetElementPointer());
}



template<typename ElementType>
inline std::basic_string<ElementType>& String<ElementType>::StdString()
{
    return (*m_StringData);
}


template<typename ElementType>
inline const std::basic_string<ElementType>& String<ElementType>::StdString() const
{
    return (*m_StringData);
}


template<typename ElementType>
inline
void String<ElementType>::Copy(const String<ElementType>* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Input is nullptr @ String::Copy(String* InputString)")
        return;
    }

    this->Copy(*InputString);
}


template<typename ElementType>
inline
void String<ElementType>::Copy(const ElementType* InputElementPointer)
{
    if (InputElementPointer == nullptr)
    {
        MDK_Error("Input pointer is nullptr @ String::Copy(ElementType*)")
        return;
    }

    // if this String is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (std::size_t(InputElementPointer) == std::size_t(this->GetElementPointer()))
        {
            return;
        }
    }

    (*m_StringData) = InputElementPointer;
}


template<typename ElementType>
inline
void String<ElementType>::Fill(const ElementType& Element)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ String::Fill")
        return;
    }

    auto BeginPointer = this->GetElementPointer();

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfLength; ++Ptr)
    {
        Ptr[0] = Element;
    }
}


template<typename ElementType>
inline
void String<ElementType>::Share(String<ElementType>& InputString)
{
    // String = String
    if (this == &InputString)
    {
        MDK_Warning("A String tries to Share itself @ String::Share(InputString)")
        return;
    }

    //--------------------------------------------------------------------------------------------------------

    m_StringData = InputString.m_StringData; // std::Shared_ptr, self assignment test is not necessary
}


template<typename ElementType>
inline
void String<ElementType>::Share(String<ElementType>* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Input is nullptr @ String::Share(mdkString* InputString)")
        return;
    }

    this->Share(*InputString);
}


template<typename ElementType>
inline
void String<ElementType>::ForceShare(const String<ElementType>& InputString)
{
    // String = String
    if (this == &InputString)
    {
        MDK_Warning("A String tries to ForceShare itself @ String::ForceShare(InputString)")
        return;
    }

    m_StringData = InputString.m_StringData; // std::Shared_ptr, self assignment check is not necessary
}


template<typename ElementType>
inline
void String<ElementType>::ForceShare(const String<ElementType>* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Input is nullptr @ String::ForceShare(mdkString* InputString)")
        return;
    }

    this->ForceShare(*InputString);
}


template<typename ElementType>
inline
void String<ElementType>::Take(String<ElementType>&& InputString)
{
    this->Take(std::forward<String<ElementType>&>(InputString));
}


template<typename ElementType>
inline
void String<ElementType>::Take(String<ElementType>& InputString)
{
    // String = String
    if (this == &InputString)
    {
        MDK_Warning("A String tries to take itself @ String::take(InputString)")
        return;
    }

    if (InputString.IsEmpty() == true)
    {
        this->Clear();

        return;
    }

    // StringA = StringA
    if (this->GetElementPointer() == InputString.GetElementPointer())
    {
        return;
    }

    // now, InputString is not empty, and is not self
    
    //note: m_StringData.swap(InputString.m_StringData) will invalidate Share()

    (*m_StringData) = std::move(*InputString.m_StringData);

    // Clear InputString to be empty
    InputString.Clear();
}


template<typename ElementType>
inline
void String<ElementType>::Take(String<ElementType>* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Input is nullptr @ String::Take(mdkString* InputString)")
        return;
    }

    this->Take(*InputString);
}


template<typename ElementType>
inline
void String<ElementType>::SwapSmartPointer(String<ElementType>& InputString)
{
    // String = String
    if (this == &InputString)
    {
        MDK_Warning("A String tries to Swap with itself @ String::SwapSmartPointer(InputString)")
        return;
    }

    m_StringData.swap(InputString.m_StringData); // shared_ptr self swap check is not necessary
}


template<typename ElementType>
inline
void String<ElementType>::Clear()
{
    m_StringData->clear();
}


template<typename ElementType>
inline 
bool String<ElementType>::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid Input: negtive @ String::Resize(int_max InputLength)")
        return false;
    }

try
{
    //--------initialize the String data ----------------------------------------
    if (!m_StringData)
    {
        m_StringData = std::make_shared<std::basic_string<ElementType>>();
    }
    //-------------------------------------------------------------------------

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

    m_StringData->resize(InputLength);
}
catch (...)
{
    MDK_Error("Out of Memory @ String::Resize(int_max InputLength)")

    return false;
}
    return true;
}


template<typename ElementType>
inline
bool String<ElementType>::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ String::FastResize(int_max InputLength)")
        return false;    
    }

    auto SelfLength = this->GetElementNumber();

    if (InputLength == SelfLength)
    {
        return true;
    }

try
{
    if (InputLength != SelfLength)
    {
        if (InputLength > int_max(m_StringData->capacity()))
        {
            m_StringData->clear();
        }

        m_StringData->resize(InputLength);
    }   
}
catch (...)
{
    MDK_Error("Out of Memory @ String::FastResize(int_max InputLength)")

    return false;
}
    return true;
}



template<typename ElementType>
inline
bool String<ElementType>::ReserveCapacity(int_max InputElementNumber)
{

try
{
    auto SelfLength = this->GetElementNumber();

    if (InputElementNumber > SelfLength)
    {
        m_StringData->reserve(InputElementNumber);
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ String::ReserveCapacity(int_max InputElementNumber)")
    return false;
}
    
    return true;
}


template<typename ElementType>
inline
void String<ElementType>::Squeeze()
{
    m_StringData->shrink_to_fit();
}


template<typename ElementType>
inline
bool String<ElementType>::IsEmpty() const
{
    return (m_StringData->size() <= 0);
}


template<typename ElementType>
inline
bool String<ElementType>::IsShared() const
{
    return (m_StringData.use_count() > 1);
}


template<typename ElementType>
inline
int_max String<ElementType>::GetElementNumber() const
{
    return int_max(m_StringData->size());
}


template<typename ElementType>
inline
ElementType* String<ElementType>::GetElementPointer()
{
    return const_cast<ElementType*>(m_StringData->data());
}


template<typename ElementType>
inline
const ElementType* String<ElementType>::GetElementPointer() const
{
    return m_StringData->data();
}


//----------- Get/Set String(Index) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& String<ElementType>::operator[](int_max Index)
{
    return (*m_StringData)[Index];
}


template<typename ElementType>
inline
const ElementType& String<ElementType>::operator[](int_max Index) const
{
    return (*m_StringData)[Index];
}


template<typename ElementType>
inline
ElementType& String<ElementType>::operator()(int_max Index)
{
    return (*m_StringData)[Index];
}


template<typename ElementType>
inline
const ElementType& String<ElementType>::operator()(int_max Index) const
{
    return (*m_StringData)[Index];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& String<ElementType>::at(int_max Index)
{
    return m_StringData->at(Index);
}


template<typename ElementType>
inline
const ElementType& String<ElementType>::at(int_max Index) const
{
    return m_StringData->at(Index);
}


template<typename ElementType>
inline
void String<ElementType>::Append(const ElementType& Element)
{
    m_StringData->append(1, Element);
}


template<typename ElementType>
inline 
void String<ElementType>::Append(const std::initializer_list<ElementType>& InputString)
{
    m_StringData->append(InputString);
}


template<typename ElementType>
inline
void String<ElementType>::Append(const std::vector<ElementType>& InputString)
{
    m_StringData->Append(InputString.data(), InputString.data() + InputString.size());
}


template<typename ElementType>
inline
void String<ElementType>::Append(const DenseMatrix<ElementType>& InputString)
{
    if (InputString.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ String::Append(const DenseMatrix<ElementType>& InputString)")
        return false;
    }

    m_StringData->Append(InputString.GetElementPointer(), InputString.GetElementPointer() + InputString.GetElementNumber());
}


template<typename ElementType>
inline
void String<ElementType>::Append(const DataContainer<ElementType>& InputString)
{
    m_StringData->Append(InputString.GetElementPointer(), InputString.GetElementPointer() + InputString.GetElementNumber());
}


template<typename ElementType>
inline
void String<ElementType>::Append(const String<ElementType>& InputString)
{
    m_StringData->Append(*InputString.m_StringData);
}


template<typename ElementType>
inline 
void String<ElementType>::Append(const ElementType* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Invalid Input: empty @ String::Append(const ElementType* InputString)")
        return;
    }

    m_StringData->Append(InputString);
}


template<typename ElementType>
inline 
void String<ElementType>::Delete(int_max Index)
{
    this->Delete(&Index, 1);
}


template<typename ElementType>
inline
void String<ElementType>::Delete(const std::initializer_list<int_max>& IndexList)
{
    this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
void String<ElementType>::Delete(const std::vector<int_max>& IndexList)
{    
    this->Delete(IndexList.data(), int_max(IndexList.size()));
}


template<typename ElementType>
inline
void String<ElementType>::Delete(const DenseMatrix<int_max>& IndexList)
{
    if (IndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ String::Delete(const DenseMatrix<int_max>& IndexList)")
        return;
    }

    this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
void String<ElementType>::Delete(const DataContainer<int_max>& IndexList)
{
    this->Delete(IndexList.GetElementPointer(), IndexList.GetElementNumber());
}


template<typename ElementType>
inline
void String<ElementType>::Delete(const int_max* IndexList, int_max ListLength)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        MDK_Error("Self is empty @ String::Delete(const int_max* IndexList, int_max ListLength)")
        return;
    }

    if (IndexList == nullptr || Length <= 0)
    {
        MDK_Error("Empty Input @ String::Delete(const int_max* IndexList, int_max ListLength)")
        return;
    }

    for (auto it = IndexList; it != IndexList + Length; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ String::Delete(const int_max* IndexList, int_max ListLength)")
            return;
        }
    }

    std::vector<int_max> IndexList_max_to_min(Length);

    for (int_max i = 0; i < Length; ++i)
    {
        IndexList_max_to_min[i] = IndexList[i];
    }

    std::sort(IndexList_max_to_min.begin(), IndexList_max_to_min.end(), [](int_max a, int_max b) { return a > b; });

    int_max Index_prev = -1;

    for (int_max i = 0; i < int_max(IndexList_max_to_min.size()); ++i)
    {
        auto Index_i = IndexList_max_to_min[i];

        if (Index_i == Index_prev)
        {
            MDK_Warning("duplicate index @ String::Delete(const int_max* IndexPtr, int_max Length)")
        }
        else
        {
            m_StringData->erase(m_StringData->begin() + Index_i, m_StringData->begin() + Index_i + 1);

            Index_prev = Index_i;
        }
    }
}


template<typename ElementType>
inline 
void String<ElementType>::Delete(int_max Index_start, int_max Index_end)
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ String::Delete(int_max Index_start, int_max Index_end)")
        return;
    }

    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ String::Delete(int_max Index_start, int_max Index_end)")
        return;
    }

    m_StringData->erase(m_StringData->begin() + Index_start, m_StringData->begin() + Index_end + 1);
}


template<typename ElementType>
inline 
void String<ElementType>::Insert(int_max Index, const ElementType& Element)
{
    m_StringData->Insert(Index, 1, Element);
}


template<typename ElementType>
inline
void String<ElementType>::Insert(int_max Index, const std::initializer_list<ElementType>& InputString)
{
    m_StringData->Insert(m_StringData->begin() + Index, InputString);
}


template<typename ElementType>
inline
void String<ElementType>::Insert(int_max Index, const std::vector<ElementType>& InputString)
{
    auto Ptr = InputString.data();
    auto Number = InputString.size();
    m_StringData->Insert(m_StringData->begin() + Index, Ptr, Ptr + Number);
}


template<typename ElementType>
inline
void String<ElementType>::Insert(int_max Index, const DenseMatrix<ElementType>& InputString)
{
    if (DenseMatrix.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ String::Insert(Index, DenseMatrix)")
        return;
    }

    auto Ptr = InputString.GetElementPointer();
    auto Number = InputString.GetElementNumber();
    m_StringData->Insert(m_StringData->begin() + Index, Ptr, Ptr + Number);
}


template<typename ElementType>
inline
void String<ElementType>::Insert(int_max Index, const DataContainer<ElementType>& InputString)
{
    auto Ptr = InputString.GetElementPointer();
    auto Number = InputString.GetElementNumber();
    m_StringData->Insert(m_StringData->begin() + Index, Ptr, Ptr + Number);
}


template<typename ElementType>
inline
void String<ElementType>::Insert(int_max Index, const String<ElementType>& InputString)
{
    m_StringData->Insert(Index, (*InputString.m_StringData));
}


template<typename ElementType>
inline 
void Insert(int_max Index, const std::basic_string<ElementType>& InputString)
{
    m_StringData->Insert(Index, InputString);
}


template<typename ElementType>
inline
void String<ElementType>::Insert(int_max Index, const ElementType* InputString)
{
    auto SelfLength = this->GetElementNumber();

    if (SelfLength == 0)
    {
        if (Index != 0 || InputString == nullptr)
        {
            MDK_Error("Invalid Input @ String::Insert(Index, const ElementType* InputString)")
            return;
        }
    }
    else
    {
        if (Index >= SelfLength || Index < 0 || InputString == nullptr)
        {
            MDK_Error("Invalid Input @ String::Insert(Index, const ElementType* InputString)")
            return;
        }
    }

    m_StringData->insert(Index, InputString);
}


template<typename ElementType>
inline
void String<ElementType>::operator+=(const String<ElementType>& InputString)
{
    (*m_StringData) += (*InputString.m_StringData);
}


template<typename ElementType>
inline 
void String<ElementType>::operator+=(const std::basic_string<ElementType>& InputString)
{
    (*m_StringData) += InputString;
}


template<typename ElementType>
inline 
void String<ElementType>::operator+=(const ElementType* InputString)
{
    (*m_StringData) += InputString;
}

//------------------------------- non-member function -----------------------------------------------------------//

template<typename ElementType>
inline
String<ElementType> operator+(const String<ElementType>& StringA, const String<ElementType>& StringB)
{
    String<ElementType> StringC;

    StringC.StdString() = StringA.StdString() + StringB.StdString();

    return StringC;
}


template<typename ElementType>
inline
String<ElementType> operator+(const String<ElementType>& StringA, const std::basic_string<ElementType>& StringB)
{
    String<ElementType> StringC;

    StringC.StdString() = StringA.StdString() + StringB;

    return StringC;
}


template<typename ElementType>
inline
String<ElementType> operator+(const std::basic_string<ElementType>& StringA, const String<ElementType>& StringB)
{
    String<ElementType> StringC;

    StringC.StdString() = StringA + StringB.StdString();

    return StringC;
}


template<typename ElementType>
inline
String<ElementType> operator+(const String<ElementType>& StringA, const ElementType* StringB)
{
    String<ElementType> StringC;

    StringC.StdString() = StringA.StdString() + StringB;

    return StringC;
}


template<typename ElementType>
inline
String<ElementType> operator+(const ElementType* StringA, const String<ElementType>& StringB)
{
    String<ElementType> StringC;

    StringC.StdString() = StringA + StringB.StdString();

    return StringC;
}


template<typename ElementType>
inline
bool operator==(const String<ElementType>& StringA, const String<ElementType>& StringB)
{
    return StringA.StdString() == StringB.StdString();
}


template<typename ElementType>
inline
bool operator==(const String<ElementType>& StringA, const std::basic_string<ElementType>& StringB)
{
    return StringA.StdString() == StringB;
}


template<typename ElementType>
inline
bool operator==(const std::basic_string<ElementType>& StringA, const String<ElementType>& StringB)
{
    return StringA == StringB.StdString();
}


template<typename ElementType>
inline
bool operator==(const String<ElementType>& StringA, const ElementType* StringB)
{
    return StringA.StdString() == StringB;
}


template<typename ElementType>
inline
bool operator==(const ElementType* StringA, const String<ElementType>& StringB)
{
    return StringA == StringB.StdString();
}


template<typename ElementType>
inline
bool operator!=(const String<ElementType>& StringA, const String<ElementType>& StringB)
{
    return StringA.StdString() != StringB.StdString();
}


template<typename ElementType>
inline
bool operator!=(const String<ElementType>& StringA, const std::basic_string<ElementType>& StringB)
{
    return StringA.StdString() != StringB;
}


template<typename ElementType>
inline
bool operator!=(const std::basic_string<ElementType>& StringA, const String<ElementType>& StringB)
{
    return StringA != StringB.StdString();
}


template<typename ElementType>
inline
bool operator!=(const String<ElementType>& StringA, const ElementType* StringB)
{
    return StringA.StdString() != StringB;
}


template<typename ElementType>
inline
bool operator!=(const ElementType* StringA, const String<ElementType>& StringB)
{
    return StringA != StringB.StdString();
}


template <typename ElementType, typename Traits>
inline
std::basic_ostream<ElementType, Traits>& operator<<(std::basic_ostream<ElementType, Traits>& os, const String<ElementType>& str)
{
    return operator<<(os, str.StdString());
}


}//end namespace mdk

#endif