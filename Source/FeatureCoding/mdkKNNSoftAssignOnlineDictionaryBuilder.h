#ifndef __mdkKNNSoftAssignOnlineDictionaryBuilder_h
#define __mdkKNNSoftAssignOnlineDictionaryBuilder_h

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
struct Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> ParameterOfKNNSoftAssign;

    // sort the pair i, j according to score = weigth_s * Similarity(i, j) + (1-weigth_s) * 0.5*(prob(i) + prob(j))
    ElementType weigth_s;

    // prob_basis_updated = weigth_past * prob_basis_past + (1-weigth_past) * prob_basis_data;
    ElementType weigth_past; // weight for the past experience, i.e., the initial dictionary

    // parameter for data sampling --------

    int_max NumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfIteration;

    int_max MaxNumberOfThreads;

    // parameter for updating dictionary information

    bool UpdateDictionaryInformation_Other;

    ElementType SimilarityThresholdToComputeBasisRedundancy;

    Parameter_Of_KNNReconstructionSparseEncoder ParameterOfKNNReconstruction;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder() {}

    void Clear()
    {
        BasisNumber = -1;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        ParameterOfKNNSoftAssign.Clear();

        weigth_s = 0;

        weigth_past = 0;

        NumberOfDataInEachBatch = -1;
        MaxNumberOfIteration = -1;

        MaxNumberOfThreads = 1;

        UpdateDictionaryInformation_Other = false;

        SimilarityThresholdToComputeBasisRedundancy = 0;

        ParameterOfKNNReconstruction.Clear();
    }
};


template<typename ElementType>
class KNNSoftAssignOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{

public:
    Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder<ElementType> m_Parameter;

    typedef MDK_SimilarityType_Enum_For_FeatureCoding SimilarityTypeEnum;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;


public:
    KNNSoftAssignOnlineDictionaryBuilder();
    ~KNNSoftAssignOnlineDictionaryBuilder();

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

    void UpdateDictionaryInformation_Necessary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                               const DenseMatrix<ElementType>& FeatureData,
                                               const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                               const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                               const DenseMatrix<int_max>& VectorIndexList_Basis,
                                               const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init);

    void UpdateDictionaryInformation_Other(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, 
                                           const DenseMatrix<ElementType>& FeatureData,
                                           const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                           const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                           const DenseMatrix<int_max>& VectorIndexList_Basis,
                                           const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init);

    DenseMatrix<ElementType> ComputeRepresentativeAbilityOfEachDataVectorInCombinedData(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init,
                                                                                        int_max CombinedDataVectorNumber);

    DenseMatrix<ElementType> ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init,
                                                           const DenseMatrix<ElementType>& FeatureData);

    inline ElementType ComputeSimilarityBetweenTwoDataVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

    DataContainer<DenseMatrix<int_max>> FindKNNVectorIndexTableByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix);

    DenseMatrix<ElementType> EstimateKNNSmoothedProbabilityMassFunctionAtEachVector(const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable);

    DenseMatrix<ElementType> EstimateKNNSmoothedProbabilityMassFunctionAtEachVector(const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable,
                                                                                    const DenseMatrix<ElementType>& RepresentativeAbilityOfEachVector);

    void ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix);

    DataContainer<DenseMatrix<int_max>> FindKNNBasisIndexTableOfDataByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                                             const DenseMatrix<int_max>&     VectorIndexList_Basis,
                                                                                             int_max BasisNumber_init);

    void UpdateProbabilityMassFunctionAtEachBasisVector(DenseMatrix<ElementType>& ProbabilityMassFunction,
                                                        ElementType WeightedNumberOfTrainingSamplesInHistory,
                                                        const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData);

    void UpdateBasisRedundancy(DenseMatrix<ElementType>&       BasisRedundancy,
                               const DenseMatrix<int_max>&     VectorIndexList_Basis,
                               const DenseMatrix<ElementType>& VectorSimilarityMatrix);   

    void UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const ElementType WeightedNumberOfTrainingSamplesInHistory);

    void UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const ElementType WeightedNumberOfTrainingSamplesInHistory);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                                      const DenseMatrix<ElementType>& FeatureData,
                                      const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                      const DenseMatrix<ElementType>& BasisMatrix,
                                      const ElementType WeightedNumberOfTrainingSamplesInHistory);

    void UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                                        const DenseMatrix<ElementType>& FeatureData,
                                        const DataContainer<DenseMatrix<int_max>>& KNNBasisIndexTableOfData,
                                        const DenseMatrix<ElementType>& BasisMatrix,
                                        const ElementType WeightedNumberOfTrainingSamplesInHistory);
    
    void ReconstructDataVectorByKNNBasisMatrix(DenseMatrix<ElementType>&       ReconstructedDataVector, 
                                               const DenseMatrix<ElementType>& DataVector,
                                               const DenseMatrix<ElementType>& KNNBasisMatrix,
                                               const DenseMatrix<int_max>&     KNNBasisIndexList,
                                               const DenseMatrix<ElementType>& GramianMatrix_DtD);


private:
    KNNSoftAssignOnlineDictionaryBuilder(const KNNSoftAssignOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNSoftAssignOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNSoftAssignOnlineDictionaryBuilder.hpp"

#endif