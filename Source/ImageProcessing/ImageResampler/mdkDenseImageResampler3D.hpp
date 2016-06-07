#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::DenseImageResampler3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageResampler3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	m_InputImage = nullptr;
	m_ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Linear;
	m_ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Constant;
	m_ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);	
	m_Flag_Input_Output_SameOrigin = false;
	m_Flag_Input_Output_SameSpacing = false;
	m_Flag_Input_Output_SameOrientation = false;
	m_Flag_Input_Output_SameOrigin_SameOrientation = false;
	m_OutputImage.Clear();
	m_MaxThreadCount = 1;

	m_Flag_SmoothWhenDownsmapling = false;
	m_Flag_SmoothInputImage = false;
	m_SmoothedImage.Clear();

	m_3DPositionTransform_from_OutputImage_to_InputImage = nullptr;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::EnableSmoothingWhenDownsampling(bool On_Off)
{
	m_Flag_SmoothWhenDownsmapling = On_Off;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SmoothInputImageIfNecessary()
{
	m_Flag_SmoothInputImage = false;
	m_SmoothedImage.Clear();

	if (m_Flag_SmoothWhenDownsmapling == true)
	{
		auto InputSpacing = m_InputImage->GetSpacing();
		auto OutputSpacing = m_OutputImage.GetSpacing();
		for (int_max k = 0; k < 3; ++k)
		{
			auto Ratio = OutputSpacing[k] / InputSpacing[k];
			if (Ratio > 1.5)
			{
				m_Flag_SmoothInputImage = true;
			}
		}

		if (m_Flag_SmoothInputImage == true)
		{
			IntegralImageBasedAverageDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType> SmoothingFilter;
			SmoothingFilter.SetInputImage(m_InputImage);
			SmoothingFilter.SetOutputImageInfo(m_InputImage->GetInfo());
			SmoothingFilter.SetRadius(OutputSpacing[0], OutputSpacing[1], OutputSpacing[2]);
			SmoothingFilter.SetMaxThreadCount(m_MaxThreadCount);
			SmoothingFilter.Update();
			m_SmoothedImage = std::move(SmoothingFilter.OutputImage());
		}
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo3D& Info)
{
	auto Size_old = m_OutputImage.GetSize();
	if (Size_old[0] != Info.Size[0] || Size_old[1] != Info.Size[1] || Size_old[2] != Info.Size[2])
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
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
SetOutputImageInfo(const DenseVector<double, 3>& Origin, const DenseVector<double, 3>& Spacing, const DenseVector<int_max, 3>& Size, const DenseMatrix<double>& Orientation)
{
	ImageInfo3D Info;
	Info.Origin = Origin;
	Info.Spacing = Spacing;
	Info.Size = Size;
	Info.Orientation = Orientation;
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1], Size[2]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ DenseImageResampler3D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 1 || Ly <= 1 || Lz <= 1)
	{
		MDK_Error("Invalid input @ DenseImageResampler3D::SetOutputImageInfoBySize(...)")
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
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 3>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1], Spacing[2]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ DenseImageResampler3D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_y <= Zero || Spacing_z <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ DenseImageResampler3D::SetOutputImageInfoBySpacing(...)")
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
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
{
	auto InputImageInfo = m_InputImage->GetInfo();
	auto OutputImageInfo = m_OutputImage.GetInfo();
	{
		auto M = InputImageInfo.Orientation.Inv();
		m_3DPositionTransformFromOuputToInput_Matrix = MatrixMultiply(M, OutputImageInfo.Orientation);
		auto D = OutputImageInfo.Origin - InputImageInfo.Origin;
		// m_3DPositionTransformFromOuputToInput_Offset = M*D
		m_3DPositionTransformFromOuputToInput_Offset[0] = M[0] * D[0] + M[3] * D[1] + M[6] * D[2];
		m_3DPositionTransformFromOuputToInput_Offset[1] = M[1] * D[0] + M[4] * D[1] + M[7] * D[2];
		m_3DPositionTransformFromOuputToInput_Offset[2] = M[2] * D[0] + M[5] * D[1] + M[8] * D[2];
	}

	{
		auto M = OutputImageInfo.Orientation.Inv();
		m_3DPositionTransformFromInputToOutput_Matrix = MatrixMultiply(M, InputImageInfo.Orientation);
		auto D = InputImageInfo.Origin - OutputImageInfo.Origin;
		// m_3DPositionTransformFromInputToOutput_Offset = M*D
		m_3DPositionTransformFromInputToOutput_Offset[0] = M[0] * D[0] + M[3] * D[1] + M[6] * D[2];
		m_3DPositionTransformFromInputToOutput_Offset[1] = M[1] * D[0] + M[4] * D[1] + M[7] * D[2];
		m_3DPositionTransformFromInputToOutput_Offset[2] = M[2] * D[0] + M[5] * D[1] + M[8] * D[2];
	}

	auto Eps = std::numeric_limits<double>::epsilon();

	auto OriginDiff = (InputImageInfo.Origin - OutputImageInfo.Origin).L2Norm();
	if (OriginDiff <= Eps * 3)
	{
		m_Flag_Input_Output_SameOrigin = true;
	}
	else
	{
		m_Flag_Input_Output_SameOrigin = false;
	}

	auto SpacingDiff = (InputImageInfo.Spacing - OutputImageInfo.Spacing).L2Norm();
	if (SpacingDiff <= Eps * 3)
	{
		m_Flag_Input_Output_SameSpacing = true;
	}
	else
	{
		m_Flag_Input_Output_SameSpacing = false;
	}

	DenseMatrix<double> OrientationDiff = MatrixSubtract(InputImageInfo.Orientation, OutputImageInfo.Orientation);
	OrientationDiff.ElementOperation("abs");
	auto SumAbsDiff = OrientationDiff.Sum();
	if (SumAbsDiff <= Eps*9.0)// 9 element in matrix
	{
		m_Flag_Input_Output_SameOrientation = true;
	}
	else
	{
		m_Flag_Input_Output_SameOrientation = false;
	}

	if (m_Flag_Input_Output_SameOrigin == true && m_Flag_Input_Output_SameOrientation == true)
	{
		m_Flag_Input_Output_SameOrigin_SameOrientation = true;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
Transform3DPositionInInputImageTo3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position_in)
{
	if (m_Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_in;
	}
	else if (m_Flag_Input_Output_SameOrientation == true)
	{
		auto Position_out = m_InputImage->GetOrigin() - m_OutputImage.GetOrigin() + Position_in;
		return Position_out;
	}
	else
	{
		auto R = m_3DPositionTransformFromInputToOutput_Matrix.GetElementPointer();
		auto T = m_3DPositionTransformFromInputToOutput_Offset.GetElementPointer();
		DenseVector<ScalarType, 3> Position_out;
		Position_out[0] = R[0] * Position_in[0] + R[3] * Position_in[1] + R[6] * Position_in[2] + T[0];
		Position_out[1] = R[1] * Position_in[0] + R[4] * Position_in[1] + R[7] * Position_in[2] + T[1];
		Position_out[2] = R[2] * Position_in[0] + R[5] * Position_in[1] + R[8] * Position_in[2] + T[2];
		return Position_out;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out)
{
	if (m_Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_out;
	}
	else if (m_Flag_Input_Output_SameOrientation == true)
	{
		auto Position_in = m_OutputImage.GetOrigin() - m_InputImage->GetOrigin() + Position_out;
		return Position_in;
	}
	else
	{
		auto R = m_3DPositionTransformFromOuputToInput_Matrix.GetElementPointer();
		auto T = m_3DPositionTransformFromOuputToInput_Offset.GetElementPointer();
		DenseVector<ScalarType, 3> Position_in;
		Position_in[0] = R[0] * Position_out[0] + R[3] * Position_out[1] + R[6] * Position_out[2] + T[0];
		Position_in[1] = R[1] * Position_out[0] + R[4] * Position_out[1] + R[7] * Position_out[2] + T[1];
		Position_in[2] = R[2] * Position_out[0] + R[5] * Position_out[1] + R[8] * Position_out[2] + T[2];
		return Position_in;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ DenseImageResampler3D::SelfCheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ DenseImageResampler3D::CheckInput()")
		return false;
	}

	if (m_OutputImage.IsEmpty() == true)
	{
		MDK_Error("OutputImage is Empty, call SetOutputImageInfo first  @ DenseImageResampler3D::CheckInput()")
		return false;
	}

	if (m_MaxThreadCount <= 0)
	{
		MDK_Error("m_MaxThreadCount <= 0 @ DenseImageResampler3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->Update3DPositionTransform_Input_Output();

	this->SmoothInputImageIfNecessary();

	//--------------------------------------------------------------------------------
	// this is slow
	/*
	auto PixelCount = m_OutputImage.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		auto Pos_out = m_OutputImage.TransformLinearIndexTo3DPosition<ScalarType>(k);
		DenseVector<ScalarType, 3> Pos_in;
		if (m_3DPositionTransform_from_OutputImage_to_InputImage != nullptr)
		{
			Pos_in = m_3DPositionTransform_from_OutputImage_to_InputImage->TransformPoint(Pos_out);
		}
		else
		{
			Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
		}		
		if (m_Flag_SmoothInputImage == false)
		{
			m_OutputImage[k] = m_InputImage->GetPixelAt3DPosition<OutputPixelType>(Pos_in, m_ImageInterpolationOption);
		}
		else
		{
			m_OutputImage[k] = m_SmoothedImage.GetPixelAt3DPosition<OutputPixelType>(Pos_in, m_ImageInterpolationOption);
		}
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, m_MaxThreadCount);
	*/
	//---------------------------------------------------------------------------------
	auto Size = m_OutputImage.GetSize();
	//for (int_max z = 0; z <= Size[2]-1; ++z)
	auto TempFunction = [&](int_max z)
	{
		for (int_max y = 0; y < Size[1]; ++y)
		{
			for (int_max x = 0; x < Size[0]; ++x)
			{
				auto Pos_out = m_OutputImage.Transform3DIndexTo3DPosition<ScalarType>(x,y,z);
				DenseVector<ScalarType, 3> Pos_in;
				if (m_3DPositionTransform_from_OutputImage_to_InputImage != nullptr)
				{
					Pos_in = m_3DPositionTransform_from_OutputImage_to_InputImage->TransformPoint(Pos_out);
				}
				else
				{
					Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
				}
				if (m_Flag_SmoothInputImage == false)
				{
					m_OutputImage(x,y,z) = m_InputImage->GetPixelAt3DPosition<OutputPixelType>(Pos_in, m_ImageInterpolationOption);
				}
				else
				{
					m_OutputImage(x,y,z) = m_SmoothedImage.GetPixelAt3DPosition<OutputPixelType>(Pos_in, m_ImageInterpolationOption);
				}
			}
		}
	};
	ParallelForLoop(TempFunction, 0, Size[2]-1, m_MaxThreadCount);
	//---------------------------------------------------------------------------------
	m_SmoothedImage.Clear();
}

}

