#ifndef __mdkFeatureDictionary_h
#define __mdkFeatureDictionary_h


#include "mdkObject.h"
#include "mdkDenseMatrix.h"


namespace mdk
{

template<typename ElementType>
class FeatureDictionary : Object
{
public:

    DenseMatrix<ElementType> m_Record;

    DenseMatrix<ElementType> m_Covariance;

    DenseMatrix<ElementType> m_Variance;  // Variance(j) = sqrt(sum_i(Prob(i,j)*(Feature_i - Code(:,j))^2))

public:

    FeatureDictionary();

    FeatureDictionary(const FeatureDictionary& InputDictionary);

    FeatureDictionary(FeatureDictionary&& InputDictionary);

    ~FeatureDictionary();

    void operator=(const FeatureDictionary& InputDictionary);

    void operator=(FeatureDictionary&& InputDictionary);

    bool Copy(const FeatureDictionary& InputDictionary);

    bool Copy(const FeatureDictionary* InputDictionary);

    bool Share(FeatureDictionary& InputDictionary);

    bool Share(FeatureDictionary* InputDictionary);

    void ForceShare(const FeatureDictionary& InputDictionary);

    bool ForceShare(const FeatureDictionary* InputDictionary);

    void Clear();

    bool IsEmpty();

    MatrixSize GetSize();

    bool Load(const std::string& FilePathAndName);

    bool Save(const std::string& FilePathAndName);
};


}// namespace mdk

#include "mdkFeatureDictionary.hpp"

#endif