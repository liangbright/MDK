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

    DenseMatrix<ElementType> m_Record; // D

    DenseMatrix<ElementType> m_Covariance;
    // relation between bases

    DenseMatrix<ElementType> m_StandardDeviation;
    // StandardDeviation(j) = sqrt(sum_i(Prob(i,j)*(FeatureData_i - D(:,j))^2))

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

    bool IsEmpty() const;

    MatrixSize GetSize() const;

    bool Load(const std::string& FilePathAndName);

    bool Save(const std::string& FilePathAndName) const;
};


}// namespace mdk

#include "mdkFeatureDictionary.hpp"

#endif