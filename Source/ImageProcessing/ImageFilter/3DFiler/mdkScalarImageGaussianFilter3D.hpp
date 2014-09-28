#ifndef __mdkScalarImageGaussianFilter3D_hpp
#define __mdkScalarImageGaussianFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::ScalarImageGaussianFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType>
ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::~ScalarImageGaussianFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::Clear()
{
    this->ScalarImageConvolutionFilter3D::Clear();

    m_SigmaList.Clear();

    m_RotationMatrix.Clear();

    m_CutOffRatio = 0;

    m_Mask_3DIndex_Gaussian.Clear();
    m_Mask_3DPosition_Gaussian.Clear();
	m_ConvolutionCoefficient_Gaussian.Clear();
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::
SetMaskParameter(double Sigma_x, double Sigma_y, double Sigma_z, const DenseMatrix<double>& RotationMatrix, double CutOffRatio)
{
	if (Sigma_x <= 0.0 || Sigma_y <= 0.0 || Sigma_z <= 0.0)
    {
        MDK_Error("Sigma_x <= 0.0 || Sigma_y <= 0.0 || Sigma_z <= 0.0 @ ScalarImageGaussianFilter3D::SetMaskParameter(...)")
        return;
    }

    if (RotationMatrix.GetColNumber() != 3 || RotationMatrix.GetRowNumber() != 3)
    {
        MDK_Error("RotationMatrix is invalid @ ScalarImageGaussianFilter3D::SetMaskParameter(...)")
        return;
    }

	if (RotationMatrix.GetRowNumber() != 3 || RotationMatrix.GetColNumber() != 3)
	{
		MDK_Error("RotationMatrix is not 3 x 3 @ ScalarImageGaussianFilter3D::SetMaskParameter(...)")
		return;
	}

	if (CutOffRatio <= 0.0)
	{
		MDK_Error("CutOffRatio <= 0.0 @ ScalarImageGaussianFilter3D::SetMaskParameter(...)")
		return;
	}

    m_SigmaList.Clear();
    m_SigmaList.Resize(3, 1);
    m_SigmaList.FixSize();
	m_SigmaList(0) = Sigma_x;
	m_SigmaList(1) = Sigma_y;
	m_SigmaList(2) = Sigma_z;

    m_RotationMatrix = RotationMatrix;    

    m_CutOffRatio = CutOffRatio;
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::BuildMaskOf3DIndex()
{
    DenseMatrix<double> InverseCovarianceMatrix(3, 3);
    InverseCovarianceMatrix.Fill(0);
    InverseCovarianceMatrix.SetDiagonal({ 1.0 / (m_SigmaList(0)*m_SigmaList(0)), 
                                          1.0 / (m_SigmaList(1)*m_SigmaList(1)), 
                                          1.0 / (m_SigmaList(2)*m_SigmaList(2))});

    InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;

    auto Radius = int_max(m_SigmaList.Max() * m_CutOffRatio * 1.5) + 1;

    // construct a grid of relative indexes according to the maximum sigma
    // at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
    // add the points within the m_CutOffRatio to Mask
    
	m_Mask_3DIndex_Gaussian.FastResize(0);
	m_Mask_3DIndex_Gaussian.ReserveCapacity(3*Radius*Radius*Radius);

	m_ConvolutionCoefficient_Gaussian.FastResize(0);
	m_ConvolutionCoefficient_Gaussian.ReserveCapacity(Radius*Radius*Radius);

    DenseMatrix<double> Relative3DIndex(3, 1);
    DenseMatrix<double> Relative3DIndex_Transpose(1, 3);

    DenseMatrix<double> SquaredRatio(1, 1);

    auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

    for (int_max z = -Radius; z <= Radius; ++z)
    {
        for (int_max y = -Radius; y <= Radius; ++y)
        {
            for (int_max x = -Radius; x <= Radius; ++x)
            {
                Relative3DIndex = { double(x), double(y), double(z) };

                Relative3DIndex_Transpose = { double(x), double(y), double(z) };
    
                SquaredRatio = Relative3DIndex_Transpose * InverseCovarianceMatrix * Relative3DIndex;

                auto tempRatio = SquaredRatio(0);

                if (tempRatio < CutOffRatio_square)
                {
                    auto tempValue = std::exp(-0.5*tempRatio);

					m_Mask_3DIndex_Gaussian.AppendCol({ double(x), double(y), double(z)});
					m_ConvolutionCoefficient_Gaussian.Append(tempValue);
                }
            }
        }
    }

	m_Mask_3DIndex_Gaussian.ReleaseUnusedCapacity();
	m_ConvolutionCoefficient_Gaussian.ReleaseUnusedCapacity();

    // ----------- normalize coefficient in Mask ---------
	auto tempRow = m_Mask_3DIndex_Gaussian.GetRow(3);
    auto tempSum = tempRow.Sum();
	m_Mask_3DIndex_Gaussian.RowOperationInPlace(3, '/', tempSum);

    //--------------------------------------------------
	this->SetMaskOf3DIndex(&m_Mask_3DIndex_Gaussian);
	this->SetConvolutionCoefficient(&m_ConvolutionCoefficient_Gaussian);
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::BuildMaskOf3DPosition()
{
	auto InputImageSpacing = m_InputImage->GetSpacing();
	auto InputImageOrigin = m_InputImage->GetOrigin();

	if (m_Mask_3DIndex_Gaussian.IsEmpty() == true)
	{
		auto InputSigmaList = m_SigmaList;
		m_SigmaList[0] /= InputImageSpacing.Sx;
		m_SigmaList[1] /= InputImageSpacing.Sy;
		m_SigmaList[2] /= InputImageSpacing.Sz;
		this->BuildMaskOf3DIndex();
		m_SigmaList = InputSigmaList;
	}

	m_Mask_3DPosition_Gaussian.FastResize(3, m_Mask_3DIndex_Gaussian.GetColNumber());
	for (int_max k = 0; k < m_Mask_3DIndex_Gaussian.GetColNumber(); ++k)
	{
		auto x_Index = m_Mask_3DIndex_Gaussian(0, k);
		auto y_Index = m_Mask_3DIndex_Gaussian(1, k);
		auto z_Index = m_Mask_3DIndex_Gaussian(2, k);

		auto x = InputImageOrigin.x + InputImageSpacing.Sx * x_Index;
		auto y = InputImageOrigin.y + InputImageSpacing.Sy * y_Index;
		auto z = InputImageOrigin.z + InputImageSpacing.Sz * z_Index;

		m_Mask_3DPosition_Gaussian(0, k) = x;
		m_Mask_3DPosition_Gaussian(1, k) = y;
		m_Mask_3DPosition_Gaussian(2, k) = z;
	}
	//--------------------------------------------------
	this->SetMaskOf3DPosition(&m_Mask_3DPosition_Gaussian);
}


}//end namespace mdk

#endif