#ifndef __mdkImageFilterWithMultiMask3D_h
#define __mdkImageFilterWithMultiMask3D_h

#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ImageFilterWithMultiMask3D : public ImageFilter3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
    typedef OutputPixelType::ElementType  ElementTypeOfOutputPixel;

protected:
	int_max m_Flag_3DPositionInMask;
	// 1: true
	// 0: false
	// -1: unknown

    ObjectArray<DenseMatrix<double>> m_MaskList;
	// each column has 3 elements
    // dx or dx_Index
    // dy or dy_Index
    // dz or dz_Index
    
	ObjectArray<Image3DBoxRegionOf3DIndex>            m_NOBoundCheckRegionList_3DIndex;

	ObjectArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPosition;

protected:		
	ImageFilterWithMultiMask3D();
	virtual ~ImageFilterWithMultiMask3D();
 
public:
    virtual void Clear();

	ObjectArray<DenseMatrix<double>>& MaskList();
	const ObjectArray<DenseMatrix<double>>& MaskList() const;

	void Use3DIndexInMask();
	void Use3DPhysicalPositionInMask();

protected:
	virtual bool Preprocess();
    virtual void BuildMaskOf3DIndex() {}
    virtual void BuildMaskOf3DPosition() {}
    void ComputeRegionOfNOBoundCheck_3DIndex();
    void ComputeRegionOfNOBoundCheck_3DPosition();

	inline bool WhetherToCheckBoundAtMaskOrigin_3DIndex(double x, double y, double z, int_max MaskIndex);
	inline bool WhetherToCheckBoundAtMaskOrigin_3DPosition(double x, double y, double z, int_max MaskIndex);

private:
	ImageFilterWithMultiMask3D(const ImageFilterWithMultiMask3D&) = delete;
	void operator=(const ImageFilterWithMultiMask3D&) = delete;
};

}//end namespace mdk


#include "mdkImageFilterWithMultiMask3D.hpp"

#endif