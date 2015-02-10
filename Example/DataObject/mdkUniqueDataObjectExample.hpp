#ifndef mdk_UniqueDataObjectExample_hpp
#define mdk_UniqueDataObjectExample_hpp

namespace mdk
{

template<typename DataType>
inline
UniqueDataObjectExample<DataType>::UniqueDataObjectExample()
{
	m_Data = std::make_unique<DataType>();
}

template<typename DataType>
inline
UniqueDataObjectExample<DataType>::UniqueDataObjectExample(const MDK_Symbol_PureEmpty&)
{// m_Data == nullptr
}

template<typename DataType>
inline
UniqueDataObjectExample<DataType>::UniqueDataObjectExample(const UniqueDataObjectExample& InputObject)
{	
	this->Copy(InputObject);
}

template<typename DataType>
inline 
UniqueDataObjectExample<DataType>::UniqueDataObjectExample(UniqueDataObjectExample&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
UniqueDataObjectExample<DataType>::~UniqueDataObjectExample()
{
}

template<typename DataType>
inline 
void UniqueDataObjectExample<DataType>::operator=(const UniqueDataObjectExample& InputObject)
{
	this->Copy(InputObject);
}

template<typename DataType>
inline
void UniqueDataObjectExample<DataType>::operator=(UniqueDataObjectExample&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
bool UniqueDataObjectExample<DataType>::Copy(const UniqueDataObjectExample& InputObject)
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
bool UniqueDataObjectExample<DataType>::Copy(UniqueDataObjectExample&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
void UniqueDataObjectExample<DataType>::Swap(UniqueDataObjectExample& InputObject)
{
	m_Data.swap(InputObject.m_Data);
}

template<typename DataType>
inline
void UniqueDataObjectExample<DataType>::Move(UniqueDataObjectExample& InputObject)
{
	m_Data=std::move(InputObject.m_Data);
}

template<typename DataType>
inline
void UniqueDataObjectExample<DataType>::Move(UniqueDataObjectExample&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
void UniqueDataObjectExample<DataType>::Clear()
{
	if (m_Data)
	{
		m_Data->Clear();
	}
}

template<typename DataType>
inline
void UniqueDataObjectExample<DataType>::Clear(const MDK_Symbol_PureEmpty&)
{
	m_Data.reset();
}


template<typename DataType>
inline 
void UniqueDataObjectExample<DataType>::Recreate()
{
	m_Data = std::make_unique<DataType>();
}

template<typename DataType>
inline 
bool UniqueDataObjectExample<DataType>::IsEmpty() const
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
bool UniqueDataObjectExample<DataType>::IsPureEmpty() const
{
	return (!m_Data);
}

}//end namespace mdk

#endif