#ifndef MDK_IntegralImageBuilder2D_hpp
#define MDK_IntegralImageBuilder2D_hpp

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
	m_MaxNumberOfThread = 1;
}

template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBuilder2D<InputPixelType, OutputPixelType>::SetInputImage(const DenseImage2D<InputPixelType>* InputImage);
{
	m_InputImage = InputImage;
}

template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBuilder2D<InputPixelType, OutputPixelType>::SetMaxNumberOfThread(int_max Number)
{
	m_MaxNumberOfThread = Number;
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

	if (m_MaxNumberOfThread <= 0)
	{
		MDK_Error("m_MaxNumberOfThread <= 0 @ IntegralImageBuilder2D::CheckInput()")
		return false;
	}

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
bool IntegralImageBuilder2D<InputPixelType, OutputPixelType>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

	auto InputSize = m_InputImage->GetSize();

	for (int_max y = 0; y < InputSize[1]; ++y)
	{
		auto tempOutoutPixel = OutputPixelType(0);

		for (int_max x = 0; x < InputSize[0]; ++x)
		{
			tempOutoutPixel += OutputPixelType((*m_InputImage)(x, y));

			m_OutputImage(x, y) = tempOutoutPixel;
		}
	}

	for (int_max x = 0; x < InputSize[0]; ++x)
	{
		auto tempOutoutPixel = OutputPixelType(0);

		for (int_max y = 0; y < InputSize[1]; ++y)
		{
			tempOutoutPixel += m_OutputImage(x, y);

			m_OutputImage(x, y) = tempOutoutPixel;
		}
	}

    return true;
}

}//end namespace mdk

#endif