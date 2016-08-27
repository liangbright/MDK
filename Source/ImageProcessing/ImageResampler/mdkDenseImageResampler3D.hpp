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
	Self.Position3DTransform_from_OutputImage_to_InputImage = nullptr;

	Self.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Nothing;	
	Self.Flag_SmoothInputImage = false;
	Self.SmoothedImage.Clear();	
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::EnableTriangleSmoothingWhenDownsampling(bool On_Off)
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
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::EnableAverageSmoothingWhenDownsampling(bool On_Off)
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
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SmoothInputImageIfNecessary()
{
	auto& Self = *this;

	Self.SmoothedImage.Clear();

	Self.Flag_SmoothInputImage = false;
	if (Self.SmoothingMethodWhenDownsmapling != SmoothingMethodTypeWhenDownsmapling::Nothing)
	{
		auto InputSpacing = Self.InputImage->GetSpacing();
		auto OutputSpacing = Self.OutputImage.GetSpacing();
		for (int_max k = 0; k < 3; ++k)
		{
			auto Ratio = OutputSpacing[k] / InputSpacing[k];
			if (Ratio > 1.2)
			{
				Self.Flag_SmoothInputImage = true;
			}
		}

		if (Self.Flag_SmoothInputImage == true)
		{
			if (Self.SmoothingMethodWhenDownsmapling == SmoothingMethodTypeWhenDownsmapling::Average)
			{
				IntegralImageBasedAverageDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType> SmoothingFilter;
				SmoothingFilter.InputImage = Self.InputImage;
				SmoothingFilter.SetOutputImageInfo(Self.InputImage->GetInfo());
				SmoothingFilter.Radius = OutputSpacing;
				SmoothingFilter.MaxThreadCount = Self.MaxThreadCount;
				SmoothingFilter.Update();
				Self.SmoothedImage = std::move(SmoothingFilter.OutputImage);
			}
			else if (Self.SmoothingMethodWhenDownsmapling == SmoothingMethodTypeWhenDownsmapling::Triangle)
			{
				DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType> SmoothingFilter;
				SmoothingFilter.InputImage = Self.InputImage;
				SmoothingFilter.CreateTriangleMask(Self.InputImage->GetSpacing(), OutputSpacing[0], OutputSpacing[1], OutputSpacing[2]);
				SmoothingFilter.MaxThreadCount = Self.MaxThreadCount;
				SmoothingFilter.Update();
				Self.SmoothedImage = std::move(SmoothingFilter.OutputImage);
			}
		}
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo3D& Info)
{
	auto& Self = *this;
	auto Size_old = Self.OutputImage.GetSize();
	if (Size_old[0] != Info.Size[0] || Size_old[1] != Info.Size[1] || Size_old[2] != Info.Size[2])
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
	auto& Self = *this;
	if (Self.InputImage == nullptr)
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

	auto Size_input = Self.InputImage->GetSize();
	auto Spacing_input = Self.InputImage->GetSpacing();
	Info.Spacing[0] = double(Size_input[0] - 1) * Spacing_input[0] / double(Lx - 1);
	Info.Spacing[1] = double(Size_input[1] - 1) * Spacing_input[1] / double(Ly - 1);
	Info.Spacing[2] = double(Size_input[2] - 1) * Spacing_input[2] / double(Lz - 1);

	Info.Origin = Self.InputImage->GetOrigin();
	Info.Orientation = Self.InputImage->GetOrientation();
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
	auto& Self = *this;
	if (Self.InputImage == nullptr)
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

	auto Size_input = Self.InputImage->GetSize();
	auto Spacing_input = Self.InputImage->GetSpacing();
	Info.Size[0] = int_max(std::ceil(Size_input[0] * Spacing_input[0] / Spacing_x));
	Info.Size[1] = int_max(std::ceil(Size_input[1] * Spacing_input[1] / Spacing_y));
	Info.Size[2] = int_max(std::ceil(Size_input[2] * Spacing_input[2] / Spacing_z));

	Info.Origin = Self.InputImage->GetOrigin();
	Info.Orientation = Self.InputImage->GetOrientation();
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
{
	auto& Self = *this;
	auto InputImageInfo = Self.InputImage->GetInfo();
	auto OutputImageInfo = Self.OutputImage.GetInfo();
	{
		auto M = InputImageInfo.Orientation.Inv();
		Self.Position3DTransformFromOuputToInput_Matrix = MatrixMultiply(M, OutputImageInfo.Orientation);
		auto D = OutputImageInfo.Origin - InputImageInfo.Origin;
		// Self.Position3DTransformFromOuputToInput_Offset = M*D
		Self.Position3DTransformFromOuputToInput_Offset[0] = M[0] * D[0] + M[3] * D[1] + M[6] * D[2];
		Self.Position3DTransformFromOuputToInput_Offset[1] = M[1] * D[0] + M[4] * D[1] + M[7] * D[2];
		Self.Position3DTransformFromOuputToInput_Offset[2] = M[2] * D[0] + M[5] * D[1] + M[8] * D[2];
	}

	{
		auto M = OutputImageInfo.Orientation.Inv();
		Self.Position3DTransformFromInputToOutput_Matrix = MatrixMultiply(M, InputImageInfo.Orientation);
		auto D = InputImageInfo.Origin - OutputImageInfo.Origin;
		// Self.Position3DTransformFromInputToOutput_Offset = M*D
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

	bool Flag_Input_Output_SameOrientation = false;
	DenseMatrix<double> OrientationDiff = MatrixSubtract(InputImageInfo.Orientation, OutputImageInfo.Orientation);
	OrientationDiff.ElementOperation("abs");
	auto SumAbsDiff = OrientationDiff.Sum();
	if (SumAbsDiff <= Eps*9.0)// 9 element in matrix
	{
		Flag_Input_Output_SameOrientation = true;
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
DenseVector<ScalarType, 3> DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
Transform3DPositionInInputImageTo3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position_in)
{
	auto& Self = *this;
	if (Self.Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_in;
	}
	else if (Self.Flag_Input_Output_Orientation_IdentityMatrix == true)
	{
		auto Position_out = Self.InputImage->GetOrigin() - Self.OutputImage.GetOrigin() + Position_in;
		return Position_out;
	}
	else
	{
		auto R = Self.Position3DTransformFromInputToOutput_Matrix.GetElementPointer();
		auto T = Self.Position3DTransformFromInputToOutput_Offset.GetElementPointer();
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
	auto& Self = *this;
	if (Self.Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_out;
	}
	else if (Self.Flag_Input_Output_Orientation_IdentityMatrix == true)
	{
		auto Position_in = Self.OutputImage.GetOrigin() - Self.InputImage->GetOrigin() + Position_out;
		return Position_in;
	}
	else
	{
		auto R = Self.Position3DTransformFromOuputToInput_Matrix.GetElementPointer();
		auto T = Self.Position3DTransformFromOuputToInput_Offset.GetElementPointer();
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
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ DenseImageResampler3D::SelfCheckInput()")
		return false;
	}

	if (Self.InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ DenseImageResampler3D::CheckInput()")
		return false;
	}

	if (Self.OutputImage.IsEmpty() == true)
	{
		MDK_Error("OutputImage is Empty, call SetOutputImageInfo first  @ DenseImageResampler3D::CheckInput()")
		return false;
	}

	if (Self.MaxThreadCount <= 0)
	{
		MDK_Error("Self.MaxThreadCount <= 0 @ DenseImageResampler3D::CheckInput()")
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

	auto& Self = *this;

	//--------------------------------------------------------------------------------
	// this is slow
	/*
	auto PixelCount = Self.OutputImage.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		auto Pos_out = Self.OutputImage.TransformLinearIndexTo3DPosition<ScalarType>(k);
		DenseVector<ScalarType, 3> Pos_in;
		if (Self.Position3DTransform_from_OutputImage_to_InputImage != nullptr)
		{
			Pos_in = Self.Position3DTransform_from_OutputImage_to_InputImage->TransformPoint(Pos_out);
		}
		else
		{
			Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
		}		
		if (Self.Flag_SmoothInputImage == false)
		{
			Self.OutputImage[k] = Self.InputImage->GetPixelAt3DPosition<OutputPixelType>(Pos_in, Self.ImageInterpolationOption);
		}
		else
		{
			Self.OutputImage[k] = Self.SmoothedImage.GetPixelAt3DPosition<OutputPixelType>(Pos_in, Self.ImageInterpolationOption);
		}
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Self.MaxThreadCount);
	*/
	//---------------------------------------------------------------------------------
	auto Size = Self.OutputImage.GetSize();
	//for (int_max z = 0; z <= Size[2]-1; ++z)
	auto TempFunction = [&](int_max z)
	{
		for (int_max y = 0; y < Size[1]; ++y)
		{
			for (int_max x = 0; x < Size[0]; ++x)
			{
				auto Pos_out = Self.OutputImage.Transform3DIndexTo3DPosition<ScalarType>(x,y,z);
				DenseVector<ScalarType, 3> Pos_in;
				if (Self.Position3DTransform_from_OutputImage_to_InputImage != nullptr)
				{
					Pos_in = Self.Position3DTransform_from_OutputImage_to_InputImage->TransformPoint(Pos_out);
				}
				else
				{
					Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
				}
				if (Self.Flag_SmoothInputImage == false)
				{
					Self.OutputImage(x,y,z) = Self.InputImage->GetPixelAt3DPosition<OutputPixelType>(Pos_in, Self.ImageInterpolationOption);
				}
				else
				{
					Self.OutputImage(x,y,z) = Self.SmoothedImage.GetPixelAt3DPosition<OutputPixelType>(Pos_in, Self.ImageInterpolationOption);
				}
			}
		}
	};
	ParallelForLoop(TempFunction, 0, Size[2]-1, Self.MaxThreadCount);
	//---------------------------------------------------------------------------------
	Self.SmoothedImage.Clear();
}

}

