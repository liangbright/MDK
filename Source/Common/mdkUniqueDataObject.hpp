#ifndef mdk_UniqueDataObject_hpp
#define mdk_UniqueDataObject_hpp

namespace mdk
{

template<typename DataType>
inline
UniqueDataObject<DataType>::UniqueDataObject()
{
	m_Data = std::make_unique<DataType>();
}

template<typename DataType>
inline
UniqueDataObject<DataType>::UniqueDataObject(const MDK_Symbol_PureEmpty&)
{// m_Data == nullptr
}

template<typename DataType>
inline
UniqueDataObject<DataType>::UniqueDataObject(const UniqueDataObject& InputObject)
{	
	this->Copy(InputObject);
}

template<typename DataType>
inline 
UniqueDataObject<DataType>::UniqueDataObject(UniqueDataObject&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
UniqueDataObject<DataType>::~UniqueDataObject()
{
}

template<typename DataType>
inline 
void UniqueDataObject<DataType>::operator=(const UniqueDataObject& InputObject)
{
	this->Copy(InputObject);
}

template<typename DataType>
inline
void UniqueDataObject<DataType>::operator=(UniqueDataObject&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
bool UniqueDataObject<DataType>::Copy(const UniqueDataObject& InputObject)
{
	if (!m_Data)
	{
		m_Data = std::make_unique<DataType>();
	}

	if (InputObject.m_Data)
	{		
		return m_Data->Copy(*InputObject.m_Data);
	}
	else
	{
		return m_Data->Copy(MDK_EMPTY);
	}
}


template<typename DataType>
inline 
bool UniqueDataObject<DataType>::Take(UniqueDataObject& InputObject)
{
	if (!m_Data)
	{
		m_Data = std::make_unique<DataType>();
	}

	if (InputObject.m_Data)
	{
		return m_Data->Take(std::move(*InputObject.m_Data));
	}
	else
	{
		return m_Data->Take(MDK_EMPTY);
	}
}

template<typename DataType>
inline
bool UniqueDataObject<DataType>::Take(UniqueDataObject&& InputObject)
{
	return this->Take(std::forward<UniqueDataObject&>(InputObject))
}

template<typename DataType>
inline
void UniqueDataObject<DataType>::Swap(UniqueDataObject& InputObject)
{
	m_Data.swap(InputObject.m_Data);
}

template<typename DataType>
inline
void UniqueDataObject<DataType>::Clear()
{
	if (m_Data)
	{
		m_Data->Clear();
	}
}

template<typename DataType>
inline
void UniqueDataObject<DataType>::Clear(const MDK_Symbol_PureEmpty&)
{
	m_Data.reset();
}


template<typename DataType>
inline 
void UniqueDataObject<DataType>::Create()
{
	m_Data = std::make_unique<DataType>();
}

template<typename DataType>
inline 
bool UniqueDataObject<DataType>::IsEmpty() const
{
	if (!m_Data)
	{
		return true;
	}
	else
	{
		return m_Data->IsEmpty();
	}
}

template<typename DataType>
inline 
bool UniqueDataObject<DataType>::IsPureEmpty() const
{
	return (!m_Data);
}

}//end namespace mdk

#endif