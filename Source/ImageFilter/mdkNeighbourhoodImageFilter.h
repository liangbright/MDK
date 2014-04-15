#ifndef __mdkNeighbourhoodImageFilter_h
#define __mdkNeighbourhoodImageFilter_h


#include "mdkImageFilter.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class NeighbourhoodImageFilter : public ImageFilter<VoxelType_Input, VoxelType_Output>
{

protected:
    std::vector<DenseMatrix<double>> m_MaskList_3DIndex;
	// each column has at least 3 elements
    // dx_Index
    // dy_Index
    // dz_Index
    
    // if the filter is a convolution filter, another element (in the 4th row) in each column is 
    // w
    //
	// w is the coefficient at (dx_Index, dy_Index, dz_Index) 

    std::vector<DenseMatrix<double>> m_MaskList_3DPosition;
    // each column has at least 3 elements
    // dx
    // dy
    // dz

    // if the filter is a convolution filter, another element (in the 4th row) in each column is 
    // w
    //
    // w is the coefficient at (dx, dy, dz)

    std::vector<ImageBoxRegionOf3DIndex>    m_NOBoundCheckRegionList_3DIndex;

    std::vector<ImageBoxRegionOf3DPosition> m_NOBoundCheckRegionList_3DPosition;

    ImageDimension      m_InputImageDimension;

    ImagePhysicalSize   m_InputImagePhysicalSize;

    ImagePhysicalOrigin m_InputImagePhysicalOrigin;

    ImageVoxelSpacing   m_InputVoxelPhysicalSize;

protected:		
	NeighbourhoodImageFilter();
	virtual ~NeighbourhoodImageFilter();
 
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

private:
	NeighbourhoodImageFilter(const NeighbourhoodImageFilter&); // Not implemented.
	void operator=(const NeighbourhoodImageFilter&);   // Not implemented.
};

}//end namespace mdk


#include "mdkNeighbourhoodImageFilter.hpp"

#endif