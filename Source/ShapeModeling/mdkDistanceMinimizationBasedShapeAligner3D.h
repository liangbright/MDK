#pragma once

#include "mdkDenseMatrix.h"
#include "mdkSparseVector.h"
#include "mdkRigidTransform3D.h"
#include "mdkSimilarityTransform3D.h"
#include "mdkShapeSimilarityMeasurementFunction.h"
#include "mdkParallelForLoop.h"

// the objective function E=sum{Aij*||Ti(Xi) - Tj(Xj)||^2, i,j=0, 1, 2, ...}
// input: shape X0, X1, X2, ... and similarity Aij between each shape pair 
//        they have point-to-point corresponedece 
//        No noise point: every point of a shape will be used in the algorithm 
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
	typedef Scalar_Type ScalarType;// double or float

private:
	//--------------------- input-------------------------------------------//
	const ObjectArray<DenseMatrix<ScalarType>>* m_InputShapeList;
	// (*m_InputShapeList)[k] is Shape_k, 3 by N matrix, N is PointCount

	const ObjectArray<SparseVector<ScalarType>>* m_InputSimilarityTable;
	//(*m_SimilarityTable)[k] is the similarity list of Shape_k compared to each neighbour
	//(*m_SimilarityTable)[k][n] is the similarity between Shape_k and Shape_n
	//(*m_InputSimilarityTable)[k][k] is NOT usd, it can be any value, e.g., 0 or 1

	bool m_Flag_use_SimilarityTransform;
	// true: SimilarityTransform
	// false: RigidTransform
	DenseVector<int_max> m_Landmark;// to get transform

	int_max m_MaxNeighbourCount;
	//Aij is 0 if Xi and Xj are not neighbour to each other
	// Xi can only have a limited number of neighbour = m_MaxNeighbourCount

	int_max m_MaxIterCount;
	// max iteration count for Algorithm Part-II

	int_max m_InputReferenceShapeIndex;
	// align all the other to this reference shape after Algorithm Part-II

	bool m_Flag_ComputeObjectiveFunctionValue;

	bool m_Flag_Parallel_UpdateTransform;
	// more memory needed

	int_max m_MaxThreadCount;

	//------------------------------- output ---------------------------------//
	ObjectArray<Parameter_of_SimilarityTransform3D<ScalarType>> m_OutputTransformList;	
	// {scale, rotation, translation}

	ObjectArray<DenseMatrix<ScalarType>> m_OutputShapeList;// each aligned shape

	DenseVector<ScalarType> m_ObjectiveFunctionValue;

public:
	DistanceMinimizationBasedShapeAligner3D();
	~DistanceMinimizationBasedShapeAligner3D();
	void Clear();	
	void SelectSimilarityTransform() { m_Flag_use_SimilarityTransform = true; }
	void SelectRigidTransform() { m_Flag_use_SimilarityTransform = false; }
	void SetLandmark(const DenseVector<int_max>& Landmark) { m_Landmark = Landmark; }
	void SetInputShapeList(const ObjectArray<DenseMatrix<ScalarType>>* InputShapeList) { m_InputShapeList = InputShapeList; }
	void SetInputReferenceShapeIndex(int_max) { m_InputReferenceShapeIndex = Index; }
	void SetInputSimilarityTable(const ObjectArray<SparseVector<ScalarType>>* SimilarityTable) { m_InputSimilarityTable = SimilarityTable; }
	void SetMaxNeighbourCount(int_max Count) { m_MaxNeighbourCount = Count; }
	void SetMaxIterCount(int_max Count) { m_MaxIterCount = Count; }
	void SetMaxThreadCount(int_max Count) { m_MaxThreadCount = Count; }
	void EnableObjectiveFunctionEvaluation(bool On_or_Off = true) { m_Flag_ComputeObjectiveFunctionValue = On_or_Off; }	
	void EnableParallelUpdateTransform(bool On_or_Off = true) { m_Flag_Parallel_UpdateTransform = On_or_Off; }
	bool CheckInput();
	void Update();
	ObjectArray<Parameter_of_SimilarityTransform3D<ScalarType>>& OutputTransformList() {return m_OutputTransformList;}
	ObjectArray<DenseMatrix<ScalarType>>& OutputShapeList() { return m_OutputShapeList; }
	DenseVector<ScalarType> GetObjectiveFunctionValue() {return m_ObjectiveFunctionValue;}
	//---------------- provide a default method to get similarity --------------------------------------------------------------------
	//Flag_use_SimilarityTransfrom is true then use SimilarityTransfrom
	//Flag_use_SimilarityTransfrom is false then use RigidTransfrom
	static ObjectArray<SparseVector<ScalarType>> ComputeShapeSimilarity(const ObjectArray<DenseMatrix<ScalarType>>& ShapeList, const std::string& TransformName, bool Flag_Symmetry, ScalarType SimilarityThreshold, int_max MaxThreadCount);
	static ScalarType ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const std::string& TransformName, bool Flag_Symmetry);

	static ObjectArray<SparseVector<ScalarType>> ComputeShapeSimilarity(const ObjectArray<DenseMatrix<ScalarType>>& ShapeList, const DenseVector<int_max>& Landmark, const std::string& TransformName, bool Flag_Symmetry, ScalarType SimilarityThreshold, int_max MaxThreadCount);
	static ScalarType ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark, const std::string& TransformName, bool Flag_Symmetry);

private:
	void FindInitialTransform();
	void UpdateTransform_sequential();
	void UpdateTransform_parallel();
	void AlignToReferenceShape();
	ScalarType ComputeObjectiveFunctionValue();
	Parameter_of_SimilarityTransform3D<ScalarType> EstimateTransformParameter(const DenseMatrix<ScalarType>& SourceShape, const DenseMatrix<ScalarType>& TagetShape);

private:
	DistanceMinimizationBasedShapeAligner3D(const DistanceMinimizationBasedShapeAligner3D&) = delete;
	void operator=(const DistanceMinimizationBasedShapeAligner3D&) = delete;
};

}//namespace mdk

#include "mdkDistanceMinimizationBasedShapeAligner3D.hpp"
