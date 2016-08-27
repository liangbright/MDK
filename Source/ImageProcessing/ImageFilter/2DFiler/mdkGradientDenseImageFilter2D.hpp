#pragma once

namespace mdk
{

template<typename InputPixelType, typename ScalarType>
GradientDenseImageFilter2D<InputPixelType, ScalarType>::GradientDenseImageFilter2D()
{
    this->Clear();
}


template<typename InputPixelType, typename ScalarType>
GradientDenseImageFilter2D<InputPixelType, ScalarType>::~GradientDenseImageFilter2D()
{
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::Clear()
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

	Self.Radius = 0;
	Self.Flag_MaskOriginLocation = 0;
	Self.AngleResolution = 1;
	Self.MaskList.Clear();
	Self.MaskCountAtEachLevel.Clear();
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo2D& Info)
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 2>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1]);
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly)
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ GradientDenseImageFilter2D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 1 || Ly <= 1)
	{
		MDK_Error("Invalid input @ GradientDenseImageFilter2D::SetOutputImageInfoBySize(...)")
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 2>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1]);
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y)
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ GradientDenseImageFilter2D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_y <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ GradientDenseImageFilter2D::SetOutputImageInfoBySpacing(...)")
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
{
	auto& Self = *this;
	auto InputImageInfo = Self.InputImage->GetInfo();
	auto OutputImageInfo = Self.OutputImage.GetInfo();
	{
		auto M = InputImageInfo.Orientation.Inv();
		Self.Position3DTransformFromOuputToInput_Matrix = MatrixMultiply(M, OutputImageInfo.Orientation);
		auto D = OutputImageInfo.Origin - InputImageInfo.Origin;
		// Self.2DPositionTransformFromOuputToInput_Offset = M*D
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


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 2> GradientDenseImageFilter2D<InputPixelType, ScalarType>::
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


template<typename InputPixelType, typename ScalarType>
bool GradientDenseImageFilter2D<InputPixelType, ScalarType>::CheckInput()
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ GradientDenseImageFilter2D::SelfCheckInput()")
		return false;
	}

	if (Self.InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ GradientDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.OutputImage.IsEmpty() == true)
	{
		MDK_Error("OutputImage is Empty, call SetOutputImageInfo first  @ GradientDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.MaxThreadCount <= 0)
	{
		MDK_Error("MaxThreadCount <= 0 @ GradientDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.Radius <= 0.0)
	{
		MDK_Error("Radius <= 0.0 @ GradientDenseImageFilter2D::CheckInput(...)")
		return false;
	}

	if (Self.AngleResolution <= 0 || Self.AngleResolution >= 6.2832)
	{
		MDK_Error("AngleResolution is invalid @ GradientDenseImageFilter2D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::BuildMaskWithGradientPrior(const DenseVector<ScalarType, 2>& GradientPrior)
{
	this->InitializeMaskList();

	auto& Self = *this;
	// Select a subset from MaskList[0] Level 0 based on GradientPrior
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(Self.MaskList[0].GetElementCount());
	for (int_max n = 0; n < Self.MaskList[0].GetElementCount(); ++n)
	{
		auto Direction = Self.MaskList[0][n].PointP - Self.MaskList[0][n].PointN;
		DotProductList[n] = Direction[0] * GradientPrior[0] + Direction[1] * GradientPrior[1];
	}
	auto IndexList_sort = DotProductList.Sort("descend");
	int_max MaskCount_keep = int_max(double(Self.MaskCountAtEachLevel[0]) * 0.1) + 1;// keep 10% of MaskList[0]
	auto MaskIndexList_keep = IndexList_sort.GetSubSet(0, MaskCount_keep - 1);
	Self.MaskList[0] = Self.MaskList[0].GetSubSet(MaskIndexList_keep);

	// Link
	for (int_max k = 0; k < Self.MaskList.GetElementCount() - 1; ++k)
	{
		for (int_max n = 0; n < Self.MaskList[k].GetElementCount(); ++n)
		{
			this->BuildMaskLink(k, n);
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::BuildMask()
{
	this->InitializeMaskList();
	auto& Self = *this;
	for (int_max k = 0; k < Self.MaskList.GetElementCount()-1; ++k)
	{
		for (int_max n = 0; n < Self.MaskList[k].GetElementCount(); ++n)
		{
			this->BuildMaskLink(k, n);
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::InitializeMaskList()
{
	auto& Self = *this;
	//------------------------------------------------------
	const auto pi = ScalarType(std::acos(-1.0));
	auto MaskCount_max = int_max(2*pi/Self.AngleResolution);
	int_max Level_max = std::log2(MaskCount_max+1)/2 - 1;
	if (Level_max < 0) { Level_max = 0; }
    //------------------------------------------------------
	Self.MaskCountAtEachLevel.Clear();
	Self.MaskCountAtEachLevel.Resize(Level_max+1);
	Self.MaskCountAtEachLevel[0] = 8; //Level 0
	if (Level_max >= 1)
	{
		Self.MaskCountAtEachLevel[1] = 32;// Level 1			
	}
	// from Level 2
	for (int_max k = 2; k <= Level_max; ++k)
	{
		Self.MaskCountAtEachLevel[k] = 4 * Self.MaskCountAtEachLevel[k - 1];
	}
	//------------------------------------------------------
	Self.MaskList.Clear();
	Self.MaskList.Resize(Self.MaskCountAtEachLevel.GetElementCount());
	for (int_max k = 0; k < Self.MaskList.GetElementCount(); ++k)
	{
		Self.MaskList[k].Resize(Self.MaskCountAtEachLevel[k]);
		this->InitializeMaskAtLevel(k);
	}
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::InitializeMaskAtLevel(int_max Level)
{
	auto& Self = *this;
	auto MaskCount = Self.MaskCountAtEachLevel[Level];

	const double pi = std::acos(-1.0);
	auto Theta = 2*pi/double(MaskCount);

	if (Self.Flag_MaskOriginLocation == 0)
	{
		double HalfRadius = Self.Radius / 2.0;

		for (int_max k = 0; k < MaskCount; ++k)
		{
			auto Angle = Theta*double(k);
			auto y = HalfRadius * std::sin(Angle);
			auto x = HalfRadius * std::cos(Angle);

			Self.MaskList[Level][k].PointP = {x, y};
			Self.MaskList[Level][k].PointN = { -x, -y };
		}
	}
	else if (Self.Flag_MaskOriginLocation == 1)
	{
		for (int_max k = 0; k < MaskCount; ++k)
		{
			auto Angle = Theta*double(k);
			auto y = Self.Radius * std::sin(Angle);
			auto x = Self.Radius * std::cos(Angle);

			Self.MaskList[Level][k].PointP = { 0, 0 };
			Self.MaskList[Level][k].PointN = { -x, -y };
		}
	}
	else // Self.Flag_MaskOriginLocation == -1
	{
		for (int_max k = 0; k < MaskCount; ++k)
		{
			auto Angle = Theta*double(k);
			auto y = Self.Radius * std::sin(Angle);
			auto x = Self.Radius * std::cos(Angle);

			Self.MaskList[Level][k].PointP = { x, y };
			Self.MaskList[Level][k].PointN = { 0, 0 };
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::BuildMaskLink(int_max Level, int_max MaskIndex)
{
	auto& Self = *this;

	auto Level_next = Level+1;

	auto VectorA = Self.MaskList[Level][MaskIndex].PointP - Self.MaskList[Level][MaskIndex].PointN;
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(Self.MaskList[Level_next].GetElementCount());
	for (int_max n = 0; n < Self.MaskList[Level_next].GetElementCount(); ++n)
	{
		auto VectorB = Self.MaskList[Level_next][n].PointP - Self.MaskList[Level_next][n].PointN;
		DotProductList[n] = VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1];
	}

	auto IndexList_sort = DotProductList.Sort("descend");
	auto MaskCount_keep = int_max(double(Self.MaskCountAtEachLevel[Level_next]) / double(Self.MaskCountAtEachLevel[Level])) + 1;
	Self.MaskList[Level][MaskIndex].MaskIndexListAtNextLevel = IndexList_sort.GetSubSet(0, MaskCount_keep - 1);
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::Transform2DPositonInMask_from_Output_to_Input()
{
	auto& Self = *this;
	for (int_max Level = 0; Level < Self.MaskList.GetLength(); ++Level)
	{
		for (int_max MaskIndex = 0; MaskIndex < Self.MaskList[Level].GetLength(); ++MaskIndex)
		{
			auto PointP = Self.MaskList[Level][MaskIndex].PointP;
			auto PointP_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(PointP);
			Self.MaskList[Level][MaskIndex].PointP = PointP_in;

			auto PointN = Self.MaskList[Level][MaskIndex].PointN;
			auto PointN_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(PointN);
			Self.MaskList[Level][MaskIndex].PointN = PointN;
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->Update3DPositionTransform_Input_Output();

	auto& Self = *this;
	if (Self.GradientDirection_Prior.L2Norm() < 0.001)
	{
		this->BuildMask();
	}
	else
	{
		Self.GradientDirection_Prior /= Self.GradientDirection_Prior.L2Norm();
		this->BuildMaskWithGradientPrior(Self.GradientDirection_Prior);
	}

	this->Transform2DPositonInMask_from_Output_to_Input();

	//--------------------------------------------------------------------------------
	auto PixelCount = Self.OutputImage.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		Self.OutputImage[k] = this->EvaluateAtPixelInOutputImage(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Self.MaxThreadCount);
	//---------------------------------------------------------------------------------
}


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 2> GradientDenseImageFilter2D<InputPixelType, ScalarType>::
EvaluateAtPixelInOutputImage(int_max LinearIndex)
{
	auto& Self = *this;
	auto Pos_out = Self.OutputImage.TransformLinearIndexTo2DPosition<ScalarType>(LinearIndex);
	auto Pos_in = this->Transform2DPositionInOutputImageTo2DPositionInInputImage(Pos_out);
	auto x0 = Pos_in[0];
	auto y0 = Pos_in[1];

	DenseVector<ScalarType, 2> Gradient_max;
	int_max MaskIndex_max = -1;

	// Level 0
	DenseVector<int_max> MaskIndexList = span(0, Self.MaskList[0].GetLength()-1);
	this->EvaluateAt2DPositionInInputImage_SingleLevel(MaskIndex_max, Gradient_max, x0, y0, 0, MaskIndexList);
	
	//from Level 1
	for (int_max k = 1; k < Self.MaskList.GetLength(); ++k)
	{
		MaskIndexList = Self.MaskList[k - 1][MaskIndex_max].MaskIndexListAtNextLevel;
		this->EvaluateAt2DPositionInInputImage_SingleLevel(MaskIndex_max, Gradient_max, x0, y0, k, MaskIndexList);
	}

	return Gradient_max;
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter2D<InputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, int_max Level, const DenseVector<int_max>& MaskIndexList)
{	
	auto& Self = *this;

	ScalarType Magnitude = 0;
	for (int_max k = 0; k < MaskIndexList.GetLength(); ++k)
	{
		auto MaskIndex = MaskIndexList[k];
		
		auto xp = Self.MaskList[Level][MaskIndex].PointP[0] + x0;
		auto yp = Self.MaskList[Level][MaskIndex].PointP[1] + y0;

		auto xn = Self.MaskList[Level][MaskIndex].PointN[0] + x0;
		auto yn = Self.MaskList[Level][MaskIndex].PointN[1] + y0;

		auto PixelP = Self.InputImage->GetPixelAt2DPosition<ScalarType>(xp, yp, Self.ImageInterpolationOption);
		auto PixelN = Self.InputImage->GetPixelAt2DPosition<ScalarType>(xn, yn, Self.ImageInterpolationOption);
		auto Difference = PixelP - PixelN;

		if (k == 0 || Magnitude < Difference)
		{
			Magnitude = Difference;
			MaskIndex_max = MaskIndex;
			Gradient_max[0] = xp - xn;
			Gradient_max[1] = yp - yn;
		}
	}

	auto EPS = std::numeric_limits<ScalarType>::epsilon();
	if (std::abs(Magnitude) > 2 * EPS)
	{
		Gradient_max /= Gradient_max.L2Norm();
		Gradient_max *= Magnitude;
	}
	else
	{
		Gradient_max.Fill(0);
	}
}

}//end namespace mdk

