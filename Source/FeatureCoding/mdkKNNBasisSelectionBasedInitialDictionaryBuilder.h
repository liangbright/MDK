#ifndef __mdkKNNBasisSelectionBasedInitialDictionaryBuilder_h
#define __mdkKNNBasisSelectionBasedInitialDictionaryBuilder_h

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"
#include "mdkKNNSimilaritySparseEncoder.h"


namespace mdk
{
template<typename ElementType>
struct Parameter_Of_KNNBasisSelectionBasedInitialDictionaryBuilder : Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType>
{
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

    // If we want to keep all the input bases,
    // set m_Parameter.ParameterOfKNNBasisSelection.BasisNumber = MaxNumberOfNewBases + BasisNumber Of Input Dictionary

//---------------------------------------------------
    Parameter_Of_KNNBasisSelectionBasedInitialDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionBasedInitialDictionaryBuilder() {}

    void Clear()
    {
        // Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder ------

        BasisNumber = 0;

        BasisPositive = false;
        BasisNormalizedWithL1Norm = false;
        BasisNormalizedWithL2Norm = false;

        ParameterOfKNNSoftAssign.NeighbourNumber = 0;
        ParameterOfKNNSoftAssign.SimilarityType = VectorSimilarityTypeEnum::Unknown;
        ParameterOfKNNSoftAssign.SimilarityThreshold = 0;
        ParameterOfKNNSoftAssign.Variance_L1 = 0;
        ParameterOfKNNSoftAssign.Variance_L2 = 0;
        ParameterOfKNNSoftAssign.Variance_KL = 0;

        ExperienceDiscountFactor = 0;

        WeightOnProbabiliyForBasisSelection = 0.5;

        MaxNumberOfDataInEachBatch = 512;

        MaxNumberOfThreads = 1;

        Update_BasisID = true;

        Update_BasisAge = false;

        Update_Variance = false;

        Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        Update_BasisExperience = false;

        Update_SimilarityMatrix = true;

        Update_BasisRedundancy = true;

        SimilarityThreshold_For_ComputingBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne = false;

        DebugInfo.Clear();
        
        //------------------------------------------------------------//

        SimilarityThreshold_For_Classification = 0;

        MaxNumberOfNewBases = 0;

        DirectlyRetireOldAndInexperiencedBasis = false;
    }

    Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType> Get_Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder()
    {
        Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType> SubParameter;

        SubParameter.DictionaryName = DictionaryName;
        
        SubParameter.BasisNumber = BasisNumber;
        
        SubParameter.BasisPositive = BasisPositive;
        SubParameter.BasisNormalizedWithL1Norm = BasisNormalizedWithL1Norm;
        SubParameter.BasisNormalizedWithL2Norm = BasisNormalizedWithL2Norm;

        SubParameter.ParameterOfKNNSoftAssign.NeighbourNumber = ParameterOfKNNSoftAssign.NeighbourNumber;
        SubParameter.ParameterOfKNNSoftAssign.SimilarityType = ParameterOfKNNSoftAssign.SimilarityType;
        SubParameter.ParameterOfKNNSoftAssign.SimilarityThreshold = ParameterOfKNNSoftAssign.SimilarityThreshold;
        SubParameter.ParameterOfKNNSoftAssign.Variance_L1 = ParameterOfKNNSoftAssign.Variance_L1;
        SubParameter.ParameterOfKNNSoftAssign.Variance_L2 = ParameterOfKNNSoftAssign.Variance_L2;
        SubParameter.ParameterOfKNNSoftAssign.Variance_KL = ParameterOfKNNSoftAssign.Variance_KL;

        SubParameter.ExperienceDiscountFactor = ExperienceDiscountFactor;

        SubParameter.WeightOnProbabiliyForBasisSelection = WeightOnProbabiliyForBasisSelection;

        SubParameter.MaxNumberOfDataInEachBatch = MaxNumberOfDataInEachBatch;

        SubParameter.MaxNumberOfThreads = MaxNumberOfThreads;

        SubParameter.Update_BasisID = Update_BasisID;

        SubParameter.Update_BasisAge = Update_BasisAge;

        SubParameter.Update_BasisExperience = Update_BasisExperience;

        SubParameter.Update_Variance = Update_Variance;

        SubParameter.Update_VarianceOfReconstruction_Using_KNNBasisMatrix = Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

        SubParameter.Update_SimilarityMatrix = Update_SimilarityMatrix;

        SubParameter.Update_BasisRedundancy = Update_BasisRedundancy;

        SubParameter.SimilarityThreshold_For_ComputingBasisRedundancy = SimilarityThreshold_For_ComputingBasisRedundancy;

        SubParameter.ConstraintOnKNNReconstructionCode = ConstraintOnKNNReconstructionCode;

        SubParameter.DebugInfo = DebugInfo;

        return SubParameter;
    }
};


template<typename ElementType>
class KNNBasisSelectionBasedInitialDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    Parameter_Of_KNNBasisSelectionBasedInitialDictionaryBuilder<ElementType> m_Parameter;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InputDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNBasisSelectionOnlineDictionaryBuilder<ElementType> m_KNNBasisSelectionDictionaryBuilder;

    KNNSimilaritySparseEncoder<ElementType> m_KNNSimilaritySparseEncoder;

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

    void ClassifyFeatureDataBySimilarityThreshold(DenseMatrix<int_max>& IndexList_InClass, 
                                                  DenseMatrix<int_max>& IndexList_OutClass, 
                                                  const DataContainer<SparseVector<ElementType>>& CodeTable);

    void DirectlyRetireBasisInInputDictionary(FeatureDictionaryForSparseCoding<ElementType>& InputDictionary_Modified,
                                              const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary, 
                                              int_max BasisNumber_to_keep);

    void UpdateDictionaryInformationInModifiedInputDictionary(FeatureDictionaryForSparseCoding<ElementType>& InputDictionary_Modified);

    void UpdateSimilarityMatrix(DenseMatrix<ElementType>& SimilarityMatrix,
                                const DenseMatrix<ElementType>& BasisMatrix,
                                const DenseMatrix<ElementType>& VarianceList);

    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);

    void CombineModifiedInputDictionaryAndNewDictionary(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary,
                                                        const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary,
                                                        const FeatureDictionaryForSparseCoding<ElementType>& NewDictionary);

    void UpdateSimilarityMatrixInCombinedDictionary(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary, int_max BasisNumber_input);

private:
    KNNBasisSelectionBasedInitialDictionaryBuilder(const KNNBasisSelectionBasedInitialDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionBasedInitialDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionBasedInitialDictionaryBuilder.hpp"

#endif