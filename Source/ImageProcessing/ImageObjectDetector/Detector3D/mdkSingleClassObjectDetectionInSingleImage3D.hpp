#ifndef __mdkSingleClassObjectDetectionInSingleImage3D_hpp
#define __mdkSingleClassObjectDetectionInSingleImage3D_hpp

namespace mdk
{
template<typename VoxelType, typename ScalarType>
SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::SingleClassObjectDetectionInSingleImage3D()
{
	this->Clear();
}


template<typename VoxelType, typename ScalarType>
SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::~SingleClassObjectDetectionInSingleImage3D()
{
}


template<typename VoxelType, typename ScalarType>
SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::Clear()
{
	m_InputImage = nullptr;
	m_CandidateOriginList_3DContinuousIndex = nullptr;
	m_CandidateOriginList_3DPyhsicalPosition = nullptr;
	m_MaskList = nullptr;
	m_MaxNumberOfThread = 1;

	m_NOBoundCheckRegionList_3DContinuousIndex.Clear();
	m_NOBoundCheckRegionList_3DPyhsicalPosition.Clear();
	m_TotalCandidateOriginNumber = 0;
	m_Flag_ScanWholeImageGrid = false;

	m_Output.Clear();
}


template<typename VoxelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::SetInputImage(const Image3D<VoxelType>* InputImage)
{
	m_InputImage = InputImage;
}


template<typename VoxelType, typename OutputVoxelType>
void SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::SetCandidateOriginListOf3DContinuousIndex(const DenseMatrix<double>* Input3DIndexList)
{
	m_CandidateOriginList_3DContinuousIndex = Input3DIndexList;
}


template<typename VoxelType, typename OutputVoxelType>
void SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<double>* Input3DPositionList)
{
	m_CandidateOriginList_3DPyhsicalPosition = Input3DPositionList;
}


template<typename VoxelType, typename OutputVoxelType>
void SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::
SetMaskList(const ObjectArray<Mask_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>* InputMaskList)
{
	m_MaskList = InputMaskList;
}


template<typename VoxelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::SetThreadNumber(int_max MaxNumber)
{
	m_ThreadMaxNumber = MaxNumber;
}


template<typename VoxelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return false;
	}

	if (this->Preprocess() == false)
	{
		return false;
	}

	// multi-thread -----------------------------------------------------------------

	ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex){this->EvaluateMultipleCandidate(Index_start, Index_end, ThreadIndex); },
					  0, m_TotalCandidateOriginNumber - 1, m_MaxNumberOfThread, 1);

	//-------------------------------------------------------------------------------
	if (this->Postprocess() == false)
	{
		return false;
	}

}


template<typename VoxelType, typename ScalarType>
ObjectArray<EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>&
SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::OutputResult()
{
	return m_Output;
}


template<typename VoxelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ SingleClassObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ SingleClassObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_MaskList == nullptr)
	{
		MDK_Error("Input MaskList is Empty (nullptr) @ SingleClassObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_MaskList->IsEmpty() == true)
	{
		MDK_Error("Input MaskList is Empty @ SingleClassObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_CandidateOriginList_3DPyhsicalPosition != nullptr && m_CandidateOriginList_3DContinuousIndex != nullptr)
	{
		MDK_Error("CandidateOriginList can not be both 3DPyhsicalPosition and 3DContinuousIndex @ SingleClassObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename VoxelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::Preprocess()
{
	if (m_CandidateOriginList_3DContinuousIndex == nullptr && m_CandidateOriginList_3DPyhsicalPosition == nullptr)
	{
		auto InputSize = m_InputImage->GetSize();
		m_TotalCandidateOriginNumber = InputSize.Lx*InputSize.Ly*InputSize.Lz;
		m_Flag_ScanWholeImageGrid = true;
	}
	else if (m_CandidateOriginList_3DContinuousIndex != nullptr && m_CandidateOriginList_3DPyhsicalPosition == nullptr)
	{
		m_TotalCandidateOriginNumber = m_CandidateOriginList_3DContinuousIndex->GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}
	else if (m_CandidateOriginList_3DContinuousIndex == nullptr && m_CandidateOriginList_3DPyhsicalPosition != nullptr)
	{
		m_TotalCandidateOriginNumber = m_CandidateOriginList_3DPyhsicalPosition->GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}

	this->BuildMask();
	this->ComputeRegionOfNOBoundCheck_3DContinuousIndex();
	this->ComputeRegionOfNOBoundCheck_3DPyhsicalPosition();

	m_Output.FastResize(m_OriginNumber*m_MaskList->GetElementNumber());

	return true;
}


template<typename VoxelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DContinuousIndex()
{
	// take reference, avoid m_MaskList->
	auto& InputMaskList = *m_MaskList;

	if (InputMaskList.IsEmpty() == true)
	{
		m_NOBoundCheckRegionList_3DIndex.Clear();
		return;
	}

	auto MaskNumber = InputMaskList.GetLength();

	m_NOBoundCheckRegionList_3DIndex.Resize(MaskNumber);

	auto InputImageSize = m_InputImage->GetSize();

	int_max SafeDistance = 2;

	for (int_max i = 0; i < MaskNumber; ++i)
	{
		m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = true;

		int_max MaxDeviation_x[2] = { 0, 0 }; // maximum deviation from center in x direction
		int_max MaxDeviation_y[2] = { 0, 0 };
		int_max MaxDeviation_z[2] = { 0, 0 };

		for (int_max k = 0; k < InputMaskList[i].PointSet.GetElementNumber(); ++k)
		{
			for (int_max j = 0; j < InputMaskList[i].PointSet[i].GetColNumber(); ++j)
			{
				auto temp_x = InputMaskList[i].PointSet[i](0, j);

				if (temp_x < 0.0)
				{
					MaxDeviation_x[0] = std::max(MaxDeviation_x[0], int_max(-temp_x));
				}
				else
				{
					MaxDeviation_x[1] = std::max(MaxDeviation_x[1], int_max(temp_x));
				}

				auto temp_y = m_MaskList[i](1, j);

				if (temp_y < 0.0)
				{
					MaxDeviation_y[0] = std::max(MaxDeviation_y[0], int_max(-temp_y));
				}
				else
				{
					MaxDeviation_y[1] = std::max(MaxDeviation_y[1], int_max(temp_y));
				}

				auto temp_z = InputMaskList[i](2, j);

				if (temp_z < 0.0)
				{
					MaxDeviation_z[0] = std::max(MaxDeviation_z[0], int_max(-temp_z));
				}
				else
				{
					MaxDeviation_z[1] = std::max(MaxDeviation_z[1], int_max(temp_z));
				}
			}
		}

		if (MaxDeviation_x[0] + MaxDeviation_x[1] + 1 + 2 * SafeDistance < InputImageSize.Lx
			&& MaxDeviation_y[0] + MaxDeviation_y[1] + 1 + 2 * SafeDistance < InputImageSize.Ly
			&& MaxDeviation_z[0] + MaxDeviation_z[1] + 1 + 2 * SafeDistance < InputImageSize.Lz)
		{
			m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = false;

			m_NOBoundCheckRegionList_3DIndex[i].x0 = MaxDeviation_x[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].x1 = InputImageSize.Lx - 1 - MaxDeviation_x[1] - SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].y0 = MaxDeviation_y[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].y1 = InputImageSize.Ly - 1 - MaxDeviation_y[1] - SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].z0 = MaxDeviation_z[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].z1 = InputImageSize.Lz - 1 - MaxDeviation_z[1] - SafeDistance;
		}
	}
}


template<typename VoxelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DPyhsicalPosition()
{
	// take reference, avoid m_MaskList->
	auto& InputMaskList = *m_MaskList;

	if (InputMaskList.IsEmpty() == true)
	{
		m_NOBoundCheckRegionList_3DPosition.Clear();
		return;
	}

	auto MaskNumber = InputMaskList.GetLength();

	m_NOBoundCheckRegionList_3DPosition.resize(MaskNumber);

	auto PhysicalOrigin = m_InputImage->GetOrigin();
	auto PhysicalSize = m_InputImage->GetPhysicalSize();
	auto VoxelSpacing = m_InputImage->GetSpacing();

	auto SafeDistance_x = 2 * VoxelSpacing.Sx;
	auto SafeDistance_y = 2 * VoxelSpacing.Sy;
	auto SafeDistance_z = 2 * VoxelSpacing.Sz;

	for (int_max i = 0; i < MaskNumber; ++i)
	{
		m_NOBoundCheckRegionList_3DPosition[i].IsEmpty = true;

		double MaxDeviation_x[2] = { 0, 0 };
		double MaxDeviation_y[2] = { 0, 0 };
		double MaxDeviation_z[2] = { 0, 0 };

		for (int_max k = 0; k < InputMaskList[i].PointSet.GetElementNumber(); ++k)
		{
			for (int_max j = 0; j < InputMaskList[i].PointSet[i].GetColNumber(); ++j)
			{
				auto temp_x = InputMaskList[i].PointSet[i](0, j);

				if (temp_x < 0.0)
				{
					MaxDeviation_x[0] = std::max(MaxDeviation_x[0], -temp_x);
				}
				else
				{
					MaxDeviation_x[1] = std::max(MaxDeviation_x[1], temp_x);
				}

				auto temp_y = InputMaskList[i](1, j);

				if (temp_y < 0.0)
				{
					MaxDeviation_y[0] = std::max(MaxDeviation_y[0], -temp_y);
				}
				else
				{
					MaxDeviation_y[1] = std::max(MaxDeviation_y[1], temp_y);
				}

				auto temp_z = InputMaskList[i](2, j);

				if (temp_z < 0.0)
				{
					MaxDeviation_z[0] = std::max(MaxDeviation_z[0], -temp_z);
				}
				else
				{
					MaxDeviation_z[1] = std::max(MaxDeviation_z[1], temp_z);
				}
			}
		}

		if (MaxDeviation_x[0] + MaxDeviation_x[1] + 2 * SafeDistance_x < PhysicalSize.Lx
			&& MaxDeviation_y[0] + MaxDeviation_y[1] + 2 * SafeDistance_y < PhysicalSize.Ly
			&& MaxDeviation_z[0] + MaxDeviation_z[1] + 2 * SafeDistance_z < PhysicalSize.Lz)
		{
			m_NOBoundCheckRegionList_3DPosition[i].IsEmpty = false;

			m_NOBoundCheckRegionList_3DPosition[i].x0 = PhysicalOrigin.x + MaxDeviation_x[0] + SafeDistance_x;

			m_NOBoundCheckRegionList_3DPosition[i].x1 = PhysicalOrigin.x + PhysicalSize.Lx - MaxDeviation_x[1] - SafeDistance_x;

			m_NOBoundCheckRegionList_3DPosition[i].y0 = PhysicalOrigin.y + MaxDeviation_y[0] + SafeDistance_y;

			m_NOBoundCheckRegionList_3DPosition[i].y1 = PhysicalOrigin.y + PhysicalSize.Ly - MaxDeviation_y[1] - SafeDistance_y;

			m_NOBoundCheckRegionList_3DPosition[i].z0 = PhysicalOrigin.z + MaxDeviation_z[0] + SafeDistance_z;

			m_NOBoundCheckRegionList_3DPosition[i].z1 = PhysicalOrigin.z + PhysicalSize.Lz - MaxDeviation_z[1] - SafeDistance_z;
		}
	}
}


template<typename VoxelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::
WhetherMaskIsInsideImage_AtOrigin_3DContinuousIndex(double x, double y, double z, int_max MaskIndex)
{
	if (x < double(m_NOBoundCheckRegionList_3DContinuousIndex[MaskIndex].x0) || x > double(m_NOBoundCheckRegionList_3DContinuousIndex[MaskIndex].x1)
		|| y < double(m_NOBoundCheckRegionList_3DContinuousIndex[MaskIndex].y0) || y > double(m_NOBoundCheckRegionList_3DContinuousIndex[MaskIndex].y1)
		|| z < double(m_NOBoundCheckRegionList_3DContinuousIndex[MaskIndex].z0) || z > double(m_NOBoundCheckRegionList_3DContinuousIndex[MaskIndex].z1))
	{
		return true;
	}

	return false;
}


template<typename VoxelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DPyhsicalPosition(double x, double y, double z, int_max MaskIndex)
{
	bool WhetherToCheck = false;

	if (m_NOBoundCheckRegionList_3DPosition[MaskIndex].IsEmpty == true)
	{
		WhetherToCheck = true;
	}
	else
	{
		if (x < m_NOBoundCheckRegionList_3DPosition[MaskIndex].x0 || x > m_NOBoundCheckRegionList_3DPosition[MaskIndex].x1
			|| y < m_NOBoundCheckRegionList_3DPosition[MaskIndex].y0 || y > m_NOBoundCheckRegionList_3DPosition[MaskIndex].y1
			|| z < m_NOBoundCheckRegionList_3DPosition[MaskIndex].z0 || z > m_NOBoundCheckRegionList_3DPosition[MaskIndex].z1)
		{
			WhetherToCheck = true;
		}
	}

	return WhetherToCheck;
}


template<typename VoxelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::
EvaluateMultipleCandidate_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex)
{
	auto MaskNumber = m_MaskList->GetElementNumber();

	if (m_Flag_ScanWholeImageGrid == true)
	{ 
		for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
		{
			auto Origin = m_InputImage->TransformLinearIndexTo3DIndex<ScalarType>(k);
			for (int_max n = 0; n < MaskNumber; ++n)
			{
				auto& EvaluationResult = m_Output[k*MaskNumber + n];
				this->EvaluateCandidateAt3DIndex(EvaluationResult, n, Origin[0], Origin[1], Origin[2], ThreadIndex);
			}
		}
	}
	else
	{
		if (m_CandidateOriginList_3DPosition != nullptr)
		{
			for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
			{
				auto x = (*m_CandidateOriginList_3DPosition)(0, k);
				auto y = (*m_CandidateOriginList_3DPosition)(1, k);
				auto z = (*m_CandidateOriginList_3DPosition)(2, k);
				for (int_max n = 0; n < MaskNumber; ++n)
				{
					auto& EvaluationResult = m_Output[k*MaskNumber + n];
					if ((*m_MaskList)[n].Flag_3DPhysicalPosition == true)
					{
						this->EvaluateCandidateAt3DPyhsicalPosition(EvaluationResult, n, x, y, z, ThreadIndex);
					}
					else
					{
						auto ContinuousIndex = m_InputImage->Transform3DPhysicalPositionTo3DContinuousIndex(x, y, z);
						this->EvaluateCandidateAt3DContinuousIndex(EvaluationResult, n, ContinuousIndex[0], ContinuousIndex[1], ContinuousIndex[2], ThreadIndex);
					}
				}
			}
		}
		else if (m_CandidateOriginList_3DIndex != nullptr)
		{
			for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
			{
				auto x = (*m_CandidateOriginList_3DIndex)(0, k);
				auto y = (*m_CandidateOriginList_3DIndex)(1, k);
				auto z = (*m_CandidateOriginList_3DIndex)(2, k);
				for (int_max n = 0; n < MaskNumber; ++n)
				{
					auto& EvaluationResult = m_Output[k*MaskNumber + n];
					if ((*m_MaskList)[n].Flag_3DPhysicalPosition == false)
					{
						this->EvaluateCandidateAt3DContinuousIndex(EvaluationResult, n, x, y, z, ThreadIndex);
					}
					else
					{
						auto PhysicalPosition = m_InputImage->Transform3DContinuousIndexTo3DPhysicalPosition(x, y, z);
						this->EvaluateCandidateAt3DPyhsicalPosition(EvaluationResult, n, PhysicalPosition[0], PhysicalPosition[1], PhysicalPosition[2], ThreadIndex);
					}
				}
			}
		}
	}
}


template<typename VoxelType, typename ScalarType>
int_max SingleClassObjectDetectionInSingleImage3D<VoxelType, ScalarType>::GetNumberOfThreadsTobeCreated()
{

}

}// namespace mdk


#endif