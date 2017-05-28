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
	Input.Image = nullptr;
	Input.ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Linear;
	Input.ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Constant;
	Input.ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);	
	Input.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Nothing;
	Input.Position3DTransform_from_OutputImage_to_InputImage = nullptr;
	Input.MaxThreadCount = 1;

	Internal.Flag_SmoothInputImage = false;
	Internal.SmoothedImage.Clear();
	Internal.Flag_Input_Output_Orientation_IdentityMatrix = false;
	Internal.Flag_Input_Output_SameOrigin_SameOrientation = false;
	Internal.Position3DTransformFromOuputToInput_Matrix.Clear();
	Internal.Position3DTransformFromOuputToInput_Offset.Clear();
	Internal.Position3DTransformFromInputToOutput_Matrix.Clear();
	Internal.Position3DTransformFromInputToOutput_Offset.Clear();
	
	Output.Image.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SmoothInputImageIfNecessary()
{
	Internal.SmoothedImage.Clear();
	Internal.Flag_SmoothInputImage = false;
	if (Input.SmoothingMethodWhenDownsmapling != SmoothingMethodTypeWhenDownsmapling::Nothing)
	{
		auto InputSpacing = Input.Image->GetSpacing();
		auto OutputSpacing = Output.Image.GetSpacing();
		for (int_max k = 0; k < 3; ++k)
		{
			auto Ratio = OutputSpacing[k] / InputSpacing[k];
			if (Ratio > 1.2)
			{
				Internal.Flag_SmoothInputImage = true;
			}
		}

		if (Internal.Flag_SmoothInputImage == true)
		{
			if (Input.SmoothingMethodWhenDownsmapling == SmoothingMethodTypeWhenDownsmapling::Average)
			{
				IntegralImageBasedAverageDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType> SmoothingFilter;
				SmoothingFilter.Input.Image = Input.Image;
				SmoothingFilter.SetOutputImageInfo(Input.Image->GetInfo());
				SmoothingFilter.Input.Radius = OutputSpacing;
				SmoothingFilter.Input.MaxThreadCount = Input.MaxThreadCount;
				SmoothingFilter.Update();
				Internal.SmoothedImage = std::move(SmoothingFilter.Output.Image);
			}
			else if (Input.SmoothingMethodWhenDownsmapling == SmoothingMethodTypeWhenDownsmapling::Triangle)
			{
				DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType> SmoothingFilter;
				SmoothingFilter.Input.Image = Input.Image;
				SmoothingFilter.CreateTriangleMask(Input.Image->GetSpacing(), OutputSpacing[0], OutputSpacing[1], OutputSpacing[2]);
				SmoothingFilter.Input.MaxThreadCount = Input.MaxThreadCount;
				SmoothingFilter.Update();
				Internal.SmoothedImage = std::move(SmoothingFilter.Output.Image);
			}
		}
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo3D& Info)
{	
	auto Size_old = Output.Image.GetSize();
	if (Size_old[0] != Info.Size[0] || Size_old[1] != Info.Size[1] || Size_old[2] != Info.Size[2])
	{
		Output.Image.Clear();
		Output.Image.SetInfo(Info);
	}
	else
	{// no new memory allocation
		Output.Image.SetInfo(Info, false);
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
	if (Input.Image == nullptr)
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

	auto Size_input = Input.Image->GetSize();
	auto Spacing_input = Input.Image->GetSpacing();
	Info.Spacing[0] = double(Size_input[0] - 1) * Spacing_input[0] / double(Lx - 1);
	Info.Spacing[1] = double(Size_input[1] - 1) * Spacing_input[1] / double(Ly - 1);
	Info.Spacing[2] = double(Size_input[2] - 1) * Spacing_input[2] / double(Lz - 1);

	Info.Origin = Input.Image->GetOrigin();
	Info.Orientation = Input.Image->GetOrientation();
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
	if (Input.Image == nullptr)
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

	auto Size_input = Input.Image->GetSize();
	auto Spacing_input = Input.Image->GetSpacing();
	Info.Size[0] = int_max(std::ceil(Size_input[0] * Spacing_input[0] / Spacing_x));
	Info.Size[1] = int_max(std::ceil(Size_input[1] * Spacing_input[1] / Spacing_y));
	Info.Size[2] = int_max(std::ceil(Size_input[2] * Spacing_input[2] / Spacing_z));

	Info.Origin = Input.Image->GetOrigin();
	Info.Orientation = Input.Image->GetOrientation();
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
{	
	auto InputImageInfo = Input.Image->GetInfo();
	auto OutputImageInfo = Output.Image.GetInfo();
	{
		auto M = InputImageInfo.Orientation.Inv();
		Internal.Position3DTransformFromOuputToInput_Matrix = MatrixMultiply(M, OutputImageInfo.Orientation);
		auto D = OutputImageInfo.Origin - InputImageInfo.Origin;
		// Internal.Position3DTransformFromOuputToInput_Offset = M*D
		Internal.Position3DTransformFromOuputToInput_Offset[0] = M[0] * D[0] + M[3] * D[1] + M[6] * D[2];
		Internal.Position3DTransformFromOuputToInput_Offset[1] = M[1] * D[0] + M[4] * D[1] + M[7] * D[2];
		Internal.Position3DTransformFromOuputToInput_Offset[2] = M[2] * D[0] + M[5] * D[1] + M[8] * D[2];
	}

	{
		auto M = OutputImageInfo.Orientation.Inv();
		Internal.Position3DTransformFromInputToOutput_Matrix = MatrixMultiply(M, InputImageInfo.Orientation);
		auto D = InputImageInfo.Origin - OutputImageInfo.Origin;
		// Internal.Position3DTransformFromInputToOutput_Offset = M*D
		Internal.Position3DTransformFromInputToOutput_Offset[0] = M[0] * D[0] + M[3] * D[1] + M[6] * D[2];
		Internal.Position3DTransformFromInputToOutput_Offset[1] = M[1] * D[0] + M[4] * D[1] + M[7] * D[2];
		Internal.Position3DTransformFromInputToOutput_Offset[2] = M[2] * D[0] + M[5] * D[1] + M[8] * D[2];
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
	for (auto& i : OrientationDiff) { i = std::abs(i); }
	auto SumAbsDiff = OrientationDiff.Sum();
	if (SumAbsDiff <= Eps*9.0)// 9 element in matrix
	{
		Flag_Input_Output_SameOrientation = true;
	}

	if (Flag_Input_Output_SameOrigin == true && Flag_Input_Output_SameOrientation == true)
	{
		Internal.Flag_Input_Output_SameOrigin_SameOrientation = true;
	}

	Internal.Flag_Input_Output_Orientation_IdentityMatrix = false;
	if (InputImageInfo.Orientation.IsIdentityMatrix() == true && OutputImageInfo.Orientation.IsIdentityMatrix() == true)
	{
		Internal.Flag_Input_Output_Orientation_IdentityMatrix = true;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
Transform3DPositionInInputImageTo3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position_in)
{	
	if (Internal.Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_in;
	}
	else if (Internal.Flag_Input_Output_Orientation_IdentityMatrix == true)
	{
		auto Position_out = Input.Image->GetOrigin() - Output.Image.GetOrigin() + Position_in;
		return Position_out;
	}
	else
	{
		auto R = Internal.Position3DTransformFromInputToOutput_Matrix.GetElementPointer();
		auto T = Internal.Position3DTransformFromInputToOutput_Offset.GetElementPointer();
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
	if (Internal.Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_out;
	}
	else if (Internal.Flag_Input_Output_Orientation_IdentityMatrix == true)
	{
		auto Position_in = Output.Image.GetOrigin() - Input.Image->GetOrigin() + Position_out;
		return Position_in;
	}
	else
	{
		auto R = Internal.Position3DTransformFromOuputToInput_Matrix.GetElementPointer();
		auto T = Internal.Position3DTransformFromOuputToInput_Offset.GetElementPointer();
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
	if (Input.Image == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ DenseImageResampler3D::SelfCheckInput()")
		return false;
	}

	if (Input.Image->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ DenseImageResampler3D::CheckInput()")
		return false;
	}

	if (Output.Image.IsEmpty() == true)
	{
		MDK_Error("OutputImage is Empty, call SetOutputImageInfo first  @ DenseImageResampler3D::CheckInput()")
		return false;
	}

	if (Input.MaxThreadCount <= 0)
	{
		MDK_Error("Input.MaxThreadCount <= 0 @ DenseImageResampler3D::CheckInput()")
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
	auto PixelCount = Output.Image.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		auto Pos_out = Output.Image.TransformLinearIndexTo3DPosition<ScalarType>(k);
		DenseVector<ScalarType, 3> Pos_in;
		if (Input.Position3DTransform_from_OutputImage_to_InputImage != nullptr)
		{
			Pos_in = Input.Position3DTransform_from_OutputImage_to_InputImage->TransformPoint(Pos_out);
		}
		else
		{
			Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
		}		
		if (Internal.Flag_SmoothInputImage == false)
		{
			Output.Image[k] = Input.Image->GetPixelAt3DPosition<OutputPixelType>(Pos_in, Input.ImageInterpolationOption);
		}
		else
		{
			Output.Image[k] = Internal.SmoothedImage.GetPixelAt3DPosition<OutputPixelType>(Pos_in, Input.ImageInterpolationOption);
		}
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Input.MaxThreadCount);
	*/
	//---------------------------------------------------------------------------------
	auto Size = Output.Image.GetSize();
	//for (int_max z = 0; z <= Size[2]-1; ++z)
	auto TempFunction = [&](int_max z)
	{
		for (int_max y = 0; y < Size[1]; ++y)
		{
			for (int_max x = 0; x < Size[0]; ++x)
			{
				auto Pos_out = Output.Image.Transform3DIndexTo3DPosition<ScalarType>(x,y,z);
				DenseVector<ScalarType, 3> Pos_in;
				if (Input.Position3DTransform_from_OutputImage_to_InputImage != nullptr)
				{
					Pos_in = Input.Position3DTransform_from_OutputImage_to_InputImage->TransformPoint(Pos_out);
				}
				else
				{
					Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
				}
				if (Internal.Flag_SmoothInputImage == false)
				{
					Output.Image(x,y,z) = Input.Image->GetPixelAt3DPosition<OutputPixelType>(Pos_in, Input.ImageInterpolationOption);
				}
				else
				{
					Output.Image(x,y,z) = Internal.SmoothedImage.GetPixelAt3DPosition<OutputPixelType>(Pos_in, Input.ImageInterpolationOption);
				}
			}
		}
	};
	ParallelForLoop(TempFunction, 0, Size[2]-1, Input.MaxThreadCount);
	//---------------------------------------------------------------------------------
	Internal.SmoothedImage.Clear();
}

}//namespace mdk

