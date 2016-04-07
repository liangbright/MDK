#pragma once

#include <random>

#include "mdkShapeDictionary.h"
#include "mdkKNNSoftAssignBasedSparseShapeEncoder.h"


namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_BasisSelectionBasedShapeDictionaryBuilder
{
    std::string Name;

    int_max BasisCount;

	std::string TransformName;// IdentityTransform, RigidTransform, SimilarityTransform, ThinPlateSplineTransform

    //----------------------- parameter for data processing ------------------------------//
	int_max MiniBatchSize;  // mini-batch size
	int_max MaxEpochCount;  // epoch: train once on all the input shape 
	int_max MaxThreadCount; // CPU thread

    //--------------------- parameter for updating some dictionary information -------------//
    bool Flag_Update_BasisID;
    bool Flag_Update_BasisSimilarity;

	// -------------------------- debug -----------------------------------------------//
	bool Debug_Flag;
	String Debug_FilePath;
//--------------------------------------------------------------------------------------------------------

    Parameter_Of_BasisSelectionBasedShapeDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_BasisSelectionBasedShapeDictionaryBuilder() {}

    void Clear()
    {
		Name = "";
        BasisCount = 0;
		TransformName = "RigidTransform";

		MiniBatchSize = 100;
		MaxEpochCount = 1;
		MaxThreadCount = 1;

        Flag_Update_BasisID  = true; // new basis ID = 0
		Flag_Update_BasisSimilarity = true;
        
		Debug_Flag = false;
		Debug_FilePath = "";
    }
};


template<typename Scalar_Type>
class BasisSelectionBasedShapeDictionaryBuilder : public Object
{
public:
	typedef Scalar_Type ScalarType;

private:
    Parameter_Of_BasisSelectionBasedShapeDictionaryBuilder<ScalarType> m_Parameter;

    // training data
    const ObjectArray<DenseMatrix<ScalarType>>* m_TrainingShapeData;

	DenseVector<int_max> m_LandmarkOnShape; // to get transform

	// input initial dictionary
    const ShapeDictionary<ScalarType>* m_InitialDictionary;

	// output dictionary
	ShapeDictionary<ScalarType> m_Dictionary;

public:
    BasisSelectionBasedShapeDictionaryBuilder();
    ~BasisSelectionBasedShapeDictionaryBuilder();
    void Clear();
	Parameter_Of_BasisSelectionBasedShapeDictionaryBuilder<ScalarType>& Parameter() { return m_Parameter; }
	void SetTrainingShapeData(const ObjectArray<DenseMatrix<ScalarType>>* ShapeData) { m_TrainingShapeData = ShapeData; }
	void SetLandmarkOnShape(const DenseVector<int_max>& Landmark) { m_LandmarkOnShape = Landmark;	}
	void SetInitialDictionary(const ShapeDictionary<ScalarType>* InitialDictionary) { m_InitialDictionary = InitialDictionary; }
    bool CheckInput();
	void Update();
	ShapeDictionary<ScalarType>& OutputDictionary() { return m_Dictionary; }

private:    
    ShapeDictionary<ScalarType> BuildDictionaryInMiniBatch(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData);

	DenseVector<int_max> SelectBasis(const int_max BasisCount_desired, const DenseMatrix<ScalarType>& SimilarityMatrix);

	DenseVector<int_max> SelectBasis_By_SimilarityThreshold(const DenseVector<int_max>& ShapeIndexList_sort, const DenseMatrix<ScalarType>& SimilarityMatrix, ScalarType SimilarityThreshold);

    DenseMatrix<ScalarType> ComputeShapeSimilarityMatrix(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData);
   
	ScalarType ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

	void UpdateDictionaryInformation_AtEachMiniBatch(ShapeDictionary<ScalarType>& Dictionary,
													 const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,													 
													 const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
													 const DenseMatrix<int_max>& ShapeIndexList_Basis,
													 const ShapeDictionary<ScalarType>& Dictionary_init);

	void UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary, int_max TotalDataCount);

private:
    BasisSelectionBasedShapeDictionaryBuilder(const BasisSelectionBasedShapeDictionaryBuilder&) = delete;
    void operator=(const BasisSelectionBasedShapeDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkBasisSelectionBasedShapeDictionaryBuilder.hpp"
