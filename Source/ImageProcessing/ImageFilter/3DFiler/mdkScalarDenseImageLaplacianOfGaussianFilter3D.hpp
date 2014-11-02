#ifndef __mdkScalarDenseImageLaplacianOfGaussianFilter3D_hpp
#define __mdkScalarDenseImageLaplacianOfGaussianFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageLaplacianOfGaussianFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageLaplacianOfGaussianFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
    this->ScalarDenseImageConvolutionFilter3D::Clear();
    m_SigmaList.Clear();
    m_RotationMatrix.Clear();
    m_CutOffRatio = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::
SetGaussianParameter(const DenseVector<ScalarType, 3>& SigmaList, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio)
{    
	m_SigmaList = SigmaList;
    m_RotationMatrix = RotationMatrix;    
    m_CutOffRatio = CutOffRatio;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->ScalarDenseImageConvolutionFilter3D::CheckInput() == false)
	{
		return false;
	}

	if (m_SigmaList[0] <= 0.0 || m_SigmaList[1] <= 0.0 || m_SigmaList[2] <= 0.0)
	{
		MDK_Error("Sigma_x <= 0.0 || Sigma_y <= 0.0 || Sigma_z <= 0.0 @ ScalarDenseImageLaplacianOfGaussianFilter3D::CheckInput(...)")
		return false;
	}

	if (m_RotationMatrix.GetColNumber() != 3 || m_RotationMatrix.GetRowNumber() != 3)
	{
		MDK_Error("RotationMatrix is invalid @ ScalarDenseImageLaplacianOfGaussianFilter3D::CheckInput(...)")
		return false;
	}

	if (m_CutOffRatio <= 0.0)
	{
		MDK_Error("CutOffRatio <= 0.0 @ ScalarDenseImageLaplacianOfGaussianFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	this->SelectMaskOf3DIndex();
	if (this->ScalarDenseImageConvolutionFilter3D::Preprocess() == false)
	{
		return false;
	}
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DPhysicalPosition()
{
    DenseMatrix<ScalarType> InverseCovarianceMatrix(3, 3);
    InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ ScalarType(1) / (m_SigmaList(0)*m_SigmaList(0)),
										  ScalarType(1) / (m_SigmaList(1)*m_SigmaList(1)), 
                                          ScalarType(1) / (m_SigmaList(2)*m_SigmaList(2))});

    InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;

	auto MaxRadius = int_max(m_SigmaList.Max() * m_CutOffRatio * ScalarType(1.5)) + 1;

    // construct a grid of relative indexes according to the maximum sigma
    // at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
    // add the points within the m_CutOffRatio to Mask
    
	m_Mask_3DPhysicalPosition.FastResize(0);
	m_Mask_3DPhysicalPosition.ReserveCapacity(3 * MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(MaxRadius*MaxRadius*MaxRadius);

	DenseMatrix<ScalarType> Position(3, 1);
	DenseMatrix<ScalarType> Position_Transpose(1, 3);

    DenseMatrix<ScalarType> SquaredRatio(1, 1);

    auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (int_max z = -MaxRadius; z <= MaxRadius; ++z)
    {
		for (int_max y = -MaxRadius; y <= MaxRadius; ++y)
        {
			for (int_max x = -MaxRadius; x <= MaxRadius; ++x)
            {
                Position = { ScalarType(x), ScalarType(y), ScalarType(z) };

				Position_Transpose = { ScalarType(x), ScalarType(y), ScalarType(z) };
    
				SquaredRatio = Position_Transpose * InverseCovarianceMatrix * Position;

                auto tempRatio = SquaredRatio(0);

                if (tempRatio < CutOffRatio_square)
                {
					auto tempValue = std::exp(-ScalarType(0.5)*tempRatio);

					m_Mask_3DPhysicalPosition.AppendCol({ ScalarType(x), ScalarType(y), ScalarType(z) });
					m_ConvolutionCoefficient.Append(tempValue);
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
void ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DIndex()
{
	auto InputImageSpacing = m_InputImage->GetSpacing();
	auto InputImageOrigin = m_InputImage->GetOrigin();

	DenseVector<ScalarType, 3> Sigma_xyz;
	Sigma_xyz[0] = m_SigmaList[0] / InputImageSpacing[0];
	Sigma_xyz[1] = m_SigmaList[1] / InputImageSpacing[1];
	Sigma_xyz[2] = m_SigmaList[2] / InputImageSpacing[2];

	DenseMatrix<ScalarType> InverseCovarianceMatrix(3, 3);
	InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ ScalarType(1) / (Sigma_xyz[0] * Sigma_xyz[0]),
								          ScalarType(1) / (Sigma_xyz[1] * Sigma_xyz[1]), 
										  ScalarType(1) / (Sigma_xyz[2] * Sigma_xyz[2])});

	InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;

	auto MaxRadius = int_max(Sigma_xyz.Max() * m_CutOffRatio * ScalarType(1.5)) + 1;

	// construct a grid of relative indexes according to the maximum sigma
	// at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
	// add the points within the m_CutOffRatio to Mask

	m_Mask_3DIndex.FastResize(0);
	m_Mask_3DIndex.ReserveCapacity(3 * MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(MaxRadius*MaxRadius*MaxRadius);

	DenseMatrix<ScalarType> Index3D(3, 1);
	DenseMatrix<ScalarType> Index3D_Transpose(1, 3);

	DenseMatrix<ScalarType> SquaredRatio(1, 1);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (int_max z = -MaxRadius; z <= MaxRadius; ++z)
	{
		for (int_max y = -MaxRadius; y <= MaxRadius; ++y)
		{
			for (int_max x = -MaxRadius; x <= MaxRadius; ++x)
			{
				Index3D = { ScalarType(x), ScalarType(y), ScalarType(z) };

				Index3D_Transpose = { ScalarType(x), ScalarType(y), ScalarType(z) };

				SquaredRatio = Index3D_Transpose * InverseCovarianceMatrix * Index3D;

				auto tempRatio = SquaredRatio(0);

				if (tempRatio < CutOffRatio_square)
				{
					auto tempValue = std::exp(-ScalarType(0.5)*tempRatio);

					m_Mask_3DIndex.AppendCol({ ScalarType(x), ScalarType(y), ScalarType(z) });
					m_ConvolutionCoefficient.Append(tempValue);
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