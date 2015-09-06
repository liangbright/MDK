#ifndef mdk_ScalarDenseImageMinPoolingFilter2D_hpp
#define mdk_ScalarDenseImageMinPoolingFilter2D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMinPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMinPoolingFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMinPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMinPoolingFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMinPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex)
{
	//DataArray<OutputPixelType> PixelSet;
	auto PixelSet = this->GetInputImagePixelByMask_At2DPosition<OutputPixelType>(x0, y0);

	OutputPixelType MinPixel = PixelSet[0];
	for (int_max k = 1; k < PixelSet.GetElementCount(); ++k)
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