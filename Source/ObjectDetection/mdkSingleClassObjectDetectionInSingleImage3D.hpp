#ifndef __mdkSingleClassObjectDetectionInSingleImage3D_hpp
#define __mdkSingleClassObjectDetectionInSingleImage3D_hpp

namespace mdk
{
template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::SingleClassObjectDetectionInSingleImage3D()
{
	this->Clear();
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::~SingleClassObjectDetectionInSingleImage3D()
{
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::Clear()
{
	m_ObjectImage = nullptr;
	m_ObjectImageInterpolationOption.MethodType = ObjectImageInterpolationMethodEnum::Nearest;
	m_ObjectImageInterpolationOption.BoundaryOption = ObjectImageInterpolationBoundaryOptionEnum::Constant;
	m_ObjectImageInterpolationOption.Pixel_OutsideImage = ObjectImagePixelType(0);

	m_CandidateOriginList_3DIndex.Clear();
	m_CandidateOriginList_3DPyhsicalPosition = nullptr;
	m_ObjectMaskList = nullptr;

	m_MaxNumberOfThread = 1;

	m_NOBoundCheckRegionList_3DIndex.Clear();
	m_TotalCandidateOriginNumber = 0;
	m_Flag_ScanWholeImageGrid = false;

	m_Flag_StoreObjectMembershipInSparseImage = false;
	m_ObjectMembershipDenseImage.Clear();
	m_ObjectMembershipSparseImage.Clear();
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::ClearOutput()
{
	m_ObjectMembershipDenseImage.Clear();
	m_ObjectMembershipSparseImage.Clear();
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetObjectImage(const DenseImage3D<ObjectImagePixelType>* InputImage)
{
	m_ObjectImage = InputImage;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetObjectImageInterpolationOption(const ObjectImageInterpolationOptionType& InputOption)
{
	m_ObjectImageInterpolationOption = InputOption;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
Option_Of_Image3DInterpolation<ObjectImagePixelType>
SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetObjectImageInterpolationOption()
{
	return m_ObjectImageInterpolationOption;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetupObjectMembershipDenseImage(const DenseVector<ScalarType, 3>& Origin,
	 			                const DenseVector<ScalarType, 3>& Spacing,
						        const DenseVector<ScalarType, 3>& Size)
{
	m_ObjectMembershipDenseImage.SetOrigin(Origin);
	m_ObjectMembershipDenseImage.SetSpacing(Spacing);
	m_ObjectMembershipDenseImage.SetSize(Size);
	m_Flag_StoreObjectMembershipInSparseImage = false;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetupObjectMembershipSparseImage(const DenseVector<ScalarType, 3>& Origin,
								 const DenseVector<ScalarType, 3>& Spacing,
								 const DenseVector<ScalarType, 3>& Size)
{
	m_ObjectMembershipSparseImage.SetOrigin(Origin);
	m_ObjectMembershipSparseImage.SetSpacing(Spacing);
	m_ObjectMembershipSparseImage.SetSize(Size);
	m_Flag_StoreObjectMembershipInSparseImage = true;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetCandidateOriginListOf3DIndex(DenseMatrix<int_max> Input3DIndexList)
{
	m_CandidateOriginList_3DIndex = std::move(Input3DIndexList);
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList)
{
	m_CandidateOriginList_3DPyhsicalPosition = Input3DPositionList;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::SetObjectMaskList(const DataArray<ObjectMaskType>* InputMaskList)
{
	m_ObjectMaskList = InputMaskList;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
const DataArray<ObjectMask_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>*
SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetObjectMaskList()
{
	return m_ObjectMaskList;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::SetThreadNumber(int_max MaxNumber)
{
	m_MaxNumberOfThread = MaxNumber;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::Update()
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


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
DenseImage3D<ObjectMembershipImagePixelType>* 
SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetObjectMembershipDenseImage()
{
	return &m_ObjectMembershipDenseImage;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
SparseImage3D<ObjectMembershipImagePixelType>*
SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetObjectMembershipSparseImage()
{
	return &m_ObjectMembershipSparseImage;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::CheckInput()
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


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::Preprocess()
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

	DenseVector<int_max, 3> OutputImageSize;
	DenseVector<double, 3> OutputImageSpacing;
	DenseVector<double, 3> OutputImageOrigin;
	int_max TotalOutputPixelNumber = 0;

	if (m_Flag_StoreObjectMembershipInSparseImage == true)
	{
		OutputImageSize = m_ObjectMembershipSparseImage.GetSize();
		TotalOutputPixelNumber = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];
		if (TotalOutputPixelNumber <= 0)// do not if (use m_ObjectMembershipSparseImage.IsEmpty() == true)
		{
			m_ObjectMembershipSparseImage.SetOrigin(m_ObjectImage->GetOrigin());
			m_ObjectMembershipSparseImage.SetSpacing(m_ObjectImage->GetSpacing());
			m_ObjectMembershipSparseImage.SetSize(m_ObjectImage->GetSize());
		}
		m_ObjectMembershipSparseImage.SetOrientation(m_ObjectImage->GetOrientation());
		OutputImageSize = m_ObjectMembershipSparseImage.GetSize();
		OutputImageSpacing = m_ObjectMembershipSparseImage.GetSpacing();
		OutputImageOrigin = m_ObjectMembershipSparseImage.GetOrigin();
		TotalOutputPixelNumber = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];
	}
	else
	{
		if (m_ObjectMembershipDenseImage.IsEmpty() == true)
		{
			m_ObjectMembershipDenseImage.SetOrigin(m_ObjectImage->GetOrigin());
			m_ObjectMembershipDenseImage.SetSpacing(m_ObjectImage->GetSpacing());
			m_ObjectMembershipDenseImage.SetSize(m_ObjectImage->GetSize());
		}
		m_ObjectMembershipDenseImage.SetOrientation(m_ObjectImage->GetOrientation());
		TotalOutputPixelNumber = m_ObjectMembershipDenseImage.GetPixelNumber();
		OutputImageSize = m_ObjectMembershipDenseImage.GetSize();
		OutputImageSpacing = m_ObjectMembershipDenseImage.GetSpacing();
		OutputImageOrigin = m_ObjectMembershipDenseImage.GetOrigin();
	}

	if (m_CandidateOriginList_3DIndex.IsEmpty() == true && m_CandidateOriginList_3DPyhsicalPosition == nullptr)
	{
		m_TotalCandidateOriginNumber = TotalOutputPixelNumber;
		m_Flag_ScanWholeImageGrid = true;
	}
	else if (m_CandidateOriginList_3DIndex.IsEmpty() == false && m_CandidateOriginList_3DPyhsicalPosition == nullptr)
	{
		m_TotalCandidateOriginNumber = m_CandidateOriginList_3DIndex.GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}
	else if (m_CandidateOriginList_3DIndex.IsEmpty() == true && m_CandidateOriginList_3DPyhsicalPosition != nullptr)
	{
		m_CandidateOriginList_3DIndex.ReserveCapacity(m_CandidateOriginList_3DPyhsicalPosition->GetSize());
		for (int_max k = 0; k < m_CandidateOriginList_3DPyhsicalPosition->GetColNumber(); ++k)
		{
			DenseVector<ScalarType, 3> Position, Index3D;
			m_CandidateOriginList_3DPyhsicalPosition->GetCol(k, Position);
			Index3D[0] = (Position[0] - ScalarType(OutputImageOrigin[0])) / ScalarType(OutputImageSpacing[0]);
			Index3D[1] = (Position[1] - ScalarType(OutputImageOrigin[1])) / ScalarType(OutputImageSpacing[1]);
			Index3D[2] = (Position[2] - ScalarType(OutputImageOrigin[2])) / ScalarType(OutputImageSpacing[2]);
			auto xIndex = int_max(Index3D[0]); xIndex = std::max(xIndex, int_max(0)); xIndex = std::min(xIndex, OutputImageSize[0]);
			auto yIndex = int_max(Index3D[1]); yIndex = std::max(yIndex, int_max(0)); yIndex = std::min(yIndex, OutputImageSize[1]);
			auto zIndex = int_max(Index3D[2]); zIndex = std::max(zIndex, int_max(0)); zIndex = std::min(zIndex, OutputImageSize[2]);
			m_CandidateOriginList_3DIndex.AppendCol({ xIndex, yIndex, zIndex });
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


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
EvaluateCandidateAtMultipleOrigin_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanWholeImageGrid == true)
	{
		if (m_Flag_StoreObjectMembershipInSparseImage = true)
		{
			for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
			{
				auto OriginIndex3D = m_ObjectMembershipSparseImage.TransformLinearIndexTo3DIndex(k);
				this->EvaluateCandidateAtOrigin_3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
			}
		}
		else
		{
			for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
			{
				auto OriginIndex3D = m_ObjectMembershipDenseImage.TransformLinearIndexTo3DIndex(k);
				this->EvaluateCandidateAtOrigin_3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
			}
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


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
int_max SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetNumberOfThreadTobeCreated()
{
	return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(m_TotalCandidateOriginNumber, m_MaxNumberOfThread, 1);
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
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
			PixelSet[k] = m_ObjectImage->GetPixelAt3DPhysicalPosition<PixelTypeForMask>(Position, m_ObjectImageInterpolationOption);
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
			PixelSet[k] = m_ObjectImage->GetPixelAt3DIndex<PixelTypeForMask>(Index3D, m_ObjectImageInterpolationOption);
		}
	}
	return PixelSet;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DIndex()
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


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
bool SingleClassObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
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


}// namespace mdk

#endif