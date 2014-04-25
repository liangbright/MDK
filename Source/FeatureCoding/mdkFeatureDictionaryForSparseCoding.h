#ifndef __mdkFeatureDictionaryForSparseCoding_h
#define __mdkFeatureDictionaryForSparseCoding_h

#include "mdkFileIO.h"
#include "mdkFeatureDictionary.h"


namespace mdk
{

template<typename ElementType>
class FeatureDictionaryForSparseCoding : public FeatureDictionary<ElementType>
{
private:

    CharString m_Name; // name of the dictionary

    DenseMatrix<ElementType> m_BasisMatrix; // D

    DenseMatrix<ElementType> m_ReconstructionStd;
    // m_ReconstructionStd(j) = sqrt(sum_i(Prob(i,j)*(FeatureData_i - D(:,j))^2))

    // not used for now ----------------
    DenseMatrix<ElementType> m_Covariance;  // relation between bases

public:

    FeatureDictionaryForSparseCoding();

    FeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding& InputDictionary);

    FeatureDictionaryForSparseCoding(FeatureDictionaryForSparseCoding&& InputDictionary);

    ~FeatureDictionaryForSparseCoding();

    void operator=(const FeatureDictionaryForSparseCoding& InputDictionary);

    void operator=(FeatureDictionaryForSparseCoding&& InputDictionary);

    bool Copy(const FeatureDictionaryForSparseCoding& InputDictionary);

    bool Copy(const FeatureDictionaryForSparseCoding* InputDictionary);

    bool Share(FeatureDictionaryForSparseCoding& InputDictionary);

    bool Share(FeatureDictionaryForSparseCoding* InputDictionary);

    void ForceShare(const FeatureDictionaryForSparseCoding& InputDictionary);

    bool ForceShare(const FeatureDictionaryForSparseCoding* InputDictionary);

    void Take(FeatureDictionaryForSparseCoding& InputDictionary);

    void Clear();

    bool IsEmpty() const;

    MatrixSize GetSize() const;

    bool Load(const CharString& FilePathAndName);

    bool Save(const CharString& FilePathAndName) const;

    CharString& Name();

    const CharString& Name() const;

    DenseMatrix<ElementType>& BasisMatrix();

    const DenseMatrix<ElementType>& BasisMatrix() const;

    DenseMatrix<ElementType>& ReconstructionStd();

    const DenseMatrix<ElementType>& ReconstructionStd() const;

};


}// namespace mdk

#include "mdkFeatureDictionaryForSparseCoding.hpp"


#endif