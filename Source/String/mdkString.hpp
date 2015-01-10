#ifndef __mdkString_hpp
#define __mdkString_hpp

#include "mdkDenseMatrix.h"

namespace mdk
{
 
inline String::String()
{
    this->Resize(0);
}


inline String::String(char Element)
{
    this->Resize(1);
    (*this)(0) = Element;
}


inline String::String(const char* InputString)
{
    this->Resize(0);
    (*m_StringData) = InputString;
}


inline String::String(const std::basic_string<char>& InputString)
{
    this->Resize(0);
    (*m_StringData) = InputString;
}


inline String::String(const String& InputString, ObjectConstructionTypeEnum Method)
{
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
inline String::String(String&& InputString) noexcept
{
	(*this) = std::move(InputString);
}


inline String::~String()
{
}


inline void String::operator=(const String& InputString)
{
    this->Copy(InputString);
}


// move assignment operator
inline void String::operator=(String&& InputString)
{
	m_StringData = std::move(InputString.m_StringData);
}


inline void String::operator=(char Element)
{
    auto ElementNumber = this->GetCharNumber();
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


inline void String::operator=(const char* InputString)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
    (*m_StringData) = InputString;
}


inline void String::operator=(const std::basic_string<char>& InputString)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
    (*m_StringData) = InputString;
}


inline std::basic_string<char>& String::StdString()
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
	return (*m_StringData);
}


inline const std::basic_string<char>& String::StdString() const
{
	if (this->IsPureEmpty() == true)
	{
		const_cast<String*>(this)->Resize(0);
	}
	return (*m_StringData);
}


inline void String::Copy(const String& InputString)
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

	// if this String is not empty, check if this and Input Share the same data
	if (this->IsEmpty() == false)
	{
		if (std::size_t(InputString.GetCharPointer()) == std::size_t(this->GetCharPointer()))
		{
			return;
		}
	}
	else
	{
		if (this->IsPureEmpty() == true)
		{
			this->Resize(0);
		}
	}
	*m_StringData = *InputString.m_StringData;
}


inline void String::Copy(const String* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Input is nullptr @ String::Copy(String* InputString)")
        return;
    }
    this->Copy(*InputString);
}


inline void String::Copy(const char* InputElementPointer)
{
    if (InputElementPointer == nullptr)
    {
        MDK_Error("Input pointer is nullptr @ String::Copy(char*)")
        return;
    }

    // if this String is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (std::size_t(InputElementPointer) == std::size_t(this->GetCharPointer()))
        {
            return;
        }
    }
	else
	{
		if (this->IsPureEmpty() == true)
		{
			this->Resize(0);
		}
	}
    (*m_StringData) = InputElementPointer;
}


inline void String::Fill(char Element)
{
    auto SelfLength = this->GetCharNumber();
    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ String::Fill")
        return;
    }
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
	auto BeginPointer = this->GetCharPointer();
    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfLength; ++Ptr)
    {
        Ptr[0] = Element;
    }
}


inline void String::Share(String& InputString)
{
    // String = String
    if (this == &InputString)
    {
        MDK_Warning("A String tries to Share itself @ String::Share(InputString)")
        return;
    }

    m_StringData = InputString.m_StringData; // std::Shared_ptr, self assignment test is not necessary
}


inline void String::Share(String* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Input is nullptr @ String::Share(String* InputString)")
        return;
    }
    this->Share(*InputString);
}


inline void String::ForceShare(const String& InputString)
{
    // String = String
    if (this == &InputString)
    {
        MDK_Warning("A String tries to ForceShare itself @ String::ForceShare(InputString)")
        return;
    }

    m_StringData = InputString.m_StringData; // std::Shared_ptr, self assignment check is not necessary
}


inline void String::ForceShare(const String* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Input is nullptr @ String::ForceShare(mdkString* InputString)")
        return;
    }
    this->ForceShare(*InputString);
}


inline void String::Take(String&& InputString)
{
    this->Take(std::forward<String&>(InputString));
}


inline void String::Take(String& InputString)
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
	if (this->GetCharPointer() == InputString.GetCharPointer())
    {
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

    // now, InputString is not empty, and is not self
    
    //note: m_StringData.swap(InputString.m_StringData) will invalidate Share()

    (*m_StringData) = std::move(*InputString.m_StringData);

    // Clear InputString to be empty
    InputString.Clear();
}


inline void String::Take(String* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Error("Input is nullptr @ String::Take(mdkString* InputString)")
        return;
    }
    this->Take(*InputString);
}


inline void String::Swap(String& InputString)
{
    // String = String
    if (this == &InputString)
    {
        MDK_Warning("A String tries to Swap with itself @ String::Swap(InputString)")
        return;
    }
    m_StringData.swap(InputString.m_StringData); // shared_ptr self swap check is not necessary
}


inline void String::Clear()
{
	if (m_StringData)
	{
		m_StringData->clear();
	}
}


inline bool String::Resize(int_max InputLength)
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
        m_StringData = std::make_shared<std::basic_string<char>>();
    }
    //-------------------------------------------------------------------------

    auto SelfLength = this->GetCharNumber();

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


inline bool String::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ String::FastResize(int_max InputLength)")
        return false;    
    }

    auto SelfLength = this->GetCharNumber();
    if (InputLength == SelfLength)
    {
        return true;
    }

try
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

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


inline bool String::ReserveCapacity(int_max InputElementNumber)
{

try
{
	if (this->IsPureEmpty()==true)
	{
		this->Resize(0);
	}

    auto SelfLength = this->GetCharNumber();

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


inline void String::ReleaseUnusedCapacity()
{
	if (m_StringData)
	{
		m_StringData->shrink_to_fit();
	}
}


inline bool String::IsEmpty() const
{
	if (m_StringData)
	{
		return (m_StringData->size() <= 0);
	}
	else
	{
		return true;
	}
}


inline bool String::IsPureEmpty() const
{
	return (!m_StringData);
}


inline bool String::IsShared() const
{
	if (m_StringData)
	{
		return (m_StringData.use_count() > 1);
	}
	else
	{
		return false;
	}
}


inline int_max String::GetCharNumber() const
{
	if (m_StringData)
	{
		return int_max(m_StringData->size());
	}
	else
	{
		return 0;
	}
}


inline char* String::GetCharPointer()
{
	if (m_StringData)
	{
		return const_cast<char*>(m_StringData->data());
	}
	else
	{
		return nullptr;
	}
}


inline const char* String::GetCharPointer() const
{
	if (m_StringData)
	{
		return m_StringData->data();
	}
	else
	{
		return nullptr;
	}
}

inline char* String::begin()
{
	return this->GetCharPointer();
}

inline const char* String::begin() const
{
	return this->GetCharPointer();
}

inline char* String::end()
{
	auto BeginPtr = this->begin();
	if (BeginPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		return BeginPtr + this->GetCharNumber();
	}
}

inline const char* String::end() const
{
	auto BeginPtr = this->begin();
	if (BeginPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		return BeginPtr + this->GetCharNumber();
	}
}


//----------- Get/Set String(Index) -----------------------------------//

// operator[] (): no bound check in release mode

inline char& String::operator[](int_max Index)
{
    return (*m_StringData)[Index];
}


inline char String::operator[](int_max Index) const
{
    return (*m_StringData)[Index];
}


inline char& String::operator()(int_max Index)
{
    return (*m_StringData)[Index];
}


inline char String::operator()(int_max Index) const
{
    return (*m_StringData)[Index];
}

// at(): bound check

inline char& String::at(int_max Index)
{
    return m_StringData->at(Index);
}


inline char String::at(int_max Index) const
{
    return m_StringData->at(Index);
}


inline bool String::Append(char Element)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
try
{
	m_StringData->append(1, Element);
	return true;
}
catch (...)
{
	MDK_Error("Exceed max length @ String::Append(...)")
	return false;
}	
}


inline bool String::Append(const char* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Warning("nullptr Input @ String::Append(...)")
        return true;
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

try
{
	m_StringData->append(InputString);
	return true;
}
catch (...)
{
	MDK_Error("Exceed max length @ String::Append(...)")
	return false;
}
}


inline bool String::Append(const std::basic_string<char>& InputString)
{
	if (InputString.size() == 0)
	{
		return true;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

try
{
	m_StringData->append(InputString);
	return true;
}
catch (...)
{
	MDK_Error("Exceed max length @ String::Append(...)")
	return false;
}
}


inline bool String::Append(const String& InputString)
{
	if (InputString.GetCharNumber() == 0)
	{
		return true;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
try
{
	m_StringData->append(*InputString.m_StringData);
	return true;
}
catch (...)
{
	MDK_Error("Exceed max length @ String::Append(...)")
	return false;
}	
}


inline bool String::Delete(int_max Index)
{
    return this->Delete(&Index, 1);
}


inline bool String::Delete(const std::initializer_list<int_max>& IndexList)
{
	return this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


inline bool String::Delete(const int_max* IndexList, int_max ListLength)
{
	if (IndexList == nullptr || ListLength <= 0)
    {
        MDK_Warning("Empty Input @ String::Delete(const int_max* IndexList, int_max ListLength)")
        return true;
    }

	auto SelfLength = this->GetCharNumber();
	if (SelfLength == 0)
	{
		MDK_Error("Self is empty @ String::Delete(const int_max* IndexList, int_max ListLength)")
		return false;
	}

	for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ String::Delete(const int_max* IndexList, int_max ListLength)")
            return false;
        }
    }

	std::vector<int_max> IndexList_max_to_min(ListLength);

	for (int_max i = 0; i < ListLength; ++i)
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
	return true;
}


inline bool String::Delete(int_max Index_start, int_max Index_end)
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ String::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }
	auto SelfLength = this->GetCharNumber();
    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ String::Delete(int_max Index_start, int_max Index_end)")
        return false;
    }
    m_StringData->erase(m_StringData->begin() + Index_start, m_StringData->begin() + Index_end + 1);
	return true;
}


inline bool String::Insert(int_max Index, char Element)
{
	auto SelfLength = this->GetCharNumber();
	if (SelfLength == 0)
	{
		if (Index != 0)
		{
			MDK_Error("Invalid Input @ String::Insert(int_max Index, char Element)")
			return false;
		}
		if (this->IsPureEmpty() == true)
		{
			this->Resize(0);
		}
	}
	else
	{
		if (Index >= SelfLength || Index < 0)
		{
			MDK_Error("Invalid Input @ String::Insert(int_max Index, char Element)")
			return false;
		}
	}
    m_StringData->insert(Index, 1, Element);
	return true;
}


inline bool String::Insert(int_max Index, const char* InputString)
{
    auto SelfLength = this->GetCharNumber();
    if (SelfLength == 0)
    {
        if (Index != 0)
        {
            MDK_Error("Invalid Input @ String::Insert(Index, const char* InputString)")
            return false;
        }
		if (this->IsPureEmpty() == true)
		{
			this->Resize(0);
		}
    }
    else
    {
        if (Index >= SelfLength || Index < 0)
        {
            MDK_Error("Invalid Input @ String::Insert(Index, const char* InputString)")
            return false;
        }
    }
	if (InputString != nullptr)
	{
		m_StringData->insert(Index, InputString);
	}
	return true;
}


inline bool String::Insert(int_max Index, const std::basic_string<char>& InputString)
{
	auto SelfLength = this->GetCharNumber();
	if (SelfLength == 0)
	{
		if (Index != 0)
		{
			MDK_Error("Invalid Input @ String::Insert(...)")
			return false;
		}
		if (this->IsPureEmpty() == true)
		{
			this->Resize(0);
		}
	}
	else
	{
		if (Index >= SelfLength || Index < 0)
		{
			MDK_Error("Invalid Input @ String::Insert(...)")
			return false;
		}
	}
	if (InputString.size() > 0)
	{
		m_StringData->insert(Index, InputString);
	}
	return true;
}


inline bool String::Insert(int_max Index, const String& InputString)
{
	auto SelfLength = this->GetCharNumber();
	if (SelfLength == 0)
	{
		if (Index != 0)
		{
			MDK_Error("Invalid Input @ String::Insert(...)")
			return false;
		}
		if (this->IsPureEmpty() == true)
		{
			this->Resize(0);
		}
	}
	else
	{
		if (Index >= SelfLength || Index < 0)
		{
			MDK_Error("Invalid Input @ String::Insert(...)")
			return false;
		}
	}

	if (InputString.IsEmpty() == false)
	{
		m_StringData->insert(Index, (*InputString.m_StringData));
	}
	return true;
}


inline void String::operator+=(char Element)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
	(*m_StringData) += Element;
}


inline void String::operator+=(const char* InputString)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
	(*m_StringData) += InputString;
}


inline void String::operator+=(const std::basic_string<char>& InputString)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
	(*m_StringData) += InputString;
}


inline void String::operator+=(const String& InputString)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
    (*m_StringData) += (*InputString.m_StringData);
}


}//end namespace mdk

#endif