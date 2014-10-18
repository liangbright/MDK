#ifndef __mdkScalarDenseImageMaxPoolingFilter3D_hpp
#define __mdkScalarDenseImageMaxPoolingFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMaxPoolingFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMaxPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->DenseImageFilterWithSingleMask3D::Clear();
	m_Radius = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetMaxPoolingRadius(ScalarType Radius)
{    
	m_Radius = Radius;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->DenseImageFilterWithSingleMask3D::CheckInput() == false)
	{
		return false;
	}

	if (m_Radius[0] <= 0.0)
	{
		MDK_Error("Radius <= 0.0 @ ScalarDenseImageMaxPoolingFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	this->SelectMaskOf3DPhysicalPosition();
	if (this->DenseImageFilterWithSingleMask3D::Preprocess() == false)
	{
		return false;
	}
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DPhysicalPosition()
{
	auto InputImageSpacing = m_InputImage->GetSpacing();    
	auto MaxRadius_x = int_max(m_Radius / InputImageSpacing[0]) + 1;
	auto MaxRadius_y = int_max(m_Radius / InputImageSpacing[1]) + 1;
	auto MaxRadius_z = int_max(m_Radius / InputImageSpacing[2]) + 1;

	m_Mask_3DPhysicalPosition.FastResize(0);
	m_Mask_3DPhysicalPosition.ReserveCapacity(3 * MaxRadius_z*MaxRadius_y*MaxRadius_x);

	auto Radius_square = m_Radius*m_Radius;

	for (int_max z_Index = -MaxRadius_z; z_Index <= MaxRadius_z; ++z_Index)
    {
		auto z = ScalarType(InputImageSpacing[2] * double(z_Index));

		for (int_max y_Index = -MaxRadius_y; y_Index <= MaxRadius_y; ++y_Index)
        {
			auto y = ScalarType(InputImageSpacing[1] * double(y_Index));

			for (int_max x_Index = -MaxRadius_x; x_Index <= MaxRadius_x; ++x_Index)
            {
				auto x = ScalarType(InputImageSpacing[0] * double(x_Index));

				auto Distance_sq = x*x + y*y + z*z;

				if (Distance_sq < Radius_square)
                {
					m_Mask_3DPhysicalPosition.AppendCol({x, y, z});
                }
            }
        }
    }

	m_Mask_3DPhysicalPosition.ReleaseUnusedCapacity();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DIndex()
{
	MDK_Error("This is not used @ ScalarDenseImageMaxPoolingFilter3D::BuildMask_3DIndex()")
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMaxPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{
	auto PixelSet = this->GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition<OutputPixelType>(m_Mask_3DPhysicalPosition, x0, y0, z0);
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