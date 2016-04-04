#pragma once

#include <random>

#include "mdkShapeDictionary.h"
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

	std::string TransformName;// IdentityTransform, RigidTransform, SimilarityTransform, ThinPlateSplineTransform

    ScalarType ExperienceDiscountFactor; 
    // weight for the past experience when new training data is used
    // When to use the weight : 
    // discount the Experience of dictionary right after training is done on new data
    // implemented in UpdateBasisExperienceForEachBasisShape(...)

    ScalarType WeightOnProbabiliyForBasisSelection;
    // this weight is used to sort shape pair    
    // range [0, 1]

    //----------------------- parameter for data processing ------------------------------//
	int_max MiniBatchSize;  // mini-batch size
	int_max MaxEpochCount;  // epoch: train once on all the input shape 
	int_max MaxThreadCount; // CPU thread

    //--------------------- parameter for updating some dictionary information -------------//
    bool Flag_Update_BasisID;
    bool Flag_Update_BasisAge;
    bool Flag_Update_BasisSimilarity;
    bool Flag_Update_BasisRedundancy;

	// -------------------------- debug -----------------------------------------------//
	bool Debug_Flag;
	String Debug_FilePath;
//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder() {}

    void Clear()
    {
		Name = "";
        BasisCount = 0;
		MaxNeighbourCount = 0;
		SimilarityThreshold = 0;
		TransformName = "RigidTransform";
		
        ExperienceDiscountFactor = 1;
        WeightOnProbabiliyForBasisSelection = 0;

		MiniBatchSize = 1;
		MaxEpochCount = 1;
		MaxThreadCount = 1;

        Flag_Update_BasisID  = true; // new basis ID = 0
        Flag_Update_BasisAge = true; // new basis age = 0
		Flag_Update_BasisSimilarity = true;
        Flag_Update_BasisRedundancy = true;
        
		Debug_Flag = false;
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

	DenseVector<int_max> m_LandmarkOnShape; // to get transform

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
	void SetLandmarkOnShape(const DenseVector<int_max>& Landmark) { m_LandmarkOnShape = Landmark;	}
	void SetInitialDictionary(const ShapeDictionary<ScalarType>* InitialDictionary) { m_InitialDictionary = InitialDictionary; }
    bool CheckInput();
	void Update();
	ShapeDictionary<ScalarType>& OutputDictionary() { return m_Dictionary; }

private:

    void AdjustBasisExperience_BeforeEachEpoch(DenseMatrix<ScalarType>& BasisExperience);
    
    ShapeDictionary<ScalarType> BuildDictionaryInMiniBatch(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData);

    DenseMatrix<int_max> SelectBasis(const int_max BasisCount_desired, const DenseMatrix<ScalarType>& SimilarityMatrix, const DenseMatrix<ScalarType>& ProbabilityOfEachShape);

    DenseMatrix<ScalarType> ComputeInitialRepresentativeAbilityOfEachShape(const ShapeDictionary<ScalarType>& Dictionary_init, int_max TotalShapeCount);

    DenseMatrix<ScalarType> ComputeShapeSimilarityMatrix(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData);

    ObjectArray<DenseMatrix<int_max>> FindKNNShapeIndexTableByShapeSimilarityMatrix(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix);
   
    DenseMatrix<ScalarType> EstimateSmoothedAndNormalizedRepresentativeAbilityOfEachShape(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
                                                                                          const ObjectArray<DenseMatrix<int_max>>& KNNShapeIndexTable,
                                                                                          const DenseMatrix<ScalarType>& RepresentativeAbilityOfEachShape);

    ObjectArray<SparseVector<ScalarType>> EncodeShapeDataBySimilarity(const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
                                                                      const DenseMatrix<int_max>&    ShapeIndexList_Basis,
                                                                      int_max BasisCount_init);

	ScalarType ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

	void UpdateDictionaryInformation_AtEachMiniBatch(ShapeDictionary<ScalarType>& Dictionary,
													 const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,
													 const ObjectArray<SparseVector<ScalarType>>& CodeTable,
													 const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
													 const DenseMatrix<int_max>& ShapeIndexList_Basis,
													 const ShapeDictionary<ScalarType>& Dictionary_init);

	void UpdateBasisExperience_AtEachMiniBatch(DenseMatrix<ScalarType>& BasisExperience, const ObjectArray<SparseVector<ScalarType>>& CodeTable);

	void AdjustBasisExperience_AfterEachEpoch(DenseMatrix<ScalarType>& BasisExperience, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount);

	void UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary, int_max TotalDataCount);

    void UpdateBasisRedundancy_AfterALLEpoch(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& BasisSimilarity);

private:
    KNNBasisSelectionBasedShapeDictionaryBuilder(const KNNBasisSelectionBasedShapeDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionBasedShapeDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionBasedShapeDictionaryBuilder.hpp"
