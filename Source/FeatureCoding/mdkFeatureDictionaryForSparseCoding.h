#ifndef __mdkFeatureDictionaryForSparseCoding_h
#define __mdkFeatureDictionaryForSparseCoding_h

#include "mdkFileIO.h"
#include "mdkFeatureDictionary.h"


namespace mdk
{

struct Infomation_Of_FeatureDictionaryForSparseCoding
{
    CharString Name; // name of the dictionary

    bool BasisNonnegative;

    bool BasisSumToOne;
};


template<typename ElementType>
struct DictionaryData_Of_FeatureDictionaryForSparseCoding
{
    Infomation_Of_FeatureDictionaryForSparseCoding DictionaryInfo;

    DenseMatrix<ElementType> BasisMatrix; // D

    DenseMatrix<ElementType> StandardDeviation;
    // m_StandardDeviation(j) = sqrt(sum_i(Prob(i,j)*(FeatureData_i - D(:,j))^2))

    DenseMatrix<ElementType> BasisProbability;

    // not used yet ----------------
    DenseMatrix<ElementType> Covariance;  // relation between bases
};


template<typename ElementType>
class FeatureDictionaryForSparseCoding : public FeatureDictionary<ElementType>
{
private:

    std::shared_ptr<DictionaryData_Of_FeatureDictionaryForSparseCoding<ElementType>> m_DictionaryData;

public:

    FeatureDictionaryForSparseCoding();

    FeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding& InputDictionary);

    FeatureDictionaryForSparseCoding(FeatureDictionaryForSparseCoding&& InputDictionary);

    ~FeatureDictionaryForSparseCoding();

    void operator=(const FeatureDictionaryForSparseCoding& InputDictionary);

    void operator=(FeatureDictionaryForSparseCoding&& InputDictionary);

    void Copy(const FeatureDictionaryForSparseCoding& InputDictionary);

    bool Copy(const FeatureDictionaryForSparseCoding* InputDictionary);

    void Share(FeatureDictionaryForSparseCoding& InputDictionary);

    bool Share(FeatureDictionaryForSparseCoding* InputDictionary);

    void ForceShare(const FeatureDictionaryForSparseCoding& InputDictionary);

    bool ForceShare(const FeatureDictionaryForSparseCoding* InputDictionary);

    void Take(FeatureDictionaryForSparseCoding& InputDictionary);

    void Clear();

    bool IsEmpty() const;

    MatrixSize GetSize() const;

    bool Load(const CharString& FilePathAndName);

    bool Save(const CharString& FilePathAndName) const;

    // -------------- get/set info---------------------------------------------------//

    inline const Infomation_Of_FeatureDictionaryForSparseCoding& GetDictionaryInformation() const;

    inline void SetDictionaryInformation(const Infomation_Of_FeatureDictionaryForSparseCoding& Info);

    inline const CharString& GetName() const;

    inline void SetName(const CharString& Name);

    inline void SetDictionaryInfo_BasisNonnegative(bool Nonnegative = true);

    inline void SetDictionaryInfo_BasisSumToOne(bool SumToOne = true);

    inline bool IsBasisNonnegative() const;

    inline bool IsBasisSumToOne() const;

    // --------------------- get/set data --------------------------------------------//

    inline DenseMatrix<ElementType>& BasisMatrix();

    inline const DenseMatrix<ElementType>& BasisMatrix() const;

    inline DenseMatrix<ElementType>& StandardDeviation();

    inline const DenseMatrix<ElementType>& StandardDeviation() const;

    inline DenseMatrix<ElementType>& BasisProbability();

    inline const DenseMatrix<ElementType>& BasisProbability() const;

    
};


}// namespace mdk

#include "mdkFeatureDictionaryForSparseCoding.hpp"


#endif