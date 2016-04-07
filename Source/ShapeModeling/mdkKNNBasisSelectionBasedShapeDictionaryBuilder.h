#pragma once

#include <random>

#include "mdkShapeDictionary.h"
#include "mdkKNNSoftAssignBasedSparseShapeEncoder.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder
{
	std::string Name;

	int_max BasisCount;// desired basis count

	int_max MaxNeighbourCount;

	ScalarType SimilarityThreshold;
	// shape_a and shape_b not similar if Similarity(a,b) <= SimilarityThreshold

	std::string TransformName;// IdentityTransform, RigidTransform, SimilarityTransform, ThinPlateSplineTransform

	ScalarType ExperienceDiscountFactor;
	// weight for the past experience when new training data is used
	// When to use the weight : 
	// discount the Experience of dictionary right after training is done on new data
	// implemented in UpdateBasisExperienceForEachBasisShape(...)

	//----------------------- parameter for data processing ------------------------------//
	int_max MiniBatchSize;  // mini-batch size
	int_max MaxEpochCount;  // epoch: train once on all the input shape 
	int_max MaxThreadCount; // CPU thread

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
		
		MiniBatchSize = 1;
		MaxEpochCount = 1;
		MaxThreadCount = 1;

		Debug_Flag = false;
		Debug_FilePath = "";
    }
};


template<typename Scalar_Type>
class KNNBasisSelectionBasedShapeDictionaryBuilder : Object
{
public:
	typedef Scalar_Type ScalarType;

private:
	Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType> m_Parameter;

	// training data
	const ObjectArray<DenseMatrix<ScalarType>>* m_TrainingShapeData;

	// land mark index list of each traing shape
	// to compute transform
	DenseVector<int_max> m_LandmarkOnShape;

	// input initial dictionary
	const ShapeDictionary<ScalarType>* m_InitialDictionary;

	// output selected basis from m_InitialDictionary
	DenseVector<int_max> m_SelectedBasisIndexList;
	DenseVector<ScalarType> m_SelectedBasisExperience;

public:
	KNNBasisSelectionBasedShapeDictionaryBuilder();
	~KNNBasisSelectionBasedShapeDictionaryBuilder();
	void Clear();
	Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType>& Parameter() { return m_Parameter; }
	void SetTrainingShapeData(const ObjectArray<DenseMatrix<ScalarType>>* ShapeData) { m_TrainingShapeData = ShapeData; }
	void SetLandmarkOnShape(const DenseVector<int_max>& Landmark) { m_LandmarkOnShape = Landmark; }
	void SetInitialDictionary(const ShapeDictionary<ScalarType>* InitialDictionary) { m_InitialDictionary = InitialDictionary; }
	bool CheckInput();
	void Update();
	DenseVector<int_max>& OutputSelectedBasisIndexList() { return m_SelectedBasisIndexList; }
	DenseVector<int_max>& OutputSelectedBasisExperience() {	return m_SelectedBasisExperience; }

protected:
	void AdjustBasisExperience_BeforeEachEpoch(DenseMatrix<ScalarType>& BasisExperience);
    void UpdateBasisExperience(DenseMatrix<ScalarType>& BasisExperience, const ObjectArray<SparseVector<ScalarType>>& CodeTable);
    void AdjustBasisExperience_AfterEachEpoch(DenseMatrix<ScalarType>& BasisExperience, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount);
};


}// namespace mdk


#include "mdkKNNBasisSelectionBasedShapeDictionaryBuilder.hpp"
