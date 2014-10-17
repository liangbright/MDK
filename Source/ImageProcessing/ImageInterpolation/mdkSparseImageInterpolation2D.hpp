#ifndef __mdkSparseImageInterpolation2D_hpp
#define __mdkSparseImageInterpolation2D_hpp

namespace mdk
{
template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DContinuousIndex(const SparseImage2D<InputPixelType>& InputImage,
                                                    ScalarType x, ScalarType y,
													const Option_Of_Image2DInterpolation<InputPixelType>& Option)
{
	switch (Option.MethodType)
    {
	case MethodEnum_Of_Image2DInterpolation::Nearest:
		return InterpolateImageAt2DContinuousIndex_Nearest<OutputPixelType>(InputImage, x, y, Option);

	case MethodEnum_Of_Image2DInterpolation::Linear:
		return InterpolateImageAt2DContinuousIndex_Linear<OutputPixelType>(InputImage, x, y, Option);

    //case MethodEnum_Of_Image2DInterpolation::Cubic:

    //    return InterpolateImageAt2DContinuousIndex_Cubic<OutputPixelType(InputImage, x, y, Option);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkImageInterpolation2D InterpolateImageAtContinuousIndex(...)")
		return OutputPixelType(Option.Pixel_OutsideImage);
    }
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPhysicalPosition(const SparseImage2D<InputPixelType>& InputImage,
                                                     ScalarType x, ScalarType y,
											         const Option_Of_Image2DInterpolation<InputPixelType>& Option)
{
	switch (Option.MethodType)
    {
	case MethodEnum_Of_Image2DInterpolation::Nearest:
		return InterpolateImageAt2DPhysicalPosition_Nearest<OutputPixelType>(InputImage, x, y, Option);

	case MethodEnum_Of_Image2DInterpolation::Linear:
		return InterpolateImageAt2DPhysicalPosition_Linear<OutputPixelType>(InputImage, x, y, Option);

	//case MethodEnum_Of_Image2DInterpolation::Cubic:
    //    return InterpolateImageAt2DPhysicalPosition_Cubic<OutputPixelType>(InputImage, x, y, Option);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkImageInterpolation2D InterpolateImageAt2DPhysicalPosition(...)")
		return OutputPixelType(Option.Pixel_OutsideImage);
    }	
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DContinuousIndex_Nearest(const SparseImage2D<InputPixelType>& InputImage,
                                                            ScalarType x, ScalarType y,
													        const Option_Of_Image2DInterpolation<InputPixelType>& Option)
{
    if (InputImage.IsEmpty() == true)
    {
        MDK_Error("InputImage is empty @ mdkImageInterpolation2D InterpolateImageAt2DContinuousIndex_Nearest(...)")
        return OutputPixelType(Option.Pixel_OutsideImage);
    }

    auto Size = InputImage.GetSize();

	auto x0 = int_max(std::round(x));
	auto y0 = int_max(std::round(y));
	
	if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image2DInterpolation::Constant)
	{		
		if (x0 < 0 || x0 >= Size[0])
		{
			return OutputPixelType(Option.Pixel_OutsideImage);
		}

		if (y0 < 0 || y0 >= Size[1])
		{
			return OutputPixelType(Option.Pixel_OutsideImage);
		}
	}
	else if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image2DInterpolation::Nearest)
	{
		if (x0 < 0)
		{
			x0 = 0;
		}
		else if (x0 >= Size[0])
		{
			x0 = Size[0] - 1;
		}

		if (y0 < 0)
		{
			y0 = 0;
		}
		else if (y0 >= Size[1])
		{
			y0 = Size[1] - 1;
		}
	}	

	return OutputPixelType(InputImage(x0, y0));
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPhysicalPosition_Nearest(const SparseImage2D<InputPixelType>& InputImage,
                                                             ScalarType x, ScalarType y,
													         const Option_Of_Image2DInterpolation<InputPixelType>& Option)
{
	auto Index2D = InputImage.Transform2DPhysicalPositionTo2DIndex(x, y);
	return InterpolateImageAt2DContinuousIndex_Nearest<OutputPixelType>(InputImage, Index2D[0], Index2D[1], Option);
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DContinuousIndex_Linear(const SparseImage2D<InputPixelType>& InputImage,
                                                           ScalarType x, ScalarType y,
													       const Option_Of_Image2DInterpolation<InputPixelType>& Option)
{
    if (InputImage.IsEmpty()  == true)
    {
        MDK_Error("InputImage is empty @ mdkImageInterpolation2D InterpolateImageAtContinuousIndex_Linear(...)")
        return OutputPixelType(Option.Pixel_OutsideImage);
    }

    auto Size = InputImage.GetSize();
   
	auto x0 = int_max(x);
	auto y0 = int_max(y);
	//---------------------------------------------
	if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image2DInterpolation::Constant)
	{
		if (x0 < 0 || x0 >= Size[0])
		{
			return OutputPixelType(Option.Pixel_OutsideImage);
		}

		if (y0 < 0 || y0 >= Size[1])
		{
			return OutputPixelType(Option.Pixel_OutsideImage);
		}
	}
	else if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image2DInterpolation::Nearest)
	{
		if (x0 < 0)
		{
			x0 = 0;
		}
		else if (x0 >= Size[0])
		{
			x0 = Size[0] - 1;
		}

		if (y0 < 0)
		{
			y0 = 0;
		}
		else if (y0 >= Size[1])
		{
			y0 = Size[1] - 1;
		}

		return OutputPixelType(InputImage(x0, y0));
	}
	//---------------------------------------------

    int_max x1 = x0 + 1;
    int_max y1 = y0 + 1;

    if (x0 < 0)
    {
        x0 = 0;
        x1 = 0;
    }
    else if (x0 >= Size[0]-1)
    {
        x0 = Size[0] - 1;
        x1 = x0;
    }

    if (y0 < 0)
    {
        y0 = 0;
        y1 = 0;
    }
    else if (y0 >= Size[1]-1)
    {
        y0 = Size[1] - 1;
        y1 = y0;
    }

	auto Pixel_x0_y0 = OutputPixelType(InputImage(x0, y0));
	auto Pixel_x0_y1 = OutputPixelType(InputImage(x0, y1));
	auto Pixel_x1_y0 = OutputPixelType(InputImage(x1, y0));
	auto Pixel_x1_y1 = OutputPixelType(InputImage(x1, y1));

	auto dx = x - ScalarType(x0);
	auto dy = y - ScalarType(y0);

	auto One = ScalarType(1);

	auto c00 = Pixel_x0_y0*(One - dx) + Pixel_x1_y0*dx;
	auto c10 = Pixel_x0_y1*(One - dx) + Pixel_x1_y1*dx;

	auto c0 = c00*(One - dy) + c10*dy;

    return c0;
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPhysicalPosition_Linear(const SparseImage2D<InputPixelType>& InputImage,
                                                            ScalarType x, ScalarType y,
													        const Option_Of_Image2DInterpolation<InputPixelType>& Option)
{
	auto Index2D = InputImage.Transform2DPhysicalPositionTo2DIndex(x, y);
	return InterpolateImageAt2DContinuousIndex_Linear<OutputPixelType>(InputImage, Index2D[0], Index2D[1], Option);
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DContinuousIndex_Cubic(const SparseImage2D<InputPixelType>& InputImage,
                                                          ScalarType x, ScalarType y,
													      const Option_Of_Image2DInterpolation<InputPixelType>& Option)
{   
	MDK_Error("Not implemented yet")
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPhysicalPosition_Cubic(const SparseImage2D<InputPixelType>& InputImage,
                                                           ScalarType x, ScalarType y,
													       const Option_Of_Image2DInterpolation<InputPixelType>& Option)
{
	MDK_Error("Not implemented yet")
}

}// namespace mdk

#endif