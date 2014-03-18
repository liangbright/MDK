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
mdkFeatureDictionary<ElementType>::~mdkFeatureDictionary()
{

}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::Reset()
{
    m_Dictionary.Reset();

    m_Covariance.Reset();

    m_Variance.Reset(); // Variance(j) = sqrt(sum_i(Prob(i,j)*(Feature_i - Dictionary(:,j))^2))

}


template<typename ElementType>
void mdkFeatureDictionary<ElementType>::Clear()
{
    m_Dictionary.Clear();

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



}// namespace mdk

#endif