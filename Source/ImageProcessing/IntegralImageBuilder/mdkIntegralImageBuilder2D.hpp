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
	Input.Image = nullptr;
	Output.Image.Clear();
}


template<typename InputPixelType, typename OutputPixelType>
bool IntegralImageBuilder2D<InputPixelType, OutputPixelType>::CheckInput()
{
	if (Input.Image == nullptr)
    {
        MDK_Error("Input.Image is nullptr @ IntegralImageBuilder2D::CheckInput()")
        return false;
    }

	if (Input.Image->IsEmpty() == true)
	{
		MDK_Error("Input.Image is empty @ IntegralImageBuilder2D::CheckInput()")
		return false;
	}

	bool Flag_Init_OutputImage = false;
	if (Output.Image.IsEmpty() == true)
	{
		Flag_Init_OutputImage = true;
	}
	else
	{
		auto InputInfo = Input.Image->GetInfo();
		auto OutputInfo = Output.Image.GetInfo();
		if (InputInfo.Size[0] != OutputInfo.Size[0] || InputInfo.Size[1] != OutputInfo.Size[1]
			|| InputInfo.Spacing[0] != OutputInfo.Spacing[0] || InputInfo.Spacing[1] != OutputInfo.Spacing[1]
			|| InputInfo.Origin[0] != OutputInfo.Origin[0] || InputInfo.Origin[1] != OutputInfo.Origin[1] || InputInfo.Origin[2] != OutputInfo.Origin[2])
		{
			Flag_Init_OutputImage = true;
		}
	}

	if (Flag_Init_OutputImage == true)
	{
		Output.Image.SetOrigin(Input.Image->GetOrigin());
		Output.Image.SetSpacing(Input.Image->GetSpacing());
		Output.Image.SetOrientation(Input.Image->GetOrientation());
		Output.Image.SetSize(Input.Image->GetSize());
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

	auto InputSize = Input.Image->GetSize();

	for (int_max y = 0; y < InputSize[1]; ++y)
	{
		auto tempOutputPixel = OutputPixelType(0);

		for (int_max x = 0; x < InputSize[0]; ++x)
		{
			tempOutputPixel += OutputPixelType((*Input.Image)(x, y));

			Output.Image(x, y) = tempOutputPixel;
		}
	}

	for (int_max x = 0; x < InputSize[0]; ++x)
	{
		auto tempOutputPixel = OutputPixelType(0);

		for (int_max y = 0; y < InputSize[1]; ++y)
		{
			tempOutputPixel += Output.Image(x, y);

			Output.Image(x, y) = tempOutputPixel;
		}
	}
}

}//end namespace mdk

