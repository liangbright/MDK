#ifndef __mdkKNNBasisSelectMergeOnlineDictionaryBuilder_h
#define __mdkKNNBasisSelectMergeOnlineDictionaryBuilder_h

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"

namespace mdk
{

// This is a modified version of KNNBasisSelectionOnlineDictionaryBuilder

template<typename Element_Type>
class KNNBasisSelectMergeOnlineDictionaryBuilder : public FeatureDictionaryBuilder<Element_Type>
{
public:
	typedef Element_Type ElementType;

public:
    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType> m_Parameter;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNBasisSelectionOnlineDictionaryBuilder<ElementType> m_KNNBasisSelectionDictionaryBuilder;

    KNNSimilaritySparseEncoder<ElementType> m_KNNSimilaritySparseEncoder;

public:
    KNNBasisSelectMergeOnlineDictionaryBuilder();
    ~KNNBasisSelectMergeOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InputDictionary);

    void SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    bool CheckInput();

    FeatureDictionaryForSparseCoding<ElementType>* GetOutputDictionary();

protected:

    void ClearPipelineOutput();

    void UpdatePipelineOutput();

    void GenerateDictionary();

    FeatureDictionaryForSparseCoding<ElementType> PreprocessInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>& InitialDictionary);

    DenseMatrix<int_max> ComputeDataNumberInEachBatch(int_max TotalDataNumber);

    FeatureDictionaryForSparseCoding<ElementType> BuildDictionaryFromDataBatch(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init,
                                                                               const DenseMatrix<ElementType>& FeatureData);

    FeatureDictionaryForSparseCoding<ElementType> CombineInitalDictionaryAndNewDictionary(const FeatureDictionaryForSparseCoding<ElementType>& InitalDictionary,
                                                                                          const FeatureDictionaryForSparseCoding<ElementType>& NewDictionary);

    void AdjustBasisExperience(DenseMatrix<ElementType>& BasisExperience, int_max DataNumber, ElementType TotalExperience_init);

    void UpdateDictionaryInformation_Other(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary, int_max DataNumber);

    DenseMatrix<ElementType> ComputeVectorSimilarityMatrix(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary_init, 
                                                           const DenseMatrix<ElementType>& BasisMatrix_new);

    DenseMatrix<ElementType> ComputeVectorProbabilityList(const DenseMatrix<ElementType>& BasisExperience_init,
                                                          const DenseMatrix<ElementType>& BasisExperience_new);

    DenseMatrix<int_max> SelectBasis(int_max BasisNumber_desired,
                                     const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                     const DenseMatrix<ElementType>& VectorProbabilityList);

    
    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);

private:
    KNNBasisSelectMergeOnlineDictionaryBuilder(const KNNBasisSelectMergeOnlineDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectMergeOnlineDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectMergeOnlineDictionaryBuilder.hpp"

#endif