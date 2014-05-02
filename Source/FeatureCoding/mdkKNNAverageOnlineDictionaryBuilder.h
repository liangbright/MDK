#ifndef __mdkKNNAverageOnlineDictionaryBuilder_h
#define __mdkKNNAverageOnlineDictionaryBuilder_h


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
struct Parameter_Of_KNNAverageOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> ParameterOfKNNSoftAssign;

    ElementType ExperienceDiscountFactor;

    // parameter for data sampling --------

    int_max MaxNumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfThreads;

    // parameter for updating dictionary information

    bool Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    ElementType SimilarityThresholdToComputeBasisRedundancy;

    Parameter_Of_KNNReconstructionSparseEncoder ParameterOfKNNReconstruction;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNAverageOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNAverageOnlineDictionaryBuilder() {}

    void Clear()
    {
        ParameterOfKNNSoftAssign.Clear();

        BasisNumber = 0;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        ExperienceDiscountFactor = 0;

        MaxNumberOfDataInEachBatch = 0;

        MaxNumberOfThreads = 1;

        Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        SimilarityThresholdToComputeBasisRedundancy = 0;

        ParameterOfKNNReconstruction.Clear();
    }
};


template<typename ElementType>
class KNNAverageOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    Parameter_Of_KNNAverageOnlineDictionaryBuilder<ElementType> m_Parameter;

    typedef MDK_SimilarityType_Enum_For_FeatureCoding SimilarityTypeEnum;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNSoftAssignSparseEncoder<ElementType> m_KNNSoftAssignSparseEncoder;

public:
    KNNAverageOnlineDictionaryBuilder();
    ~KNNAverageOnlineDictionaryBuilder();

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

    void SetupParameter();

    void UpdateBasisMatrix(DenseMatrix<ElementType>&       BasisMatrix,
                           const DenseMatrix<ElementType>& FeatureData,
                           const DataContainer<SparseVector<ElementType>>& CodeTable,
                           const DenseMatrix<ElementType>  BasisExperience);

    void ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix);

    void UpdateDictionaryInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                     const DenseMatrix<ElementType>& FeatureData,
                                     const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateDictionaryInformation_Other(FeatureDictionaryForSparseCoding<ElementType>& Dictionary);

    void UpdateBasisExperience(DenseMatrix<ElementType>&  BasisExperience, const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix, 
                                const DenseMatrix<ElementType>& BasisMatrix, 
                                const DenseMatrix<ElementType>& VarianceList);

    ElementType ComputeSimilarityBetweenTwoDataVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, ElementType Variance);

    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);

    void UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>  BasisExperience);

    void UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>  BasisExperience);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                                      const DenseMatrix<ElementType>& FeatureData,
                                      const DataContainer<SparseVector<ElementType>>& CodeTable,
                                      const DenseMatrix<ElementType>& BasisMatrix,
                                      const DenseMatrix<ElementType>  BasisExperience);

    void UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                                        const DenseMatrix<ElementType>& FeatureData,
                                        const DataContainer<SparseVector<ElementType>>& CodeTable,
                                        const DenseMatrix<ElementType>& BasisMatrix,
                                        const DenseMatrix<ElementType>  BasisExperience);

    DenseMatrix<ElementType> ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  FeatureData,
                                                                  const DataContainer<SparseVector<ElementType>>& CodeTable,
                                                                  const DenseMatrix<ElementType>&  BasisMatrix);

    void ReconstructDataVectorByKNNBasisMatrix(DenseMatrix<ElementType>&       ReconstructedDataVector,
                                               const DenseMatrix<ElementType>& DataVector,
                                               const DenseMatrix<ElementType>& KNNBasisMatrix,                                               
                                               const std::vector<int_max>&     KNNBasisIndexList,
                                               const DenseMatrix<ElementType>& GramianMatrix_DtD);
};


}// namespace mdk


#include "mdkKNNAverageOnlineDictionaryBuilder.hpp"

#endif