#ifndef __mdkFeatureDictionaryForCommonSparseEndocer_h
#define __mdkFeatureDictionaryForCommonSparseEndocer_h


#include "mdkFeatureDictionary.h"


namespace mdk
{

template<typename ElementType>
class FeatureDictionaryForCommonSparseEndocer : FeatureDictionary<ElementType>
{
public:

    DenseMatrix<ElementType> m_BasisMatrix; // D

    DenseMatrix<ElementType> m_Covariance;
    // relation between bases

    DenseMatrix<ElementType> m_StandardDeviation;
    // StandardDeviation(j) = sqrt(sum_i(Prob(i,j)*(FeatureData_i - D(:,j))^2))

public:

    FeatureDictionaryForCommonSparseEndocer();

    FeatureDictionary(const FeatureDictionaryForCommonSparseEndocer& InputDictionary);

    FeatureDictionaryForCommonSparseEndocer(FeatureDictionaryForCommonSparseEndocer&& InputDictionary);

    ~FeatureDictionaryForCommonSparseEndocer();

    void operator=(const FeatureDictionaryForCommonSparseEndocer& InputDictionary);

    void operator=(FeatureDictionaryForCommonSparseEndocer&& InputDictionary);

    bool Copy(const FeatureDictionaryForCommonSparseEndocer& InputDictionary);

    bool Copy(const FeatureDictionaryForCommonSparseEndocer* InputDictionary);

    bool Share(FeatureDictionaryForCommonSparseEndocer& InputDictionary);

    bool Share(FeatureDictionaryForCommonSparseEndocer* InputDictionary);

    void ForceShare(const FeatureDictionaryForCommonSparseEndocer& InputDictionary);

    bool ForceShare(const FeatureDictionaryForCommonSparseEndocer* InputDictionary);

    void Clear();

    bool IsEmpty() const;

    MatrixSize GetSize() const;

    bool Load(const std::string& FilePathAndName);

    bool Save(const std::string& FilePathAndName) const;

    DenseMatrix<ElementType>& BasisMatrix();

    const DenseMatrix<ElementType> BasisMatrix() const;

};


}// namespace mdk

#include "mdkFeatureDictionaryForCommonSparseEndocer.hpp"


#endif