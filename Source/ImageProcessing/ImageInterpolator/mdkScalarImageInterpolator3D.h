#ifndef __mdkScalarImageInterpolator3D_h
#define __mdkScalarImageInterpolator3D_h


namespace mdk
{

//--------- forward declare -----------------------
template<typename PixelType>
class Image3D;
//--------------------------------------------------

enum struct ScalarImage3DInterpolationMethodEnum
{
    Nearest,
    Linear,
    //Cubic,
};

enum struct BoundaryOption_Of_ScalarImageInterpolator3D
{
	Constant,
	Nearest
};

struct Option_Of_ScalarImageInterpolator3D
{
	BoundaryOption_Of_ScalarImageInterpolator3D BoundaryOption = BoundaryOption_Of_ScalarImageInterpolator3D::Nearest;
	double PixelValue_Outside = 0;
};


template<typename PixelType>
inline
double InterpolateScalarImageAtContinuousIndex(const Image3D<PixelType>& InputImage,
                                               double x, double y, double z, 
											   ScalarImage3DInterpolationMethodEnum InterpolationMethod,
											   const Option_Of_ScalarImageInterpolator3D& Option);

template<typename PixelType>
inline
double InterpolateScalarImageAtPhysicalPosition(const Image3D<PixelType>& InputImage,
                                                double x, double y, double z, 
                                                ScalarImage3DInterpolationMethodEnum InterpolationMethod,
                                                const Option_Of_ScalarImageInterpolator3D& Option);

template<typename PixelType>
inline
PixelType InterpolateScalarImageAtContinuousIndex_Nearest(const Image3D<PixelType>& InputImage,
                                                          double x, double y, double z, 
                                                          const Option_Of_ScalarImageInterpolator3D& Option);

template<typename PixelType>
inline
PixelType InterpolateScalarImageAtPhysicalPosition_Nearest(const Image3D<PixelType>& InputImage,
                                                           double x, double y, double z, 
                                                           const Option_Of_ScalarImageInterpolator3D& Option);

template<typename PixelType>
inline
double InterpolateScalarImageAtContinuousIndex_Linear(const Image3D<PixelType>& InputImage,
                                                      double x, double y, double z, 
                                                      const Option_Of_ScalarImageInterpolator3D& Option);

template<typename PixelType>
inline
double InterpolateScalarImageAtPhysicalPosition_Linear(const Image3D<PixelType>& InputImage,
                                                       double x, double y, double z, 
                                                       const Option_Of_ScalarImageInterpolator3D& Option);

template<typename PixelType>
inline
double InterpolateScalarImageAtContinuousIndex_Cubic(const Image3D<PixelType>& InputImage,
                                                     double x, double y, double z, 
                                                     const Option_Of_ScalarImageInterpolator3D& Option);

template<typename PixelType>
inline
double InterpolateScalarImageAtPhysicalPosition_Cubic(const Image3D<PixelType>& InputImage,
										              double x, double y, double z, 
												      const Option_Of_ScalarImageInterpolator3D& Option);

}// namespace mdk

#include "mdkScalarImageInterpolator3D.hpp"

#endif