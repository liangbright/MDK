#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseImageFilterWithSingleMaskInInputImage3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImagePoolingFilter3D : public DenseImageFilterWithSingleMaskInInputImage3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

protected:
	ScalarType m_Radius; 

protected:
    ScalarDenseImagePoolingFilter3D();
    virtual ~ScalarDenseImagePoolingFilter3D();
  
public:
	// Sigma in Physical size (mm)
	void SetPoolingRadius(ScalarType Radius);

    virtual void Clear();

private:
	void ClearSelf();
	bool CheckInput();
	void BuildMask();

private:
    ScalarDenseImagePoolingFilter3D(const ScalarDenseImagePoolingFilter3D&) = delete;
    void operator=(const ScalarDenseImagePoolingFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImagePoolingFilter3D.hpp"

