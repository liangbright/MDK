#pragma once

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


inline String::String(const String& InputString)
{
	this->Copy(InputString);
}


inline String::String(String&& InputString) noexcept
{
	m_StringData = std::move(InputString.m_StringData);
}


inline String::~String()
{
}


inline void String::operator=(const String& InputString)
{
    this->Copy(InputString);
}


inline void String::operator=(String&& InputString)
{
	this->Copy(std::move(InputString));
}


inline void String::operator=(char Element)
{
    auto ElementCount = this->GetCharCount();
    if (ElementCount == 0)
    {
        this->Resize(1);
        (*this)[0] = Element;
    }
    else if (ElementCount == 1)
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
        MDK_Warning("A String try to Copy itself @ String::Copy(const String& InputString)")
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
		if (InputString.GetCharPointer() == this->GetCharPointer())
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


inline void String::Copy(const char* InputElementPointer)
{
    if (InputElementPointer == nullptr)
    {		
		this->Clear();
    }

    // if this String is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (InputElementPointer == this->GetCharPointer())
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


inline void String::Copy(String&& InputString)
{
	// if same String
	if (this == &InputString)
	{
		MDK_Warning("A String try to copy itself @ String::Copy(&& InputString)")
		return;
	}

	if (InputString.IsEmpty() == true)
	{
		this->Clear();
		return;
	}

	// if same String
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


inline void String::Fill(char Element)
{
    auto SelfLength = this->GetCharCount();
    if (SelfLength <= 0)
    {
        MDK_Error("Self is empty @ String::Fill(...)")
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
        MDK_Warning("A String try to Share itself @ String::Share(...)")
        return;
    }

    m_StringData = InputString.m_StringData; // std::Shared_ptr, self assignment test is not necessary
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


inline void String::Swap(String& InputString)
{
    if (this == &InputString)
    {
        MDK_Warning("A String try to Swap with itself @ String::Swap(InputString)")
        return;
    }
    m_StringData.swap(InputString.m_StringData); // shared_ptr self swap check is not necessary
}

inline void String::Clear(MDK_Symbol_PureEmpty&)
{
	m_StringData.reset();
}

inline void String::Clear()
{
	if (m_StringData)
	{
		m_StringData->clear();
	}
}

inline void String::Recreate()
{
	m_StringData = std::make_shared<std::basic_string<char>>();
}

inline void String::Resize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid Input: < 0 @ String::Resize(int_max InputLength)")
        return;
    }

try
{
	if (!m_StringData)
	{
		m_StringData = std::make_shared<std::basic_string<char>>();
	}

    auto SelfLength = this->GetCharCount();
	if (InputLength != SelfLength)
	{
		m_StringData->resize(InputLength);
	}
}
catch (...)
{
    MDK_Error("Out of Memory @ String::Resize(int_max InputLength)")
}
}


inline void String::FastResize(int_max InputLength)
{
    if (InputLength < 0)
    {
        MDK_Error("Invalid input @ String::FastResize(int_max InputLength)")
        return;    
    }

try
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

	auto SelfLength = this->GetCharCount();
	
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
}
}


inline void String::ReserveCapacity(int_max InputElementCount)
{

try
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

    auto SelfLength = this->GetCharCount();

    if (InputElementCount > SelfLength)
    {
        m_StringData->reserve(InputElementCount);
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ String::ReserveCapacity(int_max InputElementCount)")
}
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


inline bool String::IsSharedWith(const String& InputString) const
{
	if (this->GetCharPointer() != nullptr)
	{
		return (this->GetCharPointer() == InputString.GetCharPointer());
	}
	return false;
}


inline int_max String::GetCharCount() const
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
		return BeginPtr + this->GetCharCount();
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
		return BeginPtr + this->GetCharCount();
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


inline void String::Append(char Element)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
try
{
	m_StringData->append(1, Element);
}
catch (...)
{
	MDK_Error("Exceed max length @ String::Append(...)")
}	
}


inline void String::Append(const char* InputString)
{
    if (InputString == nullptr)
    {
        MDK_Warning("Input is nullptr @ String::Append(...)")
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

try
{
	m_StringData->append(InputString);
}
catch (...)
{
	MDK_Error("Exceed max length @ String::Append(...)")
}
}


inline void String::Append(const std::basic_string<char>& InputString)
{
	if (InputString.size() == 0)
	{
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}

try
{
	m_StringData->append(InputString);
}
catch (...)
{
	MDK_Error("Exceed max length @ String::Append(...)")
}
}


inline void String::Append(const String& InputString)
{
	if (InputString.IsEmpty() == true)
	{
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0);
	}
try
{
	m_StringData->append(*InputString.m_StringData);
}
catch (...)
{
	MDK_Error("Exceed max length @ String::Append(...)")
}	
}


inline void String::Delete(int_max Index)
{
    this->Delete(&Index, 1);
}


inline void String::Delete(const std::initializer_list<int_max>& IndexList)
{
	this->Delete(IndexList.begin(), int_max(IndexList.size()));
}


inline void String::Delete(const int_max* IndexList, int_max ListLength)
{
	if (IndexList == nullptr || ListLength <= 0)
    {
        MDK_Warning("Empty Input @ String::Delete(const int_max* IndexList, int_max ListLength)")
        return;
    }

	auto SelfLength = this->GetCharCount();
	if (SelfLength == 0)
	{
		MDK_Error("Self is empty @ String::Delete(const int_max* IndexList, int_max ListLength)")
		return;
	}

	for (auto it = IndexList; it != IndexList + ListLength; ++it)
    {
        if (*it >= SelfLength || *it < 0)
        {
            MDK_Error("Out of bound Input @ String::Delete(const int_max* IndexList, int_max ListLength)")
            return;
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
}


inline void String::Delete(int_max Index_start, int_max Index_end)
{
    if (this->IsEmpty() == true)
    {
        MDK_Error("Self is empty @ String::Delete(int_max Index_start, int_max Index_end)")
        return;
    }

	auto SelfLength = this->GetCharCount();
    if (Index_end < Index_start 
        || Index_start >= SelfLength || Index_start < 0
        || Index_end >= SelfLength || Index_end < 0 )
    {
        MDK_Error("Invalid Input @ String::Delete(int_max Index_start, int_max Index_end)")
        return;
    }

    m_StringData->erase(m_StringData->begin() + Index_start, m_StringData->begin() + Index_end + 1);
}


inline void String::Insert(int_max Index, char Element)
{
	auto SelfLength = this->GetCharCount();
	if (SelfLength == 0)
	{
		if (Index != 0)
		{
			MDK_Error("Invalid Input @ String::Insert(int_max Index, char Element)")
			return;
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
			return;
		}
	}
    m_StringData->insert(Index, 1, Element);
}


inline void String::Insert(int_max Index, const char* InputString)
{
    auto SelfLength = this->GetCharCount();
    if (SelfLength == 0)
    {
        if (Index != 0)
        {
            MDK_Error("Invalid Input @ String::Insert(Index, const char* InputString)")
            return;
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
            return;
        }
    }

	if (InputString != nullptr)
	{
		m_StringData->insert(Index, InputString);
	}
}


inline void String::Insert(int_max Index, const std::basic_string<char>& InputString)
{
	auto SelfLength = this->GetCharCount();
	if (SelfLength == 0)
	{
		if (Index != 0)
		{
			MDK_Error("Invalid Input @ String::Insert(...)")
			return;
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
			return;
		}
	}

	if (InputString.size() > 0)
	{
		m_StringData->insert(Index, InputString);
	}
}


inline void String::Insert(int_max Index, const String& InputString)
{
	auto SelfLength = this->GetCharCount();
	if (SelfLength == 0)
	{
		if (Index != 0)
		{
			MDK_Error("Invalid Input @ String::Insert(...)")
			return;
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
			return;
		}
	}

	if (InputString.IsEmpty() == false)
	{
		m_StringData->insert(Index, (*InputString.m_StringData));
	}
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
