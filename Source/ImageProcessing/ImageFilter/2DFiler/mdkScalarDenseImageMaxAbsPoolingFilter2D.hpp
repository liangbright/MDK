#ifndef mdk_ScalarDenseImageMaxAbsPoolingFilter2D_hpp
#define mdk_ScalarDenseImageMaxAbsPoolingFilter2D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxAbsPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMaxAbsPoolingFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxAbsPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMaxAbsPoolingFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMaxAbsPoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex)
{
	//DataArray<OutputPixelType> PixelSet;
	auto PixelSet = this->GetInputImagePixelByMask_At2DPosition<OutputPixelType>(x0, y0);

	OutputPixelType MaxPixel_P = 0;
	OutputPixelType MaxPixel_N = 0;
	for (int_max k = 0; k < PixelSet.GetElementCount(); ++k)
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