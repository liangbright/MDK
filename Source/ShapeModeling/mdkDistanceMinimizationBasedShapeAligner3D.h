#pragma once

#include "mdkDenseMatrix.h"
#include "mdkSparseVector.h"

// the objective function E=sum{Aij*||Ti(Xi) - Tj(Xj)||^2, i,j=0, 1, 2, ...}
// input: shape X0, X1, X2, ... and similarity Aij between each shape pair 
//        they have point-to-point corresponedece 
// output:
// Ti is the similarity/rigid transform of shape Xi
// Tj is the similarity/rigid transform of shape Xj
// Ti(Xi) is the transformed version of Xi
// Tj(Xj) is the transformed version of Xj
//
// Algorithm Part-I:  find initial transform {Ti, i=0,1,2,...}
// Algorithm Part-II: update Ti(Xi) ~ sum{Aij*Tj(Xj), Ai0+...+Aij+...=1} iteratively for each i
//                    only use K-nearest neighbour 
//-----------------------------------------------------------------------------------------------------

namespace mdk
{ 
template<typename Scalar_Type>
class DistanceMinimizationBasedShapeAligner3D : Object
{
public:
	typedef Scalar_Type ScalarType;

private:
	//--------------------- input-------------------------------------------//
	const ObjectArray<DenseMatrix<ScalarType>>* m_InputShapeList;

	const ObjectArray<SparseVector<ScalarType>>* m_InputSimilarityTable;
	//m_SimilarityTable[k] is the similarity list of Shape_k
	//m_SimilarityTable[k][n] is the similarity between Shape_k and Shape_n

	bool m_Flag_use_SimilarityTransform;
	// true : use SimilarityTransform
	// false: use RigidTransform

	int_max m_NeighbourCount;
	//Aij is 0 if Xi and Xj are not neighbour to each other
	// Xi can only have a limited number of neighbour = m_NeighbourCount

	//------------------------------- output ---------------------------------//
	ObjectArray<std::pair<DenseMatrix<ScalarType>, DenseMatrix<ScalarType>> m_OutputTransformList;
	// {[s1*R1, t1] of Shape1, [S2*R2, t2] of Shape1, ...]
	// s is scale, R is rotation, t is translation

	ObjectArray<DenseMatrix<ScalarType>> m_OutputShapeList;// each aligned shape

public:
	DistanceMinimizationBasedShapeAligner3D();
	~DistanceMinimizationBasedShapeAligner3D();
	void Clear();	
	void SelectSimilarityTransform() { m_Flag_use_SimilarityTransform = true; }
	void SelectRigidTransform() { m_Flag_use_SimilarityTransform = false; }
	void SetInputShapeList(const ObjectArray<DenseMatrix<ScalarType>>* InputShapeList);
	void SetInputSimilarityTable(const ObjectArray<SparseVector<ScalarType>>* SimilarityTable);	
	void SetNeighbourCount(int_max Count) { m_NeighbourCount = Count; }
	bool CheckInput();
	void Update();
	ObjectArray<std::pair<DenseMatrix<ScalarType>, DenseMatrix<ScalarType>>& OutputTransformList() {return m_OutputTransformList;}
	ObjectArray<DenseMatrix<ScalarType>>& OutputShapeList() { return m_OutputShapeList; }

	//---------------- provide a default method to get similarity --------------------------------------------------------------------
	static ObjectArray<SparseVector<ScalarType>> ComputeSimilarityBetweenShape(const ObjectArray<DenseMatrix<ScalarType>>& ShapeList);

private:
	DistanceMinimizationBasedShapeAligner3D(const DistanceMinimizationBasedShapeAligner3D&) = delete;
	void operator=(const DistanceMinimizationBasedShapeAligner3D&) = delete;
};

}//namespace mdk

#include "mdkDistanceMinimizationBasedShapeAligner3D.hpp"
