#pragma once

namespace mdk
{
template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex(const DenseImage2D<InputPixelType>& InputImage,
                                          ScalarType x, ScalarType y,
										  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
										  bool EnableBoundCheck)
{
	switch (Option.MethodType)
    {
	case MethodEnum_Of_Image2DInterpolation::Nearest:
		return InterpolateImageAt2DIndex_Nearest<OutputPixelType>(InputImage, x, y, Option, EnableBoundCheck);

	case MethodEnum_Of_Image2DInterpolation::Linear:
		return InterpolateImageAt2DIndex_Linear<OutputPixelType>(InputImage, x, y, Option, EnableBoundCheck);

    //case MethodEnum_Of_Image2DInterpolation::Cubic:
    //    return InterpolateImageAt2DIndex_Cubic<OutputPixelType(InputImage, x, y, Option, EnableBoundCheck);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkImageInterpolation2D InterpolateImageAtIndex(...)")
		return  OutputPixelType(Option.Pixel_OutsideImage);
    }
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition(const DenseImage2D<InputPixelType>& InputImage,
                                             ScalarType x, ScalarType y,
											 const Option_Of_Image2DInterpolation<InputPixelType>& Option,
											 bool EnableBoundCheck)
{
	auto Index2D = InputImage.Transform2DPositionTo2DIndex(x, y);
	return InterpolateImageAt2DIndex<OutputPixelType>(InputImage, Index2D[0], Index2D[1], Option, EnableBoundCheck);
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition(const DenseImage2D<InputPixelType>& InputImage,
											      ScalarType x, ScalarType y, ScalarType z,
                                                  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck)
{
	auto Index2D = InputImage.Transform3DWorldPositionTo2DIndex(x, y, z);
	return InterpolateImageAt2DIndex<OutputPixelType>(InputImage, Index2D[0], Index2D[1], Option, EnableBoundCheck);
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Nearest(const DenseImage2D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y,
												  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck)
{
    auto Size = InputImage.GetSize();

	auto x0 = int_max(std::round(x));
	auto y0 = int_max(std::round(y));

	if (EnableBoundCheck == true)
	{
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
		else if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image2DInterpolation::Replicate)
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
	}

	return OutputPixelType(InputImage.GetPixelAt2DIndex(x0, y0));
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Linear(const DenseImage2D<InputPixelType>& InputImage,
                                                 ScalarType x, ScalarType y,
												 const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												 bool EnableBoundCheck)
{
    auto Size = InputImage.GetSize();
   
	auto x0 = int_max(x);
	auto y0 = int_max(y);

	//---------------------------------------------
	if (EnableBoundCheck == true)
	{
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

			if (x0 == Size[0] - 1 || y0 == Size[1] - 1)// on edge
			{
				return OutputPixelType(InputImage.GetPixelAt2DIndex(x0, y0));
			}
		}
		else if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image2DInterpolation::Replicate)
		{
			bool Flag_return = false;

			if (x0 < 0)
			{
				x0 = 0;
				Flag_return = true;
			}
			else if (x0 >= Size[0] - 1) // on the edge or outside
			{
				x0 = Size[0] - 1;
				Flag_return = true;
			}

			if (y0 < 0)
			{
				y0 = 0;
				Flag_return = true;
			}
			else if (y0 >= Size[1] - 1)// on the edge or outside
			{
				y0 = Size[1] - 1;
				Flag_return = true;
			}

			if (Flag_return == true)
			{
				return OutputPixelType(InputImage.GetPixelAt2DIndex(x0, y0));
			}
		}
	}
	else // assume (x0, y0) is in the image (may be on edge)
	{//if on edge, then return
		if (x0 == Size[0] - 1)
		{
			return OutputPixelType(InputImage.GetPixelAt2DIndex(x0, y0));
		}

		if (y0 == Size[1] - 1)
		{
			return OutputPixelType(InputImage.GetPixelAt2DIndex(x0, y0));
		}
	}
	//-----------------------------------------------
	// (x0, y0) is now inside the image, NOT on edge

    int_max x1 = x0 + 1;
    int_max y1 = y0 + 1;

	auto Pixel_x0_y0 = OutputPixelType(InputImage.GetPixelAt2DIndex(x0, y0));
	auto Pixel_x0_y1 = OutputPixelType(InputImage.GetPixelAt2DIndex(x0, y1));
	auto Pixel_x1_y0 = OutputPixelType(InputImage.GetPixelAt2DIndex(x1, y0));
	auto Pixel_x1_y1 = OutputPixelType(InputImage.GetPixelAt2DIndex(x1, y1));

	auto dx = x - ScalarType(x0);
	auto dy = y - ScalarType(y0);

	auto One = ScalarType(1);

	auto c00 = Pixel_x0_y0*(One - dx) + Pixel_x1_y0*dx;
	auto c10 = Pixel_x0_y1*(One - dx) + Pixel_x1_y1*dx;

	auto c0 = c00*(One - dy) + c10*dy;

	return OutputPixelType(c0);
}

/*
template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Cubic(const DenseImage2D<InputPixelType>& InputImage,
                                                ScalarType x, ScalarType y,
												const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												bool EnableBoundCheck)
{   
	MDK_Error("Not implemented yet")
	return GetZeroPixel<OutputPixelType>();
}
*/

}// namespace mdk

