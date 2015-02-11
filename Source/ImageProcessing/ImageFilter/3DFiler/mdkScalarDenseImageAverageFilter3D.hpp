#ifndef mdk_ScalarDenseImageAverageFilter3D_hpp
#define mdk_ScalarDenseImageAverageFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageAverageFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageAverageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType ScalarDenseImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{
	//DataArray<OutputPixelType> PixelSet;
	auto PixelSet = this->GetInputImagePixelByPointMask_At3DPhysicalPosition<OutputPixelType>(x0, y0, z0);

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