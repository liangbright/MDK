#ifndef mdk_ImageFilter3D_hpp
#define mdk_ImageFilter3D_hpp

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
	m_ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Replicate;
	m_ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);
	
	m_PointList_3DPhysicalPosition = nullptr;
	m_PointList_3DIndex_InputImage = nullptr;
	m_PointList_3DIndex_OutputImage = nullptr;
	
	m_Flag_ScanWholeImageGrid = false;
	m_TotalNumberOfOutputPixelTobeProcessed = 0;
	m_MaxNumberOfThread = 1;

	m_Flag_EnableOutputImage = true;
	m_Flag_EnableOutputPixelArray = false;
	m_Flag_EnableOutputToOtherPlace = false;

	m_OutputImageInfo.Clear();
	m_OutputImage.Clear();
	m_OutputPixelArray.Clear();
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
																					const DenseVector<int_max, 3>& Size,
																					const DenseMatrix<double>& Orientation)
{
	m_OutputImageInfo.Origin = Origin;
	m_OutputImageInfo.Spacing = Spacing;
	m_OutputImageInfo.Size = Size;
	m_OutputImageInfo.Orientation = Orientation;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1], Size[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ ImageFilter3D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 0 || Ly <= 0 || Lz <= 0)
	{
		MDK_Error("Invalid input @ ImageFilter3D::SetOutputImageInfoBySize(...)")
		return;
	}

	Image3DInfo Info;
	Info.Size[0] = Lx;
	Info.Size[1] = Ly;
	Info.Size[2] = Lz;

	auto Size_input = m_InputImage->GetSize();
	auto Spacing_input = m_InputImage->GetSpacing();
	Info.Spacing[0] = double(Size_input[0] - 1) * Spacing_input[0] / double(Lx - 1);
	Info.Spacing[1] = double(Size_input[1] - 1) * Spacing_input[1] / double(Ly - 1);
	Info.Spacing[2] = double(Size_input[2] - 1) * Spacing_input[2] / double(Lz - 1);

	Info.Origin = m_InputImage->GetOrigin();
	Info.Orientation = m_InputImage->GetOrientation();

	this->SetOutputImageInfo(Info);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 3>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1], Spacing[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ ImageFilter3D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_x <= Zero || Spacing_x <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ ImageFilter3D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	Image3DInfo Info;

	Info.Spacing[0] = Spacing_x;
	Info.Spacing[1] = Spacing_y;
	Info.Spacing[2] = Spacing_z;

	auto Size_input = m_InputImage->GetSize();
	auto Spacing_input = m_InputImage->GetSpacing();
	Info.Size[0] = int_max(std::ceil(Size_input[0] * Spacing_input[0] / Spacing_x));
	Info.Size[1] = int_max(std::ceil(Size_input[1] * Spacing_input[1] / Spacing_y));
	Info.Size[2] = int_max(std::ceil(Size_input[2] * Spacing_input[2] / Spacing_z));

	Info.Origin = m_InputImage->GetOrigin();
	Info.Orientation = m_InputImage->GetOrientation();

	this->SetOutputImageInfo(Info);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
const Image3DInfo& ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputImageInfo()
{
	return m_OutputImageInfo;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::EnableOutputImage(bool On_Off)
{
	m_Flag_EnableOutputImage = On_Off;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::EnableOutputPixelArray(bool On_Off)
{
	m_Flag_EnableOutputPixelArray = On_Off;
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
	else if (m_PointList_3DPhysicalPosition != nullptr)
	{
		PixelNumberInArray = m_PointList_3DPhysicalPosition->GetColNumber();
	}

	m_OutputPixelArray.FastResize(PixelNumberInArray);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
ObjectArray<typename OutputImageType::PixelType>* ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputPixelArray()
{
	return &m_OutputPixelArray;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DPhysicalPosition(const DenseMatrix<ScalarType>* ListOf3DPhysicalPosition)
{
	m_PointList_3DPhysicalPosition = std::move(ListOf3DPhysicalPosition);
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
const Option_Of_Image3DInterpolation<typename InputImageType::PixelType>&
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

	if (m_Flag_EnableOutputImage == true)
	{
		auto OutputImageSize = m_OutputImageInfo.Size;
		auto PixelNumber = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];
		if (PixelNumber <= 0) // m_OutputImage may be spasre, do not use GetPixelNumber()
		{
			MDK_Error("Output Image size is 0 @ ImageFilter3D::CheckInput()")
			return false;
		}
	}

	if (m_Flag_EnableOutputImage == false && m_Flag_EnableOutputPixelArray == false && m_Flag_EnableOutputToOtherPlace == false)
	{
		MDK_Error("NO output is enabled @ ImageFilter3D::CheckInput()")
		return false;
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

	auto OutputImageSize = m_OutputImageInfo.Size;
	auto OutputImageSpacing = m_OutputImageInfo.Spacing;
	auto OutputImageOrigin = m_OutputImageInfo.Origin;
	auto PixelNumberOfOutputImage = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];

	if (m_PointList_3DIndex_OutputImage == nullptr && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPhysicalPosition == nullptr)
	{
		m_TotalNumberOfOutputPixelTobeProcessed = PixelNumberOfOutputImage;
		m_Flag_ScanWholeImageGrid = true;
	}
	else if (m_PointList_3DIndex_OutputImage != nullptr && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPhysicalPosition == nullptr)
	{
		m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DIndex_OutputImage->GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}
	else if (m_PointList_3DIndex_OutputImage == nullptr && m_PointList_3DIndex_InputImage != nullptr && m_PointList_3DPhysicalPosition == nullptr)
	{
		m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DIndex_InputImage->GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}
	else if (m_PointList_3DIndex_OutputImage == nullptr && m_PointList_3DIndex_InputImage == nullptr && m_PointList_3DPhysicalPosition != nullptr)
	{
		m_TotalNumberOfOutputPixelTobeProcessed = m_PointList_3DPhysicalPosition->GetColNumber();
		m_Flag_ScanWholeImageGrid = false;
	}
	else
	{
		MDK_Error("Something is wrong @ ImageFilter3D::Preprocess()")
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
		m_OutputPixelArray.FastResize(m_TotalNumberOfOutputPixelTobeProcessed);
	}
	else
	{
		m_OutputPixelArray.Clear();
	}

	//-------------
	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Evaluate_in_a_thread(int_max PointIndex_start, int_max PointIndex_end, int_max ThreadIndex)
{
	if (m_Flag_ScanWholeImageGrid == true)
	{
		for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
		{
			auto Index3D = this->TransformLinearIndexTo3DIndexInOutputImage(k);
			auto Pos3D = this->Transform3DIndexInOutputImageTo3DPhysicalPosition(Index3D);
			auto OutputPixel = this->EvaluateAt3DPhysicalPosition(k, Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
			if (m_Flag_EnableOutputImage == true)
			{
				m_OutputImage(Index3D[0], Index3D[1], Index3D[2]) = OutputPixel;
			}
			if (m_Flag_EnableOutputPixelArray == true)
			{
				m_OutputPixelArray[k] = OutputPixel;
			}
			if (m_Flag_EnableOutputToOtherPlace == true)
			{
				this->StoreOutputPixelInPixelArrayOfOtherFormat(OutputPixel, k, ThreadIndex);
			}
		}
	}
	else
	{
		if (m_PointList_3DIndex_OutputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<int_max, 3> Index3D;
				m_PointList_3DIndex_OutputImage->GetCol(k, Index3D);
				auto Pos3D = this->Transform3DIndexInOutputImageTo3DPhysicalPosition(Index3D);
				auto OutputPixel = this->EvaluateAt3DPhysicalPosition(k, Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					m_OutputImage(Index3D[0], Index3D[1], Index3D[2]) = OutputPixel;
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelInPixelArrayOfOtherFormat(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_3DIndex_InputImage != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<int_max, 3> Index3D_input;
				m_PointList_3DIndex_InputImage->GetCol(k, Index3D_input);
				auto Pos3D = m_InputImage->Transform3DIndexTo3DPhysicalPosition(Index3D_input);
				auto OutputPixel = this->EvaluateAt3DPhysicalPosition(k, Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D = m_OutputImage.Transform3DPhysicalPositionToNearest3DDiscreteIndex(Pos3D);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D) == true)
					{
						m_OutputImage(Index3D[0], Index3D[1], Index3D[2]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
					this->StoreOutputPixelInPixelArrayOfOtherFormat(OutputPixel, k, ThreadIndex);
				}
			}
		}
		else if (m_PointList_3DPhysicalPosition != nullptr)
		{
			for (int_max k = PointIndex_start; k <= PointIndex_end; ++k)
			{
				DenseVector<ScalarType, 3> Pos3D;
				m_PointList_3DPhysicalPosition->GetCol(k, Pos3D);
				auto OutputPixel = this->EvaluateAt3DPhysicalPosition(k, Pos3D[0], Pos3D[1], Pos3D[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D = m_OutputImage.Transform3DPhysicalPositionToNearest3DDiscreteIndex(Pos3D);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D) == true)
					{
						m_OutputImage(Index3D[0], Index3D[1], Index3D[2]) = OutputPixel;
					}
				}
				if (m_Flag_EnableOutputPixelArray == true)
				{
					m_OutputPixelArray[k] = OutputPixel;
				}
				if (m_Flag_EnableOutputToOtherPlace == true)
				{
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
DenseVector<int_max, 3> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
TransformLinearIndexTo3DIndexInOutputImage(int_max LinearIndex)
{
	auto divresult = std::div(LinearIndex, m_OutputImageInfo.Size[0] * m_OutputImageInfo.Size[1]);

	auto zIndex = divresult.quot; // z

	divresult = std::div(divresult.rem, m_OutputImageInfo.Size[0]);

	auto yIndex = divresult.quot; // y
	auto xIndex = divresult.rem; // x

	DenseVector<int_max, 3> Index3D;
	Index3D[0] = int_max(xIndex);
	Index3D[1] = int_max(yIndex);
	Index3D[2] = int_max(zIndex);

	return Index3D;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename IndexType>
DenseVector<ScalarType, 3> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Transform3DIndexInOutputImageTo3DPhysicalPosition(const DenseVector<IndexType, 3>& Index3D)
{
	return this->Transform3DIndexInOutputImageTo3DPhysicalPosition(Index3D[0], Index3D[1], Index3D[2]);
}

template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename IndexType>
DenseVector<ScalarType, 3> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Transform3DIndexInOutputImageTo3DPhysicalPosition(IndexType x_Index, IndexType y_Index, IndexType z_Index)
{
	DenseVector<ScalarType, 3> Position;
	Position[0] = ScalarType(m_OutputImageInfo.Origin[0] + double(x_Index) * m_OutputImageInfo.Spacing[0]);
	Position[1] = ScalarType(m_OutputImageInfo.Origin[1] + double(y_Index) * m_OutputImageInfo.Spacing[1]);
	Position[2] = ScalarType(m_OutputImageInfo.Origin[2] + double(z_Index) * m_OutputImageInfo.Spacing[2]);
	return Position;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
DenseVector<ScalarType, 3> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Transform3DPhysicalPositionTo3DIndexInOutputImage(const DenseVector<ScalarType, 3>& Position)
{
	return this->Transform3DPhysicalPositionTo3DIndexInOutputImage(Position[0], Position[1], Position[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
DenseVector<ScalarType, 3> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Transform3DPhysicalPositionTo3DIndexInOutputImage(ScalarType x, ScalarType y, ScalarType z)
{
	DenseVector<ScalarType, 3> Index3D;
	Index3D[0] = ScalarType((double(x) - m_OutputImageInfo.Origin[0]) / m_OutputImageInfo.Spacing[0]);
	Index3D[1] = ScalarType((double(y) - m_OutputImageInfo.Origin[1]) / m_OutputImageInfo.Spacing[1]);
	Index3D[2] = ScalarType((double(z) - m_OutputImageInfo.Origin[2]) / m_OutputImageInfo.Spacing[2]);
	return Index3D;
}


}// namespace mdk

#endif