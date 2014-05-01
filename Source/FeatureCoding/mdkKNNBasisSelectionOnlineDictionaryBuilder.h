#ifndef __mdkKNNBasisSelectionOnlineDictionaryBuilder_h
#define __mdkKNNBasisSelectionOnlineDictionaryBuilder_h

#include <random>


#include "mdkFileIO.h"
#include "mdkString.h"
#include "mdkDataContainer.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkLinearLeastSquaresProblemSolver.h"
#include "mdkKNNSoftAssignSparseEncoder.h"
#include "mdkKNNReconstructionSparseEncoder.h"

namespace mdk
{

template<typename ElementType>
struct Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> ParameterOfKNNSoftAssign;

    ElementType weigth_s;
    // sort the pair i, j according to score = weigth_s * Similarity(i, j) + (1-weigth_s) * 0.5*(prob(i) + prob(j))


    ElementType ExperienceDiscountFactor; 
    // weight for the past experience when new training data is used
    // When to use the weight : 
    // discount the Experience of dictionary right after training is done on new data
    // implemented in UpdateExperienceOfRepresentingDataForEachBasisVector(...)

    // parameter for data sampling --------

    int_max NumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfIteration;

    int_max MaxNumberOfThreads;

    // parameter for updating dictionary information

    bool UpdateDictionaryInformation_Variance;

    ElementType SimilarityThresholdToComputeBasisRedundancy;

    Parameter_Of_KNNReconstructionSparseEncoder ParameterOfKNNReconstruction;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder() {}

    void Clear()
    {
        BasisNumber = -1;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        ParameterOfKNNSoftAssign.Clear();

        weigth_s = 0;

        ExperienceDiscountFactor = 0;

        NumberOfDataInEachBatch = 0;
        MaxNumberOfIteration = 0;

        MaxNumberOfThreads = 1;

        UpdateDictionaryInformation_Variance = false;

        SimilarityThresholdToComputeBasisRedundancy = 0;

        ParameterOfKNNReconstruction.Clear();
    }
};


template<typename ElementType>
class KNNBasisSelectionOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{

public:
    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType> m_Parameter;

    typedef MDK_SimilarityType_Enum_For_FeatureCoding SimilarityTypeEnum;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;


public:
    KNNBasisSelectionOnlineDictionaryBuilder();
    ~KNNBasisSelectionOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary);

    void SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    bool CheckInput();

    FeatureDictionaryForSparseCoding<ElementType>* GetOutputDictionary();

protected:

    void SetupDefaultPipelineOutput();

    void UpdatePipelineOutput();

    void GenerateDictionary();

    FeatureDictionaryForSparseCoding<ElementType> BuildDictionaryFromData(int_max BasisNumber_desired,
                                                                           const DenseMatrix<ElementType>& FeatureData,
                                                                           const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init);

    void UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                     const DenseMatrix<ElementType>& FeatureData,
                                     const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                     const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                     const DenseMatrix<int_max>& VectorIndexList_Basis,
                                     const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init);

    void UpdateDictionaryInformation_Variance(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                              const DenseMatrix<ElementType>& FeatureData,
                                              const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                              const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                              const DenseMatrix<int_max>& VectorIndexList_Basis,
                                              const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init);

    DenseMatrix<ElementType> ComputeRepresentativeAbilityOfEachDataVectorInCombinedData(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init,
                                                                                        int_max CombinedDataVectorNumber);

    DenseMatrix<ElementType> ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init,
                                                           const DenseMatrix<ElementType>& FeatureData);

    inline ElementType ComputeSimilarityBetweenTwoDataVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length);

    DataContainer<DenseMatrix<int_max>> FindKNNVectorIndexTableByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix);

    DenseMatrix<ElementType> EstimateKNNSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable);

    DenseMatrix<ElementType> EstimateKNNSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable,
                                                                                               const DenseMatrix<ElementType>& RepresentativeAbilityOfEachVector);

    void ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix);

    DataContainer<DenseMatrix<int_max>> FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                                             const DenseMatrix<int_max>&     VectorIndexList_Basis,
                                                                                             int_max BasisNumber_init);

    void UpdateExperienceOfRepresentingDataForEachBasisVector(DenseMatrix<ElementType>& ExperienceOfRepresentingData,
                                                              const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData);

    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);   

    void UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>& ExperienceOfRepresentingData);

    void UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>& ExperienceOfRepresentingData);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                                      const DenseMatrix<ElementType>& FeatureData,
                                      const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                      const DenseMatrix<ElementType>& BasisMatrix,
                                      const DenseMatrix<ElementType>& ExperienceOfRepresentingData);

    void UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                                        const DenseMatrix<ElementType>& FeatureData,
                                        const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                        const DenseMatrix<ElementType>& BasisMatrix,
                                        const DenseMatrix<ElementType>& ExperienceOfRepresentingData);
    
    void ReconstructDataVectorByKNNBasisMatrix(DenseMatrix<ElementType>&       ReconstructedDataVector, 
                                               const DenseMatrix<ElementType>& DataVector,
                                               const DenseMatrix<ElementType>& KNNBasisMatrix,
                                               const DenseMatrix<int_max>&     KNNBasisIndexList,
                                               const DenseMatrix<ElementType>& GramianMatrix_DtD);


private:
    KNNBasisSelectionOnlineDictionaryBuilder(const KNNBasisSelectionOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.hpp"

#endif