#pragma once

#include "mdkShapeDictionary.h"
#include "mdkShapeSimilarityMeasurementFunction.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNSoftAssignBasedSparseShapeEncoder
{
	String TransformName; // RigidTransform, SimilarityTransform, ThinPlateSplineTransform
    int_max MaxNeighborCount;
	ScalarType SimilarityThreshold; // set Similarity to 0 if it is < SimilarityThreshold
	int_max MaxThreadCount;
};

template<typename ScalarType>
struct Input_Of_KNNSoftAssignBasedSparseShapeEncoder
{
	Parameter_Of_KNNSoftAssignBasedSparseShapeEncoder<ScalarType> Parameter;
	const ObjectArray<DenseMatrix<ScalarType>>* ShapeData;
	DenseVector<int_max> LandmarkOnShape;
	const ObjectArray<DenseMatrix<ScalarType>>* Dictionary;
};

template<typename ScalarType>
struct Output_Of_KNNSoftAssignBasedSparseShapeEncoder
{
	ObjectArray<SparseVector<ScalarType>> MembershipCode;
	ObjectArray<SparseVector<ScalarType>> SimilarityCode;
};

template<typename Scalar_Type>
class KNNSoftAssignBasedSparseShapeEncoder : public Object
{
public:
	typedef Scalar_Type ScalarType;
public:
	Input_Of_KNNSoftAssignBasedSparseShapeEncoder<ScalarType> Input;
	Output_Of_KNNSoftAssignBasedSparseShapeEncoder<ScalarType> Output;	
public:
    KNNSoftAssignBasedSparseShapeEncoder();
    ~KNNSoftAssignBasedSparseShapeEncoder();
	void Clear();		
	bool CheckInput();
	void Update();	
private:
	void EncodeShape(int_max ShapeIndex, SparseVector<ScalarType>& MemebershipCode, SparseVector<ScalarType>& SimilarityCode);
	ScalarType ComputeShapeSimilarity(const DenseMatrix<ScalarType>& Basis, const DenseMatrix<ScalarType>& Data);
private:
    KNNSoftAssignBasedSparseShapeEncoder(const KNNSoftAssignBasedSparseShapeEncoder&) = delete;
    void operator=(const KNNSoftAssignBasedSparseShapeEncoder&) = delete;
};

}//namespace mdk

#include "mdkKNNSoftAssignBasedSparseShapeEncoder.hpp"
