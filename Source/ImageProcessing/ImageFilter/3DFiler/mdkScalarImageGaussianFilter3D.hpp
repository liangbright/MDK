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
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::
SetMaskParameter(double Sx, double Sy, double Sz, const DenseMatrix<double>& RotationMatrix, double CutOffRatio)
{
    if (Sx <= 0.0 || Sy <= 0.0 || Sz <= 0.0)
    {
        MDK_Error("Sx <= 0.0 || Sy <= 0.0 || Sz <= 0.0 @ ScalarImageGaussianFilter3D::SetMaskParameter(...)")
        return;
    }

    if (RotationMatrix.GetColNumber() != 3 || RotationMatrix.GetRowNumber() != 3)
    {
        MDK_Error("RotationMatrix is invalid @ ScalarImageGaussianFilter3D::SetMaskParameter(...)")
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
    m_SigmaList(0) = Sx;
    m_SigmaList(1) = Sy;
    m_SigmaList(2) = Sz;

    m_RotationMatrix = RotationMatrix;    

    m_CutOffRatio = CutOffRatio;
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::BuildMaskOf3DIndex()
{
    m_Mask_3DIndex_Gaussian = this->BuildGaussianMask();

    this->SetMaskOf3DIndex(&m_Mask_3DIndex_Gaussian);
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::BuildMaskOf3DPosition()
{
    m_Mask_3DPosition_Gaussian = this->BuildGaussianMask();

    this->SetMaskOf3DPosition(&m_Mask_3DPosition_Gaussian);
}


template<typename InputPixelType, typename OutputPixelType>
DenseMatrix<double> ScalarImageGaussianFilter3D<InputPixelType, OutputPixelType>::BuildGaussianMask()
{
    DenseMatrix<double> InverseCovarianceMatrix(3, 3);

    InverseCovarianceMatrix.Fill(0);

    InverseCovarianceMatrix.SetDiagonal({ 1.0 / (m_SigmaList(0)*m_SigmaList(0)), 
                                          1.0 / (m_SigmaList(1)*m_SigmaList(1)), 
                                          1.0 / (m_SigmaList(2)*m_SigmaList(2))});

    InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;

    int_max Radius = 0;

    Radius = int_max(m_SigmaList.Max() * m_CutOffRatio * 1.5) + 1;

    // construct a grid of relative indexes according to the maximum sigma
    // at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
    // add the points within the m_CutOffRatio to Mask
    
    DenseMatrix<double> GaussianMask;
    GaussianMask.ReserveCapacity(Radius*Radius*Radius);

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

                    GaussianMask.AppendCol({ double(x), double(y), double(z), tempValue });
                }
            }
        }
    }

    GaussianMask.Squeeze();

    // ----------- normalize coefficient in Mask ---------

    auto tempRow = GaussianMask.GetRow(3);

    auto tempSum = tempRow.Sum();

    GaussianMask.RowOperationInPlace(3, '/', tempSum);

    //--------------------------------------------------

    return GaussianMask;
}



}//end namespace mdk

#endif