#ifndef mdk_KNNAverageDictionaryBuilder_h
#define mdk_KNNAverageDictionaryBuilder_h

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
struct Parameter_Of_KNNAverageDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisCount;

    bool Flag_BasisPositive;
    bool Flag_BasisNormalizedWithL1Norm;
    bool Flag_BasisNormalizedWithL2Norm;

    Parameter_Of_KNNSoftAssignSparseEncoder<ScalarType> ParameterOfKNNSoftAssign;

    ScalarType ExperienceDiscountFactor;

    // ------ whether or not use scale factor --------
	// ||x - s*y||, s is scale factor, e.g., 0.9
    bool Flag_WhetherToUseScaleFactor;

    // parameter for data sampling --------

    int_max MaxCountOfDataInEachBatch; // the number of data in each batch/thread

	int_max MaxCountOfInteration;

	int_max MaxThreadCount;

    // parameter for updating dictionary information

    bool Flag_Update_BasisAge;

    bool Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

    ScalarType SimilarityThreshold_For_ComputeBasisRedundancy;

    Constraint_on_Code_Of_KNNReconstruction_For_FeatureCoding ConstraintOnKNNReconstructionCode;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNAverageDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNAverageDictionaryBuilder() {}

    void Clear()
    {
        ParameterOfKNNSoftAssign.Clear();

        BasisCount = 0;

		Flag_BasisPositive = false;
		Flag_BasisNormalizedWithL1Norm = false;
		Flag_BasisNormalizedWithL2Norm = false;

        ExperienceDiscountFactor = 0;

        Flag_WhetherToUseScaleFactor = false;

        MaxCountOfDataInEachBatch = 0;

        MaxCountOfInteration = 1;

		MaxThreadCount = 1;

        Flag_Update_BasisAge = true;

        Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne    = false;
    }
};


template<typename Scalar_Type>
class KNNAverageDictionaryBuilder : public FeatureDictionaryBuilder<FeatureDictionaryForSparseCoding<Scalar_Type>>
{
public:
	typedef Scalar_Type ScalarType;

public:
    Parameter_Of_KNNAverageDictionaryBuilder<ScalarType> m_Parameter;

private:

	// input data
    const DenseMatrix<ScalarType>* m_FeatureData;

	// input initial dictionary
    const FeatureDictionaryForSparseCoding<ScalarType>* m_InitialDictionary;

	// output
    FeatureDictionaryForSparseCoding<ScalarType> m_Dictionary;

    KNNSoftAssignSparseEncoder<ScalarType> m_KNNSoftAssignSparseEncoder;

public:
    KNNAverageDictionaryBuilder();
    ~KNNAverageDictionaryBuilder();

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
                          const ObjectArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience(DenseMatrix<ScalarType>&       BasisMatrix,
                                             DenseMatrix<ScalarType>&       BasisExperience,
                                             const DenseMatrix<ScalarType>& FeatureData,
                                             const ObjectArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience_UseScaleFactor(DenseMatrix<ScalarType>&       BasisMatrix,
                                                            DenseMatrix<ScalarType>&       BasisExperience,
                                                            const DenseMatrix<ScalarType>& FeatureData,
                                                            const ObjectArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience_NoScaleFactor(DenseMatrix<ScalarType>&       BasisMatrix,
                                                           DenseMatrix<ScalarType>&       BasisExperience,
                                                           const DenseMatrix<ScalarType>& FeatureData,
                                                           const ObjectArray<SparseVector<ScalarType>>& CodeTable);

    void ApplyConstraintOnBasis(DenseMatrix<ScalarType>& BasisMatrix);

    void UpdateDictionary_OtherInformation(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                                           const DenseMatrix<ScalarType>& BasisExperience_init,
                                           int_max TotalDataCount);

    void AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, 
                               const DenseMatrix<ScalarType>& BasisExperience_init,
                               int_max TotalDataCount);

    void UpdateSimilarityMatrix(DenseMatrix<ScalarType>& SimilarityMatrix, 
                                const DenseMatrix<ScalarType>& BasisMatrix, 
                                const DenseMatrix<ScalarType>& VarianceList);

    void UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& SimilarityMatrix);

    void UpdateVarianceOfL1Distance(DenseMatrix<ScalarType>& Variance,
                                    const DenseMatrix<ScalarType>& FeatureData,
                                    const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                    const DenseMatrix<ScalarType>& BasisMatrix,
                                    const DenseMatrix<ScalarType>& BasisExperience_init);

    void UpdateVarianceOfL2Distance(DenseMatrix<ScalarType>& Variance,
                                    const DenseMatrix<ScalarType>& FeatureData,
                                    const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                    const DenseMatrix<ScalarType>& BasisMatrix,
                                    const DenseMatrix<ScalarType>& BasisExperience_init);

    void UpdateVarianceOfKLDivergence(DenseMatrix<ScalarType>& Variance,
                                      const DenseMatrix<ScalarType>& FeatureData,
                                      const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                      const DenseMatrix<ScalarType>& BasisMatrix,
                                      const DenseMatrix<ScalarType>& BasisExperience_init);

    void UpdateVarianceOfReconstruction(DenseMatrix<ScalarType>& Variance,
                                        const DenseMatrix<ScalarType>& FeatureData,
                                        const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                        const DenseMatrix<ScalarType>& BasisMatrix,
                                        const DenseMatrix<ScalarType>& BasisExperience_init);

    DenseMatrix<ScalarType> ComputeDataReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  FeatureData,
                                                                 const ObjectArray<SparseVector<ScalarType>>& CodeTable,
                                                                 const DenseMatrix<ScalarType>&  BasisMatrix);

};


}// namespace mdk


#include "mdkKNNAverageDictionaryBuilder.hpp"

#endif