#ifndef __mdkScalarDenseImageMaxAbsPoolingFilter3D_hpp
#define __mdkScalarDenseImageMaxAbsPoolingFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMaxAbsPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMaxAbsPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMaxAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::
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

	OutputPixelType MaxPixel_P = 0;
	OutputPixelType MaxPixel_N = 0;
	for (int_max k = 0; k < PixelSet.GetElementNumber(); ++k)
	{
		if (PixelSet[k] >= 0)
		{
			if (MaxPixel_P < PixelSet[k])
			{
				MaxPixel_P = PixelSet[k];
			}
		}
		else
		{
			if (MaxPixel_N > PixelSet[k])
			{
				MaxPixel_N = PixelSet[k];
			}
		}
	}

	if (MaxPixel_P >= -MaxPixel_N)
	{
		return MaxPixel_P;
	}
	else
	{
		return MaxPixel_N;
	}
}


}//end namespace mdk

#endif