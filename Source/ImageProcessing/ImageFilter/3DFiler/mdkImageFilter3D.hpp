#ifndef __mdkImageFilter3D_hpp
#define __mdkImageFilter3D_hpp

namespace mdk
{
template<typename InputImageType, typename OutputImageType, typename ScalarType>
ImageFilter3D<InputImageType, OutputImageType, ScalarType>::ImageFilter3D()
{
	this->Clear();
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
ImageFilter3D<InputImageType, OutputImageType, ScalarType>::~ImageFilter3D()
{
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::Clear()
{
	m_InputImage = nullptr;
	m_ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Nearest;
	m_ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Nearest;
	m_ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);
	
	m_PointList_3DPyhsicalPosition = nullptr;
	m_PointList_3DIndex_InputImage = nullptr;
	m_PointList_3DIndex_OutputImage.Clear();
	
	m_OutputPlaceOption = OutputPlaceOption_Of_ImageFilter3D::OutputImage;

	m_Flag_ScanWholeImageGrid = false;
	m_TotalNumberOfOutputPixelTobeProcessed = 0;
	m_MaxNumberOfThread = 1;

	m_OutputImage.Clear();
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetInputImage(const InputImageType* InputImage)
{
	m_InputImage = InputImage;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const Image3DInfo& Info)
{
	m_OutputImageInfo = Info;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const DenseVector<double, 3>& Origin,
																				    const DenseVector<double, 3>& Spacing,
																					const DenseVector<int_max, 3>& Size)
{
	m_OutputImageInfo.Origin = Origin;
	m_OutputImageInfo.Spacing = Spacing;
	m_OutputImageInfo.Size = Size;
	m_OutputImageInfo.Orientation.Resize(3, 3);
	m_OutputImageInfo.Orientation.FillDiagonal(1);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const DenseVector<double, 3>& Origin,
																					const DenseVector<double, 3>& Spacing,
																					const DenseVector<int_max, 3>& Size,
																					const DenseMatrix<double>& Orientation)
{
	m_OutputImageInfo.Origin = Origin;
	m_OutputImageInfo.Spacing = Spacing;
	m_OutputImageInfo.Size = Size;
	m_OutputImageInfo.Orientation = Orientation;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
Image3DInfo ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputImageInfo()
{
	return m_OutputImageInfo;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputAsImage()
{
	m_OutputPlaceOption = OutputPlaceOption_Of_ImageFilter3D::OutputImage;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputAsPixelArray()
{
	m_OutputPlaceOption = OutputPlaceOption_Of_ImageFilter3D::OutputPixelArray_DataArray;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::InitializeOutputImage()
{
	m_OutputImage.SetSize(m_OutputImageInfo.Size);
	m_OutputImage.SetSpacing(m_OutputImageInfo.Spacing);
	m_OutputImage.SetOrigin(m_OutputImageInfo.Origin);
	m_OutputImage.SetOrientation(m_OutputImageInfo.Orientation);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
OutputImageType* ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputImage()
{
	return &m_OutputImage;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::InitializeOutputPixelArray()
{
	int_max PixelNumberInArray = 0;

	if (m_PointList_3DIndex_OutputImage.IsEmpty() == false)
	{
		PixelNumberInArray = m_PointList_3DIndex_OutputImage.GetColNumber();
	}
	else if (m_PointList_3DIndex_InputImage != nullptr)
	{
		PixelNumberInArray = m_PointList_3DIndex_InputImage->GetColNumber();
	}
	else if (m_PointList_3DPyhsicalPosition != nullptr)
	{
		PixelNumberInArray = m_PointList_3DPyhsicalPosition->GetColNumber();
	}

	m_OutputPixelArray.FastResize(PixelNumberInArray);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
DataArray<typename OutputImageType::PixelType>* ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputPixelArray()
{
	return &m_OutputPixelArray;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* ListOf3DPyhsicalPosition)
{
	m_PointList_3DPyhsicalPosition = std::move(ListOf3DPyhsicalPosition);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DIndexInInputImage(const DenseMatrix<int_max>* ListOf3DIndex)
{
	m_PointList_3DIndex_InputImage = ListOf3DIndex;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DIndexInOutputImage(DenseMatrix<int_max> ListOf3DIndex)
{
	m_PointList_3DIndex_OutputImage = std::move(ListOf3DIndex);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption)
{
	m_ImageInterpolationOption = InputOption;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
Option_Of_Image3DInterpolation<typename InputImageType::PixelType>
ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetImageInterpolationOption()
{
	return m_ImageInterpolationOption;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetMaxNumberOfThread(int_max MaxNumber)
{
	m_MaxNumberOfThread = MaxNumber;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
int_max ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetMaxNumberOfThread_UserInput()
{
	return m_MaxNumberOfThread;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
bool ImageFilter3D<InputImageType, OutputImageType, ScalarType>::Update()
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
	if (m_TotalNumberOfOutputPixelTobeProcessed > 0)
	{ 
		ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex)
					  {this->Evaluate_in_a_thread(Index_start, Index_end, ThreadIndex); },
					  0, m_TotalNumberOfOutputPixelTobeProcessed - 1, m_MaxNumberOfThread, 1);
	}
	//-------------------------------------------------------------------------------
	if (this->Postprocess() == false)
	{
		return false;
	}
	//-------------
	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
bool ImageFilter3D<InputImageType, OutputImageType, ScalarType>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ ImageFilter3D::CheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ ImageFilter3D::CheckInput()")
		return false;
	}

	if (m_OutputPlaceOption == OutputPlaceOption_Of_ImageFilter3D::OutputImage)
	{
		auto OutputImageSize = m_OutputImageInfo.Size;
		auto PixelNumber = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];
		if (PixelNumber <= 0) // image may be spasre, do not use GetPixelNumber()
		{
			MDK_Error("Output Image size is 0 @ ImageFilter3D::CheckInput()")
			return false;
		}
	}
	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
bool ImageFilter3D<InputImageType, OutputImageType, ScalarType>::Preprocess()
{
	if (m_OutputImageInfo.Orientation.IsEmpty() == true)
	{
		m_OutputImageInfo.Orientation = m_InputImage->GetOrientation();
	}

	if (m_OutputPlaceOption == OutputPlaceOption_Of_ImageFilter3D::OutputImage)
	{
		m_OutputPixelArray.Clear();

		auto OutputImageSize = m_OutputImageInfo.Size;
		auto OutputImageSpacing = m_OutputImageInfo.Spacing;
		auto OutputImageOrigin = m_OutputImageInfo.Origin;
		auto PixelNumberOfOutputImage = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];

		this->InitializeOutputImage();

		if (m_PointList_3DIndex_OutputImage.IsEmpty() == true && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPyhsicalPosition == nullptr)
		{
			m_TotalNumberOfOutputPixelTobeProcessed = PixelNumberOfOutputImage;
			m_Flag_ScanWholeImageGrid = true;
		}
		else if (m_PointList_3DIndex_OutputImage.IsEmpty() == false && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPyhsicalPosition == nullptr)
		{
			m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DIndex_OutputImage.GetColNumber();
			m_Flag_ScanWholeImageGrid = false;
		}
		else if (m_PointList_3DIndex_OutputImage.IsEmpty() == true && m_PointList_3DIndex_InputImage != nullptr && m_PointList_3DPyhsicalPosition == nullptr)
		{
			m_PointList_3DIndex_OutputImage.Clear();
			m_PointList_3DIndex_OutputImage.ReserveCapacity(m_PointList_3DIndex_InputImage->GetSize());
			for (int_max k = 0; k < m_PointList_3DIndex_InputImage->GetColNumber(); ++k)
			{
				DenseVector<ScalarType, 3> Index3D_input;
				m_PointList_3DIndex_InputImage->GetCol(k, Index3D_input);
				auto Position = m_InputImage->Transform3DIndexTo3DPhysicalPosition<ScalarType>(Index3D_input);
				auto Index3D_output = m_OutputImage.Transform3DPhysicalPositionTo3DIndex(Position);
				auto xIndex = int_max(Index3D_output[0]); xIndex = std::max(xIndex, int_max(0)); xIndex = std::min(xIndex, OutputImageSize[0] - 1);
				auto yIndex = int_max(Index3D_output[1]); yIndex = std::max(yIndex, int_max(0)); yIndex = std::min(yIndex, OutputImageSize[1] - 1);
				auto zIndex = int_max(Index3D_output[2]); zIndex = std::max(zIndex, int_max(0)); zIndex = std::min(zIndex, OutputImageSize[2] - 1);
				m_PointList_3DIndex_OutputImage.AppendCol({ xIndex, yIndex, zIndex });
			}
			auto tempColIndexList = m_PointList_3DIndex_OutputImage.FindUniqueCol();
			m_PointList_3DIndex_OutputImage = m_PointList_3DIndex_OutputImage.GetSubMatrix(ALL, tempColIndexList);
			//---------------------------------------------------------------------------
			m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DIndex_OutputImage.GetColNumber();
			m_Flag_ScanWholeImageGrid = false;
		}
		else if (m_PointList_3DIndex_OutputImage.IsEmpty() == true && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPyhsicalPosition != nullptr)
		{
			m_PointList_3DIndex_OutputImage.ReserveCapacity(m_PointList_3DPyhsicalPosition->GetSize());
			for (int_max k = 0; k < m_PointList_3DPyhsicalPosition->GetColNumber(); ++k)
			{
				DenseVector<ScalarType, 3> Position, Index3D;
				m_PointList_3DPyhsicalPosition->GetCol(k, Position);
				Index3D = m_OutputImage.Transform3DPhysicalPositionTo3DIndex(Position);
				auto xIndex = int_max(Index3D[0]); xIndex = std::max(xIndex, int_max(0)); xIndex = std::min(xIndex, OutputImageSize[0] - 1);
				auto yIndex = int_max(Index3D[1]); yIndex = std::max(yIndex, int_max(0)); yIndex = std::min(yIndex, OutputImageSize[1] - 1);
				auto zIndex = int_max(Index3D[2]); zIndex = std::max(zIndex, int_max(0)); zIndex = std::min(zIndex, OutputImageSize[2] - 1);
				m_PointList_3DIndex_OutputImage.AppendCol({ xIndex, yIndex, zIndex });
			}
			auto tempColIndexList = m_PointList_3DIndex_OutputImage.FindUniqueCol();
			m_PointList_3DIndex_OutputImage = m_PointList_3DIndex_OutputImage.GetSubMatrix(ALL, tempColIndexList);
			//---------------------------------------------------------------------------
			m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DIndex_OutputImage.GetColNumber();
			m_Flag_ScanWholeImageGrid = false;
		}
		else
		{
			MDK_Error("Something is wrong @ ImageFilter3D::Preprocess()")
			return false;
		}
	}
	else // OutputPixelArray_DataArray or OutputPixelArray_OtherFormat
	{
		m_OutputImage.Clear();

		m_Flag_ScanWholeImageGrid = false;

		if (m_PointList_3DIndex_OutputImage.IsEmpty() == true && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPyhsicalPosition == nullptr)
		{
			m_TotalNumberOfOutputPixelTobeProcessed = 0;
		}
		else if (m_PointList_3DIndex_OutputImage.IsEmpty() == false && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPyhsicalPosition == nullptr)
		{
			m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DIndex_OutputImage.GetColNumber();
		}
		else if (m_PointList_3DIndex_OutputImage.IsEmpty() == true && m_PointList_3DIndex_InputImage != nullptr && m_PointList_3DPyhsicalPosition == nullptr)
		{
			m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DIndex_InputImage->GetColNumber();
		}
		else if (m_PointList_3DIndex_OutputImage.IsEmpty() == true && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPyhsicalPosition != nullptr)
		{
			m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DPyhsicalPosition->GetColNumber();
		}
		else
		{
			MDK_Error("Something is wrong @ ImageFilter3D::Preprocess()")
			return false;
		}

		if (m_OutputPlaceOption == OutputPlaceOption_Of_ImageFilter3D::OutputPixelArray_DataArray)
		{
			m_OutputPixelArray.FastResize(m_TotalNumberOfOutputPixelTobeProcessed);
		}
		else // if (m_OutputPlaceOption == OutputPlaceOption_Of_ImageFilter3D::OutputPixelArray_OtherFormat)
		{
			m_OutputPixelArray.Clear();
		}
	}
	//-------------
	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Evaluate_in_a_thread(int_max Index_start, int_max Index_end, int_max ThreadIndex)
{
	if (m_OutputPlaceOption == OutputPlaceOption_Of_ImageFilter3D::OutputImage)
	{
		if (m_Flag_ScanWholeImageGrid == true)
		{
			for (int_max k = Index_start; k <= Index_end; ++k)
			{
				auto Index3D = m_OutputImage.TransformLinearIndexTo3DIndex(k);
				auto Pos3D = m_OutputImage.Transform3DIndexTo3DPhysicalPosition(Index3D);
				m_OutputImage(Index3D[0], Index3D[1], Index3D[2]) = this->EvaluateAt3DPhysicalPosition(Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
			}
		}
		else
		{
			for (int_max k = Index_start; k <= Index_end; ++k)
			{
				DenseVector<int_max, 3> Index3D;
				m_PointList_3DIndex_OutputImage.GetCol(k, Index3D);
				auto Pos3D = m_OutputImage.Transform3DIndexTo3DPhysicalPosition(Index3D);
				m_OutputImage(Index3D[0], Index3D[1], Index3D[2]) = this->EvaluateAt3DPhysicalPosition(Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
			}
		}
	}
	else // OutputPixelArray_DataArray or OutputPixelArray_OtherFormat
	{
		if (m_OutputPlaceOption == OutputPlaceOption_Of_ImageFilter3D::OutputPixelArray_DataArray)
		{
			if (m_PointList_3DIndex_OutputImage.IsEmpty() == false)
			{
				for (int_max k = Index_start; k <= Index_end; ++k)
				{
					DenseVector<int_max, 3> Index3D;
					m_PointList_3DIndex_OutputImage.GetCol(k, Index3D);
					auto Pos3D = m_OutputImage.Transform3DIndexTo3DPhysicalPosition(Index3D);
					m_OutputPixelArray[k] = this->EvaluateAt3DPhysicalPosition(Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
				}
			}
			else if (m_PointList_3DIndex_InputImage != nullptr)
			{
				for (int_max k = Index_start; k <= Index_end; ++k)
				{
					DenseVector<int_max, 3> Index3D;
					m_PointList_3DIndex_InputImage->GetCol(k, Index3D);
					auto Pos3D = m_InputImage->Transform3DIndexTo3DPhysicalPosition(Index3D);
					m_OutputPixelArray[k] = this->EvaluateAt3DPhysicalPosition(Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);					 
				}
			}
			else if (m_PointList_3DPyhsicalPosition != nullptr)
			{
				for (int_max k = Index_start; k <= Index_end; ++k)
				{
					DenseVector<ScalarType, 3> Pos3D;
					m_PointList_3DPyhsicalPosition->GetCol(k, Pos3D);
					m_OutputPixelArray[k] = this->EvaluateAt3DPhysicalPosition(Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
				}
			}
		}
		else //if (m_OutputPlaceOption == OutputPlaceOption_Of_ImageFilter3D::OutputPixelArray_OtherFormat)
		{
			if (m_PointList_3DIndex_OutputImage.IsEmpty() == false)
			{
				for (int_max k = Index_start; k <= Index_end; ++k)
				{
					DenseVector<int_max, 3> Index3D;
					m_PointList_3DIndex_OutputImage.GetCol(k, Index3D);
					auto Pos3D = m_OutputImage.Transform3DIndexTo3DPhysicalPosition(Index3D);
					auto OutputPixel = this->EvaluateAt3DPhysicalPosition(Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
					this->StoreOutputPixelInPixelArrayOfOtherFormat(OutputPixel, k, ThreadIndex);
				}
			}
			else if (m_PointList_3DIndex_InputImage != nullptr)
			{
				for (int_max k = Index_start; k <= Index_end; ++k)
				{
					DenseVector<int_max, 3> Index3D;
					m_PointList_3DIndex_InputImage->GetCol(k, Index3D);
					auto Pos3D = m_InputImage->Transform3DIndexTo3DPhysicalPosition(Index3D);
					auto OutputPixel = this->EvaluateAt3DPhysicalPosition(Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
					this->StoreOutputPixelInPixelArrayOfOtherFormat(OutputPixel, k, ThreadIndex);
				}
			}
			else if (m_PointList_3DPyhsicalPosition != nullptr)
			{
				for (int_max k = Index_start; k <= Index_end; ++k)
				{
					DenseVector<ScalarType, 3> Pos3D;
					m_PointList_3DPyhsicalPosition->GetCol(k, Pos3D);
					auto OutputPixel = this->EvaluateAt3DPhysicalPosition(Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
					this->StoreOutputPixelInPixelArrayOfOtherFormat(OutputPixel, k, ThreadIndex);
				}
			}
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
int_max ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetNumberOfThreadTobeCreated()
{
	return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(m_TotalNumberOfOutputPixelTobeProcessed, m_MaxNumberOfThread, 1);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, ScalarType x0, ScalarType y0, ScalarType z0)
{
	auto MaskOriginOf3DIndex_Input = m_InputImage->Transform3DPhysicalPositionTo3DIndex(x0, y0, z0);
	auto PixelNumber = PointMask.GetColNumber();
	DataArray<PixelTypeForMask> PixelSet;
	PixelSet.Resize(PixelNumber);
	for (int_max k = 0; k < PixelNumber; ++k)
	{
		DenseVector<ScalarType, 3> Index3D;
		PointMask.GetCol(k, Index3D);
		Index3D += MaskOriginOf3DIndex_Input;
		PixelSet[k] = m_InputImage->GetPixelAt3DIndex<PixelTypeForMask>(Index3D, m_ImageInterpolationOption);
	}
	return PixelSet;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, const DenseVector<ScalarType, 3>& Position)
{
	return this->GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(PointMask, Position[0], Position[1], Position[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, ScalarType x0, ScalarType y0, ScalarType z0)
{
	auto PixelNumber = PointMask.GetColNumber();
	DataArray<PixelTypeForMask> PixelSet;
	PixelSet.Resize(PixelNumber);
	for (int_max k = 0; k < PixelNumber; ++k)
	{
		DenseVector<ScalarType, 3> Position;
		PointMask.GetCol(k, Position);
		Position[0] += x0;
		Position[1] += y0;
		Position[2] += z0;
		PixelSet[k] = m_InputImage->GetPixelAt3DPhysicalPosition<PixelTypeForMask>(Position, m_ImageInterpolationOption);
	}
	return PixelSet;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, const DenseVector<ScalarType, 3>& Position)
{
	return this->GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition(PointMask, Position[0], Position[1], Position[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0)
{
	auto MaskOriginOf3DPosition = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
	return this->GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(MaskOriginOf3DPosition);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, const DenseVector<int_max, 3>& Index3D)
{
	return this->GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(PointMask, Index3D[0], Index3D[1], Index3D[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0)
{
	auto MaskOriginOf3DPosition = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
	return this->GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition(MaskOriginOf3DPosition);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, const DenseVector<int_max, 3>& Index3D)
{
	return this->GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(PointMask, Index3D[0], Index3D[1], Index3D[2]);
}

}// namespace mdk

#endif