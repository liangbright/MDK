#ifndef __mdkFeatureDictionary_h
#define __mdkFeatureDictionary_h


#include "mdkObject.h"
#include "mdkDenseMatrix.h"


namespace mdk
{

template<typename ElementType>
class mdkFeatureDictionary : mdkObject
{
public:

    mdkDenseMatrix<ElementType> m_Record;

    mdkDenseMatrix<ElementType> m_Covariance;

    mdkDenseMatrix<ElementType> m_Variance;  // Variance(j) = sqrt(sum_i(Prob(i,j)*(Feature_i - Code(:,j))^2))

public:

    mdkFeatureDictionary();

    mdkFeatureDictionary(const mdkFeatureDictionary& InputDictionary);

    mdkFeatureDictionary(mdkFeatureDictionary&& InputDictionary);

    ~mdkFeatureDictionary();

    void operator=(const mdkFeatureDictionary& InputDictionary);

    void operator=(mdkFeatureDictionary&& InputDictionary);

    bool Copy(const mdkFeatureDictionary<ElementType>& InputDictionary);

    bool Copy(const mdkFeatureDictionary<ElementType>* InputDictionary);

    bool Share(mdkFeatureDictionary<ElementType>& InputDictionary);

    bool Share(mdkFeatureDictionary<ElementType>* InputDictionary);

    void ForceShare(const mdkFeatureDictionary<ElementType>& InputDictionary);

    bool ForceShare(const mdkFeatureDictionary<ElementType>* InputDictionary);

    void Clear();

    bool IsEmpty();

    mdkMatrixSize GetSize();

    bool Load(const std::string& FilePathAndName);

    bool Save(const std::string& FilePathAndName);
};


}// namespace mdk

#include "mdkFeatureDictionary.hpp"

#endif