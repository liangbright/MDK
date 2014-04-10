#ifndef __mdkFeatureDictionaryForDenseCoding_h
#define __mdkFeatureDictionaryForDenseCoding_h


#include "mdkFeatureDictionary.h"


namespace mdk
{

template<typename ElementType>
class FeatureDictionaryForDenseCoding : public FeatureDictionary<ElementType>
{
public:

    std::string m_Name;

    DenseMatrix<ElementType> m_BasisMatrix; // D

    DenseMatrix<ElementType> m_MeanVector; // sample mean in PCA

    DenseMatrix<ElementType> m_Covariance;
    // relation between bases

    DenseMatrix<ElementType> m_StandardDeviation;
    // StandardDeviation(j) = sqrt(sum_i(Prob(i,j)*(FeatureData_i - D(:,j))^2))

public:

    FeatureDictionaryForDenseCoding();

    FeatureDictionaryForDenseCoding(const FeatureDictionaryForDenseCoding& InputDictionary);

    FeatureDictionaryForDenseCoding(FeatureDictionaryForDenseCoding&& InputDictionary);

    ~FeatureDictionaryForDenseCoding();

    void operator=(const FeatureDictionaryForDenseCoding& InputDictionary);

    void operator=(FeatureDictionaryForDenseCoding&& InputDictionary);

    bool Copy(const FeatureDictionaryForDenseCoding& InputDictionary);

    bool Copy(const FeatureDictionaryForDenseCoding* InputDictionary);

    bool Share(FeatureDictionaryForDenseCoding& InputDictionary);

    bool Share(FeatureDictionaryForDenseCoding* InputDictionary);

    void ForceShare(const FeatureDictionaryForDenseCoding& InputDictionary);

    bool ForceShare(const FeatureDictionaryForDenseCoding* InputDictionary);

    void Clear();

    bool IsEmpty() const;

    MatrixSize GetSize() const;

    bool Load(const std::string& FilePathAndName);

    bool Save(const std::string& FilePathAndName) const;

    const std::string& GetName() const;

    DenseMatrix<ElementType>& BasisMatrix();

    const DenseMatrix<ElementType>& BasisMatrix() const;

};


}// namespace mdk

#include "mdkFeatureDictionaryForDenseCoding.hpp"


#endif