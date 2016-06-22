#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::IntegralImageBasedAverageDenseImageFilter2D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::~IntegralImageBasedAverageDenseImageFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	m_InputImage = nullptr;
	m_ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Linear;
	m_ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Constant;
	m_ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);
	m_OutputImage.Clear();
	m_MaxThreadCount = 1;

	m_Flag_Input_Output_Orientation_IdentityMatrix = false;
	m_Flag_Input_Output_SameOrigin_SameOrientation = false;
	m_3DPositionTransformFromOuputToInput_Matrix.Clear();
	m_3DPositionTransformFromOuputToInput_Offset.Clear();

	m_IntegralImage_Internal.Clear();
	m_IntegralImage = &m_IntegralImage_Internal;
	m_Radius = { 0, 0};
	m_Radius_Index2D = { 0, 0};
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo2D& Info)
{
	auto Size_old = m_OutputImage.GetSize();
	if (Size_old[0] != Info.Size[0] || Size_old[1] != Info.Size[1])
	{
		m_OutputImage.Clear();
		m_OutputImage.SetInfo(Info);
	}
	else
	{// no new memory allocation
		m_OutputImage.SetInfo(Info, false);
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
SetOutputImageInfo(const DenseVector<double, 3>& Origin, const DenseVector<double, 2>& Spacing, const DenseVector<int_max, 2>& Size, const DenseMatrix<double>& Orientation)
{
	ImageInfo2D Info;
	Info.Origin = Origin;
	Info.Spacing = Spacing;
	Info.Size = Size;
	Info.Orientation = Orientation;
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 2>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ IntegralImageBasedAverageDenseImageFilter2D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 0 || Ly <= 0)
	{
		MDK_Error("Invalid input @ IntegralImageBasedAverageDenseImageFilter2D::SetOutputImageInfoBySize(...)")
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
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 2>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ IntegralImageBasedAverageDenseImageFilter2D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_y <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ IntegralImageBasedAverageDenseImageFilter2D::SetOutputImageInfoBySpacing(...)")
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
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
{
	auto InputImageInfo = m_InputImage->GetInfo();
	auto OutputImageInfo = m_OutputImage.GetInfo();
	{
		auto M = InputImageInfo.Orientation.Inv();
		m_3DPositionTransformFromOuputToInput_Matrix = MatrixMultiply(M, OutputImageInfo.Orientation);
		auto D = OutputImageInfo.Origin - InputImageInfo.Origin;
		// m_2DPositionTransformFromOuputToInput_Offset = M*D
		m_3DPositionTransformFromOuputToInput_Offset[0] = M[0] * D[0] + M[3] * D[1] + M[6] * D[2];
		m_3DPositionTransformFromOuputToInput_Offset[1] = M[1] * D[0] + M[4] * D[1] + M[7] * D[2];
		m_3DPositionTransformFromOuputToInput_Offset[2] = M[2] * D[0] + M[5] * D[1] + M[8] * D[2];
	}

	auto Eps = std::numeric_limits<double>::epsilon();

	bool Flag_Input_Output_SameOrigin = false;
	auto OriginDiff = (InputImageInfo.Origin - OutputImageInfo.Origin).L2Norm();
	if (OriginDiff <= Eps * 3)
	{
		Flag_Input_Output_SameOrigin = true;
	}
	else
	{
		Flag_Input_Output_SameOrigin = false;
	}

	bool Flag_Input_Output_SameOrientation = false;
	DenseMatrix<double> OrientationDiff = MatrixSubtract(InputImageInfo.Orientation, OutputImageInfo.Orientation);
	OrientationDiff.ElementOperation("abs");
	auto SumAbsDiff = OrientationDiff.Sum();
	if (SumAbsDiff <= Eps*9.0)// 9 element in matrix
	{
		Flag_Input_Output_SameOrientation = true;
	}
	else
	{
		Flag_Input_Output_SameOrientation = false;
	}

	if (Flag_Input_Output_SameOrigin == true && Flag_Input_Output_SameOrientation == true)
	{
		m_Flag_Input_Output_SameOrigin_SameOrientation = true;
	}

	m_Flag_Input_Output_Orientation_IdentityMatrix = false;
	if (InputImageInfo.Orientation.IsIdentityMatrix() == true && OutputImageInfo.Orientation.IsIdentityMatrix() == true)
	{
		m_Flag_Input_Output_Orientation_IdentityMatrix = true;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseVector<ScalarType, 2> IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out)
{
	if (m_Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_out;
	}
	else if (m_Flag_Input_Output_Orientation_IdentityMatrix == true)
	{
		auto Position_in = m_OutputImage.GetOrigin2D() - m_InputImage->GetOrigin2D() + Position_out;
		return Position_in;
	}
	else
	{
		auto R = m_3DPositionTransformFromOuputToInput_Matrix.GetElementPointer();
		auto T = m_3DPositionTransformFromOuputToInput_Offset.GetElementPointer();
		DenseVector<ScalarType, 2> Position_in;
		Position_in[0] = R[0] * Position_out[0] + R[3] * Position_out[1] + R[6] * Position_out[2] + T[0];
		Position_in[1] = R[1] * Position_out[0] + R[4] * Position_out[1] + R[7] * Position_out[2] + T[1];
		//Position_in[2] = R[2] * Position_out[0] + R[5] * Position_out[1] + R[8] * Position_out[2] + T[2];
		return Position_in;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetIntegralImage(const DenseImage2D<InputPixelType>* IntegralImage)
{
	m_IntegralImage = IntegralImage;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetRadius(double RadiusX, double RadiusY)
{
	m_Radius[0] = RadiusX;
	m_Radius[1] = RadiusY;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::BuildIntegralImageIfNecessary()
{
	auto Size = m_InputImage->GetSize();
	if (Size[0] < 2 || Size[1] < 2)
	{
		MDK_Error(" SizeX or SizeY of InputImage < 2 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
		return false;
	}

	if (m_IntegralImage == nullptr)
	{
		m_IntegralImage = &m_IntegralImage_Internal;
	}
	else
	{
		if (m_IntegralImage != &m_IntegralImage_Internal)
		{
			if (m_IntegralImage->IsEmpty() == true)
			{
				MDK_Error("Input IntegralImage is empty @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary(...)")
				return false;
			}

			auto Size = m_IntegralImage->GetSize();
			if (Size[0] < 2 || Size[1] < 2)
			{
				MDK_Error(" SizeX or SizeY of Input IntegralImage < 2 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
				return false;
			}
		}
	}

	auto Spacing = m_OutputImage.GetSpacing();
	m_Radius_Index2D[0] = m_Radius[0] / Spacing[0];
	m_Radius_Index2D[1] = m_Radius[1] / Spacing[1];

	if (m_Radius_Index2D[0] <= 0 || m_Radius_Index2D[1] <= 0)
	{
		MDK_Error(" RadiusX or RadiusY in Index2D <= 0 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
		return false;
	}

	m_IntegralImage_Internal.Clear();
	if (m_IntegralImage == &m_IntegralImage_Internal)
	{
		IntegralImageBuilder2D<double> ImBuilder;
		ImBuilder.SetInputImage(m_InputImage);
		ImBuilder.Update();
		m_IntegralImage_Internal = std::move(ImBuilder.OutputImage());
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ IntegralImageBasedAverageDenseImageFilter2D::SelfCheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ IntegralImageBasedAverageDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (m_MaxThreadCount <= 0)
	{
	    MDK_Error("m_MaxThreadCount <= 0 @ IntegralImageBasedAverageDenseImageFilter2D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->Update3DPositionTransform_Input_Output();

	if (this->BuildIntegralImageIfNecessary() == false)
	{
		return;
	}

	//--------------------------------------------------------------------------------
	auto PixelCount = m_OutputImage.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		m_OutputImage[k] = this->EvaluateAtPixelInOutputImage(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, m_MaxThreadCount);
	//---------------------------------------------------------------------------------
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAtPixelInOutputImage(int_max LinearIndex)
{
	auto Pos_out = m_OutputImage.TransformLinearIndexTo2DPosition<ScalarType>(LinearIndex);
	auto Pos_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(Pos_out);
	auto x0 = Pos_in[0];
	auto y0 = Pos_in[1];

	// Index2D=[x, y] : ScalarType
	auto Index2D = m_IntegralImage->Transform2DPositionTo2DIndex(x0, y0);
	auto Size = m_IntegralImage->GetSize();
	//---------------------------------------------------------
	// x1 <= x0 <= x2, y1 <= y0 <= y2
	//
	// y 
	// ^            X2
	// |       X0
	// |   X1 
	// |------------->x

	//auto x1 = int_max(std::round(Index2D[0] - m_Radius_Index2D[0]-1));
	//auto x2 = int_max(std::round(Index2D[0] + m_Radius_Index2D[0]));
	//auto y1 = int_max(std::round(Index2D[1] - m_Radius_Index2D[1]-1));
	//auto y2 = int_max(std::round(Index2D[1] + m_Radius_Index2D[1]));
	auto x1 = int_max(Index2D[0] - m_Radius_Index2D[0] - 1);
	auto x2 = int_max(Index2D[0] + m_Radius_Index2D[0] + 0.5);
	auto y1 = int_max(Index2D[1] - m_Radius_Index2D[1] - 1);
	auto y2 = int_max(Index2D[1] + m_Radius_Index2D[1] + 0.5);

	// if the window is outside the image
	if (x1 >= Size[0] || x2 < 0 || y1 >= Size[1] || y2 < 0)
	{
		return OutputPixelType(m_ImageInterpolationOption.Pixel_OutsideImage);
	}

	// now, the window overlap with the image
	
	if (x1 < 0)	{ x1 = 0; }
	if (x2 >= Size[0]) { x2 = Size[0] - 1; }
	if (x1 == x2) 
	{ 
		if (x1 > 0)
		{
			x1 = x1 - 1;
		}
		else
		{
			x2 = x2 + 1;
		}
	} 
	
	if (y1 < 0)	{ y1 = 0; }
	if (y2 >= Size[1]) {y2 = Size[1] - 1;}
	if (y1 == y2)
	{
		if (y1 > 0)
		{
			y1 = y1 - 1;
		}
		else
		{
			y2 = y2 + 1;
		}
	}

	//---------------------------------------------------------
	auto Pixel_x1y1 = (*m_IntegralImage)(x1, y1);
	auto Pixel_x1y2 = (*m_IntegralImage)(x1, y2);
	auto Pixel_x2y1 = (*m_IntegralImage)(x2, y1);
	auto Pixel_x2y2 = (*m_IntegralImage)(x2, y2);
	//---------------------------------------------------------
	OutputPixelType OutputPixel;
	OutputPixel = Pixel_x2y2 - Pixel_x1y2 - Pixel_x2y1 + Pixel_x1y1;
	OutputPixel /= ScalarType((y2 - y1)*(x2 - x1));
	return OutputPixel;	
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
const DenseImage2D<InputPixelType>* IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::GetIntegralImage()
{
	return m_IntegralImage;
}


}//end namespace mdk

