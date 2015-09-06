#ifndef mdk_ScalarDenseImageAverageFilter2D_hpp
#define mdk_ScalarDenseImageAverageFilter2D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageAverageFilter2D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageAverageFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType ScalarDenseImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex)
{
	//DataArray<OutputPixelType> PixelSet;
	auto PixelSet = this->GetInputImagePixelByPointMask_At2DPhysicalPosition<OutputPixelType>(x0, y0);

	ScalarType AveragePixel = PixelSet[0];
	for (int_max k = 1; k < PixelSet.GetElementNumber(); ++k)
	{
			AveragePixel += PixelSet[k];
	}
	AveragePixel /= ScalarType(PixelSet.GetElementNumber());

	return OutputPixelType(AveragePixel);
}

}//end namespace mdk

#endif