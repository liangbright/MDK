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
	Input.Image = nullptr;
	Input.ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Linear;
	Input.ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Constant;
	Input.ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);
	Input.IntegralImage = &Internal.IntegralImage;
	Input.Radius = { 0, 0 };
	Input.MaxThreadCount = 1;

	Internal.Flag_Input_Output_Orientation_IdentityMatrix = false;
	Internal.Flag_Input_Output_SameOrigin_SameOrientation = false;
	Internal.Position3DTransformFromOuputToInput_Matrix.Clear();
	Internal.Position3DTransformFromOuputToInput_Offset.Clear();
	Internal.IntegralImage.Clear();
	Internal.Radius_Index2D = { 0, 0};

	Output.Image.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo2D& Info)
{	
	auto Size_old = Output.Image.GetSize();
	if (Size_old[0] != Info.Size[0] || Size_old[1] != Info.Size[1])
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
	if (Input.Image == nullptr)
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

	auto Size_input = Input.Image->GetSize();
	auto Spacing_input = Input.Image->GetSpacing();
	Info.Spacing[0] = double(Size_input[0] - 1) * Spacing_input[0] / double(Lx - 1);
	Info.Spacing[1] = double(Size_input[1] - 1) * Spacing_input[1] / double(Ly - 1);

	Info.Origin = Input.Image->GetOrigin();
	Info.Orientation = Input.Image->GetOrientation();
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
	if (Input.Image == nullptr)
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

	auto Size_input = Input.Image->GetSize();
	auto Spacing_input = Input.Image->GetSpacing();
	Info.Size[0] = int_max(std::ceil(Size_input[0] * Spacing_input[0] / Spacing_x));
	Info.Size[1] = int_max(std::ceil(Size_input[1] * Spacing_input[1] / Spacing_y));

	Info.Origin = Input.Image->GetOrigin();
	Info.Orientation = Input.Image->GetOrientation();
	Info.UpdateTransformMatrix();
	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
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
	for (auto& i : OrientationDiff) { i = std::abs(i); }
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
		Internal.Flag_Input_Output_SameOrigin_SameOrientation = true;
	}

	Internal.Flag_Input_Output_Orientation_IdentityMatrix = false;
	if (InputImageInfo.Orientation.IsIdentityMatrix() == true && OutputImageInfo.Orientation.IsIdentityMatrix() == true)
	{
		Internal.Flag_Input_Output_Orientation_IdentityMatrix = true;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseVector<ScalarType, 2> IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out)
{	
	if (Internal.Flag_Input_Output_SameOrigin_SameOrientation == true)
	{
		return Position_out;
	}
	else if (Internal.Flag_Input_Output_Orientation_IdentityMatrix == true)
	{
		auto Position_in = Output.Image.GetOrigin2D() - Input.Image->GetOrigin2D() + Position_out;
		return Position_in;
	}
	else
	{
		auto R = Internal.Position3DTransformFromOuputToInput_Matrix.GetElementPointer();
		auto T = Internal.Position3DTransformFromOuputToInput_Offset.GetElementPointer();
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
	auto Size = Input.Image->GetSize();
	if (Size[0] < 2 || Size[1] < 2)
	{
		MDK_Error(" SizeX or SizeY of InputImage < 2 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
		return false;
	}

	if (Input.IntegralImage == nullptr)
	{
		Input.IntegralImage = &Internal.IntegralImage;
	}
	else
	{
		if (Input.IntegralImage != &Internal.IntegralImage)
		{
			if (Input.IntegralImage->IsEmpty() == true)
			{
				MDK_Error("Input IntegralImage is empty @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary(...)")
				return false;
			}

			auto Size = Input.IntegralImage->GetSize();
			if (Size[0] < 2 || Size[1] < 2)
			{
				MDK_Error(" SizeX or SizeY of Input IntegralImage < 2 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
				return false;
			}
		}
	}

	auto Spacing = Output.Image.GetSpacing();
	Internal.Radius_Index2D[0] = Input.Radius[0] / Spacing[0];
	Internal.Radius_Index2D[1] = Input.Radius[1] / Spacing[1];

	if (Internal.Radius_Index2D[0] <= 0 || Internal.Radius_Index2D[1] <= 0)
	{
		MDK_Error(" RadiusX or RadiusY in Index2D <= 0 @ IntegralImageBasedAverageDenseImageFilter2D::BuildIntegralImageIfNecessary()")
		return false;
	}

	Internal.IntegralImage.Clear();
	if (Input.IntegralImage == &Internal.IntegralImage)
	{
		IntegralImageBuilder2D<double> ImBuilder;
		ImBuilder.Input.Image = Input.Image;
		ImBuilder.Update();
		Internal.IntegralImage = std::move(ImBuilder.Output.Image);
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{	
	if (Input.Image == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ IntegralImageBasedAverageDenseImageFilter2D::SelfCheckInput()")
		return false;
	}

	if (Input.Image->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ IntegralImageBasedAverageDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Input.MaxThreadCount <= 0)
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
	
	auto PixelCount = Output.Image.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		Output.Image[k] = this->EvaluateAtPixelInOutputImage(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Input.MaxThreadCount);
	//---------------------------------------------------------------------------------
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType IntegralImageBasedAverageDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAtPixelInOutputImage(int_max LinearIndex)
{	
	auto Pos_out = Output.Image.TransformLinearIndexTo2DPosition<ScalarType>(LinearIndex);
	auto Pos_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(Pos_out);
	auto x0 = Pos_in[0];
	auto y0 = Pos_in[1];

	// Index2D=[x, y] : ScalarType
	auto Index2D = Input.IntegralImage->Transform2DPositionTo2DIndex(x0, y0);
	auto Size = Input.IntegralImage->GetSize();
	//---------------------------------------------------------
	// x1 <= x0 <= x2, y1 <= y0 <= y2
	//
	// y 
	// ^            X2
	// |       X0
	// |   X1 
	// |------------->x

	//auto x1 = int_max(std::round(Index2D[0] - Internal.Radius_Index2D[0]-1));
	//auto x2 = int_max(std::round(Index2D[0] + Internal.Radius_Index2D[0]));
	//auto y1 = int_max(std::round(Index2D[1] - Internal.Radius_Index2D[1]-1));
	//auto y2 = int_max(std::round(Index2D[1] + Internal.Radius_Index2D[1]));
	auto x1 = int_max(Index2D[0] - Internal.Radius_Index2D[0] - 1);
	auto x2 = int_max(Index2D[0] + Internal.Radius_Index2D[0] + 0.5);
	auto y1 = int_max(Index2D[1] - Internal.Radius_Index2D[1] - 1);
	auto y2 = int_max(Index2D[1] + Internal.Radius_Index2D[1] + 0.5);

	// if the window is outside the image
	if (x1 >= Size[0] || x2 < 0 || y1 >= Size[1] || y2 < 0)
	{
		return OutputPixelType(Input.ImageInterpolationOption.Pixel_OutsideImage);
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
	auto Pixel_x1y1 = (*Input.IntegralImage)(x1, y1);
	auto Pixel_x1y2 = (*Input.IntegralImage)(x1, y2);
	auto Pixel_x2y1 = (*Input.IntegralImage)(x2, y1);
	auto Pixel_x2y2 = (*Input.IntegralImage)(x2, y2);
	//---------------------------------------------------------
	OutputPixelType OutputPixel;
	OutputPixel = Pixel_x2y2 - Pixel_x1y2 - Pixel_x2y1 + Pixel_x1y1;
	OutputPixel /= ScalarType((y2 - y1)*(x2 - x1));
	return OutputPixel;	
}


}//end namespace mdk

