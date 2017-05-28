#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::DiscreteConvolutionDenseImageFilter3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::~DiscreteConvolutionDenseImageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	Input.Image = nullptr;	
	Input.BoundaryOption = BoundaryOptionEnum::Replicate;
	Input.BoundaryValue = OutputPixelType(0);	
	Input.ConvolutionMask.Clear();
	Input.ConvolutionCoef.Clear();
	Input.MaxThreadCount = 1;
	Internal.MaskBox.Clear();
	Output.Image.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (Input.Image == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.Image->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.ConvolutionMask.IsEmpty() == true)
	{
		MDK_Error("ConvolutionMask is Empty @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.ConvolutionMask.GetRowCount() != 3)
	{
		MDK_Error("ConvolutionMask size is wrong @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.ConvolutionCoef.IsEmpty() == true)
	{
		MDK_Error("ConvolutionCoef is Empty @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Input.ConvolutionMask.GetColCount() != Input.ConvolutionCoef.GetElementCount())
	{
		MDK_Error("ConvolutionMask NOT match ConvolutionCoef @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}
	
	if (Input.MaxThreadCount <= 0)
	{
		MDK_Error("MaxThreadCount <= 0) @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	auto Size_input = Input.Image->GetSize();
	auto Size_output = Output.Image.GetSize();
	if (Size_output[0] != Size_input[0] || Size_output[1] != Size_input[1] || Size_output[2] != Size_input[2])
	{
		Output.Image.Clear();
		Output.Image.SetInfo(Input.Image->GetInfo());
	}
	else
	{
		Output.Image.SetInfo(Input.Image->GetInfo(), false);
	}

	// [x_min, x_max, y_min, y_max, z_min, z_max]
	auto MaskRowX = Input.ConvolutionMask.GetRow(0);
	auto MaskRowY = Input.ConvolutionMask.GetRow(1);
	auto MaskRowZ = Input.ConvolutionMask.GetRow(2);
	Internal.MaskBox[0] = MaskRowX.Min();
	Internal.MaskBox[1] = MaskRowX.Max();
	Internal.MaskBox[2] = MaskRowY.Min();
	Internal.MaskBox[3] = MaskRowY.Max();
	Internal.MaskBox[4] = MaskRowZ.Min();
	Internal.MaskBox[5] = MaskRowZ.Max();

	auto PixelCount = Output.Image.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		Output.Image[k] = this->EvaluateAtPixel(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Input.MaxThreadCount);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::EvaluateAtPixel(int_max LinearIndex)
{
	auto CenterIndex3D = Output.Image.TransformLinearIndexTo3DIndex(LinearIndex);
	bool Flag_MaskOutsideImage = false;
	auto ImageSize = Output.Image.GetSize();
	if (CenterIndex3D[0] + Internal.MaskBox[0] < 0 || CenterIndex3D[0] + Internal.MaskBox[1] >= ImageSize[0]
		|| CenterIndex3D[1] + Internal.MaskBox[2] < 0 || CenterIndex3D[1] + Internal.MaskBox[3] >= ImageSize[1]
		|| CenterIndex3D[2] + Internal.MaskBox[4] < 0 || CenterIndex3D[2] + Internal.MaskBox[5] >= ImageSize[2])
	{
		Flag_MaskOutsideImage = true;
	}

	if (Flag_MaskOutsideImage == false)
	{
		auto OutputPixel = OutputPixelType(0);
		auto PtrMask_begin = Input.ConvolutionMask.GetPointer();
		auto PtrCoef = Input.ConvolutionCoef.GetPointer();
		auto PointCountInMask = Input.ConvolutionCoef.GetElementCount();
		for (auto PtrMask = PtrMask_begin; PtrMask < PtrMask_begin + 3*PointCountInMask; PtrMask += 3, ++PtrCoef)
		{
			auto x = PtrMask[0] + CenterIndex3D[0];
			auto y = PtrMask[1] + CenterIndex3D[1];
			auto z = PtrMask[2] + CenterIndex3D[2];
			OutputPixel += OutputPixelType((*Input.Image)(x, y, z) * PtrCoef[0]);
		}
		return OutputPixel;
	}
	else
	{
		auto OutputPixel = OutputPixelType(0);
		auto PtrMask_begin = Input.ConvolutionMask.GetPointer();
		auto PtrCoef = Input.ConvolutionCoef.GetPointer();
		auto PointCountInMask = Input.ConvolutionCoef.GetElementCount();
		for (auto PtrMask = PtrMask_begin; PtrMask < PtrMask_begin + 3*PointCountInMask; PtrMask += 3, ++PtrCoef)
		{
			auto x = PtrMask[0] + CenterIndex3D[0];
			auto y = PtrMask[1] + CenterIndex3D[1];
			auto z = PtrMask[2] + CenterIndex3D[2];			
			
			if (Input.BoundaryOption == BoundaryOptionEnum::Constant)
			{
				if (x < 0 || x >= ImageSize[0])
				{
					OutputPixel += Input.BoundaryValue*PtrCoef[0];
				}
				else  if (y < 0 || y >= ImageSize[1])
				{
					OutputPixel += Input.BoundaryValue*PtrCoef[0];
				}
				else if (z < 0 || z >= ImageSize[2])
				{
					OutputPixel += Input.BoundaryValue*PtrCoef[0];
				}
				else
				{
					OutputPixel += OutputPixelType((*Input.Image)(x, y, z) * PtrCoef[0]);
				}
			}
			else if (Input.BoundaryOption == BoundaryOptionEnum::Replicate)
			{
				if (x < 0)
				{
					x = 0;
				}
				else if (x >= ImageSize[0])
				{
					x = ImageSize[0] - 1;
				}

				if (y < 0)
				{
					y = 0;
				}
				else if (y >= ImageSize[1])
				{
					y = ImageSize[1] - 1;
				}

				if (z < 0)
				{
					z = 0;
				}
				else if (z >= ImageSize[2])
				{
					z = ImageSize[2] - 1;
				}

				OutputPixel += OutputPixelType((*Input.Image)(x, y, z) * PtrCoef[0]);
			}			
		}
		return OutputPixel;	
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma, ScalarType CutOffRatio)
{
	this->CreateGaussianMask(Spacing, Sigma, Sigma, Sigma, CutOffRatio);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, ScalarType CutOffRatio)
{
	DenseMatrix<ScalarType> RotationMatrix;
	RotationMatrix.Resize(3, 3);
	RotationMatrix.Fill(0);
	RotationMatrix.FillDiagonal(1);
	this->CreateGaussianMask(Spacing, Sigma_x, Sigma_y, Sigma_z, RotationMatrix, CutOffRatio);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio)
{	
	if (Sigma_x <= 0 || Sigma_y <= 0 || Sigma_z <= 0 || CutOffRatio <= 0)
	{
		MDK_Error("Invalid input @ DiscreteConvolutionDenseImageFilter3D::CreateGaussianMask(...)")
		return;
	}

	if (RotationMatrix.GetColCount() != 3 || RotationMatrix.GetRowCount() != 3)
	{
		MDK_Error("RotationMatrix is invalid @ DiscreteConvolutionDenseImageFilter3D::CreateGaussianMask(...)")
		return;
	}
	//---------------------------------------------------------------------------------------------------
	// Sigma is in real physical unit
	Sigma_x /= Spacing[0];
	Sigma_y /= Spacing[1];
	Sigma_z /= Spacing[2];
	//---------------------------------------------------------------------------------------------------

	DenseMatrix<ScalarType> InverseCovarianceMatrix(3, 3);
	InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ ScalarType(1) / (Sigma_x*Sigma_x), ScalarType(1) / (Sigma_y*Sigma_y),  ScalarType(1) / (Sigma_z*Sigma_z) });

	InverseCovarianceMatrix = RotationMatrix.Transpose() * InverseCovarianceMatrix * RotationMatrix;

	ScalarType Sigma_max = std::max(std::max(Sigma_x, Sigma_y), Sigma_z);

	auto Radius = int_max(1.5*CutOffRatio*Sigma_max) + 1;

	// construct a grid of relative indexes according to the maximum sigma
	// at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
	// add the point to mask if mahalanobis distance <= CutOffRatio

	Input.ConvolutionMask.Clear();
	Input.ConvolutionMask.SetCapacity(3*Radius*Radius*Radius);

	Input.ConvolutionCoef.Clear();
	Input.ConvolutionCoef.SetCapacity(Radius*Radius*Radius);

	DenseMatrix<ScalarType> Relative3DIndex(3, 1);
	DenseMatrix<ScalarType> Relative3DIndex_Transpose(1, 3);
	DenseMatrix<ScalarType> SquaredRatio(1, 1);

	for (int_max z = -Radius; z <= Radius; ++z)
	{
		for (int_max y = -Radius; y <= Radius; ++y)
		{
			for (int_max x = -Radius; x <= Radius; ++x)
			{
				Relative3DIndex = { ScalarType(x), ScalarType(y), ScalarType(z) };
				Relative3DIndex_Transpose = { ScalarType(x), ScalarType(y), ScalarType(z) };
				SquaredRatio = Relative3DIndex_Transpose * InverseCovarianceMatrix * Relative3DIndex;
				auto tempRatio = SquaredRatio(0);
				if (tempRatio <= CutOffRatio*CutOffRatio)
				{
					auto tempValue = std::exp(-ScalarType(0.5)*tempRatio);
					Input.ConvolutionMask.AppendCol({x, y, z});
					Input.ConvolutionCoef.Append(tempValue);
				}
			}
		}
	}

	Input.ConvolutionMask.ReleaseUnusedCapacity();
	Input.ConvolutionCoef.ReleaseUnusedCapacity();

	Input.ConvolutionCoef /= Input.ConvolutionCoef.Sum();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateLaplacianOfGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma, ScalarType CutOffRatio)
{
	if (Sigma <= 0 || CutOffRatio <= 1)
	{
		MDK_Error("Invalid input @ DiscreteConvolutionDenseImageFilter3D::CreateGaussianMask(...)")
		return;
	}

	auto Radius = int_max(1.5*CutOffRatio*Sigma/Spacing.Min()) + 1;

	Input.ConvolutionMask.Clear();
	Input.ConvolutionMask.SetCapacity(3*Radius*Radius*Radius);

	Input.ConvolutionCoef.Clear();
	Input.ConvolutionCoef.SetCapacity(Radius*Radius*Radius);

	auto CutOffRatio_square = CutOffRatio*CutOffRatio;

	for (int_max z = -Radius; z <= Radius; ++z)
	{
		for (int_max y = -Radius; y <= Radius; ++y)
		{
			for (int_max x = -Radius; x <= Radius; ++x)
			{
				auto temp = ScalarType(x*x*Spacing[0]*Spacing[0] + y*y*Spacing[1]*Spacing[1] + z*z*Spacing[2]*Spacing[2]) / (Sigma*Sigma);
				if (temp <= CutOffRatio_square)
				{
					ScalarType tempValue = (1.0 - temp)*std::exp(-0.5*temp);
					Input.ConvolutionMask.AppendCol({x, y, z});
					Input.ConvolutionCoef.Append(tempValue);
				}
			}
		}
	}

	Input.ConvolutionMask.ReleaseUnusedCapacity();
	Input.ConvolutionCoef.ReleaseUnusedCapacity();

	//normalize coefficient
	auto IndexList_p = Input.ConvolutionCoef.Find([](ScalarType Coef) { return Coef > ScalarType(0); });
	auto IndexList_n = Input.ConvolutionCoef.Find([](ScalarType Coef) { return Coef < ScalarType(0); });

	ScalarType Sump = 0;
	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Sump += Input.ConvolutionCoef[IndexList_p[k]];
	}

	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Input.ConvolutionCoef[IndexList_p[k]] /= Sump;
	}

	ScalarType Sumn = 0;
	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Sumn += Input.ConvolutionCoef[IndexList_n[k]];
	}
	Sumn = -Sumn;

	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Input.ConvolutionCoef[IndexList_n[k]] /= Sumn;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateTriangleMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Radius_x, ScalarType Radius_y, ScalarType Radius_z)
{
	if (Radius_x <= 0 || Radius_y <= 0 || Radius_z <= 0)
	{
		MDK_Error("Invalid input @ DiscreteConvolutionDenseImageFilter3D::CreateTriangleMask(...)")
		return;
	}

	//---------------------------------------------------------------------------------------------------
	// Radius_x/y/z is in real physical unit
	auto Rx = int_max(Radius_x / Spacing[0] + 0.5) + 1;
	auto Ry = int_max(Radius_y / Spacing[1] + 0.5) + 1;
	auto Rz = int_max(Radius_z / Spacing[2] + 0.5) + 1;
	//---------------------------------------------------------------------------------------------------
	Input.ConvolutionMask.Clear();
	Input.ConvolutionMask.SetCapacity(3*8*(Rx-1)*(Ry-1)*(Rz-1));

	Input.ConvolutionCoef.Clear();
	Input.ConvolutionCoef.SetCapacity(8*(Rx-1)*(Ry-1)*(Rz-1));

	for (int_max z = -Rz+1; z <= Rz-1; ++z)
	{
		for (int_max y = -Ry+1; y <= Ry-1; ++y)
		{
			for (int_max x = -Rx+1; x <= Rx-1; ++x)
			{
				ScalarType tempValue = 1 - (ScalarType(std::abs(x))/ScalarType(Rx))*(ScalarType(std::abs(y))/ScalarType(Ry))*(ScalarType(std::abs(z))/ScalarType(Rz));
				Input.ConvolutionMask.AppendCol({x, y, z});
				Input.ConvolutionCoef.Append(tempValue);
			}
		}
	}

	Input.ConvolutionCoef /= Input.ConvolutionCoef.Sum();
}

}// namespace mdk

