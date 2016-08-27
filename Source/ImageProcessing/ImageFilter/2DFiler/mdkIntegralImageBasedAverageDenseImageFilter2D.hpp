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

	Self.IntegralImage_Internal.Clear();
	Self.IntegralImage = &Self.IntegralImage_Internal;
	Self.Radius = { 0, 0};
	Self.Radius_Index2D = { 0, 0};
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo2D& Info)
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
	auto& Self = *this;
	if (Self.InputImage == nullptr)
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
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 2>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y)
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
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
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
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
DenseVector<ScalarType, 2> IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
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
		Position_in[0] = R[0] * Position_out[0] + R[3] * Position_out[1] + R[6] * Position_out[2] + T[0];
		Position_in[1] = R[1] * Position_out[0] + R[4] * Position_out[1] + R[7] * Position_out[2] + T[1];
		//Position_in[2] = R[2] * Position_out[0] + R[5] * Position_out[1] + R[8] * Position_out[2] + T[2];
		return Position_in;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::BuildIntegralImageIfNecessary()
{
	auto& Self = *this;
	auto Size = Self.InputImage->GetSize();
	if (Size[0] < 2 || Size[1] < 2)
	{
		MDK_Error(" SizeX or SizeY of InputImage < 2 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
		return false;
	}

	if (Self.IntegralImage == nullptr)
	{
		Self.IntegralImage = &Self.IntegralImage_Internal;
	}
	else
	{
		if (Self.IntegralImage != &Self.IntegralImage_Internal)
		{
			if (Self.IntegralImage->IsEmpty() == true)
			{
				MDK_Error("Input IntegralImage is empty @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary(...)")
				return false;
			}

			auto Size = Self.IntegralImage->GetSize();
			if (Size[0] < 2 || Size[1] < 2)
			{
				MDK_Error(" SizeX or SizeY of Input IntegralImage < 2 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
				return false;
			}
		}
	}

	auto Spacing = Self.OutputImage.GetSpacing();
	Self.Radius_Index2D[0] = Self.Radius[0] / Spacing[0];
	Self.Radius_Index2D[1] = Self.Radius[1] / Spacing[1];

	if (Self.Radius_Index2D[0] <= 0 || Self.Radius_Index2D[1] <= 0)
	{
		MDK_Error(" RadiusX or RadiusY in Index2D <= 0 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
		return false;
	}

	Self.IntegralImage_Internal.Clear();
	if (Self.IntegralImage == &Self.IntegralImage_Internal)
	{
		IntegralImageBuilder2D<double> ImBuilder;
		ImBuilder.SetInputImage(Self.InputImage);
		ImBuilder.Update();
		Self.IntegralImage_Internal = std::move(ImBuilder.OutputImage());
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ IntegralImageBasedAverageDenseImageFilter2D::SelfCheckInput()")
		return false;
	}

	if (Self.InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ IntegralImageBasedAverageDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.MaxThreadCount <= 0)
	{
	    MDK_Error("MaxThreadCount <= 0 @ IntegralImageBasedAverageDenseImageFilter2D::CheckInput()")
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
	auto& Self = *this;
	auto PixelCount = Self.OutputImage.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		Self.OutputImage[k] = this->EvaluateAtPixelInOutputImage(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Self.MaxThreadCount);
	//---------------------------------------------------------------------------------
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAtPixelInOutputImage(int_max LinearIndex)
{
	auto& Self = *this;
	auto Pos_out = Self.OutputImage.TransformLinearIndexTo2DPosition<ScalarType>(LinearIndex);
	auto Pos_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(Pos_out);
	auto x0 = Pos_in[0];
	auto y0 = Pos_in[1];

	// Index2D=[x, y] : ScalarType
	auto Index2D = Self.IntegralImage->Transform2DPositionTo2DIndex(x0, y0);
	auto Size = Self.IntegralImage->GetSize();
	//---------------------------------------------------------
	// x1 <= x0 <= x2, y1 <= y0 <= y2
	//
	// y 
	// ^            X2
	// |       X0
	// |   X1 
	// |------------->x

	//auto x1 = int_max(std::round(Index2D[0] - Self.Radius_Index2D[0]-1));
	//auto x2 = int_max(std::round(Index2D[0] + Self.Radius_Index2D[0]));
	//auto y1 = int_max(std::round(Index2D[1] - Self.Radius_Index2D[1]-1));
	//auto y2 = int_max(std::round(Index2D[1] + Self.Radius_Index2D[1]));
	auto x1 = int_max(Index2D[0] - Self.Radius_Index2D[0] - 1);
	auto x2 = int_max(Index2D[0] + Self.Radius_Index2D[0] + 0.5);
	auto y1 = int_max(Index2D[1] - Self.Radius_Index2D[1] - 1);
	auto y2 = int_max(Index2D[1] + Self.Radius_Index2D[1] + 0.5);

	// if the window is outside the image
	if (x1 >= Size[0] || x2 < 0 || y1 >= Size[1] || y2 < 0)
	{
		return OutputPixelType(Self.ImageInterpolationOption.Pixel_OutsideImage);
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
	auto Pixel_x1y1 = (*Self.IntegralImage)(x1, y1);
	auto Pixel_x1y2 = (*Self.IntegralImage)(x1, y2);
	auto Pixel_x2y1 = (*Self.IntegralImage)(x2, y1);
	auto Pixel_x2y2 = (*Self.IntegralImage)(x2, y2);
	//---------------------------------------------------------
	OutputPixelType OutputPixel;
	OutputPixel = Pixel_x2y2 - Pixel_x1y2 - Pixel_x2y1 + Pixel_x1y1;
	OutputPixel /= ScalarType((y2 - y1)*(x2 - x1));
	return OutputPixel;	
}


}//end namespace mdk

