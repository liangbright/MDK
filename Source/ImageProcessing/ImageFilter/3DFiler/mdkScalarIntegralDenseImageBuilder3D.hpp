#ifndef __mdkScalarIntegralDenseImageBuilder3D_hpp
#define __mdkScalarIntegralDenseImageBuilder3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType, ScalarType>::ScalarIntegralDenseImageBuilder3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarIntegralDenseImageBuilder3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter3D::Clear();

	m_Flag_EnableOutputImage = true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->ImageFilter3D::CheckInput() == false)
    {
        return false;
    }

    if (m_Flag_EnableOutputImage == false)
    {
        MDK_Error("m_Flag_EnableOutputImage is false @ ScalarIntegralDenseImageBuilder3D::CheckInput()")
        return false;
    }

    return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
void ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType, ScalarType>::
Compute2DScalarIntegralDenseImage(int_max z_Index_start, int_max z_Index_end)
{
    if (z_Index_end < z_Index_start || z_Index_start < 0)
    {
        MDK_Error("Invalid input @ ScalarIntegralDenseImageBuilder3D::Compute2DScalarIntegralDenseImage")
        return;
    }

    auto InputSize = m_InputImage->GetSize();

    for (int_max z = z_Index_start; z <= z_Index_end; ++z)
    {
        for (int_max y = 0; y < InputSize[1]; ++y)
        {
			auto tempOutoutPixel = OutputPixelType(0);

            for (int_max x = 0; x < InputSize[0]; ++x)
            {
                tempOutoutPixel += OutputPixelType((*m_InputImage)(x, y, z));

                m_OutputImage(x, y, z) = tempOutoutPixel;
            }
        }

        for (int_max x = 0; x < InputSize[0]; ++x)
        {
			auto tempOutoutPixel = OutputPixelType(0);

            for (int_max y = 0; y < InputSize[1]; ++y)
            {
                tempOutoutPixel += m_OutputImage(x, y, z);

				m_OutputImage(x, y, z) = tempOutoutPixel;
            }
        }
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType, ScalarType>::
ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end)
{
    if (xy_LinearIndex_end < xy_LinearIndex_start || xy_LinearIndex_start < 0)
    {
        MDK_Error("Invalid input @ ScalarIntegralDenseImageBuilder3D::ComputeSumInZDirection")
        return;
    }

    auto InputSize = m_InputImage->GetSize();

    for (int_max k = xy_LinearIndex_start; k <= xy_LinearIndex_end; ++k)
    {
        int_max y = k / InputSize[0];

        int_max x = k % InputSize[0];

		auto tempOutoutPixel = OutputPixelType(0);

        for (int_max z = 0; z < InputSize[2]; ++z)
        {
            tempOutoutPixel += m_OutputImage(x, y, z);

			m_OutputImage(x, y, z) = tempOutoutPixel;
        }                
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarIntegralDenseImageBuilder3D<InputPixelType, OutputPixelType, ScalarType>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    // compute each 2D ScalarIntegralDenseImage -------------------------------------------------------------------------------------
   
    auto InputSize = m_InputImage->GetSize();

    ParallelBlock([&](int_max z_Index_start, int_max z_Index_end, int_max){this->Compute2DScalarIntegralDenseImage(z_Index_start, z_Index_end); },
                  0, InputSize[2] - 1, m_MaxNumberOfThread, 1);

    // sum in z-direction ------------------------------------------------------------------------------------------------------------

    int_max MinNumberOfPositionPerThread = 100;

    ParallelBlock([&](int_max xy_Linear_start, int_max xy_Linear_end, int_max){this->ComputeSumInZDirection(xy_Linear_start, xy_Linear_end); },
				   0, InputSize[0]*InputSize[1] - 1, m_MaxNumberOfThread, MinNumberOfPositionPerThread);

    return true;
}

}//end namespace mdk

#endif