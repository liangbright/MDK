#ifndef __mdkKNNBasisSelectionBasedInitialDictionaryBuilder_h
#define __mdkKNNBasisSelectionBasedInitialDictionaryBuilder_h

#include "mdkKNNBasisSelectionOnlineDictionaryBuilder.h"
#include "mdkKNNSimilaritySparseEncoder.h"


namespace mdk
{

// This is an extended version of KNNBasisSelectionOnlineDictionaryBuilder
// It is used to build an initial dictionary and pass this initial dictionary to other dictionary-builder


template<typename ElementType>
struct Parameter_Of_KNNBasisSelectionBasedInitialDictionaryBuilder : Parameter_Of_KNNBasisSelectionOnlineDictionaryBuilder<ElementType>
{
    //---------------------------------------- Strategy --------------------------------------------------------------------//
    // select some new bases and keep some old bases and retire some old bases
    // Step-1
    // Select some new bases by using StrategyForNewBasisGeneration
    //
    // new bases are from OutClass data, the number is BasisNumber
    // use old/input bases to vote new bases by using KNN method : one old basis can only vote its KNN neighbour
    // keep the new bases that have the least number of votes,     
    // the maximum number of new bases to keep = BasisNumber - BasisNumber_Of_InputDictionary
    //
    // Step-2
    // combine old bases and new bases by using StrategyForDictionaryCombination
    //
    // repeat from Step-1 to Step-2 in each data batch
    //-----------------------------------------------------------------------------------------------------------------------//

    //----------------- StrategyForNewBasisGeneration (memory efficient) ----------------------------------------------------//
    // Step-1
    // divide the data into two classes, InClass and OutClass
    // InClass:  the class represented by the input bases
    // OutClass: the class not represented by the input bases    
    // Step-2
    // Select new bases from OutClass by using KNNBasisSelectionOnlineDictionaryBuilder
    //------------------------------------------------------------------------------------------------------------------------//

    //----------------- StrategyForNewBasisGeneration (not memory efficient, just put here for reference) --------------------//
    // Step-1
    // divide the data into two classes, InClass and OutClass
    // InClass:  the class represented by the input bases
    // OutClass: the class not represented by the input bases    
    // Step-2
    // divide the InClass data to sub datasets, each dataset should be as large as possible    
    // do NOT Mix the input bases and any sub dataset
    // Step-3
    // Select new bases from each sub dataset, i.e., build a new dictionary from a sub dataset
    // Step-4
    // treat all the new bases as input data, try to select bases from these "input data"
    // if the number of the "input data" is too large to compute the similarity matrix,
    // then repeat from Step-1 to Step-3
    //------------------------------------------------------------------------------------------------------------------------//
  
    //--------------------------------------- StrategyForDictionaryCombination ------------------------------------------------------------------//
    // Strategy_0:
    // keep all the new bases and old bases
    // directly combine the input dictionary and the new dictionary: every basis will be used  
    //
    // use Strategy_0 by set: BasisNumber = BasisNumber_Of_InputDictionary + MaxNumberOfNewBases
    // 
    // Strategy_1:
    // keep all the new bases and select some old bases
    // directly retire old bases: using Experience/Age ratio
    // BasisNumber_Of_InputDictionary - (MaxNumberOfTotalBases - NumberOfNewBases) = the number of input bases that are retired
    //
    // use Strategy_1 by set DirectlyRetireOldAndInexperiencedBasis = true
    // 
    // Strategy_2:
    // keep all the new bases and select some old bases
    // indirectly retire old bases: 
    // mix input bases and data vectors of InClass to get combined dataset, select bases from the combined dataset
    // Selection is done by using KNNBasisSelectionOnlineDictionaryBuilder
    //
    // use Strategy_2 by set DirectlyRetireOldAndInexperiencedBasis = false
    //-----------------------------------------------------------------------------------------------------------------------------------------//

    //
    ElementType SimilarityThreshold_For_Classification;
    // Similarity(i,j) between data vector i and basis vector j in the input dictionary (m_InputDictionary)
    // if Similarity(i,j) >= SimilarityThreshold_For_Classification for some j
    // then the data vector can be represented by the basis, i.e., belong to the class/distribution represented by the input Dictionary
    // 
    // if Similarity(i,j) is very small, i.e., data vector is far away from every basis 
    // then EM-style reconstruction based Dictionary learning methods may not move basis to desired position
    // and therefore, new bases are needed:
    // select new bases from the data vectors that can not be represented by any basis in the input dictionary (m_InputDictionary)

    bool DirectlyRetireOldAndInexperiencedBasis;

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

    // to directly retire some bases in the input dictionary
    // compute the Experience to Age ratio, BasisExperience / BasisAge
    // sort the ratio list in ascend order, and start to retire 

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

        Flag_Update_BasisID = false;

        Flag_Update_BasisAge = false;

        Flag_Update_Variance = false;

        Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix = false;

        Flag_Update_SimilarityMatrix = true;

        Flag_Update_BasisRedundancy = true;

        SimilarityThreshold_For_ComputeBasisRedundancy = 0;

        ConstraintOnKNNReconstructionCode.CodeNonnegative = false;
        ConstraintOnKNNReconstructionCode.CodeSumToOne = false;

        DebugInfo.Clear();
        
        //------------------------------------------------------------//

        SimilarityThreshold_For_Classification = 0;

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

        SubParameter.Flag_Update_BasisID = Flag_Update_BasisID;

        SubParameter.Flag_Update_BasisAge = Flag_Update_BasisAge;

        SubParameter.Flag_Update_Variance = Flag_Update_Variance;

        SubParameter.Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix = Flag_Update_VarianceOfReconstruction_Using_KNNBasisMatrix;

        SubParameter.Flag_Update_SimilarityMatrix = Flag_Update_SimilarityMatrix;

        SubParameter.Flag_Update_BasisRedundancy = Flag_Update_BasisRedundancy;

        SubParameter.SimilarityThreshold_For_ComputeBasisRedundancy = SimilarityThreshold_For_ComputeBasisRedundancy;

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

    void ClearPipelineOutput();

    void UpdatePipelineOutput();

    void GenerateDictionary();

    DenseMatrix<int_max> ComputeDataNumberInEachBatch(int_max TotalDataNumber);

    void UpdateDictionary(FeatureDictionaryForSparseCoding<ElementType>& Dictionary, 
                          const DenseMatrix<ElementType>& FeatureData);

    FeatureDictionaryForSparseCoding<ElementType> CopyDictionaryAndDiscountBasisExperience(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary);


    void ClassifyFeatureDataBySimilarityThreshold(DenseMatrix<int_max>& DataIndexList_InClass, 
                                                  DenseMatrix<int_max>& DataIndexList_OutClass, 
                                                  const DataContainer<SparseVector<ElementType>>& CodeTable);

    FeatureDictionaryForSparseCoding<ElementType> CombineInputDictionaryAndNewDictionary(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary,
                                                                                         const FeatureDictionaryForSparseCoding<ElementType>& NewDictionary);

    void AdjustBasisExperience(DenseMatrix<ElementType>& BasisExperience, int_max DataNumber, ElementType TotalExperience_init);

    void UpdateDictionary_OtherInformation(FeatureDictionaryForSparseCoding<ElementType>& CombinedDictionary, int_max DataNumber);

    DenseMatrix<ElementType> ComputeVectorSimilarityMatrix(const DenseMatrix<ElementType>& BasisMatrix_input,
                                                           const DenseMatrix<ElementType>& BasisMatrix_new);

    DenseMatrix<ElementType> ComputeVectorProbabilityList(const DenseMatrix<ElementType>& BasisExperience_input,
                                                          const DenseMatrix<ElementType>& BasisExperience_new);

    DenseMatrix<int_max> SelectBasis(int_max BasisNumber_desired,
                                     const DenseMatrix<ElementType>& VectorSimilarityMatrix,
                                     const DenseMatrix<ElementType>& VectorProbabilityList);

    
    void UpdateBasisRedundancy(DenseMatrix<ElementType>& BasisRedundancy, const DenseMatrix<ElementType>& SimilarityMatrix);

private:
    KNNBasisSelectionBasedInitialDictionaryBuilder(const KNNBasisSelectionBasedInitialDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionBasedInitialDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionBasedInitialDictionaryBuilder.hpp"

#endif