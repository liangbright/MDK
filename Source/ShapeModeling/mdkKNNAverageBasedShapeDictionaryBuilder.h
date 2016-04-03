#pragma once

#include <random>

#include "mdkShapeDictionary.h"
#include "mdkKNNSoftAssignBasedSparseShapeEncoder.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNAverageBasedShapeDictionaryBuilder
{
	std::string Name;

	int_max NeighbourCount;

	ScalarType SimilarityThreshold;
	// shape_a and shape_b not similar if Similarity(a,b) <= SimilarityThreshold

	std::string TransformName;// RigidTransform, SimilarityTransform, ThinPlateSplineTransform
	DenseVector<int_max> Landmark; // to get transform

	ScalarType ExperienceDiscountFactor;
	// weight for the past experience when new training data is used
	// When to use the weight : 
	// discount the Experience of dictionary right after training is done on new data
	// implemented in UpdateBasisExperienceForEachBasisShape(...)

	//----------------------- parameter for data processing ------------------------------//
	int_max MiniBatchSize;  // mini-batch size
	int_max MaxEpochCount;  // epoch: train once on all the input shape 
	int_max MaxThreadCount; // CPU thread

	//--------------------- parameter for updating some dictionary information -------------//
	bool Flag_Update_BasisAge;
	bool Flag_Update_BasisSimilarity;
	bool Flag_Update_BasisRedundancy;

	// -------------------------- debug -----------------------------------------------//
	bool Debug_Flag_OutputInfo;
	std::string Debug_FilePath;
//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNAverageBasedShapeDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNAverageBasedShapeDictionaryBuilder() {}

    void Clear()
    {
		Name = "";
		NeighbourCount = 0;
		SimilarityThreshold = 0;
		TransformName = "RigidTransform";
		Landmark.Clear();
		ExperienceDiscountFactor = 1;
		
		MiniBatchSize = 1;
		MaxEpochCount = 1;
		MaxThreadCount = 1;

		Flag_Update_BasisID = true; 
		Flag_Update_BasisAge = true;
		Flag_Update_BasisSimilarity = true;
		Flag_Update_BasisRedundancy = true;

		Debug_Flag_OutputInfo = false;
		Debug_FilePath = "";
    }
};


template<typename Scalar_Type>
class KNNAverageBasedShapeDictionaryBuilder : Object
{
public:
	typedef Scalar_Type ScalarType;

private:
	Parameter_Of_KNNAverageBasedShapeDictionaryBuilder<ScalarType> m_Parameter;

	// training data
	const ObjectArray<DenseMatrix<ScalarType>>* m_TrainingShapeData;

	// land mark index list of each traing shape
	// to compute transform
	DenseVector<int_max> m_LandmarkIndexList;

	// input initial dictionary
	const ShapeDictionary<ScalarType>* m_InitialDictionary;

	// output dictionary
	ShapeDictionary<ScalarType> m_Dictionary;

public:
	KNNAverageBasedShapeDictionaryBuilder();
	~KNNAverageBasedShapeDictionaryBuilder();
	void Clear();
	Parameter_Of_KNNAverageBasedShapeDictionaryBuilder<ScalarType>& Parameter() { return m_Parameter; }
	void SetTrainingShapeData(const ObjectArray<DenseMatrix<ScalarType>>* ShapeData) { m_TrainingShapeData = ShapeData; }
	void SetLandmark(const DenseVector<int_max>& LandmarkIndexList) { m_LandmarkIndexList = LandmarkIndexList; }
	void SetInitialDictionary(const ShapeDictionary<ScalarType>* InitialDictionary) { m_InitialDictionary = InitialDictionary; }
	bool CheckInput();
	void Update();
	ShapeDictionary<ScalarType>& OutputDictionary() { return m_Dictionary; }

protected:
	ShapeDictioanry<ScalarType> PreprocessInitialDictionary(const ShapeDictionary<ScalarType>& InitialDictionary);

	DenseMatrix<int_max> ComputeDataCountInEachBatch(int_max TotalShapeCount);

    void UpdateBasisAndBasisExperience(ObjectArray<DenseMatrix<ScalarType>>& Basis,
                                       DenseMatrix<ScalarType>& BasisExperience,
                                       const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,
                                       const ObjectArray<SparseVector<ScalarType>>& CodeTable);

	DenseMatrix<ScalarType> TransformShape(const <DenseMatrix<ScalarType>& Shape, const DenseMatrix<ScalarType>& Basis);

    void UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary,
                                                   const DenseMatrix<ScalarType>& BasisExperience_init,
                                                   int_max TotalDataCount);

    void AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount);

    void UpdateBasisSimilarity(DenseMatrix<ScalarType>& BasisSimilarity, const ObjectArray<DenseMatrix<ScalarType>>& Basis);

    void UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& BasisSimilarity);


};


}// namespace mdk


#include "mdkKNNAverageBasedShapeDictionaryBuilder.hpp"
