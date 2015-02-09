#ifndef mdk_SharedDataObject_hpp
#define mdk_SharedDataObject_hpp

namespace mdk
{

template<typename DataType>
inline
SharedDataObject<DataType>::SharedDataObject()
{
	m_Data = std::make_shared<DataType>();
}

template<typename DataType>
inline
SharedDataObject<DataType>::SharedDataObject(const MDK_Symbol_PureEmpty&)
{// m_Data == nullptr
}

template<typename DataType>
inline
SharedDataObject<DataType>::SharedDataObject(const SharedDataObject& InputObject)
{	
	this->Copy(InputObject);
}

template<typename DataType>
inline 
SharedDataObject<DataType>::SharedDataObject(SharedDataObject&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
SharedDataObject<DataType>::~SharedDataObject()
{
}

template<typename DataType>
inline 
void SharedDataObject<DataType>::operator=(const SharedDataObject& InputObject)
{
	this->Copy(InputObject);
}

template<typename DataType>
inline
void SharedDataObject<DataType>::operator=(SharedDataObject&& InputObject)
{
	this->Copy(std::move(InputObject));
}

template<typename DataType>
inline
bool SharedDataObject<DataType>::Copy(const SharedDataObject& InputObject)
{
	if (!m_Data)
	{
		m_Data = std::make_shared<DataType>();
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
bool SharedDataObject<DataType>::Copy(SharedDataObject&& InputObject)
{
	if (!m_Data)
	{
		m_Data = std::make_shared<DataType>();
	}

	if (InputObject.m_Data)
	{
		return m_Data->Copy(std::move(*InputObject.m_Data));
	}
	else
	{
		return m_Data->Copy(MDK_EMPTY);
	}
}

template<typename DataType>
inline 
bool SharedDataObject<DataType>::Share(SharedDataObject& InputObject)
{
	if (m_Data)
	{
		if (InputObject.m_Data)
		{
			if (m_Data->IsReadyToShare(*InputObject.m_Data) == false)
			{
				return false;
			}
			else
			{
				if (m_Data->IsReadyToShare(MDK_EMPTY) == false)
				{
					return false;
				}
			}
		}
	}
	m_Data = InputObject.m_Data;
	return true;
}

template<typename DataType>
inline
void SharedDataObject<DataType>::ForceShare(const SharedDataObject& InputObject)
{
	m_Data = InputObject.m_Data;
}

template<typename DataType>
inline
void SharedDataObject<DataType>::Swap(SharedDataObject& InputObject)
{
	m_Data.swap(InputObject.m_Data);
}

template<typename DataType>
inline
void SharedDataObject<DataType>::Move(SharedDataObject& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
void SharedDataObject<DataType>::Move(SharedDataObject&& InputObject)
{
	m_Data = std::move(InputObject.m_Data);
}

template<typename DataType>
inline
void SharedDataObject<DataType>::Clear()
{
	if (m_Data)
	{
		m_Data->Clear();
	}
}

template<typename DataType>
inline
void SharedDataObject<DataType>::Clear(const MDK_Symbol_PureEmpty&)
{
	m_Data.reset();
}


template<typename DataType>
inline void SharedDataObject<DataType>::Recreate()
{
	m_Data = std::make_shared<DataType>();
}

template<typename DataType>
inline 
bool SharedDataObject<DataType>::IsEmpty() const
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
bool SharedDataObject<DataType>::IsPureEmpty() const
{
	return (!m_Data);
}

template<typename DataType>
inline 
bool SharedDataObject<DataType>::IsShared() const
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
bool SharedDataObject<DataType>::IsSharedWith(const SharedDataObject& InputObject) const
{
	if (!m_Data || !InputObject.m_Data)
	{
		return false;
	}
	else
	{
		return m_Data->IsSharedWith(*InputObject.m_Data);
	}
}

}//end namespace mdk

#endif