#ifndef __mdkScalarDenseImageMinPoolingFilter3D_hpp
#define __mdkScalarDenseImageMinPoolingFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMinPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMinPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMinPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMinPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMinPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{
	DataArray<OutputPixelType> PixelSet;
	if (this->IsPhysicalPositionUsedInSelectedMask() == true)
	{
		PixelSet = this->GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DPhysicalPosition<OutputPixelType>(m_Mask_3DPhysicalPosition, x0, y0, z0);
	}
	else
	{
		PixelSet = this->GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition<OutputPixelType>(m_Mask_3DIndex, x0, y0, z0);
	}

	OutputPixelType MinPixel = PixelSet[0];
	for (int_max k = 1; k < PixelSet.GetElementNumber(); ++k)
	{
		if (MinPixel > PixelSet[k])
		{
			MinPixel = PixelSet[k];
		}
	}
	return MinPixel;
}


}//end namespace mdk

#endif