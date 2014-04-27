#ifndef __mdkKNNSoftAssignOnlineDictionaryBuilder_h
#define __mdkKNNSoftAssignOnlineDictionaryBuilder_h

#include "mdkString.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkKNNSoftAssignSparseEncoder.h"

namespace mdk
{

template<typename ElementType>
struct Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder
{
    int_max BasisNumber;

    int_max NeighbourNumber;

    std::string SimilarityType;
    //
    // If a Feature vector is treated as ordinary signal
    // L1Distance
    // L2Distance
    // Correlation
    //
    // If a Feature vector is normalized histogram or probability mass function (PMF), i.e., discrete probability distribution
    // KLDivergence

    ElementType Sigma_L1; // standard deviation to convert L1Distance to Similarity

    ElementType Sigma_L2; // standard deviation to convert L2Distance to Similarity

    ElementType Sigma_KL; // standard deviation to convert KLDivergence to Similarity

    bool IgnoreSign_Correlation; // if it is true, Similarity = abs(Correlation)
                                 // else, Similarity = (Correlation +1)/2

    // sort the pair i, j according to score = factor_s * Similarity(i, j) + factor_p * (prob(i) + prob(j))
    ElementType weigth_s;
    ElementType weigth_p;

    // prob_basis_updated = weigth_past * prob_basis_past + weigth_current * prob_basis_data;
    ElementType weigth_past; // weight for initial dictionary, i.e., the set of FeatureData that is used long ago
    ElementType weigth_current;  // weight for current FeatureData


    int_max NumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfIteration;

    int_max MaxNumberOfThreads;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignOnlineDictionaryBuilder() {}

    void Clear()
    {
        BasisNumber = 0;

        NeighbourNumber = 0;

        SimilarityType.clear();

        Sigma_L1 = 0;
        Sigma_L2 = 0;
        Sigma_KL = 0;
        IgnoreSign_Correlation = false;

        weigth_s = 0;
        weigth_p = 0;

        weigth_past = 0;
        weigth_new = 0;

        NumberOfDataInEachBatch = 0;
        MaxNumberOfIteration = 0;

        MaxNumberOfThreads = 0;
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

    FeatureDictionaryForSparseCoding<ElementType> m_Dictionary_SharedCopy;

    KNNSoftAssignSparseEncoder<ElementType> m_SparseEncoder;

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

    FeatureDictionaryForSparseCoding<ElementType> ExtractDictionaryFromData(int_max BasisNumber, 
                                                                            const DenseMatrix<ElementType>& FeatureData,
                                                                            const DenseMatrix<ElementType>& ProbabilityMassFunctionOfData);

    inline ElementType ComputeSimilarityBetweenTwoDataVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

    inline DenseMatrix<ElementType> GetProbabilityMassFunctionOfCombinedData(int_max DataVectorNumber);

    inline DataContainer<DenseMatrix<int_max>> FindKNNDataVectorIndexTableBySimilarityMatrix(const DenseMatrix<ElementType>& SimilarityMatrix);

    inline DenseMatrix<ElementType> EstimateBasisProbabilityMassFunctionFromData(const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable);

    inline DenseMatrix<ElementType> EstimateBasisProbabilityMassFunctionFromData(const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable,
                                                                                 const DenseMatrix<ElementType>& DataProbabilityMassFunction);

    inline DataContainer<DenseMatrix<int_max>> FindKNNBasisVectorIndexTableBySimilarityMatrix(const DenseMatrix<int_max>&     DataVectorIndexList_Basis,
                                                                                              const DenseMatrix<ElementType>& SimilarityMatrix);

    inline DenseMatrix<ElementType> EstimateBasisProbabilityMassFunctionFromData(const DataContainer<DenseMatrix<int_max>>& KNNBasisVectorIndexTable,
                                                                                 const DenseMatrix<ElementType>& DataProbabilityMassFunction);
    
    inline DenseMatrix<ElementType> EsimateBasisStandardDeviation(const DenseMatrix<ElementType>& FeatureData, 
                                                                  const DataContainer<DenseMatrix<int_max>>& KNNDataVectorIndexTable,
                                                                  const DenseMatrix<int_max>&     DataVectorIndexList_Basis,
                                                                  const DenseMatrix<ElementType>& BasisMatrix,                                                                                                                                  
                                                                  const DenseMatrix<ElementType>& StandardDeviation_init);

private:
    KNNSoftAssignOnlineDictionaryBuilder(const KNNSoftAssignOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNSoftAssignOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNSoftAssignOnlineDictionaryBuilder.hpp"

#endif