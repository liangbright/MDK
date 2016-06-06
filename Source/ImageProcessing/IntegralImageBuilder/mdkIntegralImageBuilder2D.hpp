#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
IntegralImageBuilder2D<InputPixelType, OutputPixelType>::IntegralImageBuilder2D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType>
IntegralImageBuilder2D<InputPixelType, OutputPixelType>::~IntegralImageBuilder2D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBuilder2D<InputPixelType, OutputPixelType>::Clear()
{
	m_InputImage = nullptr;
	m_OutputImage.Clear();
}


template<typename InputPixelType, typename OutputPixelType>
bool IntegralImageBuilder2D<InputPixelType, OutputPixelType>::CheckInput()
{
	if (m_InputImage == nullptr)
    {
        MDK_Error("m_InputImage is nullptr @ IntegralImageBuilder2D::CheckInput()")
        return false;
    }

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("m_InputImage is empty @ IntegralImageBuilder2D::CheckInput()")
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
		if (InputInfo.Size[0] != OutputInfo.Size[0] || InputInfo.Size[1] != OutputInfo.Size[1]
			|| InputInfo.Spacing[0] != OutputInfo.Spacing[0] || InputInfo.Spacing[1] != OutputInfo.Spacing[1]
			|| InputInfo.Origin[0] != OutputInfo.Origin[0] || InputInfo.Origin[1] != OutputInfo.Origin[1] || InputInfo.Origin[2] != OutputInfo.Origin[2])
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
void IntegralImageBuilder2D<InputPixelType, OutputPixelType>::Update()
{
    if (this->CheckInput() == false)
    {
        return;
    }

	auto InputSize = m_InputImage->GetSize();

	for (int_max y = 0; y < InputSize[1]; ++y)
	{
		auto tempOutputPixel = OutputPixelType(0);

		for (int_max x = 0; x < InputSize[0]; ++x)
		{
			tempOutputPixel += OutputPixelType((*m_InputImage)(x, y));

			m_OutputImage(x, y) = tempOutputPixel;
		}
	}

	for (int_max x = 0; x < InputSize[0]; ++x)
	{
		auto tempOutputPixel = OutputPixelType(0);

		for (int_max y = 0; y < InputSize[1]; ++y)
		{
			tempOutputPixel += m_OutputImage(x, y);

			m_OutputImage(x, y) = tempOutputPixel;
		}
	}
}

}//end namespace mdk

