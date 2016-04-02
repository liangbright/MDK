#pragma once

namespace mdk
{

template<typename ScalarType>
KNNSimilaritySparseShapeEncoder<ScalarType>::KNNSimilaritySparseShapeEncoder()
{
	this->Clear();
}


template<typename ScalarType>
KNNSimilaritySparseShapeEncoder<ScalarType>::~KNNSimilaritySparseShapeEncoder()
{
}


template<typename ScalarType>
void KNNSimilaritySparseShapeEncoder<ScalarType>::Clear()
{
	m_ShapeData = nullptr;
	m_Dictioanry = nullptr;
	m_Parameter.NeighbourCount = 5;
	m_Parameter.TransformName = "SimilarityTransform";
	MaxThreadCount = 1;
}


template<typename ScalarType>
bool KNNSimilaritySparseShapeEncoder<ScalarType>::CheckInput()
{
    if (m_Parameter.NeighbourCount <= 0)
    {
        MDK_Error("NeighbourCount <= 0  @ KNNSimilaritySparseShapeEncoder::CheckInput()")
        return false;
    }

    if (m_Parameter.TransformName != "SimilarityTransform" && m_Parameter.TransformName != "RigidTransform")
    {
		MDK_Error("TransformName is NOT SimilarityTransform or RigidTransform @ KNNSimilaritySparseShapeEncoder::CheckInput()")
		return false;
    }

	if (m_Parameter.MaxThreadCount <= 0)
	{
		m_Parameter.MaxThreadCount = 1;
	}

	if (m_ShapeData == nullptr)
	{
		MDK_Error("input ShapeData is nullptr @ KNNSimilaritySparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_ShapeData->IsEmpty() == true)
	{
		MDK_Error("input ShapeData is empty @ KNNSimilaritySparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_Dictioanry == nullptr)
	{
		MDK_Error("input Dictioanry is nullptr @ KNNSimilaritySparseShapeEncoder::CheckInput()")
		return false;
	}

	if (m_Dictioanry->IsEmpty() == true)
	{
		MDK_Error("input Dictioanry is empty @ KNNSimilaritySparseShapeEncoder::CheckInput()")
		return false;
	}

    return true;
}


template<typename ScalarType>
void KNNSimilaritySparseShapeEncoder<ScalarType>::Update()
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
SparseVector<ScalarType> KNNSimilaritySparseShapeEncoder<ScalarType>::EncodeShape(int_max ShapeIndex)
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
	auto NeighbourSimilarityList = SimilarityList.GetSubMatrix(ALL, NeighbourIndexList);
	//----------------------------------------------------------------------------------------------------
	SparseVector<ScalarType> Code;
	Code.Construct(NeighbourIndexList, NeighbourSimilarityList, BasisCount);
	return Code;
}

}// namespace mdk

