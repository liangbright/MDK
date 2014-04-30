#ifndef __mdkFeatureDictionaryForSparseCoding_h
#define __mdkFeatureDictionaryForSparseCoding_h

#include "mdkFileIO.h"
#include "mdkFeatureDictionary.h"
#include "mdkDenseMatrix.h"

namespace mdk
{

template<typename ElementType>
struct DictionaryData_Of_FeatureDictionaryForSparseCoding
{
    CharString Name; // name of the dictionary

    DenseMatrix<ElementType> BasisMatrix; // D
    // ColNumber is BasisNumber (the number of bases)
    // RowNumber is Length of Feature Data Vector

    //------------ constraint -----------------------------------

    bool BasisPositive;

    bool BasisNormalizedWithL1Norm;

    bool BasisNormalizedWithL2Norm;

    //---------- Similarity between bases -----------------------

    MDK_SimilarityType_Enum_For_FeatureCoding SimilarityType;

    DenseMatrix<ElementType> SimilarityMatrix;

    ElementType SimilarityThresholdToComputeBasisRedundancy;

    DenseMatrix<ElementType> BasisRedundancy;

    // BasisRedundancy[j] =  the number of the other bases near the basis j ( Similarity(i, j) >= SimilarityThreshold) 
    //                       divided by (the total number of bases - 1);
    // range [0, 1]

    //----------------- Variance -------------------------

    DenseMatrix<ElementType> VarianceOfL1Distance;
    // Length = BasisNumber
    // Variance_i_j = || X_i - D(:, j)||^2
    // Variance(j) = sqrt(sum_i(Indicator_i_j * Variance_i_j))
    // if Basis_j is related to X_i, then  Indicator_i_j = 1, else it = 0

    DenseMatrix<ElementType> VarianceOfL2Distance;
    // Length = BasisNumber
    // Variance_i_j = || X_i - D(:, j)||^2
    // Variance(j) = sqrt(sum_i(Indicator_i_j * Variance_i_j))
    // if Basis_j is related to X_i, then  Indicator_i_j = 1, else it = 0

    DenseMatrix<ElementType> VarianceOfKLDivergence; // not real Variance
    // Length = BasisNumber
    // Variance_i_j = KL( X_i, - D(:, j))
    // Variance(j) = sqrt(sum_i(Indicator_i_j * Variance_i_j))
    // if Basis_j is related to X_i, then  Indicator_i_j = 1, else it = 0

    DenseMatrix<ElementType> VarianceOfReconstruction;
    // Variance_i_j = || X_i - D * Alpha||^2
    // VarianceOfReconstruction(j) = mean (ErrorNorm_i related to Basis_j (i.e., Alpha(j) > 0 ))

    ElementType WeightedNumberOfTrainingSamplesInHistory; // the total weighted-number of data samples used to build the dictionary
                                                          // the "experience" of the dictionary

    DenseMatrix<ElementType> ProbabilityMassFunction; // discrete probability density function, i.e., probability mass function
                                                      // feature-data distribution on each basis

    // note: 
    // Histogram = WeightedNumberOfTrainingSamplesInHistory * ProbabilityMassFunction

    // not used yet ----------------
    DenseMatrix<ElementType> BasisCovariance;  // relation between bases
};


template<typename ElementType>
class FeatureDictionaryForSparseCoding : public FeatureDictionary<ElementType>
{
public:
    typedef MDK_SimilarityType_Enum_For_FeatureCoding SimilarityTypeEnum;

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

    // -------------- get/set ---------------------------------------------------//

    inline const CharString& GetName() const;

    inline void SetName(const CharString& Name);

    inline DenseMatrix<ElementType>& BasisMatrix();

    inline const DenseMatrix<ElementType>& BasisMatrix() const;

    inline void SetInfo_BasisPositive(bool YesNO);
    inline void SetInfo_BasisNormalizedWithL1Norm(bool YesNO);
    inline void SetInfo_BasisNormalizedWithL2Norm(bool YesNO);

    inline bool GetInfo_BasisPositive() const;
    inline bool GetInfo_BasisNormalizedWithL1Norm() const;
    inline bool GetInfo_BasisNormalizedWithL2Norm() const;

    inline void SetInfo_SimilarityType(MDK_SimilarityType_Enum_For_FeatureCoding SimilarityType);
    inline MDK_SimilarityType_Enum_For_FeatureCoding GetInfo_SimilarityType() const;

    inline DenseMatrix<ElementType>& SimilarityMatrix();
    inline const DenseMatrix<ElementType>& SimilarityMatrix() const;

    inline void SetInfo_SimilarityThresholdToComputeBasisRedundancy(ElementType SimilarityThreshold);
    inline ElementType GetInfo_SimilarityThresholdToComputeBasisRedundancy() const;

    inline DenseMatrix<ElementType>& BasisRedundancy();
    inline const DenseMatrix<ElementType>& BasisRedundancy() const;

    inline DenseMatrix<ElementType>& VarianceOfL1Distance();
    inline const DenseMatrix<ElementType>& VarianceOfL1Distance() const;

    inline DenseMatrix<ElementType>& VarianceOfL2Distance();
    inline const DenseMatrix<ElementType>& VarianceOfL2Distance() const;

    inline DenseMatrix<ElementType>& VarianceOfKLDivergence();
    inline const DenseMatrix<ElementType>& VarianceOfKLDivergence() const;

    inline DenseMatrix<ElementType>& VarianceOfReconstruction();
    inline const DenseMatrix<ElementType>& VarianceOfReconstruction() const;

    inline ElementType GetWeightedNumberOfTrainingSamplesInHistory() const;
    inline void SetWeightedNumberOfTrainingSamplesInHistory(ElementType Number);

    inline DenseMatrix<ElementType>& ProbabilityMassFunction();
    inline const DenseMatrix<ElementType>& ProbabilityMassFunction() const;

};


}// namespace mdk

#include "mdkFeatureDictionaryForSparseCoding.hpp"


#endif