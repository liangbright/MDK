#ifndef __mdkScalarIntegralDenseImageBuilder3D_hpp
#define __mdkScalarIntegralDenseImageBuilder3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType>::ScalarIntegralDenseImageBuilder3D()
{

}


template<typename InputPixelType, typename OutputPixelType>
ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType>::~ScalarIntegralDenseImageBuilder3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType>::Clear()
{
    this->DenseImageFilter3D::Clear();

    m_Flag_OutputDenseImage = true;
}


template<typename InputPixelType, typename OutputPixelType>
bool ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType>::CheckInput()
{
    if (this->ScalarDenseImageFilter3D::CheckInput() == false)
    {
        return false;
    }

    if (m_Flag_OutputDenseImage == false)
    {
        MDK_Error("OutputDenseImage is invalid @ ScalarIntegralDenseImageBuilder3D::CheckInput()")
        return false;
    }

    auto InputSize = m_InputDenseImage->GetSize();

    auto OutputSize = m_OutputDenseImage->GetSize();

    if (InputSize.Lx != OutputSize.Lx || InputSize.Ly != OutputSize.Ly || InputSize.Lz != OutputSize.Lz)
    {
        MDK_Error("Size does not match @ ScalarIntegralDenseImageBuilder3D::CheckInput()")
        return false;
    }

    if (InputSize.Lz == 0)
    {
        return false;
    }

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType>::
Compute2DScalarIntegralDenseImage(int_max z_Index_start, int_max z_Index_end)
{
    if (z_Index_end < z_Index_start || z_Index_start < 0)
    {
        MDK_Error("Invalid input @ ScalarIntegralDenseImageBuilder3D::Compute2DScalarIntegralDenseImage")
        return;
    }

    auto InputSize = m_InputDenseImage->GetSize();

    for (int_max z = z_Index_start; z <= z_Index_end; ++z)
    {
        for (int_max y = 0; y < InputSize.Ly; ++y)
        {
            auto tempOutoutPixel = m_ZeroPixelOfOutputDenseImage;

            for (int_max x = 0; x < InputSize.Lx; ++x)
            {
                tempOutoutPixel += OutputPixelType((*m_InputDenseImage)(x, y, z));

                (*m_OutputDenseImage)(x, y, z) = tempOutoutPixel;
            }
        }

        for (int_max x = 0; x < InputSize.Lx; ++x)
        {
            auto tempOutoutPixel = m_ZeroPixelOfOutputDenseImage;

            for (int_max y = 0; y < InputSize.Ly; ++y)
            {
                tempOutoutPixel += (*m_OutputDenseImage)(x, y, z);

                (*m_OutputDenseImage)(x, y, z) = tempOutoutPixel;
            }
        }
    }
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType>::
ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end)
{
    if (xy_LinearIndex_end < xy_LinearIndex_start || xy_LinearIndex_start < 0)
    {
        MDK_Error("Invalid input @ ScalarIntegralDenseImageBuilder3D::ComputeSumInZDirection")
        return;
    }

    auto InputSize = m_InputDenseImage->GetSize();

    for (int_max k = xy_LinearIndex_start; k <= xy_LinearIndex_end; ++k)
    {
        int_max y = k / InputSize.Lx;

        int_max x = k % InputSize.Lx;

        auto tempOutoutPixel = m_ZeroPixelOfOutputDenseImage;

        for (int_max z = 0; z < InputSize.Lz; ++z)
        {
            tempOutoutPixel += (*m_OutputDenseImage)(x, y, z);

            (*m_OutputDenseImage)(x, y, z) = tempOutoutPixel;
        }                
    }
}


template<typename InputPixelType, typename OutputPixelType>
bool ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    // compute each 2D ScalarIntegralDenseImage -------------------------------------------------------------------------------------
   
    auto InputSize = m_InputDenseImage->GetSize();

    ParallelBlock([&](int_max z_Index_start, int_max z_Index_end, int_max){this->Compute2DScalarIntegralDenseImage(z_Index_start, z_Index_end); },
                  0, InputSize.Lz - 1, m_MaxNumberOfThreads, 1);

    // sum in z-direction ------------------------------------------------------------------------------------------------------------

    int_max MinNumberOfPositionsPerThread = 100;

    ParallelBlock([&](int_max xy_Linear_start, int_max xy_Linear_end, int_max){this->ComputeSumInZDirection(xy_Linear_start, xy_Linear_end); },
                  0, InputSize.Lx*InputSize.Ly - 1, m_MaxNumberOfThreads, MinNumberOfPositionsPerThread);

    return true;
}

}//end namespace mdk

#endif