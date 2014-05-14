#ifndef __mdkScalarImageInterpolator3D_h
#define __mdkScalarImageInterpolator3D_h


namespace mdk
{
//============================================= PixelType must be float or dobule ========================================================================//

//--------- forward declare -----------------------
template<typename PixelType>
class ScalarImage3D;
//--------------------------------------------------

enum struct ImageInterpolation3DMethodTypeEnum
{
    Nearest,
    Linear,
    //Cubic,
};


template<typename PixelType>
struct Option_Of_ImageInterpolator3D
{
    PixelType PixelValueToFillIfOutOfBound = PixelType(0);
};


template<typename PixelType>
inline
double InterpolateImageAtContinuousIndex(const Image3D<PixelType>& InputImage,
                                         double x, double y, double z, 
                                         ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                         const Option_Of_ImageInterpolator3D<PixelType>& Option);

template<typename PixelType>
inline
double InterpolateImageAtPhysicalPosition(const Image3D<PixelType>& InputImage,
                                          double x, double y, double z, 
                                          ImageInterpolation3DMethodTypeEnum InterpolationMethod,
                                          const Option_Of_ImageInterpolator3D<PixelType>& Option);

template<typename PixelType>
inline
double InterpolateImageAtContinuousIndex_Nearest(const Image3D<PixelType>& InputImage,
                                                 double x, double y, double z, 
                                                 const Option_Of_ImageInterpolator3D<PixelType>& Option);

template<typename PixelType>
inline
double InterpolateImageAtPhysicalPosition_Nearest(const Image3D<PixelType>& InputImage,
                                                  double x, double y, double z, 
                                                  const Option_Of_ImageInterpolator3D<PixelType>& Option);

template<typename PixelType>
inline
double InterpolateImageAtContinuousIndex_Linear(const Image3D<PixelType>& InputImage,
                                                double x, double y, double z, 
                                                const Option_Of_ImageInterpolator3D<PixelType>& Option);

template<typename PixelType>
inline
double InterpolateImageAtPhysicalPosition_Linear(const Image3D<PixelType>& InputImage,
                                                 double x, double y, double z, 
                                                 const Option_Of_ImageInterpolator3D<PixelType>& Option);

template<typename PixelType>
inline
double InterpolateImageAtContinuousIndex_Cubic(const ScalarImage3D<PixelType>& InputImage,
                                               double x, double y, double z, 
                                               const Option_Of_ImageInterpolator3D<PixelType>& Option);

template<typename PixelType>
inline
double InterpolateImageAtPhysicalPosition_Cubic(const Image3D<PixelType>& InputImage,
                                                double x, double y, double z, 
                                                const Option_Of_ImageInterpolator3D<PixelType>& Option);


}// namespace mdk

#include "mdkScalarImageInterpolator3D.hpp"
#include "mdkScalarImageInterpolator3D_WrongPixelType.hpp"

#endif