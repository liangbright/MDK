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
	auto& Self = *this;
	Self.InputImage = nullptr;	
	Self.BoundaryOption = BoundaryOptionEnum::Replicate;
	Self.BoundaryValue = OutputPixelType(0);
	Self.MaskBox.Clear();
	Self.ConvolutionMask.Clear();
	Self.ConvolutionCoef.Clear();
	Self.OutputImage.Clear();
	Self.MaxThreadCount = 1;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Self.InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Self.ConvolutionMask.IsEmpty() == true)
	{
		MDK_Error("ConvolutionMask is Empty @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Self.ConvolutionMask.GetRowCount() != 3)
	{
		MDK_Error("ConvolutionMask size is wrong @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Self.ConvolutionCoef.IsEmpty() == true)
	{
		MDK_Error("ConvolutionCoef is Empty @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (Self.ConvolutionMask.GetColCount() != Self.ConvolutionCoef.GetElementCount())
	{
		MDK_Error("ConvolutionMask NOT match ConvolutionCoef @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}
	
	if (Self.MaxThreadCount <= 0)
	{
		MDK_Error("Self.MaxThreadCount <= 0) @ DiscreteConvolutionDenseImageFilter3D::CheckInput()")
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

	auto& Self = *this;
	auto Size_input = Self.InputImage->GetSize();
	auto Size_output = Self.OutputImage.GetSize();
	if (Size_output[0] != Size_input[0] || Size_output[1] != Size_input[1] || Size_output[2] != Size_input[2])
	{
		Self.OutputImage.Clear();
		Self.OutputImage.SetInfo(Self.InputImage->GetInfo());
	}
	else
	{
		Self.OutputImage.SetInfo(Self.InputImage->GetInfo(), false);
	}

	// [x_min, x_max, y_min, y_max, z_min, z_max]
	auto MaskRowX = Self.ConvolutionMask.GetRow(0);
	auto MaskRowY = Self.ConvolutionMask.GetRow(1);
	auto MaskRowZ = Self.ConvolutionMask.GetRow(2);
	Self.MaskBox[0] = MaskRowX.Min();
	Self.MaskBox[1] = MaskRowX.Max();
	Self.MaskBox[2] = MaskRowY.Min();
	Self.MaskBox[3] = MaskRowY.Max();
	Self.MaskBox[4] = MaskRowZ.Min();
	Self.MaskBox[5] = MaskRowZ.Max();

	auto PixelCount = Self.OutputImage.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		Self.OutputImage[k] = this->EvaluateAtPixel(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Self.MaxThreadCount);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::EvaluateAtPixel(int_max LinearIndex)
{
	auto& Self = *this;
	auto CenterIndex3D = Self.OutputImage.TransformLinearIndexTo3DIndex(LinearIndex);
	bool Flag_MaskOutsideImage = false;
	auto ImageSize = Self.OutputImage.GetSize();
	if (CenterIndex3D[0] + Self.MaskBox[0] < 0 || CenterIndex3D[0] + Self.MaskBox[1] >= ImageSize[0]
		|| CenterIndex3D[1] + Self.MaskBox[2] < 0 || CenterIndex3D[1] + Self.MaskBox[3] >= ImageSize[1]
		|| CenterIndex3D[2] + Self.MaskBox[4] < 0 || CenterIndex3D[2] + Self.MaskBox[5] >= ImageSize[2])
	{
		Flag_MaskOutsideImage = true;
	}

	if (Flag_MaskOutsideImage == false)
	{
		auto OutputPixel = OutputPixelType(0);
		auto PtrMask_begin = Self.ConvolutionMask.GetPointer();
		auto PtrCoef = Self.ConvolutionCoef.GetPointer();
		auto PointCountInMask = Self.ConvolutionCoef.GetElementCount();
		for (auto PtrMask = PtrMask_begin; PtrMask < PtrMask_begin + 3*PointCountInMask; PtrMask += 3, ++PtrCoef)
		{
			auto x = PtrMask[0] + CenterIndex3D[0];
			auto y = PtrMask[1] + CenterIndex3D[1];
			auto z = PtrMask[2] + CenterIndex3D[2];
			OutputPixel += OutputPixelType((*Self.InputImage)(x, y, z) * PtrCoef[0]);
		}
		return OutputPixel;
	}
	else
	{
		auto OutputPixel = OutputPixelType(0);
		auto PtrMask_begin = Self.ConvolutionMask.GetPointer();
		auto PtrCoef = Self.ConvolutionCoef.GetPointer();
		auto PointCountInMask = Self.ConvolutionCoef.GetElementCount();
		for (auto PtrMask = PtrMask_begin; PtrMask < PtrMask_begin + 3*PointCountInMask; PtrMask += 3, ++PtrCoef)
		{
			auto x = PtrMask[0] + CenterIndex3D[0];
			auto y = PtrMask[1] + CenterIndex3D[1];
			auto z = PtrMask[2] + CenterIndex3D[2];			
			
			if (Self.BoundaryOption == BoundaryOptionEnum::Constant)
			{
				if (x < 0 || x >= ImageSize[0])
				{
					OutputPixel += Self.BoundaryValue*PtrCoef[0];
				}
				else  if (y < 0 || y >= ImageSize[1])
				{
					OutputPixel += Self.BoundaryValue*PtrCoef[0];
				}
				else if (z < 0 || z >= ImageSize[2])
				{
					OutputPixel += Self.BoundaryValue*PtrCoef[0];
				}
				else
				{
					OutputPixel += OutputPixelType((*Self.InputImage)(x, y, z) * PtrCoef[0]);
				}
			}
			else if (Self.BoundaryOption == BoundaryOptionEnum::Replicate)
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

				OutputPixel += OutputPixelType((*Self.InputImage)(x, y, z) * PtrCoef[0]);
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

	auto& Self = *this;
	Self.ConvolutionMask.Clear();
	Self.ConvolutionMask.SetCapacity(3*Radius*Radius*Radius);

	Self.ConvolutionCoef.Clear();
	Self.ConvolutionCoef.SetCapacity(Radius*Radius*Radius);

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
					Self.ConvolutionMask.AppendCol({x, y, z});
					Self.ConvolutionCoef.Append(tempValue);
				}
			}
		}
	}

	Self.ConvolutionMask.ReleaseUnusedCapacity();
	Self.ConvolutionCoef.ReleaseUnusedCapacity();

	Self.ConvolutionCoef /= Self.ConvolutionCoef.Sum();
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

	auto& Self = *this;
	Self.ConvolutionMask.Clear();
	Self.ConvolutionMask.SetCapacity(3*Radius*Radius*Radius);

	Self.ConvolutionCoef.Clear();
	Self.ConvolutionCoef.SetCapacity(Radius*Radius*Radius);

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
					Self.ConvolutionMask.AppendCol({x, y, z});
					Self.ConvolutionCoef.Append(tempValue);
				}
			}
		}
	}

	Self.ConvolutionMask.ReleaseUnusedCapacity();
	Self.ConvolutionCoef.ReleaseUnusedCapacity();

	//normalize coefficient
	auto IndexList_p = Self.ConvolutionCoef.Find([](ScalarType Coef) { return Coef > ScalarType(0); });
	auto IndexList_n = Self.ConvolutionCoef.Find([](ScalarType Coef) { return Coef < ScalarType(0); });

	ScalarType Sump = 0;
	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Sump += Self.ConvolutionCoef[IndexList_p[k]];
	}

	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Self.ConvolutionCoef[IndexList_p[k]] /= Sump;
	}

	ScalarType Sumn = 0;
	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Sumn += Self.ConvolutionCoef[IndexList_n[k]];
	}
	Sumn = -Sumn;

	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Self.ConvolutionCoef[IndexList_n[k]] /= Sumn;
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
	auto& Self = *this;
	Self.ConvolutionMask.Clear();
	Self.ConvolutionMask.SetCapacity(3*8*(Rx-1)*(Ry-1)*(Rz-1));

	Self.ConvolutionCoef.Clear();
	Self.ConvolutionCoef.SetCapacity(8*(Rx-1)*(Ry-1)*(Rz-1));

	for (int_max z = -Rz+1; z <= Rz-1; ++z)
	{
		for (int_max y = -Ry+1; y <= Ry-1; ++y)
		{
			for (int_max x = -Rx+1; x <= Rx-1; ++x)
			{
				ScalarType tempValue = 1 - (ScalarType(std::abs(x))/ScalarType(Rx))*(ScalarType(std::abs(y))/ScalarType(Ry))*(ScalarType(std::abs(z))/ScalarType(Rz));
				Self.ConvolutionMask.AppendCol({x, y, z});
				Self.ConvolutionCoef.Append(tempValue);
			}
		}
	}

	Self.ConvolutionCoef /= Self.ConvolutionCoef.Sum();
}

}// namespace mdk

