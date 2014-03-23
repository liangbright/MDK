#ifndef __mdkFeatureDictionary_hpp
#define __mdkFeatureDictionary_hpp


#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
mdkFeatureDictionary<ElementType>::mdkFeatureDictionary()
{

}


template<typename ElementType>
mdkFeatureDictionary<ElementType>::mdkFeatureDictionary(const mdkFeatureDictionary& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
mdkFeatureDictionary<ElementType>::mdkFeatureDictionary(mdkFeatureDictionary&& InputDictionary)
{
    m_Record = std::move(InputDictionary.m_Record);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_Variance = std::move(InputDictionary.m_Variance);
}


template<typename ElementType>
mdkFeatureDictionary<ElementType>::~mdkFeatureDictionary()
{

}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::operator=(const mdkFeatureDictionary& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::operator=(mdkFeatureDictionary&& InputDictionary)
{
    m_Record = std::move(InputDictionary.m_Record);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_Variance = std::move(InputDictionary.m_Variance);
}


template<typename ElementType>
bool mdkFeatureDictionary<ElementType>::Copy(const mdkFeatureDictionary<ElementType>& InputDictionary)
{
    if (this->IsEmpty() == false)
    {
        mdkError << "Self is not empty @ mdkFeatureDictionary::Copy(mdkFeatureDictionary&)" << '\n';
        return false;
    }

    this->Clear();

    m_Record.Copy(InputDictionary.m_Record);

    m_Covariance.Copy(InputDictionary.m_Covariance);

    m_Variance.Copy(InputDictionary.m_Variance);

    return true;

}


template<typename ElementType>
bool mdkFeatureDictionary<ElementType>::Copy(const mdkFeatureDictionary<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        mdkError << "Input is nullptr @ mdkFeatureDictionary::Copy(mdkFeatureDictionary*)" << '\n';
        return false;
    }

    return this->Copy(*InputDictionary);
}


template<typename ElementType>
bool mdkFeatureDictionary<ElementType>::Share(mdkFeatureDictionary<ElementType>& InputDictionary)
{
    // only empty dictionary can use this method
    if (this->IsEmpty() == false)
    {
        mdkError << "Self is not empty @ mdkFeatureDictionary::Share(mdkFeatureDictionary&)" << '\n';
        return false;
    }

    this->Clear();

    m_Record.Share(InputDictionary.m_Record);

    m_Covariance.Share(InputDictionary.m_Covariance);

    m_Variance.Share(InputDictionary.m_Variance);

    return true;
}


template<typename ElementType>
bool mdkFeatureDictionary<ElementType>::Share(mdkFeatureDictionary<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        mdkError << "Input is nullptr @ mdkFeatureDictionary::Share(mdkFeatureDictionary*)" << '\n';
        return false;
    }

    return this->Share(*InputDictionary);
}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::ForceShare(const mdkFeatureDictionary<ElementType>& InputDictionary)
{
    this->Clear();

    m_Record.ForceShare(InputDictionary.m_Record);

    m_Covariance.ForceShare(InputDictionary.m_Covariance);

    m_Variance.ForceShare(InputDictionary.m_Variance);
}


template<typename ElementType>
bool mdkFeatureDictionary<ElementType>::ForceShare(const mdkFeatureDictionary<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        mdkError << "Input is nullptr @ mdkFeatureDictionary::ForceShare(mdkFeatureDictionary*)" << '\n';
        return false;
    }

    this->ForceShare(*InputDictionary);

    return true;
}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::Clear()
{
    m_Record.Clear();

    m_Covariance.Clear();

    m_Variance.Clear();
}


template<typename ElementType>
bool mdkFeatureDictionary<ElementType>::IsEmpty()
{
    return m_Record.IsEmpty();
}


template<typename ElementType>
mdkMatrixSize mdkFeatureDictionary<ElementType>::GetSize()
{
    return m_Record.GetSize();
}

template<typename ElementType>
bool mdkFeatureDictionary<ElementType>::Load(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool mdkFeatureDictionary<ElementType>::Save(const std::string& FilePathAndName)
{
    return true;
}


}// namespace mdk

#endif