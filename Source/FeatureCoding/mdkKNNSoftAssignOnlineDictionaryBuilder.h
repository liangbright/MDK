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

namespace mdk
{

template<typename ElementType>
struct Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    int_max MaxNumberOfNeighbours;

    std::string SimilarityType;
    //
    // If a Feature data vector is treated as ordinary signal
    // L1Distance
    // L2Distance
    // Correlation
    //
    // If a Feature data vector is normalized histogram or probability mass function (PMF), i.e., discrete probability distribution
    // KLDivergence

    ElementType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold
                                     // K in KNN can be < MaxNumberOfNeighbours

    ElementType Sigma_L1; // standard deviation to convert L1Distance to Similarity

    ElementType Sigma_L2; // standard deviation to convert L2Distance to Similarity

    ElementType Sigma_KL; // standard deviation to convert KLDivergence to Similarity

    bool IgnoreSign_Correlation; // if it is true, Similarity = abs(Correlation)
                                 // else, Similarity = (Correlation +1)/2

    // sort the pair i, j according to score = weigth_s * Similarity(i, j) + (1-weigth_s) * 0.5*(prob(i) + prob(j))
    ElementType weigth_s;

    // prob_basis_updated = weigth_past * prob_basis_past + (1-weigth_past) * prob_basis_data;
    ElementType weigth_past; // weight for the past experience, i.e., the initial dictionary

    // parameter for reconstruction

    bool KNNReconstruction_CodeNonnegative;

    bool KNNReconstruction_CodeSumToOne;

    // if a Feature data vector is PMF
    bool WhetherToEstimateStandardDeviationOfKLDivergence;

    // parameter for data sampling --------

    int_max NumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfIteration;

    int_max MaxNumberOfThreads;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder() {}

    void Clear()
    {
        BasisNumber = -1;

        MaxNumberOfNeighbours = -1;

        SimilarityType.clear();

        SimilarityThreshold = 0.1;

        Sigma_L1 = 0;
        Sigma_L2 = 0;
        Sigma_KL = 0;
        IgnoreSign_Correlation = false;

        weigth_s = 0;

        weigth_past = 0;

        KNNReconstruction_CodeNonnegative = false;
        KNNReconstruction_CodeSumToOne = false;

        WhetherToEstimateStandardDeviationOfKLDivergence = true;

        NumberOfDataInEachBatch = -1;
        MaxNumberOfIteration = -1;

        MaxNumberOfThreads = 1;
    }
};


template<typename ElementType>
class KNNSoftAssignOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{

public:
    Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder<ElementType> m_Parameter;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    // for multi thread

    DataContainer<FeatureDictionaryForSparseCoding<ElementType>> m_SubDictionaryList;

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

    void GenerateDictionary_in_a_thread();

    FeatureDictionaryForSparseCoding<ElementType> ExtractDictionaryFromDataBatch(int_max BasisNumber_desired,
                                                                                 const DenseMatrix<ElementType>& FeatureData,
                                                                                 const DenseMatrix<ElementType>& ProbabilityMassFunctionOfData,
                                                                                 const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init);

    DenseMatrix<ElementType> GetProbabilityMassFunctionOfCombinedData(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init, 
                                                                      int_max CombinedDataVectorNumber);

    template<typename SimilarityFunctionType>
    DenseMatrix<ElementType> ComputeSimilarityMatrix(const DenseMatrix<ElementType>& FeatureData, SimilarityFunctionType SimilarityFunction);

    inline ElementType ComputeSimilarityBetweenTwoDataVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

    DataContainer<DenseMatrix<int_max>> FindKNNDataVectorIndexTableBySimilarityMatrix(const DenseMatrix<ElementType>& SimilarityMatrix);

    DenseMatrix<ElementType> EstimateBasisProbabilityMassFunction_Data(const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable);

    DenseMatrix<ElementType> EstimateBasisProbabilityMassFunction_Data(const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable,
                                                                       const DenseMatrix<ElementType>& DataProbabilityMassFunction);

    DataContainer<DenseMatrix<int_max>> FindKNNBasisVectorIndexTableBySimilarityMatrix(const DenseMatrix<int_max>&     DataVectorIndexList_Basis,
                                                                                       const DenseMatrix<ElementType>& SimilarityMatrix);

    DenseMatrix<ElementType> EstimateBasisProbabilityMassFunction_Basis(int_max BasisNumber, 
                                                                        const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                                                        const DenseMatrix<ElementType>& DataProbabilityMassFunction);

    DenseMatrix<ElementType> EsimateBasisStandardDeviationOfL1Distance(const DenseMatrix<ElementType>& FeatureData,
                                                                       const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                                                       const DenseMatrix<ElementType>& BasisMatrix,                                                                                                                                  
                                                                       const DenseMatrix<ElementType>& StandardDeviation_init);

    DenseMatrix<ElementType> EsimateBasisStandardDeviationOfL2Distance(const DenseMatrix<ElementType>& FeatureData,
                                                                       const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                                                       const DenseMatrix<ElementType>& BasisMatrix,                                                                                                                                  
                                                                       const DenseMatrix<ElementType>& StandardDeviation_init);

    DenseMatrix<ElementType> EsimateBasisStandardDeviationOfKLDivergence(const DenseMatrix<ElementType>& FeatureData,
                                                                         const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                                                         const DenseMatrix<ElementType>& BasisMatrix,                                                                                                                                  
                                                                         const DenseMatrix<ElementType>& StandardDeviation_init);

    DenseMatrix<ElementType> ReconstructDataVectorByKNNBasisMatrix(const DenseMatrix<ElementType>& DataVector,    
                                                                   const DenseMatrix<ElementType>& KNNBasisMatrix,
                                                                   const DenseMatrix<ElementType>& GramianMatrix_DtD,
                                                                   const DenseMatrix<int_max>&     KNNBasisIndexList);

    DenseMatrix<ElementType> EsimateBasisStandardDeviationOfReconstruction(const DenseMatrix<ElementType>& FeatureData,
                                                                           const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                                                           const DenseMatrix<ElementType>& BasisMatrix,                                                                                                                                  
                                                                           const DenseMatrix<ElementType>& StandardDeviation_init);

private:
    KNNSoftAssignOnlineDictionaryBuilder(const KNNSoftAssignOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNSoftAssignOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNSoftAssignOnlineDictionaryBuilder.hpp"

#endif