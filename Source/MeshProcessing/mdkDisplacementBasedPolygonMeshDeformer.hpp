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
	m_InputMesh.Recreate();
	m_InputDisplacementField.Recreate();
	m_ConfidenceOfInputDisplacementField.Recreate();
	m_ConfidenceOfSmoothness.Recreate();
	m_WeigthType = WeightTypeEnum::Distance;

	m_WeightMatrix.Clear();
	m_OutputDisplacementField.Clear();
	m_OutputMesh.Clear();
}

template<typename MeshAttribute>
bool DisplacementBasedPolygonMeshDeformer<MeshAttribute>::CheckInput()
{
	if (m_InputMesh.IsEmpty() == true)
	{
		return false;
	}

	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
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
	
	m_OutputMesh.Clear();
	if (m_OutputDisplacementField.IsEmpty() == false)
	{
		m_OutputMesh = m_InputMesh;
		auto PointCount = m_OutputMesh.GetPointCount();
		for (int_max k = 0; k < PointCount; ++k)
		{
			DenseVector<ScalarType, 3> Displacement;
			m_OutputDisplacementField.GetCol(k, Displacement);
			auto Pos = m_OutputMesh.GetPointPosition(k);
			Pos += Displacement;
			m_OutputMesh.SetPointPosition(k, Pos);
		}
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeWeightMatrix()
{
	auto PointCount = m_InputMesh.GetPointCount();
	m_WeightMatrix.Clear();
	m_WeightMatrix.Resize(PointCount);
	// sum_j {Lambda[i][j]} = 1 for a fixed i

	for (int_max k = 0; k < PointCount; ++k)
	{		
		auto AdjPointIndexList = m_InputMesh.Point(k).GetAdjacentPointIndexList();
		DenseVector<ScalarType> WeightList;
		WeightList.Resize(AdjPointIndexList.GetLength());
		auto Pos_k = m_InputMesh.GetPointPosition(k);
		for (int_max n = 0; n < AdjPointIndexList.GetLength(); ++n)
		{
			auto Pos_n = m_InputMesh.GetPointPosition(AdjPointIndexList[n]);
			WeightList[n] = (Pos_k - Pos_n).L2Norm() + 0.0000001;
			//WeightList[n] = 1;
		}
		WeightList /= WeightList.Sum();
		WeightList = ScalarType(1) - WeightList;

		m_WeightMatrix[k].Resize(PointCount);
		m_WeightMatrix[k].IndexList() = AdjPointIndexList;
		m_WeightMatrix[k].ElementList() = WeightList;
	}
}


template<typename MeshAttribute>
void DisplacementBasedPolygonMeshDeformer<MeshAttribute>::ComputeDisplacementField()
{
	auto PointCount = m_InputMesh.GetPointCount();

	//-------------------------------------------------------------------------------------------//
	std::vector<Eigen::Triplet<ScalarType, int_max>> CoefList;
	Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> Bx(PointCount), By(PointCount), Bz(PointCount);;

	const auto& Alpha = m_ConfidenceOfInputDisplacementField;
	const auto& Beta = m_ConfidenceOfSmoothness;
	const auto& Lambda = m_WeightMatrix;

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

		auto AdjPointIndexList_i = m_InputMesh.Point(i).GetAdjacentPointIndexList();
		for (int_max idx_j = 0; idx_j < AdjPointIndexList_i.GetLength(); ++idx_j)
		{
			auto j = AdjPointIndexList_i[idx_j];
			//(-Beta[i]*Lambda[i][j] -Beta[j]* Lambda[j][i])*U[j]
			RowCoef[j] += -Beta[i]*Lambda[i][j] -Beta[j]* Lambda[j][i];
			PointFlagList[j] = 1;
			
			auto AdjPointIndexList_j = m_InputMesh.Point(j).GetAdjacentPointIndexList();
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

		Bx[i] = Alpha[i] * m_InputDisplacementField(0, i);
		By[i] = Alpha[i] * m_InputDisplacementField(1, i);
		Bz[i] = Alpha[i] * m_InputDisplacementField(2, i);
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
	m_OutputDisplacementField.Clear();
	m_OutputDisplacementField.Resize(3, PointCount);
	for (int_max k = 0; k < PointCount; ++k)
	{
		m_OutputDisplacementField(0, k) = Ux(k);
		m_OutputDisplacementField(1, k) = Uy(k);
		m_OutputDisplacementField(2, k) = Uz(k);
	}
}


}//namespace mdk