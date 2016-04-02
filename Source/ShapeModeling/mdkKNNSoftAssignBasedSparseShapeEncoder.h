#pragma once

#include "mdkString.h"
#include "mdkShapeDictioanry.h"
#include "mdkShapeSimilarityMeasurementFunction.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNSoftAssignBasedSparseShapeEncoder
{
    int_max NeighbourCount;
	std::string TransformName; // SimilarityTransform or RigidTransform	
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
	const ObjectArray<DenseMatirx<ScalarType>>* m_ShapeData;
	const ObjectArray<DenseMatirx<ScalarType>>* m_Dictioanry;
	ObjectArray<SparseVector<ScalarType>> m_Code;
public:
    KNNSoftAssignBasedSparseShapeEncoder();
    ~KNNSoftAssignBasedSparseShapeEncoder();
	void Clear();
	Parameter_Of_KNNSoftAssignBasedSparseShapeEncoder<ScalarType>& Parameter() {return m_Parameter;}
	void SetInputShapeData(const ObjectArray<DenseMatirx<ScalarType>>* ShapeData) { m_ShapeData = ShapeData; }
	void SetInputDictioanry(const ObjectArray<DenseMatirx<ScalarType>>* Dictioanry) {m_Dictioanry = Dictioanry;}
	bool CheckInput();
	void Update();
	ObjectArray<SparseVector<ScalarType>> OuputCode() { return m_Code; }   
	//----------------------------------------------------------------------------------------------------
private:
	SparseVector<ScalarType> EncodeShape(int_max ShapeIndex);
private:
    KNNSoftAssignBasedSparseShapeEncoder(const KNNSoftAssignBasedSparseShapeEncoder&) = delete;
    void operator=(const KNNSoftAssignBasedSparseShapeEncoder&) = delete;
};

}

#include "mdkKNNSoftAssignBasedSparseShapeEncoder.hpp"
