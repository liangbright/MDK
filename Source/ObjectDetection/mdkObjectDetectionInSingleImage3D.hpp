#ifndef __mdkObjectDetectionInSingleImage3D_hpp
#define __mdkObjectDetectionInSingleImage3D_hpp

namespace mdk
{
template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::ObjectDetectionInSingleImage3D()
{
	this->Clear();
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::~ObjectDetectionInSingleImage3D()
{
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::Clear()
{
	m_ObjectImage = nullptr;
	m_ObjectImageInterpolationOption.MethodType = ObjectImageInterpolationMethodEnum::Nearest;
	m_ObjectImageInterpolationOption.BoundaryOption = ObjectImageInterpolationBoundaryOptionEnum::Constant;
	m_ObjectImageInterpolationOption.Pixel_OutsideImage = ObjectImagePixelType(0);

	m_CandidateOriginList_3DIndex.Clear();
	m_CandidateOriginList_3DPyhsicalPosition = nullptr;

	m_MaxNumberOfThread = 1;

	m_TotalCandidateOriginNumber = 0;
	m_Flag_ScanWholeImageGrid = false;

	m_Flag_StoreObjectMembershipInSparseImage = false;
	m_ObjectMembershipDenseImage.Clear();
	m_ObjectMembershipSparseImage.Clear();
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::ClearOutput()
{
	m_ObjectMembershipDenseImage.Clear();
	m_ObjectMembershipSparseImage.Clear();
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetObjectImage(const DenseImage3D<ObjectImagePixelType>* InputImage)
{
	m_ObjectImage = InputImage;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetObjectImageInterpolationOption(const ObjectImageInterpolationOptionType& InputOption)
{
	m_ObjectImageInterpolationOption = InputOption;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
Option_Of_Image3DInterpolation<ObjectImagePixelType>
ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetObjectImageInterpolationOption()
{
	return m_ObjectImageInterpolationOption;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
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
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
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
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetCandidateOriginListOf3DIndex(DenseMatrix<int_max> Input3DIndexList)
{
	m_CandidateOriginList_3DIndex = std::move(Input3DIndexList);
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList)
{
	m_CandidateOriginList_3DPyhsicalPosition = Input3DPositionList;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::SetThreadNumber(int_max MaxNumber)
{
	m_MaxNumberOfThread = MaxNumber;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
bool ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::Update()
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
ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetObjectMembershipDenseImage()
{
	return &m_ObjectMembershipDenseImage;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
SparseImage3D<ObjectMembershipImagePixelType>*
ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetObjectMembershipSparseImage()
{
	return &m_ObjectMembershipSparseImage;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
bool ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::CheckInput()
{
	if (m_ObjectImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ ObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_ObjectImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ ObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	if (m_CandidateOriginList_3DPyhsicalPosition != nullptr && m_CandidateOriginList_3DIndex.IsEmpty() == false)
	{
		MDK_Error("CandidateOriginList can not be both 3DPyhsicalPosition and 3DIndex @ ObjectDetectionInSingleImage3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
bool ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::Preprocess()
{
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
void ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
EvaluateCandidateAtMultipleOrigin_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanWholeImageGrid == true)
	{
		if (m_Flag_StoreObjectMembershipInSparseImage = true)
		{
			for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
			{
				auto OriginIndex3D = m_ObjectMembershipSparseImage.TransformLinearIndexTo3DIndex(k);
				this->EvaluateCandidateAtOriginOf3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
			}
		}
		else
		{
			for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
			{
				auto OriginIndex3D = m_ObjectMembershipDenseImage.TransformLinearIndexTo3DIndex(k);
				this->EvaluateCandidateAtOriginOf3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
			}
		}
	}
	else
	{
		for (int_max k = OriginIndex_start; k <= OriginIndex_end; ++k)
		{
			DenseVector<int_max, 3> OriginIndex3D;
			m_CandidateOriginList_3DIndex.GetCol(k, OriginIndex3D);
			this->EvaluateCandidateAtOriginOf3DIndex(OriginIndex3D[0], OriginIndex3D[1], OriginIndex3D[2], ThreadIndex);
		}
	}
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
int_max ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::GetNumberOfThreadTobeCreated()
{
	return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(m_TotalCandidateOriginNumber, m_MaxNumberOfThread, 1);
}


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
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


template<typename ObjectImagePixelType, typename ObjectMembershipImagePixelType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ObjectDetectionInSingleImage3D<ObjectImagePixelType, ObjectMembershipImagePixelType, ScalarType>::
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