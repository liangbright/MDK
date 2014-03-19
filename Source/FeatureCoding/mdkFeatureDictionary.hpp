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
    this->DeepCopy(InputDictionary)
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
    this->DeepCopy(InputDictionary)
}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::operator=(mdkFeatureDictionary&& InputDictionary)
{
    m_Record = std::move(InputDictionary.m_Record);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_Variance = std::move(InputDictionary.m_Variance);
}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::SharedCopy(mdkFeatureDictionary<ElementType>& InputDictionary)
{
    m_Record.SharedCopy(InputDictionary.m_Record);

    m_Covariance.SharedCopy(InputDictionary.m_Covariance);

    m_Variance.SharedCopy(InputDictionary.m_Variance);

}



template<typename ElementType>
void mdkFeatureDictionary<ElementType>::ForceSharedCopy(const mdkFeatureDictionary<ElementType>& InputDictionary)
{
    m_Record.ForceSharedCopy(InputDictionary.m_Record);

    m_Covariance.ForceSharedCopy(InputDictionary.m_Covariance);

    m_Variance.ForceSharedCopy(InputDictionary.m_Variance);
}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::Reset()
{
    m_Record.Reset();

    m_Covariance.Reset();

    m_Variance.Reset(); // Variance(j) = sqrt(sum_i(Prob(i,j)*(Feature_i - Dictionary(:,j))^2))

}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::Clear()
{
    m_Record.Clear();

    m_Covariance.Clear();

    m_Variance.Clear();

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


template<typename ElementType>
mdkMatrixSize mdkFeatureDictionary<ElementType>::GetSize()
{
    return m_Record.GetSize();
}


}// namespace mdk

#endif