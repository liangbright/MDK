#ifndef __mdkImageFilterWithSingleMask3D_h
#define __mdkImageFilterWithSingleMask3D_h

#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ImageFilterWithSingleMask3D : public ImageFilter3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

protected:
	int_max m_Flag_3DPositionInMask;
	// 1: use 3D physical position in Region
	// 0: use 3D Index in Region
	// -1: unknown

	DenseMatrix<double> m_Mask; // may be continuous index or physical position
    // row_0: dx or dx_Index
    // row_1: dy or dy_Index
    // row_2: dz or dz_Index

    Image3DBoxRegionOf3DIndex            m_NOBoundCheckRegion_3DIndex;
    Image3DBoxRegionOf3DPhysicalPosition m_NOBoundCheckRegion_3DPosition;

protected:		
	ImageFilterWithSingleMask3D();
	virtual ~ImageFilterWithSingleMask3D();
 
public:

    virtual void Clear();

	void SetMask(DenseMatrix<double> Mask);
	const DenseMatrix<double>& GetMask() const;

	DenseMatrix<double>& Mask();
	const DenseMatrix<double>& Mask() const;

	void Use3DIndexInMask();
	void Use3DPhysicalPositionInMask();

protected:
	virtual bool Preprocess();
    virtual void BuildMaskOf3DIndex() {}
    virtual void BuildMaskOf3DPosition() {}

	inline bool WhetherToCheckBoundAtMaskOrigin_3DIndex(double x, double y, double z);
	inline bool WhetherToCheckBoundAtMaskOrigin_3DPosition(double x, double y, double z);

private:
    void ComputeRegionOfNOBoundCheck_3DIndex();
    void ComputeRegionOfNOBoundCheck_3DPosition();

private:
	ImageFilterWithSingleMask3D(const ImageFilterWithSingleMask3D&) = delete;
	void operator=(const ImageFilterWithSingleMask3D&) = delete;
};

}//end namespace mdk


#include "mdkImageFilterWithSingleMask3D.hpp"

#endif