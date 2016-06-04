#pragma once

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
	m_InputImageInfo.Clear();

	m_ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Nearest;
	m_ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Replicate;
	m_ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);

	m_PointList_3DWorldPosition = nullptr;
	m_PointList_3DIndex_InputImage = nullptr;
	m_PointList_3DPosition_InputImage = nullptr;
	m_PointList_3DPosition_OutputImage = nullptr;
	m_PointList_3DIndex_OutputImage = nullptr;
	
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
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetInputImage(const InputImageType* InputImage)
{
	m_InputImage = InputImage;
	if (m_InputImage != nullptr)
	{
		m_InputImageInfo = m_InputImage->GetInfo();
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetOutputImageInfo(const ImageInfo3D& Info)
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
	m_OutputImageInfo.UpdateTransformMatrix();
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

	ImageInfo3D Info;
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
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);

	m_Flag_Input_Output_SameOrientation = true;
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

	ImageInfo3D Info;

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
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);

	m_Flag_Input_Output_SameOrientation = true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
const ImageInfo3D& ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputImageInfo()
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
	int_max PixelCountInArray = 0;

	if (m_PointList_3DWorldPosition != nullptr)
	{
		PixelCountInArray = m_PointList_3DWorldPosition->GetColCount();
	}
	else if (m_PointList_3DPosition_OutputImage != nullptr)
	{
		PixelCountInArray = m_PointList_3DPosition_OutputImage->GetColCount();
	}
	else if (m_PointList_3DIndex_OutputImage != nullptr)
	{
		PixelCountInArray = m_PointList_3DIndex_OutputImage->GetColCount();
	}
	else if (m_PointList_3DIndex_InputImage != nullptr)
	{
		PixelCountInArray = m_PointList_3DIndex_InputImage->GetColCount();
	}

	m_OutputPixelArray.FastResize(PixelCountInArray);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
ObjectArray<typename OutputImageType::PixelType>* ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOutputPixelArray()
{
	return &m_OutputPixelArray;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DWorldPosition(const DenseMatrix<ScalarType>* ListOf3DWorldPosition)
{
	m_PointList_3DWorldPosition = ListOf3DWorldPosition;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DPositionInInputImage(const DenseMatrix<ScalarType>* ListOf3DPosition)
{
	m_PointList_3DPosition_InputImage = ListOf3DPosition;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DIndexInInputImage(const DenseMatrix<int_max>* ListOf3DIndex)
{
	m_PointList_3DIndex_InputImage = ListOf3DIndex;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DPositionInOutputImage(const DenseMatrix<ScalarType>* ListOf3DPosition)
{
	m_PointList_3DPosition_OutputImage = ListOf3DIndex;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetPointListOf3DIndexInOutputImage(const DenseMatrix<int_max>* ListOf3DIndex)
{
	m_PointList_3DIndex_OutputImage = ListOf3DIndex;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption)
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
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SetMaxThreadCount(int_max MaxNumber)
{
	m_MaxThreadCount = MaxNumber;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
int_max ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetMaxThreadCount()
{
	return m_MaxThreadCount;
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

	if (m_OutputPixelCount > 0)
	{ 
		this->Evaluate();
	}

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
		auto PixelCount = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];
		if (PixelCount <= 0) // m_OutputImage may be spasre, do not use GetPixelCount()
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

	if (m_CoordinateSystemForEvaluation == CoordinateSystemForEvaluation::UNKNOWN)
	{
		MDK_Error("CoordinateSystemForEvaluation is UNKNOWN")
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
	auto PixelCountOfOutputImage = OutputImageSize[0] * OutputImageSize[1] * OutputImageSize[2];

	if (m_PointList_3DWorldPosition == nullptr 
		&& m_PointList_3DPosition_InputImage == nullptr && m_PointList_3DIndex_InputImage == nullptr
		&& m_PointList_3DPosition_OutputImage == nullptr && m_PointList_3DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = PixelCountOfOutputImage;
		m_Flag_ScanOutputImageGrid = true;
	}
	else if (m_PointList_3DWorldPosition != nullptr
		     && m_PointList_3DPosition_InputImage == nullptr && m_PointList_3DIndex_InputImage == nullptr
		     && m_PointList_3DPosition_OutputImage == nullptr && m_PointList_3DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = m_PointList_3DWorldPosition->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else if (m_PointList_3DWorldPosition == nullptr
		     && m_PointList_3DPosition_InputImage != nullptr && m_PointList_3DIndex_InputImage == nullptr
		     && m_PointList_3DPosition_OutputImage == nullptr && m_PointList_3DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = m_PointList_3DPosition_InputImage->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else if (m_PointList_3DWorldPosition == nullptr
		     && m_PointList_3DPosition_InputImage == nullptr && m_PointList_3DIndex_InputImage != nullptr
		     && m_PointList_3DPosition_OutputImage == nullptr && m_PointList_3DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = m_PointList_3DIndex_InputImage->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else if (m_PointList_3DWorldPosition == nullptr
		     && m_PointList_3DPosition_InputImage == nullptr && m_PointList_3DIndex_InputImage == nullptr
		     && m_PointList_3DPosition_OutputImage != nullptr && m_PointList_3DIndex_OutputImage == nullptr)
	{
		m_OutputPixelCount = m_PointList_3DPosition_OutputImage->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
	}
	else if (m_PointList_3DWorldPosition == nullptr
		     && m_PointList_3DPosition_InputImage == nullptr && m_PointList_3DIndex_InputImage == nullptr
		     && m_PointList_3DPosition_OutputImage == nullptr && m_PointList_3DIndex_OutputImage != nullptr)
	{
		m_OutputPixelCount = m_PointList_3DIndex_OutputImage->GetColCount();
		m_Flag_ScanOutputImageGrid = false;
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
		m_OutputPixelArray.FastResize(m_OutputPixelCount);
	}
	else
	{
		m_OutputPixelArray.Clear();
	}

	this->Update3DPositionTransform_Input_Output();

	//-------------
	return true;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::Evaluate()
{
	switch (m_CoordinateSystemForEvaluation)
	{
	case CoordinateSystemForEvaluation::WORLD:
		this->Evaluate_3DWorldPosition();
		break;
	case CoordinateSystemForEvaluation::INPUT:
		this->Evaluate_3DPositionInInputImage();
		break;
	case CoordinateSystemForEvaluation::OUTPUT:
		this->Evaluate_3DPositionInOutputImage();
		break;
	case CoordinateSystemForEvaluation::UNKNOWN:
		MDK_Error("CoordinateSystemForEvaluation is UNKNOWN")
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::Evaluate_3DWorldPosition()
{
	if (m_Flag_ScanOutputImageGrid == true)
	{
		//for (int_max k = 0; k < m_OutputPixelCount; ++k)
		auto TempFunction = [&](int_max k, int_max ThreadIndex)
		{
			auto Index3D_out = ImageCoordinateTransform_LinearIndexTo3DIndex(k, m_OutputImageInfo);
			auto Pos3D_world = m_OutputImage.Transform3DIndexTo3DWorldPosition(Index3D_out);
			auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
			if (m_Flag_EnableOutputImage == true)
			{
				m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
			}
			if (m_Flag_EnableOutputPixelArray == true)
			{
				m_OutputPixelArray[k] = OutputPixel;
			}
			if (m_Flag_EnableOutputToOtherPlace == true)
			{
				this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
			}
		};
		ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
	}
	else
	{
		if (m_PointList_3DWorldPosition != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_world;
				m_PointList_3DWorldPosition->GetCol(k, Pos3D_world);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D_out = m_OutputImage.Transform3DWorldPositionToNearest3DDiscreteIndex(Pos3D_world);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DPosition_InputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_in;
				m_PointList_3DPosition_InputImage->GetCol(k, Pos3D_in);
				auto Pos3D_world = m_InputImage->Transform3DPositionTo3DWorldPosition(Pos3D_in);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Pos3D_out = m_OutputImage.Transform3DWorldPositionTo3DPosition(Pos3D_world);
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DIndex_InputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<int_max, 3> Index3D_in;
				m_PointList_3DIndex_InputImage->GetCol(k, Index3D_in);
				auto Pos3D_world = m_InputImage->Transform3DIndexTo3DWorldPosition(Index3D_in);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Pos3D_out = m_OutputImage.Transform3DWorldPositionTo3DPosition(Pos3D_world);
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DPosition_OutputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_out;
				m_PointList_3DPosition_OutputImage->GetCol(k, Pos3D_out);
				auto Pos3D_world = m_OutputImage.Transform3DPositionTo3DWorldPosition(Pos3D_out);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DIndex_OutputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<int_max, 3> Index3D_out;
				m_PointList_3DIndex_OutputImage->GetCol(k, Index3D_out);
				auto Pos3D_world = m_OutputImage.Transform3DIndexTo3DWorldPosition(Index3D_out);
				auto OutputPixel = this->EvaluateAt3DWorldPosition(k, Pos3D_world[0], Pos3D_world[1], Pos3D_world[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto xIndex_out = int_max(std::round(Index3D_out[0]));
					auto yIndex_out = int_max(std::round(Index3D_out[1]));
					auto zIndex_out = int_max(std::round(Index3D_out[2]));
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(xIndex_out, yIndex_out, zIndex_out) == true)
					{
						m_OutputImage(xIndex_out, yIndex_out, zIndex_out) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::Evaluate_3DPositionInInputImage()
{
	if (m_Flag_ScanOutputImageGrid == true)
	{
		//for (int_max k = 0; k < m_OutputPixelCount; ++k)
		auto TempFunction = [&](int_max k, int_max ThreadIndex)
		{
			auto Index3D_out = ImageCoordinateTransform_LinearIndexTo3DIndex(k, m_OutputImageInfo);
			auto Pos3D_out = m_OutputImage.Transform3DIndexTo3DPosition(Index3D_out);
			auto Pos3D_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos3D_out);
			auto OutputPixel = this->EvaluateAt3DPositionInInputImage(k, Pos3D_in[0], Pos3D_in[1], Pos3D_in[2], ThreadIndex);
			//if (m_Flag_EnableOutputImage == true)
			//{
				m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
			//}

			/*
			auto Index3D_out = ImageCoordinateTransform_LinearIndexTo3DIndex(k, m_OutputImageInfo);
			auto Pos3D_out = m_OutputImage.Transform3DIndexTo3DPosition(Index3D_out);
			auto Pos3D_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos3D_out);
			auto OutputPixel = this->EvaluateAt3DPositionInInputImage(k, Pos3D_in[0], Pos3D_in[1], Pos3D_in[2], ThreadIndex);
			if (m_Flag_EnableOutputImage == true)
			{
				m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
			}
			if (m_Flag_EnableOutputPixelArray == true)
			{
				m_OutputPixelArray[k] = OutputPixel;
			}
			if (m_Flag_EnableOutputToOtherPlace == true)
			{
				this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
			}
			*/
		};
		ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
	}
	else
	{
		if (m_PointList_3DWorldPosition != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_world;
				m_PointList_3DWorldPosition->GetCol(k, Pos3D_world);
				auto Pos3D_in = m_InputImage->Transform3DWorldPositionTo3DPosition(Pos3D_world);
				auto OutputPixel = this->EvaluateAt3DPositionInInputImage(k, Pos3D_in[0], Pos3D_in[1], Pos3D_in[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D_out = m_OutputImage.Transform3DWorldPositionToNearest3DDiscreteIndex(Pos3D_world);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DPosition_InputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_in;
				m_PointList_3DPosition_InputImage->GetCol(k, Pos3D_in);
				auto OutputPixel = this->EvaluateAt3DPositionInInputImage(k, Pos3D_in[0], Pos3D_in[1], Pos3D_in[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Pos3D_out = this->Transform3DPositionInInputImageTo3DPositionInOutputImage(Pos3D_in);
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DIndex_InputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<int_max, 3> Index3D_in;
				m_PointList_3DIndex_InputImage->GetCol(k, Index3D_in);
				auto Pos3D_in = m_InputImage->Transform3DIndexTo3DPosition(Index3D_in);
				auto OutputPixel = this->EvaluateAt3DPositionInInputImage(k, Pos3D_in[0], Pos3D_in[1], Pos3D_in[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Pos3D_out = this->Transform3DPositionInInputImageTo3DPositionInOutputImage(Pos3D_in);
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DPosition_OutputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_out;
				m_PointList_3DPosition_OutputImage->GetCol(k, Pos3D_out);
				auto Pos3D_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos3D_out);
				auto OutputPixel = this->EvaluateAt3DPositionInInputImage(k, Pos3D_in[0], Pos3D_in[1], Pos3D_in[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DIndex_OutputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<int_max, 3> Index3D_out;
				m_PointList_3DIndex_OutputImage->GetCol(k, Index3D_out);
				auto Pos_out = m_OutputImage.Transform3DIndexTo3DPosition(Index3D_out);
				auto Pos3D_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
				auto OutputPixel = this->EvaluateAt3DPositionInInputImage(k, Pos3D_in[0], Pos3D_in[1], Pos3D_in[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto xIndex_out = int_max(std::round(Index3D_out[0]));
					auto yIndex_out = int_max(std::round(Index3D_out[1]));
					auto zIndex_out = int_max(std::round(Index3D_out[2]));
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(xIndex_out, yIndex_out, zIndex_out) == true)
					{
						m_OutputImage(xIndex_out, yIndex_out, zIndex_out) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::Evaluate_3DPositionInOutputImage()
{
	if (m_Flag_ScanOutputImageGrid == true)
	{
		//for (int_max k = 0; k < m_OutputPixelCount; ++k)
		auto TempFunction = [&](int_max k, int_max ThreadIndex)
		{
			auto Index3D_out = ImageCoordinateTransform_LinearIndexTo3DIndex(k, m_OutputImageInfo);
			auto Pos3D_out = m_OutputImage.Transform3DIndexTo3DPosition(Index3D_out);
			auto OutputPixel = this->EvaluateAt3DPositionInOutputImage(k, Pos3D_out[0], Pos3D_out[1], Pos3D_out[2], ThreadIndex);
			if (m_Flag_EnableOutputImage == true)
			{
				m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
			}
			if (m_Flag_EnableOutputPixelArray == true)
			{
				m_OutputPixelArray[k] = OutputPixel;
			}
			if (m_Flag_EnableOutputToOtherPlace == true)
			{
				this->StoreOutputPixelToOtherPlace(OutputPixel, k, ThreadIndex);
			}
		};
		ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
	}
	else
	{
		if (m_PointList_3DWorldPosition != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_world;
				m_PointList_3DWorldPosition->GetCol(k, Pos3D_world);
				auto Pos3D_out = m_OutputImage.Transform3DWorldPositionTo3DPosition(Pos3D_world);
				auto OutputPixel = this->EvaluateAt3DPositionInOutputImage(k, Pos3D_out[0], Pos3D_out[1], Pos3D_out[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DPosition_InputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_in;
				m_PointList_3DPosition_InputImage->GetCol(k, Pos3D_in);
				auto Pos3D_out = this->Transform3DPositionInInputImageTo3DPositionInOutputImage(Pos3D_in);
				auto OutputPixel = this->EvaluateAt3DPositionInOutputImage(k, Pos3D_out[0], Pos3D_out[1], Pos3D_out[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DIndex_InputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<int_max, 3> Index3D_in;
				m_PointList_3DIndex_InputImage->GetCol(k, Index3D_in);
				auto Pos3D_in = m_InputImage->Transform3DIndexTo3DPosition(Index3D_in);
				auto Pos3D_out = this->Transform3DPositionInInputImageTo3DPositionInOutputImage(Pos3D_in);
				auto OutputPixel = this->EvaluateAt3DPositionInOutputImage(k, Pos3D_out[0], Pos3D_out[1], Pos3D_out[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DPosition_OutputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<ScalarType, 3> Pos3D_out;
				m_PointList_3DPosition_OutputImage->GetCol(k, Pos3D_out);
				auto OutputPixel = this->EvaluateAt3DPositionInOutputImage(k, Pos3D_out[0], Pos3D_out[1], Pos3D_out[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto Index3D_out = m_OutputImage.Transform3DPositionToNearest3DDiscreteIndex(Pos3D_out);
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(Index3D_out) == true)
					{
						m_OutputImage(Index3D_out[0], Index3D_out[1], Index3D_out[2]) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
		else if (m_PointList_3DIndex_OutputImage != nullptr)
		{
			//for (int_max k = 0; k < m_OutputPixelCount; ++k)
			auto TempFunction = [&](int_max k, int_max ThreadIndex)
			{
				DenseVector<int_max, 3> Index3D_out;
				m_PointList_3DIndex_OutputImage->GetCol(k, Index3D_out);
				auto Pos3D_out = m_OutputImage.Transform3DIndexTo3DPosition(Index3D_out);
				auto OutputPixel = this->EvaluateAt3DPositionInOutputImage(k, Pos3D_out[0], Pos3D_out[1], Pos3D_out[2], ThreadIndex);
				if (m_Flag_EnableOutputImage == true)
				{
					auto xIndex_out = int_max(std::round(Index3D_out[0]));
					auto yIndex_out = int_max(std::round(Index3D_out[1]));
					auto zIndex_out = int_max(std::round(Index3D_out[2]));
					if (m_OutputImage.CheckIf3DIndexIsInsideImage(xIndex_out, yIndex_out, zIndex_out) == true)
					{
						m_OutputImage(xIndex_out, yIndex_out, zIndex_out) = OutputPixel;
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
			};
			ParallelForLoop_WithThreadIndex(TempFunction, 0, m_OutputPixelCount - 1, m_MaxThreadCount);
		}
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
int_max ImageFilter3D<InputImageType, OutputImageType, ScalarType>::GetOptimalThreadCount()
{
	return Compute_Optimal_ThreadCount_For_ParallelBlock(m_OutputPixelCount, m_MaxThreadCount, 1);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation Option)
{
	m_CoordinateSystemForEvaluation = Option;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
void ImageFilter3D<InputImageType, OutputImageType, ScalarType>::Update3DPositionTransform_Input_Output()
{
	{
		auto M = m_InputImageInfo.Orientation.Inv();
		m_3DPositionTransformFromOuputToInput_Matrix = MatrixMultiply(M, m_OutputImageInfo.Orientation);
		auto D = m_OutputImageInfo.Origin - m_InputImageInfo.Origin;
		// m_3DPositionTransformFromOuputToInput_Offset = M*D
		m_3DPositionTransformFromOuputToInput_Offset[0] = M[0]*D[0] + M[3]*D[1] + M[6]*D[2];
		m_3DPositionTransformFromOuputToInput_Offset[1] = M[1]*D[0] + M[4]*D[1] + M[7]*D[2];
		m_3DPositionTransformFromOuputToInput_Offset[2] = M[2]*D[0] + M[5]*D[1] + M[8]*D[2];
	}

	{
		auto M = m_OutputImageInfo.Orientation.Inv();
		m_3DPositionTransformFromInputToOutput_Matrix = MatrixMultiply(M, m_InputImageInfo.Orientation);
		auto D = m_InputImageInfo.Origin - m_OutputImageInfo.Origin;
		// m_3DPositionTransformFromInputToOutput_Offset = M*D
		m_3DPositionTransformFromInputToOutput_Offset[0] = M[0]*D[0] + M[3]*D[1] + M[6]*D[2];
		m_3DPositionTransformFromInputToOutput_Offset[1] = M[1]*D[0] + M[4]*D[1] + M[7]*D[2];
		m_3DPositionTransformFromInputToOutput_Offset[2] = M[2]*D[0] + M[5]*D[1] + M[8]*D[2];
	}

	DenseMatrix<double> OrientationDiff = MatrixSubtract(m_InputImageInfo.Orientation, m_OutputImageInfo.Orientation);
	OrientationDiff.ElementOperation("abs");
	auto SumAbsDiff = OrientationDiff.Sum();
	auto Eps = std::numeric_limits<double>::epsilon();
	if (SumAbsDiff <= Eps*9.0)// 9 element in matrix
	{
		m_Flag_Input_Output_SameOrientation = true;
	}
	else
	{
		m_Flag_Input_Output_SameOrientation = false;
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
DenseVector<ScalarType, 3> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Transform3DPositionInInputImageTo3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position_in)
{
	if (m_Flag_Input_Output_SameOrientation == true)
	{
		auto Position_out = m_InputImageInfo.Origin - m_OutputImageInfo.Origin + Position_in;
		return Position_out;
	}
	else
	{
		auto R = m_3DPositionTransformFromInputToOutput_Matrix.GetElementPointer();
		DenseVector<ScalarType, 3> Position_out;
		Position_out[0] = R[0] * Position_in[0] + R[3] * Position_in[1] + R[6] * Position_in[2] + m_3DPositionTransformFromInputToOutput_Offset[0];
		Position_out[1] = R[1] * Position_in[0] + R[4] * Position_in[1] + R[7] * Position_in[2] + m_3DPositionTransformFromInputToOutput_Offset[1];
		Position_out[2] = R[2] * Position_in[0] + R[5] * Position_in[1] + R[8] * Position_in[2] + m_3DPositionTransformFromInputToOutput_Offset[2];
		return Position_out;
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
DenseVector<ScalarType, 3> ImageFilter3D<InputImageType, OutputImageType, ScalarType>::
Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out)
{
	if (m_Flag_Input_Output_SameOrientation == true)
	{
		auto Position_in = m_OutputImageInfo.Origin - m_InputImageInfo.Origin + Position_out;
		return Position_in;
	}
	else
	{
		auto R = m_3DPositionTransformFromOuputToInput_Matrix.GetElementPointer();
		DenseVector<ScalarType, 3> Position_in;
		Position_in[0] = R[0] * Position_out[0] + R[3] * Position_out[1] + R[6] * Position_out[2] + m_3DPositionTransformFromOuputToInput_Offset[0];
		Position_in[1] = R[1] * Position_out[0] + R[4] * Position_out[1] + R[7] * Position_out[2] + m_3DPositionTransformFromOuputToInput_Offset[1];
		Position_in[2] = R[2] * Position_out[0] + R[5] * Position_out[1] + R[8] * Position_out[2] + m_3DPositionTransformFromOuputToInput_Offset[2];
		return Position_in;
	}
}

}// namespace mdk

