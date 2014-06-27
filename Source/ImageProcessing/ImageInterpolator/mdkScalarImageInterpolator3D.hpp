#ifndef __mdkScalarImageInterpolator3D_hpp
#define __mdkScalarImageInterpolator3D_hpp

namespace mdk
{

template<typename PixelType>
inline
double InterpolateImageAtContinuousIndex(const Image3D<PixelType>& InputImage,
                                         double x, double y, double z, 
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<PixelType>& Option)
{
    switch (InterpolationMethod)
    {
    case ImageInterpolation3DMethodTypeEnum::Nearest:

        return InterpolateImageAtContinuousIndex_Nearest(InputImage, x, y, z, Option);

    case ImageInterpolation3DMethodTypeEnum::Linear:

        return InterpolateImageAtContinuousIndex_Linear(InputImage, x, y, z, Option);

    //case ImageInterpolation3DMethodTypeEnum::Cubic:

    //    return InterpolateImageAtContinuousIndex_Cubic(InputImage, x, y, z, Option);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
        return 0.0;
    }
}


template<typename PixelType>
inline
double InterpolateImageAtPhysicalPosition(const Image3D<PixelType>& InputImage,
                                          double x, double y, double z, 
                                          ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                          const Option_Of_ImageInterpolator3D<PixelType>& Option)
{
    switch (InterpolationMethod)
    {
    case ImageInterpolation3DMethodTypeEnum::Nearest:

        return InterpolateImageAtPhysicalPosition_Nearest(InputImage, x, y, z, Option);

    case ImageInterpolation3DMethodTypeEnum::Linear:

        return InterpolateImageAtPhysicalPosition_Linear(InputImage, x, y, z, Option);

    //case ImageInterpolation3DMethodTypeEnum::Cubic:

    //    return InterpolateImageAtPhysicalPosition_Cubic(InputImage, x, y, z, Option);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkLinearImageInterpolator3D InterpolateImageAtPhysicalPosition(...)")
        return 0.0;
    }
}


template<typename PixelType>
inline
double InterpolateImageAtContinuousIndex_Nearest(const Image3D<PixelType>& InputImage,
                                                 double x, double y, double z, 
                                                 const Option_Of_ImageInterpolator3D<PixelType>& Option)
{
    if (InputImage.IsEmpty() == true)
    {
        MDK_Error("InputImage is empty @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex_Nearest(...)")
        return 0.0;
    }

    auto Size = InputImage.GetSize();

    int_max x0 = int_max(x);
    int_max y0 = int_max(y);
    int_max z0 = int_max(z);

    if (x0 < 0 || x0 >= Size.Lx)
    {
        return double(Option.PixelValueToFillIfOutOfBound);
    }

    if (y0 < 0 || y0 >= Size.Ly)
    {
        return double(Option.PixelValueToFillIfOutOfBound);
    }

    if (z0 < 0 || z0 >= Size.Lz)
    {
        return double(Option.PixelValueToFillIfOutOfBound);
    }

    return double(InputImage(x0, y0, z0));
}


template<typename PixelType>
inline
double InterpolateImageAtPhysicalPosition_Nearest(const Image3D<PixelType>& InputImage,
                                                  double x, double y, double z, 
                                                  const Option_Of_ImageInterpolator3D<PixelType>& Option)
{
    double x_Index, y_Index, z_Index;

    InputImage.Transform3DPhysicalPositionTo3DContinuousIndex(x, y, z, x_Index, y_Index, z_Index);

    return InterpolateImageAtContinuousIndex_Nearest(InputImage, x_Index, y_Index, z_Index, Option);
}


template<typename PixelType>
double InterpolateImageAtContinuousIndex_Linear(const Image3D<PixelType>& InputImage,
                                                double x, double y, double z, 
                                                const Option_Of_ImageInterpolator3D<PixelType>& Option)
{
    if (InputImage.IsEmpty()  == true)
    {
        MDK_Error("InputImage is empty @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex_Linear(...)")
        return 0.0;
    }

    auto Size = InputImage.GetSize();

    if (x < 0.0 || x > double(Size.Lx))
    {
        return double(Option.PixelValueToFillIfOutOfBound);
    }

    if (y < 0.0 || y > double(Size.Ly))
    {
        return double(Option.PixelValueToFillIfOutOfBound);
    }

    if (z < 0.0 || z > double(Size.Lz))
    {
        return double(Option.PixelValueToFillIfOutOfBound);
    }

    int_max x0 = int_max(x);
    int_max x1 = x0 + 1;

    int_max y0 = int_max(y);
    int_max y1 = y0 + 1;

    int_max z0 = int_max(z);
    int_max z1 = z0 + 1;

    double dx = x - double(x0);
    double dy = y - double(y0);
    double dz = z - double(z0);

    double PixelValue_x0_y0_z0 = 0;
    double PixelValue_x0_y1_z0 = 0;
    double PixelValue_x0_y0_z1 = 0;
    double PixelValue_x0_y1_z1 = 0;
    double PixelValue_x1_y0_z0 = 0;
    double PixelValue_x1_y1_z0 = 0;
    double PixelValue_x1_y0_z1 = 0;
    double PixelValue_x1_y1_z1 = 0;

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
double InterpolateImageAtPhysicalPosition_Linear(const Image3D<PixelType>& InputImage,
                                                 double x, double y, double z, 
                                                 const Option_Of_ImageInterpolator3D<PixelType>& Option)
{
    double x_Index, y_Index, z_Index;

    InputImage.Transform3DPhysicalPositionTo3DContinuousIndex(x, y, z, x_Index, y_Index, z_Index);

    return InterpolateImageAtContinuousIndex_Linear(InputImage, x_Index, y_Index, z_Index, Option);
}


template<typename PixelType>
inline
double InterpolateImageAtContinuousIndex_Cubic(const Image3D<PixelType>& InputImage,
                                                  double x, double y, double z, 
                                                  const Option_Of_ImageInterpolator3D<PixelType>& Option)
{   
    // compiler error
    temp = 1;
}


template<typename PixelType>
inline
double InterpolateImageAtPhysicalPosition_Cubic(const Image3D<PixelType>& InputImage,
                                                   double x, double y, double z, 
                                                   const Option_Of_ImageInterpolator3D<PixelType>& Option)
{
    // compiler error
    temp = 1;
}

}// namespace mdk

#endif