#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::ConvolutionDenseImageFilter3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ConvolutionDenseImageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	m_InputImage = nullptr;	
	m_BoundaryOption = BoundaryOptionEnum::Replicate;
	m_BoundaryValue = OutputPixelType(0);
	m_ConvolutionMask.Clear();
	m_ConvolutionCoef.Clear();
	m_OutputImage.Clear();
	m_MaxThreadCount = 1;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ ConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ ConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (m_ConvolutionMask.IsEmpty() == true)
	{
		MDK_Error("ConvolutionMask is Empty @ ConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (m_ConvolutionMask.GetRowCount() != 3)
	{
		MDK_Error("ConvolutionMask size is wrong @ ConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (m_ConvolutionCoef.IsEmpty() == true)
	{
		MDK_Error("ConvolutionCoef is Empty @ ConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	if (m_ConvolutionMask.GetColCount() != m_ConvolutionCoef.GetElementCount())
	{
		MDK_Error("ConvolutionCoef NOT match ConvolutionCoef @ ConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}
	
	if (m_MaxThreadCount <= 0)
	{
		MDK_Error("m_MaxThreadCount <= 0) @ ConvolutionDenseImageFilter3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	auto Size_input = m_InputImage->GetSize();
	auto Size_output = m_OutputImage.GetSize();
	if (Size_output[0] != Size_input[0] || Size_output[1] != Size_input[1] || Size_output[2] != Size_input[2])
	{
		m_OutputImage.Clear();
		m_OutputImage.SetInfo(m_InputImage->GetInfo());
	}
	m_OutputImage.Fill(m_BoundaryValue);

	// [x_min, x_max, y_min, y_max, z_min, z_max]
	auto MaskRowX = m_ConvolutionMask.GetRow(0);
	auto MaskRowY = m_ConvolutionMask.GetRow(1);
	auto MaskRowZ = m_ConvolutionMask.GetRow(2);
	m_MaskBox[0] = MaskRowX.Min();
	m_MaskBox[1] = MaskRowX.Max();
	m_MaskBox[2] = MaskRowY.Min();
	m_MaskBox[3] = MaskRowY.Max();
	m_MaskBox[4] = MaskRowZ.Min();
	m_MaskBox[5] = MaskRowZ.Max();

	auto PixelCount = m_OutputImage.GetPixelCount();
	//for (int_max k = 0; k <= PixelCount-1; ++k)
	auto TempFunction = [&](int_max k)
	{
		m_OutputImage[k] = this->EvaluateAtPixel(k);
	};
	ParallelForLoop(TempFunction, 0, PixelCount - 1, m_MaxThreadCount);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::EvaluateAtPixel(int_max LinearIndex)
{
	auto CenterIndex3D = m_OutputImage.TransformLinearIndexTo3DIndex(LinearIndex);
	bool Flag_MaskOutsideImage = false;
	auto ImageSize = m_OutputImage.GetSize();
	if (CenterIndex3D[0] + m_MaskBox[0] < 0 || CenterIndex3D[0] + m_MaskBox[1] >= ImageSize[0]
		|| CenterIndex3D[1] + m_MaskBox[2] < 0 || CenterIndex3D[1] + m_MaskBox[3] >= ImageSize[1]
		|| CenterIndex3D[2] + m_MaskBox[4] < 0 || CenterIndex3D[2] + m_MaskBox[5] >= ImageSize[2])
	{
		Flag_MaskOutsideImage = true;
	}

	if (Flag_MaskOutsideImage == false)
	{
		auto OutputPixel = OutputPixelType(0);
		auto PtrMask_begin = m_ConvolutionMask.GetPointer();
		auto PtrCoef = m_ConvolutionCoef.GetPointer();
		auto PointCountInMask = m_ConvolutionCoef.GetElementCount();
		for (auto PtrMask = PtrMask_begin; PtrMask < PtrMask_begin + 3*PointCountInMask; PtrMask += 3, ++PtrCoef)
		{
			auto x = PtrMask[0] + CenterIndex3D[0];
			auto y = PtrMask[1] + CenterIndex3D[1];
			auto z = PtrMask[2] + CenterIndex3D[2];
			OutputPixel += OutputPixelType((*m_InputImage)(x, y, z) * PtrCoef[0]);
		}
		return OutputPixel;
	}
	else
	{
		auto OutputPixel = OutputPixelType(0);
		auto PtrMask_begin = m_ConvolutionMask.GetPointer();
		auto PtrCoef = m_ConvolutionCoef.GetPointer();
		auto PointCountInMask = m_ConvolutionCoef.GetElementCount();
		for (auto PtrMask = PtrMask_begin; PtrMask < PtrMask_begin + 3*PointCountInMask; PtrMask += 3, ++PtrCoef)
		{
			auto x = PtrMask[0] + CenterIndex3D[0];
			auto y = PtrMask[1] + CenterIndex3D[1];
			auto z = PtrMask[2] + CenterIndex3D[2];			
			
			if (m_BoundaryOption == BoundaryOptionEnum::Constant)
			{
				if (x < 0 || x >= ImageSize[0])
				{
					OutputPixel += m_BoundaryValue*PtrCoef[0];
				}
				else  if (y < 0 || y >= ImageSize[1])
				{
					OutputPixel += m_BoundaryValue*PtrCoef[0];
				}
				else if (z < 0 || z >= ImageSize[2])
				{
					OutputPixel += m_BoundaryValue*PtrCoef[0];
				}
				else
				{
					OutputPixel += OutputPixelType((*m_InputImage)(x, y, z) * PtrCoef[0]);
				}
			}
			else if (m_BoundaryOption == BoundaryOptionEnum::Replicate)
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

				OutputPixel += OutputPixelType((*m_InputImage)(x, y, z) * PtrCoef[0]);
			}			
		}
		return OutputPixel;	
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::CreateGaussianMask(const ImageInfo3D& InputImageInfo, ScalarType Sigma, ScalarType CutOffRatio)
{
	this->CreateGaussianMask(InputImageInfo, Sigma, Sigma, Sigma, CutOffRatio);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const ImageInfo3D& InputImageInfo, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, ScalarType CutOffRatio)
{
	DenseMatrix<ScalarType> RotationMatrix;
	RotationMatrix.Resize(3, 3);
	RotationMatrix.Fill(0);
	RotationMatrix.FillDiagonal(1);
	this->CreateGaussianMask(InputImageInfo, Sigma_x, Sigma_y, Sigma_z, RotationMatrix, CutOffRatio);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateGaussianMask(const ImageInfo3D& InputImageInfo, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio)
{	
	if (Sigma_x <= 0 || Sigma_y <= 0 || Sigma_z <= 0 || CutOffRatio <= 0)
	{
		MDK_Error("Invalid input @ ConvolutionDenseImageFilter3D::CreateGaussianMask(...)")
		return;
	}

	if (RotationMatrix.GetColCount() != 3 || RotationMatrix.GetRowCount() != 3)
	{
		MDK_Error("RotationMatrix is invalid @ ConvolutionDenseImageFilter3D::CreateGaussianMask(...)")
		return;
	}
	//---------------------------------------------------------------------------------------------------
	// Sigma is in real physical unit
	auto Spacing = InputImageInfo.Spacing;
	Sigma_x /= Spacing[0];
	Sigma_y /= Spacing[1];
	Sigma_z /= Spacing[2];
	//---------------------------------------------------------------------------------------------------

	DenseMatrix<ScalarType> InverseCovarianceMatrix(3, 3);
	InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ ScalarType(1) / (Sigma_x*Sigma_x), ScalarType(1) / (Sigma_y*Sigma_y),  ScalarType(1) / (Sigma_z*Sigma_z) });

	InverseCovarianceMatrix = RotationMatrix.Transpose() * InverseCovarianceMatrix * RotationMatrix;

	ScalarType Sigma_max = std::max(std::max(Sigma_x, Sigma_y), Sigma_z);

	auto Radius = int_max(Sigma_max*2.5) + 1;

	// construct a grid of relative indexes according to the maximum sigma
	// at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
	// add the point to mask if mahalanobis distance <= CutOffRatio

	m_ConvolutionMask.FastResize(0);
	m_ConvolutionMask.SetCapacity(3*Radius*Radius*Radius);

	m_ConvolutionCoef.FastResize(0);
	m_ConvolutionCoef.SetCapacity(Radius*Radius*Radius);

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
					m_ConvolutionMask.AppendCol({ ScalarType(x), ScalarType(y), ScalarType(z) });
					m_ConvolutionCoef.Append(tempValue);
				}
			}
		}
	}

	m_ConvolutionMask.ReleaseUnusedCapacity();
	m_ConvolutionCoef.ReleaseUnusedCapacity();

	m_ConvolutionCoef /= m_ConvolutionCoef.Sum();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
CreateLaplacianOfGaussianMask(const ImageInfo3D& InputImageInfo, ScalarType Sigma, ScalarType CutOffRatio)
{
	if (Sigma <= 0 || CutOffRatio <= 1)
	{
		MDK_Error("Invalid input @ ConvolutionDenseImageFilter3D::CreateGaussianMask(...)")
		return;
	}

	auto Spacing = InputImageInfo.Spacing;	
	auto Radius = int_max(2.5*Sigma/Spacing.Min()) + 1;

	m_ConvolutionMask.FastResize(0);
	m_ConvolutionMask.SetCapacity(3*Radius*Radius*Radius);

	m_ConvolutionCoef.FastResize(0);
	m_ConvolutionCoef.SetCapacity(Radius*Radius*Radius);

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
					m_ConvolutionMask.AppendCol({ x, y, z });
					m_ConvolutionCoef.Append(tempValue);
				}
			}
		}
	}

	m_ConvolutionMask.ReleaseUnusedCapacity();
	m_ConvolutionCoef.ReleaseUnusedCapacity();

	//normalize coefficient
	auto IndexList_p = m_ConvolutionCoef.Find([](ScalarType Coef) { return Coef > ScalarType(0); });
	auto IndexList_n = m_ConvolutionCoef.Find([](ScalarType Coef) { return Coef < ScalarType(0); });

	ScalarType Sum_p = 0;
	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Sum_p += m_ConvolutionCoef[IndexList_p[k]];
	}

	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		m_ConvolutionCoef[IndexList_p[k]] /= Sum_p;
	}

	ScalarType Sum_n = 0;
	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Sum_n += m_ConvolutionCoef[IndexList_n[k]];
	}
	Sum_n = -Sum_n;

	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		m_ConvolutionCoef[IndexList_n[k]] /= Sum_n;
	}
}

}// namespace mdk

