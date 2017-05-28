#pragma once

#include <cmath>

namespace mdk
{

//--------- forward declare -----------------------
template<typename PixelType>
class DenseImage3D;

template<typename PixelType>
class SparseImage3D;
//--------------------------------------------------

enum struct MethodEnum_Of_Image3DInterpolation
{
    Nearest,
    Linear,
    //Cubic,
};
typedef MethodEnum_Of_Image3DInterpolation  Image3DInterpolationMethodEnum;

// similar to Boundary Option of imfilter in Matlab
enum struct BoundaryOptionEnum_Of_Image3DInterpolation
{
	Constant, // X
	Replicate,
	//Symmetric
};
typedef BoundaryOptionEnum_Of_Image3DInterpolation Image3DInterpolationBoundaryOptionEnum;

template<typename PixelType>
struct Option_Of_Image3DInterpolation
{
	Image3DInterpolationMethodEnum MethodType = Image3DInterpolationMethodEnum::Linear;
	Image3DInterpolationBoundaryOptionEnum BoundaryOption = Image3DInterpolationBoundaryOptionEnum::Replicate;
	PixelType Pixel_OutsideImage; // it is used only if BoundaryOption is Constant
};
template<typename PixelType>
using Image3DInterpolationOption = Option_Of_Image3DInterpolation<PixelType>;

//====================================== DenseImage3D ===============================================================//

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex(const DenseImage3D<InputPixelType>& InputImage,
									      ScalarType x, ScalarType y, ScalarType z, 
										  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
										  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DPosition(const DenseImage3D<InputPixelType>& InputImage,
                                             ScalarType x, ScalarType y, ScalarType z, 
											 const Option_Of_Image3DInterpolation<InputPixelType>& Option,
											 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition(const DenseImage3D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y, ScalarType z, 
												  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Nearest(const DenseImage3D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y, ScalarType z, 
												  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Linear(const DenseImage3D<InputPixelType>& InputImage,
                                                 ScalarType x, ScalarType y, ScalarType z, 
												 const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Cubic(const DenseImage3D<InputPixelType>& InputImage,
                                                ScalarType x, ScalarType y, ScalarType z, 
											    const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												bool EnableBoundCheck = true);

//====================================== SparseImage3D ===============================================================//

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex(const SparseImage3D<InputPixelType>& InputImage,
										  ScalarType x, ScalarType y, ScalarType z, 
										  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
										  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DPosition(const SparseImage3D<InputPixelType>& InputImage,
                                             ScalarType x, ScalarType y, ScalarType z, 
											 const Option_Of_Image3DInterpolation<InputPixelType>& Option,
											 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition(const SparseImage3D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y, ScalarType z, 
												  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Nearest(const SparseImage3D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y, ScalarType z, 
												  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DPosition_Nearest(const SparseImage3D<InputPixelType>& InputImage,
                                                     ScalarType x, ScalarType y, ScalarType z, 
													 const Option_Of_Image3DInterpolation<InputPixelType>& Option,
													 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Nearest(const SparseImage3D<InputPixelType>& InputImage,
                                                          ScalarType x, ScalarType y, ScalarType z, 
														  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
														  bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Linear(const SparseImage3D<InputPixelType>& InputImage,
                                                 ScalarType x, ScalarType y, ScalarType z, 
												 const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DPosition_Linear(const SparseImage3D<InputPixelType>& InputImage,
                                                    ScalarType x, ScalarType y, ScalarType z, 
												    const Option_Of_Image3DInterpolation<InputPixelType>& Option,
													bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Linear(const SparseImage3D<InputPixelType>& InputImage,
                                                         ScalarType x, ScalarType y, ScalarType z, 
														 const Option_Of_Image3DInterpolation<InputPixelType>& Option,
														 bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Cubic(const SparseImage3D<InputPixelType>& InputImage,
                                                ScalarType x, ScalarType y, ScalarType z, 
												const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DPosition_Cubic(const SparseImage3D<InputPixelType>& InputImage,
								                   ScalarType x, ScalarType y, ScalarType z, 
												   const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												   bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition_Cubic(const SparseImage3D<InputPixelType>& InputImage,
								                        ScalarType x, ScalarType y, ScalarType z, 
														const Option_Of_Image3DInterpolation<InputPixelType>& Option,
														bool EnableBoundCheck = true);
}// namespace mdk

#include "mdkDenseImageInterpolation3D.hpp"
//#include "mdkSparseImageInterpolation3D.hpp"

