#pragma once

namespace mdk
{ 
template<typename ScalarType>
MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::MinimumStretchBasedTriangleMesh3DTo2DMapper()
{
	this->Clear();
}

template<typename ScalarType>
MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::~MinimumStretchBasedTriangleMesh3DTo2DMapper()
{
}

template<typename ScalarType>
void MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::Clear()
{
	m_InputMesh.Recreate();// do NOT use Clear
	m_BoundaryPointHandleList.Clear();
	m_UVTableOfBoundary.Clear();
	m_InnerPointHandleList.Clear();
	m_OutputMesh.Clear();

	m_MaxInteration = 100;
	m_DiffusionCoef = 0.5;
}

template<typename ScalarType>
void MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::SetDiffusionCoefficient(ScalarType Coef)
{
	m_DiffusionCoef = Coef;
}

template<typename ScalarType>
void  MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::SetMaxIteration(int_max MaxIteration)
{
	m_MaxInteration = MaxIteration;
}

template<typename ScalarType>
bool MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::CheckInput()
{
	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("m_InputMesh DataStructure is NOT Clean @ MinimumStretchBasedTriangleMesh3DTo2DMapper::CheckInput()")
		return false;
	}

	if (m_MaxInteration < 0)
	{
		MDK_Error("MaxInteration < 0 @ MinimumStretchBasedTriangleMesh3DTo2DMapper::CheckInput()")
		return false;
	}

	if (m_DiffusionCoef > 1 || m_DiffusionCoef < 0)
	{
		MDK_Error("Diffusion Coefficient is OUT of range (0,1) @ MinimumStretchBasedTriangleMesh3DTo2DMapper::CheckInput()")
		return false;
	}

	return true;
}

template<typename ScalarType>
void MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::Preprocess()
{
	auto PointCount = m_InputMesh.GetPointCount();
	auto PointCount_boundary = m_BoundaryPointHandleList.GetLength();
	auto PointCount_inner = PointCount - PointCount_boundary;

	m_InnerPointHandleList.Clear();
	m_InnerPointHandleList.SetCapacity(PointCount_inner);
	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto tempIndex = m_BoundaryPointHandleList.ExactMatch("first", it.GetPointHandle());
		if (tempIndex < 0)
		{
			m_InnerPointHandleList.Append(it.GetPointHandle());
		}
	}

	m_Map_PointIndex_to_InnerIndex.Clear();
	m_Map_PointIndex_to_InnerIndex.Resize(PointCount);
	m_Map_PointIndex_to_InnerIndex.Fill(-1);
	for (int_max k = 0; k < m_InnerPointHandleList.GetLength(); ++k)
	{
		m_Map_PointIndex_to_InnerIndex[m_InnerPointHandleList[k].GetIndex()] = k;
	}

	m_Map_PointIndex_to_BoundaryIndex.Clear();
	m_Map_PointIndex_to_BoundaryIndex.Resize(PointCount);
	m_Map_PointIndex_to_BoundaryIndex.Fill(-1);
	for (int_max k = 0; k < m_BoundaryPointHandleList.GetLength(); ++k)
	{
		m_Map_PointIndex_to_BoundaryIndex[m_BoundaryPointHandleList[k].GetIndex()] = k;
	}

	m_InputMesh.UpdateAreaOfFace(ALL);
}

template<typename ScalarType>
void MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->Preprocess();
	this->ApplyStretchMinimizationBasedParameterization();
}


template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>> MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeWeightMatrix_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh)
{
	auto PointCount_boundary = m_BoundaryPointHandleList.GetLength();
	auto PointCount_inner = m_InnerPointHandleList.GetLength();
	auto PointCount = PointCount_boundary + PointCount_inner;

	ObjectArray<SparseVector<ScalarType>> WeightMatrix;
	WeightMatrix.Resize(PointCount_inner);
	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		auto PointHandle = m_InnerPointHandleList[k];
		auto AdjPointHandleList = TargetMesh.Point(PointHandle).GetAdjacentPointHandleList();
		WeightMatrix[k].Resize(PointCount);
		for (int_max n = 0; n < AdjPointHandleList.GetLength(); ++n)
		{
			auto Weight = this->ComputeWeight_MeanValue(TargetMesh, PointHandle, AdjPointHandleList[n]);
			WeightMatrix[k].SetElement(AdjPointHandleList[n].GetIndex(), Weight);
		}
	}
	return WeightMatrix;
}

template<typename ScalarType>
ScalarType MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeWeight_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1)
{
	// Weight_0_1 != Weight_1_0
	//
	//-----------------
	//     0
	//   / | \ 
	//  /__|__\
	//  2  1  3
	//-----------------

	auto AdjPointHandleList0 = TargetMesh.Point(PointH0).GetAdjacentPointHandleList();
	auto AdjPointHandleList1 = TargetMesh.Point(PointH1).GetAdjacentPointHandleList();
	auto Set23 = Intersect(AdjPointHandleList0, AdjPointHandleList1);
	auto PointH2 = Set23[0];
	auto PointH3 = Set23[1];

	auto Pos0 = TargetMesh.GetPointPosition(PointH0);
	auto Pos1 = TargetMesh.GetPointPosition(PointH1);
	auto Pos2 = TargetMesh.GetPointPosition(PointH2);
	auto Pos3 = TargetMesh.GetPointPosition(PointH3);

	auto V01 = Pos1 - Pos0;
	auto V02 = Pos2 - Pos0;
	auto V03 = Pos3 - Pos0;
	auto Angle102_half = ComputeAngleBetweenTwoVectorIn3D(V01, V02) / ScalarType(2);
	auto Angle103_half = ComputeAngleBetweenTwoVectorIn3D(V01, V03) / ScalarType(2);
	auto tan_Angle102_half = std::tan(Angle102_half);
	auto tan_Angle103_half = std::tan(Angle103_half);
	auto L01 = V01.L2Norm();
	auto Weight = (tan_Angle102_half + tan_Angle103_half) / L01;

	return Weight;
}

template<typename ScalarType>
void MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ApplyStretchMinimizationBasedParameterization()
{
	auto PointCount_boundary = m_BoundaryPointHandleList.GetLength();
	auto PointCount_inner = m_InnerPointHandleList.GetLength();
	//-------------------------- initialization by using Mean Value Method ---------------------------------//
	auto WeightMatrix = this->ComputeWeightMatrix_MeanValue(m_InputMesh);
	auto UVTable_init = this->ComputeUV_Given_WeightMatrix(WeightMatrix);
	m_OutputMesh.Clear();
	m_OutputMesh = m_InputMesh;
	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		auto u = UVTable_init(0, k);
		auto v = UVTable_init(1, k);
		m_OutputMesh.SetPointPosition(m_InnerPointHandleList[k], u, v, 0);
	}
	for (int_max k = 0; k < PointCount_boundary; ++k)
	{
		auto u = m_UVTableOfBoundary(0, k);
		auto v = m_UVTableOfBoundary(1, k);
		m_OutputMesh.SetPointPosition(m_BoundaryPointHandleList[k], u, v, 0);
	}
	//------------------------------------------------------------------------------------------------------//
	int_max MaxIter = m_MaxInteration;
	ScalarType alpha = m_DiffusionCoef;
	if (alpha <= std::numeric_limits<ScalarType>::epsilon())
	{
		MaxIter = 1;
	}

	ScalarType AverageStretch_old, AverageStretch;

	for (int_max Iter = 0; Iter < MaxIter; ++Iter)
	{		
   	    auto StretchList_face = this->ComputeLocalStretch_Face();
		auto StretchList_point = this->ComputeLocalStretch_Point(StretchList_face);
		AverageStretch = this->ComputeAverageLocalStretch(StretchList_face);
		
		//std::cout << "Iter=" << Iter << ", AverageStretch = " << AverageStretch << '\n';

		// check convergence
		if (Iter > 0)
		{
			if (AverageStretch > AverageStretch_old)
			{
				break;
			}
		}

		//update weight by using local stretch
		for (int_max k = 0; k < PointCount_inner; ++k)
		{
			const auto& AdjPointIndexList = WeightMatrix[k].IndexList();
			auto& WeightList = WeightMatrix[k].ElementList();
			auto AdjPointCount = WeightList.GetLength();
			for (int_max n = 0; n < AdjPointCount; ++n)
			{
				auto Stretch = StretchList_point[AdjPointIndexList[n]];
				Stretch = std::pow(Stretch, alpha);
				WeightList[n] /= Stretch;				
			}
		}

		//update UV
		auto UVTable = this->ComputeUV_Given_WeightMatrix(WeightMatrix);
		for (int_max k = 0; k < PointCount_inner; ++k)
		{
			auto u = UVTable(0, k);
			auto v = UVTable(1, k);
			m_OutputMesh.SetPointPosition(m_InnerPointHandleList[k], u, v, 0);
		}

		//update record
		AverageStretch_old = AverageStretch;
	}
}


template<typename ScalarType>
DenseVector<ScalarType> MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeLocalStretch_Point()
{// compute stretch at every point
	auto FaceCount = m_InputMesh.GetFaceCount();
	auto PointCount = m_InputMesh.GetPointCount();

	DenseVector<ScalarType> StretchList_face;
	StretchList_face.Resize(FaceCount);

	for (int_max k = 0; k < FaceCount; ++k)
	{
		FaceHandleType FaceHandle;
		FaceHandle.SetIndex(k);
		auto PointHandleList_input = m_InputMesh.Face(FaceHandle).GetPointHandleList();
		auto PointPosition_input = m_InputMesh.GetPointPosition(PointHandleList_input);
		auto PointHandleList_output = m_OutputMesh.Face(FaceHandle).GetPointHandleList();
		auto PointPosition_output = m_OutputMesh.GetPointPosition(PointHandleList_output);

		//NOT this affine
		/*
		AffineTransform3D<ScalarType> AffineMap;
		AffineMap.SetSourceLandmarkPointSet(&PointPosition_output);
		AffineMap.SetTargetLandmarkPointSet(&PointPosition_input);
		AffineMap.EstimateParameter();
		auto Transform = AffineMap.GetTransformationMatrix();
		auto T = Transform.GetSubMatrix(span(0, 2), span(0, 2));	
		auto TT = MatrixMultiply(T, T.Transpose());
		auto Result = SymmetricRealMatrixEigen(TT);
		auto MaxValue = Result.EigenValue.Max();
		auto MinValue = Result.EigenValue.Min();
		*/
		//
		auto s1 = PointPosition_output(0, 0);
		auto t1 = PointPosition_output(1, 0);
		auto s2 = PointPosition_output(0, 1);
		auto t2 = PointPosition_output(1, 1);
		auto s3 = PointPosition_output(0, 2);
		auto t3 = PointPosition_output(1, 2);	
		auto q1 = m_InputMesh.GetPointPosition(PointHandleList_input[0]);
		auto q2 = m_InputMesh.GetPointPosition(PointHandleList_input[1]);
		auto q3 = m_InputMesh.GetPointPosition(PointHandleList_input[2]);
		auto A = ((s2 - s1)*(t3 - t1) - (s3 - s1)*(t2 - t1)) / 2;
		auto Ss = (q1*(t2 - t3) + q2*(t3 - t1) + q3*(t1 - t2)) / (2 * A);
		auto St = (q1*(s3 - s2) + q2*(s1 - s3) + q3*(s2 - s1)) / (2 * A);
		auto a = ComputeVectorDotProductIn3D(Ss, Ss);
		auto b = ComputeVectorDotProductIn3D(Ss, St);
		auto c = ComputeVectorDotProductIn3D(St, St);
		auto MinValue = std::sqrt(((a + c) - std::sqrt((a - c)*(a - c) + 4*b*b)) / 2);
		auto MaxValue = std::sqrt(((a + c) + std::sqrt((a - c)*(a - c) + 4*b*b)) / 2);

		StretchList_face[k] = std::sqrt((MaxValue + MinValue) / ScalarType(2));
	}

	DenseVector<ScalarType> StretchList_point;
	StretchList_point.Resize(PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		PointHandleType PointHandle;
		PointHandle.SetIndex(k);
		auto AdjFaceHandleList = m_InputMesh.Point(PointHandle).GetAdjacentFaceHandleList();
		ScalarType Value_a = 0;
		ScalarType Value_b = 0;
		for (int_max n = 0; n < AdjFaceHandleList.GetLength(); ++n)
		{
			auto Area = m_InputMesh.Face(AdjFaceHandleList[n]).Attribute().Area;
			auto Stretch = StretchList_face[AdjFaceHandleList[n].GetIndex()];
			Value_a += Area*Stretch*Stretch;
			Value_b += Area;
		}
		StretchList_point[k] = std::sqrt(Value_a / Value_b);
	}

	return StretchList_point;
}


template<typename ScalarType>
DenseVector<ScalarType> MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeLocalStretch_Face()
{// compute stretch at every face
	auto FaceCount = m_InputMesh.GetFaceCount();

	DenseVector<ScalarType> StretchList_face;
	StretchList_face.Resize(FaceCount);

	for (int_max k = 0; k < FaceCount; ++k)
	{
		FaceHandleType FaceHandle;
		FaceHandle.SetIndex(k);
		auto PointHandleList_input = m_InputMesh.Face(FaceHandle).GetPointHandleList();
		auto PointPosition_input = m_InputMesh.GetPointPosition(PointHandleList_input);
		auto PointHandleList_output = m_OutputMesh.Face(FaceHandle).GetPointHandleList();
		auto PointPosition_output = m_OutputMesh.GetPointPosition(PointHandleList_output);

		//NOT this affine
		/*
		AffineTransform3D<ScalarType> AffineMap;
		AffineMap.SetSourceLandmarkPointSet(&PointPosition_output);
		AffineMap.SetTargetLandmarkPointSet(&PointPosition_input);
		AffineMap.EstimateParameter();
		auto Transform = AffineMap.GetTransformationMatrix();
		auto T = Transform.GetSubMatrix(span(0, 2), span(0, 2));
		auto TT = MatrixMultiply(T, T.Transpose());
		auto Result = SymmetricRealMatrixEigen(TT);
		auto MaxValue = Result.EigenValue.Max();
		auto MinValue = Result.EigenValue.Min();
		*/
		//
		auto s1 = PointPosition_output(0, 0);
		auto t1 = PointPosition_output(1, 0);
		auto s2 = PointPosition_output(0, 1);
		auto t2 = PointPosition_output(1, 1);
		auto s3 = PointPosition_output(0, 2);
		auto t3 = PointPosition_output(1, 2);
		auto q1 = m_InputMesh.GetPointPosition(PointHandleList_input[0]);
		auto q2 = m_InputMesh.GetPointPosition(PointHandleList_input[1]);
		auto q3 = m_InputMesh.GetPointPosition(PointHandleList_input[2]);
		auto A = ((s2 - s1)*(t3 - t1) - (s3 - s1)*(t2 - t1)) / 2;
		auto Ss = (q1*(t2 - t3) + q2*(t3 - t1) + q3*(t1 - t2)) / (2 * A);
		auto St = (q1*(s3 - s2) + q2*(s1 - s3) + q3*(s2 - s1)) / (2 * A);
		auto a = ComputeVectorDotProductIn3D(Ss, Ss);
		auto b = ComputeVectorDotProductIn3D(Ss, St);
		auto c = ComputeVectorDotProductIn3D(St, St);
		auto MinValue = std::sqrt(((a + c) - std::sqrt((a - c)*(a - c) + 4 * b*b)) / 2);
		auto MaxValue = std::sqrt(((a + c) + std::sqrt((a - c)*(a - c) + 4 * b*b)) / 2);

		StretchList_face[k] = std::sqrt((MaxValue + MinValue) / ScalarType(2));
	}

	return StretchList_face;
}


template<typename ScalarType>
DenseVector<ScalarType> MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeLocalStretch_Point(const DenseVector<ScalarType>& StretchList_face)
{
	auto PointCount = m_InputMesh.GetPointCount();

	DenseVector<ScalarType> StretchList_point;
	StretchList_point.Resize(PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		PointHandleType PointHandle;
		PointHandle.SetIndex(k);
		auto AdjFaceHandleList = m_InputMesh.Point(PointHandle).GetAdjacentFaceHandleList();
		ScalarType Value_a = 0;
		ScalarType Value_b = 0;
		for (int_max n = 0; n < AdjFaceHandleList.GetLength(); ++n)
		{
			auto Area = m_InputMesh.Face(AdjFaceHandleList[n]).Attribute().Area;
			auto Stretch = StretchList_face[AdjFaceHandleList[n].GetIndex()];
			Value_a += Area*Stretch*Stretch;
			Value_b += Area;
		}
		StretchList_point[k] = std::sqrt(Value_a / Value_b);
	}

	return StretchList_point;
}


template<typename ScalarType>
ScalarType MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeAverageLocalStretch(const DenseVector<ScalarType>& StretchList_face)
{
	ScalarType StretchSum = 0;
	ScalarType AreaSum = 0;
	auto FaceCount = StretchList_face.GetLength();
	for (int_max k = 0; k < FaceCount; ++k)
	{
		FaceHandleType FaceHandle;
		FaceHandle.SetIndex(k);
		auto Area = m_InputMesh.Face(FaceHandle).Attribute().Area;
		StretchSum += Area*StretchList_face[k] * StretchList_face[k];
		AreaSum += Area;
	}
	auto AverageStretch = std::sqrt(StretchSum / AreaSum);
	return AverageStretch;
}


template<typename ScalarType>
DenseMatrix<ScalarType> MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeUV_Given_WeightMatrix(const ObjectArray<SparseVector<ScalarType>>& WeightMatrix)
{
	auto PointCount_inner = WeightMatrix.GetLength();

	std::vector<Eigen::Triplet<ScalarType, int_max>> CoefList;
	Eigen::VectorXd Ub(PointCount_inner), Vb(PointCount_inner);

	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		const auto& AdjPointIndexList = WeightMatrix[k].IndexList();
		const auto& WeightList = WeightMatrix[k].ElementList();
		auto AdjPointCount = WeightList.GetLength();

		DenseVector<int_max> AdjIndexList_inner;// index in WeightMatrix
		DenseVector<int_max> AdjIndexList_boundary;// index in m_UVTableOfBoundary, index+PointCount_inner in WeightMatrix
		AdjIndexList_inner.SetCapacity(AdjPointCount);
		AdjIndexList_boundary.SetCapacity(AdjPointCount);

		DenseVector<ScalarType> WeightList_inner, WeightList_boundary;
		WeightList_inner.SetCapacity(AdjPointCount);
		WeightList_boundary.SetCapacity(AdjPointCount);

		for (int_max n = 0; n < AdjPointCount; ++n)
		{
			auto Index_inner = m_Map_PointIndex_to_InnerIndex[AdjPointIndexList[n]];
			auto Index_boundary = m_Map_PointIndex_to_BoundaryIndex[AdjPointIndexList[n]];
			if (Index_inner >= 0)
			{
				AdjIndexList_inner.Append(Index_inner);
				WeightList_inner.Append(WeightList[n]);
			}
			else
			{
				AdjIndexList_boundary.Append(Index_boundary);
				WeightList_boundary.Append(WeightList[n]);
			}
		}

		auto Weight_kk = WeightList_inner.Sum() + WeightList_boundary.Sum();
		Weight_kk = -Weight_kk;

		//-------------------------------			
		for (int_max n = 0; n < AdjIndexList_inner.GetLength(); ++n)
		{
			int_max RowIndex = k;
			int_max ColIndex = AdjIndexList_inner[n];
			ScalarType Weight = WeightList_inner[n];
			Eigen::Triplet<ScalarType, int_max> Coef(RowIndex, ColIndex, Weight);
			CoefList.push_back(Coef);
		}
		Eigen::Triplet<ScalarType, int_max> Coef(k, k, Weight_kk);
		CoefList.push_back(Coef);
		//-------------------------------	
		ScalarType ub = 0;
		ScalarType vb = 0;
		for (int_max n = 0; n < AdjIndexList_boundary.GetLength(); ++n)
		{
			auto u = m_UVTableOfBoundary(0, AdjIndexList_boundary[n]);
			auto v = m_UVTableOfBoundary(1, AdjIndexList_boundary[n]);
			ub += -WeightList_boundary[n] * u;
			vb += -WeightList_boundary[n] * v;
		}
		Ub[k] = ub;
		Vb[k] = vb;
	}

	//-------------------------------------------------------------//
	typedef Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> EigenVector;
	typedef Eigen::SparseMatrix<ScalarType,0,int_max> EigenSparseMatrix;
	EigenSparseMatrix A(PointCount_inner, PointCount_inner);
	A.setFromTriplets(CoefList.begin(), CoefList.end());
	//solve
	Eigen::SparseLU<EigenSparseMatrix> solver;
	solver.analyzePattern(A);
	solver.factorize(A);
	EigenVector U = solver.solve(Ub);//assume no internal state in this solve function
	EigenVector V = solver.solve(Vb);//
	//-------------------------------------------------------------------//
	DenseMatrix<ScalarType> UVTable;
	UVTable.Resize(2, PointCount_inner);
	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		UVTable(0, k) = U(k);
		UVTable(1, k) = V(k);		
	}
	return UVTable;
}

template<typename ScalarType>
DenseVector<typename MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::PointHandleType> 
MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ConvertIndexToHandle(const DenseVector<int_max>& IndexList)
{
	DenseVector<PointHandleType> HandleList;
	HandleList.Resize(IndexList.GetLength());
	for (int_max k = 0; k < IndexList.GetLength(); ++k)
	{
		HandleList[k].SetIndex(IndexList[k]);
	}
	return HandleList;
}

}//namespace


