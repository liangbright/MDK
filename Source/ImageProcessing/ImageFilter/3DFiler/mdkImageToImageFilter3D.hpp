#ifndef __mdkImageToImageFilter3D_hpp
#define __mdkImageToImageFilter3D_hpp

namespace mdk
{
template<typename InputImageType, typename OutputImageType, typename ScalarType>
ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::ImageToImageFilter3D()
{
	this->Clear();
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::~ImageToImageFilter3D()
{
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::Clear()
{
	m_InputImage = nullptr;
	m_ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Nearest;
	m_ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Constant;
	m_ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);
	
	m_PointList_3DPyhsicalPosition = nullptr;
	m_PointList_3DIndex_InputImage = nullptr;
	m_PointList_3DIndex_OutputImage.Clear();
	
	m_Flag_ScanWholeImageGrid = false;
	m_TotalNumberOfOutputPixelTobeProcessed = 0;
	m_MaxNumberOfThread = 1;

	m_OutputImage.Clear();
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetInputImage(const InputImageType* InputImage)
{
	m_InputImage = InputImage;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const Image3DInfo& Info)
{
	m_OutputImage.SetOrigin(Info.Origin);
	m_OutputImage.SetSpacing(Info.Spacing);
	m_OutputImage.SetSize(Info.Size);
	m_OutputImage.SetOrientation(Info.Orientation);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const DenseVector<double, 3>& Origin,
																						   const DenseVector<double, 3>& Spacing,
																						   const DenseVector<int_max, 3>& Size)
{
	m_OutputImage.SetOrigin(Origin);
	m_OutputImage.SetSpacing(Spacing);
	m_OutputImage.SetSize(Size);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const DenseVector<double, 3>& Origin,
																						   const DenseVector<double, 3>& Spacing,
																						   const DenseVector<int_max, 3>& Size,
																						   const DenseMatrix<double>& Orientation)
{
	m_OutputImage.SetOrigin(Origin);
	m_OutputImage.SetSpacing(Spacing);
	m_OutputImage.SetSize(Size);
	m_OutputImage.SetOrientation(Orientation);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
Image3DInfo ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputImageInfo()
{
	return m_OutputImage.GetInfo();
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
OutputImageType* ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputImage()
{
	return &m_OutputImage;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* ListOf3DPyhsicalPosition)
{
	m_PointList_3DPyhsicalPosition = std::move(ListOf3DPyhsicalPosition);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetListOf3DIndexInInputImage(const DenseMatrix<int_max>* ListOf3DIndex)
{
	m_PointList_3DIndex_InputImage = ListOf3DIndex;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetListOf3DIndexInOutputImage(DenseMatrix<int_max> ListOf3DIndex)
{
	m_PointList_3DIndex_OutputImage = std::move(ListOf3DIndex);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::
SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption)
{
	m_ImageInterpolationOption = InputOption;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
Option_Of_Image3DInterpolation<typename InputImageType::PixelType>
ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetImageInterpolationOption()
{
	return m_ImageInterpolationOption;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetThreadNumber(int_max MaxNumber)
{
	m_MaxNumberOfThread = MaxNumber;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
bool ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::Update()
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
	
	ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex)
	              {this->Evaluate_in_a_thread(Index_start, Index_end, ThreadIndex); },
				  0, m_TotalNumberOfOutputPixelTobeProcessed - 1, m_MaxNumberOfThread, 1);

	//-------------------------------------------------------------------------------
	if (this->Postprocess() == false)
	{
		return false;
	}
	//-------------
	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
bool ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ ImageToImageFilter3D::CheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ ImageToImageFilter3D::CheckInput()")
		return false;
	}

	auto OutputImageSize = m_OutputImage.GetSize();
	auto PixelNumber = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];
	if (PixelNumber <= 0) // image may be spasre, do not use GetPixelNumber()
	{
		MDK_Error("Output Image is Empty @ ImageToImageFilter3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
bool ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::Preprocess()
{
	DenseVector<int_max, 3> OutputImageSize;
	DenseVector<double, 3> OutputImageSpacing;
	DenseVector<double, 3> OutputImageOrigin;
	int_max TotalOutputPixelNumber = 0;

	if (m_InputImage->GetOrientation().IsEmpty() == true)
	{
		m_OutputImage.SetOrientation(m_InputImage->GetOrientation());
	}

	TotalOutputPixelNumber = m_OutputImage.GetPixelNumber();
	OutputImageSize = m_OutputImage.GetSize();
	OutputImageSpacing = m_OutputImage.GetSpacing();
	OutputImageOrigin = m_OutputImage.GetOrigin();

	if (m_PointList_3DIndex_OutputImage.IsEmpty() == true && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPyhsicalPosition == nullptr)
	{
		m_TotalNumberOfOutputPixelTobeProcessed = TotalOutputPixelNumber;
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
			auto xIndex = int_max(Index3D_output[0]); xIndex = std::max(xIndex, int_max(0)); xIndex = std::min(xIndex, OutputImageSize[0]);
			auto yIndex = int_max(Index3D_output[1]); yIndex = std::max(yIndex, int_max(0)); yIndex = std::min(yIndex, OutputImageSize[1]);
			auto zIndex = int_max(Index3D_output[2]); zIndex = std::max(zIndex, int_max(0)); zIndex = std::min(zIndex, OutputImageSize[2]);
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
			auto xIndex = int_max(Index3D[0]); xIndex = std::max(xIndex, int_max(0)); xIndex = std::min(xIndex, OutputImageSize[0]);
			auto yIndex = int_max(Index3D[1]); yIndex = std::max(yIndex, int_max(0)); yIndex = std::min(yIndex, OutputImageSize[1]);
			auto zIndex = int_max(Index3D[2]); zIndex = std::max(zIndex, int_max(0)); zIndex = std::min(zIndex, OutputImageSize[2]);
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
		MDK_Error("Something is wrong @ ImageToImageFilter3D::Preprocess()")
		return false;
	}
	//-------------
	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Evaluate_in_a_thread(int_max Index_start, int_max Index_end, int_max ThreadIndex)
{
	if (m_Flag_ScanWholeImageGrid == true)
	{
		for (int_max k = Index_start; k <= Index_end; ++k)
		{
			auto Index3D = m_OutputImage.TransformLinearIndexTo3DIndex(k);
			this->EvaluateAt3DIndexInOutputImage(Index3D[0], Index3D[1], Index3D[2], ThreadIndex);
		}
	}
	else
	{
		for (int_max k = Index_start; k <= Index_end; ++k)
		{
			DenseVector<int_max, 3> Index3D;
			m_PointList_3DIndex_OutputImage.GetCol(k, Index3D);
			this->EvaluateAt3DIndexInOutputImage(Index3D[0], Index3D[1], Index3D[2], ThreadIndex);
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
int_max ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetNumberOfThreadTobeCreated()
{
	return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(m_TotalNumberOfOutputPixelTobeProcessed, m_MaxNumberOfThread, 1);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
DataArray<PixelTypeForMask> ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0)
{
	auto MaskOriginOf3DPosition = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
	auto MaskOriginOf3DIndex_Input = m_InputImage->Transform3DPhysicalPositionTo3DIndex(MaskOriginOf3DPosition);

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
DataArray<PixelTypeForMask> ImageToImageFilter3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0)
{
	auto MaskOriginOf3DPosition = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);

	auto PixelNumber = PointMask.GetColNumber();
	DataArray<PixelTypeForMask> PixelSet;
	PixelSet.Resize(PixelNumber);
	for (int_max k = 0; k < PixelNumber; ++k)
	{
		DenseVector<ScalarType, 3> Position;
		PointMask.GetCol(k, Position);
		Position += MaskOriginOf3DPosition;
		PixelSet[k] = m_InputImage->GetPixelAt3DPhysicalPosition<PixelTypeForMask>(Position, m_ImageInterpolationOption);
	}
	return PixelSet;
}


}// namespace mdk

#endif