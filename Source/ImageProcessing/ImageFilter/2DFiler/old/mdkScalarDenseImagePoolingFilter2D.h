#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseImageFilterWithSingleMaskInInputImage2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImagePoolingFilter2D : public DenseImageFilterWithSingleMaskInInputImage2D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

protected:
	ScalarType m_Radius; 

protected:
    ScalarDenseImagePoolingFilter2D();
    virtual ~ScalarDenseImagePoolingFilter2D();
  
public:
	// Sigma in Physical size (mm)
	void SetPoolingRadius(ScalarType Radius);

    virtual void Clear();

private:
	void ClearSelf();
	bool CheckInput();
	void BuildMask();

private:
    ScalarDenseImagePoolingFilter2D(const ScalarDenseImagePoolingFilter2D&) = delete;
    void operator=(const ScalarDenseImagePoolingFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImagePoolingFilter2D.hpp"

