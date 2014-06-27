#ifndef __mdkScalarImageConvolutionFilter3D_hpp
#define __mdkScalarImageConvolutionFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::ScalarImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::~ScalarImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
SetImageInterpolationMethodAndOption(ImageInterpolation3DMethodTypeEnum Method, const Option_Of_ImageInterpolator3D<OutputPixelType>& Option)
{
    m_InterpolationMethod = Method;

    m_InterpolationOption = Option;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex)
{    
    auto PointNumberInMask = m_Mask_3DIndex->GetElementNumber();

    auto BeginPointerOfMask = m_Mask_3DIndex->GetElementPointer();

    auto InputImageSize = m_InputImage->GetSize();

    auto tempPixel = OutputPixelType(0);

    bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskCenter_3DIndex(x_Index, y_Index, z_Index);

    if (CheckBoundAtThisCenter == true)
    {
        auto Lx = InputImageSize.Lx;
        auto Ly = InputImageSize.Ly;
        auto Lz = InputImageSize.Lz;

        for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
		{
            auto temp_x = int_max(Ptr[0]) + x_Index;

            if (temp_x < 0)
            {
                temp_x = 0;
            }
            else if (temp_x >= Lx)
            {
                temp_x = Lx - 1;
            }

            auto temp_y = int_max(Ptr[1]) + y_Index;

            if (temp_y < 0)
            {
                temp_y = 0;
            }
            else if (temp_y >= Ly)
            {
                temp_y = Ly - 1;
            }

            auto temp_z = int_max(Ptr[2]) + z_Index;

            if (temp_z < 0)
            {
                temp_z = 0;
            }
            else if (temp_z >= Lz)
            {
                temp_z = Lz - 1;
            }

            tempPixel += OutputPixelType((*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3]);
		}
	}
	else
	{
        for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
		{
            auto temp_x = int_max(Ptr[0]) + x_Index;
            auto temp_y = int_max(Ptr[1]) + y_Index;
            auto temp_z = int_max(Ptr[2]) + z_Index;

            tempPixel += OutputPixelType((*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3]);
		}
	}

	OutputPixel = tempPixel;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex)
{
    auto PointNumberInMask = m_Mask_3DPosition->GetElementNumber();

    auto BeginPointerOfMask = m_Mask_3DPosition->GetElementPointer();

    auto tempPixel = OutputPixelType(0);

    for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
    {
        auto tempValue = InterpolateImageAtPhysicalPosition(*m_InputImage, Ptr[0], Ptr[1], Ptr[2], m_InterpolationMethod, m_InterpolationOption);

        tempPixel += OutputPixelType(tempValue * Ptr[3]);
    }

    OutputPixel = tempPixel;
}

}// namespace mdk

#endif