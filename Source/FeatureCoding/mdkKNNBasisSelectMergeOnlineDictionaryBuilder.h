#ifndef __mdkKNNBasisSelectMergeOnlineDictionaryBuilder_h
#define __mdkKNNBasisSelectMergeOnlineDictionaryBuilder_h

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"

namespace mdk
{

// This is a modified version of KNNBasisSelectionOnlineDictionaryBuilder

template<typename Scalar_Type>
class KNNBasisSelectMergeOnlineDictionaryBuilder : public FeatureDictionaryBuilder<FeatureDictionaryForSparseCoding<Scalar_Type>>
{
public:
	typedef Scalar_Type ScalarType;
	typedef FeatureDictionaryForSparseCoding<ScalarType>  DictionaryType;

public:
    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ScalarType> m_Parameter;

private:

    const DenseMatrix<ScalarType>* m_FeatureData;//input data

	const DictionaryType* m_InitialDictionary;// input initial dictionary

	DictionaryType m_Dictionary;// output dictionary

    KNNBasisSelectionOnlineDictionaryBuilder<ScalarType> m_KNNBasisSelectionDictionaryBuilder;

    KNNSimilaritySparseEncoder<ScalarType> m_KNNSimilaritySparseEncoder;

public:
    KNNBasisSelectMergeOnlineDictionaryBuilder();
    ~KNNBasisSelectMergeOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData);

	void SetInitialDictionary(const DictionaryType* InitialDictionary);

    bool CheckInput();

	bool Update();

	DictionaryType* GetOutputDictionary();

protected:
    void GenerateDictionary();

    FeatureDictionaryForSparseCoding<ScalarType> PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>& InitialDictionary);

    DenseMatrix<int_max> ComputeDataNumberInEachBatch(int_max TotalDataNumber);

    FeatureDictionaryForSparseCoding<ScalarType> BuildDictionaryFromDataBatch(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init,
                                                                               const DenseMatrix<ScalarType>& FeatureData);

    FeatureDictionaryForSparseCoding<ScalarType> CombineInitalDictionaryAndNewDictionary(const FeatureDictionaryForSparseCoding<ScalarType>& InitalDictionary,
                                                                                          const FeatureDictionaryForSparseCoding<ScalarType>& NewDictionary);

    void AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, int_max DataNumber, ScalarType TotalExperience_init);

    void UpdateDictionaryInformation_Other(FeatureDictionaryForSparseCoding<ScalarType>& CombinedDictionary, int_max DataNumber);

    DenseMatrix<ScalarType> ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ScalarType>& Dictionary_init, 
                                                           const DenseMatrix<ScalarType>& BasisMatrix_new);

    DenseMatrix<ScalarType> ComputeVectorProbabilityList(const DenseMatrix<ScalarType>& BasisExperience_init,
                                                          const DenseMatrix<ScalarType>& BasisExperience_new);

    DenseMatrix<int_max> SelectBasis(int_max BasisNumber_desired,
                                     const DenseMatrix<ScalarType>& VectorSimilarityMatrix,
                                     const DenseMatrix<ScalarType>& VectorProbabilityList);

    
    void UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& SimilarityMatrix);

private:
    KNNBasisSelectMergeOnlineDictionaryBuilder(const KNNBasisSelectMergeOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectMergeOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectMergeOnlineDictionaryBuilder.hpp"

#endif