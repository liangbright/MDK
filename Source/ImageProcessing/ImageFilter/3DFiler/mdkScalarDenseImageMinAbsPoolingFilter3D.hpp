#ifndef mdk_ScalarDenseImageMinAbsPoolingFilter3D_hpp
#define mdk_ScalarDenseImageMinAbsPoolingFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMinAbsPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMinAbsPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{
	//DataArray<OutputPixelType> PixelSet;
	auto PixelSet = this->GetInputImagePixelByMask_At3DPosition<OutputPixelType>(x0, y0, z0);

	OutputPixelType MinPixel_P = 0;
	OutputPixelType MinPixel_N = 0;
	for (int_max k = 0; k < PixelSet.GetElementNumber(); ++k)
	{
		if (PixelSet[k] >= 0)
		{
			if (MinPixel_P > PixelSet[k])
			{
				MinPixel_P = PixelSet[k];
			}
		}
		else
		{
			if (MinPixel_N < PixelSet[k])
			{
				MinPixel_N = PixelSet[k];
			}
		}
	}

	if (MinPixel_P <= -MinPixel_N)
	{
		return MinPixel_P;
	}
	else
	{
		return MinPixel_N;
	}
}


}//end namespace mdk

#endif