#ifndef __mdkDenseImageFilterWithMultiMask3D_h
#define __mdkDenseImageFilterWithMultiMask3D_h

#include "mdkDenseImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class DenseImageFilterWithMultiMask3D : public DenseImageFilter3D<InputPixel_Type, OutputPixel_Type>
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

    DataArray<DenseMatrix<double>> m_MaskList;
	// each column has 3 elements
    // dx or dx_Index
    // dy or dy_Index
    // dz or dz_Index
    
	DataArray<Image3DBoxRegionOf3DIndex>            m_NOBoundCheckRegionList_3DIndex;

	DataArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPosition;

protected:		
	DenseImageFilterWithMultiMask3D();
	virtual ~DenseImageFilterWithMultiMask3D();
 
public:
    virtual void Clear();

	DataArray<DenseMatrix<double>>& MaskList();
	const DataArray<DenseMatrix<double>>& MaskList() const;

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
	DenseImageFilterWithMultiMask3D(const DenseImageFilterWithMultiMask3D&) = delete;
	void operator=(const DenseImageFilterWithMultiMask3D&) = delete;
};

}//end namespace mdk


#include "mdkDenseImageFilterWithMultiMask3D.hpp"

#endif