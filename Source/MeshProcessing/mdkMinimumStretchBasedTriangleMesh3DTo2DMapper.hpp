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
	auto& Self = *this;

	Self.InputMesh.Recreate();// do NOT use Clear
	Self.BoundaryPointIndexList.Clear();
	Self.UVTableOfBoundary.Clear();
	Self.InnerPointIndexList.Clear();
	Self.Map_PointIndex_to_Inner.Clear();
	Self.Map_PointIndex_to_Boundary.Clear();
	Self.OutputMesh.Clear();

	Self.MaxInteration = 100;
	Self.DiffusionCoefficient = 0.5;
}


template<typename ScalarType>
bool MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::CheckInput()
{
	auto& Self = *this;

	if (Self.InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("Self.InputMesh DataStructure is NOT Clean @ MinimumStretchBasedTriangleMesh3DTo2DMapper::CheckInput()")
		return false;
	}

	if (Self.MaxInteration < 0)
	{
		MDK_Error("MaxInteration < 0 @ MinimumStretchBasedTriangleMesh3DTo2DMapper::CheckInput()")
		return false;
	}

	if (Self.DiffusionCoefficient > 1 || Self.DiffusionCoefficient < 0)
	{
		MDK_Error("Diffusion Coefficient is OUT of range (0,1) @ MinimumStretchBasedTriangleMesh3DTo2DMapper::CheckInput()")
		return false;
	}

	return true;
}

template<typename ScalarType>
void MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::Preprocess()
{
	auto& Self = *this;

	auto PointCount = Self.InputMesh.GetPointCount();
	auto PointCount_boundary = Self.BoundaryPointIndexList.GetLength();
	auto PointCount_inner = PointCount - PointCount_boundary;

	Self.InnerPointIndexList.Clear();
	Self.InnerPointIndexList.SetCapacity(PointCount_inner);
	for (auto it = Self.InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto tempIndex = Self.BoundaryPointIndexList.ExactMatch("first", it.GetPointIndex());
		if (tempIndex < 0)
		{
			Self.InnerPointIndexList.Append(it.GetPointIndex());
		}
	}

	Self.Map_PointIndex_to_Inner.Clear();
	Self.Map_PointIndex_to_Inner.Resize(PointCount);
	Self.Map_PointIndex_to_Inner.Fill(-1);
	for (int_max k = 0; k < Self.InnerPointIndexList.GetLength(); ++k)
	{
		Self.Map_PointIndex_to_Inner[Self.InnerPointIndexList[k]] = k;
	}

	Self.Map_PointIndex_to_Boundary.Clear();
	Self.Map_PointIndex_to_Boundary.Resize(PointCount);
	Self.Map_PointIndex_to_Boundary.Fill(-1);
	for (int_max k = 0; k < Self.BoundaryPointIndexList.GetLength(); ++k)
	{
		Self.Map_PointIndex_to_Boundary[Self.BoundaryPointIndexList[k]] = k;
	}

	Self.InputMesh.UpdateAreaOfFace(ALL);
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
	auto& Self = *this;

	auto PointCount_boundary = Self.BoundaryPointIndexList.GetLength();
	auto PointCount_inner = Self.InnerPointIndexList.GetLength();
	auto PointCount = PointCount_boundary + PointCount_inner;

	ObjectArray<SparseVector<ScalarType>> WeightMatrix;
	WeightMatrix.Resize(PointCount_inner);
	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		auto PointIndex = Self.InnerPointIndexList[k];
		auto AdjPointIndexList = TargetMesh.Point(PointIndex).GetAdjacentPointIndexList();
		WeightMatrix[k].Resize(PointCount);
		for (int_max n = 0; n < AdjPointIndexList.GetLength(); ++n)
		{
			auto Weight = this->ComputeWeight_MeanValue(TargetMesh, PointIndex, AdjPointIndexList[n]);
			WeightMatrix[k].SetElement(AdjPointIndexList[n], Weight);
		}
	}
	return WeightMatrix;
}

template<typename ScalarType>
ScalarType MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeWeight_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh, int_max PointH0, int_max PointH1)
{
	// Weight_0_1 != Weight_1_0
	//
	//-----------------
	//     0
	//   / | \ 
	//  /__|__\
	//  2  1  3
	//-----------------

	auto AdjPointIndexList0 = TargetMesh.Point(PointH0).GetAdjacentPointIndexList();
	auto AdjPointIndexList1 = TargetMesh.Point(PointH1).GetAdjacentPointIndexList();
	auto Set23 = Intersect(AdjPointIndexList0, AdjPointIndexList1);
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
	auto& Self = *this;
	auto PointCount_boundary = Self.BoundaryPointIndexList.GetLength();
	auto PointCount_inner = Self.InnerPointIndexList.GetLength();
	//-------------------------- initialization by using Mean Value Method ---------------------------------//
	auto WeightMatrix = this->ComputeWeightMatrix_MeanValue(Self.InputMesh);
	auto UVTable_init = this->ComputeUV_Given_WeightMatrix(WeightMatrix);
	Self.OutputMesh.Clear();
	Self.OutputMesh = Self.InputMesh;
	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		auto u = UVTable_init(0, k);
		auto v = UVTable_init(1, k);
		Self.OutputMesh.SetPointPosition(Self.InnerPointIndexList[k], u, v, 0);
	}
	for (int_max k = 0; k < PointCount_boundary; ++k)
	{
		auto u = Self.UVTableOfBoundary(0, k);
		auto v = Self.UVTableOfBoundary(1, k);
		Self.OutputMesh.SetPointPosition(Self.BoundaryPointIndexList[k], u, v, 0);
	}
	//------------------------------------------------------------------------------------------------------//
	int_max MaxIter = Self.MaxInteration;
	ScalarType alpha = Self.DiffusionCoefficient;
	if (alpha <= std::numeric_limits<ScalarType>::epsilon())
	{
		MaxIter = 0;
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
			Self.OutputMesh.SetPointPosition(Self.InnerPointIndexList[k], u, v, 0);
		}

		//update record
		AverageStretch_old = AverageStretch;
	}
}


template<typename ScalarType>
DenseVector<ScalarType> MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeLocalStretch_Point()
{// compute stretch at every point
	auto& Self = *this;
	auto FaceCount = Self.InputMesh.GetFaceCount();
	auto PointCount = Self.InputMesh.GetPointCount();

	DenseVector<ScalarType> StretchList_face;
	StretchList_face.Resize(FaceCount);

	for (int_max k = 0; k < FaceCount; ++k)
	{
		int_max FaceIndex = k;
		auto PointIndexList_input = Self.InputMesh.Face(FaceIndex).GetPointIndexList();
		auto PointPosition_input = Self.InputMesh.GetPointPosition(PointIndexList_input);
		auto PointIndexList_output = Self.OutputMesh.Face(FaceIndex).GetPointIndexList();
		auto PointPosition_output = Self.OutputMesh.GetPointPosition(PointIndexList_output);

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
		auto q1 = Self.InputMesh.GetPointPosition(PointIndexList_input[0]);
		auto q2 = Self.InputMesh.GetPointPosition(PointIndexList_input[1]);
		auto q3 = Self.InputMesh.GetPointPosition(PointIndexList_input[2]);
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
		int_max PointIndex = k;
		auto AdjFaceIndexList = Self.InputMesh.Point(PointIndex).GetAdjacentFaceIndexList();
		ScalarType Value_a = 0;
		ScalarType Value_b = 0;
		for (int_max n = 0; n < AdjFaceIndexList.GetLength(); ++n)
		{
			auto Area = Self.InputMesh.Face(AdjFaceIndexList[n]).Attribute().Area;
			auto Stretch = StretchList_face[AdjFaceIndexList[n]];
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
	auto& Self = *this;
	auto FaceCount = Self.InputMesh.GetFaceCount();

	DenseVector<ScalarType> StretchList_face;
	StretchList_face.Resize(FaceCount);

	for (int_max k = 0; k < FaceCount; ++k)
	{
		int_max FaceIndex = k;
		auto PointIndexList_input = Self.InputMesh.Face(FaceIndex).GetPointIndexList();
		auto PointPosition_input = Self.InputMesh.GetPointPosition(PointIndexList_input);
		auto PointIndexList_output = Self.OutputMesh.Face(FaceIndex).GetPointIndexList();
		auto PointPosition_output = Self.OutputMesh.GetPointPosition(PointIndexList_output);

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
		auto q1 = Self.InputMesh.GetPointPosition(PointIndexList_input[0]);
		auto q2 = Self.InputMesh.GetPointPosition(PointIndexList_input[1]);
		auto q3 = Self.InputMesh.GetPointPosition(PointIndexList_input[2]);
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
	auto& Self = *this;
	auto PointCount = Self.InputMesh.GetPointCount();

	DenseVector<ScalarType> StretchList_point;
	StretchList_point.Resize(PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		int_max PointIndex = k;
		auto AdjFaceIndexList = Self.InputMesh.Point(PointIndex).GetAdjacentFaceIndexList();
		ScalarType Value_a = 0;
		ScalarType Value_b = 0;
		for (int_max n = 0; n < AdjFaceIndexList.GetLength(); ++n)
		{
			auto Area = Self.InputMesh.Face(AdjFaceIndexList[n]).Attribute().Area;
			auto Stretch = StretchList_face[AdjFaceIndexList[n]];
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
	auto& Self = *this;
	ScalarType StretchSum = 0;
	ScalarType AreaSum = 0;
	auto FaceCount = StretchList_face.GetLength();
	for (int_max k = 0; k < FaceCount; ++k)
	{
		int_max FaceIndex = k;
		auto Area = Self.InputMesh.Face(FaceIndex).Attribute().Area;
		StretchSum += Area*StretchList_face[k] * StretchList_face[k];
		AreaSum += Area;
	}
	auto AverageStretch = std::sqrt(StretchSum / AreaSum);
	return AverageStretch;
}


template<typename ScalarType>
DenseMatrix<ScalarType> MinimumStretchBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeUV_Given_WeightMatrix(const ObjectArray<SparseVector<ScalarType>>& WeightMatrix)
{
	auto& Self = *this;

	auto PointCount_inner = WeightMatrix.GetLength();

	std::vector<Eigen::Triplet<ScalarType, int_max>> CoefList;
	Eigen::VectorXd Ub(PointCount_inner), Vb(PointCount_inner);

	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		const auto& AdjPointIndexList = WeightMatrix[k].IndexList();
		const auto& WeightList = WeightMatrix[k].ElementList();
		auto AdjPointCount = WeightList.GetLength();

		DenseVector<int_max> AdjIndexList_inner;// index in WeightMatrix
		DenseVector<int_max> AdjIndexList_boundary;// index in Self.UVTableOfBoundary, index+PointCount_inner in WeightMatrix
		AdjIndexList_inner.SetCapacity(AdjPointCount);
		AdjIndexList_boundary.SetCapacity(AdjPointCount);

		DenseVector<ScalarType> WeightList_inner, WeightList_boundary;
		WeightList_inner.SetCapacity(AdjPointCount);
		WeightList_boundary.SetCapacity(AdjPointCount);

		for (int_max n = 0; n < AdjPointCount; ++n)
		{
			auto Index_inner = Self.Map_PointIndex_to_Inner[AdjPointIndexList[n]];
			auto Index_boundary = Self.Map_PointIndex_to_Boundary[AdjPointIndexList[n]];
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
			auto u = Self.UVTableOfBoundary(0, AdjIndexList_boundary[n]);
			auto v = Self.UVTableOfBoundary(1, AdjIndexList_boundary[n]);
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

}//namespace


