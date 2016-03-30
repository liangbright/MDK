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
	m_MaxNeighbourCount = 5;
	m_MaxIterCount = 10;
	m_InputReferenceShapeIndex = 0;
	m_Flag_ComputeObjectiveFunctionValue = false;
	m_MaxThreadCount = 1;
	m_Flag_Parallel_UpdateTransform = false;
	m_OutputTransformList.Clear();
	if (m_OutputTransformList.IsPureEmpty() == true)
	{
		m_OutputTransformList.Recreate();
	}
	m_OutputShapeList.Clear();
	if (m_OutputShapeList.IsPureEmpty() == true)
	{
		m_OutputShapeList.Recreate();
	}
}

template<typename ScalarType>
bool DistanceMinimizationBasedShapeAligner3D<ScalarType>::CheckInput()
{
	if (m_InputShapeList == nullptr)
	{
		MDK_Error("InputShapeList is nullptr @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_InputShapeList->GetLength() <= 1)
	{
		MDK_Error("InputShapeList is empty or only 1 shape @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	for (int_max k = 0; k < m_InputShapeList->GetLength(); ++k)
	{
		const auto& Shape_k = (*m_InputShapeList)[k];
		if (Shape_k.GetRowCount() != 3)
		{
			MDK_Error("input shape is wrong @ DistanceMinimizationBasedShapeAligner3D::CheckInput(...)")
			return false;
		}
	}

	if (m_InputSimilarityTable == nullptr)
	{
		MDK_Error("InputSimilarityTable is nullptr @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_InputSimilarityTable->IsEmpty() == true)
	{
		MDK_Error("InputSimilarityTable is empty @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_InputSimilarityTable->GetLength() != m_InputShapeList->GetLength())
	{
		MDK_Error("InputSimilarityTable size not the same InputShapeList size @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	//check m_InputSimilarityTable to make sure (*m_InputSimilarityTable)[k] is not empty for all k
	for (int_max k = 0; k < m_InputSimilarityTable->GetLength(); ++k)
	{
		if ((*m_InputSimilarityTable)[k].IsEmpty() == true)
		{
			MDK_Error("InputSimilarityTable[" << std::to_string(k) << "] is empty @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
			return false;
		}
	}

	if (m_MaxNeighbourCount <= 0 || m_MaxNeighbourCount > m_InputShapeList->GetLength())
	{
		MDK_Error("MaxNeighbourCount is out of range @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_MaxIterCount < 0)
	{
		MDK_Error("MaxIterCount is out of range @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_InputReferenceShapeIndex < 0 || m_InputReferenceShapeIndex >= m_InputShapeList->GetLength())
	{
		MDK_Error("InputReferenceShapeIndex is out of range @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	if (m_MaxThreadCount < 0)
	{
		MDK_Error("MaxThreadCount < 0 @ DistanceMinimizationBasedShapeAligner3D::CheckInput()")
		return false;
	}

	return true;
}

template<typename ScalarType>
void DistanceMinimizationBasedShapeAligner3D<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	m_OutputShapeList.Clear();
	m_OutputShapeList = *m_InputShapeList;//copy

	m_OutputTransformList.Clear();
	m_OutputTransformList.Resize(m_OutputShapeList.GetLength());
	for (int_max k = 0; k < m_OutputTransformList.GetLength();++k)
	{
		m_OutputTransformList[k].Scale = 1;		
		m_OutputTransformList[k].Rotation.Resize(3, 3);
		m_OutputTransformList[k].Rotation.Fill(0);
		m_OutputTransformList[k].Rotation.FillDiagonal(1);
		m_OutputTransformList[k].Rotation.FixSize();
		m_OutputTransformList[k].Translation.Resize(3, 1);
		m_OutputTransformList[k].Translation.Fill(0);
	}

	if (m_Flag_ComputeObjectiveFunctionValue == true)
	{
		m_ObjectiveFunctionValue.SetCapacity(100);
		m_ObjectiveFunctionValue.Append(this->ComputeObjectiveFunctionValue());
	}

	this->FindInitialTransform();
	
	if (m_MaxIterCount > 0)
	{
		if (m_MaxThreadCount > 1 && m_Flag_Parallel_UpdateTransform == true)
		{
			this->UpdateTransform_parallel();
		}
		else
		{
			this->UpdateTransform_sequential();
		}
	}

	this->AlignToReferenceShape();
}


template<typename ScalarType>
void DistanceMinimizationBasedShapeAligner3D<ScalarType>::FindInitialTransform()
{
	int_max ShapeCount = m_InputShapeList->GetLength();
	int_max ShapePairCount = ShapeCount*(ShapeCount - 1) / 2;

	//sort shape-pair similarity from the highest to the lowest ----------------------------------//
	DenseMatrix<int_max> ShapePairList;
	ShapePairList.SetCapacity(2, ShapePairCount);
	// pair_n: (shape_i, shape_j)
	//ShapePairList(0,n) : index of shape_i in m_InputShapeList
	//ShapePairList(1,n) : index of shape_j in m_InputShapeList
	{
		DenseVector<ScalarType> SimilarityOfEachPair;		
		SimilarityOfEachPair.SetCapacity(ShapePairCount);
		for (int_max n = 0; n < ShapeCount -1; ++n)
		{
			for (int_max m = n+1; m < ShapeCount; ++m)
			{
				ShapePairList.AppendCol({ n, m });
				SimilarityOfEachPair.Append((*m_InputSimilarityTable)[n][m]);
			}
		}
		auto PairIndexList_sort = SimilarityOfEachPair.Sort("descend");
		
		DenseMatrix<int_max> ShapePairList_sort;
		ShapePairList_sort.Resize(2, ShapePairCount);
		DenseVector<ScalarType> SimilarityOfEachPair_sort;
		SimilarityOfEachPair_sort.Resize(ShapePairCount);
		for (int_max n = 0; n < ShapePairCount; ++n)
		{
			ShapePairList_sort(0, n) = ShapePairList(0, PairIndexList_sort[n]);
			ShapePairList_sort(1, n) = ShapePairList(1, PairIndexList_sort[n]);
			SimilarityOfEachPair_sort[n] = SimilarityOfEachPair[n];
		}
		ShapePairList = std::move(ShapePairList_sort);
		SimilarityOfEachPair= std::move(SimilarityOfEachPair_sort);
	}
	//--------------------------------------------------------------------------------------------//

	ObjectArray<DenseVector<int_max>> ShapeGroup;
	//ShapeGroup[k]: shape index list of group-k 
	// initialization: each shape is in a different group
	ShapeGroup.Resize(ShapeCount);
	for (int_max n = 0; n < ShapeCount; ++n)
	{
		ShapeGroup[n] = n;
	}

	DenseVector<int_max> GroupIndexOfEachShape;
	//GroupIndexOfEachShape[n]: Group Index of Shape_n, n is Shape Index in m_InputShapeList
	// initialization: each shape is in a different group
	GroupIndexOfEachShape.Resize(ShapeCount);
	for (int_max n = 0; n < ShapeCount; ++n)
	{
		GroupIndexOfEachShape[n] = n;
	}

	for (int_max n = 0; n < ShapePairCount; ++n)
	{
		auto ShapeIndex_a = ShapePairList(0, n);	
		auto ShapeIndex_b = ShapePairList(1, n);

		auto GroupIndex_a = GroupIndexOfEachShape[ShapeIndex_a];
		auto ShapeCount_in_Group_a = ShapeGroup[GroupIndex_a].GetLength();
	
		auto GroupIndex_b = GroupIndexOfEachShape[ShapeIndex_b];
		auto ShapeCount_in_Group_b = ShapeGroup[GroupIndex_b].GetLength();

		if (GroupIndex_a != GroupIndex_b)//Not in the same group -> not aligned
		{
			int_max ShapeIndex_ref = -1;
			int_max ShapeIndex_moving = -1;
			int_max GroupIndex_ref = -1;
			int_max GroupIndex_moving = -1;
			DenseVector<int_max> Group_ref, Group_moving;
			// transform Shape in Group_moving to Group_ref

			if (ShapeCount_in_Group_a >= ShapeCount_in_Group_b)
			{// transform all the shape in group-b to group-a
				ShapeIndex_ref = ShapeIndex_a;
				ShapeIndex_moving = ShapeIndex_b;
				GroupIndex_ref = GroupIndex_a;
				GroupIndex_moving = GroupIndex_b;
				Group_ref = ShapeGroup[GroupIndex_a];
				Group_moving = ShapeGroup[GroupIndex_b];
			}
			else
			{// transform all the shape in group-a to group-b
				ShapeIndex_ref = ShapeIndex_b;
				ShapeIndex_moving = ShapeIndex_a;
				GroupIndex_ref = GroupIndex_b;
				GroupIndex_moving = GroupIndex_a;
				Group_ref = ShapeGroup[GroupIndex_b];
				Group_moving = ShapeGroup[GroupIndex_a];
			}

			// Estimate transform from Shape_moving (X) to Shape_ref (Y):  Y ~ R*s*X+t
			// transfrom every shape in Group_moving (ShapeIndex_moving)
			DenseMatrix<ScalarType> Rotation;
			ScalarType Scale = 1;
			DenseMatrix<ScalarType> Translation;
			if (m_Flag_use_SimilarityTransform == true)
			{
				SimilarityTransform3D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&m_OutputShapeList[ShapeIndex_moving]);
				Transform.SetTargetLandmarkPointSet(&m_OutputShapeList[ShapeIndex_ref]);
				Transform.EstimateParameter();
				Rotation = Transform.GetRotationMatrix();
				Scale = Transform.GetScale();
				Translation = Transform.GetTranslation_After_Scale_Rotation();
				for (int_max k = 0; k < Group_moving.GetLength(); ++k)
				{
					auto& Shape_k = m_OutputShapeList[Group_moving[k]];
					Shape_k = Transform.TransformPoint(Shape_k);
				}
			}
			else
			{
				RigidTransform3D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&m_OutputShapeList[ShapeIndex_moving]);
				Transform.SetTargetLandmarkPointSet(&m_OutputShapeList[ShapeIndex_ref]);
				Transform.EstimateParameter();
				Rotation = Transform.GetRotationMatrix();
				Scale = 1;
				Translation = Transform.GetTranslation_AfterRotation();
				for (int_max k = 0; k < Group_moving.GetLength(); ++k)
				{
					auto& Shape_k = m_OutputShapeList[Group_moving[k]];
					Shape_k = Transform.TransformPoint(Shape_k);
				}
			}

			//transform shape in Group_moving to Group_ref
			//update GroupIndexOfEachShape
			for (int_max k = 0; k < Group_moving.GetLength(); ++k)
			{
				GroupIndexOfEachShape[Group_moving[k]] = GroupIndex_ref;
			}
			// update m_OutputTransformList
			for (int_max k = 0; k < Group_moving.GetLength(); ++k)
			{
				m_OutputTransformList[Group_moving[k]].Scale *= Scale;
				m_OutputTransformList[Group_moving[k]].Rotation *= Rotation;
				auto Translation_old = m_OutputTransformList[Group_moving[k]].Translation;
				m_OutputTransformList[Group_moving[k]].Translation = Scale*Rotation*Translation_old + Translation;
			}
			//update ShapeGroup
			ShapeGroup[GroupIndex_ref].Append(ShapeGroup[GroupIndex_moving]);
			ShapeGroup[GroupIndex_moving].Clear();
		}
	}

	if (m_Flag_ComputeObjectiveFunctionValue == true)
	{
		m_ObjectiveFunctionValue.Append(this->ComputeObjectiveFunctionValue());
	}
}


template<typename ScalarType>
void DistanceMinimizationBasedShapeAligner3D<ScalarType>::UpdateTransform_sequential()
{
	int_max ShapeCount = m_OutputShapeList.GetLength();
	int_max PointCountPerShape = m_OutputShapeList[0].GetColCount();

	for (int_max Iter = 0; Iter < m_MaxIterCount; ++Iter)
	{
		for (int_max k = 0; k < ShapeCount; ++k)
		{
			auto NeighbourShapeIndexList = (*m_InputSimilarityTable)[k].IndexList();
			auto NeighbourSimilarityList = (*m_InputSimilarityTable)[k].ElementList();	

			// exclude self in NeighbourShapeIndexList
			/*
			{
				auto tempIndex = NeighbourShapeIndexList.ExactMatch("first", k);
				if (tempIndex >= 0)
				{
					NeighbourShapeIndexList.Delete(tempIndex);
					NeighbourSimilarityList.Delete(tempIndex);
				}				
			}
			if (NeighbourShapeIndexList.GetLength() > m_MaxNeighbourCount)
			{
				auto RelativeIndexList_sort = NeighbourSimilarityList.Sort("descend");
				RelativeIndexList_sort = RelativeIndexList_sort.GetSubSet(0, m_MaxNeighbourCount - 1);
				NeighbourShapeIndexList = NeighbourShapeIndexList.GetSubSet(RelativeIndexList_sort);
				NeighbourSimilarityList = NeighbourSimilarityList.GetSubSet(RelativeIndexList_sort);
			}
			*/

			// include self in NeighbourShapeIndexList			
			{
				auto tempIndex = NeighbourShapeIndexList.ExactMatch("first", k);
				if (tempIndex < 0)
				{
					NeighbourShapeIndexList.Append(k);
					NeighbourSimilarityList.Append(1);
				}
			}
			if (NeighbourShapeIndexList.GetLength() > m_MaxNeighbourCount + 1)
			{
				auto RelativeIndexList_sort = NeighbourSimilarityList.Sort("descend");
				RelativeIndexList_sort = RelativeIndexList_sort.GetSubSet(0, m_MaxNeighbourCount);
				NeighbourShapeIndexList = NeighbourShapeIndexList.GetSubSet(RelativeIndexList_sort);
				NeighbourSimilarityList = NeighbourSimilarityList.GetSubSet(RelativeIndexList_sort);
			}
			
			//normalize NeighbourSimilarityList
			auto tempSum = NeighbourSimilarityList.Sum();
			auto EPS = std::numeric_limits<ScalarType>::epsilon();
			if (tempSum <= m_MaxNeighbourCount*EPS)
			{
				NeighbourSimilarityList.Fill(1);				
			}
			NeighbourSimilarityList /= ScalarType(NeighbourSimilarityList.GetLength());

			//get mean shape of neighbour
			DenseMatrix<ScalarType> MeanShape;
			MeanShape.Resize(3, PointCountPerShape);
			MeanShape.Fill(0);
			for (int_max n = 0; n < NeighbourShapeIndexList.GetLength(); ++n)
			{
				const auto& Shape_n = m_OutputShapeList[NeighbourShapeIndexList[n]];
				for (int_max m = 0; m < 3*PointCountPerShape; ++m)
				{
					MeanShape[m] += NeighbourSimilarityList[n] * Shape_n[m];
				}
			}

			//transfrom from Shape_k (X) to MeanShape (Y): :  Y ~ R*s*X+t
			auto& Shape_k = m_OutputShapeList[k];
			DenseMatrix<ScalarType> Rotation;
			ScalarType Scale = 1;
			DenseMatrix<ScalarType> Translation;
			if (m_Flag_use_SimilarityTransform == true)
			{
				SimilarityTransform3D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&Shape_k);
				Transform.SetTargetLandmarkPointSet(&MeanShape);
				Transform.EstimateParameter();
				Rotation = Transform.GetRotationMatrix();
				Scale = Transform.GetScale();
				Translation = Transform.GetTranslation_After_Scale_Rotation();
				Shape_k = Transform.TransformPoint(Shape_k);
			}
			else
			{
				RigidTransform3D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&Shape_k);
				Transform.SetTargetLandmarkPointSet(&MeanShape);
				Transform.EstimateParameter();
				Rotation = Transform.GetRotationMatrix();
				Scale = 1;
				Translation = Transform.GetTranslation_AfterRotation();
				Shape_k = Transform.TransformPoint(Shape_k);
			}

			// update m_OutputTransformList
			m_OutputTransformList[k].Scale *= Scale;
			m_OutputTransformList[k].Rotation *= Rotation;
			auto Translation_old = m_OutputTransformList[k].Translation;
			m_OutputTransformList[k].Translation = Scale*Rotation*Translation_old + Translation;
		}

		if (m_Flag_ComputeObjectiveFunctionValue == true)
		{
			m_ObjectiveFunctionValue.Append(this->ComputeObjectiveFunctionValue());
		}
	}
}


template<typename ScalarType>
void DistanceMinimizationBasedShapeAligner3D<ScalarType>::UpdateTransform_parallel()
{
	int_max ShapeCount = m_OutputShapeList.GetLength();
	int_max PointCountPerShape = m_OutputShapeList[0].GetColCount();

	//copy data
	auto OutputShapeList_updated = m_OutputShapeList;

	for (int_max Iter = 0; Iter < m_MaxIterCount; ++Iter)
	{		
		//for (int_max k = 0; k <= ShapeCount-1; ++k)
		auto TempFunction = [&](int_max k)
		{
			auto NeighbourShapeIndexList = (*m_InputSimilarityTable)[k].IndexList();
			auto NeighbourSimilarityList = (*m_InputSimilarityTable)[k].ElementList();

			// exclude self in NeighbourShapeIndexList
			/*
			{
			auto tempIndex = NeighbourShapeIndexList.ExactMatch("first", k);
			if (tempIndex >= 0)
			{
			NeighbourShapeIndexList.Delete(tempIndex);
			NeighbourSimilarityList.Delete(tempIndex);
			}
			}
			if (NeighbourShapeIndexList.GetLength() > m_MaxNeighbourCount)
			{
			auto RelativeIndexList_sort = NeighbourSimilarityList.Sort("descend");
			RelativeIndexList_sort = RelativeIndexList_sort.GetSubSet(0, m_MaxNeighbourCount - 1);
			NeighbourShapeIndexList = NeighbourShapeIndexList.GetSubSet(RelativeIndexList_sort);
			NeighbourSimilarityList = NeighbourSimilarityList.GetSubSet(RelativeIndexList_sort);
			}
			*/

			// include self in NeighbourShapeIndexList			
			{
				auto tempIndex = NeighbourShapeIndexList.ExactMatch("first", k);
				if (tempIndex < 0)
				{
					NeighbourShapeIndexList.Append(k);
					NeighbourSimilarityList.Append(1);
				}
			}
			if (NeighbourShapeIndexList.GetLength() > m_MaxNeighbourCount + 1)
			{
				auto RelativeIndexList_sort = NeighbourSimilarityList.Sort("descend");
				RelativeIndexList_sort = RelativeIndexList_sort.GetSubSet(0, m_MaxNeighbourCount);
				NeighbourShapeIndexList = NeighbourShapeIndexList.GetSubSet(RelativeIndexList_sort);
				NeighbourSimilarityList = NeighbourSimilarityList.GetSubSet(RelativeIndexList_sort);
			}

			//normalize NeighbourSimilarityList
			auto tempSum = NeighbourSimilarityList.Sum();
			auto EPS = std::numeric_limits<ScalarType>::epsilon();
			if (tempSum <= m_MaxNeighbourCount*EPS)
			{
				NeighbourSimilarityList.Fill(1);
			}
			NeighbourSimilarityList /= ScalarType(NeighbourSimilarityList.GetLength());

			//get mean shape of neighbour
			DenseMatrix<ScalarType> MeanShape;
			MeanShape.Resize(3, PointCountPerShape);
			MeanShape.Fill(0);
			for (int_max n = 0; n < NeighbourShapeIndexList.GetLength(); ++n)
			{
				const auto& Shape_n = m_OutputShapeList[NeighbourShapeIndexList[n]];
				for (int_max m = 0; m < 3 * PointCountPerShape; ++m)
				{
					MeanShape[m] += NeighbourSimilarityList[n] * Shape_n[m];
				}
			}

			//transfrom from Shape_k (X) to MeanShape (Y): :  Y ~ R*s*X+t
			auto& Shape_k = OutputShapeList_updated[k];
			DenseMatrix<ScalarType> Rotation;
			ScalarType Scale = 1;
			DenseMatrix<ScalarType> Translation;
			if (m_Flag_use_SimilarityTransform == true)
			{
				SimilarityTransform3D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&Shape_k);
				Transform.SetTargetLandmarkPointSet(&MeanShape);
				Transform.EstimateParameter();
				Rotation = Transform.GetRotationMatrix();
				Scale = Transform.GetScale();
				Translation = Transform.GetTranslation_After_Scale_Rotation();
				Shape_k = Transform.TransformPoint(Shape_k);
			}
			else
			{
				RigidTransform3D<ScalarType> Transform;
				Transform.SetSourceLandmarkPointSet(&Shape_k);
				Transform.SetTargetLandmarkPointSet(&MeanShape);
				Transform.EstimateParameter();
				Rotation = Transform.GetRotationMatrix();
				Scale = 1;
				Translation = Transform.GetTranslation_AfterRotation();
				Shape_k = Transform.TransformPoint(Shape_k);
			}

			// update m_OutputTransformList
			m_OutputTransformList[k].Scale *= Scale;
			m_OutputTransformList[k].Rotation *= Rotation;
			auto Translation_old = m_OutputTransformList[k].Translation;
			m_OutputTransformList[k].Translation = Scale*Rotation*Translation_old + Translation;
		};
		ParallelForLoop(TempFunction, 0, ShapeCount - 1, m_MaxThreadCount);

		m_OutputShapeList = OutputShapeList_updated;

		if (m_Flag_ComputeObjectiveFunctionValue == true)
		{
			m_ObjectiveFunctionValue.Append(this->ComputeObjectiveFunctionValue());
		}		
	}
}


template<typename ScalarType>
void DistanceMinimizationBasedShapeAligner3D<ScalarType>::AlignToReferenceShape()
{
	const auto& Shape_input = (*m_InputShapeList)[m_InputReferenceShapeIndex];
	const auto& Shape_output = m_OutputShapeList[m_InputReferenceShapeIndex];
	int_max ShapeCount = m_InputShapeList->GetLength();

	if (m_Flag_use_SimilarityTransform == true)
	{
		SimilarityTransform3D<ScalarType> Transform;		
		Transform.SetSourceLandmarkPointSet(&Shape_output);
		Transform.SetTargetLandmarkPointSet(&Shape_input);
		Transform.EstimateParameter();
		auto Rotation = Transform.GetRotationMatrix();
		auto Scale = Transform.GetScale();
		auto Translation = Transform.GetTranslation_After_Scale_Rotation();
				
		//for (int_max k = 0; k <= ShapeCount-1; ++k)
		auto TempFunction = [&](int_max k)
		{
			//transfrom from Shape_k (X) to Shape_ref (Y): :  Y ~ R*s*X+t
			auto& Shape_k = m_OutputShapeList[k];
			Shape_k = Transform.TransformPoint(Shape_k);
			// update m_OutputTransformList
			m_OutputTransformList[k].Scale *= Scale;
			m_OutputTransformList[k].Rotation *= Rotation;
			auto Translation_old = m_OutputTransformList[k].Translation;
			m_OutputTransformList[k].Translation = Scale*Rotation*Translation_old + Translation;
		};
		ParallelForLoop(TempFunction, 0, ShapeCount - 1, m_MaxThreadCount);
	}
	else
	{
		RigidTransform3D<ScalarType> Transform;
		Transform.SetSourceLandmarkPointSet(&Shape_output);
		Transform.SetTargetLandmarkPointSet(&Shape_input);
		Transform.EstimateParameter();
		auto Rotation = Transform.GetRotationMatrix();		
		auto Translation = Transform.GetTranslation_AfterRotation();
		
		//for (int_max k = 0; k <= ShapeCount-1; ++k)
		auto TempFunction = [&](int_max k)
		{
			//transfrom from Shape_k (X) to Shape_ref (Y): :  Y ~ R*s*X+t
			auto& Shape_k = m_OutputShapeList[k];
			Shape_k = Transform.TransformPoint(Shape_k);
			// update m_OutputTransformList			
			m_OutputTransformList[k].Rotation *= Rotation;
			auto Translation_old = m_OutputTransformList[k].Translation;
			m_OutputTransformList[k].Translation = Rotation*Translation_old + Translation;
		};
		ParallelForLoop(TempFunction, 0, ShapeCount - 1, m_MaxThreadCount);
	}		

	if (m_Flag_ComputeObjectiveFunctionValue == true)
	{
		m_ObjectiveFunctionValue.Append(this->ComputeObjectiveFunctionValue());
	}
}


template<typename ScalarType>
ScalarType DistanceMinimizationBasedShapeAligner3D<ScalarType>::ComputeObjectiveFunctionValue()
{
	ScalarType RMSE = 0;
	ScalarType SimilaritySum = 0;
	int_max ShapeCount = m_OutputShapeList.GetLength();
	for (int_max n = 0; n < ShapeCount - 1; ++n)
	{
		auto NeighbourShapeIndexList = (*m_InputSimilarityTable)[n].IndexList();
		auto NeighbourSimilarityList = (*m_InputSimilarityTable)[n].ElementList();
		// exclude self in NeighbourShapeIndexList		
		{
			auto tempIndex = NeighbourShapeIndexList.ExactMatch("first", n);
			if (tempIndex >= 0)
			{
				NeighbourShapeIndexList.Delete(tempIndex);
				NeighbourSimilarityList.Delete(tempIndex);
			}
		}
		if (NeighbourShapeIndexList.GetLength() > m_MaxNeighbourCount)
		{
			auto RelativeIndexList_sort = NeighbourSimilarityList.Sort("descend");
			RelativeIndexList_sort = RelativeIndexList_sort.GetSubSet(0, m_MaxNeighbourCount - 1);
			NeighbourShapeIndexList = NeighbourShapeIndexList.GetSubSet(RelativeIndexList_sort);
			NeighbourSimilarityList = NeighbourSimilarityList.GetSubSet(RelativeIndexList_sort);
		}

		//normalize NeighbourSimilarityList
		auto tempSum = NeighbourSimilarityList.Sum();
		auto EPS = std::numeric_limits<ScalarType>::epsilon();
		if (tempSum <= m_MaxNeighbourCount*EPS)
		{
			NeighbourSimilarityList.Fill(1);
		}
		NeighbourSimilarityList /= ScalarType(NeighbourSimilarityList.GetLength());
		
		const auto& Shape_n = m_OutputShapeList[n];
		for (int_max m = n + 1; m < ShapeCount; ++m)
		{
			const auto& Shape_m = m_OutputShapeList[m];
			auto Similarity_nm = (*m_InputSimilarityTable)[n][m];
			ScalarType MSE_nm = 0;
			for (int_max k = 0; k < Shape_m.GetElementCount(); ++k)
			{
				MSE_nm += (Shape_n[k] - Shape_m[k])*(Shape_n[k] - Shape_m[k]);
			}
			MSE_nm /= ScalarType(Shape_m.GetColCount());
			RMSE += Similarity_nm*MSE_nm;
			SimilaritySum += Similarity_nm;
		}
	}
	RMSE /= ScalarType(SimilaritySum);
	RMSE = std::sqrt(RMSE);
	return RMSE;
}

//---------------------------------------------------------- public static function ---------------------------------------------------------------------------//

template<typename ScalarType>
ObjectArray<SparseVector<ScalarType>> DistanceMinimizationBasedShapeAligner3D<ScalarType>::ComputeSimilarityBetweenShape(const ObjectArray<DenseMatrix<ScalarType>>& ShapeList, bool Flag_use_SimilarityTransfrom, int_max MaxThreadCount)
{
	ObjectArray<SparseVector<ScalarType>> SimilarityTable;

	int_max ShapeCount = ShapeList.GetLength();
	if (ShapeCount == 0)
	{
		return SimilarityTable;
	}

	SimilarityTable.Resize(ShapeCount);
		
	//for (int_max n = 0; n <= ShapeCount-2; ++n)
	auto TempFunction = [&](int_max n)
	{
		SimilarityTable[n].Resize(ShapeCount);
		for (int_max m = n+1; m < ShapeCount; ++m)
		{
			// compute similarity between Shape_n and Shape_m
			auto Similarity_nm = DistanceMinimizationBasedShapeAligner3D<ScalarType>::ComputeSimilarityBetweenShape(ShapeList[n], ShapeList[m], Flag_use_SimilarityTransfrom);
			SimilarityTable[n].SetElement(m, Similarity_nm);
		}
	};
	ParallelForLoop(TempFunction, 0, ShapeCount - 2, MaxThreadCount);

	SimilarityTable[ShapeCount-1].Resize(ShapeCount);
	for (int_max n = 0; n < ShapeCount; ++n)
	{		
		for (int_max m = 0; m < ShapeCount; ++m)
		{
			if (m == n)
			{
				SimilarityTable[n].SetElement(m, 1);
			}
			else if (m < n)
			{
				SimilarityTable[n].SetElement(m, SimilarityTable[m][n]);
			}
		}
	}
	return SimilarityTable;
}

template<typename ScalarType>
ScalarType DistanceMinimizationBasedShapeAligner3D<ScalarType>::ComputeSimilarityBetweenShape(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, bool Flag_use_SimilarityTransfrom)
{
	// check input
	if (ShapeA.IsEmpty() || ShapeB.IsEmpty())
	{
		return 0;
	}

	if (ShapeA.GetRowCount() !=3 || ShapeA.GetRowCount() != 3)
	{
		MDK_Error("input shape is wrong @ DistanceMinimizationBasedShapeAligner3D::ComputeSimilarityBetweenShape(...)")
		return 0;
	}

	// Center each shape to [0,0,0]
	// set mean distance to center to 1

	//-------- function to normalize shape --------------------------------//
	auto TempFunction_NormalizeShape = [](const DenseMatrix<ScalarType>& Shape)
	{
		DenseMatrix<ScalarType> Shape_new = Shape;
		auto Center = Shape_new.MeanOfEachRow();
		auto PointCount = Shape_new.GetColCount();		
		ScalarType MeanDistance = 0;
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto& x = Shape_new(0, k);
			auto& y = Shape_new(1, k);
			auto& z = Shape_new(2, k);
			x -= Center[0];
			y -= Center[1];
			z -= Center[2];			
			MeanDistance += std::sqrt(x*x+y*y+z*z);
		}
		MeanDistance /= ScalarType(PointCount);

		for (int_max k = 0; k < 3*PointCount; ++k)
		{
			Shape_new[k]/= MeanDistance;
		}
		return Shape_new;
	};
	//-------- function to center shape --------------------------------//
	auto TempFunction_CenterShape = [](const DenseMatrix<ScalarType>& Shape)
	{
		DenseMatrix<ScalarType> Shape_new = Shape;
		auto Center = Shape_new.MeanOfEachRow();
		auto PointCount = Shape_new.GetColCount();
		ScalarType MeanDistance = 0;
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto& x = Shape_new(0, k);
			auto& y = Shape_new(1, k);
			auto& z = Shape_new(2, k);
			x -= Center[0];
			y -= Center[1];
			z -= Center[2];
		}
		return Shape_new;
	};

	//---------------------------------------------------------------------
	// calculate mean distance error 
	ScalarType MDE = 0;
	if (Flag_use_SimilarityTransfrom == true)
	{
		auto ShapeA_new = TempFunction_NormalizeShape(ShapeA);
		auto ShapeB_new = TempFunction_NormalizeShape(ShapeB);
		auto PointCount = ShapeA_new.GetColCount();
		// transfrom B to A
		SimilarityTransform3D<ScalarType> Transform;
		Transform.SetSourceLandmarkPointSet(&ShapeB_new);
		Transform.SetTargetLandmarkPointSet(&ShapeA_new);
		Transform.EstimateParameter();
		ShapeB_new = Transform.TransformPoint(ShapeB_new);
		
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto xa = ShapeA_new(0, k);
			auto ya = ShapeA_new(1, k);
			auto za = ShapeA_new(2, k);
			auto xb = ShapeB_new(0, k);
			auto yb = ShapeB_new(1, k);
			auto zb = ShapeB_new(2, k);
			MDE += std::sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb) + (za - zb)*(za - zb));
		}
		MDE /= ScalarType(PointCount);
	}
	else// use RigidTransform
	{
		auto ShapeA_new = TempFunction_CenterShape(ShapeA);
		auto ShapeB_new = TempFunction_CenterShape(ShapeB);
		auto PointCount = ShapeA_new.GetColCount();
		// transfrom B to A
		RigidTransform3D<ScalarType> Transform;
		Transform.SetSourceLandmarkPointSet(&ShapeB_new);
		Transform.SetTargetLandmarkPointSet(&ShapeA_new);
		Transform.EstimateParameter();
		ShapeB_new = Transform.TransformPoint(ShapeB_new);

		//calculate mean radius of Shape A
		ScalarType MeanRadiusA = 0;
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto x = ShapeA_new(0, k);
			auto y = ShapeA_new(1, k);
			auto z = ShapeA_new(2, k);
			MeanRadiusA += std::sqrt(x*x + y*y + z*z);
		}
		MeanRadiusA /= ScalarType(PointCount);

		//calculate mean radius of Shape B
		ScalarType MeanRadiusB = 0;
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto x = ShapeB_new(0, k);
			auto y = ShapeB_new(1, k);
			auto z = ShapeB_new(2, k);
			MeanRadiusB += std::sqrt(x*x + y*y + z*z);
		}
		MeanRadiusB /= ScalarType(PointCount);

		auto MeanRadius = std::min(MeanRadiusA, MeanRadiusB);

		// calculate mean distance error 
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto xa = ShapeA_new(0, k);
			auto ya = ShapeA_new(1, k);
			auto za = ShapeA_new(2, k);
			auto xb = ShapeB_new(0, k);
			auto yb = ShapeB_new(1, k);
			auto zb = ShapeB_new(2, k);
			MDE += std::sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb) + (za - zb)*(za - zb));
		}
		MDE /= ScalarType(PointCount);
		MDE /= MeanRadius;		
	}	

	// calculate similarity 
	auto Similarity = std::exp(-MDE*MDE / ScalarType(0.2));
	return Similarity;
}


}//namespace mdk