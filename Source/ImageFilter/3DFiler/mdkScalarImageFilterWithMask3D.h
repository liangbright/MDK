#ifndef __mdkScalarImageFilterWithMask3D_h
#define __mdkScalarImageFilterWithMask3D_h


#include "mdkScalarImageFilter3D.h"


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class ScalarImageFilterWithMask3D : public ScalarImageFilter3D<InputPixelType, OutputPixelType>
{

protected:
    DenseMatrix<double> m_Mask_3DIndex;
	// each column has at least 3 elements
    // dx_Index
    // dy_Index
    // dz_Index
    
    // if the filter is a convolution filter, the 4th element in each column is 
    // w
    //
	// w is the coefficient at (dx_Index, dy_Index, dz_Index) 

    DenseMatrix<double> m_Mask_3DPosition;
    // each column has at least 3 elements
    // dx
    // dy
    // dz

    // if the filter is a convolution filter, the 4th element in each column is 
    // w
    //
    // w is the coefficient at (dx, dy, dz)

    Image3DBoxRegionOf3DIndex            m_NOBoundCheckRegion_3DIndex;

    Image3DBoxRegionOf3DPhysicalPosition m_NOBoundCheckRegion_3DPosition;

protected:		
	ScalarImageFilterWithMask3D();
	virtual ~ScalarImageFilterWithMask3D();
 
public:

    void SetMaskOf3DIndex(const DenseMatrix<double>& Mask);

    void SetMaskOf3DPosition(const DenseMatrix<double>& Mask);

    const DenseMatrix<double>& GetMaskOf3DIndex() const;

    const DenseMatrix<double>& GetMaskOf3DPosition() const;

    virtual void Clear();

protected:

    virtual void BuildMaskOf3DIndex() {}

    virtual void BuildMaskOf3DPosition() {}

    inline bool WhetherToCheckBoundAtMaskCenter_3DIndex(int_max x, int_max y, int_max z);

    inline bool WhetherToCheckBoundAtMaskCenter_3DPosition(double x, double y, double z);

    virtual bool CheckInput();

    virtual bool Preprocess();

private:
    void ComputeRegionOfNOBoundCheck_3DIndex();

    void ComputeRegionOfNOBoundCheck_3DPosition();

private:
	ScalarImageFilterWithMask3D(const ScalarImageFilterWithMask3D&) = delete;
	void operator=(const ScalarImageFilterWithMask3D&) = delete;
};

}//end namespace mdk


#include "mdkScalarImageFilterWithMask3D.hpp"

#endif