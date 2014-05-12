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

    // ------ whether or not use scale factor --------

    bool WhetherToUseScaleFactor;

    // parameter for data sampling --------

    int_max MaxNumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfInterations;

    int_max MaxNumberOfThreads;

    // parameter for updating dictionary information

    bool Flag_Update_BasisAge;

    bool Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    ElementType SimilarityThreshold_For_ComputeBasisRedundancy;

    Constraint_on_Code_Of_KNNReconstruction_For_FeatureCoding ConstraintOnKNNReconstructionCode;

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

        WhetherToUseScaleFactor = false;

        MaxNumberOfDataInEachBatch = 0;

        MaxNumberOfInterations = 1;

        MaxNumberOfThreads = 1;

        Flag_Update_BasisAge = true;

        Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne    = false;
    }
};


template<typename ElementType>
class KNNAverageOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    Parameter_Of_KNNAverageOnlineDictionaryBuilder<ElementType> m_Parameter;

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

    void ClearPipelineOutput();

    void UpdatePipelineOutput();

    void GenerateDictionary();

    void SetupParameter();

    FeatureDictionaryForSparseCoding<ElementType> PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>& InitialDictionary);

    void UpdateDictionary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, 
                          const DenseMatrix<ElementType>& FeatureData,
                          const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience(DenseMatrix<ElementType>&       BasisMatrix,
                                             DenseMatrix<ElementType>&       BasisExperience,
                                             const DenseMatrix<ElementType>& FeatureData,
                                             const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience_UseScaleFactor(DenseMatrix<ElementType>&       BasisMatrix,
                                                            DenseMatrix<ElementType>&       BasisExperience,
                                                            const DenseMatrix<ElementType>& FeatureData,
                                                            const DataContainer<SparseVector<ElementType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience_NoScaleFactor(DenseMatrix<ElementType>&       BasisMatrix,
                                                           DenseMatrix<ElementType>&       BasisExperience,
                                                           const DenseMatrix<ElementType>& FeatureData,
                                                           const DataContainer<SparseVector<ElementType>>& CodeTable);

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
                                        const DenseMatrix<ElementType>& BasisMatrix,
                                        const DenseMatrix<ElementType>& BasisExperience_init);

    DenseMatrix<ElementType> ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  FeatureData,
                                                                  const DataContainer<SparseVector<ElementType>>& CodeTable,
                                                                  const DenseMatrix<ElementType>&  BasisMatrix);

};


}// namespace mdk


#include "mdkKNNAverageOnlineDictionaryBuilder.hpp"

#endif