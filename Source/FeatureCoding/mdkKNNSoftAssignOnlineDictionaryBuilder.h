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

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> ParameterOfKNNSoftAssign;

    // sort the pair i, j according to score = weigth_s * Similarity(i, j) + (1-weigth_s) * 0.5*(prob(i) + prob(j))
    ElementType weigth_s;

    // prob_basis_updated = weigth_past * prob_basis_past + (1-weigth_past) * prob_basis_data;
    ElementType weigth_past; // weight for the past experience, i.e., the initial dictionary

    // parameter for data sampling --------

    int_max NumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfIteration;

    int_max MaxNumberOfThreads;

    // parameter for updating reconstruction information after the dictionary is built

    ElementType SimilarityThresholdToComputeBasisRedundancy;

    Parameter_Of_KNNReconstructionSparseEncoder ParameterOfKNNReconstruction;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder() {}

    void Clear()
    {
        BasisNumber = -1;

        ParameterOfKNNSoftAssign.Clear();

        weigth_s = 0;

        weigth_past = 0;

        NumberOfDataInEachBatch = -1;
        MaxNumberOfIteration = -1;

        MaxNumberOfThreads = 1;

        SimilarityThresholdToComputeBasisRedundancy = 0;

        ParameterOfKNNReconstruction.Clear();
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

    FeatureDictionaryForSparseCoding<ElementType> BuildDictionaryFromDataBatch(int_max BasisNumber_desired,
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

    DenseMatrix<int_max> EstimateBasisRedundancy(const DenseMatrix<int_max>&     DataVectorIndexList_Basis,
                                                 const DenseMatrix<ElementType>& SimilarityMatrix);   

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