#pragma once

#include <cmath>

namespace mdk
{

//--------- forward declare -----------------------
template<typename PixelType>
class DenseImage2D;

template<typename PixelType>
class SparseImage2D;
//--------------------------------------------------

enum struct MethodEnum_Of_Image2DInterpolation
{
    Nearest,
    Linear,
    //Cubic,
};
typedef MethodEnum_Of_Image2DInterpolation  Image2DInterpolationMethodEnum;

// similar to Boundary Option of imfilter in Matlab
enum struct BoundaryOptionEnum_Of_Image2DInterpolation
{
	Constant, // X
	Replicate,
	//Symmetric
};
typedef BoundaryOptionEnum_Of_Image2DInterpolation Image2DInterpolationBoundaryOptionEnum;

template<typename PixelType>
struct Option_Of_Image2DInterpolation
{
	Image2DInterpolationMethodEnum MethodType = Image2DInterpolationMethodEnum::Linear;
	Image2DInterpolationBoundaryOptionEnum BoundaryOption = Image2DInterpolationBoundaryOptionEnum::Replicate;
	PixelType Pixel_OutsideImage;
};
template<typename PixelType>
using Image2DInterpolationOption = Option_Of_Image2DInterpolation<PixelType>;

//====================================== DenseImage2D ===============================================================//

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex(const DenseImage2D<InputPixelType>& InputImage,
										  ScalarType x, ScalarType y,
										  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
										  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition(const DenseImage2D<InputPixelType>& InputImage,
										     ScalarType x, ScalarType y,
										     const Option_Of_Image2DInterpolation<InputPixelType>& Option,
											 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition(const DenseImage2D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y, ScalarType z,
												  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Nearest(const DenseImage2D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y,
												  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition_Nearest(const DenseImage2D<InputPixelType>& InputImage,
                                                     ScalarType x, ScalarType y,
												     const Option_Of_Image2DInterpolation<InputPixelType>& Option,
													 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Nearest(const DenseImage2D<InputPixelType>& InputImage,
                                                          ScalarType x, ScalarType y, ScalarType z,
														  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
														  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Linear(const DenseImage2D<InputPixelType>& InputImage,
                                                 ScalarType x, ScalarType y,
												 const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition_Linear(const DenseImage2D<InputPixelType>& InputImage,
                                                    ScalarType x, ScalarType y,
												    const Option_Of_Image2DInterpolation<InputPixelType>& Option,
													bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Linear(const DenseImage2D<InputPixelType>& InputImage,
                                                         ScalarType x, ScalarType y, ScalarType z, 
														 const Option_Of_Image2DInterpolation<InputPixelType>& Option,
														 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Cubic(const DenseImage2D<InputPixelType>& InputImage,
                                                ScalarType x, ScalarType y,
												const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition_Cubic(const DenseImage2D<InputPixelType>& InputImage,
											       ScalarType x, ScalarType y,
                                                   const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												   bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Cubic(const DenseImage2D<InputPixelType>& InputImage,
								                        ScalarType x, ScalarType y, ScalarType z,
														const Option_Of_Image2DInterpolation<InputPixelType>& Option,
														bool EnableBoundCheck = true);

//====================================== SparseImage2D ===============================================================//

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex(const SparseImage2D<InputPixelType>& InputImage,
										  ScalarType x, ScalarType y,
										  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
										  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition(const SparseImage2D<InputPixelType>& InputImage,
										     ScalarType x, ScalarType y,
										     const Option_Of_Image2DInterpolation<InputPixelType>& Option,
											 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition(const SparseImage2D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y, ScalarType z,
												  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Nearest(const SparseImage2D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y,
												  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition_Nearest(const SparseImage2D<InputPixelType>& InputImage,
                                                     ScalarType x, ScalarType y,
												     const Option_Of_Image2DInterpolation<InputPixelType>& Option,
													 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Nearest(const SparseImage2D<InputPixelType>& InputImage,
                                                          ScalarType x, ScalarType y, ScalarType z,
														  const Option_Of_Image2DInterpolation<InputPixelType>& Option,
														  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Linear(const SparseImage2D<InputPixelType>& InputImage,
                                                 ScalarType x, ScalarType y,
												 const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition_Linear(const SparseImage2D<InputPixelType>& InputImage,
                                                    ScalarType x, ScalarType y,
												    const Option_Of_Image2DInterpolation<InputPixelType>& Option,
													bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Linear(const SparseImage2D<InputPixelType>& InputImage,
                                                         ScalarType x, ScalarType y, ScalarType z, 
														 const Option_Of_Image2DInterpolation<InputPixelType>& Option,
														 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DIndex_Cubic(const SparseImage2D<InputPixelType>& InputImage,
                                                ScalarType x, ScalarType y,
												const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt2DPosition_Cubic(const SparseImage2D<InputPixelType>& InputImage,
											       ScalarType x, ScalarType y,
                                                   const Option_Of_Image2DInterpolation<InputPixelType>& Option,
												   bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Cubic(const SparseImage2D<InputPixelType>& InputImage,
								                        ScalarType x, ScalarType y, ScalarType z,
														const Option_Of_Image2DInterpolation<InputPixelType>& Option,
														bool EnableBoundCheck = true);

}// namespace mdk

#include "mdkDenseImageInterpolation2D.hpp"
//#include "mdkSparseImageInterpolation2D.hpp"

