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
	Input.ShapeData = nullptr;
	Input.Dictionary = nullptr;
	Input.LandmarkOnShape.Clear();
	Input.Parameter.MaxNeighborCount = 5;
	Input.Parameter.SimilarityThreshold = 0;
	Input.Parameter.TransformName = "RigidTransform";
	Input.Parameter.MaxThreadCount = 1;
	Output.MembershipCode.Clear();
	Output.SimilarityCode.Clear();
}


template<typename ScalarType>
bool KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::CheckInput()
{
	if (Input.ShapeData == nullptr)
	{
		MDK_Error("input ShapeData is nullptr @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (Input.ShapeData->IsEmpty() == true)
	{
		MDK_Error("input ShapeData is empty @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	int_max Dimension = (*Input.ShapeData)[0].GetRowCount();
	if (Dimension != 2 && Dimension != 3)
	{
		MDK_Error("Shape Dimension is NOT 2 or 3 @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (Input.Dictionary == nullptr)
	{
		MDK_Error("input Dictioanry is nullptr @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (Input.Dictionary->IsEmpty() == true)
	{
		MDK_Error("input Dictioanry is empty @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

    if (Input.Parameter.MaxNeighborCount <= 0)
    {
        MDK_Error("MaxNeighborCount <= 0  @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
        return false;
    }

    if (Input.Parameter.TransformName != "SimilarityTransform" && Input.Parameter.TransformName != "RigidTransform" && Input.Parameter.TransformName != "ThinPlateSplineTransform")
    {
		MDK_Error("TransformName is unknown @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
    }

	if (Input.Parameter.TransformName == "ThinPlateSplineTransform")
	{
		if (Input.LandmarkOnShape.GetLength() < 9)
		{
			MDK_Error("too few Landmark for ThinPlateSplineTransform @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
			return false;
		}
	}

	if (Input.Parameter.SimilarityThreshold < 0 || Input.Parameter.SimilarityThreshold > 1)
	{
		MDK_Error("SimilarityThreshold is out of range @ KNNSoftAssignBasedSparseShapeEncoder::CheckInput()")
		return false;
	}

	if (Input.Parameter.MaxThreadCount <= 0)
	{
		Input.Parameter.MaxThreadCount = 1;
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
	Output.MembershipCode.Clear();
	Output.MembershipCode.Resize(Input.ShapeData->GetLength());
	Output.SimilarityCode.Clear();
	Output.SimilarityCode.Resize(Input.ShapeData->GetLength());
	//for (int_max k = 0; k <= Input.ShapeData.GetLength()-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		this->EncodeShape(k, Output.MembershipCode[k], Output.SimilarityCode[k]);
	};
	ParallelForLoop(TempFunction, 0, Input.ShapeData->GetLength() - 1, Input.Parameter.MaxThreadCount);
}


template<typename ScalarType>
void KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::EncodeShape(int_max ShapeIndex, SparseVector<ScalarType>& MemebershipCode, SparseVector<ScalarType>& SimilarityCode)
{
	auto BasisCount = Input.Dictionary->GetLength();
	const auto& Basis = *Input.Dictionary;
    //----------------------------------------------------------------------------------------------------
	DenseMatrix<ScalarType> SimilarityList;
	SimilarityList.Resize(1, BasisCount);
	for (int_max k = 0; k < BasisCount; ++k)
	{
		SimilarityList[k]= this->ComputeShapeSimilarity(Basis[k], (*Input.ShapeData)[ShapeIndex]);
	}
	auto NeighborIndexList = FindKNNBySimilarityList(SimilarityList, Input.Parameter.MaxNeighborCount);
	auto NeighborSimilarityList = SimilarityList.GetSubMatrix(ALL, NeighborIndexList);
	auto NeighborMembershipList = NeighborSimilarityList;
	// process NeighborMembershipList
	auto eps_value = std::numeric_limits<ScalarType>::epsilon();
	auto tempSum = ScalarType(0);
	for (int_max i = 0; i < NeighborIndexList.GetElementCount(); ++i)
	{
		if (NeighborMembershipList[i] < Input.Parameter.SimilarityThreshold)
		{
			NeighborMembershipList[i] = 0;
		}
		NeighborMembershipList[i] += eps_value;
		tempSum += NeighborMembershipList[i];
	}
	for (int_max i = 0; i < NeighborIndexList.GetElementCount(); ++i)
	{
		NeighborMembershipList[i] /= tempSum;
	}
	//----------------------------------------------------------------------------------------------------	
	MemebershipCode.Initialize(NeighborIndexList, NeighborMembershipList, BasisCount);		
	SimilarityCode.Initialize(NeighborIndexList, NeighborSimilarityList, BasisCount);
}

template<typename ScalarType>
ScalarType KNNSoftAssignBasedSparseShapeEncoder<ScalarType>::ComputeShapeSimilarity(const DenseMatrix<ScalarType>& Basis, const DenseMatrix<ScalarType>& Data)
{
	return ComputeSimilarityBetweenShapeWithPointCorrespondence(Basis, Data, Input.LandmarkOnShape, Input.Parameter.TransformName, false);
}

}// namespace mdk

