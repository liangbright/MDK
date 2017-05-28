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
	// dot NOT use Clear
	Input.SourceMesh.Recreate();
	Input.DisplacementField.Recreate();
	Input.ConfidenceOfDisplacementField.Recreate();
	Input.ConfidenceOfSmoothness.Recreate();
	Input.WeigthType = WeightTypeEnum::Distance;

	Internal.WeightMatrix.Clear();

	Output.DisplacementField.Clear();
	Output.DeformedMesh.Clear();
}

template<typename MeshAttribute>
bool DisplacementBasedPolygonMeshDeformer<MeshAttribute>::CheckInput()
{
	if (Input.SourceMesh.IsEmpty() == true)
	{
		return false;
	}

	if (Input.SourceMesh.Check_If_DataStructure_is_Clean() == false)
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
	
	Output.DeformedMesh.Clear();
	if (Output.DisplacementField.IsEmpty() == false)
	{
		Output.DeformedMesh = Input.SourceMesh;
		auto PointCount = Output.DeformedMesh.GetPointCount();
		for (int_max k = 0; k < PointCount; ++k)
		{
			DenseVector<ScalarType, 3> Displacement;
			Output.DisplacementField.GetCol(k, Displacement);
			auto Pos = Output.DeformedMesh.GetPointPosition(k);
			Pos += Displacement;
			Output.DeformedMesh.SetPointPosition(k, Pos);
		}
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeWeightMatrix()
{
	switch (Input.WeigthType)
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
	auto PointCount = Input.SourceMesh.GetPointCount();
	Internal.WeightMatrix.Clear();
	Internal.WeightMatrix.Resize(PointCount);
	// sum_j {Lambda[i][j]} = 1 for a fixed i

	for (int_max k = 0; k < PointCount; ++k)
	{
		auto Pos_k = Input.SourceMesh.GetPointPosition(k);
		auto AdjPointIndexList = Input.SourceMesh.Point(k).GetAdjacentPointIndexList();
		DenseVector<ScalarType> WeightList;
		WeightList.Resize(AdjPointIndexList.GetLength());		
		WeightList.Fill(ScalarType(1) / AdjPointIndexList.GetLength());
		Internal.WeightMatrix[k].Resize(PointCount);
		Internal.WeightMatrix[k].IndexList() = AdjPointIndexList;
		Internal.WeightMatrix[k].ElementList() = WeightList;
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeWeightMatrix_Type_Distance()
{
	auto PointCount = Input.SourceMesh.GetPointCount();
	Internal.WeightMatrix.Clear();
	Internal.WeightMatrix.Resize(PointCount);
	// sum_j {Lambda[i][j]} = 1 for a fixed i

	for (int_max k = 0; k < PointCount; ++k)
	{		
		auto Pos_k = Input.SourceMesh.GetPointPosition(k);
		auto AdjPointIndexList = Input.SourceMesh.Point(k).GetAdjacentPointIndexList();
		DenseVector<ScalarType> WeightList;
		WeightList.Resize(AdjPointIndexList.GetLength());
		for (int_max n = 0; n < AdjPointIndexList.GetLength(); ++n)
		{
			auto Pos_n = Input.SourceMesh.GetPointPosition(AdjPointIndexList[n]);
			auto Distance = (Pos_k - Pos_n).L2Norm();
			WeightList[n] = ScalarType(1)/(Distance + 0.000001);
			//WeightList[n] = 1;
		}
		WeightList /= WeightList.Sum();

		Internal.WeightMatrix[k].Resize(PointCount);
		Internal.WeightMatrix[k].IndexList() = AdjPointIndexList;
		Internal.WeightMatrix[k].ElementList() = WeightList;
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeDisplacementField()
{
	auto PointCount = Input.SourceMesh.GetPointCount();

	//-------------------------------------------------------------------------------------------//
	std::vector<Eigen::Triplet<ScalarType, int_max>> CoefList;
	Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> Bx(PointCount), By(PointCount), Bz(PointCount);;

	const auto& Alpha = Input.ConfidenceOfDisplacementField;
	const auto& Beta = Input.ConfidenceOfSmoothness;
	const auto& Lambda = Internal.WeightMatrix;

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

		auto AdjPointIndexList_i = Input.SourceMesh.Point(i).GetAdjacentPointIndexList();
		for (int_max idx_j = 0; idx_j < AdjPointIndexList_i.GetLength(); ++idx_j)
		{
			auto j = AdjPointIndexList_i[idx_j];
			//(-Beta[i]*Lambda[i][j] -Beta[j]* Lambda[j][i])*U[j]
			RowCoef[j] += -Beta[i]*Lambda[i][j] -Beta[j]* Lambda[j][i];
			PointFlagList[j] = 1;
			
			auto AdjPointIndexList_j = Input.SourceMesh.Point(j).GetAdjacentPointIndexList();
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

		Bx[i] = Alpha[i] * Input.DisplacementField(0, i);
		By[i] = Alpha[i] * Input.DisplacementField(1, i);
		Bz[i] = Alpha[i] * Input.DisplacementField(2, i);
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
	Output.DisplacementField.Clear();
	Output.DisplacementField.Resize(3, PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		Output.DisplacementField(0, k) = Ux(k);
		Output.DisplacementField(1, k) = Uy(k);
		Output.DisplacementField(2, k) = Uz(k);
	}
}


}//namespace mdk