#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::DenseImageResampler2D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageResampler2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	auto& Self = *this;
	Self.InputImage = nullptr;
	Self.ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Linear;
	Self.ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Constant;
	Self.ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);
	Self.OutputImage.Clear();
	Self.MaxThreadCount = 1;

	Self.Flag_Input_Output_Orientation_IdentityMatrix = false;
	Self.Flag_Input_Output_SameOrigin_SameOrientation = false;
	Self.Position3DTransformFromOuputToInput_Matrix.Clear();
	Self.Position3DTransformFromOuputToInput_Offset.Clear();
	Self.Position3DTransformFromInputToOutput_Matrix.Clear();
	Self.Position3DTransformFromInputToOutput_Offset.Clear();
	Self.Position2DTransform_from_OutputImage_to_InputImage = nullptr;

	Self.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Nothing;
	Self.Flag_SmoothInputImage = false;
	Self.SmoothedImage.Clear();	
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::EnableTriangleSmoothingWhenDownsampling(bool On_Off)
{
	auto& Self = *this;
	if (On_Off == true)
	{
		Self.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Triangle;
	}
	else
	{
		Self.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Nothing;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::EnableAverageSmoothingWhenDownsampling(bool On_Off)
{
	auto& Self = *this;
	if (On_Off == true)
	{
		Self.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Average;
	}
	else
	{
		Self.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Nothing;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::SmoothInputImageIfNecessary()
{
	auto& Self = *this;
	Self.SmoothedImage.Clear();
	Self.Flag_SmoothInputImage = false;
	if (Self.SmoothingMethodWhenDownsmapling != SmoothingMethodTypeWhenDownsmapling::Nothing)
	{
		auto InputSpacing = Self.InputImage->GetSpacing();
		auto OutputSpacing = Self.OutputImage.GetSpacing();
		for (int_max k = 0; k < 2; ++k)
		{
			auto Ratio = OutputSpacing[k] / InputSpacing[k];
			if (Ratio > 1.2)
			{
				Self.Flag_SmoothInputImage = true;
			}
		}

		if (Flag_SmoothInputImage == true)
		{			
			if (Self.SmoothingMethodWhenDownsmapling == SmoothingMethodTypeWhenDownsmapling::Average)
			{
				IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType> SmoothingFilter;
				SmoothingFilter.InputImage = Self.InputImage;
				SmoothingFilter.SetOutputImageInfo(Self.InputImage->GetInfo());
				SmoothingFilter.Radius = OutputSpacing;
				SmoothingFilter.MaxThreadCount = Self.MaxThreadCount;
				SmoothingFilter.Update();
				Self.SmoothedImage = std::move(SmoothingFilter.OutputImage);
			}
			else if (Self.SmoothingMethodWhenDownsmapling == SmoothingMethodTypeWhenDownsmapling::Triangle)
			{
				DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType> SmoothingFilter;
				SmoothingFilter.InputImage = Self.InputImage;
				SmoothingFilter.CreateTriangleMask(Self.InputImage->GetSpacing(), OutputSpacing[0], OutputSpacing[1]);
				SmoothingFilter.MaxThreadCount = Self.MaxThreadCount;
				SmoothingFilter.Update();
				Self.SmoothedImage = std::move(SmoothingFilter.OutputImage);
			}
		}
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo2D& Info)
{
	auto& Self = *this;
	auto Size_old = Self.OutputImage.GetSize();
	if (Size_old[0] != Info.Size[0] || Size_old[1] != Info.Size[1])
	{
		Self.OutputImage.Clear();
		Self.OutputImage.SetInfo(Info);
	}
	else
	{// no new memory allocation
		Self.OutputImage.SetInfo(Info, false);
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::
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
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 2>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly)
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ DenseImageResampler2D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 1 || Ly <= 1)
	{
		MDK_Error("Invalid input @ DenseImageResampler2D::SetOutputImageInfoBySize(...)")
		return;
	}

	ImageInfo2D Info;
	Info.Size[0] = Lx;
	Info.Size[1] = Ly;

	auto Size_input = Self.InputImage->GetSize();
	auto Spacing_input = Self.InputImage->GetSpacing();
	Info.Spacing[0] = double(Size_input[0] - 1) * Spacing_input[0] / double(Lx - 1);
	Info.Spacing[1] = double(Size_input[1] - 1) * Spacing_input[1] / double(Ly - 1);

	Info.Origin = Self.InputImage->GetOrigin();
	Info.Orientation = Self.InputImage->GetOrientation();
	Info.UpdateTransformMatrix();

	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 2>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y)
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ DenseImageResampler2D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_y <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ DenseImageResampler2D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	ImageInfo2D Info;

	Info.Spacing[0] = Spacing_x;
	Info.Spacing[1] = Spacing_y;

	auto Size_input = Self.InputImage->GetSize();
	auto Spacing_input = Self.InputImage->GetSpacing();
	Info.Size[0] = int_max(std::ceil(Size_input[0] * Spacing_input[0] / Spacing_x));
	Info.Size[1] = int_max(std::ceil(Size_input[1] * Spacing_input[1] / Spacing_y));

	Info.Origin = Self.InputImage->GetOrigin();
	Info.Orientation = Self.InputImage->GetOrientation();
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
{
	auto& Self = *this;
	auto InputImageInfo = Self.InputImage->GetInfo();
	auto OutputImageInfo = Self.OutputImage.GetInfo();
	{
		auto M = InputImageInfo.Orientation.Inv();
		Self.Position3DTransformFromOuputToInput_Matrix = MatrixMultiply(M, OutputImageInfo.Orientation);
		auto D = OutputImageInfo.Origin - InputImageInfo.Origin;
		// Self.Position2DTransformFromOuputToInput_Offset = M*D
		Self.Position3DTransformFromOuputToInput_Offset[0] = M[0] * D[0] + M[3] * D[1] + M[6] * D[2];
		Self.Position3DTransformFromOuputToInput_Offset[1] = M[1] * D[0] + M[4] * D[1] + M[7] * D[2];
		Self.Position3DTransformFromOuputToInput_Offset[2] = M[2] * D[0] + M[5] * D[1] + M[8] * D[2];
	}

	{
		auto M = OutputImageInfo.Orientation.Inv();
		Self.Position3DTransformFromInputToOutput_Matrix = MatrixMultiply(M, InputImageInfo.Orientation);
		auto D = InputImageInfo.Origin - OutputImageInfo.Origin;
		// Self.Position2DTransformFromInputToOutput_Offset = M*D
		Self.Position3DTransformFromInputToOutput_Offset[0] = M[0] * D[0] + M[3] * D[1] + M[6] * D[2];
		Self.Position3DTransformFromInputToOutput_Offset[1] = M[1] * D[0] + M[4] * D[1] + M[7] * D[2];
		Self.Position3DTransformFromInputToOutput_Offset[2] = M[2] * D[0] + M[5] * D[1] + M[8] * D[2];
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
		Self.Flag_Input_Output_SameOrigin_SameOrientation = true;
	}

	Self.Flag_Input_Output_Orientation_IdentityMatrix = false;
	if (InputImageInfo.Orientation.IsIdentityMatrix() == true && OutputImageInfo.Orientation.IsIdentityMatrix() == true)
	{
		Self.Flag_Input_Output_Orientation_IdentityMatrix = true;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseVector<ScalarType, 2> DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::
Transform2DPositionInInputImageTo2DPositionInOutputImage(const DenseVector<ScalarType, 2>& Position_in)
{
	auto& Self = *this;
	if (Self.Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_in;
	}
	else if (Self.Flag_Input_Output_Orientation_IdentityMatrix == true)
	{
		auto Position_out = Self.InputImage->GetOrigin2D() - Self.OutputImage.GetOrigin2D() + Position_in;
		return Position_out;
	}
	else
	{
		auto R = Self.Position3DTransformFromInputToOutput_Matrix.GetElementPointer();
		auto T = Self.Position3DTransformFromInputToOutput_Offset.GetElementPointer();
		DenseVector<ScalarType, 2> Position_out;
		//Position_out[0] = R[0] * Position_in[0] + R[3] * Position_in[1] + R[6] * Position_in[2] + T[0];
		//Position_out[1] = R[1] * Position_in[0] + R[4] * Position_in[1] + R[7] * Position_in[2] + T[1];
		//Position_out[2] = R[2] * Position_in[0] + R[5] * Position_in[1] + R[8] * Position_in[2] + T[2];
		Position_out[0] = R[0] * Position_in[0] + R[3] * Position_in[1] + T[0];
		Position_out[1] = R[1] * Position_in[0] + R[4] * Position_in[1] + T[1];
		return Position_out;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseVector<ScalarType, 2> DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::
Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out)
{
	auto& Self = *this;
	if (Self.Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_out;
	}
	else if (Self.Flag_Input_Output_Orientation_IdentityMatrix == true)
	{
		auto Position_in = Self.OutputImage.GetOrigin2D() - Self.InputImage->GetOrigin2D() + Position_out;
		return Position_in;
	}
	else
	{
		auto R = Self.Position3DTransformFromOuputToInput_Matrix.GetElementPointer();
		auto T = Self.Position3DTransformFromOuputToInput_Offset.GetElementPointer();
		DenseVector<ScalarType, 2> Position_in;
		//Position_in[0] = R[0] * Position_out[0] + R[3] * Position_out[1] + R[6] * Position_out[2] + T[0];
		//Position_in[1] = R[1] * Position_out[0] + R[4] * Position_out[1] + R[7] * Position_out[2] + T[1];
		//Position_in[2] = R[2] * Position_out[0] + R[5] * Position_out[1] + R[8] * Position_out[2] + T[2];
		Position_in[0] = R[0] * Position_out[0] + R[3] * Position_out[1] + T[0];
		Position_in[1] = R[1] * Position_out[0] + R[4] * Position_out[1] + T[1];
		return Position_in;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ DenseImageResampler2D::SelfCheckInput()")
		return false;
	}

	if (Self.InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ DenseImageResampler2D::CheckInput()")
		return false;
	}
	
	if (Self.OutputImage.IsEmpty() == true)
	{
		MDK_Error("Output image is Empty, call SetOutputImageInfo first @ DenseImageResampler2D::CheckInput()")
		return false;
	}

	// OutputImage and InputImage should be in the same x-y plane
	auto Info_in = Self.InputImage->GetInfo();
	auto Info_out = Self.OutputImage.GetInfo();
	DenseVector<double, 3> DirectionZ_in, DirectionZ_out;	
	Info_in.Orientation.GetCol(2, DirectionZ_in);
	Info_out.Orientation.GetCol(2, DirectionZ_out);
	double diff = (DirectionZ_in - DirectionZ_out).L2Norm();
	if (diff > 0.000001)
	{
		MDK_Error("OutputImage and InputImage NOT in the same x-y plane (DirectionZ) @ DenseImageResampler2D::CheckInput()")
		return false;
	}
	auto Displacement = Info_out.Origin - Info_in.Origin;
	auto Proj = Displacement[0] * DirectionZ_in[0] + Displacement[1] * DirectionZ_in[1] + Displacement[2] * DirectionZ_in[2];
	Proj = std::abs(Proj);
	if (Proj > 0.000001)
	{
		MDK_Error("OutputImage and InputImage NOT in the same x-y plane (Origin) @ DenseImageResampler2D::CheckInput()")
		return false;
	}	
	//------------------------------------------------------------

	if (Self.MaxThreadCount <= 0)
	{
		MDK_Error("MaxThreadCount <= 0 @ DenseImageResampler2D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->Update3DPositionTransform_Input_Output();

	this->SmoothInputImageIfNecessary();

	//---------------------------------------------------------------------------------
	auto& Self = *this;
	auto Size = Self.OutputImage.GetSize();
	//for (int_max y = 0; y <= Size[1]-1; ++y)
	auto TempFunction = [&](int_max y)
	{			
		for (int_max x = 0; x < Size[0]; ++x)
		{
			auto Pos_out = Self.OutputImage.Transform2DIndexTo2DPosition<ScalarType>(x, y);
			DenseVector<ScalarType, 2> Pos_in;
			if (Self.Position2DTransform_from_OutputImage_to_InputImage != nullptr)
			{
				Pos_in = Self.Position2DTransform_from_OutputImage_to_InputImage->TransformPoint(Pos_out);
			}
			else
			{
				Pos_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(Pos_out);
			}
			if (Self.Flag_SmoothInputImage == false)
			{
				Self.OutputImage(x, y) = Self.InputImage->GetPixelAt2DPosition<OutputPixelType>(Pos_in, Self.ImageInterpolationOption);
			}
			else
			{
				Self.OutputImage(x, y) = Self.SmoothedImage.GetPixelAt2DPosition<OutputPixelType>(Pos_in, Self.ImageInterpolationOption);
			}
		}		
	};
	ParallelForLoop(TempFunction, 0, Size[1]-1, Self.MaxThreadCount);
	//---------------------------------------------------------------------------------
	Self.SmoothedImage.Clear();
}

}

