#ifndef mdk_ScalarDenseImagePoolingFilter3D_h
#define mdk_ScalarDenseImagePoolingFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
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
	bool CheckInput();
    void BuildMask_3DIndex();
	void BuildMask_3DPhysicalPosition();

private:
    ScalarDenseImagePoolingFilter3D(const ScalarDenseImagePoolingFilter3D&) = delete;
    void operator=(const ScalarDenseImagePoolingFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImagePoolingFilter3D.hpp"

#endif