#ifndef __mdkKNNReconstructionOnlineDictionaryBuilder_h
#define __mdkKNNReconstructionOnlineDictionaryBuilder_h

#include <random>

#include "mdkFileIO.h"
#include "mdkString.h"
#include "mdkObjectArray.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkLinearLeastSquaresProblemSolver.h"
#include "mdkKNNReconstructionSparseEncoder.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNReconstructionOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    Parameter_Of_KNNReconstructionSparseEncoder<ScalarType> ParameterOfKNNReconstruction;

    ScalarType ExperienceDiscountFactor;

    // parameter for data sampling --------

    int_max MaxNumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfInterations;

    int_max MaxNumberOfThread;

    // parameter for updating dictionary information

    Parameter_Of_KNNSoftAssignSparseEncoder<ScalarType> ParameterOfKNNSoftAssign;

    ScalarType SimilarityThreshold_For_ComputeBasisRedundancy;

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

        MaxNumberOfThread = 1;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ParameterOfKNNSoftAssign.Clear();
    }
};


template<typename Scalar_Type>
class KNNReconstructionOnlineDictionaryBuilder : public FeatureDictionaryBuilder<FeatureDictionaryForSparseCoding<Scalar_Type>>
{
public:
	typedef Scalar_Type ScalarType;
	typedef FeatureDictionaryForSparseCoding<ScalarType> DictionaryType;

public:
    Parameter_Of_KNNReconstructionOnlineDictionaryBuilder<ScalarType> m_Parameter;

private:
    const DenseMatrix<ScalarType>* m_FeatureData;

	const DictionaryType* m_InitialDictionary;

	DictionaryType m_Dictionary;

    KNNReconstructionSparseEncoder<ScalarType> m_KNNReconstructionSparseEncoder;

public:
    KNNReconstructionOnlineDictionaryBuilder();
    ~KNNReconstructionOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData);

	void SetInitialDictionary(const DictionaryType* InitialDictionary);

    bool CheckInput();

	bool Update();

	DictionaryType* GetOutputDictionary();

protected:
    void GenerateDictionary();

    void SetupParameter();

	DictionaryType PreprocessInitialDictionary(const DictionaryType& InitialDictionary);

	void UpdateDictionary(DictionaryType& Dictionary,
                          const DenseMatrix<ScalarType>& FeatureData,
                          const DataArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience(DenseMatrix<ScalarType>&       BasisMatrix,
                                             DenseMatrix<ScalarType>&       BasisExperience,
                                             const DenseMatrix<ScalarType>& FeatureData,
                                             const DataArray<SparseVector<ScalarType>>& CodeTable,
                                             const DenseMatrix<ScalarType>& ReconstructedData);

    void ApplyConstraintOnBasis(DenseMatrix<ScalarType>& BasisMatrix);

	void UpdateDictionary_OtherInformation(DictionaryType& Dictionary,
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
                                        const DenseMatrix<ScalarType>& ReconstructedData,
                                        const DenseMatrix<ScalarType>& BasisExperience_init);
};


}// namespace mdk


#include "mdkKNNReconstructionOnlineDictionaryBuilder.hpp"

#endif