#ifndef __mdkPartBasedObjectDetectionInSingleImage3D_hpp
#define __mdkPartBasedObjectDetectionInSingleImage3D_hpp

namespace mdk
{
template<typename PixelType, typename ScalarType>
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::PartBasedObjectDetectionInSingleImage3D()
{
	this->Clear();
}


template<typename PixelType, typename ScalarType>
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::~PartBasedObjectDetectionInSingleImage3D()
{
}


template<typename PixelType, typename ScalarType>
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::Clear()
{
	m_ObjectImage = nullptr;
	m_ObjectPartDetectorList.Clear();
	m_CandidateOriginList_3DIndex = nullptr;
	m_CandidateOriginList_3DPyhsicalPosition = nullptr;
	m_ObjectMaskList = nullptr;
	m_MaxNumberOfThread = 1;

	m_NOBoundCheckRegionList_3DIndex.Clear();
	m_NOBoundCheckRegionList_3DPyhsicalPosition.Clear();

	m_TotalCandidateOriginNumber = 0;
	m_Flag_ScanWholeImageGrid = false;

	m_ObjectProbabilityImage.Clear();
}


template<typename PixelType, typename ScalarType>
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::ClearOutput()
{
	m_ObjectProbabilityImage.Clear();
}


template<typename PixelType, typename ScalarType>
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::SetObjectImage(const Image3D<PixelType>* ObjectImage)
{
	m_ObjectImage = ObjectImage;
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::SetObjectPartNumber(int_max PartNumber)
{
	m_ObjectPartNumber = PartNumber; // number of parts, NOT PartIndex	
	if (m_ObjectPartDetectorList.IsEmpty() == true)
	{		
		m_ObjectPartDetectorList.Resize(m_ObjectPartNumber);
		m_ObjectPartDetectorList.Fill(nullptr);
	}
	else
	{
		auto OldNumber = m_ObjectPartDetectorList.GetLength();
		m_ObjectPartDetectorList.Resize(m_ObjectPartNumber);
		for (int_max k = OldNumber; k < m_ObjectPartNumber; ++k)
		{
			m_ObjectPartDetectorList[k] = nullptr;
		}
	}
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::
SetObjectPartDetector(int_max PartIndex, PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>* PartDetector)
{
	if (PartIndex >= m_ObjectPartDetectorList.GetLength())
	{
		MDK_Error("PartIndex >= m_ObjectPartNumber @ PartBasedObjectDetectionInSingleImage3D::SetObjectPartDetector(...)")
		return;
	}
	m_ObjectPartDetectorList[PartIndex] = PartDetector;
}


template<typename PixelType, typename ScalarType>
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>* 
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::GetObjectPartDetector(int_max PartIndex)
{
	if (PartIndex >= m_ObjectPartDetectorList.GetLength())
	{
		MDK_Error("PartIndex >= PartNumber @ PartBasedObjectDetectionInSingleImage3D::SetObjectPartDetector(...)")
		return nullptr;
	}
	return m_ObjectPartDetectorList[PartIndex];
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::
const ObjectArray<PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>*>& GetObjectPartDetectorList()
{
	return m_ObjectPartDetectorList;
}


template<typename PixelType, typename OutputPixelType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::SetCandidateOriginListOf3DIndex(DenseMatrix<int_max> Input3DIndexList)
{
	m_CandidateOriginList_3DIndex = std::move(Input3DIndexList);
}


template<typename PixelType, typename OutputPixelType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList)
{
	m_CandidateOriginList_3DPyhsicalPosition = Input3DPositionList;
}


template<typename PixelType, typename OutputPixelType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::
SetObjectMaskList(const ObjectArray<ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D<ScalarType>>* InputMaskList)
{
	m_ObjectMaskList = InputMaskList;
}


template<typename PixelType, typename OutputPixelType>
const ObjectArray<ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D<ScalarType>>*
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::GetObjectMaskList()
{
	return m_ObjectMaskList;
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::SetThreadNumber(int_max MaxNumber)
{
	m_MaxNumberOfThread = MaxNumber;
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::EnablePipeline()
{
	m_Flag_Pipeline = true;
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::DisablePipeline()
{
	m_Flag_Pipeline = false;
}


template<typename PixelType, typename ScalarType>
bool PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return false;
	}

	if (this->Preprocess() == false)
	{
		return false;
	}

	if (m_Flag_Pipeline == true)
	{
		for (int_max k = 0; k < m_ObjectPartDetectorList[k]; ++k)
		{
			m_ObjectPartDetectorList[k]->Update();
			this->SetObjectPartProbabilityImage(k, m_ObjectPartDetectorList[k]->GetObjectProbabilityImage());
		}
		for (int_max k = 0; k < m_ObjectPartDetectorList[k]; ++k)
		{
			auto tempDetectorList = m_ObjectPartDetectorList[k]->GetObjectPartDetectorList();
			for (int_max n = 0; n < tempDetectorList.GetLength(); ++n)
			{
				tempDetectorList[n]->Clear();
			}
		}
	}

	// multi-thread -----------------------------------------------------------------

	ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex){this->EvaluateMultipleCandidate_in_a_thread(Index_start, Index_end, ThreadIndex);},
					  0, m_TotalCandidateOriginNumber - 1, m_MaxNumberOfThread, 1);

	//-------------------------------------------------------------------------------
	if (this->Postprocess() == false)
	{
		return false;
	}
	//-----------
	return true;
}


template<typename PixelType, typename ScalarType>
Image3D<typename PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::ObjectProbabilityImagePixelType>*
PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::GetObjectProbabilityImage()
{
	return &m_ObjectProbabilityImage;
}


template<typename PixelType, typename ScalarType>
bool PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::CheckInput()
{
	if (m_ObjectMaskList == nullptr)
	{
		MDK_Error("Input MaskList is Empty (nullptr) @ PartBasedObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_ObjectMaskList->IsEmpty() == true)
	{
		MDK_Error("Input MaskList is Empty @ PartBasedObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_CandidateOriginList_3DPyhsicalPosition != nullptr && m_CandidateOriginList_3DIndex.IsEmpty() == false)
	{
		MDK_Error("CandidateOriginList can not be both 3DPyhsicalPosition and 3DIndex @ PartBasedObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename PixelType, typename ScalarType>
bool PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::Preprocess()
{
	if (m_CandidateOriginList_3DIndex.IsEmpty() == true && m_CandidateOriginList_3DPyhsicalPosition == nullptr)
	{
		m_TotalCandidateOriginNumber = m_ObjectImage->GetPixelNumber();
		m_Flag_ScanWholeImageGrid = true;
	}
	else if (m_CandidateOriginList_3DIndex.IsEmpty() == false && m_CandidateOriginList_3DPyhsicalPosition == nullptr)
	{
		m_TotalCandidateOriginNumber = m_CandidateOriginList_3DIndex.GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}
	else if (m_CandidateOriginList_3DIndex.IsEmpty() == true && m_CandidateOriginList_3DPyhsicalPosition != nullptr)
	{
		auto InputImageSize = m_ObjectImage->GetSize();
		m_CandidateOriginList_3DIndex.ReserveCapacity(m_CandidateOriginList_3DPyhsicalPosition->GetSize());
		for (int_max k = 0; k < m_CandidateOriginList_3DPyhsicalPosition->GetColNumber; ++k)
		{
			DenseVector<ScalarType, 3> Position;
			m_CandidateOriginList_3DPyhsicalPosition->GetCol(k, Position);
			auto Index3D = m_ObjectImage->Transform3DPhysicalPositionTo3DIndex(Position);
			if (Index3D[0] >= 0 && Index3D[0] < InputImageSize[0]
				&& Index3D[1] >= 0 && Index3D[1] < InputImageSize[1]
				&& Index3D[2] >= 0 && Index3D[2] < InputImageSize[2])
			{
				m_CandidateOriginList_3DIndex.AppendCol(Index3D);
			}
		}
		auto tempColIndexList = m_CandidateOriginList_3DIndex.FindUniqueCol();
		m_CandidateOriginList_3DIndex = m_CandidateOriginList_3DIndex.GetSubMatrix(ALL, tempColIndexList);
		//---------------------------------------------------------------------------
		m_TotalCandidateOriginNumber = m_CandidateOriginList_3DIndex.GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}
	//-------------
	this->BuildMask();
	this->ComputeRegionOfNOBoundCheck_3DIndex();
	this->ComputeRegionOfNOBoundCheck_3DPyhsicalPosition();
	//-------------
	m_ObjectProbabilityImage.SetSize(m_ObjectImage->GetSize());
	m_ObjectProbabilityImage.SetOrigin(m_ObjectImage->GetOrigin());
	m_ObjectProbabilityImage.SetSpacing(m_ObjectImage->GetSpacing());
	m_ObjectProbabilityImage.SetOrientation(m_ObjectImage->GetOrientation());
	//-------------
	return true;
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DIndex()
{
	// take reference, avoid m_ObjectMaskList->
	auto& MaskList = *m_ObjectMaskList;

	if (MaskList.IsEmpty() == true)
	{
		m_NOBoundCheckRegionList_3DIndex.Clear();
		return;
	}

	auto MaskNumber = MaskList.GetLength();

	m_NOBoundCheckRegionList_3DIndex.Resize(MaskNumber);

	int_max SafeDistance = 2;

	for (int_max i = 0; i < MaskNumber; ++i)
	{
		m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = true;

		int_max MaxDeviation_x[2] = { 0, 0 }; // maximum deviation from center in x direction
		int_max MaxDeviation_y[2] = { 0, 0 };
		int_max MaxDeviation_z[2] = { 0, 0 };

		for (int_max j = 0; j < MaskList[i].PointSet.GetColNumber(); ++j)
		{
			auto temp_x = MaskList[i].PointSet(0, j);

			if (temp_x < 0.0)
			{
				MaxDeviation_x[0] = std::max(MaxDeviation_x[0], int_max(-temp_x));
			}
			else
			{
				MaxDeviation_x[1] = std::max(MaxDeviation_x[1], int_max(temp_x));
			}

			auto temp_y = MaskList[i].PointSet(1, j);

			if (temp_y < 0.0)
			{
				MaxDeviation_y[0] = std::max(MaxDeviation_y[0], int_max(-temp_y));
			}
			else
			{
				MaxDeviation_y[1] = std::max(MaxDeviation_y[1], int_max(temp_y));
			}

			auto temp_z = MaskList[i].PointSet(2, j);

			if (temp_z < 0.0)
			{
				MaxDeviation_z[0] = std::max(MaxDeviation_z[0], int_max(-temp_z));
			}
			else
			{
				MaxDeviation_z[1] = std::max(MaxDeviation_z[1], int_max(temp_z));
			}
		}

		if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 1 + 2 * SafeDistance < m_ImageSize[0]
			&& MaxDeviation_y[0] + MaxDeviation_y[1] + 1 + 2 * SafeDistance < m_ImageSize[1]
			&& MaxDeviation_z[0] + MaxDeviation_z[1] + 1 + 2 * SafeDistance < m_ImageSize[2])
		{
			m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = false;

			m_NOBoundCheckRegionList_3DIndex[i].x0 = MaxDeviation_x[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].x1 = m_ImageSize[0] - 1 - MaxDeviation_x[1] - SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].y0 = MaxDeviation_y[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].y1 = m_ImageSize[1] - 1 - MaxDeviation_y[1] - SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].z0 = MaxDeviation_z[0] + SafeDistance;

			m_NOBoundCheckRegionList_3DIndex[i].z1 = m_ImageSize[2] - 1 - MaxDeviation_z[1] - SafeDistance;
		}
	}
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DPyhsicalPosition()
{
	// take reference, avoid m_ObjectMaskList->
	auto& MaskList = *m_ObjectMaskList;

	if (MaskList.IsEmpty() == true)
	{
		m_NOBoundCheckRegionList_3DPyhsicalPosition.Clear();
		return;
	}

	auto MaskNumber = MaskList.GetLength();

	m_NOBoundCheckRegionList_3DPyhsicalPosition.resize(MaskNumber);

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

		if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 2 * SafeDistance_x < PhysicalSize[0]
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


template<typename PixelType, typename ScalarType>
bool PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::
WhetherMaskIsInsideImage_AtOrigin_3DIndex(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex)
{
	if (x < ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].x0) || x > ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].x1)
		|| y < ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].y0) || y > ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].y1)
		|| z < ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].z0) || z > ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].z1))
	{
		return true;
	}

	return false;
}


template<typename PixelType, typename ScalarType>
bool PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DPyhsicalPosition(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex)
{
	bool WhetherToCheck = false;

	if (m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].IsEmpty == true)
	{
		WhetherToCheck = true;
	}
	else
	{
		if (x < m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].x0 || x > m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].x1
			|| y < m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].y0 || y > m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].y1
			|| z < m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].z0 || z > m_NOBoundCheckRegionList_3DPyhsicalPosition[MaskIndex].z1)
		{
			WhetherToCheck = true;
		}
	}

	return WhetherToCheck;
}


template<typename PixelType, typename ScalarType>
void PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::
EvaluateCandidateAtMultipleOrigin_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanWholeImageGrid == true)
	{ 
		for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
		{
			auto OriginIndex3D = m_ObjectImage->TransformLinearIndexTo3DIndex<ScalarType>(k);
			this->EvaluateCandidateAtOrigin_3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
		}
	}
	else
	{
		DenseVector<ScalarType, 3> OriginIndex3D;
		m_CandidateOriginList_3DIndex.GetCol(k, OriginIndex3D);
		this->EvaluateCandidateAtOrigin_3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
	}
}


template<typename PixelType, typename ScalarType>
int_max PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::GetNumberOfThreadTobeCreated()
{
	return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(m_TotalCandidateOriginNumber, m_MaxNumberOfThread, 1);
}


template<typename PixelType, typename ScalarType>
ObjectArray<PixelType> PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::
GetPixelSetByObjectMaskAtOrigin_3DIndex(const ObjectMaskType& Mask, ScalarType x0, ScalarType y0, ScalarType z0)
{
	ObjectArray<PixelType> PixelSet;
	auto PixelNumber = Mask.PointSet.GetColNumber();
	ObjectArray.Resize(PixelNumber);
	if (Mask.Flag_PhysicalPosition == true)
	{
		auto Origin = m_ObjectImage->Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
		for (int_max k = 0; k < PixelNumber; ++k)
		{
			DenseVector<ScalarType, 3> Position;
			Mask.PointSet.GetCol(k, Position);
			Position += Origin;
			PixelSet[k] = m_ObjectImage->GetPixelAt3DPhysicalPosition(Position);
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
			PixelSet[k] = m_ObjectImage->GetPixelAt3DIndex(Index3D);
		}
	}
	return PixelSet;
}


template<typename PixelType, typename ScalarType>
ObjectArray<PixelType> PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>::
GetPixelSetByObjectMaskAtOrigin_3DPyhsicalPosition(const ObjectMaskType& Mask, ScalarType x0, ScalarType y0, ScalarType z0)
{
	auto Origin = m_ObjectImage->Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
	return this->GetPixelSetByObjectMaskAtOrigin_3DIndex(Mask, Origin[0], Origin[1], Origin[2]);
}


}// namespace mdk


#endif