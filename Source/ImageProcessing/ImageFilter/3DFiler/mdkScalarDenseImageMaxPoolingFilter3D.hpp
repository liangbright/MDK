#ifndef __mdkScalarDenseImageMaxPoolingFilter3D_hpp
#define __mdkScalarDenseImageMaxPoolingFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMaxPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMaxPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{
	DataArray<OutputPixelType> PixelSet;
	if (this->IsPhysicalPositionUsedInSelectedMask() == true)
	{
		PixelSet = this->GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition<OutputPixelType>(m_Mask_3DPhysicalPosition, x0, y0, z0);
	}
	else
	{
		PixelSet = this->GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition<OutputPixelType>(m_Mask_3DIndex, x0, y0, z0);
	}

	OutputPixelType MaxPixel = PixelSet[0];
	for (int_max k = 1; k < PixelSet.GetElementNumber(); ++k)
	{
		if (MaxPixel < PixelSet[k])
		{
			MaxPixel = PixelSet[k];
		}
	}
	return MaxPixel;
}


}//end namespace mdk

#endif