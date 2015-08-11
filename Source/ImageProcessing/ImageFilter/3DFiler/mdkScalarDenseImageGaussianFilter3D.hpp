#ifndef mdk_ScalarDenseImageGaussianFilter3D_hpp
#define mdk_ScalarDenseImageGaussianFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageGaussianFilter3D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageGaussianFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
    this->ScalarDenseImageConvolutionFilter3D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
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
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::
SetGaussianParameter(const DenseVector<double, 3>& SigmaList, double CutOffRatio)
{
	m_SigmaList = SigmaList;
	m_CutOffRatio = CutOffRatio;
	m_RotationMatrix.Resize(3, 3);
	m_RotationMatrix.Fill(0);
	m_RotationMatrix.FillDiagonal(1);
	m_RotationMatrix.FixSize();
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

	if (m_RotationMatrix.GetColCount() != 3 || m_RotationMatrix.GetRowCount() != 3)
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
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DPhysicalPosition()
{
	auto Spacing = m_InputImage->GetSpacing();

	auto MaxRadius = m_SigmaList.Max() * m_CutOffRatio * 1.5;
	auto MaxRadius_x = double(int_max(MaxRadius / Spacing[0]) + 1)*Spacing[0];
	auto MaxRadius_y = double(int_max(MaxRadius / Spacing[1]) + 1)*Spacing[1];
	auto MaxRadius_z = double(int_max(MaxRadius / Spacing[2]) + 1)*Spacing[2];

	DenseMatrix<double> InverseCovarianceMatrix(3, 3);
    InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ 1.0 / (m_SigmaList(0)*m_SigmaList(0)),
										  1.0 / (m_SigmaList(1)*m_SigmaList(1)), 
										  1.0 / (m_SigmaList(2)*m_SigmaList(2)) });
    InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;

    // construct a grid of relative indexes according to the maximum sigma
    // at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
    // add the points within the m_CutOffRatio to Mask
    
	m_PointMask_3DPosition_InputImage.FastResize(0);
	m_PointMask_3DPosition_InputImage.ReserveCapacity(3 * 8 * MaxRadius_x*MaxRadius_y*MaxRadius_z);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(8*MaxRadius_x*MaxRadius_y*MaxRadius_z);

	DenseMatrix<double> Position(3, 1);
	DenseMatrix<double> Position_Transpose(1, 3);
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
					m_PointMask_3DPosition_InputImage.AppendCol(Position);
					m_ConvolutionCoefficient.Append(ScalarType(tempValue));
                }
            }
        }
    }

	m_PointMask_3DPosition_InputImage.ReleaseUnusedCapacity();
	m_ConvolutionCoefficient.ReleaseUnusedCapacity();

	//normalize coefficient
	m_ConvolutionCoefficient /= m_ConvolutionCoefficient.Sum();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DIndex()
{
	auto Spacing = m_InputImage->GetSpacing();

	DenseVector<double, 3> Sigma_xyz;
	Sigma_xyz[0] = m_SigmaList[0] / Spacing[0];
	Sigma_xyz[1] = m_SigmaList[1] / Spacing[1];
	Sigma_xyz[2] = m_SigmaList[2] / Spacing[2];
	auto MaxRadius = int_max(Sigma_xyz.Max() * m_CutOffRatio * 1.5) + 1;

	DenseMatrix<double> InverseCovarianceMatrix(3, 3);
	InverseCovarianceMatrix.Fill(0);
	InverseCovarianceMatrix.SetDiagonal({ 1.0 / (m_SigmaList[0] * m_SigmaList[0]),
								          1.0 / (m_SigmaList[1] * m_SigmaList[1]), 
										  1.0 / (m_SigmaList[2] * m_SigmaList[2]) });
	InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;

	// construct a grid of relative indexes according to the maximum sigma
	// at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
	// add the points within the m_CutOffRatio to Mask

	m_PointMask_3DIndex_InputImage.FastResize(0);
	m_PointMask_3DIndex_InputImage.ReserveCapacity(3*8*MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(8*MaxRadius*MaxRadius*MaxRadius);

	DenseMatrix<double> Position(3, 1);
	DenseMatrix<double> Position_Transpose(1, 3);
	DenseMatrix<double> SquaredRatio(1, 1);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (int_max zIndex = -MaxRadius; zIndex <= MaxRadius; ++zIndex)
	{
		auto z = double(zIndex)*Spacing[2];
		for (int_max yIndex = -MaxRadius; yIndex <= MaxRadius; ++yIndex)
		{
			auto y = double(yIndex)*Spacing[1];
			for (int_max xIndex = -MaxRadius; xIndex <= MaxRadius; ++xIndex)
			{
				auto x = double(xIndex)*Spacing[0];
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
					m_PointMask_3DIndex_InputImage.AppendCol({ xIndex, yIndex, zIndex });
					m_ConvolutionCoefficient.Append(ScalarType(tempValue));
				}
			}
		}
	}

	m_PointMask_3DIndex_InputImage.ReleaseUnusedCapacity();
	m_ConvolutionCoefficient.ReleaseUnusedCapacity();

	//normalize coefficient
	m_ConvolutionCoefficient /= m_ConvolutionCoefficient.Sum();
}


}//end namespace mdk

#endif