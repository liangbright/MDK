#ifndef mdk_ImageFilter2D_hpp
#define mdk_ImageFilter2D_hpp

namespace mdk
{

template<typename InputImageType, typename OutputImageType, typename ScalarType>
ImageFilter2D<InputImageType, OutputImageType, ScalarType>::ImageFilter2D()
{
	this->Clear();
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
ImageFilter2D<InputImageType, OutputImageType, ScalarType>::~ImageFilter2D()
{
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::Clear()
{
	m_InputImage = nullptr;
	m_InputImageInfo.Clear();

	m_ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Nearest;
	m_ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Replicate;
	m_ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);

	m_PointList_3DWorldPosition = nullptr;
	m_PointList_2DIndex_InputImage = nullptr;
	m_PointList_2DPosition_InputImage = nullptr;
	m_PointList_2DPosition_OutputImage = nullptr;
	m_PointList_2DIndex_OutputImage = nullptr;
	
	m_Flag_ScanOutputImageGrid = false;
	m_OutputPixelCount = 0;
	m_MaxThreadCount = 1;

	m_Flag_EnableOutputImage = true;
	m_Flag_EnableOutputPixelArray = false;
	m_Flag_EnableOutputToOtherPlace = false;

	m_Flag_Input_Output_SameOrientation = false;

	m_OutputImageInfo.Clear();
	m_OutputImage.Clear();
	m_OutputPixelArray.Clear();

	m_CoordinateSystemForEvaluation = CoordinateSystemForEvaluation::UNKNOWN;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetInputImage(const InputImageType* InputImage)
{
	m_InputImage = InputImage;
	if (m_InputImage != nullptr)
	{
		m_InputImageInfo = m_InputImage->GetInfo();
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const ImageInfo2D& Info)
{
	m_OutputImageInfo = Info;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const DenseVector<double, 3>& Origin,
																					const DenseVector<double, 2>& Spacing,
																					const DenseVector<int_max, 2>& Size,
																					const DenseMatrix<double>& Orientation)
{
	m_OutputImageInfo.Origin = Origin;
	m_OutputImageInfo.Spacing = Spacing;
	m_OutputImageInfo.Size = Size;
	m_OutputImageInfo.Orientation = Orientation;
	m_OutputImageInfo.UpdateTransformMatrix();
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 2>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ ImageFilter2D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 0 || Ly <= 0)
	{
		MDK_Error("Invalid input @ ImageFilter2D::SetOutputImageInfoBySize(...)")
		return;
	}

	ImageInfo2D Info;
	Info.Size[0] = Lx;
	Info.Size[1] = Ly;

	auto Size_input = m_InputImage->GetSize();
	auto Spacing_input = m_InputImage->GetSpacing();
	Info.Spacing[0] = double(Size_input[0] - 1) * Spacing_input[0] / double(Lx - 1);
	Info.Spacing[1] = double(Size_input[1] - 1) * Spacing_input[1] / double(Ly - 1);

	Info.Origin = m_InputImage->GetOrigin();
	Info.Orientation = m_InputImage->GetOrientation();
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);

	m_Flag_Input_Output_SameOrientation = true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 2>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ ImageFilter2D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_x <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ ImageFilter2D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	ImageInfo2D Info;

	Info.Spacing[0] = Spacing_x;
	Info.Spacing[1] = Spacing_y;

	auto Size_input = m_InputImage->GetSize();
	auto Spacing_input = m_InputImage->GetSpacing();
	Info.Size[0] = int_max(std::ceil(Size_input[0] * Spacing_input[0] / Spacing_x));
	Info.Size[1] = int_max(std::ceil(Size_input[1] * Spacing_input[1] / Spacing_y));

	Info.Origin = m_InputImage->GetOrigin();
	Info.Orientation = m_InputImage->GetOrientation();
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);

	m_Flag_Input_Output_SameOrientation = true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
const ImageInfo2D& ImageFilter2D<InputImageType, OutputImageType, ScalarType>::GetOutputImageInfo()
{
	return m_OutputImageInfo;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::EnableOutputImage(bool On_Off)
{
	m_Flag_EnableOutputImage = On_Off;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::EnableOutputPixelArray(bool On_Off)
{
	m_Flag_EnableOutputPixelArray = On_Off;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::InitializeOutputImage()
{
	m_OutputImage.SetSize(m_OutputImageInfo.Size);
	m_OutputImage.SetSpacing(m_OutputImageInfo.Spacing);
	m_OutputImage.SetOrigin(m_OutputImageInfo.Origin);
	m_OutputImage.SetOrientation(m_OutputImageInfo.Orientation);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
OutputImageType* ImageFilter2D<InputImageType, OutputImageType, ScalarType>::GetOutputImage()
{
	return &m_OutputImage;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::InitializeOutputPixelArray()
{
	int_max PixelCountInArray = 0;

	if (m_PointList_3DWorldPosition != nullptr)
	{
		PixelCountInArray = m_PointList_3DWorldPosition->GetColCount();
	}
	else if (m_PointList_2DPosition_OutputImage != nullptr)
	{
		PixelCountInArray = m_PointList_2DPosition_OutputImage->GetColCount();
	}
	else if (m_PointList_2DIndex_OutputImage != nullptr)
	{
		PixelCountInArray = m_PointList_2DIndex_OutputImage->GetColCount();
	}
	else if (m_PointList_2DIndex_InputImage != nullptr)
	{
		PixelCountInArray = m_PointList_2DIndex_InputImage->GetColCount();
	}

	m_OutputPixelArray.FastResize(PixelCountInArray);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
ObjectArray<typename OutputImageType::PixelType>* ImageFilter2D<InputImageType, OutputImageType, ScalarType>::GetOutputPixelArray()
{
	return &m_OutputPixelArray;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DWorldPosition(const DenseMatrix<ScalarType>* ListOf3DWorldPosition)
{
	m_PointList_3DWorldPosition = ListOf3DWorldPosition;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetPointListOf2DPositionInInputImage(const DenseMatrix<ScalarType>* ListOf2DPosition)
{
	m_PointList_2DPosition_InputImage = ListOf2DPosition;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetPointListOf2DIndexInInputImage(const DenseMatrix<int_max>* ListOf2DIndex)
{
	m_PointList_2DIndex_InputImage = ListOf2DIndex;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetPointListOf2DPositionInOutputImage(const DenseMatrix<ScalarType>* ListOf2DPosition)
{
	m_PointList_2DPosition_OutputImage = ListOf2DIndex;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetPointListOf2DIndexInOutputImage(const DenseMatrix<int_max>* ListOf2DIndex)
{
	m_PointList_2DIndex_OutputImage = ListOf2DIndex;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption)
{
	m_ImageInterpolationOption = InputOption;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
const Option_Of_Image2DInterpolation<typename InputImageType::PixelType>&
ImageFilter2D<InputImageType, OutputImageType, ScalarType>::GetImageInterpolationOption()
{
	return m_ImageInterpolationOption;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SetMaxThreadCount(int_max MaxNumber)
{
	m_MaxThreadCount = MaxNumber;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
int_max ImageFilter2D<InputImageType, OutputImageType, ScalarType>::GetMaxThreadCount()
{
	return m_MaxThreadCount;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
bool ImageFilter2D<InputImageType, OutputImageType, ScalarType>::Update()
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
	if (m_OutputPixelCount > 0)
	{ 
		ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex)
					  {this->Evaluate_in_a_thread(Index_start, Index_end, ThreadIndex); },
					  0, m_OutputPixelCount - 1, m_MaxThreadCount, 1);
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
bool ImageFilter2D<InputImageType, OutputImageType, ScalarType>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ ImageFilter2D::CheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ ImageFilter2D::CheckInput()")
		return false;
	}

	if (m_Flag_EnableOutputImage == true)
	{
		auto OutputImageSize = m_OutputImageInfo.Size;
		auto PixelCount = OutputImageSize[0] * OutputImageSize[1];
		if (PixelCount <= 0) // m_OutputImage may be spasre, do not use GetPixelCount()
		{
			MDK_Error("Output Image size is 0 @ ImageFilter2D::CheckInput()")
			return false;
		}
	}

	if (m_Flag_EnableOutputImage == false && m_Flag_EnableOutputPixelArray == false && m_Flag_EnableOutputToOtherPlace == false)
	{
		MDK_Error("NO output is enabled @ ImageFilter2D::CheckInput()")
		return false;
	}

	if (m_CoordinateSystemForEvaluation == CoordinateSystemForEvaluation::UNKNOWN)
	{
		MDK_Error("CoordinateSystemForEvaluation is UNKNOWN")
		return false;
	}

	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
bool ImageFilter2D<InputImageType, OutputImageType, ScalarType>::Preprocess()
{
	if (m_OutputImageInfo.Orientation.IsEmpty() == true)
	{
		m_OutputImageInfo.Orientation = m_InputImage->GetOrientation();
	}

	auto OutputImageSize = m_OutputImageInfo.Size;
	auto OutputImageSpacing = m_OutputImageInfo.Spacing;
	auto OutputImageOrigin = m_OutputImageInfo.Origin;
	auto PixelCountOfOutputImage = OutputImageSize[0] * OutputImageSize[1];

	if (m_PointList_3DWorldPosition == nullptr 
		&& m_PointList_2DPosition_InputImage == nullptr && m_PointList_2DIndex_InputImage == nullptr
		&& m_PointList_2DPosition_OutputImage == nullptr && m_PointList_2DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = PixelCountOfOutputImage;
		m_Flag_ScanOutputImageGrid = true;
	}
	else if (m_PointList_3DWorldPosition != nullptr
		     && m_PointList_2DPosition_InputImage == nullptr && m_PointList_2DIndex_InputImage == nullptr
		     && m_PointList_2DPosition_OutputImage == nullptr && m_PointList_2DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = m_PointList_3DWorldPosition->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else if (m_PointList_3DWorldPosition == nullptr
		     && m_PointList_2DPosition_InputImage != nullptr && m_PointList_2DIndex_InputImage == nullptr
		     && m_PointList_2DPosition_OutputImage == nullptr && m_PointList_2DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = m_PointList_2DPosition_InputImage->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else if (m_PointList_3DWorldPosition == nullptr
		     && m_PointList_2DPosition_InputImage == nullptr && m_PointList_2DIndex_InputImage != nullptr
		     && m_PointList_2DPosition_OutputImage == nullptr && m_PointList_2DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = m_PointList_2DIndex_InputImage->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else if (m_PointList_3DWorldPosition == nullptr
		     && m_PointList_2DPosition_InputImage == nullptr && m_PointList_2DIndex_InputImage == nullptr
		     && m_PointList_2DPosition_OutputImage != nullptr && m_PointList_2DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = m_PointList_2DPosition_OutputImage->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else if (m_PointList_3DWorldPosition == nullptr
		     && m_PointList_2DPosition_InputImage == nullptr && m_PointList_2DIndex_InputImage == nullptr
		     && m_PointList_2DPosition_OutputImage == nullptr && m_PointList_2DIndex_OutputImage != nullptr)
	{
		m_OutputPixelCount = m_PointList_2DIndex_OutputImage->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else
	{
		MDK_Error("Something is wrong @ ImageFilter2D::Preprocess()")
	    return false;
	}

	if (m_Flag_EnableOutputImage == true)
	{
		this->InitializeOutputImage();
	}
	else
	{
		m_OutputImage.Clear();
		// only set info in m_OutputImage: except size
		m_OutputImage.SetOrigin(m_OutputImageInfo.Origin);
		m_OutputImage.SetSpacing(m_OutputImageInfo.Spacing);
		m_OutputImage.SetOrientation(m_OutputImageInfo.Orientation);		
	}

	if (m_Flag_EnableOutputPixelArray == true)
	{
		m_OutputPixelArray.FastResize(m_OutputPixelCount);
	}
	else
	{
		m_OutputPixelArray.Clear();
	}

	this->Update2DPositionTransform_Input_Output();

	this->CompareOrientation_Input_Output();

	//-------------
	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::
Evaluate_in_a_thread(int_max PointIndex_start, int_max PointIndex_end, int_max ThreadIndex)
{
	switch (m_CoordinateSystemForEvaluation)
	{
	case CoordinateSystemForEvaluation::WORLD:
		Evaluate_in_a_thread_At3DWorldPosition(PointIndex_start, PointIndex_end, ThreadIndex);
		break;
	case CoordinateSystemForEvaluation::INPUT:
		Evaluate_in_a_thread_At2DPositionInInputImage(PointIndex_start, PointIndex_end, ThreadIndex);
		break;
	case CoordinateSystemForEvaluation::OUTPUT:
		Evaluate_in_a_thread_At2DPositionInOutputImage(PointIndex_start, PointIndex_end, ThreadIndex);
		break;
	case CoordinateSystemForEvaluation::UNKNOWN:
		MDK_Error("CoordinateSystemForEvaluation is UNKNOWN")
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::
Evaluate_in_a_thread_At3DWorldPosition(int_max PointIndex_start, int_max PointIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanOutputImageGrid == true)
	{
		for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
		{
			auto Index2D_out = ImageCoordinateTransform_LinearIndexTo2DIndex(k, m_OutputImageInfo);
			auto Pos3D_world = m_OutputImage.Transform2DIndexTo3DWorldPosition(Index2D_out);
			auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
			if (m_Flag_EnableOutputImage == true)
			{
				m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
			}
			if (m_Flag_EnableOutputPixelArray == true)
			{
				m_OutputPixelArray[k] = OutputPixel;
			}
			if (m_Flag_EnableOutputToOtherPlace == true)
			{
				this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
			}
		}
	}
	else
	{
		if (m_PointList_3DWorldPosition != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 3> Pos3D_world;
				m_PointList_3DWorldPosition->GetCol(k, Pos3D_world);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index2D_out = m_OutputImage.Transform3DWorldPositionToNearest2DDiscreteIndex(Pos3D_world);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DPosition_InputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 2> Pos2D_in;
				m_PointList_2DPosition_InputImage->GetCol(k, Pos2D_in);
				auto Pos3D_world = m_InputImage->Transform2DPositionTo3DWorldPosition(Pos2D_in);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Pos2D_out = m_OutputImage.Transform3DWorldPositionTo2DPosition(Pos3D_world);
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DIndex_InputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<int_max, 2> Index2D_in;
				m_PointList_2DIndex_InputImage->GetCol(k, Index2D_in);
				auto Pos3D_world = m_InputImage->Transform2DIndexTo3DWorldPosition(Index2D_in);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Pos2D_out = m_OutputImage.Transform3DWorldPositionTo2DPosition(Pos3D_world);
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DPosition_OutputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 2> Pos2D_out;
				m_PointList_2DPosition_OutputImage->GetCol(k, Pos2D_out);
				auto Pos3D_world = m_OutputImage.Transform2DPositionTo3DWorldPosition(Pos2D_out);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DIndex_OutputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<int_max, 2> Index2D_out;
				m_PointList_2DIndex_OutputImage->GetCol(k, Index2D_out);
				auto Pos3D_world = m_OutputImage.Transform2DIndexTo3DWorldPosition(Index2D_out);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto xIndex_out = int_max(std::round(Index2D_out[0]));
					auto yIndex_out = int_max(std::round(Index2D_out[1]));
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(xIndex_out, yIndex_out) == true)
					{
						m_OutputImage(xIndex_out, yIndex_out) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::
Evaluate_in_a_thread_At2DPositionInInputImage(int_max PointIndex_start, int_max PointIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanOutputImageGrid == true)
	{
		for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
		{
			auto Index2D_out = ImageCoordinateTransform_LinearIndexTo2DIndex(k, m_OutputImageInfo);
			auto Pos2D_out = m_OutputImage.Transform2DIndexTo2DPosition(Index2D_out);
			auto Pos2D_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(Pos2D_out);
			auto OutputPixel = this->EvaluateAt2DPositionInInputImage(k, Pos2D_in[0], Pos2D_in[1], ThreadIndex);
			if (m_Flag_EnableOutputImage == true)
			{
				m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
			}
			if (m_Flag_EnableOutputPixelArray == true)
			{
				m_OutputPixelArray[k] = OutputPixel;
			}
			if (m_Flag_EnableOutputToOtherPlace == true)
			{
				this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
			}
		}
	}
	else
	{
		if (m_PointList_3DWorldPosition != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 3> Pos3D_world;
				m_PointList_3DWorldPosition->GetCol(k, Pos3D_world);
				auto Pos2D_in = m_InputImage->Transform3DWorldPositionTo2DPosition(Pos3D_world);
				auto OutputPixel = this->EvaluateAt2DPositionInInputImage(k, Pos2D_in[0], Pos2D_in[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index2D_out = m_OutputImage.Transform3DWorldPositionToNearest2DDiscreteIndex(Pos3D_world);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DPosition_InputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 2> Pos2D_in;
				m_PointList_2DPosition_InputImage->GetCol(k, Pos2D_in);
				auto OutputPixel = this->EvaluateAt2DPositionInInputImage(k, Pos2D_in[0], Pos2D_in[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Pos2D_out = this->Transform2DPositionInInputImageTo2DPositionInOutputImage(Pos2D_in);
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DIndex_InputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<int_max, 2> Index2D_in;
				m_PointList_2DIndex_InputImage->GetCol(k, Index2D_in);
				auto Pos2D_in = m_InputImage->Transform2DIndexTo2DPosition(Index2D_in);
				auto OutputPixel = this->EvaluateAt2DPositionInInputImage(k, Pos2D_in[0], Pos2D_in[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Pos2D_out = this->Transform2DPositionInInputImageTo2DPositionInOutputImage(Pos2D_in);
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DPosition_OutputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 2> Pos2D_out;
				m_PointList_2DPosition_OutputImage->GetCol(k, Pos2D_out);
				auto Pos2D_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(Pos2D_out);
				auto OutputPixel = this->EvaluateAt2DPositionInInputImage(k, Pos2D_in[0], Pos2D_in[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DIndex_OutputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<int_max, 2> Index2D_out;
				m_PointList_2DIndex_OutputImage->GetCol(k, Index2D_out);
				auto Pos_out = m_OutputImage.Transform2DIndexTo2DPosition(Index2D_out);
				auto Pos2D_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(Pos_out);
				auto OutputPixel = this->EvaluateAt2DPositionInInputImage(k, Pos2D_in[0], Pos2D_in[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto xIndex_out = int_max(std::round(Index2D_out[0]));
					auto yIndex_out = int_max(std::round(Index2D_out[1]));
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(xIndex_out, yIndex_out) == true)
					{
						m_OutputImage(xIndex_out, yIndex_out) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::
Evaluate_in_a_thread_At2DPositionInOutputImage(int_max PointIndex_start, int_max PointIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanOutputImageGrid == true)
	{
		for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
		{
			auto Index2D_out = ImageCoordinateTransform_LinearIndexTo2DIndex(k, m_OutputImageInfo);
			auto Pos2D_out = m_OutputImage.Transform2DIndexTo2DPosition(Index2D_out);
			auto OutputPixel = this->EvaluateAt2DPositionInOutputImage(k, Pos2D_out[0], Pos2D_out[1], ThreadIndex);
			if (m_Flag_EnableOutputImage == true)
			{
				m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
			}
			if (m_Flag_EnableOutputPixelArray == true)
			{
				m_OutputPixelArray[k] = OutputPixel;
			}
			if (m_Flag_EnableOutputToOtherPlace == true)
			{
				this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
			}
		}
	}
	else
	{
		if (m_PointList_3DWorldPosition != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 3> Pos3D_world;
				m_PointList_3DWorldPosition->GetCol(k, Pos3D_world);
				auto Pos2D_out = m_OutputImage.Transform3DWorldPositionTo2DPosition(Pos3D_world);
				auto OutputPixel = this->EvaluateAt2DPositionInOutputImage(k, Pos2D_out[0], Pos2D_out[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DPosition_InputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 2> Pos2D_in;
				m_PointList_2DPosition_InputImage->GetCol(k, Pos2D_in);
				auto Pos2D_out = this->Transform2DPositionInInputImageTo2DPositionInOutputImage(Pos2D_in);
				auto OutputPixel = this->EvaluateAt2DPositionInOutputImage(k, Pos2D_out[0], Pos2D_out[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DIndex_InputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<int_max, 2> Index2D_in;
				m_PointList_2DIndex_InputImage->GetCol(k, Index2D_in);
				auto Pos2D_in = m_InputImage->Transform2DIndexTo2DPosition(Index2D_in);
				auto Pos2D_out = this->Transform2DPositionInInputImageTo2DPositionInOutputImage(Pos2D_in);
				auto OutputPixel = this->EvaluateAt2DPositionInOutputImage(k, Pos2D_out[0], Pos2D_out[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DPosition_OutputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 2> Pos2D_out;
				m_PointList_2DPosition_OutputImage->GetCol(k, Pos2D_out);
				auto OutputPixel = this->EvaluateAt2DPositionInOutputImage(k, Pos2D_out[0], Pos2D_out[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index2D_out = m_OutputImage.Transform2DPositionToNearest2DDiscreteIndex(Pos2D_out);
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(Index2D_out) == true)
					{
						m_OutputImage(Index2D_out[0], Index2D_out[1]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_2DIndex_OutputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<int_max, 2> Index2D_out;
				m_PointList_2DIndex_OutputImage->GetCol(k, Index2D_out);
				auto Pos2D_out = m_OutputImage.Transform2DIndexTo2DPosition(Index2D_out);
				auto OutputPixel = this->EvaluateAt2DPositionInOutputImage(k, Pos2D_out[0], Pos2D_out[1], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto xIndex_out = int_max(std::round(Index2D_out[0]));
					auto yIndex_out = int_max(std::round(Index2D_out[1]));
					if (m_OutputImage.CheckIf2DIndexIsInsideImage(xIndex_out, yIndex_out) == true)
					{
						m_OutputImage(xIndex_out, yIndex_out) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
				}
			}
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
int_max ImageFilter2D<InputImageType, OutputImageType, ScalarType>::GetOptimalThreadCount()
{
	return Compute_Optimal_ThreadCount_For_ParallelBlock(m_OutputPixelCount, m_MaxThreadCount, 1);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation Option)
{
	m_CoordinateSystemForEvaluation = Option;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::CompareOrientation_Input_Output()
{
	if (m_Flag_Input_Output_SameOrientation == false)
	{
		DenseMatrix<double> OrientationDiff = MatrixSubtract(m_InputImageInfo.Orientation, m_OutputImageInfo.Orientation);
		OrientationDiff.ElementOperationInPlace("abs");
		auto SumAbsDiff = OrientationDiff.Sum();
		auto Eps = std::numeric_limits<double>::epsilon();
		if (SumAbsDiff < Eps*9.0)// 9 element in matrix
		{
			m_Flag_Input_Output_SameOrientation = true;
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter2D<InputImageType, OutputImageType, ScalarType>::Update2DPositionTransform_Input_Output()
{
	{
		auto M = m_InputImageInfo.Orientation.Inv();
		m_2DPositionTransformFromOuputToInput_Matrix = MatrixMultiply(M, m_OutputImageInfo.Orientation);
		auto D = m_OutputImageInfo.Origin - m_InputImageInfo.Origin;
		// m_2DPositionTransformFromOuputToInput_Offset = M*D
		m_2DPositionTransformFromOuputToInput_Offset[0] = M[0]*D[0] + M[3]*D[1] + M[6]*D[2];
		m_2DPositionTransformFromOuputToInput_Offset[1] = M[1]*D[0] + M[4]*D[1] + M[7]*D[2];
		//m_2DPositionTransformFromOuputToInput_Offset[2] = M[2]*D[0] + M[5]*D[1] + M[8]*D[2];
	}

	{
		auto M = m_OutputImageInfo.Orientation.Inv();
		m_2DPositionTransformFromInputToOutput_Matrix = MatrixMultiply(M, m_InputImageInfo.Orientation);
		auto D = m_InputImageInfo.Origin - m_OutputImageInfo.Origin;
		// m_2DPositionTransformFromInputToOutput_Offset = M*D
		m_2DPositionTransformFromInputToOutput_Offset[0] = M[0]*D[0] + M[3]*D[1] + M[6]*D[2];
		m_2DPositionTransformFromInputToOutput_Offset[1] = M[1]*D[0] + M[4]*D[1] + M[7]*D[2];
		//m_2DPositionTransformFromInputToOutput_Offset[2] = M[2]*D[0] + M[5]*D[1] + M[8]*D[2];
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
DenseVector<ScalarType, 2> ImageFilter2D<InputImageType, OutputImageType, ScalarType>::
Transform2DPositionInInputImageTo2DPositionInOutputImage(const DenseVector<ScalarType, 2>& Position_in)
{
	DenseVector<ScalarType, 2> Position_out;
	if (m_Flag_Input_Output_SameOrientation == true)
	{
		Position_out[0] = m_InputImageInfo.Origin[0] - m_OutputImageInfo.Origin[0] + Position_in[0];
		Position_out[1] = m_InputImageInfo.Origin[1] - m_OutputImageInfo.Origin[1] + Position_in[1];		
	}
	else
	{
		auto M = m_2DPositionTransformFromInputToOutput_Matrix.GetElementPointer();		
		Position_out[0] = M[0] * Position_in[0] + M[3] * Position_in[1] + m_2DPositionTransformFromInputToOutput_Offset[0];
		Position_out[1] = M[1] * Position_in[0] + M[4] * Position_in[1] + m_2DPositionTransformFromInputToOutput_Offset[1];
		//Position_out[0] = M[0] * Position_in[0] + M[3] * Position_in[1] + M[6] * Position_in[2] + m_2DPositionTransformFromInputToOutput_Offset[0];
		//Position_out[1] = M[1] * Position_in[0] + M[4] * Position_in[1] + M[7] * Position_in[2] + m_2DPositionTransformFromInputToOutput_Offset[1];
		//Position_out[2] = M[2] * Position_in[0] + M[5] * Position_in[1] + M[8] * Position_in[2] + m_2DPositionTransformFromInputToOutput_Offset[2];		
	}
	return Position_out;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
DenseVector<ScalarType, 2> ImageFilter2D<InputImageType, OutputImageType, ScalarType>::
Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out)
{
	DenseVector<ScalarType, 2> Position_in;
	if (m_Flag_Input_Output_SameOrientation == true)
	{
		Position_in[0] = m_OutputImageInfo.Origin[0] - m_InputImageInfo.Origin[0] + Position_out[0];
		Position_in[1] = m_OutputImageInfo.Origin[1] - m_InputImageInfo.Origin[1] + Position_out[1];
	}
	else
	{
		auto M = m_2DPositionTransformFromOuputToInput_Matrix.GetElementPointer();		
		Position_in[0] = M[0] * Position_out[0] + M[3] * Position_out[1] + m_2DPositionTransformFromOuputToInput_Offset[0];
		Position_in[1] = M[1] * Position_out[0] + M[4] * Position_out[1] + m_2DPositionTransformFromOuputToInput_Offset[1];
		//Position_in[0] = M[0] * Position_out[0] + M[3] * Position_out[1] + M[6] * Position_out[2] + m_2DPositionTransformFromOuputToInput_Offset[0];
		//Position_in[1] = M[1] * Position_out[0] + M[4] * Position_out[1] + M[7] * Position_out[2] + m_2DPositionTransformFromOuputToInput_Offset[1];
		//Position_in[2] = M[2] * Position_out[0] + M[5] * Position_out[1] + M[8] * Position_out[2] + m_2DPositionTransformFromOuputToInput_Offset[2];		
	}
	return Position_in;
}

}// namespace mdk

#endif