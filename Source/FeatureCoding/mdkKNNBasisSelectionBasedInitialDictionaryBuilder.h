#ifndef __mdkKNNBasisSelectionBasedInitialDictionaryBuilder_h
#define __mdkKNNBasisSelectionBasedInitialDictionaryBuilder_h

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"
#include "mdkKNNSoftAssignSparseEncoder.h"


namespace mdk
{
template<typename ElementType>
struct Parameter_Of_KNNBasisSelectionBasedInitialDictionaryBuilder 
{
    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType> ParameterOfKNNBasisSelection;

    ElementType SimilarityThreshold_For_Classification;
    // Similarity(i,j) between data vector i and basis vector j in the input dictionary (m_InputDictionary)
    // if Similarity(i,j) >= SimilarityThreshold_For_Classification for some j
    // then the data vector can be represented by the basis, i.e., belong to the class/distribution represented by the input Dictionary
    // 
    // if Similarity(i,j) is very small, i.e., data vector is far away from every basis 
    // then EM-style reconstruction based Dictionary learning methods may not move basis to desired position
    // and therefore, new bases are needed:
    // select new bases from the data vectors that can not be represented by any basis in the input dictionary (m_InputDictionary)

    int_max MaxNumberOfNewBases;

    // if we want to keep the total number of bases to be a constant, 
    // then we need to retire some bases of the input dictionary (m_InputDictionary)
    //
    // To retire some bases of the input dictionary, there are three methods
    // (1) retrieve old datasets, and reselect bases : impractical
    // (2) directly retire old and inexperienced Basis : fast
    // (3) input the bases and the data vectors (classified as in the Class) to KNNBasisSelectionOnlineDictionaryBuilder
    //     set the desired BasisNumber and get output bases : more accurate

    // to use (2) : set DirectlyRetireOldAndInexperiencedBasis to true

    // to use (3) : set DirectlyRetireOldAndInexperiencedBasis to false 
    // (3) can not be used if the number of data vectors (classified as in the Class) is very small
    // at least DataNumber >= NeightbourNumber (K in KNN)

    bool DirectlyRetireOldAndInexperiencedBasis;

    // to directly retire some bases in the input dictionary
    // compute the Experience to Age ratio, BasisExperience / BasisAge
    // sort the ratio list in ascend order, and start to retire 

    // If we want to keep all the input bases, set DoNotRetireAnyBasis to true

    bool DoNotRetireAnyBasis;

};

template<typename ElementType>
class KNNBasisSelectionBasedInitialDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType> m_Parameter;

    typedef MDK_SimilarityType_Enum_For_FeatureCoding SimilarityTypeEnum;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InputDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNBasisSelectionOnlineDictionaryBuilder<ElementType> m_KNNBasisSelectionDictionaryBuilder;

    KNNSoftAssignSparseEncoder<ElementType> m_KNNSoftAssignSparseEncoder;

public:
    KNNBasisSelectionBasedInitialDictionaryBuilder();
    ~KNNBasisSelectionBasedInitialDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InputDictionary);

    void SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    bool CheckInput();

    FeatureDictionaryForSparseCoding<ElementType>* GetOutputDictionary();

protected:

    void SetupDefaultPipelineOutput();

    void UpdatePipelineOutput();

    void GenerateDictionary();


private:
    KNNBasisSelectionBasedInitialDictionaryBuilder(const KNNBasisSelectionBasedInitialDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionBasedInitialDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionBasedInitialDictionaryBuilder.hpp"

#endif