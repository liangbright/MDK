#ifndef mdk_ScalarDenseImagePoolingFilter3D_hpp
#define mdk_ScalarDenseImagePoolingFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImagePoolingFilter3D()
{
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImagePoolingFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->DenseImageFilterWithSingleMaskInInputImage3D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_Radius = 0;
	this->SelectPhysicalCoordinateSystemForEvaluation(PhysicalCoordinateSystemForEvaluation::INPUT);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetPoolingRadius(ScalarType Radius)
{    
	m_Radius = Radius;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->DenseImageFilterWithSingleMaskInInputImage3D::CheckInput() == false)
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
void ScalarDenseImagePoolingFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask()
{
	auto Spacing = m_InputImage->GetSpacing();

	auto MaxRadius_x = double(int_max(m_Radius / Spacing[0]) + 1)*Spacing[0];
	auto MaxRadius_y = double(int_max(m_Radius / Spacing[1]) + 1)*Spacing[1];
	auto MaxRadius_z = double(int_max(m_Radius / Spacing[2]) + 1)*Spacing[2];

	int_max PointCount_max = 4*int_max((MaxRadius_x / Spacing[0])*(MaxRadius_y / Spacing[1])*(MaxRadius_z / Spacing[2]));
	
	m_MaskOf3DPosition.FastResize(0);
	m_MaskOf3DPosition.ReserveCapacity(3 * PointCount_max);

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
					m_MaskOf3DPosition.AppendCol({x, y, z});
                }
            }
        }
    }

	m_MaskOf3DPosition.ReleaseUnusedCapacity();
}

}//end namespace mdk

#endif