#ifndef mdk_DenseImageFilterWithSingleMaskInInputImage3D_h
#define mdk_DenseImageFilterWithSingleMaskInInputImage3D_h

#include <algorithm>

#include "mdkDebugConfig.h"
#include "mdkImageFilter3D.h"

namespace mdk
{
//Mask is defined in input image space

template<typename InputPixel_Type, typename OutputPixel_Type, typename Scalar_Type = double>
class DenseImageFilterWithSingleMaskInInputImage3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

protected:
	DenseMatrix<ScalarType> m_MaskOf3DPosition; // 3DPosition in InputImage
    // row_0: dx
    // row_1: dy
    // row_2: dz

	BoxRegionOf3DPositionInImage3D m_NOBoundCheckRegionOf3DPosition;

protected:		
	DenseImageFilterWithSingleMaskInInputImage3D();
	virtual ~DenseImageFilterWithSingleMaskInInputImage3D();

public:
    virtual void Clear(); // must be virtual

private:
	void ClearSelf();

protected:

	virtual bool Preprocess();    // must be virtual
	virtual void BuildMask() {}   // must be virtual

	//3DPosition in input image space 

	inline bool WhetherToCheckBoundAtMaskOrigin_3DPosition(ScalarType x, ScalarType y, ScalarType z);

	template<typename NewPixelType = InputPixelType>
	ObjectArray<NewPixelType> GetInputImagePixelByMask_At3DPosition(ScalarType x0, ScalarType y0, ScalarType z0);

	template<typename NewPixelType = InputPixelType>
	ObjectArray<NewPixelType> GetInputImagePixelByMask_At3DPosition(const DenseVector<ScalarType, 3>& Position);

private:
	void ComputeNOBoundCheckRegionOf3DPosition();

private:
	DenseImageFilterWithSingleMaskInInputImage3D(const DenseImageFilterWithSingleMaskInInputImage3D&) = delete;
	void operator=(const DenseImageFilterWithSingleMaskInInputImage3D&) = delete;
};

}//end namespace mdk


#include "mdkDenseImageFilterWithSingleMaskInInputImage3D.hpp"

#endif