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

	std::string TransformName;
	//SimilarityTransform or RigidTransform

	ScalarType ExperienceDiscountFactor;
	// weight for the past experience when new training data is used
	// When to use the weight : 
	// discount the Experience of dictionary right after training is done on new data
	// implemented in UpdateBasisExperienceForEachBasisShape(...)

	//----------------------- parameter for data processing ------------------------------//

	int_max MaxDataCountInEachBatch; // mini-batch size
	int_max MaxEpochCount;           // epoch: train once on all the input shape 
	int_max MaxThreadCount;          // CPU thread

	//--------------------- parameter for updating some dictionary information -------------//

	bool Flag_Update_BasisID;
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
		ExperienceDiscountFactor = 1;
		
		MaxDataCountInEachBatch = 0;
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
	void SetInitialDictionary(const ShapeDictionary<ScalarType>* InitialDictionary) { m_InitialDictionary = InitialDictionary; }
	bool CheckInput();
	void Update();
	ShapeDictionary<ScalarType>& OutputDictionary() { return m_Dictionary; }

protected:
    FeatureDictionaryForSparseCoding<ScalarType> PreprocessInitialDictionary(const ShapeDictionary<ScalarType>& InitialDictionary);

	DenseMatrix<int_max> ComputeDataCountInEachBatch(int_max TotalShapeCount);

    void UpdateDictionary(ShapeDictionary<ScalarType>& Dictionary, const DenseMatrix<ScalarType>& ShapeData, const ObjectArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience_UseScaleFactor(DenseMatrix<ScalarType>&       BasisMatrix,
                                                            DenseMatrix<ScalarType>&       BasisExperience,
                                                            const DenseMatrix<ScalarType>& FeatureData,
                                                            const ObjectArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateBasisMatrixAndBasisExperience_NoScaleFactor(DenseMatrix<ScalarType>&       BasisMatrix,
                                                           DenseMatrix<ScalarType>&       BasisExperience,
                                                           const DenseMatrix<ScalarType>& FeatureData,
                                                           const ObjectArray<SparseVector<ScalarType>>& CodeTable);

    void UpdateDictionaryInformation_AfterALLEpoch(FeatureDictionaryForSparseCoding<ScalarType>& Dictionary,
                                                   const DenseMatrix<ScalarType>& BasisExperience_init,
                                                   int_max TotalDataCount);

    void AdjustBasisExperience(DenseMatrix<ScalarType>& BasisExperience, const DenseMatrix<ScalarType>& BasisExperience_init, int_max TotalDataCount);

    void UpdateBasisSimilarity(DenseMatrix<ScalarType>& SimilarityMatrix, const DenseMatrix<ScalarType>& BasisMatrix);

    void UpdateBasisRedundancy(DenseMatrix<ScalarType>& BasisRedundancy, const DenseMatrix<ScalarType>& SimilarityMatrix);


};


}// namespace mdk


#include "mdkKNNAverageBasedShapeDictionaryBuilder.hpp"
