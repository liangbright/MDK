#ifndef mdk_SharedDataObjectExample_hpp
#define mdk_SharedDataObjectExample_hpp

namespace mdk
{

template<typename DataType>
inline
SharedDataObjectExample<DataType>::SharedDataObjectExample()
{
	m_Data = std::make_shared<DataType>();
}

template<typename DataType>
inline
SharedDataObjectExample<DataType>::SharedDataObjectExample(const MDK_Symbol_PureEmpty&)
{// m_Data == nullptr
}

template<typename DataType>
inline
SharedDataObjectExample<DataType>::SharedDataObjectExample(const SharedDataObjectExample& InputObject)
{	
	this->Copy(InputObject);
}

template<typename DataType>
inline 
SharedDataObjectExample<DataType>::SharedDataObjectExample(SharedDataObjectExample&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
SharedDataObjectExample<DataType>::~SharedDataObjectExample()
{
}

template<typename DataType>
inline 
void SharedDataObjectExample<DataType>::operator=(const SharedDataObjectExample& InputObject)
{
	this->Copy(InputObject);
}

template<typename DataType>
inline
void SharedDataObjectExample<DataType>::operator=(SharedDataObjectExample&& InputObject)
{
	this->Copy(std::move(InputObject));
}


template<typename DataType>
inline
void SharedDataObjectExample<DataType>::Copy(const SharedDataObjectExample& InputObject)
{
	if (!m_Data)
	{
		m_Data = std::make_shared<DataType>();
	}

	if (InputObject.m_Data)
	{		
		m_Data->Copy(*InputObject.m_Data);
	}
	else
	{
		m_Data->Copy(MDK_EMPTY_OBJECT);
	}
}

template<typename DataType>
inline
void SharedDataObjectExample<DataType>::Copy(SharedDataObjectExample&& InputObject)
{
	if (!m_Data)
	{
		m_Data = std::make_shared<DataType>();
	}

	if (InputObject.m_Data)
	{
		m_Data->Copy(std::move(*InputObject.m_Data));
	}
	else
	{
		m_Data->Copy(MDK_EMPTY_OBJECT);
	}
}

  
template<typename DataType>
inline 
void SharedDataObjectExample<DataType>::Share(SharedDataObjectExample& InputObject)
{
	m_Data = InputObject.m_Data;
}

template<typename DataType>
inline
void SharedDataObjectExample<DataType>::ForceShare(const SharedDataObjectExample& InputObject)
{
	m_Data = InputObject.m_Data;
}

template<typename DataType>
inline
void SharedDataObjectExample<DataType>::Swap(SharedDataObjectExample& InputObject)
{
	m_Data.swap(InputObject.m_Data);
}

template<typename DataType>
inline
void SharedDataObjectExample<DataType>::Move(SharedDataObjectExample& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
void SharedDataObjectExample<DataType>::Move(SharedDataObjectExample&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
void SharedDataObjectExample<DataType>::Clear()
{
	if (m_Data)
	{
		m_Data->Clear();
	}
}

template<typename DataType>
inline
void SharedDataObjectExample<DataType>::Clear(const MDK_Symbol_PureEmpty&)
{
	m_Data.reset();
}


template<typename DataType>
inline void SharedDataObjectExample<DataType>::Recreate()
{
	m_Data = std::make_shared<DataType>();
}

template<typename DataType>
inline 
bool SharedDataObjectExample<DataType>::IsEmpty() const
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
bool SharedDataObjectExample<DataType>::IsPureEmpty() const
{
	return (!m_Data);
}

template<typename DataType>
inline 
bool SharedDataObjectExample<DataType>::IsShared() const
{
	if (!m_Data)
	{
		return false;
	}
	else
	{
		return (m_Data.use_count() > 1);
	}
}

template<typename DataType>
inline
bool SharedDataObjectExample<DataType>::IsSharedWith(const SharedDataObjectExample& InputObject) const
{
	if (!m_Data || !InputObject.m_Data)
	{
		return false;
	}
	else
	{
		return (&(*m_Data) == &(*InputObject.m_Data));
        // return m_Data->ScalarArray.data() == InputObject.m_Data->ScalarArray.data()
	}
}

}//end namespace mdk

#endif