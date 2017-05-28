#pragma once

#include <random>

#include "mdkShapeDictionary.h"
#include "mdkShapeSimilarityMeasurementFunction.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_BasisSelectionBasedShapeDictionaryBuilder
{
    std::string Name;

    int_max BasisCount;

	std::string TransformName;// IdentityTransform, RigidTransform, SimilarityTransform, ThinPlateSplineTransform

    //----------------------- parameter for data processing ------------------------------//
	int_max BatchSize;      // batch size
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
struct Input_Of_BasisSelectionBasedShapeDictionaryBuilder
{
	Parameter_Of_BasisSelectionBasedShapeDictionaryBuilder<ScalarType> Parameter;

	const ObjectArray<DenseMatrix<ScalarType>>* TrainingShapeData;

	DenseVector<int_max> LandmarkOnShape; // to get transform

	const ShapeDictionary<ScalarType>* InitialDictionary;	
};

template<typename ScalarType>
struct Output_Of_BasisSelectionBasedShapeDictionaryBuilder
{
	ShapeDictionary<ScalarType> Dictionary;
	//{Basis_from_initial_dictionary, basis_from_traning_shape_data}
};

template<typename Scalar_Type>
class BasisSelectionBasedShapeDictionaryBuilder : public Object
{
public:
	typedef Scalar_Type ScalarType;
public:
	Input_Of_BasisSelectionBasedShapeDictionaryBuilder<ScalarType> Input;
	Output_Of_BasisSelectionBasedShapeDictionaryBuilder<ScalarType> Output;
public:
    BasisSelectionBasedShapeDictionaryBuilder();
    ~BasisSelectionBasedShapeDictionaryBuilder();
    void Clear();
    bool CheckInput();
	void Update();
private:    
    ShapeDictionary<ScalarType> BuildDictionaryInMiniBatch(const ShapeDictionary<ScalarType>& Dictionary_init, const ObjectArray<DenseMatrix<ScalarType>>& ShapeData);

	DenseVector<int_max> SelectBasis(const int_max BasisCount_desired, const DenseMatrix<ScalarType>& ShapeSimilarityMatrix);

	DenseVector<int_max> SelectBasis_By_SimilarityThreshold(const DenseVector<int_max>& ShapeIndexList_sort, const DenseMatrix<ScalarType>& ShapeSimilarityMatrix, ScalarType SimilarityThreshold);

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
