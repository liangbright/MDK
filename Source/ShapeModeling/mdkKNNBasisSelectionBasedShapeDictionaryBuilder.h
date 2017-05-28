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

	int_max MaxNeighborCount;

	std::string TransformName;// IdentityTransform, RigidTransform, SimilarityTransform, ThinPlateSplineTransform

	ScalarType ExperienceDiscountFactor;

    //----------------------- parameter for data processing ------------------------------//
	int_max BatchSize;  // mini-batch size
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
		MaxNeighborCount = 10;
		TransformName = "RigidTransform";
		ExperienceDiscountFactor = 1;

		BatchSize = 100;
		MaxEpochCount = 1;
		MaxThreadCount = 1;

        Flag_Update_BasisID  = true;
		Flag_Update_BasisSimilarity = true;
        
		Debug_Flag = false;
		Debug_FilePath = "";
    }
};

template<typename ScalarType>
struct Input_Of_KNNBasisSelectionBasedShapeDictionaryBuilder
{
	Parameter_Of_KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType> Parameter;

	const ObjectArray<DenseMatrix<ScalarType>>* TrainingShapeData;

	DenseVector<int_max> LandmarkOnShape; // to get transform

	const ShapeDictionary<ScalarType>* InitialDictionary;
};

template<typename ScalarType>
struct Output_Of_KNNBasisSelectionBasedShapeDictionaryBuilder
{
	ShapeDictionary<ScalarType> Dictionary;
	//{Basis_from_initial_dictionary, basis_from_traning_shape_data}
};

template<typename Scalar_Type>
class KNNBasisSelectionBasedShapeDictionaryBuilder : public Object
{
public:
	typedef Scalar_Type ScalarType;
public:
	Input_Of_KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType> Input;
	Output_Of_KNNBasisSelectionBasedShapeDictionaryBuilder<ScalarType> Output;
public:
    KNNBasisSelectionBasedShapeDictionaryBuilder();
    ~KNNBasisSelectionBasedShapeDictionaryBuilder();
    void Clear();
    bool CheckInput();
	void Update();
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
