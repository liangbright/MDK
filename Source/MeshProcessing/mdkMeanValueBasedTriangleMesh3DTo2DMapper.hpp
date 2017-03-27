#pragma once

namespace mdk
{ 
template<typename ScalarType>
MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::MeanValueBasedTriangleMesh3DTo2DMapper()
{
	this->Clear();
}

template<typename ScalarType>
MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::~MeanValueBasedTriangleMesh3DTo2DMapper()
{
}

template<typename ScalarType>
void MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::Clear()
{
	auto& Self = *this;
	Self.InputMesh.Clear();
	Self.BoundaryPointHandleList.Clear();
	Self.UVTableOfBoundary.Clear();
	Self.InnerPointHandleList.Clear();
	Self.OutputMesh.Clear();
}


template<typename ScalarType>
bool MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::CheckInput()
{
	auto& Self = *this;
	if (Self.InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructure is NOT Clean @ MeanValueBasedTriangleMesh3DTo2DMapper::CheckInput()")
		return false;
	}
	return true;
}

template<typename ScalarType>
void MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::Preprocess()
{
	auto& Self = *this;

	auto PointCount = Self.InputMesh.GetPointCount();
	auto PointCount_boundary = Self.BoundaryPointHandleList.GetLength();
	auto PointCount_inner = PointCount - PointCount_boundary;

	Self.InnerPointHandleList.Clear();
	Self.InnerPointHandleList.SetCapacity(PointCount_inner);
	for (auto it = Self.InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto tempIndex = Self.BoundaryPointHandleList.ExactMatch("first", it.GetPointHandle());
		if (tempIndex < 0)
		{
			Self.InnerPointHandleList.Append(it.GetPointHandle());
		}
	}

	Self.Map_PointIndex_to_InnerIndex.Clear();
	Self.Map_PointIndex_to_InnerIndex.Resize(PointCount);
	Self.Map_PointIndex_to_InnerIndex.Fill(-1);
	for (int_max k = 0; k < Self.InnerPointHandleList.GetLength(); ++k)
	{
		Self.Map_PointIndex_to_InnerIndex[Self.InnerPointHandleList[k].GetIndex()] = k;
	}

	Self.Map_PointIndex_to_BoundaryIndex.Clear();
	Self.Map_PointIndex_to_BoundaryIndex.Resize(PointCount);
	Self.Map_PointIndex_to_BoundaryIndex.Fill(-1);
	for (int_max k = 0; k < Self.BoundaryPointHandleList.GetLength(); ++k)
	{
		Self.Map_PointIndex_to_BoundaryIndex[Self.BoundaryPointHandleList[k].GetIndex()] = k;
	}

	Self.InputMesh.UpdateAreaOfFace(ALL);
}

template<typename ScalarType>
void MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return false;
	}

	this->Preprocess();
	this->ApplyMeanValueBasedParameterization();
}

template<typename ScalarType>
void MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::ApplyMeanValueBasedParameterization()
{
	auto& Self = *this;

	auto PointCount_boundary = Self.BoundaryPointHandleList.GetLength();
	auto PointCount_inner = Self.InnerPointHandleList.GetLength();
	auto WeightMatrix = this->ComputeWeightMatrix_MeanValue(Self.InputMesh);
	auto UVTable = this->ComputeUV_Given_WeightMatrix(WeightMatrix);
   //-------------------------------------------------------------------//
	Self.OutputMesh.Clear();
	Self.OutputMesh = Self.InputMesh;
	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		auto u = UVTable(0, k);
		auto v = UVTable(1, k);
		Self.OutputMesh.SetPointPosition(Self.InnerPointHandleList[k], u, v, 0);
	}
	for (int_max k = 0; k < PointCount_boundary; ++k)
	{
		auto u = Self.UVTableOfBoundary(0, k);
		auto v = Self.UVTableOfBoundary(1, k);
		Self.OutputMesh.SetPointPosition(Self.BoundaryPointHandleList[k], u, v, 0);
	}
}


template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>> MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeWeightMatrix_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh)
{
	auto& Self = *this;

	auto PointCount_boundary = Self.BoundaryPointHandleList.GetLength();
	auto PointCount_inner = Self.InnerPointHandleList.GetLength();
	auto PointCount = PointCount_boundary + PointCount_inner;

	ObjectArray<SparseVector<ScalarType>> WeightMatrix;
	WeightMatrix.Resize(PointCount_inner);
	for (int_max k = 0; k < PointCount_inner; ++k)
	{
		auto PointHandle = Self.InnerPointHandleList[k];
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
ScalarType MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeWeight_MeanValue(const TriangleMesh<MeshAttributeType>& TargetMesh, PointHandleType PointH0, PointHandleType PointH1)
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
DenseMatrix<ScalarType> MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::ComputeUV_Given_WeightMatrix(const ObjectArray<SparseVector<ScalarType>>& WeightMatrix)
{
	auto PointCount_inner = WeightMatrix.GetLength();

	std::vector<Eigen::Triplet<ScalarType, int_max>> CoefList;
	//Eigen::VectorXd Ub(PointCount_inner), Vb(PointCount_inner);
	Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> Ub(PointCount_inner), Vb(PointCount_inner);

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
DenseVector<typename MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::PointHandleType> 
MeanValueBasedTriangleMesh3DTo2DMapper<ScalarType>::ConvertIndexToHandle(const DenseVector<int_max>& IndexList)
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


