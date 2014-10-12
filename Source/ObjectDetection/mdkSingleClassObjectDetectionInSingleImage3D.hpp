#ifndef __mdkSingleClassObjectDetectionInSingleImage3D_hpp
#define __mdkSingleClassObjectDetectionInSingleImage3D_hpp

namespace mdk
{
template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::SingleClassObjectDetectionInSingleImage3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::~SingleClassObjectDetectionInSingleImage3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	m_ObjectImage = nullptr;
	m_CandidateOriginList_3DIndex.Clear();
	m_CandidateOriginList_3DPyhsicalPosition = nullptr;
	m_ObjectMaskList = nullptr;
	m_MaxNumberOfThread = 1;

	m_NOBoundCheckRegionList_3DIndex.Clear();
	m_NOBoundCheckRegionList_3DPyhsicalPosition.Clear();
	m_TotalCandidateOriginNumber = 0;
	m_Flag_ScanWholeImageGrid = false;

	m_ObjectMembershipImage.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::SetObjectImage(const Image3D<InputPixelType>* InputImage)
{
	m_ObjectImage = InputImage;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::
SetInfoOfObjectMembershipImage(const DenseVector<ScalarType, 3>& Origin, 
						       const DenseVector<ScalarType, 3>& Spacing,
						       const DenseVector<ScalarType, 3>& Size)
{
	m_ObjectMembershipImage.SetOrigin(Origin);
	m_ObjectMembershipImage.SetSpacing(Spacing);
	m_ObjectMembershipImage.SetSize(Size);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::
SetCandidateOriginListOf3DIndex(DenseMatrix<int_max> Input3DIndexList)
{
	m_CandidateOriginList_3DIndex = std::move(Input3DIndexList);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::
SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList)
{
	m_CandidateOriginList_3DPyhsicalPosition = Input3DPositionList;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::SetObjectMaskList(const DataArray<ObjectMaskType>* InputMaskList)
{
	m_ObjectMaskList = InputMaskList;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
const DataArray<ObjectMask_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>*
SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::GetObjectMaskList()
{
	return m_ObjectMaskList;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::SetThreadNumber(int_max MaxNumber)
{
	m_MaxNumberOfThread = MaxNumber;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::Update()
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
	
	ParallelBlock([&](int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex)
	              {this->EvaluateCandidateAtMultipleOrigin_in_a_thread(OriginIndex_start, OriginIndex_end, ThreadIndex); },
				  0, m_TotalCandidateOriginNumber - 1, m_MaxNumberOfThread, 1);

	//-------------------------------------------------------------------------------
	if (this->Postprocess() == false)
	{
		return false;
	}
	//-------------
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
Image3D<OutputPixelType>* SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::GetObjectMembershipImage()
{
	return &m_ObjectMembershipImage;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (m_ObjectImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ SingleClassObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_ObjectImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ SingleClassObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_CandidateOriginList_3DPyhsicalPosition != nullptr && m_CandidateOriginList_3DIndex.IsEmpty() == false)
	{
		MDK_Error("CandidateOriginList can not be both 3DPyhsicalPosition and 3DIndex @ SingleClassObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	this->BuildMask();

	if (m_ObjectMaskList == nullptr)
	{
		MDK_Error("m_ObjectMaskList is Empty (nullptr) @ SingleClassObjectDetectionInSingleImage3D::Preprocess()")
		return false;
	}

	if (m_ObjectMaskList->IsEmpty() == true)
	{
		MDK_Error("m_ObjectMaskList is Empty @ SingleClassObjectDetectionInSingleImage3D::Preprocess()")
		return false;
	}

	this->ComputeRegionOfNOBoundCheck_3DIndex();
	this->ComputeRegionOfNOBoundCheck_3DPyhsicalPosition();

	if (m_ObjectMembershipImage.IsEmpty() == true)
	{
		m_ObjectMembershipImage.SetOrigin(m_ObjectImage->GetOrigin());
		m_ObjectMembershipImage.SetSpacing(m_ObjectImage->GetSpacing());
		m_ObjectMembershipImage.SetSize(m_ObjectImage->GetSize());
	}
	m_ObjectMembershipImage.SetOrientation(m_ObjectImage->GetOrientation());

	if (m_CandidateOriginList_3DIndex.IsEmpty() == true && m_CandidateOriginList_3DPyhsicalPosition == nullptr)
	{
		m_TotalCandidateOriginNumber = m_ObjectMembershipImage.GetPixelNumber();
		m_Flag_ScanWholeImageGrid = true;
	}
	else if (m_CandidateOriginList_3DIndex.IsEmpty() == false && m_CandidateOriginList_3DPyhsicalPosition == nullptr)
	{
		m_TotalCandidateOriginNumber = m_CandidateOriginList_3DIndex.GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}
	else if (m_CandidateOriginList_3DIndex.IsEmpty() == true && m_CandidateOriginList_3DPyhsicalPosition != nullptr)
	{
		auto OutputImageSize = m_ObjectMembershipImage.GetSize();
		m_CandidateOriginList_3DIndex.ReserveCapacity(m_CandidateOriginList_3DPyhsicalPosition->GetSize());
		for (int_max k = 0; k < m_CandidateOriginList_3DPyhsicalPosition->GetColNumber(); ++k)
		{
			DenseVector<ScalarType, 3> Position;
			m_CandidateOriginList_3DPyhsicalPosition->GetCol(k, Position);
			auto Index3D = m_ObjectMembershipImage.Transform3DPhysicalPositionTo3DIndex(Position);
			auto xIndex = int_max(Index3D[0]); xIndex = std::max(xIndex, int_max(0)); xIndex = std::min(xIndex, OutputImageSize[0]);
			auto yIndex = int_max(Index3D[1]); yIndex = std::max(yIndex, int_max(0)); yIndex = std::min(yIndex, OutputImageSize[1]);
			auto zIndex = int_max(Index3D[2]); zIndex = std::max(zIndex, int_max(0)); zIndex = std::min(zIndex, OutputImageSize[2]);
			m_CandidateOriginList_3DIndex.AppendCol({xIndex, yIndex, zIndex});
		}
		auto tempColIndexList = m_CandidateOriginList_3DIndex.FindUniqueCol();
		m_CandidateOriginList_3DIndex = m_CandidateOriginList_3DIndex.GetSubMatrix(ALL, tempColIndexList);
		//---------------------------------------------------------------------------
		m_TotalCandidateOriginNumber = m_CandidateOriginList_3DIndex.GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}	
	//-------------
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateCandidateAtMultipleOrigin_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanWholeImageGrid == true)
	{
		for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
		{
			auto OriginIndex3D = m_ObjectMembershipImage.TransformLinearIndexTo3DIndex(k);
			this->EvaluateCandidateAtOrigin_3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
		}
	}
	else
	{
		for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
		{
			DenseVector<int_max, 3> OriginIndex3D;
			m_CandidateOriginList_3DIndex.GetCol(k, OriginIndex3D);
			this->EvaluateCandidateAtOrigin_3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
		}
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
int_max SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::GetNumberOfThreadTobeCreated()
{
	return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(m_TotalCandidateOriginNumber, m_MaxNumberOfThread, 1);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::
GetPixelSetByObjectMaskAtOrigin_3DIndex(int_max MaskIndex, int_max x0, int_max y0, int_max z0)
{
	const auto& Mask = (*m_ObjectMaskList)[MaskIndex];	
	auto PixelNumber = Mask.PointSet.GetColNumber();
	DataArray<PixelTypeForMask> PixelSet;
	PixelSet.Resize(PixelNumber);
	if (Mask.Flag_PhysicalPosition == true)
	{
		auto Origin = m_ObjectImage->Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
		for (int_max k = 0; k < PixelNumber; ++k)
		{
			DenseVector<ScalarType, 3> Position;
			Mask.PointSet.GetCol(k, Position);
			Position += Origin;
			PixelSet[k] = PixelTypeForMask(m_ObjectImage->GetPixelAt3DPhysicalPosition(Position));
		}
	}
	else
	{
		for (int_max k = 0; k < PixelNumber; ++k)
		{
			DenseVector<ScalarType, 3> Index3D;
			Mask.PointSet.GetCol(k, Index3D);
			Index3D[0] += x0;
			Index3D[1] += y0;
			Index3D[2] += z0;
			PixelSet[k] = PixelTypeForMask(m_ObjectImage->GetPixelAt3DIndex(Index3D));
		}
	}
	return PixelSet;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::
GetPixelSetByObjectMaskAtOrigin_3DPyhsicalPosition(int_max MaskIndex, ScalarType x0, ScalarType y0, ScalarType z0)
{
	auto Origin = m_ObjectImage->Transform3DIndexTo3DPhysicalPosition<ScalarType, ScalarType>(x0, y0, z0);
	return this->GetPixelSetByObjectMaskAtOrigin_3DIndex<PixelTypeForMask>(MaskIndex, Origin[0], Origin[1], Origin[2]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DIndex()
{
	// take reference, avoid m_ObjectMaskList->
	const auto& InputMaskList = *m_ObjectMaskList;

	if (InputMaskList.IsEmpty() == true)
	{
		m_NOBoundCheckRegionList_3DIndex.Clear();
		return;
	}

	auto MaskNumber = InputMaskList.GetLength();

	m_NOBoundCheckRegionList_3DIndex.Resize(MaskNumber);

	auto InputImageSize = m_ObjectImage->GetSize();

	int_max SafeDistance = 2;

	for (int_max i = 0; i < MaskNumber; ++i)
	{
		m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = true;

		int_max MaxDeviation_x[2] = { 0, 0 }; // maximum deviation from center in x direction
		int_max MaxDeviation_y[2] = { 0, 0 };
		int_max MaxDeviation_z[2] = { 0, 0 };

		for (int_max j = 0; j < InputMaskList[i].PointSet.GetColNumber(); ++j)
		{
			auto temp_x = InputMaskList[i].PointSet(0, j);

			if (temp_x < 0.0)
			{
				MaxDeviation_x[0] = std::max(MaxDeviation_x[0], int_max(-temp_x));
			}
			else
			{
				MaxDeviation_x[1] = std::max(MaxDeviation_x[1], int_max(temp_x));
			}

			auto temp_y = InputMaskList[i].PointSet(1, j);

			if (temp_y < 0.0)
			{
				MaxDeviation_y[0] = std::max(MaxDeviation_y[0], int_max(-temp_y));
			}
			else
			{
				MaxDeviation_y[1] = std::max(MaxDeviation_y[1], int_max(temp_y));
			}

			auto temp_z = InputMaskList[i].PointSet(2, j);

			if (temp_z < 0.0)
			{
				MaxDeviation_z[0] = std::max(MaxDeviation_z[0], int_max(-temp_z));
			}
			else
			{
				MaxDeviation_z[1] = std::max(MaxDeviation_z[1], int_max(temp_z));
			}
		}

		if (MaxDeviation_x[0] + MaxDeviation_x[1] + 1 + 2 * SafeDistance < InputImageSize[0]
			&& MaxDeviation_y[0] + MaxDeviation_y[1] + 1 + 2 * SafeDistance < InputImageSize[1]
			&& MaxDeviation_z[0] + MaxDeviation_z[1] + 1 + 2 * SafeDistance < InputImageSize[2])
		{
			m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = false;

			m_NOBoundCheckRegionList_3DIndex[i].x0 = MaxDeviation_x[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].x1 = InputImageSize[0] - 1 - MaxDeviation_x[1] - SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].y0 = MaxDeviation_y[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].y1 = InputImageSize[1] - 1 - MaxDeviation_y[1] - SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].z0 = MaxDeviation_z[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].z1 = InputImageSize[2] - 1 - MaxDeviation_z[1] - SafeDistance;
		}
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DPyhsicalPosition()
{
	// take reference, avoid m_ObjectMaskList->
	const auto& InputMaskList = *m_ObjectMaskList;

	if (InputMaskList.IsEmpty() == true)
	{
		m_NOBoundCheckRegionList_3DPyhsicalPosition.Clear();
		return;
	}

	auto MaskNumber = InputMaskList.GetLength();

	m_NOBoundCheckRegionList_3DPyhsicalPosition.Resize(MaskNumber);

	auto PhysicalOrigin = m_ObjectImage->GetOrigin();
	auto PhysicalSize = m_ObjectImage->GetPhysicalSize();
	auto PixelSpacing = m_ObjectImage->GetSpacing();

	auto SafeDistance_x = 2 * PixelSpacing[0];
	auto SafeDistance_y = 2 * PixelSpacing[1];
	auto SafeDistance_z = 2 * PixelSpacing[2];

	for (int_max i = 0; i < MaskNumber; ++i)
	{
		m_NOBoundCheckRegionList_3DPyhsicalPosition[i].IsEmpty = true;

		ScalarType MaxDeviation_x[2] = { 0, 0 };
		ScalarType MaxDeviation_y[2] = { 0, 0 };
		ScalarType MaxDeviation_z[2] = { 0, 0 };

		for (int_max j = 0; j < InputMaskList[i].PointSet.GetColNumber(); ++j)
		{
			auto temp_x = InputMaskList[i].PointSet(0, j);

			if (temp_x < 0.0)
			{
				MaxDeviation_x[0] = std::max(MaxDeviation_x[0], -temp_x);
			}
			else
			{
				MaxDeviation_x[1] = std::max(MaxDeviation_x[1], temp_x);
			}

			auto temp_y = InputMaskList[i].PointSet(1, j);

			if (temp_y < 0.0)
			{
				MaxDeviation_y[0] = std::max(MaxDeviation_y[0], -temp_y);
			}
			else
			{
				MaxDeviation_y[1] = std::max(MaxDeviation_y[1], temp_y);
			}

			auto temp_z = InputMaskList[i].PointSet(2, j);

			if (temp_z < 0.0)
			{
				MaxDeviation_z[0] = std::max(MaxDeviation_z[0], -temp_z);
			}
			else
			{
				MaxDeviation_z[1] = std::max(MaxDeviation_z[1], temp_z);
			}
		}

		if (MaxDeviation_x[0] + MaxDeviation_x[1] + 2 * SafeDistance_x < PhysicalSize[0]
			&& MaxDeviation_y[0] + MaxDeviation_y[1] + 2 * SafeDistance_y < PhysicalSize[1]
			&& MaxDeviation_z[0] + MaxDeviation_z[1] + 2 * SafeDistance_z < PhysicalSize[2])
		{
			m_NOBoundCheckRegionList_3DPyhsicalPosition[i].IsEmpty = false;

			m_NOBoundCheckRegionList_3DPyhsicalPosition[i].x0 = PhysicalOrigin[0] + MaxDeviation_x[0] + SafeDistance_x;

			m_NOBoundCheckRegionList_3DPyhsicalPosition[i].x1 = PhysicalOrigin[0] + PhysicalSize[0] - MaxDeviation_x[1] - SafeDistance_x;

			m_NOBoundCheckRegionList_3DPyhsicalPosition[i].y0 = PhysicalOrigin[1] + MaxDeviation_y[0] + SafeDistance_y;

			m_NOBoundCheckRegionList_3DPyhsicalPosition[i].y1 = PhysicalOrigin[1] + PhysicalSize[1] - MaxDeviation_y[1] - SafeDistance_y;

			m_NOBoundCheckRegionList_3DPyhsicalPosition[i].z0 = PhysicalOrigin[2] + MaxDeviation_z[0] + SafeDistance_z;

			m_NOBoundCheckRegionList_3DPyhsicalPosition[i].z1 = PhysicalOrigin[2] + PhysicalSize[2] - MaxDeviation_z[1] - SafeDistance_z;
		}
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherMaskIsInsideImage_AtOrigin_3DIndex(int_max MaskIndex, int_max x, int_max y, int_max z)
{
	if (x < m_NOBoundCheckRegionList_3DIndex[MaskIndex].x0 || x > m_NOBoundCheckRegionList_3DIndex[MaskIndex].x1
		|| y < m_NOBoundCheckRegionList_3DIndex[MaskIndex].y0 || y > m_NOBoundCheckRegionList_3DIndex[MaskIndex].y1
		|| z < m_NOBoundCheckRegionList_3DIndex[MaskIndex].z0 || z > m_NOBoundCheckRegionList_3DIndex[MaskIndex].z1)
	{
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherMaskIsInsideImage_AtOrigin_3DPyhsicalPosition(int_max MaskIndex, ScalarType x, ScalarType y, ScalarType z)
{
	if (x < m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].x0 || x > m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].x1
		|| y < m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].y0 || y > m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].y1
		|| z < m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].z0 || z > m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].z1)
	{
		return false;
	}

	return true;
}


}// namespace mdk

#endif