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

	std::string TransformName;// IdentityTransform, RigidTransform, SimilarityTransform, ThinPlateSplineTransform

	ScalarType ExperienceDiscountFactor;

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

    Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder() {}

    void Clear()
    {
		Name = "";
        BasisCount = 0;
		MaxNeighbourCount = 10;
		TransformName = "RigidTransform";
		ExperienceDiscountFactor = 1;

		MiniBatchSize = 100;
		MaxEpochCount = 1;
		MaxThreadCount = 1;

        Flag_Update_BasisID  = true;
		Flag_Update_BasisSimilarity = true;
        
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
	//{Basis_from_initial_dictionary, basis_from_traning_shape_data}

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

	DenseVector<int_max> SelectBasis(const int_max BasisCount_desired, const DenseMatrix<ScalarType>& ShapeSimilarityMatrix, const DenseVector<ScalarType>& ShapeExperience_init);

	DenseVector<int_max> SelectBasis_By_SimilarityThreshold(const DenseVector<int_max>& ShapeIndexList_sort, const DenseMatrix<ScalarType>& ShapeSimilarityMatrix, ScalarType SimilarityThreshold);

    DenseMatrix<ScalarType> ComputeShapeSimilarityMatrix(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData);
   
	ScalarType ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

	void UpdateDictionaryInformation_AtEachMiniBatch(ShapeDictionary<ScalarType>& Dictionary,
													 const ObjectArray<DenseMatrix<ScalarType>>& ShapeData,													 
													 const DenseMatrix<ScalarType>& ShapeSimilarityMatrix,
													 const DenseMatrix<int_max>& ShapeIndexList_Basis,
													 const ShapeDictionary<ScalarType>& Dictionary_init);

	void AdjustBasisExperience_AfterEachEpoch(DenseMatrix<ScalarType>& BasisExperience, ScalarType TotalBasisExperience_init, int_max TotalDataCount);

	void UpdateDictionaryInformation_AfterALLEpoch(ShapeDictionary<ScalarType>& Dictionary, int_max TotalDataCount);

private:
    KNNBasisSelectionBasedShapeDictionaryBuilder(const KNNBasisSelectionBasedShapeDictionaryBuilder&) = delete;
    void operator=(const KNNBasisSelectionBasedShapeDictionaryBuilder&) = delete;
};

}//namespace mdk

#include "mdkKNNBasisSelectionBasedShapeDictionaryBuilder.hpp"
