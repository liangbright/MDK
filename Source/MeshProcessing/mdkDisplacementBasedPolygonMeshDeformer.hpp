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
	Self.InputMesh.Recreate();
	Self.InputDisplacementField.Recreate();
	Self.ConfidenceOfInputDisplacementField.Recreate();
	Self.ConfidenceOfSmoothness.Recreate();
	Self.WeigthType = WeightTypeEnum::Distance;

	Self.WeightMatrix.Clear();
	Self.OutputDisplacementField.Clear();
	Self.OutputMesh.Clear();
}

template<typename MeshAttribute>
bool DisplacementBasedPolygonMeshDeformer<MeshAttribute>::CheckInput()
{
	auto& Self = *this;
	if (Self.InputMesh.IsEmpty() == true)
	{
		return false;
	}

	if (Self.InputMesh.Check_If_DataStructure_is_Clean() == false)
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
	Self.OutputMesh.Clear();
	if (Self.OutputDisplacementField.IsEmpty() == false)
	{
		Self.OutputMesh = Self.InputMesh;
		auto PointCount = Self.OutputMesh.GetPointCount();
		for (int_max k = 0; k < PointCount; ++k)
		{
			DenseVector<ScalarType, 3> Displacement;
			Self.OutputDisplacementField.GetCol(k, Displacement);
			auto Pos = Self.OutputMesh.GetPointPosition(k);
			Pos += Displacement;
			Self.OutputMesh.SetPointPosition(k, Pos);
		}
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeWeightMatrix()
{
	auto& Self = *this;
	switch (Self.WeigthType)
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
	auto PointCount = Self.InputMesh.GetPointCount();
	Self.WeightMatrix.Clear();
	Self.WeightMatrix.Resize(PointCount);
	// sum_j {Lambda[i][j]} = 1 for a fixed i

	for (int_max k = 0; k < PointCount; ++k)
	{
		auto Pos_k = Self.InputMesh.GetPointPosition(k);
		auto AdjPointIndexList = Self.InputMesh.Point(k).GetAdjacentPointIndexList();
		DenseVector<ScalarType> WeightList;
		WeightList.Resize(AdjPointIndexList.GetLength());		
		WeightList.Fill(ScalarType(1) / AdjPointIndexList.GetLength());
		Self.WeightMatrix[k].Resize(PointCount);
		Self.WeightMatrix[k].IndexList() = AdjPointIndexList;
		Self.WeightMatrix[k].ElementList() = WeightList;
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeWeightMatrix_Type_Distance()
{
	auto& Self = *this;
	auto PointCount = Self.InputMesh.GetPointCount();
	Self.WeightMatrix.Clear();
	Self.WeightMatrix.Resize(PointCount);
	// sum_j {Lambda[i][j]} = 1 for a fixed i

	for (int_max k = 0; k < PointCount; ++k)
	{		
		auto Pos_k = Self.InputMesh.GetPointPosition(k);
		auto AdjPointIndexList = Self.InputMesh.Point(k).GetAdjacentPointIndexList();
		DenseVector<ScalarType> WeightList;
		WeightList.Resize(AdjPointIndexList.GetLength());
		for (int_max n = 0; n < AdjPointIndexList.GetLength(); ++n)
		{
			auto Pos_n = Self.InputMesh.GetPointPosition(AdjPointIndexList[n]);
			auto Distance = (Pos_k - Pos_n).L2Norm();
			WeightList[n] = ScalarType(1)/(Distance + 0.000001);
			//WeightList[n] = 1;
		}
		WeightList /= WeightList.Sum();

		Self.WeightMatrix[k].Resize(PointCount);
		Self.WeightMatrix[k].IndexList() = AdjPointIndexList;
		Self.WeightMatrix[k].ElementList() = WeightList;
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeDisplacementField()
{
	auto& Self = *this;

	auto PointCount = Self.InputMesh.GetPointCount();

	//-------------------------------------------------------------------------------------------//
	std::vector<Eigen::Triplet<ScalarType, int_max>> CoefList;
	Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> Bx(PointCount), By(PointCount), Bz(PointCount);;

	const auto& Alpha = Self.ConfidenceOfInputDisplacementField;
	const auto& Beta = Self.ConfidenceOfSmoothness;
	const auto& Lambda = Self.WeightMatrix;

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

		auto AdjPointIndexList_i = Self.InputMesh.Point(i).GetAdjacentPointIndexList();
		for (int_max idx_j = 0; idx_j < AdjPointIndexList_i.GetLength(); ++idx_j)
		{
			auto j = AdjPointIndexList_i[idx_j];
			//(-Beta[i]*Lambda[i][j] -Beta[j]* Lambda[j][i])*U[j]
			RowCoef[j] += -Beta[i]*Lambda[i][j] -Beta[j]* Lambda[j][i];
			PointFlagList[j] = 1;
			
			auto AdjPointIndexList_j = Self.InputMesh.Point(j).GetAdjacentPointIndexList();
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
	Self.OutputDisplacementField.Clear();
	Self.OutputDisplacementField.Resize(3, PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		Self.OutputDisplacementField(0, k) = Ux(k);
		Self.OutputDisplacementField(1, k) = Uy(k);
		Self.OutputDisplacementField(2, k) = Uz(k);
	}
}


}//namespace mdk