#ifndef __mdkScalarImageToVectorImageFilterWithMask3D_h
#define __mdkScalarImageToVectorImageFilterWithMask3D_h

#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ScalarImageToVectorImageFilterWithMask3D : public ImageFilter3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
    typedef OutputPixelType::ElementType  ElementTypeOfOutputPixel;

protected:
    DataArray<DenseMatrix<double>>* m_MaskList_3DIndex;
	// each column has at least 3 elements
    // dx_Index
    // dy_Index
    // dz_Index
    
    // if the filter is a convolution filter, the 4th element in each column is 
    // w
    //
	// w is the coefficient at (dx_Index, dy_Index, dz_Index) 

    DataArray<DenseMatrix<double>>* m_MaskList_3DPosition;
    // each column has at least 3 elements
    // dx
    // dy
    // dz

    // if the filter is a convolution filter, the 4th element in each column is 
    // w
    //
    // w is the coefficient at (dx, dy, dz)

    DataArray<Image3DBoxRegionOf3DIndex>            m_NOBoundCheckRegionList_3DIndex;

    DataArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPosition;

protected:		
	ScalarImageToVectorImageFilterWithMask3D();
	virtual ~ScalarImageToVectorImageFilterWithMask3D();
 
public:

    virtual void Clear();

    void SetMaskOf3DIndex(const DataArray<DenseMatrix<double>>* MaskList);

    void SetMaskOf3DPosition(const DataArray<DenseMatrix<double>>* MaskList);

    void GetMaskOf3DIndex(const DataArray<DenseMatrix<double>>* MaskList) const;

    void GetMaskOf3DPosition(const DataArray<DenseMatrix<double>>* MaskList) const;

    virtual bool CheckInput();

protected:

    virtual void BuildMaskOf3DIndex() {}

    virtual void BuildMaskOf3DPosition() {}

    void ComputeRegionOfNOBoundCheck_3DIndex();

    void ComputeRegionOfNOBoundCheck_3DPosition();

    virtual bool Preprocess();

    inline bool WhetherToCheckBoundAtMaskCenter_3DIndex(int_max x, int_max y, int_max z, int_max MaskIndex);

    inline bool WhetherToCheckBoundAtMaskCenter_3DPosition(double x, double y, double z, int_max MaskIndex);

private:
	ScalarImageToVectorImageFilterWithMask3D(const ScalarImageToVectorImageFilterWithMask3D&) = delete;
	void operator=(const ScalarImageToVectorImageFilterWithMask3D&) = delete;
};

}//end namespace mdk


#include "mdkScalarImageToVectorImageFilterWithMask3D.hpp"

#endif