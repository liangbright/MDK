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
	m_MembershipCode.Clear();
	m_SimilarityCode.Clear();
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
	m_MembershipCode.Clear();
	m_MembershipCode.Resize(m_ShapeData->GetLength());
	m_SimilarityCode.Clear();
	m_SimilarityCode.Resize(m_ShapeData->GetLength());
	//for (int_max k = 0; k <= m_ShapeData.GetLength()-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		this->EncodeShape(k, m_MembershipCode[k], m_SimilarityCode[k]);
	};
	ParallelForLoop(TempFunction, 0, m_ShapeData->GetLength() - 1, m_Parameter.MaxThreadCount);
}


template<typename ScalarType>
void KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::EncodeShape(int_max ShapeIndex, SparseVector<ScalarType>& MemebershipCode, SparseVector<ScalarType>& SimilarityCode)
{
	auto BasisCount = m_Dictionary->GetLength();
	const auto& Basis = *m_Dictionary;
    //----------------------------------------------------------------------------------------------------
	DenseMatrix<ScalarType> SimilarityList;
	SimilarityList.Resize(1, BasisCount);
	for (int_max k = 0; k < BasisCount; ++k)
	{
		SimilarityList[k]= this->ComputeShapeSimilarity(Basis[k], (*m_ShapeData)[ShapeIndex]);
	}
	auto NeighbourIndexList = FindKNNBySimilarityList(SimilarityList, m_Parameter.MaxNeighbourCount);
	auto NeighbourSimilarityList = SimilarityList.GetSubMatrix(ALL, NeighbourIndexList);
	auto NeighbourMembershipList = NeighbourSimilarityList;
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
	MemebershipCode.Initialize(NeighbourIndexList, NeighbourMembershipList, BasisCount);		
	SimilarityCode.Initialize(NeighbourIndexList, NeighbourSimilarityList, BasisCount);
}

template<typename ScalarType>
ScalarType KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::ComputeShapeSimilarity(const DenseMatrix<ScalarType>& Basis, const DenseMatrix<ScalarType>& Data)
{
	return ComputeSimilarityBetweenShapeWithPointCorrespondence(Basis, Data, m_LandmarkOnShape, m_Parameter.TransformName, false);
}

}// namespace mdk

