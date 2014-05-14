#ifndef __mdkScalarImageInterpolator3D_WrongPixelType__hpp
#define __mdkScalarImageInterpolator3D_WrongPixelType__hpp

namespace mdk
{
double InterpolateImageAtContinuousIndex(const Image3D<char>& InputImage,
                                         double x, double y, double z, 
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<char>& Option)
{
    MDK_Error("PixelType is char, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<unsigned char>& InputImage,
                                         double x, double y, double z,
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<unsigned char>& Option)
{
    MDK_Error("PixelType is unsigned char, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<short>& InputImage,
                                         double x, double y, double z, 
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<short>& Option)
{
    MDK_Error("PixelType is short, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<unsigned short>& InputImage,
                                         double x, double y, double z,
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<unsigned short>& Option)
{
    MDK_Error("PixelType is unsigned short, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<int>& InputImage,
                                         double x, double y, double z, 
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<int>& Option)
{
    MDK_Error("PixelType is int, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<unsigned int>& InputImage,
                                         double x, double y, double z,
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<unsigned int>& Option)
{
    MDK_Error("PixelType is unsigned int, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<long>& InputImage,
                                         double x, double y, double z,
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<long>& Option)
{
    MDK_Error("PixelType is long, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<unsigned long>& InputImage,
                                         double x, double y, double z,
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<unsigned long>& Option)
{
    MDK_Error("PixelType is unsigned long, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<long long>& InputImage,
                                         double x, double y, double z,
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<long long>& Option)
{
    MDK_Error("PixelType is long long, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}


double InterpolateImageAtContinuousIndex(const Image3D<unsigned long long>& InputImage,
                                         double x, double y, double z,
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<unsigned long long>& Option)
{

    MDK_Error("PixelType is unsigned long long, only double or float is supported @ mdkLinearImageInterpolator3D InterpolateImageAtContinuousIndex(...)")
    return 0;
}

}// namespace mdk

#endif