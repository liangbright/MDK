#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::DiscreteConvolutionDenseImageFilter2D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::~DiscreteConvolutionDenseImageFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
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
bool DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ DiscreteConvolutionDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ DiscreteConvolutionDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.ConvolutionMask.IsEmpty() == true)
	{
		MDK_Error("ConvolutionMask is Empty @ DiscreteConvolutionDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.ConvolutionMask.GetRowCount() != 2)
	{
		MDK_Error("ConvolutionMask size is wrong @ DiscreteConvolutionDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.ConvolutionCoef.IsEmpty() == true)
	{
		MDK_Error("ConvolutionCoef is Empty @ DiscreteConvolutionDenseImageFilter2D::CheckInput()")
		return false;
	}

	if (Self.ConvolutionMask.GetColCount() != Self.ConvolutionCoef.GetElementCount())
	{
		MDK_Error("ConvolutionMask NOT match ConvolutionCoef @ DiscreteConvolutionDenseImageFilter2D::CheckInput()")
		return false;
	}
	
	if (Self.MaxThreadCount <= 0)
	{
		MDK_Error("MaxThreadCount <= 0) @ DiscreteConvolutionDenseImageFilter2D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	auto& Self = *this;
	auto Size_input = Self.InputImage->GetSize();
	auto Size_output = Self.OutputImage.GetSize();
	if (Size_output[0] != Size_input[0] || Size_output[1] != Size_input[1])
	{
		Self.OutputImage.Clear();
		Self.OutputImage.SetInfo(Self.InputImage->GetInfo());
	}
	else
	{
		Self.OutputImage.SetInfo(Self.InputImage->GetInfo(), false);
	}

	// [x_min, x_max, y_min, y_max]
	auto MaskRowX = Self.ConvolutionMask.GetRow(0);
	auto MaskRowY = Self.ConvolutionMask.GetRow(1);
	Self.MaskBox[0] = MaskRowX.Min();
	Self.MaskBox[1] = MaskRowX.Max();
	Self.MaskBox[2] = MaskRowY.Min();
	Self.MaskBox[3] = MaskRowY.Max();

	auto PixelCount = Self.OutputImage.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		Self.OutputImage[k] = this->EvaluateAtPixel(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, Self.MaxThreadCount);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::EvaluateAtPixel(int_max LinearIndex)
{
	auto& Self = *this;
	auto CenterIndex2D = Self.OutputImage.TransformLinearIndexTo2DIndex(LinearIndex);
	bool Flag_MaskOutsideImage = false;
	auto ImageSize = Self.OutputImage.GetSize();
	if (CenterIndex2D[0] + Self.MaskBox[0] < 0 || CenterIndex2D[0] + Self.MaskBox[1] >= ImageSize[0]
		|| CenterIndex2D[1] + Self.MaskBox[2] < 0 || CenterIndex2D[1] + Self.MaskBox[3] >= ImageSize[1])
	{
		Flag_MaskOutsideImage = true;
	}

	if (Flag_MaskOutsideImage == false)
	{
		auto OutputPixel = OutputPixelType(0);
		auto PtrMask_begin = Self.ConvolutionMask.GetPointer();
		auto PtrCoef = Self.ConvolutionCoef.GetPointer();
		auto PointCountInMask = Self.ConvolutionCoef.GetElementCount();
		for (auto PtrMask = PtrMask_begin; PtrMask < PtrMask_begin + 2*PointCountInMask; PtrMask += 2, ++PtrCoef)
		{
			auto x = PtrMask[0] + CenterIndex2D[0];
			auto y = PtrMask[1] + CenterIndex2D[1];
			OutputPixel += OutputPixelType((*Self.InputImage)(x, y) * PtrCoef[0]);
		}
		return OutputPixel;
	}
	else
	{
		auto OutputPixel = OutputPixelType(0);
		auto PtrMask_begin = Self.ConvolutionMask.GetPointer();
		auto PtrCoef = Self.ConvolutionCoef.GetPointer();
		auto PointCountInMask = Self.ConvolutionCoef.GetElementCount();
		for (auto PtrMask = PtrMask_begin; PtrMask < PtrMask_begin + 2*PointCountInMask; PtrMask += 2, ++PtrCoef)
		{
			auto x = PtrMask[0] + CenterIndex2D[0];
			auto y = PtrMask[1] + CenterIndex2D[1];
			
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
				else
				{
					OutputPixel += OutputPixelType((*Self.InputImage)(x, y) * PtrCoef[0]);
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

				OutputPixel += OutputPixelType((*Self.InputImage)(x, y) * PtrCoef[0]);
			}			
		}
		return OutputPixel;	
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Sigma, ScalarType CutOffRatio)
{
	this->CreateGaussianMask(Spacing, Sigma, Sigma, CutOffRatio);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType CutOffRatio)
{
	DenseMatrix<ScalarType> RotationMatrix;
	RotationMatrix.Resize(2, 2);
	RotationMatrix.Fill(0);
	RotationMatrix.FillDiagonal(1);
	this->CreateGaussianMask(Spacing, Sigma_x, Sigma_y, RotationMatrix, CutOffRatio);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio)
{	
	if (Sigma_x <= 0 || Sigma_y <= 0 || CutOffRatio <= 0)
	{
		MDK_Error("Invalid input @ DiscreteConvolutionDenseImageFilter2D::CreateGaussianMask(...)")
		return;
	}

	if (RotationMatrix.GetColCount() != 2 || RotationMatrix.GetRowCount() != 2)
	{
		MDK_Error("RotationMatrix is invalid @ DiscreteConvolutionDenseImageFilter2D::CreateGaussianMask(...)")
		return;
	}
	//---------------------------------------------------------------------------------------------------
	// Sigma is in real physical unit
	Sigma_x /= Spacing[0];
	Sigma_y /= Spacing[1];
	//---------------------------------------------------------------------------------------------------

	DenseMatrix<ScalarType> InverseCovarianceMatrix(2, 2);
	InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ ScalarType(1) / (Sigma_x*Sigma_x), ScalarType(1) / (Sigma_y*Sigma_y) });

	InverseCovarianceMatrix = RotationMatrix.Transpose() * InverseCovarianceMatrix * RotationMatrix;

	ScalarType Sigma_max = std::max(Sigma_x, Sigma_y);

	auto Radius = int_max(1.5*CutOffRatio*Sigma_max) + 1;

	// construct a grid of relative indexes according to the maximum sigma
	// at each point of the grid, compute the mahalanobis distance to the center (0,0), i.e., sqrt(SquaredRatio)
	// add the point to mask if mahalanobis distance <= CutOffRatio

	auto& Self = *this;
	Self.ConvolutionMask.FastResize(0);
	Self.ConvolutionMask.SetCapacity(3*Radius*Radius);

	Self.ConvolutionCoef.FastResize(0);
	Self.ConvolutionCoef.SetCapacity(Radius*Radius);

	DenseMatrix<ScalarType> Relative2DIndex(2, 1);
	DenseMatrix<ScalarType> Relative2DIndex_Transpose(1, 2);
	DenseMatrix<ScalarType> SquaredRatio(1, 1);

	for (int_max y = -Radius; y <= Radius; ++y)
	{
		for (int_max x = -Radius; x <= Radius; ++x)
		{
			Relative2DIndex = { ScalarType(x), ScalarType(y)};
			Relative2DIndex_Transpose = { ScalarType(x), ScalarType(y) };
			SquaredRatio = Relative2DIndex_Transpose * InverseCovarianceMatrix * Relative2DIndex;
			auto tempRatio = SquaredRatio(0);
			if (tempRatio <= CutOffRatio*CutOffRatio)
			{
				auto tempValue = std::exp(-ScalarType(0.5)*tempRatio);
				Self.ConvolutionMask.AppendCol({ ScalarType(x), ScalarType(y)});
				Self.ConvolutionCoef.Append(tempValue);
			}
		}
	}
	Self.ConvolutionMask.ReleaseUnusedCapacity();
	Self.ConvolutionCoef.ReleaseUnusedCapacity();

	Self.ConvolutionCoef /= Self.ConvolutionCoef.Sum();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
CreateLaplacianOfGaussianMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Sigma, ScalarType CutOffRatio)
{
	if (Sigma <= 0 || CutOffRatio <= 1)
	{
		MDK_Error("Invalid input @ DiscreteConvolutionDenseImageFilter2D::CreateGaussianMask(...)")
		return;
	}

	auto Radius = int_max(1.5*CutOffRatio*Sigma/Spacing.Min()) + 1;

	auto& Self = *this;
	Self.ConvolutionMask.FastResize(0);
	Self.ConvolutionMask.SetCapacity(3*Radius*Radius*Radius);

	Self.ConvolutionCoef.FastResize(0);
	Self.ConvolutionCoef.SetCapacity(Radius*Radius*Radius);

	auto CutOffRatio_square = CutOffRatio*CutOffRatio;

	for (int_max y = -Radius; y <= Radius; ++y)
	{
		for (int_max x = -Radius; x <= Radius; ++x)
		{
			auto temp = ScalarType(x*x*Spacing[0]*Spacing[0] + y*y*Spacing[1]*Spacing[1]) / (Sigma*Sigma);
			if (temp <= CutOffRatio_square)
			{
				ScalarType tempValue = (1.0 - temp)*std::exp(-0.5*temp);
				Self.ConvolutionMask.AppendCol({x, y});
				Self.ConvolutionCoef.Append(tempValue);
			}
		}
	}

	Self.ConvolutionMask.ReleaseUnusedCapacity();
	Self.ConvolutionCoef.ReleaseUnusedCapacity();

	//normalize coefficient
	auto IndexList_p = Self.ConvolutionCoef.Find([](ScalarType Coef) { return Coef > ScalarType(0); });
	auto IndexList_n = Self.ConvolutionCoef.Find([](ScalarType Coef) { return Coef < ScalarType(0); });

	ScalarType Sum_p = 0;
	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Sum_p += Self.ConvolutionCoef[IndexList_p[k]];
	}

	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Self.ConvolutionCoef[IndexList_p[k]] /= Sum_p;
	}

	ScalarType Sum_n = 0;
	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Sum_n += Self.ConvolutionCoef[IndexList_n[k]];
	}
	Sum_n = -Sum_n;

	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Self.ConvolutionCoef[IndexList_n[k]] /= Sum_n;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DiscreteConvolutionDenseImageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
CreateTriangleMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Radius_x, ScalarType Radius_y)
{
	if (Radius_x <= 0 || Radius_y <= 0)
	{
		MDK_Error("Invalid input @ DiscreteConvolutionDenseImageFilter2D::CreateTriangleMask(...)")
		return;
	}

	//---------------------------------------------------------------------------------------------------
	// Radius_x/y is in real physical unit
	auto Rx = int_max(Radius_x / Spacing[0] + 0.5) + 1;
	auto Ry = int_max(Radius_y / Spacing[1] + 0.5) + 1;	
	//---------------------------------------------------------------------------------------------------
	
	auto& Self = *this;
	Self.ConvolutionMask.Clear();
	Self.ConvolutionMask.SetCapacity(3*4*(Rx-1)*(Ry-1));

	Self.ConvolutionCoef.Clear();
	Self.ConvolutionCoef.SetCapacity(4*(Rx-1)*(Ry-1));

	for (int_max y = -Ry + 1; y <= Ry - 1; ++y)
	{
		for (int_max x = -Rx + 1; x <= Rx - 1; ++x)
		{
			ScalarType tempValue = 1 - (ScalarType(std::abs(x))/ScalarType(Rx))*(ScalarType(std::abs(y))/ScalarType(Ry));
			Self.ConvolutionMask.AppendCol({x, y});
			Self.ConvolutionCoef.Append(tempValue);
		}
	}

	Self.ConvolutionCoef /= Self.ConvolutionCoef.Sum();
}

}// namespace mdk

