#ifndef __mdkPartBasedObjectDetectionInSingleImage3D_hpp
#define __mdkPartBasedObjectDetectionInSingleImage3D_hpp

namespace mdk
{

template<typename T1, typename T2, typename T3, typename T4>
PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::PartBasedObjectDetectionInSingleImage3D()
{
	this->Clear();
}


template<typename T1, typename T2, typename T3, typename T4>
PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::~PartBasedObjectDetectionInSingleImage3D()
{
}


template<typename T1, typename T2, typename T3, typename T4>
PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::Clear()
{
	m_ObjectImage = nullptr;
	m_ObjectImageInterpolationOption.MethodType = ObjectImageInterpolationMethodEnum::Nearest;
	m_ObjectImageInterpolationOption.BoundaryOption = ObjectImageInterpolationBoundaryOptionEnum::Constant;
	m_ObjectImageInterpolationOption.Pixel_OutsideImage = ObjectImagePixelType(0);

	m_ObjectPartNumber = 0;
	m_ObjectPartMembershipDenseImageList.Clear();
	m_ObjectPartMembershipSparseImageList.Clear();

	m_CandidateOriginList_3DPyhsicalPosition = nullptr;
	m_CandidateOriginList_3DIndex.Clear();

	m_TotalCandidateOriginNumber = 0;
	m_Flag_ScanWholeImageGrid = false;

	m_MaxNumberOfThread = 1;

	m_Flag_StoreObjectMembershipInSparseImage = false;
	m_ObjectMembershipDenseImage.Clear();
	m_ObjectMembershipSparseImage.Clear();
}


template<typename T1, typename T2, typename T3, typename T4>
PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::ClearOutput()
{
	m_ObjectMembershipDenseImage.Clear();
	m_ObjectMembershipSparseImage.Clear();
}


template<typename T1, typename T2, typename T3, typename T4>
PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::SetObjectImage(const Image3D<PixelType>* ObjectImage)
{
	m_ObjectImage = ObjectImage;
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::SetObjectImageInterpolationOption(const ObjectImageInterpolationOptionType& InputOption)
{
	m_ObjectImageInterpolationOption = InputOption;
}


template<typename T1, typename T2, typename T3, typename T4>
Option_Of_Image3DInterpolation<T1>
PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::GetObjectImageInterpolationOption()
{
	return m_ObjectImageInterpolationOption;
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::
SetupObjectMembershipDenseImage(const DenseVector<ScalarType, 3>& Origin,
	 			                const DenseVector<ScalarType, 3>& Spacing,
						        const DenseVector<ScalarType, 3>& Size)
{
	m_ObjectMembershipDenseImage.SetOrigin(Origin);
	m_ObjectMembershipDenseImage.SetSpacing(Spacing);
	m_ObjectMembershipDenseImage.SetSize(Size);
	m_Flag_StoreObjectMembershipInSparseImage = false;
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::
SetupObjectMembershipSparseImage(const DenseVector<ScalarType, 3>& Origin,
								 const DenseVector<ScalarType, 3>& Spacing,
								 const DenseVector<ScalarType, 3>& Size)
{
	m_ObjectMembershipSparseImage.SetOrigin(Origin);
	m_ObjectMembershipSparseImage.SetSpacing(Spacing);
	m_ObjectMembershipSparseImage.SetSize(Size);
	m_Flag_StoreObjectMembershipInSparseImage = true;
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::SetObjectPartNumber(int_max PartNumber)
{
	m_ObjectPartNumber = PartNumber; // number of parts, NOT PartIndex	

	if (m_ObjectPartMembershipDenseImageList.IsEmpty() == true)
	{		
		m_ObjectPartMembershipDenseImageList.Resize(m_ObjectPartNumber);
		m_ObjectPartMembershipDenseImageList.Fill(nullptr);
	}
	else
	{
		auto OldNumber = m_ObjectPartMembershipDenseImageList.GetLength();
		m_ObjectPartMembershipDenseImageList.Resize(m_ObjectPartNumber);
		for (int_max k = OldNumber; k < m_ObjectPartNumber; ++k)
		{
			m_ObjectPartMembershipDenseImageList[k] = nullptr;
		}
	}

	if (m_ObjectPartMembershipSparseImageList.IsEmpty() == true)
	{
		m_ObjectPartMembershipSparseImageList.Resize(m_ObjectPartNumber);
		m_ObjectPartMembershipSparseImageList.Fill(nullptr);
	}
	else
	{
		auto OldNumber = m_ObjectPartMembershipSparseImageList.GetLength();
		m_ObjectPartMembershipSparseImageList.Resize(m_ObjectPartNumber);
		for (int_max k = OldNumber; k < m_ObjectPartNumber; ++k)
		{
			m_ObjectPartMembershipSparseImageList[k] = nullptr;
		}
	}
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::
SetObjectPartMembershipDenseImage(int_max PartIndex, const Image3D<ObjectPartMembershipImagePixelType>* MembershipImage)
{
	if (PartIndex >= m_ObjectPartMembershipDenseImageList.GetLength())
	{
		MDK_Error("PartIndex >= m_ObjectPartNumber @ PartBasedObjectDetectionInSingleImage3D::SetObjectPartMembershipDenseImage(...)")
		return;
	}
	m_ObjectPartMembershipDenseImageList[PartIndex] = MembershipImage;
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::
SetObjectPartMembershipSparseImage(int_max PartIndex, const DenseImage3D<ObjectPartMembershipImagePixelType>* MembershipImage)
{
	if (PartIndex >= m_ObjectPartMembershipSparseImageList.GetLength())
	{
		MDK_Error("PartIndex >= m_ObjectPartNumber @ PartBasedObjectDetectionInSingleImage3D::SetObjectPartMembershipSparseImage(...)")
		return;
	}
	m_ObjectPartMembershipSparseImageList[PartIndex] = MembershipImage;
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::SetCandidateOriginListOf3DIndex(DenseMatrix<int_max> Input3DIndexList)
{
	m_CandidateOriginList_3DIndex = std::move(Input3DIndexList);
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList)
{
	m_CandidateOriginList_3DPyhsicalPosition = Input3DPositionList;
}


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::SetThreadNumber(int_max MaxNumber)
{
	m_MaxNumberOfThread = MaxNumber;
}


template<typename T1, typename T2, typename T3, typename T4>
bool PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::Update()
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
				  {this->EvaluateCandidateAtMultipleOrigin_in_a_thread(OriginIndex_start, OriginIndex_end, ThreadIndex);},
				  0, m_TotalCandidateOriginNumber - 1, m_MaxNumberOfThread, 1);

	//-------------------------------------------------------------------------------
	if (this->Postprocess() == false)
	{
		return false;
	}
	//-----------
	return true;
}


template<typename T1, typename T2, typename T3, typename T4>
DenseImage3D<T3>* PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::GetObjectMembershipDenseImage()
{
	return &m_ObjectMembershipDenseImage;
}


template<typename T1, typename T2, typename T3, typename T4>
SparseImage3D<T3>* PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::GetObjectMembershipSparseImage()
{
	return &m_ObjectMembershipSparseImage;
}


template<typename T1, typename T2, typename T3, typename T4>
bool PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::CheckInput()
{
	if (m_ObjectImage == nullptr)
	{
		MDK_Error("ObjectImage is Empty (nullptr) @ PartBasedObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_ObjectImage->IsEmpty() == true)
	{
		MDK_Error("ObjectImage is Empty @ PartBasedObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_CandidateOriginList_3DPyhsicalPosition != nullptr && m_CandidateOriginList_3DIndex.IsEmpty() == false)
	{
		MDK_Error("CandidateOriginList can not be both 3DPyhsicalPosition and 3DIndex @ PartBasedObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename T1, typename T2, typename T3, typename T4>
bool PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::Preprocess()
{
	DenseVector<int_max, 3> OutputImageSize;
	DenseVector<int_max, 3> OutputImageSpacing;
	DenseVector<int_max, 3> OutputImageOrigin;
	int_max TotalOutputPixelNumber = 0;

	if (m_Flag_StoreObjectMembershipInSparseImage == true)
	{
		if (m_ObjectMembershipSparseImage.IsEmpty() == true)
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


template<typename T1, typename T2, typename T3, typename T4>
void PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::
EvaluateCandidateAtMultipleOrigin_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanWholeImageGrid == true)
	{
		if (m_Flag_StoreObjectMembershipInSparseImage = true)
		{
			for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
			{
				auto OriginIndex3D = m_ObjectMembershipSparseImage.TransformLinearIndexTo3DIndex<ScalarType>(k);
				this->EvaluateCandidateAtOriginOf3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
			}
		}
		else
		{
			for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
			{
				auto OriginIndex3D = m_ObjectMembershipDenseImage.TransformLinearIndexTo3DIndex<ScalarType>(k);
				this->EvaluateCandidateAtOriginOf3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
			}
		}
	}
	else
	{
		DenseVector<ScalarType, 3> OriginIndex3D;
		m_CandidateOriginList_3DIndex.GetCol(k, OriginIndex3D);
		this->EvaluateCandidateAtOriginOf3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
	}
}


template<typename T1, typename T2, typename T3, typename T4>
int_max PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::GetNumberOfThreadTobeCreated()
{
	return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(m_TotalCandidateOriginNumber, m_MaxNumberOfThread, 1);
}


template<typename T1, typename T2, typename T3, typename T4>
template<typename PixelTypeForMask = T1>
DataArray<PixelTypeForMask> PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::
GetObjectImagePixelSetByPointMaskOf3DIndex_AtOriginOf3DIndex(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0)
{
	auto PixelNumber = PointMask.GetColNumber();
	DataArray<PixelTypeForMask> PixelSet;
	PixelSet.Resize(PixelNumber);
	for (int_max k = 0; k < PixelNumber; ++k)
	{
		DenseVector<ScalarType, 3> Index3D;
		PointMask.GetCol(k, Index3D);
		Index3D[0] += x0;
		Index3D[1] += y0;
		Index3D[2] += z0;
		PixelSet[k] = m_ObjectImage->GetPixelAt3DIndex<PixelTypeForMask>(Index3D, m_ObjectImageInterpolationOption);
	}
	return PixelSet;
}


template<typename T1, typename T2, typename T3, typename T4>
template<typename PixelTypeForMask = T1>
DataArray<PixelTypeForMask> PartBasedObjectDetectionInSingleImage3D<T1, T2, T3, T4>::
GetObjectImagePixelSetByPointMaskOf3DPyhsicalPosition_AtOriginOf3DIndex(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0)
{
	auto PixelNumber = PointMask.GetColNumber();
	DataArray<PixelTypeForMask> PixelSet;
	PixelSet.Resize(PixelNumber);
	auto Origin = m_ObjectImage->Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
	for (int_max k = 0; k < PixelNumber; ++k)
	{
		DenseVector<ScalarType, 3> Position;
		PointMask.GetCol(k, Position);
		Position += Origin;
		PixelSet[k] = m_ObjectImage->GetPixelAt3DPhysicalPosition<PixelTypeForMask>(Position, m_ObjectImageInterpolationOption);
	}
	return PixelSet;
}

}// namespace mdk


#endif