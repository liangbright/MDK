#ifndef __mdkImageInterpolation3D_hpp
#define __mdkImageInterpolation3D_hpp

namespace mdk
{

template<typename PixelType, typename ScalarType>
inline
PixelType InterpolateImageAt3DContinuousIndex(const Image3D<PixelType>& InputImage,
                                              ScalarType x, ScalarType y, ScalarType z, 
                                              const Option_Of_Image3DInterpolation<PixelType>& Option)
{
	switch (Option.MethodType)
    {
	case MethodEnum_Of_Image3DInterpolation::Nearest:
		return InterpolateImageAt3DContinuousIndex_Nearest(InputImage, x, y, z, Option);

	case MethodEnum_Of_Image3DInterpolation::Linear:
		return InterpolateImageAt3DContinuousIndex_Linear(InputImage, x, y, z, Option);

    //case MethodEnum_Of_Image3DInterpolation::Cubic:

    //    return InterpolateImageAt3DContinuousIndex_Cubic(InputImage, x, y, z, Option);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    }

	return PixelType(0);
}


template<typename PixelType, typename ScalarType>
inline
PixelType InterpolateImageAt3DPhysicalPosition(const Image3D<PixelType>& InputImage,
                                               ScalarType x, ScalarType y, ScalarType z, 
                                               const Option_Of_Image3DInterpolation<PixelType>& Option)
{
	switch (Option.MethodType)
    {
	case MethodEnum_Of_Image3DInterpolation::Nearest:
		return InterpolateImageAt3DPhysicalPosition_Nearest(InputImage, x, y, z, Option);

	case MethodEnum_Of_Image3DInterpolation::Linear:
		return InterpolateImageAt3DPhysicalPosition_Linear(InputImage, x, y, z, Option);

	//case MethodEnum_Of_Image3DInterpolation::Cubic:
    //    return InterpolateImageAt3DPhysicalPosition_Cubic(InputImage, x, y, z, Option);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkImageInterpolator3D InterpolateImageAt3DPhysicalPosition(...)")
    }

	return PixelType(0);
}


template<typename PixelType, typename ScalarType>
inline
PixelType InterpolateImageAt3DContinuousIndex_Nearest(const Image3D<PixelType>& InputImage,
                                                      ScalarType x, ScalarType y, ScalarType z, 
                                                      const Option_Of_Image3DInterpolation<PixelType>& Option)
{
    if (InputImage.IsEmpty() == true)
    {
        MDK_Error("InputImage is empty @ mdkImageInterpolator3D InterpolateImageAt3DContinuousIndex_Nearest(...)")
        return PixelType(0);
    }

    auto Size = InputImage.GetSize();

	auto x0 = int_max(x);
	auto y0 = int_max(y);
	auto z0 = int_max(z);

	if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image3DInterpolation::Constant)
	{		
		if (x0 < 0 || x0 >= Size[0])
		{
			return Option.Pixel_OutsideImage;
		}

		if (y0 < 0 || y0 >= Size[1])
		{
			return Option.Pixel_OutsideImage;
		}

		if (z0 < 0 || z0 >= Size[2])
		{
			return Option.Pixel_OutsideImage;
		}
	}
	else if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image3DInterpolation::Nearest)
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

		if (z0 < 0)
		{
			z0 = 0;
		}
		else if (z0 >= Size[2])
		{
			z0 = Size[2] - 1;
		}
	}	

	return InputImage(x0, y0, z0);
}


template<typename PixelType, typename ScalarType>
inline
PixelType InterpolateImageAt3DPhysicalPosition_Nearest(const Image3D<PixelType>& InputImage,
                                                       ScalarType x, ScalarType y, ScalarType z, 
                                                       const Option_Of_Image3DInterpolation<PixelType>& Option)
{
	auto ContinuousIndex = InputImage.Transform3DPhysicalPositionTo3DContinuousIndex<ScalarType>(x, y, z);
	return InterpolateImageAt3DContinuousIndex_Nearest(InputImage, ContinuousIndex[0], ContinuousIndex[1], ContinuousIndex[2], Option);
}


template<typename PixelType, typename ScalarType>
inline
PixelType InterpolateImageAt3DContinuousIndex_Linear(const Image3D<PixelType>& InputImage,
                                                     ScalarType x, ScalarType y, ScalarType z, 
                                                     const Option_Of_Image3DInterpolation<PixelType>& Option)
{
    if (InputImage.IsEmpty()  == true)
    {
        MDK_Error("InputImage is empty @ mdkImageInterpolator3D InterpolateImageAtContinuousIndex_Linear(...)")
        return PixelType(0);
    }

    auto Size = InputImage.GetSize();
   
	auto x0 = int_max(x);
	auto y0 = int_max(y);
	auto z0 = int_max(z);

	//---------------------------------------------
	if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image3DInterpolation::Constant)
	{
		if (x0 < 0 || x0 >= Size[0])
		{
			return Option.Pixel_OutsideImage;
		}

		if (y0 < 0 || y0 >= Size[1])
		{
			return Option.Pixel_OutsideImage;
		}

		if (z0 < 0 || z0 >= Size[2])
		{
			return Option.Pixel_OutsideImage;
		}
	}
	else if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image3DInterpolation::Nearest)
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

		if (z0 < 0)
		{
			z0 = 0;
		}
		else if (z0 >= Size[2])
		{
			z0 = Size[2] - 1;
		}

		return InputImage(x0, y0, z0);
	}
	//---------------------------------------------

    int_max x1 = x0 + 1;
    int_max y1 = y0 + 1;
    int_max z1 = z0 + 1;

    if (x0 < 0)
    {
        x0 = 0;
        x1 = 0;
    }
    else if (x0 >= Size[0])
    {
        x0 = Size[0] - 1;
        x1 = x0;
    }

    if (y0 < 0)
    {
        y0 = 0;
        y1 = 0;
    }
    else if (y0 >= Size[1])
    {
        y0 = Size[1] - 1;
        y1 = y0;
    }

    if (z0 < 0)
    {
        z0 = 0;
        z1 = 0;
    }
    else if (z0 >= Size[2])
    {
        z0 = Size[2] - 1;
        z1 = z0;
    }

	auto Pixel_x0_y0_z0 = InputImage(x0, y0, z0);
	auto Pixel_x0_y1_z0 = InputImage(x0, y1, z0);
	auto Pixel_x0_y0_z1 = InputImage(x0, y0, z1);
	auto Pixel_x0_y1_z1 = InputImage(x0, y1, z1);
	auto Pixel_x1_y0_z0 = InputImage(x1, y0, z0);
	auto Pixel_x1_y1_z0 = InputImage(x1, y1, z0);
	auto Pixel_x1_y0_z1 = InputImage(x1, y0, z1);
	auto Pixel_x1_y1_z1 = InputImage(x1, y1, z1);

	auto dx = x - ScalarType(x0);
	auto dy = y - ScalarType(y0);
	auto dz = z - ScalarType(z0);

	auto One = ScalarType(1);

	auto c00 = Pixel_x0_y0_z0*(One - dx) + Pixel_x1_y0_z0*dx;
	auto c10 = Pixel_x0_y1_z0*(One - dx) + Pixel_x1_y1_z0*dx;
	auto c01 = Pixel_x0_y0_z1*(One - dx) + Pixel_x1_y0_z1*dx;
	auto c11 = Pixel_x0_y1_z1*(One - dx) + Pixel_x1_y1_z1*dx;

	auto c0 = c00*(One - dy) + c10*dy;
	auto c1 = c01*(One - dy) + c11*dy;

	auto c = c0*(One - dz) + c1*dz;

    return c;
}


template<typename PixelType, typename ScalarType>
inline
PixelType InterpolateImageAt3DPhysicalPosition_Linear(const Image3D<PixelType>& InputImage,
                                                      ScalarType x, ScalarType y, ScalarType z, 
                                                      const Option_Of_Image3DInterpolation<PixelType>& Option)
{
	auto ContinuousIndex = InputImage.Transform3DPhysicalPositionTo3DContinuousIndex(x, y, z);
	return InterpolateImageAt3DContinuousIndex_Linear(InputImage, ContinuousIndex[0], ContinuousIndex[1], ContinuousIndex[2], Option);
}


template<typename PixelType, typename ScalarType>
inline
PixelType InterpolateImageAt3DContinuousIndex_Cubic(const Image3D<PixelType>& InputImage,
                                                    ScalarType x, ScalarType y, ScalarType z, 
                                                    const Option_Of_Image3DInterpolation<PixelType>& Option)
{   
	MDK_Error("Not implemented yet")
}


template<typename PixelType, typename ScalarType>
inline
PixelType InterpolateImageAt3DPhysicalPosition_Cubic(const Image3D<PixelType>& InputImage,
                                                     ScalarType x, ScalarType y, ScalarType z, 
                                                     const Option_Of_Image3DInterpolation<PixelType>& Option)
{
	MDK_Error("Not implemented yet")
}

}// namespace mdk

#endif