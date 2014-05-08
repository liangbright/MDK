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

    ElementType ExperienceDiscountFactor;

    // parameter for data sampling --------

    int_max MaxNumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfInterations;

    int_max MaxNumberOfThreads;

    // parameter for updating dictionary information

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> ParameterOfKNNSoftAssign;

    ElementType SimilarityThreshold_For_ComputeBasisRedundancy;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNReconstructionOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNReconstructionOnlineDictionaryBuilder() {}

    void Clear()
    {
        BasisNumber = 0;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        ParameterOfKNNReconstruction.Clear();

        ExperienceDiscountFactor = 0;

        MaxNumberOfDataInEachBatch = 0;

        MaxNumberOfInterations = 1;

        MaxNumberOfThreads = 1;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ParameterOfKNNSoftAssign.Clear();
    }
};


template<typename ElementType>
class KNNReconstructionOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    Parameter_Of_KNNReconstructionOnlineDictionaryBuilder<ElementType> m_Parameter;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNReconstructionSparseEncoder<ElementType> m_KNNReconstructionSparseEncoder;

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

    void ClearPipelineOutput();

    void UpdatePipelineOutput();

    void GenerateDictionary();

    void SetupParameter();

    FeatureDictionaryForSparseCoding<ElementType> CopyInitialDictionaryAndDiscountBasisExperience();

    void UpdateDictionary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                          const DenseMatrix<ElementType>& FeatureData,
                          const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience(DenseMatrix<ElementType>&       BasisMatrix,
                                             DenseMatrix<ElementType>&       BasisExperience,
                                             const DenseMatrix<ElementType>& FeatureData,
                                             const DataContainer<SparseVector<ElementType>>& CodeTable,
                                             const DenseMatrix<ElementType>& ReconstructedData);

    void ApplyConstraintOnBasis(DenseMatrix<ElementType>& BasisMatrix);

    void UpdateDictionary_OtherInformation(FeatureDictionaryForSparseCoding<ElementType>& Dictionary,
                                           const DenseMatrix<ElementType>& BasisExperience_init,
                                           int_max TotalDataNumber);

    void AdjustBasisExperience(DenseMatrix<ElementType>& BasisExperience, 
                               const DenseMatrix<ElementType>& BasisExperience_init,
                               int_max TotalDataNumber);

    void UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix, 
                                const DenseMatrix<ElementType>& BasisMatrix, 
                                const DenseMatrix<ElementType>& VarianceList);

    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);

    void UpdateVarianceOfL1Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>& BasisExperience_init);

    void UpdateVarianceOfL2Distance(DenseMatrix<ElementType>& Variance,
                                    const DenseMatrix<ElementType>& FeatureData,
                                    const DataContainer<SparseVector<ElementType>>& CodeTable,
                                    const DenseMatrix<ElementType>& BasisMatrix,
                                    const DenseMatrix<ElementType>& BasisExperience_init);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ElementType>& Variance,
                                      const DenseMatrix<ElementType>& FeatureData,
                                      const DataContainer<SparseVector<ElementType>>& CodeTable,
                                      const DenseMatrix<ElementType>& BasisMatrix,
                                      const DenseMatrix<ElementType>& BasisExperience_init);

    void UpdateVarianceOfReconstruction(DenseMatrix<ElementType>& Variance,
                                        const DenseMatrix<ElementType>& FeatureData,
                                        const DataContainer<SparseVector<ElementType>>& CodeTable,
                                        const DenseMatrix<ElementType>& ReconstructedData,
                                        const DenseMatrix<ElementType>& BasisExperience_init);
};


}// namespace mdk


#include "mdkKNNReconstructionOnlineDictionaryBuilder.hpp"

#endif