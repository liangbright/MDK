#ifndef __mdkKNNAverageOnlineDictionaryBuilder_h
#define __mdkKNNAverageOnlineDictionaryBuilder_h


#include <random>


#include "mdkFileIO.h"
#include "mdkString.h"
#include "mdkObjectArray.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkLinearLeastSquaresProblemSolver.h"
#include "mdkKNNSoftAssignSparseEncoder.h"
#include "mdkKNNReconstructionSparseEncoder.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNAverageOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    Parameter_Of_KNNSoftAssignSparseEncoder<ScalarType> ParameterOfKNNSoftAssign;

    ScalarType ExperienceDiscountFactor;

    // ------ whether or not use scale factor --------
	// ||x - s*y||, s is scale factor, e.g., 0.9
    bool WhetherToUseScaleFactor;

    // parameter for data sampling --------

    int_max MaxNumberOfDataInEachBatch; // the number of data in each batch/thread

	int_max MaxNumberOfInteration;

    int_max MaxNumberOfThread;

    // parameter for updating dictionary information

    bool Flag_Update_BasisAge;

    bool Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    ScalarType SimilarityThreshold_For_ComputeBasisRedundancy;

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

        MaxNumberOfInteration = 1;

        MaxNumberOfThread = 1;

        Flag_Update_BasisAge = true;

        Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne    = false;
    }
};


template<typename Scalar_Type>
class KNNAverageOnlineDictionaryBuilder : public FeatureDictionaryBuilder<FeatureDictionaryForSparseCoding<Scalar_Type>>
{
public:
	typedef Scalar_Type ScalarType;

public:
    Parameter_Of_KNNAverageOnlineDictionaryBuilder<ScalarType> m_Parameter;

private:

	// input data
    const DenseMatrix<ScalarType>* m_FeatureData;

	// input initial dictionary
    const FeatureDictionaryForSparseCoding<ScalarType>* m_InitialDictionary;

	// output
    FeatureDictionaryForSparseCoding<ScalarType> m_Dictionary;

    KNNSoftAssignSparseEncoder<ScalarType> m_KNNSoftAssignSparseEncoder;

public:
    KNNAverageOnlineDictionaryBuilder();
    ~KNNAverageOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData);

    void SetInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>* InitialDictionary);

    bool CheckInput();

	bool Update();

    FeatureDictionaryForSparseCoding<ScalarType>* GetOutputDictionary();

protected:
    void GenerateDictionary();

    void SetupParameter();

    FeatureDictionaryForSparseCoding<ScalarType> PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>& InitialDictionary);

    void UpdateDictionary(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary, 
                          const DenseMatrix<ScalarType>& FeatureData,
                          const DataArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience(DenseMatrix<ScalarType>&       BasisMatrix,
                                             DenseMatrix<ScalarType>&       BasisExperience,
                                             const DenseMatrix<ScalarType>& FeatureData,
                                             const DataArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience_UseScaleFactor(DenseMatrix<ScalarType>&       BasisMatrix,
                                                            DenseMatrix<ScalarType>&       BasisExperience,
                                                            const DenseMatrix<ScalarType>& FeatureData,
                                                            const DataArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience_NoScaleFactor(DenseMatrix<ScalarType>&       BasisMatrix,
                                                           DenseMatrix<ScalarType>&       BasisExperience,
                                                           const DenseMatrix<ScalarType>& FeatureData,
                                                           const DataArray<SparseVector<ScalarType>>& CodeTable);

    void ApplyConstraintOnBasis(DenseMatrix<ScalarType>& BasisMatrix);

    void UpdateDictionary_OtherInformation(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                                           const DenseMatrix<ScalarType>& BasisExperience_init,
                                           int_max TotalDataNumber);

    void AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, 
                               const DenseMatrix<ScalarType>& BasisExperience_init,
                               int_max TotalDataNumber);

    void UpdateSimilarityMatrix(DenseMatrix<ScalarType>& SimilarityMatrix, 
                                const DenseMatrix<ScalarType>& BasisMatrix, 
                                const DenseMatrix<ScalarType>& VarianceList);

    void UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& SimilarityMatrix);

    void UpdateVarianceOfL1Distance(DenseMatrix<ScalarType>& Variance,
                                    const DenseMatrix<ScalarType>& FeatureData,
                                    const DataArray<SparseVector<ScalarType>>& CodeTable,
                                    const DenseMatrix<ScalarType>& BasisMatrix,
                                    const DenseMatrix<ScalarType>& BasisExperience_init);

    void UpdateVarianceOfL2Distance(DenseMatrix<ScalarType>& Variance,
                                    const DenseMatrix<ScalarType>& FeatureData,
                                    const DataArray<SparseVector<ScalarType>>& CodeTable,
                                    const DenseMatrix<ScalarType>& BasisMatrix,
                                    const DenseMatrix<ScalarType>& BasisExperience_init);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ScalarType>& Variance,
                                      const DenseMatrix<ScalarType>& FeatureData,
                                      const DataArray<SparseVector<ScalarType>>& CodeTable,
                                      const DenseMatrix<ScalarType>& BasisMatrix,
                                      const DenseMatrix<ScalarType>& BasisExperience_init);

    void UpdateVarianceOfReconstruction(DenseMatrix<ScalarType>& Variance,
                                        const DenseMatrix<ScalarType>& FeatureData,
                                        const DataArray<SparseVector<ScalarType>>& CodeTable,
                                        const DenseMatrix<ScalarType>& BasisMatrix,
                                        const DenseMatrix<ScalarType>& BasisExperience_init);

    DenseMatrix<ScalarType> ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  FeatureData,
                                                                 const DataArray<SparseVector<ScalarType>>& CodeTable,
                                                                 const DenseMatrix<ScalarType>&  BasisMatrix);

};


}// namespace mdk


#include "mdkKNNAverageOnlineDictionaryBuilder.hpp"

#endif