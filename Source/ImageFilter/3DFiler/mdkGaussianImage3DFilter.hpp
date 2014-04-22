#ifndef __mdkGaussianImage3DFilter_hpp
#define __mdkGaussianImage3DFilter_hpp


namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output>
GaussianImage3DFilter<PixelType_Input, PixelType_Output>::GaussianImage3DFilter()
{
    m_CutOffRatio = 3; // default: within 3 * std
}


template<typename PixelType_Input, typename PixelType_Output>
GaussianImage3DFilter<PixelType_Input, PixelType_Output>::~GaussianImage3DFilter()
{
}


template<typename PixelType_Input, typename PixelType_Output>
void GaussianImage3DFilter<PixelType_Input, PixelType_Output>::SetSigmaList(double Sx, double Sy, double Sz)
{
    m_SigmaList.Clear();

    m_SigmaList.Resize(3, 1);

    m_SigmaList.FixSize();

    m_SigmaList(0) = Sx;
    m_SigmaList(1) = Sy;
    m_SigmaList(2) = Sz;
}


template<typename PixelType_Input, typename PixelType_Output>
void GaussianImage3DFilter<PixelType_Input, PixelType_Output>::SetRotationMatrix(const DenseMatrix<double>& RotationMatrix)
{
    if (RotationMatrix.GetColNumber() != 3 || RotationMatrix.GetRowNumber() != 3)
    {
        MDK_Error("RotationMatrix is invalid @ GaussianImage3DFilter::SetRotationMatrix")
        return;
    }

    m_RotationMatrix = RotationMatrix;
}


template<typename PixelType_Input, typename PixelType_Output>
void GaussianImage3DFilter<PixelType_Input, PixelType_Output>::SetCutOffRatio(double CutOffRatio)
{
    if (CutOffRatio < 0.0)
    {
        MDK_Error("CutOffRatio < 0.0 @ GaussianImage3DFilter::SetCutOffRatio")
        return;
    }

    m_CutOffRatio = CutOffRatio;
}


template<typename PixelType_Input, typename PixelType_Output>
void GaussianImage3DFilter<PixelType_Input, PixelType_Output>::BuildMaskOf3DIndex()
{
    // check to see if the Mask has been loaded from somewhere else
    if (this->IsMaskOf3DIndexEmpty() == false)
    {
        return;
    }

    if (m_SigmaList.IsEmpty() == true)
    {
        MDK_Error("m_SigmaList is empty @ GaussianImage3DFilter::BuildMaskOf3DIndex")
        return;
    }

    DenseMatrix<double> InverseCovarianceMatrix(3, 3);

    InverseCovarianceMatrix.Fill(0);

    InverseCovarianceMatrix.SetDiangonal({ 1.0 / (m_SigmaList(0)*m_SigmaList(0)), 
                                           1.0 / (m_SigmaList(1)*m_SigmaList(1)), 
                                           1.0 / (m_SigmaList(2)*m_SigmaList(2))});

    if (m_RotationMatrix.IsEmpty() == false)
    {
        if (m_RotationMatrix.GetColNumber() != 3 || m_RotationMatrix.GetRowNumber() != 3)
        {
            MDK_Error("m_RotationMatrix is invalid @ GaussianImage3DFilter::BuildMaskOf3DIndex")
            return;
        }

        InverseCovarianceMatrix = m_RotationMatrix.Transpose() * InverseCovarianceMatrix * m_RotationMatrix;
    }

    int_max OffSet = 0;

    OffSet = int_max(m_SigmaList.Max() * m_CutOffRatio * 1.5) + 1;

    // construct a grid of relative indexes according to the maximum sigma
    // at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
    // add the points within the m_CutOffRatio to Mask
    
    DenseMatrix<double> Mask;

    DenseMatrix<double> Relative3DIndex(3, 1);
    DenseMatrix<double> Relative3DIndex_Transpose(1, 3);

    DenseMatrix<double> SquaredRatio(1, 1);

    auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

    for (int_max z = -OffSet; z <= OffSet; ++z)
    {
        for (int_max y = -OffSet; y <= OffSet; ++y)
        {
            for (int_max x = -OffSet; x <= OffSet; ++x)
            {
                Relative3DIndex = { double(x), double(y), double(z) };

                Relative3DIndex_Transpose = { double(x), double(y), double(z) };
    
                SquaredRatio = Relative3DIndex_Transpose * InverseCovarianceMatrix * Relative3DIndex;

                auto tempRatio = SquaredRatio(0);

                if (tempRatio < CutOffRatio_square)
                {
                    auto tempValue = std::exp(-0.5*tempRatio);

                    Mask.AppendCol({double(x), double(y), double(z), tempValue});
                }
            }
        }
    }

    // ----------- normalize coefficient in Mask ---------

    auto tempRow = Mask.GetRow(3);

    auto tempSum = tempRow.Sum();

    Mask.Row(3) /= tempSum;

    //---------------------------------

    this->SetMaskOf3DIndex(Mask);
}


}//end namespace mdk

#endif