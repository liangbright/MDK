#ifndef __mdkImageInterpolation3D_h
#define __mdkImageInterpolation3D_h

namespace mdk
{

//--------- forward declare -----------------------
template<typename PixelType>
class Image3D;
//--------------------------------------------------

enum struct MethodEnum_Of_Image3DInterpolation
{
    Nearest,
    Linear,
    //Cubic,
};

enum struct BoundaryOptionEnum_Of_Image3DInterpolation
{
	Constant,
	Nearest,
};

template<typename PixelType>
struct Option_Of_Image3DInterpolation
{
	MethodEnum_Of_Image3DInterpolation MethodType = MethodEnum_Of_Image3DInterpolation::Linear;
	BoundaryOptionEnum_Of_Image3DInterpolation BoundaryOption = BoundaryOptionEnum_Of_Image3DInterpolation::Nearest;
	PixelType Pixel_OutsideImage;
};

template<typename InputPixelType, typename OutputPixelType = InputPixelType, typename ScalarType = double>
inline
OutputPixelType InterpolateImageAt3DContinuousIndex(const Image3D<InputPixelType>& InputImage,
										            ScalarType x, ScalarType y, ScalarType z, 
													const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

template<typename InputPixelType, typename OutputPixelType = InputPixelType, typename ScalarType = double>
inline
OutputPixelType InterpolateImageAt3DPhysicalPosition(const Image3D<InputPixelType>& InputImage,
                                                     ScalarType x, ScalarType y, ScalarType z, 
											         const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

template<typename InputPixelType, typename OutputPixelType = InputPixelType, typename ScalarType = double>
inline
OutputPixelType InterpolateImageAt3DContinuousIndex_Nearest(const Image3D<InputPixelType>& InputImage,
                                                            ScalarType x, ScalarType y, ScalarType z, 
													        const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

template<typename InputPixelType, typename OutputPixelType = InputPixelType, typename ScalarType = double>
inline
OutputPixelType InterpolateImageAt3DPhysicalPosition_Nearest(const Image3D<InputPixelType>& InputImage,
                                                             ScalarType x, ScalarType y, ScalarType z, 
													         const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

template<typename InputPixelType, typename OutputPixelType = InputPixelType, typename ScalarType = double>
inline
OutputPixelType InterpolateImageAt3DContinuousIndex_Linear(const Image3D<InputPixelType>& InputImage,
                                                           ScalarType x, ScalarType y, ScalarType z, 
													       const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

template<typename InputPixelType, typename OutputPixelType = InputPixelType, typename ScalarType = double>
inline
OutputPixelType InterpolateImageAt3DPhysicalPosition_Linear(const Image3D<InputPixelType>& InputImage,
                                                            ScalarType x, ScalarType y, ScalarType z, 
													        const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

template<typename InputPixelType, typename OutputPixelType = InputPixelType, typename ScalarType = double>
inline
OutputPixelType InterpolateImageAt3DContinuousIndex_Cubic(const Image3D<InputPixelType>& InputImage,
                                                          ScalarType x, ScalarType y, ScalarType z, 
													      const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

template<typename InputPixelType, typename OutputPixelType = InputPixelType, typename ScalarType = double>
inline
OutputPixelType InterpolateImageAt3DPhysicalPosition_Cubic(const Image3D<InputPixelType>& InputImage,
								                           ScalarType x, ScalarType y, ScalarType z, 
													       const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

}// namespace mdk

#include "mdkImageInterpolation3D.hpp"

#endif