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
	m_Dictioanry = nullptr;
	m_Parameter.NeighbourCount = 5;
	m_Parameter.SimilarityThreshold = 0;
	m_Parameter.TransformName = "SimilarityTransform";
	MaxThreadCount = 1;
}


template<typename ScalarType>
bool KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::CheckInput()
{
    if (m_Parameter.NeighbourCount <= 0)
    {
        MDK_Error("NeighbourCount <= 0  @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.TransformName != "SimilarityTransform" && m_Parameter.TransformName != "RigidTransform")
    {
		MDK_Error("TransformName is NOT SimilarityTransform or RigidTransform @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
    }

	if (m_Parameter.SimilarityThreshold > 1)
	{
		MDK_Error("SimilarityThreshold > 1 @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_Parameter.MaxThreadCount <= 0)
	{
		m_Parameter.MaxThreadCount = 1;
	}

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

	if (m_Dictioanry == nullptr)
	{
		MDK_Error("input Dictioanry is nullptr @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_Dictioanry->IsEmpty() == true)
	{
		MDK_Error("input Dictioanry is empty @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

    return true;
}


template<typename ScalarType>
void KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::Update()
{
	if (this->CheckInput() = false)
	{
		return;
	}

	m_Code.FastResize(m_ShapeData->GetLength());
	//for (int_max k = 0; k <= m_ShapeList.GetLength()-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		m_Code[k] = this->EncodeShape(k);
	};
	ParallelForLoop(TempFunction, 0, m_ShapeList.GetLength() - 1, m_Parameter.MaxThreadCount);
}

template<typename ScalarType>
SparseVector<ScalarType> KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::EncodeShape(int_max ShapeIndex)
{
	auto BasisCount = m_Dictioanry->GetLength();
	const auto& Basis = *m_Dictioanry;
    //----------------------------------------------------------------------------------------------------
	DenseMatrix<ScalarType> SimilarityList;
	SimilarityList.Resize(1,:BasisCount);
	for (int_max k = 0; k < BasisCount; ++k)
	{
		SimilarityList[k]= ComputeSimilarityBetweenShapeWithPointCorrespondence(Basis[k], (*m_ShapeList)[ShapeIndex], m_Parameter.TransformName, false);
	}
	auto NeighbourIndexList = FindKNNBySimilarityList(SimilarityList, m_Parameter.NeighbourCount);
	auto NeighbourMembershipList = SimilarityList.GetSubMatrix(ALL, NeighbourIndexList);
	// process NeighbourMembershipList
	auto eps_value = std::numeric_limits<ScalarType>::epsilon();
	auto tempSum = ScalarType(0);
	for (int_max i = 0; i < m_Parameter.NeighbourCount; ++i)
	{
		if (NeighbourMembershipList[i] < m_Parameter.SimilarityThreshold)
		{
			NeighbourMembershipList[i] = 0;
		}
		NeighbourMembershipList[i] += eps_value;
		tempSum += Membership[i];
	}
	for (int_max i = 0; i < m_Parameter.NeighbourCount; ++i)
	{
		NeighbourMembershipList[i] /= tempSum;
	}
	//----------------------------------------------------------------------------------------------------
	SparseVector<ScalarType> Code;
	Code.Construct(NeighbourIndexList, NeighbourMembershipList, BasisCount);
	return Code;
}

}// namespace mdk

