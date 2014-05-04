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

struct DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder
{
    bool Flag_OutputDebugInfo;

    CharString FilePathToSaveDebugInfo;

    CharString JsonDataFileName_Of_VectorSimilarityMatrix;

//---------------------------------------------------
    DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder() { this->Clear(); }
    ~DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder() {}

    void Clear()
    {
        Flag_OutputDebugInfo = false;
        FilePathToSaveDebugInfo.Clear();
        JsonDataFileName_Of_VectorSimilarityMatrix.Clear();
    }
};



template<typename ElementType>
struct Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> ParameterOfKNNSoftAssign;

    ElementType ExperienceDiscountFactor; 
    // weight for the past experience when new training data is used
    // When to use the weight : 
    // discount the Experience of dictionary right after training is done on new data
    // implemented in UpdateBasisExperienceForEachBasisVector(...)

    ElementType WeightOnProbabiliyForBasisSelection;
    // WeightOnProbabiliyForBasisSelection to sort vector pair    
    // range [0, 1]

    // parameter for data sampling --------

    int_max MaxNumberOfDataInEachBatch; // the maximum number of data in each batch/thread

    int_max MaxNumberOfThreads;

    // parameter for updating dictionary information

    bool Update_BasisAge;

    bool Update_BasisExperience;

    bool Update_Variance;

    bool Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    bool Update_SimilarityMatrix;

    bool Update_BasisRedundancy;

    ElementType SimilarityThresholdToComputeBasisRedundancy;

    Constraint_on_Code_Of_KNNReconstruction_For_FeatureCoding ConstraintOnKNNReconstructionCode;

    // parameter for debug information output
    DebugInfo_Of_KNNBasisSelectionOnlineDictionaryBuilder DebugInfo;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder() {}

    void Clear()
    {
        BasisNumber = 0;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        ParameterOfKNNSoftAssign.Clear();

        ExperienceDiscountFactor = 0;

        WeightOnProbabiliyForBasisSelection = 0;

        MaxNumberOfDataInEachBatch = 0;

        MaxNumberOfThreads = 1;

        Update_BasisAge = false;

        Update_Variance = false;

        Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        Update_BasisExperience  = false;

        Update_SimilarityMatrix = false;

        Update_BasisRedundancy  = false;

        SimilarityThresholdToComputeBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne    = false;

        DebugInfo.Clear();
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

    FeatureDictionaryForSparseCoding<ElementType> BuildDictionaryFromData(const int_max BasisNumber_desired,
                                                                          const DenseMatrix<ElementType>& FeatureData,
                                                                          const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init);

    DenseMatrix<int_max> SelectBasisFromCombinedDataBySimilarityAndProbability(const int_max BasisNumber_desired,
                                                                               const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                               const DenseMatrix<ElementType>& ProbabilityOfEachVector);

    void UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                     const DenseMatrix<ElementType>& FeatureData,
                                     const DataContainer<SparseVector<ElementType>>& CodeTable,
                                     const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                     const DenseMatrix<int_max>& VectorIndexList_Basis,
                                     const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init);

    void UpdateDictionaryInformation_Variance(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                              const DenseMatrix<ElementType>& FeatureData,
                                              const DataContainer<SparseVector<ElementType>>& CodeTable,
                                              const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                              const DenseMatrix<int_max>& VectorIndexList_Basis,
                                              const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init);

    DenseMatrix<ElementType> ComputeRepresentativeAbilityOfEachVectorInCombinedData(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init,
                                                                                    int_max CombinedDataVectorNumber);

    DenseMatrix<ElementType> ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init,
                                                           const DenseMatrix<ElementType>& FeatureData);

    inline ElementType ComputeSimilarityBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length);

    DataContainer<DenseMatrix<int_max>> FindKNNVectorIndexTableByVectorSimilarityMatrix(const DenseMatrix<ElementType>& VectorSimilarityMatrix);
                    
    DenseMatrix<ElementType> EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                                            const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable);
        
    DenseMatrix<ElementType> EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachVector(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                                                            const DataContainer<DenseMatrix<int_max>>& KNNVectorIndexTable,
                                                                                            const DenseMatrix<ElementType>& RepresentativeAbilityOfEachVector);

    void ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix);

    DataContainer<SparseVector<ElementType>> EncodeFeatureData(const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                                               const DenseMatrix<int_max>&     VectorIndexList_Basis,
                                                               int_max BasisNumber_init);

    void UpdateBasisExperience(DenseMatrix<ElementType>& BasisExperience, const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);   

    void UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>& BasisExperience);

    void UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>& BasisExperience);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                                      const DenseMatrix<ElementType>& FeatureData,
                                      const DataContainer<SparseVector<ElementType>>& CodeTable,
                                      const DenseMatrix<ElementType>& BasisMatrix,
                                      const DenseMatrix<ElementType>& BasisExperience);

    void UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                                        const DenseMatrix<ElementType>& FeatureData,
                                        const DataContainer<SparseVector<ElementType>>& CodeTable,
                                        const DenseMatrix<ElementType>& BasisMatrix,
                                        const DenseMatrix<ElementType>& BasisExperience);
    
    DenseMatrix<ElementType> ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  FeatureData,
                                                                  const DataContainer<SparseVector<ElementType>>& CodeTable,
                                                                  const DenseMatrix<ElementType>&  BasisMatrix);

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