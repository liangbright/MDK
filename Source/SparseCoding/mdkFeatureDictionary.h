#ifndef __mdkFeatureDictionary_h
#define __mdkFeatureDictionary_h


#include "mdkObject.h"
#include "mdkMatrix.h"


namespace mdk
{

template<typename ElementType>
class mdkFeatureDictionary : mdkObject
{
public:

    mdkMatrix<ElementType> m_Dictionary;

    mdkMatrix<ElementType> m_Covariance;

    mdkMatrix<ElementType> m_Variance;  // Variance(j) = sqrt(sum_i(Prob(i,j)*(Feature_i - Code(:,j))^2))

public:

    mdkFeatureDictionary();

    ~mdkFeatureDictionary();

    void Reset();

    void Clear();

    bool Load(const std::string& FilePathAndName);

    bool Save(const std::string& FilePathAndName);
};


}// namespace mdk

#include "mdkFeatureDictionary.hpp"

#endif