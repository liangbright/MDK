#ifndef __mdkScalarDenseImageGaussianFilter3D_hpp
#define __mdkScalarDenseImageGaussianFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageGaussianFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageGaussianFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
    this->ScalarDenseImageConvolutionFilter3D::Clear();
    m_SigmaList.Clear();
    m_RotationMatrix.Clear();
    m_CutOffRatio = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::
SetGaussianParameter(const DenseVector<double, 3>& SigmaList, const DenseMatrix<double>& RotationMatrix, double CutOffRatio)
{    
	m_SigmaList = SigmaList;
    m_RotationMatrix = RotationMatrix;    
    m_CutOffRatio = CutOffRatio;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->ScalarDenseImageConvolutionFilter3D::CheckInput() == false)
	{
		return false;
	}

	if (m_SigmaList[0] <= 0.0 || m_SigmaList[1] <= 0.0 || m_SigmaList[2] <= 0.0)
	{
		MDK_Error("Sigma_x <= 0.0 || Sigma_y <= 0.0 || Sigma_z <= 0.0 @ ScalarDenseImageGaussianFilter3D::CheckInput(...)")
		return false;
	}

	if (m_RotationMatrix.GetColNumber() != 3 || m_RotationMatrix.GetRowNumber() != 3)
	{
		MDK_Error("RotationMatrix is invalid @ ScalarDenseImageGaussianFilter3D::CheckInput(...)")
		return false;
	}

	if (m_CutOffRatio <= 0.0)
	{
		MDK_Error("CutOffRatio <= 0.0 @ ScalarDenseImageGaussianFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ScalarDenseImageConvolutionFilter3D::Preprocess() == false)
	{
		return false;
	}
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DPhysicalPosition()
{
	DenseMatrix<double> InverseCovarianceMatrix(3, 3);
    InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ 1.0 / (m_SigmaList(0)*m_SigmaList(0)),
										  1.0 / (m_SigmaList(1)*m_SigmaList(1)), 
										  1.0 / (m_SigmaList(2)*m_SigmaList(2)) });

    InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;

	auto Spacing = m_OutputImageInfo.Spacing;

	auto MaxRadius = m_SigmaList.Max() * m_CutOffRatio * 1.5;
	auto MaxRadius_x = double(int_max(MaxRadius/Spacing[0]) + 1)*Spacing[0];
	auto MaxRadius_y = double(int_max(MaxRadius/Spacing[1]) + 1)*Spacing[1];
	auto MaxRadius_z = double(int_max(MaxRadius/Spacing[2]) + 1)*Spacing[2];

    // construct a grid of relative indexes according to the maximum sigma
    // at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
    // add the points within the m_CutOffRatio to Mask
    
	m_Mask_3DPhysicalPosition.FastResize(0);
	m_Mask_3DPhysicalPosition.ReserveCapacity(3 * MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(MaxRadius*MaxRadius*MaxRadius);

	DenseMatrix<ScalarType> Position(3, 1);
	DenseMatrix<ScalarType> Position_Transpose(1, 3);
	DenseMatrix<double> SquaredRatio(1, 1);

    auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (auto z = -MaxRadius_z; z <= MaxRadius_z; z += Spacing[2])
    {
		for (auto y = -MaxRadius_y; y <= MaxRadius_y; y += Spacing[1])
        {
			for (auto x = -MaxRadius_x; x <= MaxRadius_x; x += Spacing[0])
            {
				Position[0] = x;
				Position[1] = y;
				Position[2] = z;
				Position_Transpose[0] = x;   
				Position_Transpose[1] = y;
				Position_Transpose[2] = z;
				SquaredRatio = Position_Transpose * InverseCovarianceMatrix * Position;
                auto tempRatio = SquaredRatio(0);

                if (tempRatio <= CutOffRatio_square)
                {
					auto tempValue = std::exp(-0.5*tempRatio);
					m_Mask_3DPhysicalPosition.AppendCol(Position);
					m_ConvolutionCoefficient.Append(ScalarType(tempValue));
                }
            }
        }
    }

	m_Mask_3DPhysicalPosition.ReleaseUnusedCapacity();
	m_ConvolutionCoefficient.ReleaseUnusedCapacity();

	//normalize coefficient
	m_ConvolutionCoefficient /= m_ConvolutionCoefficient.Sum();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DIndex()
{
	auto InputImageSpacing = m_InputImage->GetSpacing();

	DenseVector<double, 3> Sigma_xyz;
	Sigma_xyz[0] = m_SigmaList[0] / InputImageSpacing[0];
	Sigma_xyz[1] = m_SigmaList[1] / InputImageSpacing[1];
	Sigma_xyz[2] = m_SigmaList[2] / InputImageSpacing[2];

	DenseMatrix<double> InverseCovarianceMatrix(3, 3);
	InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ 1.0 / (Sigma_xyz[0] * Sigma_xyz[0]),
								          1.0 / (Sigma_xyz[1] * Sigma_xyz[1]), 
										  1.0 / (Sigma_xyz[2] * Sigma_xyz[2]) });

	InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;

	auto MaxRadius = int_max(Sigma_xyz.Max() * m_CutOffRatio * 1.5) + 1;

	// construct a grid of relative indexes according to the maximum sigma
	// at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
	// add the points within the m_CutOffRatio to Mask

	m_Mask_3DIndex.FastResize(0);
	m_Mask_3DIndex.ReserveCapacity(3 * MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(MaxRadius*MaxRadius*MaxRadius);

	DenseMatrix<double> Index3D(3, 1);
	DenseMatrix<double> Index3D_Transpose(1, 3);
	DenseMatrix<double> SquaredRatio(1, 1);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (int_max z = -MaxRadius; z <= MaxRadius; ++z)
	{
		for (int_max y = -MaxRadius; y <= MaxRadius; ++y)
		{
			for (int_max x = -MaxRadius; x <= MaxRadius; ++x)
			{
				Index3D = { double(x), double(y), double(z) };
				Index3D_Transpose = { double(x), double(y), double(z) };
				SquaredRatio = Index3D_Transpose * InverseCovarianceMatrix * Index3D;
				auto tempRatio = SquaredRatio(0);

				if (tempRatio <= CutOffRatio_square)
				{
					auto tempValue = std::exp(-0.5*tempRatio);
					m_Mask_3DIndex.AppendCol(Index3D);
					m_ConvolutionCoefficient.Append(ScalarType(tempValue));
				}
			}
		}
	}

	m_Mask_3DIndex.ReleaseUnusedCapacity();
	m_ConvolutionCoefficient.ReleaseUnusedCapacity();

	//normalize coefficient
	m_ConvolutionCoefficient /= m_ConvolutionCoefficient.Sum();
}


}//end namespace mdk

#endif