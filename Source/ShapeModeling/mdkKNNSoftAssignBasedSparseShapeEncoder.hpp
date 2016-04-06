#pragma once

namespace mdk
{

template<typename ScalarType>
KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::KNNSoftAssignBasedSparseShapeEncoder()
{
	this->Clear();
}


template<typename ScalarType>
KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::~KNNSoftAssignBasedSparseShapeEncoder()
{
}


template<typename ScalarType>
void KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::Clear()
{
	m_ShapeData = nullptr;
	m_Dictionary = nullptr;
	m_LandmarkOnShape.Clear();
	m_Parameter.MaxNeighbourCount = 5;
	m_Parameter.SimilarityThreshold = 0;
	m_Parameter.TransformName = "RigidTransform";
	m_Parameter.MaxThreadCount = 1;
}


template<typename ScalarType>
bool KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::CheckInput()
{
	if (m_ShapeData == nullptr)
	{
		MDK_Error("input ShapeData is nullptr @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_ShapeData->IsEmpty() == true)
	{
		MDK_Error("input ShapeData is empty @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	int_max Dimension = (*m_ShapeData)[0].GetRowCount();
	if (Dimension != 2 && Dimension != 3)
	{
		MDK_Error("Shape Dimension is NOT 2 or 3 @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_Dictionary == nullptr)
	{
		MDK_Error("input Dictioanry is nullptr @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_Dictionary->IsEmpty() == true)
	{
		MDK_Error("input Dictioanry is empty @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

    if (m_Parameter.MaxNeighbourCount <= 0)
    {
        MDK_Error("MaxNeighbourCount <= 0  @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.TransformName != "SimilarityTransform" && m_Parameter.TransformName != "RigidTransform" && m_Parameter.TransformName != "ThinPlateSplineTransform")
    {
		MDK_Error("TransformName is unknown @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
    }

	if (m_Parameter.TransformName == "ThinPlateSplineTransform")
	{
		if (m_LandmarkOnShape.GetLength() < 9)
		{
			MDK_Error("too few Landmark for ThinPlateSplineTransform @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
			return false;
		}
	}

	if (m_Parameter.SimilarityThreshold < 0 || m_Parameter.SimilarityThreshold > 1)
	{
		MDK_Error("SimilarityThreshold is out of range @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_Parameter.MaxThreadCount <= 0)
	{
		m_Parameter.MaxThreadCount = 1;
	}

    return true;
}


template<typename ScalarType>
void KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	m_Code.Clear();
	m_Code.Resize(m_ShapeData->GetLength());
	//for (int_max k = 0; k <= m_ShapeData.GetLength()-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		m_Code[k] = this->EncodeShape(k);
	};
	ParallelForLoop(TempFunction, 0, m_ShapeData->GetLength() - 1, m_Parameter.MaxThreadCount);
}


template<typename ScalarType>
SparseVector<ScalarType> KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::EncodeShape(int_max ShapeIndex)
{
	auto BasisCount = m_Dictionary->GetLength();
	const auto& Basis = *m_Dictionary;
    //----------------------------------------------------------------------------------------------------
	DenseMatrix<ScalarType> SimilarityList;
	SimilarityList.Resize(1, BasisCount);
	for (int_max k = 0; k < BasisCount; ++k)
	{
		SimilarityList[k]= this->ComputeShapeSimilarity(Basis[k], (*m_ShapeData)[ShapeIndex], m_LandmarkOnShape, m_Parameter.TransformName, false);
	}
	auto NeighbourIndexList = FindKNNBySimilarityList(SimilarityList, m_Parameter.MaxNeighbourCount);
	auto NeighbourMembershipList = SimilarityList.GetSubMatrix(ALL, NeighbourIndexList);
	// process NeighbourMembershipList
	auto eps_value = std::numeric_limits<ScalarType>::epsilon();
	auto tempSum = ScalarType(0);
	for (int_max i = 0; i < NeighbourIndexList.GetElementCount(); ++i)
	{
		if (NeighbourMembershipList[i] < m_Parameter.SimilarityThreshold)
		{
			NeighbourMembershipList[i] = 0;
		}
		NeighbourMembershipList[i] += eps_value;
		tempSum += NeighbourMembershipList[i];
	}
	for (int_max i = 0; i < NeighbourIndexList.GetElementCount(); ++i)
	{
		NeighbourMembershipList[i] /= tempSum;
	}
	//----------------------------------------------------------------------------------------------------
	SparseVector<ScalarType> Code;
	Code.Initialize(NeighbourIndexList, NeighbourMembershipList, BasisCount);
	return Code;
}

template<typename ScalarType>
ScalarType KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::ComputeShapeSimilarity(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark, const String& TransformName, bool Flag_Symmetry)
{
	return ComputeSimilarityBetweenShapeWithPointCorrespondence(ShapeA, ShapeB, Landmark, TransformName, Flag_Symmetry);
}

}// namespace mdk

