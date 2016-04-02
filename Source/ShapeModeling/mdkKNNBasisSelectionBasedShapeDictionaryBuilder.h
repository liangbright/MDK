#pragma once

#include <random>

#include "mdkShapeDictioanry.h"
#include "mdkShapeSimilarityMeasurementFunction.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder
{
    std::string Name;

    int_max BasisCount;
         
	int_max MaxNeighbourCount;

	ScalarType SimilarityThreshold;
	// shape_a and shape_b not similar if Similarity(a,b) <= SimilarityThreshold

	std::string TransformName;
	//SimilarityTransform or RigidTransform

    ScalarType ExperienceDiscountFactor; 
    // weight for the past experience when new training data is used
    // When to use the weight : 
    // discount the Experience of dictionary right after training is done on new data
    // implemented in UpdateBasisExperienceForEachBasisShape(...)

    ScalarType WeightOnProbabiliyForBasisSelection;
    // this weight is used to sort shape pair    
    // range [0, 1]

    //----------------------- parameter for data processing ------------------------------//

    int_max MaxNumberOfDataInEachBatch; // mini-batch size
    int_max MaxNumberOfEpoch;           // epoch: train once on all the input shape 
    int_max MaxNumberOfThread;          // CPU thread

    //--------------------- parameter for updating some dictionary information -------------//

    bool Flag_Update_BasisID;
    bool Flag_Update_BasisAge;
    bool Flag_Update_BasisSimilarity;
    bool Flag_Update_BasisRedundancy;

	// -------------------------- debug -----------------------------------------------//
	bool Debug_Flag_OutputInfo;
	std::string Debug_FilePath;
//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder() {}

    void Clear()
    {
        BasisCount = 0;
		MaxNeighbourCount = 0;
		SimilarityThreshold = 0;
		TransformName = "RigidTransform";
        ExperienceDiscountFactor = 1;
        WeightOnProbabiliyForBasisSelection = 0;
        MaxNumberOfDataInEachBatch = 0;
		MaxNumberOfEpoch = 1;
        MaxNumberOfThread = 1;

        Flag_Update_BasisID  = true; // new basis ID = 0
        Flag_Update_BasisAge = true; // new basis age = 0
		Flag_Update_BasisSimilarity = true;
        Flag_Update_BasisRedundancy = true;
        
		Debug_Flag_OutputInfo = false;
		Debug_FilePath = "";
    }
};


template<typename Scalar_Type>
class KNNBasisSelectionBasedShapeDictionaryBuilder : public Object
{
public:
	typedef Scalar_Type ScalarType;

private:
    Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType> m_Parameter;

    // training data
    const ObjectArray<DenseMatrix<ScalarType>>* m_TrainingShapeData;

	// input initial dictionary
    const ShapeDictionary<ScalarType>* m_InitialDictionary;

	// output dictionary
	ShapeDictionary<ScalarType> m_Dictionary;

public:
    KNNBasisSelectionBasedShapeDictionaryBuilder();
    ~KNNBasisSelectionBasedShapeDictionaryBuilder();
    void Clear();
	Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>& Parameter() { return m_Parameter; }
	void SetTrainingShapeData(const ObjectArray<DenseMatrix<ScalarType>>* ShapeData) { m_TrainingShapeData = ShapeData; }
	void SetInitialDictionary(const ShapeDictionary<ScalarType>* InitialDictionary) { m_InitialDictionary = InitialDictionary; }
    bool CheckInput();
	void Update();
	ShapeDictionary<ScalarType>& OutputDictionary() { return m_Dictionary; }

private:

    ShapeDictionary<ScalarType> PreprocessInitialDictionary(const ShapeDictionary<ScalarType>& InitialDictionary);
    
    DenseMatrix<int_max> ComputeDataCountInEachBatch(int_max TotalShapeCount);

    ShapeDictionary<ScalarType> BuildDictionaryFromDataBatch(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData);

    DenseMatrix<int_max> SelectBasis(const int_max BasisCount_desired, const DenseMatrix<ScalarType>& SimilarityMatrix, const DenseMatrix<ScalarType>& ProbabilityOfEachShape);

    DenseMatrix<ScalarType> ComputeInitialRepresentativeAbilityOfEachVector(const ShapeDictionary<ScalarType>& Dictionary_init, int_max TotalShapeCount);

    DenseMatrix<ScalarType> ComputeShapeSimilarityMatrix(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData);

    ObjectArray<DenseMatrix<int_max>> FindKNNShapeIndexTableByShapeSimilarityMatrix(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix);
   
    DenseMatrix<ScalarType> EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachShape(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
                                                                                          const ObjectArray<DenseMatrix<int_max>>& KNNShapeIndexTable,
                                                                                          const DenseMatrix<ScalarType>& RepresentativeAbilityOfEachVector);

    ObjectArray<SparseVector<ScalarType>> EncodeShapeDataBySimilarity(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
                                                                      const DenseMatrix<int_max>&    ShapeIndexList_Basis,
                                                                      int_max BasisCount_init);

	ScalarType ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>B);

	void UpdateDictionaryInformation_AtEachDataBatch(ShapeDictionary<ScalarType>& Dictionary,
													 const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,
													 const ObjectArray<SparseVector<ScalarType>>& CodeTable,
													 const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
													 const DenseMatrix<int_max>& ShapeIndexList_Basis,
													 const ShapeDictionary<ScalarType>& Dictionary_init);

	void UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary, int_max ShapeDataCount);

    void UpdateBasisExperience_AtEachDataBatch(DenseMatrix<ScalarType>& BasisExperience, const ObjectArray<SparseVector<ScalarType>>& CodeTable);

	void AdjustBasisExperience_AtEachEpoch(DenseMatrix<ScalarType>& BasisExperience, int_max DataCount, ScalarType TotalExperience_init);

    void UpdateBasisRedundancy_AfterALLEpoch(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& BasisSimilarity);

private:
    KNNBasisSelectionBasedShapeDictionaryBuilder(const KNNBasisSelectionBasedShapeDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionBasedShapeDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionBasedShapeDictionaryBuilder.hpp"
