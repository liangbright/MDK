#ifndef mdk_IntegralImageBuilder3D_hpp
#define mdk_IntegralImageBuilder3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
IntegralImageBuilder3D<InputPixelType, OutputPixelType>::IntegralImageBuilder3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType>
IntegralImageBuilder3D<InputPixelType, OutputPixelType>::~IntegralImageBuilder3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBuilder3D<InputPixelType, OutputPixelType>::Clear()
{
	m_InputImage = nullptr;
	m_OutputImage.Clear();
	m_MaxNumberOfThread = 1;
}

template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBuilder3D<InputPixelType, OutputPixelType>::SetInputImage(const DenseImage3D<InputPixelType>* InputImage)
{
	m_InputImage = InputImage;
}

template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBuilder3D<InputPixelType, OutputPixelType>::SetMaxNumberOfThread(int_max Number)
{
	m_MaxNumberOfThread = Number;
}

template<typename InputPixelType, typename OutputPixelType>
bool IntegralImageBuilder3D<InputPixelType, OutputPixelType>::CheckInput()
{
	if (m_InputImage == nullptr)
    {
        MDK_Error("m_InputImage is nullptr @ IntegralImageBuilder3D::CheckInput()")
        return false;
    }

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("m_InputImage is empty @ IntegralImageBuilder3D::CheckInput()")
		return false;
	}

	if (m_MaxNumberOfThread <= 0)
	{
		MDK_Error("m_MaxNumberOfThread <= 0 @ IntegralImageBuilder3D::CheckInput()")
		return false;
	}

	bool Flag_Init_OutputImage = false;
	if (m_OutputImage.IsEmpty() == true)
	{
		Flag_Init_OutputImage = true;
	}
	else
	{
		auto InputInfo = m_InputImage->GetInfo();
		auto OutputInfo = m_OutputImage.GetInfo();
		if (InputInfo.Size[0] != OutputInfo.Size[0] || InputInfo.Size[1] != OutputInfo.Size[1] || InputInfo.Size[2] != OutputInfo.Size[2]
			|| InputInfo.Origin[0] != OutputInfo.Origin[0] || InputInfo.Origin[1] != OutputInfo.Origin[1] || InputInfo.Origin[2] != OutputInfo.Origin[2]
			|| InputInfo.Spacing[0] != OutputInfo.Spacing[0] || InputInfo.Spacing[1] != OutputInfo.Spacing[1] || InputInfo.Spacing[2] != OutputInfo.Spacing[2])
		{
			Flag_Init_OutputImage = true;
		}
	}

	if (Flag_Init_OutputImage == true)
	{
		m_OutputImage.SetOrigin(m_InputImage->GetOrigin());
		m_OutputImage.SetSpacing(m_InputImage->GetSpacing());
		m_OutputImage.SetOrientation(m_InputImage->GetOrientation());
		m_OutputImage.SetSize(m_InputImage->GetSize());
	}

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void IntegralImageBuilder3D<InputPixelType, OutputPixelType>::
ComputeIntegralImage2D(int_max z_Index_start, int_max z_Index_end)
{
    if (z_Index_end < z_Index_start || z_Index_start < 0)
    {
        MDK_Error("Invalid input @ IntegralImageBuilder3D::ComputeIntegralImage2D")
        return;
    }

    auto InputSize = m_InputImage->GetSize();

    for (int_max z = z_Index_start; z <= z_Index_end; ++z)
    {
        for (int_max y = 0; y < InputSize[1]; ++y)
        {
			auto tempOutputPixel = OutputPixelType(0);

            for (int_max x = 0; x < InputSize[0]; ++x)
            {
				tempOutputPixel += OutputPixelType((*m_InputImage)(x, y, z));

				m_OutputImage(x, y, z) = tempOutputPixel;
            }
        }

        for (int_max x = 0; x < InputSize[0]; ++x)
        {
			auto tempOutputPixel = OutputPixelType(0);

            for (int_max y = 0; y < InputSize[1]; ++y)
            {
				tempOutputPixel += m_OutputImage(x, y, z);

				m_OutputImage(x, y, z) = tempOutputPixel;
            }
        }
    }
}


template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBuilder3D<InputPixelType, OutputPixelType>::
ComputeSumInDirectionZ(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end)
{
    if (xy_LinearIndex_end < xy_LinearIndex_start || xy_LinearIndex_start < 0)
    {
        MDK_Error("Invalid input @ IntegralImageBuilder3D::ComputeSumInDirectionZ")
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


template<typename InputPixelType, typename OutputPixelType>
bool IntegralImageBuilder3D<InputPixelType, OutputPixelType>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    // compute each 2D ScalarIntegralDenseImage -------------------------------------------------------------------------------------
   
    auto InputSize = m_InputImage->GetSize();

	ParallelBlock([&](int_max z_Index_start, int_max z_Index_end, int_max ThreadIndex){this->ComputeIntegralImage2D(z_Index_start, z_Index_end); },
                  0, InputSize[2] - 1, m_MaxNumberOfThread, 1);

    // sum in z-direction ------------------------------------------------------------------------------------------------------------

    int_max MinNumberOfPositionPerThread = 128;

	ParallelBlock([&](int_max xy_Linear_start, int_max xy_Linear_end, int_max ThreadIndex){this->ComputeSumInDirectionZ(xy_Linear_start, xy_Linear_end); },
				   0, InputSize[0]*InputSize[1] - 1, m_MaxNumberOfThread, MinNumberOfPositionPerThread);

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
DenseImage3D<InputPixelType>* IntegralImageBuilder3D<InputPixelType, OutputPixelType>::GetOutputImage()
{
	return &m_OutputImage;
}

}//end namespace mdk

#endif