#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::GenericConvolutionDenseImageFilter3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::~GenericConvolutionDenseImageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	Input.Image = nullptr;
	Input.ImageInterpolationOption.MethodType = ImageInterpolationMethodEnum::Linear;
	Input.ImageInterpolationOption.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Constant;
	Input.ImageInterpolationOption.Pixel_OutsideImage = InputPixelType(0);		
	Input.ConvolutionMask_3DPositionInOutputImage.Clear();	
	Input.ConvolutionCoef.Clear();
	Input.MaxThreadCount = 1;

	Internal.ConvolutionMask_3DIndexInInputImage.Clear();
	Internal.Flag_Input_Output_Orientation_IdentityMatrix = false;
	Internal.Flag_Input_Output_SameOrigin_SameOrientation = false;
	Internal.Position3DTransformFromOuputToInput_Matrix.Clear();
	Internal.Position3DTransformFromOuputToInput_Offset.Clear();

	Output.Image.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfo(const ImageInfo3D& Info)
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
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
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
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1], Size[2]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz)
{
	if (Input.Image == nullptr)
	{
		MDK_Error("InputImage is nullptr @ GenericConvolutionDenseImageFilter3D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 0 || Ly <= 0 || Lz <= 0)
	{
		MDK_Error("Invalid input @ GenericConvolutionDenseImageFilter3D::SetOutputImageInfoBySize(...)")
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
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 3>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1], Spacing[2]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
	if (Input.Image == nullptr)
	{
		MDK_Error("InputImage is nullptr @ GenericConvolutionDenseImageFilter3D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_x <= Zero || Spacing_x <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ GenericConvolutionDenseImageFilter3D::SetOutputImageInfoBySpacing(...)")
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
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Update3DPositionTransform_Input_Output()
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
DenseVector<ScalarType, 3> GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
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
bool GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (Input.Image == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ GenericConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.Image->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ GenericConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Output.Image.IsEmpty() == true)
	{
		MDK_Error("OutputImage is Empty, call SetOutputImageInfo first  @ GenericConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.ConvolutionMask_3DPositionInOutputImage.IsEmpty() == true)
	{
		MDK_Error("ConvolutionMask is Empty @ GenericConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.ConvolutionMask_3DPositionInOutputImage.GetRowCount() != 3)
	{
		MDK_Error("ConvolutionMask size is wrong @ GenericConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.ConvolutionCoef.IsEmpty() == true)
	{
		MDK_Error("ConvolutionCoef is Empty @ GenericConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.ConvolutionMask_3DPositionInOutputImage.GetColCount() != Input.ConvolutionCoef.GetElementCount())
	{
		MDK_Error("ConvolutionMask NOT match ConvolutionCoef @ GenericConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}
	
	if (Input.MaxThreadCount <= 0)
	{
		MDK_Error("MaxThreadCount <= 0) @ GenericConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Transform3DPositionInMask()
{
	Internal.ConvolutionMask_3DIndexInInputImage.Clear();
	Internal.ConvolutionMask_3DIndexInInputImage.Resize(Input.ConvolutionMask_3DPositionInOutputImage.GetSize());	
	for (int_max k = 0; k < Input.ConvolutionMask_3DPositionInOutputImage.GetColCount(); ++k)
	{
		DenseVector<ScalarType, 3> Pos_in, Pos_out;
		Input.ConvolutionMask_3DPositionInOutputImage.GetCol(k, Pos_out);
		Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
		auto Index_in = Input.Image->Transform3DPositionTo3DIndex(Pos_in);
		Internal.ConvolutionMask_3DIndexInInputImage.SetCol(k, Index_in);
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->Update3DPositionTransform_Input_Output();
	this->Transform3DPositionInMask();

	auto PixelCount = Output.Image.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		Output.Image[k] = this->EvaluateAtPixelInOutputImage(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Input.MaxThreadCount);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::EvaluateAtPixelInOutputImage(int_max LinearIndex)
{
	auto Pos_out = Output.Image.TransformLinearIndexTo3DPosition<ScalarType>(LinearIndex);
	auto Pos_in = this->Transform3DPositionInOutputImageTo3DPositionInInputImage(Pos_out);
	auto Index_in = Input.Image->Transform3DPositionTo3DIndex(Pos_in);
	auto OutputPixel = OutputPixelType(0);
	for (int_max k = 0; k < Internal.ConvolutionMask_3DIndexInInputImage.GetColCount(); ++k)
	{
		auto x = Internal.ConvolutionMask_3DIndexInInputImage(0, k) + Index_in[0];
		auto y = Internal.ConvolutionMask_3DIndexInInputImage(1, k) + Index_in[1];
		auto z = Internal.ConvolutionMask_3DIndexInInputImage(2, k) + Index_in[2];
		auto Pixel = OutputPixelType(Input.Image->GetPixelAt3DIndex(x, y, z, Input.ImageInterpolationOption));
		OutputPixel += Pixel*Input.ConvolutionCoef[k];
	}
	return OutputPixel;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma, ScalarType CutOffRatio)
{
	this->CreateGaussianMask(Spacing, Sigma, Sigma, Sigma, CutOffRatio);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, ScalarType CutOffRatio)
{
	DenseMatrix<ScalarType> RotationMatrix;
	RotationMatrix.Resize(3, 3);
	RotationMatrix.Fill(0);
	RotationMatrix.FillDiagonal(1);
	this->CreateGaussianMask(Spacing, Sigma_x, Sigma_y, Sigma_z, RotationMatrix, CutOffRatio);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio)
{	
	if (Spacing[0] <= 0 || Spacing[1] <= 0 || Spacing[2] <= 0 || Sigma_x <= 0 || Sigma_y <= 0 || Sigma_z <= 0 || CutOffRatio <= 0)
	{
		MDK_Error("Invalid input @ GenericConvolutionDenseImageFilter3D::CreateGaussianMask(...)")
		return;
	}

	if (RotationMatrix.GetColCount() != 3 || RotationMatrix.GetRowCount() != 3)
	{
		MDK_Error("RotationMatrix is invalid @ GenericConvolutionDenseImageFilter3D::CreateGaussianMask(...)")
		return;
	}
	//---------------------------------------------------------------------------------------------------
	// Sigma is in real physical unit
	//---------------------------------------------------------------------------------------------------

	DenseMatrix<ScalarType> InverseCovarianceMatrix(3, 3);
	InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ ScalarType(1) / (Sigma_x*Sigma_x), ScalarType(1) / (Sigma_y*Sigma_y),  ScalarType(1) / (Sigma_z*Sigma_z) });

	InverseCovarianceMatrix = RotationMatrix.Transpose() * InverseCovarianceMatrix * RotationMatrix;

	ScalarType Sigma_max = std::max(std::max(Sigma_x, Sigma_y), Sigma_z);
	ScalarType Radius = 1.5*CutOffRatio*Sigma_max + 1;

	ScalarType Sigma_min = std::min(std::min(Sigma_x, Sigma_y), Sigma_z);
	int_max PointCount_max = int_max(Radius/Sigma_min) + 1;
	PointCount_max = 8*PointCount_max*PointCount_max*PointCount_max;

	// construct a grid of relative indexes according to the maximum sigma
	// at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
	// add the point to mask if mahalanobis distance <= CutOffRatio

	Input.ConvolutionMask_3DPositionInOutputImage.Clear();
	Input.ConvolutionMask_3DPositionInOutputImage.SetCapacity(3*PointCount_max);

	Input.ConvolutionCoef.Clear();
	Input.ConvolutionCoef.SetCapacity(PointCount_max);

	DenseMatrix<ScalarType> Relative3DIndex(3, 1);
	DenseMatrix<ScalarType> Relative3DIndex_Transpose(1, 3);
	DenseMatrix<ScalarType> SquaredRatio(1, 1);

	for (ScalarType z = -Radius; z <= Radius; z+=Spacing[2])
	{
		for (ScalarType y = -Radius; y <= Radius; y+=Spacing[1])
		{
			for (ScalarType x = -Radius; x <= Radius; x+=Spacing[0])
			{
				Relative3DIndex = { ScalarType(x), ScalarType(y), ScalarType(z) };
				Relative3DIndex_Transpose = { ScalarType(x), ScalarType(y), ScalarType(z) };
				SquaredRatio = Relative3DIndex_Transpose * InverseCovarianceMatrix * Relative3DIndex;
				auto tempRatio = SquaredRatio(0);
				if (tempRatio <= CutOffRatio*CutOffRatio)
				{
					ScalarType tempValue = std::exp(-ScalarType(0.5)*tempRatio);
					Input.ConvolutionMask_3DPositionInOutputImage.AppendCol({ ScalarType(x), ScalarType(y), ScalarType(z) });
					Input.ConvolutionCoef.Append(tempValue);
				}
			}
		}
	}

	Input.ConvolutionMask_3DPositionInOutputImage.ReleaseUnusedCapacity();
	Input.ConvolutionCoef.ReleaseUnusedCapacity();

	Input.ConvolutionCoef /= Input.ConvolutionCoef.Sum();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateLaplacianOfGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma, ScalarType CutOffRatio)
{
	if (Spacing[0] <= 0 || Spacing[1] <=0 || Spacing[2] <=0 || Sigma <= 0 || CutOffRatio <= 1)
	{
		MDK_Error("Invalid input @ GenericConvolutionDenseImageFilter3D::CreateLaplacianOfGaussianMask(...)")
		return;
	}

	ScalarType Radius = 1.5*CutOffRatio*Sigma + 1;

	int_max PointCount_max =  int_max(Radius / Sigma) + 1;
	PointCount_max = 8*PointCount_max*PointCount_max*PointCount_max;

	Input.ConvolutionMask_3DPositionInOutputImage.Clear();
	Input.ConvolutionMask_3DPositionInOutputImage.SetCapacity(3* PointCount_max);

	Input.ConvolutionCoef.Clear();
	Input.ConvolutionCoef.SetCapacity(PointCount_max);

	auto CutOffRatio_square = CutOffRatio*CutOffRatio;

	for (ScalarType z = -Radius; z <= Radius; z+=Spacing[2])
	{
		for (ScalarType y = -Radius; y <= Radius; y+=Spacing[1])
		{
			for (ScalarType x = -Radius; x <= Radius; x+=Spacing[0])
			{
				ScalarType temp = (x*x + y*y + z*z) / (Sigma*Sigma);
				if (temp <= CutOffRatio_square)
				{
					ScalarType tempValue = (1.0 - temp)*std::exp(-0.5*temp);
					Input.ConvolutionMask_3DPositionInOutputImage.AppendCol({ x, y, z });
					Input.ConvolutionCoef.Append(tempValue);
				}
			}
		}
	}

	Input.ConvolutionMask_3DPositionInOutputImage.ReleaseUnusedCapacity();
	Input.ConvolutionCoef.ReleaseUnusedCapacity();

	//normalize coefficient
	auto IndexList_p = Input.ConvolutionCoef.Find([](ScalarType Coef) { return Coef > ScalarType(0); });
	auto IndexList_n = Input.ConvolutionCoef.Find([](ScalarType Coef) { return Coef < ScalarType(0); });

	ScalarType Sum_p = 0;
	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Sum_p += Input.ConvolutionCoef[IndexList_p[k]];
	}

	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Input.ConvolutionCoef[IndexList_p[k]] /= Sum_p;
	}

	ScalarType Sum_n = 0;
	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Sum_n += Input.ConvolutionCoef[IndexList_n[k]];
	}
	Sum_n = -Sum_n;

	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Input.ConvolutionCoef[IndexList_n[k]] /= Sum_n;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void GenericConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateTriangleMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Radius_x, ScalarType Radius_y, ScalarType Radius_z)
{
	if (Radius_x <= 0 || Radius_y <= 0 || Radius_z <= 0)
	{
		MDK_Error("Invalid input @ GenericConvolutionDenseImageFilter3D::CreateTriangleMask(...)")
		return;
	}

	//---------------------------------------------------------------------------------------------------
	// Radius_x/y/z is in real physical unit
	auto Rx = int_max(Radius_x / Spacing[0] + 0.5) + 1;
	auto Ry = int_max(Radius_y / Spacing[1] + 0.5) + 1;
	auto Rz = int_max(Radius_z / Spacing[2] + 0.5) + 1;

	Input.ConvolutionMask_3DPositionInOutputImage.Clear();
	Input.ConvolutionMask_3DPositionInOutputImage.SetCapacity(3 * 8 * (Rx - 1)*(Ry - 1)*(Rz - 1));
	Input.ConvolutionCoef.Clear();
	Input.ConvolutionCoef.SetCapacity(8 * (Rx - 1)*(Ry - 1)*(Rz - 1));
	//---------------------------------------------------------------------------------------------------
	for (int_max z = -Rz + 1; z <= Rz - 1; ++z)
	{
		for (int_max y = -Ry + 1; y <= Ry - 1; ++y)
		{
			for (int_max x = -Rx + 1; x <= Rx - 1; ++x)
			{
				ScalarType tempValue = 1 - (ScalarType(std::abs(x)) / ScalarType(Rx))*(ScalarType(std::abs(y)) / ScalarType(Ry))*(ScalarType(std::abs(z)) / ScalarType(Rz));
				Input.ConvolutionMask_3DPositionInOutputImage.AppendCol({ Spacing[0]*x, Spacing[1]*y, Spacing[2]*z });
				Input.ConvolutionCoef.Append(tempValue);
			}
		}
	}

	Input.ConvolutionCoef /= Input.ConvolutionCoef.Sum();
}

}// namespace mdk

