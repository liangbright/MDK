#ifndef __mdkDenseImageFilterWithSingleMask3D_h
#define __mdkDenseImageFilterWithSingleMask3D_h

#include "mdkImageToImageFilter3D.h"
#include "mdkDenseImage3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type, typename Scalar_Type = double>
class DenseImageFilterWithSingleMask3D : public ImageToImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

protected:
	bool m_Flag_PhysicalPositionInMask;
	// true: use 3D physical position in Region
	// false: use 3D Index in Region

	DenseMatrix<ScalarType> m_Mask; // may be continuous index or physical position
    // row_0: dx or dx_Index
    // row_1: dy or dy_Index
    // row_2: dz or dz_Index

    Image3DBoxRegionOf3DIndex                        m_NOBoundCheckRegion_3DIndex;
	Image3DBoxRegionOf3DPhysicalPosition<ScalarType> m_NOBoundCheckRegion_3DPosition;

protected:		
	DenseImageFilterWithSingleMask3D();
	virtual ~DenseImageFilterWithSingleMask3D();
 
public:

    virtual void Clear(); // called in Clear@ProcessObject, must be virtual

	void SetMask_3DIndex(DenseMatrix<ScalarType> Mask);
	void SetMask_3DPosition(DenseMatrix<ScalarType> Mask);

	void Use3DIndexInMask();
	void Use3DPositionInMask();

	const DenseMatrix<ScalarType>& GetMask();

protected:
	virtual bool Preprocess();             // called in Update@DenseImageFilter3D, must be virtual
    virtual void BuildMask_3DIndex() {}    // called in Preprocess@DenseImageFilterWithSingleMask3D, must be virtual
    virtual void BuildMask_3DPosition() {} // called in Preprocess@DenseImageFilterWithSingleMask3D, must be virtual

	inline bool WhetherToCheckBoundAtMaskOrigin_3DIndex(ScalarType x, ScalarType y, ScalarType z);
	inline bool WhetherToCheckBoundAtMaskOrigin_3DPosition(ScalarType x, ScalarType y, ScalarType z);

private:
    void ComputeRegionOfNOBoundCheck_3DIndex();
    void ComputeRegionOfNOBoundCheck_3DPosition();

private:
	DenseImageFilterWithSingleMask3D(const DenseImageFilterWithSingleMask3D&) = delete;
	void operator=(const DenseImageFilterWithSingleMask3D&) = delete;
};

}//end namespace mdk


#include "mdkDenseImageFilterWithSingleMask3D.hpp"

#endif