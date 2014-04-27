#ifndef __mdkMaskBasedImageFilter3D_h
#define __mdkMaskBasedImageFilter3D_h


#include "mdkImageFilter3D.h"


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class MaskBasedImageFilter3D : public ImageFilter3D<InputPixelType, OutputPixelType>
{

protected:
    std::vector<DenseMatrix<double>> m_MaskList_3DIndex;
	// each column has at least 3 elements
    // dx_Index
    // dy_Index
    // dz_Index
    
    // if the filter is a convolution filter, the 4th element in each column is 
    // w
    //
	// w is the coefficient at (dx_Index, dy_Index, dz_Index) 

    std::vector<DenseMatrix<double>> m_MaskList_3DPosition;
    // each column has at least 3 elements
    // dx
    // dy
    // dz

    // if the filter is a convolution filter, the 4th element in each column is 
    // w
    //
    // w is the coefficient at (dx, dy, dz)

    std::vector<Image3DBoxRegionOf3DIndex>            m_NOBoundCheckRegionList_3DIndex;

    std::vector<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPosition;

    Image3DSize      m_InputImageSize;

    Image3DPhysicalSize   m_InputImagePhysicalSize;

    Image3DPhysicalOrigin m_InputImagePhysicalOrigin;

    Image3DPixelSpacing   m_InputImagePixelSpacing;

protected:		
	MaskBasedImageFilter3D();
	virtual ~MaskBasedImageFilter3D();
 
public:
	bool LoadMask(const std::string& FilePathAndName);

	bool SaveMask(const std::string& FilePathAndName);

    void SetMaskOf3DIndex(const std::vector<DenseMatrix<double>>& MaskList);

    void SetMaskOf3DPosition(const std::vector<DenseMatrix<double>>& MaskList);

    void SetMaskOf3DIndex(const DenseMatrix<double>& Mask);

    void SetMaskOf3DPosition(const DenseMatrix<double>& Mask);

    void ComputeRegionOfNOBoundCheck_3DIndex();

    void ComputeRegionOfNOBoundCheck_3DPosition();

    bool IsMaskOf3DIndexEmpty();

    bool IsMaskOf3DPositionEmpty();

    virtual void BuildMaskOf3DIndex();    // code the mask in this function

    virtual void BuildMaskOf3DPosition(); // code the mask in this function

    void Clear();

protected:
    virtual bool CheckInput();

    virtual bool Preprocess();

    inline bool IsMaskCenterInsideNOBoundCheckRegion_3DIndex(int_max x, int_max y, int_max z, int_max MaskIndex);

    inline bool IsMaskCenterInsideNOBoundCheckRegion_3DPosition(double x, double y, double z, int_max MaskIndex);

    inline bool WhetherToCheckBoundAtMaskCenter_3DIndex(int_max x, int_max y, int_max z, int_max MaskIndex);

    inline bool WhetherToCheckBoundAtMaskCenter_3DPosition(double x, double y, double z, int_max MaskIndex);

private:
	MaskBasedImageFilter3D(const MaskBasedImageFilter3D&) = delete;
	void operator=(const MaskBasedImageFilter3D&) = delete;
};

}//end namespace mdk


#include "mdkMaskBasedImageFilter3D.hpp"

#endif