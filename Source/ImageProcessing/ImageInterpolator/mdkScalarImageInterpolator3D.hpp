#ifndef __mdkScalarImageInterpolator3D_hpp
#define __mdkScalarImageInterpolator3D_hpp

namespace mdk
{

template<typename PixelType>
inline
double InterpolateScalarImageAtContinuousIndex(const Image3D<PixelType>& InputImage,
                                               double x, double y, double z, 
                                               ScalarImage3DInterpolationMethodEnum InterpolationMethod,
                                               const Option_Of_ScalarImageInterpolator3D& Option)
{
    switch (InterpolationMethod)
    {
    case ScalarImage3DInterpolationMethodEnum::Nearest:

        return InterpolateScalarImageAtContinuousIndex_Nearest(InputImage, x, y, z, Option);

    case ScalarImage3DInterpolationMethodEnum::Linear:

        return InterpolateScalarImageAtContinuousIndex_Linear(InputImage, x, y, z, Option);

    //case ScalarImage3DInterpolationMethodEnum::Cubic:

    //    return InterpolateScalarImageAtContinuousIndex_Cubic(InputImage, x, y, z, Option);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkScalarImageInterpolator3D InterpolateScalarImageAtContinuousIndex(...)")
        return PixelType(0);
    }
}


template<typename PixelType>
inline
double InterpolateScalarImageAtPhysicalPosition(const Image3D<PixelType>& InputImage,
                                                double x, double y, double z, 
                                                ScalarImage3DInterpolationMethodEnum InterpolationMethod,
                                                const Option_Of_ScalarImageInterpolator3D& Option)
{
	double OutputPixelValue = 0;

    switch (InterpolationMethod)
    {
    case ScalarImage3DInterpolationMethodEnum::Nearest:

		OutputPixelValue = double(InterpolateScalarImageAtPhysicalPosition_Nearest(InputImage, x, y, z, Option));

    case ScalarImage3DInterpolationMethodEnum::Linear:

		OutputPixelValue = InterpolateScalarImageAtPhysicalPosition_Linear(InputImage, x, y, z, Option);

    //case ScalarImage3DInterpolationMethodEnum::Cubic:

    //    return InterpolateScalarImageAtPhysicalPosition_Cubic(InputImage, x, y, z, Option);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkScalarImageInterpolator3D InterpolateScalarImageAtPhysicalPosition(...)")
    }

	return OutputPixelValue;
}


template<typename PixelType>
inline
PixelType InterpolateScalarImageAtContinuousIndex_Nearest(const Image3D<PixelType>& InputImage,
                                                          double x, double y, double z, 
                                                          const Option_Of_ScalarImageInterpolator3D& Option)
{
    if (InputImage.IsEmpty() == true)
    {
        MDK_Error("InputImage is empty @ mdkScalarImageInterpolator3D InterpolateScalarImageAtContinuousIndex_Nearest(...)")
        return 0;
    }

    auto Size = InputImage.GetSize();

	int_max x0 = int_max(x);
	int_max y0 = int_max(y);
	int_max z0 = int_max(z);

	if (Option.BoundaryOption == BoundaryOption_Of_ScalarImageInterpolator3D::Constant)
	{		
		if (x0 < 0 || x0 >= Size.Lx)
		{
			return PixelType(Option.PixelValue_Outside);
		}

		if (y0 < 0 || y0 >= Size.Ly)
		{
			return PixelType(Option.PixelValue_Outside);
		}

		if (z0 < 0 || z0 >= Size.Lz)
		{
			return PixelType(Option.PixelValue_Outside);
		}
	}
	else if (Option.BoundaryOption == BoundaryOption_Of_ScalarImageInterpolator3D::Nearest)
	{
		if (x0 < 0)
		{
			x0 = 0;
		}
		else if (x0 >= Size.Lx)
		{
			x0 = Size.Lx - 1;
		}

		if (y0 < 0)
		{
			y0 = 0;
		}
		else if (y0 >= Size.Ly)
		{
			y0 = Size.Ly - 1;
		}

		if (z0 < 0)
		{
			z0 = 0;
		}
		else if (z0 >= Size.Lz)
		{
			z0 = Size.Lz - 1;
		}
	}	

	return InputImage(x0, y0, z0);
}


template<typename PixelType>
inline
PixelType InterpolateScalarImageAtPhysicalPosition_Nearest(const Image3D<PixelType>& InputImage,
                                                           double x, double y, double z, 
                                                           const Option_Of_ScalarImageInterpolator3D& Option)
{
    double x_Index, y_Index, z_Index;

    InputImage.Transform3DPhysicalPositionTo3DIndex(x, y, z, x_Index, y_Index, z_Index);

    return InterpolateScalarImageAtContinuousIndex_Nearest(InputImage, x_Index, y_Index, z_Index, Option);
}


template<typename PixelType>
double InterpolateScalarImageAtContinuousIndex_Linear(const Image3D<PixelType>& InputImage,
                                                      double x, double y, double z, 
                                                      const Option_Of_ScalarImageInterpolator3D& Option)
{
    if (InputImage.IsEmpty()  == true)
    {
        MDK_Error("InputImage is empty @ mdkScalarImageInterpolator3D InterpolateScalarImageAtContinuousIndex_Linear(...)")
        return 0;
    }

    auto Size = InputImage.GetSize();
   
	int_max x0 = int_max(x);
	int_max y0 = int_max(y);
	int_max z0 = int_max(z);

	//---------------------------------------------
	if (Option.BoundaryOption == BoundaryOption_Of_ScalarImageInterpolator3D::Constant)
	{
		if (x0 < 0 || x0 >= Size.Lx)
		{
			return Option.PixelValue_Outside;
		}

		if (y0 < 0 || y0 >= Size.Ly)
		{
			return Option.PixelValue_Outside;
		}

		if (z0 < 0 || z0 >= Size.Lz)
		{
			return Option.PixelValue_Outside;
		}
	}
	else if (Option.BoundaryOption == BoundaryOption_Of_ScalarImageInterpolator3D::Nearest)
	{
		if (x0 < 0)
		{
			x0 = 0;
		}
		else if (x0 >= Size.Lx)
		{
			x0 = Size.Lx - 1;
		}

		if (y0 < 0)
		{
			y0 = 0;
		}
		else if (y0 >= Size.Ly)
		{
			y0 = Size.Ly - 1;
		}

		if (z0 < 0)
		{
			z0 = 0;
		}
		else if (z0 >= Size.Lz)
		{
			z0 = Size.Lz - 1;
		}

		return double(InputImage(x0, y0, z0));
	}
	//---------------------------------------------

    int_max x1 = x0 + 1;
    int_max y1 = y0 + 1;
    int_max z1 = z0 + 1;

    double dx = x - double(x0);
    double dy = y - double(y0);
    double dz = z - double(z0);

    if (x0 < 0)
    {
        x0 = 0;
        x1 = 0;
    }
    else if (x0 >= Size.Lx)
    {
        x0 = Size.Lx - 1;
        x1 = x0;
    }

    if (y0 < 0)
    {
        y0 = 0;
        y1 = 0;
    }
    else if (y0 >= Size.Ly)
    {
        y0 = Size.Ly - 1;
        y1 = y0;
    }

    if (z0 < 0)
    {
        z0 = 0;
        z1 = 0;
    }
    else if (z0 >= Size.Lz)
    {
        z0 = Size.Lz - 1;
        z1 = z0;
    }

	auto Value_x0_y0_z0 = double(InputImage(x0, y0, z0));
	auto Value_x0_y1_z0 = double(InputImage(x0, y1, z0));
	auto Value_x0_y0_z1 = double(InputImage(x0, y0, z1));
	auto Value_x0_y1_z1 = double(InputImage(x0, y1, z1));
	auto Value_x1_y0_z0 = double(InputImage(x1, y0, z0));
	auto Value_x1_y1_z0 = double(InputImage(x1, y1, z0));
	auto Value_x1_y0_z1 = double(InputImage(x1, y0, z1));
	auto Value_x1_y1_z1 = double(InputImage(x1, y1, z1));

    auto c00 = Value_x0_y0_z0*(1.0 - dx) + Value_x1_y0_z0*dx;
    auto c10 = Value_x0_y1_z0*(1.0 - dx) + Value_x1_y1_z0*dx;
    auto c01 = Value_x0_y0_z1*(1.0 - dx) + Value_x1_y0_z1*dx;
    auto c11 = Value_x0_y1_z1*(1.0 - dx) + Value_x1_y1_z1*dx;

    auto c0 = c00*(1.0 - dy) + c10*dy;
    auto c1 = c01*(1.0 - dy) + c11*dy;

    auto c = c0*(1 - dz) + c1*dz;

    return c;
}


template<typename PixelType>
inline
double InterpolateScalarImageAtPhysicalPosition_Linear(const Image3D<PixelType>& InputImage,
                                                       double x, double y, double z, 
                                                       const Option_Of_ScalarImageInterpolator3D& Option)
{
    double x_Index, y_Index, z_Index;

    InputImage.Transform3DPhysicalPositionTo3DIndex(x, y, z, x_Index, y_Index, z_Index);

    return InterpolateScalarImageAtContinuousIndex_Linear(InputImage, x_Index, y_Index, z_Index, Option);
}


template<typename PixelType>
inline
double InterpolateScalarImageAtContinuousIndex_Cubic(const Image3D<PixelType>& InputImage,
                                                     double x, double y, double z, 
                                                     const Option_Of_ScalarImageInterpolator3D& Option)
{   
	MDK_Error("Not implemented yet")
}


template<typename PixelType>
inline
double InterpolateScalarImageAtPhysicalPosition_Cubic(const Image3D<PixelType>& InputImage,
                                                      double x, double y, double z, 
                                                      const Option_Of_ScalarImageInterpolator3D& Option)
{
	MDK_Error("Not implemented yet")
}

}// namespace mdk

#endif