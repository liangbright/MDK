#ifndef __mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder_h
#define __mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder_h


#include <random>


#include "mdkFileIO.h"
#include "mdkString.h"
#include "mdkDataContainer.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkLinearLeastSquaresProblemSolver.h"
#include "mdkKNNSoftAssignSparseEncoder.h"
#include "mdkKNNSoftAssignOnlineDictionaryBuilder.h"


namespace mdk
{

template<typename ElementType>
struct Parameter_Of_KNNSoftAssignAndAverageOnlineDictionaryBuilder
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

    ElementType SimilarityThresholdToComputeBasisRedundancy;

    Parameter_Of_KNNReconstructionSparseEncoder ParameterOfKNNReconstruction;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNSoftAssignAndAverageOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignAndAverageOnlineDictionaryBuilder() {}

    void Clear()
    {
        ParameterOfKNNSoftAssign.Clear();

        BasisNumber = -1;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        weigth_s = -1;

        weigth_past = -1;

        NumberOfDataInEachBatch = -1;
        MaxNumberOfIteration = -1;

        MaxNumberOfThreads = 1;

        SimilarityThresholdToComputeBasisRedundancy = 0;

        ParameterOfKNNReconstruction.Clear();
    }
};


template<typename ElementType>
class KNNSoftAssignAndAverageOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    Parameter_Of_KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType> m_Parameter;

    typedef MDK_SimilarityType_Enum_For_FeatureCoding SimilarityTypeEnum;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNSoftAssignOnlineDictionaryBuilder<ElementType> m_KNNSoftAssignDictionaryBuilder;

    KNNSoftAssignSparseEncoder<ElementType> m_KNNSoftAssignSparseEncoder;

public:
    KNNSoftAssignAndAverageOnlineDictionaryBuilder();
    ~KNNSoftAssignAndAverageOnlineDictionaryBuilder();

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

    void UpdateBasisMatrix(DenseMatrix<ElementType>&       BasisMatrix,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>  WeightOfError);

    inline ElementType ComputeVectorInnerProduct(const DenseMatrix<ElementType>& A, const DenseMatrix<ElementType>& B);

    void ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix);

    void UpdateProbabilityMassFunction(DenseMatrix<ElementType>&  ProbabilityMassFunction, 
                                       ElementType WeightedNumberOfTrainingSamplesInHistory,
                                       const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix, const DenseMatrix<ElementType>& BasisMatrix);

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

    void ReconstructDataVectorByKNNBasisMatrix(DenseMatrix<ElementType>&       ReconstructedDataVector,
                                               const DenseMatrix<ElementType>& DataVector,
                                               const DenseMatrix<ElementType>& KNNBasisMatrix,                                               
                                               const DenseMatrix<int_max>&     KNNBasisIndexList,
                                               const DenseMatrix<ElementType>& GramianMatrix_DtD);
};


}// namespace mdk


#include "mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder.hpp"

#endif