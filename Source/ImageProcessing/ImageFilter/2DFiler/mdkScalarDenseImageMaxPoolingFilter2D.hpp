#ifndef mdk_ScalarDenseImageMaxPoolingFilter2D_hpp
#define mdk_ScalarDenseImageMaxPoolingFilter2D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMaxPoolingFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMaxPoolingFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMaxPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex)
{
	//DataArray<OutputPixelType> PixelSet;
	auto PixelSet = this->GetInputImagePixelByMask_At2DPosition<OutputPixelType>(x0, y0);

	OutputPixelType MaxPixel = PixelSet[0];
	for (int_max k = 1; k < PixelSet.GetElementCount(); ++k)
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