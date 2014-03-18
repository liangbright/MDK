#ifndef __mdkFeatureCodebook_h
#define __mdkFeatureCodebook_h


#include "mdkObject.h"
#include "mdkMatrix.h"


namespace mdk
{

template<typename ElementType>
class mdkFeatureCodebook : mdkObject
{
public:

    mdkMatrix<ElementType> m_Code;

    mdkMatrix<ElementType> m_Covariance;

    mdkMatrix<ElementType> m_Variance;  // Variance(j) = sqrt(sum_i(Prob(i,j)*(Feature_i - Code(:,j))^2))

public:

    mdkFeatureCodebook();

    ~mdkFeatureCodebook();

    void Reset();

    void Clear();

    bool Load(const std::string& FilePathAndName);

    bool Save(const std::string& FilePathAndName);
};


}// namespace mdk

#include "mdkFeatureCodebook.hpp"

#endif