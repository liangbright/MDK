#pragma once

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
	this->InputImage = nullptr;
	this->OutputImage.Clear();
	this->MaxThreadCount = 1;
}

template<typename InputPixelType, typename OutputPixelType>
bool IntegralImageBuilder3D<InputPixelType, OutputPixelType>::CheckInput()
{
	auto& Self = *this;
	if (Self.InputImage == nullptr)
    {
        MDK_Error("InputImage is nullptr @ IntegralImageBuilder3D::CheckInput()")
        return false;
    }

	if (Self.InputImage->IsEmpty() == true)
	{
		MDK_Error("InputImage is empty @ IntegralImageBuilder3D::CheckInput()")
		return false;
	}

	if (Self.MaxThreadCount <= 0)
	{
		MDK_Error("MaxThreadCount <= 0 @ IntegralImageBuilder3D::CheckInput()")
		return false;
	}

	bool Flag_Init_OutputImage = false;
	if (Self.OutputImage.IsEmpty() == true)
	{
		Flag_Init_OutputImage = true;
	}
	else
	{
		auto InputInfo = Self.InputImage->GetInfo();
		auto OutputInfo = Self.OutputImage.GetInfo();
		if (InputInfo.Size[0] != OutputInfo.Size[0] || InputInfo.Size[1] != OutputInfo.Size[1] || InputInfo.Size[2] != OutputInfo.Size[2]
			|| InputInfo.Origin[0] != OutputInfo.Origin[0] || InputInfo.Origin[1] != OutputInfo.Origin[1] || InputInfo.Origin[2] != OutputInfo.Origin[2]
			|| InputInfo.Spacing[0] != OutputInfo.Spacing[0] || InputInfo.Spacing[1] != OutputInfo.Spacing[1] || InputInfo.Spacing[2] != OutputInfo.Spacing[2])
		{
			Flag_Init_OutputImage = true;
		}
	}

	if (Flag_Init_OutputImage == true)
	{
		Self.OutputImage.SetOrigin(Self.InputImage->GetOrigin());
		Self.OutputImage.SetSpacing(Self.InputImage->GetSpacing());
		Self.OutputImage.SetOrientation(Self.InputImage->GetOrientation());
		Self.OutputImage.SetSize(Self.InputImage->GetSize());
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

	auto& Self = *this;
    auto InputSize = Self.InputImage->GetSize();

    for (int_max z = z_Index_start; z <= z_Index_end; ++z)
    {
        for (int_max y = 0; y < InputSize[1]; ++y)
        {
			auto tempOutputPixel = OutputPixelType(0);

            for (int_max x = 0; x < InputSize[0]; ++x)
            {
				tempOutputPixel += OutputPixelType((*Self.InputImage)(x, y, z));

				Self.OutputImage(x, y, z) = tempOutputPixel;
            }
        }

        for (int_max x = 0; x < InputSize[0]; ++x)
        {
			auto tempOutputPixel = OutputPixelType(0);

            for (int_max y = 0; y < InputSize[1]; ++y)
            {
				tempOutputPixel += Self.OutputImage(x, y, z);

				Self.OutputImage(x, y, z) = tempOutputPixel;
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

	auto& Self = *this;
    auto InputSize = Self.InputImage->GetSize();

    for (int_max k = xy_LinearIndex_start; k <= xy_LinearIndex_end; ++k)
    {
        int_max y = k / InputSize[0];
        int_max x = k % InputSize[0];

		auto tempOutputPixel = OutputPixelType(0);

        for (int_max z = 0; z < InputSize[2]; ++z)
        {
			tempOutputPixel += Self.OutputImage(x, y, z);

			Self.OutputImage(x, y, z) = tempOutputPixel;
        }                
    }
}


template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBuilder3D<InputPixelType, OutputPixelType>::Update()
{
    if (this->CheckInput() == false)
    {
        return;
    }

    // compute each 2D ScalarIntegralDenseImage -------------------------------------------------------------------------------------
	auto& Self = *this;
    auto InputSize = Self.InputImage->GetSize();

	ParallelBlock([&](int_max z_Index_start, int_max z_Index_end, int_max ThreadIndex){this->ComputeIntegralImage2D(z_Index_start, z_Index_end); },
                  0, InputSize[2] - 1, Self.MaxThreadCount, 1);

    // sum in z-direction ------------------------------------------------------------------------------------------------------------

    int_max MinNumberOfPositionPerThread = 128;

	ParallelBlock([&](int_max xy_Linear_start, int_max xy_Linear_end, int_max ThreadIndex){this->ComputeSumInDirectionZ(xy_Linear_start, xy_Linear_end); },
				   0, InputSize[0]*InputSize[1] - 1, Self.MaxThreadCount, MinNumberOfPositionPerThread);
}

}//end namespace mdk

