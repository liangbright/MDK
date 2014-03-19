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

    mdkMatrix<ElementType> m_Record;

    mdkMatrix<ElementType> m_Covariance;

    mdkMatrix<ElementType> m_Variance;  // Variance(j) = sqrt(sum_i(Prob(i,j)*(Feature_i - Code(:,j))^2))

public:

    mdkFeatureDictionary();

    mdkFeatureDictionary(const mdkFeatureDictionary& InputDictionary);

    mdkFeatureDictionary(mdkFeatureDictionary&& InputDictionary);

    ~mdkFeatureDictionary();

    void operator=(const mdkFeatureDictionary& InputDictionary);

    void operator=(mdkFeatureDictionary&& InputDictionary);

    void DeepCopy(const mdkFeatureDictionary<ElementType>& InputDictionary);

    void SharedCopy(mdkFeatureDictionary<ElementType>& InputDictionary);

    void ForceSharedCopy(const mdkFeatureDictionary<ElementType>& InputDictionary);

    void Reset();

    void Clear();

    bool Load(const std::string& FilePathAndName);

    bool Save(const std::string& FilePathAndName);

    mdkMatrixSize GetSize();
};


}// namespace mdk

#include "mdkFeatureDictionary.hpp"

#endif