#ifndef __mdkIntegralImage3DBuilder_hpp
#define __mdkIntegralImage3DBuilder_hpp


namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output>
IntegralImage3DBuilder<PixelType_Input, PixelType_Output>::IntegralImage3DBuilder()
{

}


template<typename PixelType_Input, typename PixelType_Output>
IntegralImage3DBuilder<PixelType_Input, PixelType_Output>::~IntegralImage3DBuilder()
{
}


template<typename PixelType_Input, typename PixelType_Output>
void IntegralImage3DBuilder<PixelType_Input, PixelType_Output>::Clear()
{
    this->Image3DFilter::Clear();

    m_Flag_OutputImage = true;
}


template<typename PixelType_Input, typename PixelType_Output>
bool IntegralImage3DBuilder<PixelType_Input, PixelType_Output>::CheckInput()
{
    if (this->Image3DFilter::CheckInput() == false)
    {
        return false;
    }

    if (m_Flag_OutputImage == false)
    {
        MDK_Error("OutputImage is invalid @ IntegralImage3DBuilder::CheckInput()")
        return false;
    }

    auto InputSize = m_InputImage->GetSize();

    auto OutputSize = m_OutputImage->GetSize();

    if (InputSize.Lx != OutputSize.Lx || InputSize.Ly != OutputSize.Ly || InputSize.Lz != OutputSize.Lz)
    {
        MDK_Error("Size does not match @ IntegralImage3DBuilder::CheckInput()")
        return false;
    }

    if (InputSize.Lz == 0)
    {
        return false;
    }

    return true;
}


template<typename PixelType_Input, typename PixelType_Output>
inline
void IntegralImage3DBuilder<PixelType_Input, PixelType_Output>::
Compute2DIntegralImage(int_max z_Index_start, int_max z_Index_end)
{
    if (z_Index_end < z_Index_start || z_Index_start < 0)
    {
        MDK_Error("Invalid input @ IntegralImage3DBuilder::Compute2DIntegralImage")
        return;
    }

    auto InputSize = m_InputImage->GetSize();

    for (int_max z = z_Index_start; z <= z_Index_end; ++z)
    {
        for (int_max y = 0; y < InputSize.Ly; ++y)
        {
            auto tempOutoutPixel = m_ZeroPixelOfOutputImage;

            for (int_max x = 0; x < InputSize.Lx; ++x)
            {
                tempOutoutPixel += PixelType_Output((*m_InputImage)(x, y, z));

                (*m_OutputImage)(x, y, z) = tempOutoutPixel;
            }
        }

        for (int_max x = 0; x < InputSize.Lx; ++x)
        {
            auto tempOutoutPixel = m_ZeroPixelOfOutputImage;

            for (int_max y = 0; y < InputSize.Ly; ++y)
            {
                tempOutoutPixel += (*m_OutputImage)(x, y, z);

                (*m_OutputImage)(x, y, z) = tempOutoutPixel;
            }
        }
    }
}


template<typename PixelType_Input, typename PixelType_Output>
void IntegralImage3DBuilder<PixelType_Input, PixelType_Output>::
ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end)
{
    if (xy_LinearIndex_end < xy_LinearIndex_start || xy_LinearIndex_start < 0)
    {
        MDK_Error("Invalid input @ IntegralImage3DBuilder::ComputeSumInZDirection")
        return;
    }

    auto InputSize = m_InputImage->GetSize();

    for (int_max k = xy_LinearIndex_start; k <= xy_LinearIndex_end; ++k)
    {
        int_max y = k / InputSize.Lx;

        int_max x = k % InputSize.Lx;

        auto tempOutoutPixel = m_ZeroPixelOfOutputImage;

        for (int_max z = 0; z < InputSize.Lz; ++z)
        {
            tempOutoutPixel += (*m_OutputImage)(x, y, z);

            (*m_OutputImage)(x, y, z) = tempOutoutPixel;
        }                
    }
}


template<typename PixelType_Input, typename PixelType_Output>
bool IntegralImage3DBuilder<PixelType_Input, PixelType_Output>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    // compute each 2D IntegralImage -------------------------------------------------------------------------------------
   
    auto InputSize = m_InputImage->GetSize();

    ParallelBlock([&](int_max z_Index_start, int_max z_Index_end){this->Compute2DIntegralImage(z_Index_start, z_Index_end); },
                  0, InputSize.Lz - 1, m_MaxThreadNumber, 1);

    // sum in z-direction ------------------------------------------------------------------------------------------------------------

    int_max MinNumberOfPositionsPerThread = 100;

    ParallelBlock([&](int_max xy_Linear_start, int_max xy_Linear_end){this->ComputeSumInZDirection(xy_Linear_start, xy_Linear_end); },
                  0, InputSize.Lx*InputSize.Ly - 1, m_MaxThreadNumber, MinNumberOfPositionsPerThread);

    return true;
}

}//end namespace mdk

#endif