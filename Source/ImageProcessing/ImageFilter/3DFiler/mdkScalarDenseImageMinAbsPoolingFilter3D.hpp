#ifndef __mdkScalarDenseImageMinAbsPoolingFilter3D_hpp
#define __mdkScalarDenseImageMinAbsPoolingFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageMinAbsPoolingFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageMinAbsPoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->DenseImageFilterWithSingleMask3D::Clear();
	m_Radius = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetPoolingRadius(ScalarType Radius)
{    
	m_Radius = Radius;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->DenseImageFilterWithSingleMask3D::CheckInput() == false)
	{
		return false;
	}

	if (m_Radius <= 0.0)
	{
		MDK_Error("Radius <= 0.0 @ ScalarDenseImageMinAbsPoolingFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	this->SelectMaskOf3DPhysicalPosition();
	if (this->DenseImageFilterWithSingleMask3D::Preprocess() == false)
	{
		return false;
	}
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DPhysicalPosition()
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
void ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DIndex()
{
	MDK_Error("This is not used @ ScalarDenseImageMinAbsPoolingFilter3D::BuildMask_3DIndex()")
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageMinAbsPoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{
	auto PixelSet = this->GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition<OutputPixelType>(m_Mask_3DPhysicalPosition, x0, y0, z0);
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