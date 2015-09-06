#ifndef mdk_DenseImageFilterWithSingleMaskInInputImage2D_h
#define mdk_DenseImageFilterWithSingleMaskInInputImage2D_h

#include <algorithm>

#include "mdkDebugConfig.h"
#include "mdkImageFilter2D.h"

namespace mdk
{
//Mask is defined in input image space

template<typename InputPixel_Type, typename OutputPixel_Type, typename Scalar_Type = double>
class DenseImageFilterWithSingleMaskInInputImage2D : public ImageFilter3D<DenseImage2D<InputPixel_Type>, DenseImage2D<OutputPixel_Type>, Scalar_Type>
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

	BoxRegionOf2DPositionInImage2D m_NOBoundCheckRegionOf2DPosition;

protected:		
	DenseImageFilterWithSingleMaskInInputImage2D();
	virtual ~DenseImageFilterWithSingleMaskInInputImage2D();

public:
    virtual void Clear(); // must be virtual

private:
	void ClearSelf();

protected:

	virtual bool Preprocess();    // must be virtual
	virtual void BuildMask() {}   // must be virtual

	//3DPosition in input image space 

	inline bool WhetherToCheckBoundAtMaskOrigin_2DPosition(ScalarType x, ScalarType y);

	template<typename NewPixelType = InputPixelType>
	ObjectArray<NewPixelType> GetInputImagePixelByMask_At2DPosition(ScalarType x0, ScalarType y0);

	template<typename NewPixelType = InputPixelType>
	ObjectArray<NewPixelType> GetInputImagePixelByMask_At2DPosition(const DenseVector<ScalarType, 2>& Position);

private:
	void ComputeNOBoundCheckRegionOf2DPosition();

private:
	DenseImageFilterWithSingleMaskInInputImage2D(const DenseImageFilterWithSingleMaskInInputImage2D&) = delete;
	void operator=(const DenseImageFilterWithSingleMaskInInputImage2D&) = delete;
};

}//end namespace mdk


#include "mdkDenseImageFilterWithSingleMaskInInputImage2D.hpp"

#endif