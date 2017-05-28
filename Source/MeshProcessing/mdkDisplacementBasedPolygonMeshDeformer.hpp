#pragma once

namespace mdk
{
template<typename MeshAttribute>
DisplacementBasedPolygonMeshDeformer<MeshAttribute>::DisplacementBasedPolygonMeshDeformer()
{
	this->Clear();
}

template<typename MeshAttribute>
DisplacementBasedPolygonMeshDeformer<MeshAttribute>::~DisplacementBasedPolygonMeshDeformer()
{
}

template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::Clear()
{
	auto& Self = *this;
	
	// dot NOT use Clear
	Self.Input.SourceMesh.Recreate();
	Self.Input.DisplacementField.Recreate();
	Self.Input.ConfidenceOfDisplacementField.Recreate();
	Self.Input.ConfidenceOfSmoothness.Recreate();
	Self.WeigthType = WeightTypeEnum::Distance;

	Self.Internal.WeightMatrix.Clear();
	Self.Output.DisplacementField.Clear();
	Self.Output.DeformedMesh.Clear();
}

template<typename MeshAttribute>
bool DisplacementBasedPolygonMeshDeformer<MeshAttribute>::CheckInput()
{
	auto& Self = *this;
	if (Self.Input.SourceMesh.IsEmpty() == true)
	{
		return false;
	}

	if (Self.Input.SourceMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("InputMesh DataStructureClean is NOT clean @ DisplacementBasedPolygonMeshDeformer::CheckInput()")
		return false;
	}

	return true;
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->ComputeWeightMatrix();
	this->ComputeDisplacementField();
	
	auto& Self = *this;
	Self.Output.DeformedMesh.Clear();
	if (Self.Output.DisplacementField.IsEmpty() == false)
	{
		Self.Output.DeformedMesh = Self.Input.SourceMesh;
		auto PointCount = Self.Output.DeformedMesh.GetPointCount();
		for (int_max k = 0; k < PointCount; ++k)
		{
			DenseVector<ScalarType, 3> Displacement;
			Self.Output.DisplacementField.GetCol(k, Displacement);
			auto Pos = Self.Output.DeformedMesh.GetPointPosition(k);
			Pos += Displacement;
			Self.Output.DeformedMesh.SetPointPosition(k, Pos);
		}
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeWeightMatrix()
{
	auto& Self = *this;
	switch (Self.Input.WeigthType)
	{
	case WeightTypeEnum::One:
		this->ComputeWeightMatrix_Type_One();
		return;
	case WeightTypeEnum::Distance:
		this->ComputeWeightMatrix_Type_Distance();
		return;
	default:
		this->ComputeWeightMatrix_Type_One();
		return;
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeWeightMatrix_Type_One()
{
	auto& Self = *this;
	auto PointCount = Self.Input.SourceMesh.GetPointCount();
	Self.Internal.WeightMatrix.Clear();
	Self.Internal.WeightMatrix.Resize(PointCount);
	// sum_j {Lambda[i][j]} = 1 for a fixed i

	for (int_max k = 0; k < PointCount; ++k)
	{
		auto Pos_k = Self.Input.SourceMesh.GetPointPosition(k);
		auto AdjPointIndexList = Self.Input.SourceMesh.Point(k).GetAdjacentPointIndexList();
		DenseVector<ScalarType> WeightList;
		WeightList.Resize(AdjPointIndexList.GetLength());		
		WeightList.Fill(ScalarType(1) / AdjPointIndexList.GetLength());
		Self.Internal.WeightMatrix[k].Resize(PointCount);
		Self.Internal.WeightMatrix[k].IndexList() = AdjPointIndexList;
		Self.Internal.WeightMatrix[k].ElementList() = WeightList;
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeWeightMatrix_Type_Distance()
{
	auto& Self = *this;
	auto PointCount = Self.Input.SourceMesh.GetPointCount();
	Self.Internal.WeightMatrix.Clear();
	Self.Internal.WeightMatrix.Resize(PointCount);
	// sum_j {Lambda[i][j]} = 1 for a fixed i

	for (int_max k = 0; k < PointCount; ++k)
	{		
		auto Pos_k = Self.Input.SourceMesh.GetPointPosition(k);
		auto AdjPointIndexList = Self.Input.SourceMesh.Point(k).GetAdjacentPointIndexList();
		DenseVector<ScalarType> WeightList;
		WeightList.Resize(AdjPointIndexList.GetLength());
		for (int_max n = 0; n < AdjPointIndexList.GetLength(); ++n)
		{
			auto Pos_n = Self.Input.SourceMesh.GetPointPosition(AdjPointIndexList[n]);
			auto Distance = (Pos_k - Pos_n).L2Norm();
			WeightList[n] = ScalarType(1)/(Distance + 0.000001);
			//WeightList[n] = 1;
		}
		WeightList /= WeightList.Sum();

		Self.Internal.WeightMatrix[k].Resize(PointCount);
		Self.Internal.WeightMatrix[k].IndexList() = AdjPointIndexList;
		Self.Internal.WeightMatrix[k].ElementList() = WeightList;
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeDisplacementField()
{
	auto& Self = *this;

	auto PointCount = Self.Input.SourceMesh.GetPointCount();

	//-------------------------------------------------------------------------------------------//
	std::vector<Eigen::Triplet<ScalarType, int_max>> CoefList;
	Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> Bx(PointCount), By(PointCount), Bz(PointCount);;

	const auto& Alpha = Self.Input.ConfidenceOfDisplacementField;
	const auto& Beta = Self.Input.ConfidenceOfSmoothness;
	const auto& Lambda = Self.Internal.WeightMatrix;

	DenseVector<ScalarType> RowCoef;
	RowCoef.Resize(PointCount);	
	DenseVector<int> PointFlagList;
	PointFlagList.Resize(PointCount);	
	for (int_max i = 0; i < PointCount; ++i)
	{
		RowCoef.Fill(0);
		PointFlagList.Fill(0);
		//(Alpha[i]+Beta[i])*U[i]
		RowCoef[i]=Alpha[i] + Beta[i];
		PointFlagList[i] = 1;

		auto AdjPointIndexList_i = Self.Input.SourceMesh.Point(i).GetAdjacentPointIndexList();
		for (int_max idx_j = 0; idx_j < AdjPointIndexList_i.GetLength(); ++idx_j)
		{
			auto j = AdjPointIndexList_i[idx_j];
			//(-Beta[i]*Lambda[i][j] -Beta[j]* Lambda[j][i])*U[j]
			RowCoef[j] += -Beta[i]*Lambda[i][j] -Beta[j]* Lambda[j][i];
			PointFlagList[j] = 1;
			
			auto AdjPointIndexList_j = Self.Input.SourceMesh.Point(j).GetAdjacentPointIndexList();
			for (int_max idx_k = 0; idx_k < AdjPointIndexList_j.GetLength(); ++idx_k)
			{
				auto k = AdjPointIndexList_j[idx_k];
				//(Beta[j]*Lambda[j][i]*Lambda[j][k])*U[k]
				RowCoef[k] += Beta[j]*Lambda[j][i]*Lambda[j][k];
				PointFlagList[k] = 1;
			}
		}

		for (int_max m = 0; m < PointCount; ++m)
		{
			if (PointFlagList[m] == 1)
			{
				Eigen::Triplet<ScalarType, int_max> Coef(i, m, RowCoef[m]);
				CoefList.push_back(Coef);
			}
		}

		Bx[i] = Alpha[i] * Self.InputDisplacementField(0, i);
		By[i] = Alpha[i] * Self.InputDisplacementField(1, i);
		Bz[i] = Alpha[i] * Self.InputDisplacementField(2, i);
	}

	//-------------------------------------------------------------//
	typedef Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> EigenVector;
	typedef Eigen::SparseMatrix<ScalarType, 0, int_max> EigenSparseMatrix;
	EigenSparseMatrix A(PointCount, PointCount);
	A.setFromTriplets(CoefList.begin(), CoefList.end());
	//solve
	Eigen::SparseLU<EigenSparseMatrix> solver;
	solver.analyzePattern(A);
	solver.factorize(A);
	EigenVector Ux = solver.solve(Bx);//assume no internal state in this solve function
	EigenVector Uy = solver.solve(By);//
	EigenVector Uz = solver.solve(Bz);//
	//-------------------------------------------------------------------//
	Self.Output.DisplacementField.Clear();
	Self.Output.DisplacementField.Resize(3, PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		Self.Output.DisplacementField(0, k) = Ux(k);
		Self.Output.DisplacementField(1, k) = Uy(k);
		Self.Output.DisplacementField(2, k) = Uz(k);
	}
}


}//namespace mdk