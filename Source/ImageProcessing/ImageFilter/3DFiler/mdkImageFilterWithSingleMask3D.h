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

    // input_0:
	const DenseMatrix<double>* m_Mask_3DIndex; // may be continuous index
    // row_0: dx_Index
    // row_1: dy_Index
    // row_2: dz_Index

    // input_1:
    const DenseMatrix<double>* m_Mask_3DPosition;
    // row_0: dx
    // row_1: dy
    // row_2: dz

    Image3DBoxRegionOf3DIndex            m_NOBoundCheckRegion_3DIndex;

    Image3DBoxRegionOf3DPhysicalPosition m_NOBoundCheckRegion_3DPosition;

protected:		
	ImageFilterWithSingleMask3D();
	virtual ~ImageFilterWithSingleMask3D();
 
public:

    virtual void Clear();

	void SetMaskOf3DIndex(const DenseMatrix<double>* Mask);

    void SetMaskOf3DPosition(const DenseMatrix<double>* Mask);

    const DenseMatrix<double>* GetMaskOf3DIndex() const;

    const DenseMatrix<double>* GetMaskOf3DPosition() const;

protected:

    virtual void BuildMaskOf3DIndex() {}

    virtual void BuildMaskOf3DPosition() {}

	inline bool WhetherToCheckBoundAtMaskOrigin_3DIndex(double x, double y, double z);

	inline bool WhetherToCheckBoundAtMaskOrigin_3DPosition(double x, double y, double z);

    bool CheckMask();

    virtual bool Preprocess();

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