#pragma once

#include "mdkShapeDictionary.h"
#include "mdkShapeSimilarityMeasurementFunction.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNSoftAssignBasedSparseShapeEncoder
{
	String TransformName; // RigidTransform, SimilarityTransform, ThinPlateSplineTransform
    int_max MaxNeighbourCount;
	ScalarType SimilarityThreshold; // set Similarity to 0 if it is < SimilarityThreshold
	int_max MaxThreadCount;
};


template<typename Scalar_Type>
class KNNSoftAssignBasedSparseShapeEncoder : public Object
{
public:
	typedef Scalar_Type ScalarType;
private:
    Parameter_Of_KNNSoftAssignBasedSparseShapeEncoder<ScalarType> m_Parameter;
	const ObjectArray<DenseMatrix<ScalarType>>* m_ShapeData;
	DenseVector<int_max> m_LandmarkOnShape;
	const ObjectArray<DenseMatrix<ScalarType>>* m_Dictionary;
	ObjectArray<SparseVector<ScalarType>> m_Code;
public:
    KNNSoftAssignBasedSparseShapeEncoder();
    ~KNNSoftAssignBasedSparseShapeEncoder();
	void Clear();
	Parameter_Of_KNNSoftAssignBasedSparseShapeEncoder<ScalarType>& Parameter() {return m_Parameter;}
	void SetInputShapeData(const ObjectArray<DenseMatrix<ScalarType>>* ShapeData) { m_ShapeData = ShapeData; }
	void SetLandmarkOnShape(const DenseVector<int_max>& Landmark) { m_LandmarkOnShape = Landmark; }
	void SetInputDictionary(const ObjectArray<DenseMatrix<ScalarType>>* Dictioanry) {m_Dictionary = Dictioanry;}
	bool CheckInput();
	void Update();
	ObjectArray<SparseVector<ScalarType>> OutputCode() { return m_Code; }   
private:
	SparseVector<ScalarType> EncodeShape(int_max ShapeIndex);
public:
	//----------------------------- static function --------------------------------------------//
	static ScalarType ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark, const String& TransformName, bool Flag_Symmetry);
private:
    KNNSoftAssignBasedSparseShapeEncoder(const KNNSoftAssignBasedSparseShapeEncoder&) = delete;
    void operator=(const KNNSoftAssignBasedSparseShapeEncoder&) = delete;
};

}//namespace mdk

#include "mdkKNNSoftAssignBasedSparseShapeEncoder.hpp"
