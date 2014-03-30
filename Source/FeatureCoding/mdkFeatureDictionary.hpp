#ifndef __mdkFeatureDictionary_hpp
#define __mdkFeatureDictionary_hpp


#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
FeatureDictionary<ElementType>::FeatureDictionary()
{

}


template<typename ElementType>
FeatureDictionary<ElementType>::FeatureDictionary(const FeatureDictionary& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
FeatureDictionary<ElementType>::FeatureDictionary(FeatureDictionary&& InputDictionary)
{
    m_Record = std::move(InputDictionary.m_Record);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_Variance = std::move(InputDictionary.m_Variance);
}


template<typename ElementType>
FeatureDictionary<ElementType>::~FeatureDictionary()
{

}


template<typename ElementType>
void FeatureDictionary<ElementType>::operator=(const FeatureDictionary& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
void FeatureDictionary<ElementType>::operator=(FeatureDictionary&& InputDictionary)
{
    m_Record = std::move(InputDictionary.m_Record);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_Variance = std::move(InputDictionary.m_Variance);
}


template<typename ElementType>
bool FeatureDictionary<ElementType>::Copy(const FeatureDictionary<ElementType>& InputDictionary)
{
    m_Record.Copy(InputDictionary.m_Record);

    m_Covariance.Copy(InputDictionary.m_Covariance);

    m_Variance.Copy(InputDictionary.m_Variance);

    return true;

}


template<typename ElementType>
bool FeatureDictionary<ElementType>::Copy(const FeatureDictionary<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error << "Input is nullptr @ FeatureDictionary::Copy(FeatureDictionary*)" << '\n';
        return false;
    }

    return this->Copy(*InputDictionary);
}


template<typename ElementType>
bool FeatureDictionary<ElementType>::Share(FeatureDictionary<ElementType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return true;
    }

    auto IsOK_1 = m_Record.Share(InputDictionary.m_Record);

    auto IsOK_2 = m_Covariance.Share(InputDictionary.m_Covariance);

    auto IsOK_3 = m_Variance.Share(InputDictionary.m_Variance);

    if (IsOK_1 == false || IsOK_2 == false || IsOK_3 == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}


template<typename ElementType>
bool FeatureDictionary<ElementType>::Share(FeatureDictionary<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error << "Input is nullptr @ FeatureDictionary::Share(FeatureDictionary*)" << '\n';
        return false;
    }

    return this->Share(*InputDictionary);
}


template<typename ElementType>
void FeatureDictionary<ElementType>::ForceShare(const FeatureDictionary<ElementType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return;
    }

    m_Record.ForceShare(InputDictionary.m_Record);

    m_Covariance.ForceShare(InputDictionary.m_Covariance);

    m_Variance.ForceShare(InputDictionary.m_Variance);
}


template<typename ElementType>
bool FeatureDictionary<ElementType>::ForceShare(const FeatureDictionary<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error << "Input is nullptr @ FeatureDictionary::ForceShare(FeatureDictionary*)" << '\n';
        return false;
    }

    this->ForceShare(*InputDictionary);

    return true;
}


template<typename ElementType>
void FeatureDictionary<ElementType>::Clear()
{
    m_Record.Clear();

    m_Covariance.Clear();

    m_Variance.Clear();
}


template<typename ElementType>
bool FeatureDictionary<ElementType>::IsEmpty() const
{
    return m_Record.IsEmpty();
}


template<typename ElementType>
MatrixSize FeatureDictionary<ElementType>::GetSize() const
{
    return m_Record.GetSize();
}

template<typename ElementType>
bool FeatureDictionary<ElementType>::Load(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool FeatureDictionary<ElementType>::Save(const std::string& FilePathAndName) const
{
    return true;
}


}// namespace mdk

#endif