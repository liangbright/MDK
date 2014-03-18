#ifndef __mdkFeatureCodebook_hpp
#define __mdkFeatureCodebook_hpp


#include "mdkFeatureCodebook.h"

namespace mdk
{

template<typename ElementType>
mdkFeatureCodebook<ElementType>::mdkFeatureCodebook()
{

}


template<typename ElementType>
mdkFeatureCodebook<ElementType>::~mdkFeatureCodebook()
{

}


template<typename ElementType>
void mdkFeatureCodebook<ElementType>::Reset()
{
    m_Code.Reset();

    m_Covariance.Reset();

    m_Variance.Reset(); // Variance(j) = sqrt(sum_i(Prob(i,j)*(Feature_i - Code(:,j))^2))

}


template<typename ElementType>
void mdkFeatureCodebook<ElementType>::Clear()
{
    m_Code.Clear();

    m_Covariance.Clear();

    m_Variance.Clear();
}



template<typename ElementType>
bool mdkFeatureCodebook<ElementType>::Load(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool mdkFeatureCodebook<ElementType>::Save(const std::string& FilePathAndName)
{
    return true;
}



}// namespace mdk

#endif