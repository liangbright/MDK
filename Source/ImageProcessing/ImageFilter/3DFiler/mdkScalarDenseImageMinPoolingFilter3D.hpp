#ifndef mdk_ScalarDenseImageMinPoolingFilter3D_hpp
#define mdk_ScalarDenseImageMinPoolingFilter3D_hpp

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
	//DataArray<OutputPixelType> PixelSet;
	auto PixelSet = this->GetInputImagePixelByPointMask_At3DPhysicalPosition<OutputPixelType>(x0, y0, z0);

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