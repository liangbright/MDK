#pragma once

namespace mdk
{
template<typename ScalarType>
DistanceMinimizationBasedShapeAligner3D<ScalarType>::DistanceMinimizationBasedShapeAligner3D()
{
	this->Clear();
}

template<typename ScalarType>
DistanceMinimizationBasedShapeAligner3D<ScalarType>::~DistanceMinimizationBasedShapeAligner3D()
{
}

template<typename ScalarType>
void DistanceMinimizationBasedShapeAligner3D<ScalarType>::Clear()
{
	m_InputShapeList = nullptr;
	m_InputSimilarityTable = nullptr;
	m_Flag_use_SimilarityTransform = false;
	m_NeighbourCount = 5;

	m_OutputTransformList.Recreate();
	m_OutputShapeList.Recreate();
}

template<typename ScalarType>
bool DistanceMinimizationBasedShapeAligner3D<ScalarType>::CheckInput()
{
	if (m_InputShapeList == nullptr)
	{
		MDK_Error("InputShapeList is nullptr @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_InputShapeList.IsEmpty() == true)
	{
		MDK_Error("InputShapeList is empty @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_InputSimilarityTable == nullptr)
	{
		MDK_Error("InputSimilarityTable is nullptr @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_InputSimilarityTable.IsEmpty() == true)
	{
		MDK_Error("InputSimilarityTable is empty @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_NeighbourCount <= 0 || m_NeighbourCount > m_InputShapeList.GetLength())
	{
		MDK_Error("NeighbourCount is out of range @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	return true;
}

}//namespace mdk