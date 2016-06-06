#pragma once

namespace mdk
{

template<typename InputPixelType, typename ScalarType>
GradientDenseImageFilter3D<InputPixelType, ScalarType>::GradientDenseImageFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename ScalarType>
GradientDenseImageFilter3D<InputPixelType, ScalarType>::~GradientDenseImageFilter3D()
{
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::Clear()
{
	m_InputImage = nullptr;
	m_ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Nearest;
	m_ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Replicate;
	m_ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);
	m_Flag_Input_Output_SameOrigin = false;
	m_Flag_Input_Output_SameSpacing = false;
	m_Flag_Input_Output_SameOrientation = false;
	m_Flag_Input_Output_SameOrigin_SameOrientation = false;
	m_OutputImage.Clear();
	m_MaxThreadCount = 1;

	m_Radius = 0;
	m_Flag_MaskOriginLocation = 0;
	m_MaskList.Clear();
	m_SphereResolution = 0;	
	m_GradientDirection_Prior.Fill(0);
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo3D& Info)
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1], Size[2]);
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ GradientDenseImageFilter3D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 0 || Ly <= 0 || Lz <= 0)
	{
		MDK_Error("Invalid input @ GradientDenseImageFilter3D::SetOutputImageInfoBySize(...)")
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 3>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1], Spacing[2]);
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr @ GradientDenseImageFilter3D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_x <= Zero || Spacing_x <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ GradientDenseImageFilter3D::SetOutputImageInfoBySpacing(...)")
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
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


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> GradientDenseImageFilter3D<InputPixelType, ScalarType>::
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


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> GradientDenseImageFilter3D<InputPixelType, ScalarType>::
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


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetRadius(double Radius)
{
	m_Radius = Radius;
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetMaskOriginInMiddle()
{
	m_Flag_MaskOriginLocation = 0;
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetMaskOriginAsPositivePole()
{
	m_Flag_MaskOriginLocation = 1;
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetMaskOriginAsNegativePole()
{
	m_Flag_MaskOriginLocation = -1;
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::SetSphereResolution(int_max Resolution)
{
	m_SphereResolution = Resolution;
}


template<typename InputPixelType, typename ScalarType>
bool GradientDenseImageFilter3D<InputPixelType, ScalarType>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ GradientDenseImageFilter3D::SelfCheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ GradientDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (m_MaxThreadCount <= 0)
	{
		MDK_Error("m_MaxThreadCount <= 0 @ GradientDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (m_Radius <= 0.0)
	{
		MDK_Error("m_Radius <= 0.0 @ GradientDenseImageFilter3D::CheckInput(...)")
		return false;
	}

	if (m_SphereResolution <= 0.0)
	{
		MDK_Error("m_SphereResolution <= 0.0 @ GradientDenseImageFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::BuildMask()
{
	this->InitializeMaskList();

	for (int_max k = 0; k < m_MaskList.GetElementCount() - 1; ++k)
	{
		for (int_max n = 0; n < m_MaskList[k].GetElementCount(); ++n)
		{
			this->BuildMaskLink(k, n);
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::BuildMaskWithGradientPrior(const DenseVector<ScalarType, 3>& GradientPrior)
{
	this->InitializeMaskList();

	// Select a subset from MaskList[0] Level 0 based on GradientPrior
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(m_MaskList[0].GetElementCount());
	for (int_max n = 0; n < m_MaskList[0].GetElementCount(); ++n)
	{
		auto Direction = m_MaskList[0][n].PointP - m_MaskList[0][n].PointN;
		DotProductList[n] = Direction[0] * GradientPrior[0] + Direction[1] * GradientPrior[1] + Direction[2] * GradientPrior[2];
	}
	auto IndexList_sort = DotProductList.Sort("descend");

	int_max MaskCount_keep = 10;// keep 10 of MaskList[0]
	auto MaskIndexList_keep = IndexList_sort.GetSubSet(0, MaskCount_keep - 1);
	m_MaskList[0] = m_MaskList[0].GetSubSet(MaskIndexList_keep);

	// Link
	for (int_max k = 0; k < m_MaskList.GetElementCount() - 1; ++k)
	{
		for (int_max n = 0; n < m_MaskList[k].GetElementCount(); ++n)
		{
			this->BuildMaskLink(k, n);
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::InitializeMaskList()
{
	int_max MaxDepth = 0;
	if (m_SphereResolution <= 20)
	{
		MaxDepth = 0;
	}
	else if (m_SphereResolution <= 42)
	{
		MaxDepth = 1;
	}
	else if (m_SphereResolution <= 162)
	{
		MaxDepth = 2;
	}
	else if (m_SphereResolution <= 642)
	{
		MaxDepth = 3;
	}
	else if (m_SphereResolution <= 2562)
	{
		MaxDepth = 4;
	}
	else if (m_SphereResolution <= 10242)
	{
		MaxDepth = 5;
	}
	else if (m_SphereResolution <= 40962)
	{
		MaxDepth = 6;
	}
	else
	{
		MaxDepth = (std::log2(m_SphereResolution) / 2) - 2;
	}

	if (m_SphereResolution > 40962)
	{
		MDK_Warning("m_SphereResolution > 40962 and MaxDepth > 6 @ GradientDenseImageFilter3D::InitializeMaskList()")
	}

	GeodesicSphereBuilder<ScalarType> SphereBuilder;	
	SphereBuilder.SetMaxDepth(MaxDepth);
	SphereBuilder.Update();
	auto& SphereList = SphereBuilder.OutputSphereList();
	//-----------------------------------------------------
	m_MaskList.Clear();
	m_MaskList.Resize(SphereList.GetLength());
	for (int_max Level = 0; Level < SphereList.GetLength(); Level++)
	{
		auto& Sphere = SphereList[Level];

		m_MaskList[Level].Resize(Sphere.GetPointCount());

		if (m_Flag_MaskOriginLocation == 0)
		{
			double HalfRadius = m_Radius / 2.0;
			int_max MaskIndex = -1;
			for (auto it = Sphere.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
			{
				auto Direction = it.Point().GetPosition();
				MaskIndex += 1;				
				m_MaskList[Level][MaskIndex].PointP = Direction*HalfRadius;
				m_MaskList[Level][MaskIndex].PointN = Direction*(-HalfRadius);
			}
		}
		else if (m_Flag_MaskOriginLocation == 1)
		{
			int_max MaskIndex = -1;
			for (auto it = Sphere.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
			{
				auto Direction = it.Point().GetPosition();
				MaskIndex += 1;
				m_MaskList[Level][MaskIndex].PointP = { 0, 0 };
				m_MaskList[Level][MaskIndex].PointN = Direction*(-m_Radius);
			}
		}
		else // m_Flag_MaskOriginLocation == -1
		{
			int_max MaskIndex = -1;
			for (auto it = Sphere.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
			{
				auto Direction = it.Point().GetPosition();
				MaskIndex += 1;
				m_MaskList[Level][MaskIndex].PointP = Direction*m_Radius;
				m_MaskList[Level][MaskIndex].PointN = { 0, 0 };
			}
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::BuildMaskLink(int_max Level, int_max MaskIndex)
{
	int_max Level_next=Level+1;

	auto VectorA = m_MaskList[Level][MaskIndex].PointP - m_MaskList[Level][MaskIndex].PointN;
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(m_MaskList[Level_next].GetElementCount());
	for (int_max n = 0; n < m_MaskList[Level_next].GetElementCount(); ++n)
	{
		auto VectorB = m_MaskList[Level_next][n].PointP - m_MaskList[Level_next][n].PointN;
		DotProductList[n] = VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2];
	}

	auto IndexList_sort = DotProductList.Sort("descend");
	
	int_max MaskCount_keep = 6;// 6 neighbor at the next level

	m_MaskList[Level][MaskIndex].MaskIndexListAtNextLevel = IndexList_sort.GetSubSet(0, MaskCount_keep - 1);
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::Transform3DPositonInMask_from_Output_to_Input()
{
	for (int_max Level = 0; Level < m_MaskList.GetLength(); ++Level)
	{
		for (int_max MaskIndex = 0; MaskIndex < m_MaskList[Level].GetLength(); ++MaskIndex)
		{
			auto PointP = m_MaskList[Level][MaskIndex].PointP;			
			auto PointP_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(PointP);
			m_MaskList[Level][MaskIndex].PointP = PointP_in;

			auto PointN = m_MaskList[Level][MaskIndex].PointN;
			auto PointN_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(PointN);
			m_MaskList[Level][MaskIndex].PointN = PointN;
		}
	}
}

template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->Update3DPositionTransform_Input_Output();

	if (m_GradientDirection_Prior.L2Norm() < 0.001)
	{
		this->BuildMask();
	}
	else
	{
		m_GradientDirection_Prior /= m_GradientDirection_Prior.L2Norm();
		this->BuildMaskWithGradientPrior(m_GradientDirection_Prior);
	}

	this->Transform3DPositonInMask_from_Output_to_Input();

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


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> GradientDenseImageFilter3D<InputPixelType, ScalarType>::
EvaluateAtPixelInOutputImage(int_max LinearIndex)
{
	auto Pos_out = m_OutputImage.TransformLinearIndexTo3DPosition<ScalarType>(LinearIndex);
	auto Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
	auto x0 = Pos_in[0];
	auto y0 = Pos_in[1];
	auto z0 = Pos_in[2];

	DenseVector<ScalarType, 3> Gradient_max;
	int_max MaskIndex_max = -1;

	// Level 0
	DenseVector<int_max> MaskIndexList = span(0, m_MaskList[0].GetLength() - 1);
	this->EvaluateAt3DPositionInInputImage_SingleLevel(MaskIndex_max, Gradient_max, x0, y0, z0, 0, MaskIndexList);

	//from Level 1
	for (int_max k = 1; k < m_MaskList.GetLength(); ++k)
	{
		MaskIndexList = m_MaskList[k - 1][MaskIndex_max].MaskIndexListAtNextLevel;
		this->EvaluateAt3DPositionInInputImage_SingleLevel(MaskIndex_max, Gradient_max, x0, y0, z0, k, MaskIndexList);
	}

	return Gradient_max;
}


template<typename InputPixelType, typename ScalarType>
void GradientDenseImageFilter3D<InputPixelType, ScalarType>::
EvaluateAt3DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, ScalarType z0, int_max Level, const DenseVector<int_max>& MaskIndexList)
{
	ScalarType Magnitude = 0;

	for (int_max k = 0; k < MaskIndexList.GetLength(); ++k)
	{
		auto MaskIndex = MaskIndexList[k];

		auto xp = m_MaskList[Level][MaskIndex].PointP[0] + x0;
		auto yp = m_MaskList[Level][MaskIndex].PointP[1] + y0;
		auto zp = m_MaskList[Level][MaskIndex].PointP[2] + z0;

		auto xn = m_MaskList[Level][MaskIndex].PointN[0] + x0;
		auto yn = m_MaskList[Level][MaskIndex].PointN[1] + y0;
		auto zn = m_MaskList[Level][MaskIndex].PointN[2] + z0;

		auto PixelP = m_InputImage->GetPixelAt3DPosition<ScalarType>(xp, yp, zp, m_ImageInterpolationOption);
		auto PixelN = m_InputImage->GetPixelAt3DPosition<ScalarType>(xn, yn, zn, m_ImageInterpolationOption);
		auto Difference = PixelP - PixelN;

		if (k == 0 || Magnitude < Difference)
		{
			Magnitude = Difference;
			MaskIndex_max = MaskIndex;
			Gradient_max[0] = xp - xn;
			Gradient_max[1] = yp - yn;
			Gradient_max[2] = zp - zn;
		}
	}

	auto EPS = std::numeric_limits<ScalarType>::epsilon();
	if (std::abs(Magnitude) > 3 * EPS)
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
