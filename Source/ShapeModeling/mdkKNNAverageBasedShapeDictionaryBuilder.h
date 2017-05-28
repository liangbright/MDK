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

	int_max MaxNeighborCount;

	ScalarType SimilarityThreshold;
	// shape_a and shape_b not similar if Similarity(a,b) <= SimilarityThreshold

	std::string TransformName;// IdentityTransform, RigidTransform, SimilarityTransform, ThinPlateSplineTransform

	ScalarType ExperienceDiscountFactor;
	// weight for the past experience when new training data is used
	// When to use the weight : 
	// discount the Experience of dictionary right after training is done on new data
	// implemented in UpdateBasisExperienceForEachBasisShape(...)

	//----------------------- parameter for data processing ------------------------------//
	int_max BatchSize;  // batch size
	int_max MaxEpochCount;  // epoch: train once on all the input shape 
	int_max MaxThreadCount; // CPU thread

	//--------------------- parameter for updating some dictionary information -------------//
	bool Flag_Update_BasisAge;
	bool Flag_Update_BasisSimilarity;
	bool Flag_Update_BasisRedundancy;

	// -------------------------- debug -----------------------------------------------//
	bool Debug_Flag;
	String Debug_FilePath;
//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNAverageBasedShapeDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNAverageBasedShapeDictionaryBuilder() {}

    void Clear()
    {
		Name = "";
		MaxNeighborCount = 0;
		SimilarityThreshold = 0;
		TransformName = "RigidTransform";
		
		ExperienceDiscountFactor = 1;
		
		BatchSize = 1;
		MaxEpochCount = 1;
		MaxThreadCount = 1;

		Flag_Update_BasisAge = true;
		Flag_Update_BasisSimilarity = true;
		Flag_Update_BasisRedundancy = true;

		Debug_Flag = false;
		Debug_FilePath = "";
    }
};

template<typename ScalarType>
struct Input_Of_KNNAverageBasedShapeDictionaryBuilder
{
	Parameter_Of_KNNAverageBasedShapeDictionaryBuilder<ScalarType> Parameter;

	const ObjectArray<DenseMatrix<ScalarType>>* TrainingShapeData;

	// land mark index list of each traing shape
	// to compute transform
	DenseVector<int_max> LandmarkOnShape;

	const ShapeDictionary<ScalarType>* InitialDictionary;
};

template<typename ScalarType>
struct Output_Of_KNNAverageBasedShapeDictionaryBuilder
{
	ShapeDictionary<ScalarType> Dictionary;
};

template<typename Scalar_Type>
class KNNAverageBasedShapeDictionaryBuilder : Object
{
public:
	typedef Scalar_Type ScalarType;
public:
	Input_Of_KNNAverageBasedShapeDictionaryBuilder<ScalarType> Input;
	Output_Of_KNNAverageBasedShapeDictionaryBuilder<ScalarType> Output;
public:
	KNNAverageBasedShapeDictionaryBuilder();
	~KNNAverageBasedShapeDictionaryBuilder();
	void Clear();
	bool CheckInput();
	void Update();
protected:
	void AdjustBasisExperience_BeforeEachEpoch(DenseMatrix<ScalarType>& BasisExperience);

    void UpdateBasisAndBasisExperience(ObjectArray<DenseMatrix<ScalarType>>& Basis,
                                       DenseMatrix<ScalarType>& BasisExperience,
                                       const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,
                                       const ObjectArray<SparseVector<ScalarType>>& CodeTable);

	DenseMatrix<ScalarType> TransformShape(const DenseMatrix<ScalarType>& Shape, const DenseMatrix<ScalarType>& Basis);

    void AdjustBasisExperience_AfterEachEpoch(DenseMatrix<ScalarType>& BasisExperience, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount);

	void UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary, int_max TotalDataCount);

    void UpdateBasisSimilarity(DenseMatrix<ScalarType>& BasisSimilarity, const ObjectArray<DenseMatrix<ScalarType>>& Basis);

    void UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& BasisSimilarity);
};


}// namespace mdk


#include "mdkKNNAverageBasedShapeDictionaryBuilder.hpp"
