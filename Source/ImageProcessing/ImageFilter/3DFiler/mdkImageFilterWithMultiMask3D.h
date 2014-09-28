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
    ObjectArray<DenseMatrix<double>>* m_MaskList_3DIndex;
	// each column has 3 elements
    // dx_Index
    // dy_Index
    // dz_Index
    
	ObjectArray<DenseMatrix<double>>* m_MaskList_3DPosition;
    // each column has 3 elements
    // dx
    // dy
    // dz

	ObjectArray<Image3DBoxRegionOf3DIndex>            m_NOBoundCheckRegionList_3DIndex;

	ObjectArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPosition;

protected:		
	ImageFilterWithMultiMask3D();
	virtual ~ImageFilterWithMultiMask3D();
 
public:

    virtual void Clear();

	void SetMaskOf3DIndex(const ObjectArray<DenseMatrix<double>>* MaskList);

	void SetMaskOf3DPosition(const ObjectArray<DenseMatrix<double>>* MaskList);

	const ObjectArray<DenseMatrix<double>>* GetMaskOf3DIndex(const ObjectArray<DenseMatrix<double>>* MaskList) const;

	const ObjectArray<DenseMatrix<double>>* GetMaskOf3DPosition(const ObjectArray<DenseMatrix<double>>* MaskList) const;

    virtual bool CheckInput();

protected:

    virtual void BuildMaskOf3DIndex() {}

    virtual void BuildMaskOf3DPosition() {}

    void ComputeRegionOfNOBoundCheck_3DIndex();

    void ComputeRegionOfNOBoundCheck_3DPosition();

	bool CheckMask();

    virtual bool Preprocess();

    inline bool WhetherToCheckBoundAtMaskOrigin_3DIndex(int_max x, int_max y, int_max z, int_max MaskIndex);

	inline bool WhetherToCheckBoundAtMaskOrigin_3DPosition(double x, double y, double z, int_max MaskIndex);

private:
	ImageFilterWithMultiMask3D(const ImageFilterWithMultiMask3D&) = delete;
	void operator=(const ImageFilterWithMultiMask3D&) = delete;
};

}//end namespace mdk


#include "mdkImageFilterWithMultiMask3D.hpp"

#endif