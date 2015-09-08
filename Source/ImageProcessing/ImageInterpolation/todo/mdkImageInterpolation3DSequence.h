#ifndef mdk_ImageInterpolation3DSequence_h
#define mdk_ImageInterpolation3DSequence_h

#include <cmath>

namespace mdk
{

//--------- forward declare -----------------------
template<typename PixelType>
class DenseImage3D;

template<typename PixelType>
class DenseImage3DSequence;

template<typename PixelType>
class SparseImage3D;

template<typename PixelType>
class SparseImage3DSequence;
//--------------------------------------------------

enum struct MethodEnum_Of_Image3DSequenceInterpolation
{
    Nearest,
    Linear,
    //Cubic,
};
typedef MethodEnum_Of_Image3DSequenceInterpolation  Image3DSequenceInterpolationMethodEnum;

// similar to Boundary Option of imfilter in Matlab
enum struct BoundaryOptionEnum_Of_Image3DSequenceInterpolation
{
	Constant, // X
	Replicate,
	//Symmetric
};
typedef BoundaryOptionEnum_Of_Image3DSequenceInterpolation Image3DSequenceInterpolationBoundaryOptionEnum;

template<typename PixelType>
struct Option_Of_Image3DSequenceInterpolation
{
	Image3DSequenceInterpolationMethodEnum MethodType = Image3DSequenceInterpolationMethodEnum::Linear;
	Image3DSequenceInterpolationBoundaryOptionEnum BoundaryOption = Image3DSequenceInterpolationBoundaryOptionEnum::Replicate;
	PixelType Pixel_OutsideImage; // it is used only if BoundaryOption is Constant
};
template<typename PixelType>
using Image3DSequenceInterpolationOption = Option_Of_Image3DSequenceInterpolation<PixelType>;

//====================================== DenseImage3DSequence ===============================================================//

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndexAndTimeIndex(const DenseImage3DSequence<InputPixelType>& InputImage,
									                  ScalarType x, ScalarType y, ScalarType z, int_max t,
													  const Option_Of_Image3DSequenceInterpolation<InputPixelType>& Option,
										              bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DPositionAndTimeIndex(const DenseImage3D<InputPixelType>& InputImage,
                                                         ScalarType x, ScalarType y, ScalarType z, int_max t,
											             const Option_Of_Image3DInterpolation<InputPixelType>& Option,
											             bool EnableBoundCheck = true);

template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPositionAndTimeIndex(const DenseImage3D<InputPixelType>& InputImage,
                                                              ScalarType x, ScalarType y, ScalarType z, int_max t,
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

#include "mdkDenseImageInterpolation3DSequence.hpp"
#include "mdkSparseImageInterpolation3DSequence.hpp"

#endif