#ifndef mdk_ScalarDenseImagePoolingFilter3D_hpp
#define mdk_ScalarDenseImagePoolingFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImagePoolingFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImagePoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->DenseImageFilterWithSingleMask3D::Clear();
	m_Radius = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetPoolingRadius(ScalarType Radius)
{    
	m_Radius = Radius;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->DenseImageFilterWithSingleMask3D::CheckInput() == false)
	{
		return false;
	}

	if (m_Radius <= 0.0)
	{
		MDK_Error("Radius <= 0.0 @ ScalarDenseImagePoolingFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DPhysicalPosition()
{
	auto Spacing = m_InputImage->GetSpacing();

	auto MaxRadius_x = double(int_max(m_Radius / Spacing[0]) + 1)*Spacing[0];
	auto MaxRadius_y = double(int_max(m_Radius / Spacing[1]) + 1)*Spacing[1];
	auto MaxRadius_z = double(int_max(m_Radius / Spacing[2]) + 1)*Spacing[2];

	int_max PointNumber_max = int_max((MaxRadius_x / Spacing[0])*(MaxRadius_y / Spacing[1])*(MaxRadius_z / Spacing[2]));

	m_PointMask_3DPhysicalPosition.FastResize(0);
	m_PointMask_3DPhysicalPosition.ReserveCapacity(3 * 8 * PointNumber_max);

	auto Radius_square = m_Radius*m_Radius;

	for (auto z = -MaxRadius_z; z <= MaxRadius_z; z += Spacing[2])
    {
		for (auto y = -MaxRadius_y; y <= MaxRadius_y; y += Spacing[1])
        {
			for (auto x = -MaxRadius_x; x <= MaxRadius_x; x += Spacing[0])
            {
				auto Distance_sq = x*x + y*y + z*z;
				if (Distance_sq <= Radius_square)
                {
					m_PointMask_3DPhysicalPosition.AppendCol({x, y, z});
                }
            }
        }
    }

	m_PointMask_3DPhysicalPosition.ReleaseUnusedCapacity();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DIndex()
{
	auto Spacing = m_InputImage->GetSpacing();

	auto MaxRadius_x = int_max(m_Radius / Spacing[0]) + 1;
	auto MaxRadius_y = int_max(m_Radius / Spacing[1]) + 1;
	auto MaxRadius_z = int_max(m_Radius / Spacing[2]) + 1;
	auto MaxRadius = (std::max)((std::max)(MaxRadius_x, MaxRadius_y), MaxRadius_z);
	
	m_PointMask_3DIndex_InputImage.FastResize(0);
	m_PointMask_3DIndex_InputImage.ReserveCapacity(3 * 8 * MaxRadius*MaxRadius*MaxRadius);

	auto Radius_square = m_Radius*m_Radius;

	for (int_max zIndex = -MaxRadius; zIndex <= MaxRadius; ++zIndex)
	{
		auto z = double(zIndex)*Spacing[2];
		for (int_max yIndex = -MaxRadius; yIndex <= MaxRadius; ++yIndex)
		{
			auto y = double(yIndex)*Spacing[1];
			for (int_max xIndex = -MaxRadius; xIndex <= MaxRadius; ++xIndex)
			{
				auto x = double(xIndex)*Spacing[0];
				auto Distance_sq = x*x + y*y + z*z;
				if (Distance_sq <= Radius_square)
				{
					m_PointMask_3DIndex_InputImage.AppendCol({ xIndex, yIndex, zIndex });
				}
			}
		}
	}

	m_PointMask_3DIndex_InputImage.ReleaseUnusedCapacity();
}

}//end namespace mdk

#endif