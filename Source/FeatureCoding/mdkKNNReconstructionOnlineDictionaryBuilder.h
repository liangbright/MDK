#ifndef __mdkKNNReconstructionOnlineDictionaryBuilder_h
#define __mdkKNNReconstructionOnlineDictionaryBuilder_h


#include <random>


#include "mdkFileIO.h"
#include "mdkString.h"
#include "mdkDataContainer.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkLinearLeastSquaresProblemSolver.h"
#include "mdkKNNReconstructionSparseEncoder.h"


namespace mdk
{

template<typename ElementType>
struct Parameter_Of_KNNReconstructionOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    Parameter_Of_KNNReconstructionSparseEncoder<ElementType> ParameterOfKNNReconstruction;

    // sort the pair i, j according to score = weigth_s * Similarity(i, j) + (1-weigth_s) * 0.5*(prob(i) + prob(j))
    ElementType weigth_s;

    // prob_basis_updated = weigth_past * prob_basis_past + (1-weigth_past) * prob_basis_data;
    ElementType weigth_past; // weight for the past experience, i.e., the initial dictionary

    // parameter for data sampling --------

    int_max NumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfIteration;

    int_max MaxNumberOfThreads;

    // parameter for updating dictionary information

    ElementType SimilarityThresholdToComputeBasisRedundancy;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNReconstructionOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNReconstructionOnlineDictionaryBuilder() {}

    void Clear()
    {
        ParameterOfKNNSoftAssign.Clear();

        BasisNumber = 0;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        ParameterOfKNNReconstruction.Clear();

        weigth_s = 0;

        weigth_past = 0;

        NumberOfDataInEachBatch = 0;
        MaxNumberOfIteration = 0;

        MaxNumberOfThreads = 1;

        SimilarityThresholdToComputeBasisRedundancy = 0;

        ParameterOfKNNReconstruction.Clear();
    }
};


template<typename ElementType>
class KNNReconstructionOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    Parameter_Of_KNNReconstructionOnlineDictionaryBuilder<ElementType> m_Parameter;

    typedef MDK_SimilarityType_Enum_For_FeatureCoding SimilarityTypeEnum;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNSoftAssignSparseEncoder<ElementType> m_KNNReconstructionSparseEncoder;

public:
    KNNReconstructionOnlineDictionaryBuilder();
    ~KNNReconstructionOnlineDictionaryBuilder();

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

    void SetputParameter();

    FeatureDictionaryForSparseCoding<ElementType> BuildDictionaryFromData(int_max BasisNumber_desired,
                                                                          const DenseMatrix<ElementType>& FeatureData,
                                                                          const DenseMatrix<ElementType>& ProbabilityMassFunctionOfData,
                                                                          const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init);

    void UpdateDictionaryAfterEncoding(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                       const DenseMatrix<ElementType>& FeatureData,
                                       const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateDictionary_OtherInfo(FeatureDictionaryForSparseCoding<ElementType>& Dictionary);

    void ReconstructFeatureData(DenseMatrix<ElementType>&        ReconstructedData,
                                const DenseMatrix<ElementType>&  BasisMatrix,
                                const DataContainer<SparseVector<ElementType>>& CodeTable)

    void UpdateBasisMatrix(DenseMatrix<ElementType>&       BasisMatrix,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>& ReconstructedData,
                           DenseMatrix<ElementType>&       ProbabilityMassFunction,
                           ElementType WeightedNumberOfTrainingSamplesInHistory);

    void ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix);

    void UpdateProbabilityMassFunction(DenseMatrix<ElementType>&  ProbabilityMassFunction, 
                                       ElementType WeightedNumberOfTrainingSamplesInHistory,
                                       const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix, 
                                const DenseMatrix<ElementType>& BasisMatrix, 
                                const DenseMatrix<ElementType>& VarianceList);

    ElementType ComputeSimilarityBetweenTwoDataVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, ElementType Variance);

    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);

    void UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const ElementType WeightedNumberOfTrainingSamplesInHistory);

    void UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const ElementType WeightedNumberOfTrainingSamplesInHistory);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                                      const DenseMatrix<ElementType>& FeatureData,
                                      const DataContainer<SparseVector<ElementType>>& CodeTable,
                                      const DenseMatrix<ElementType>& BasisMatrix,
                                      const ElementType WeightedNumberOfTrainingSamplesInHistory);

    void UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                                        const DenseMatrix<ElementType>& FeatureData,
                                        const DataContainer<SparseVector<ElementType>>& CodeTable,
                                        const DenseMatrix<ElementType>& BasisMatrix,
                                        const ElementType WeightedNumberOfTrainingSamplesInHistory);
};


}// namespace mdk


#include "mdkKNNReconstructionOnlineDictionaryBuilder.hpp"

#endif