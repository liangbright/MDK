#ifndef __mdkNeighbourhoodImage3DFilter_h
#define __mdkNeighbourhoodImage3DFilter_h


#include "mdkImage3DFilter.h"


namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output>
class NeighbourhoodImage3DFilter : public Image3DFilter<PixelType_Input, PixelType_Output>
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

    std::vector<Image3DBoxRegionOf3DIndex>            m_NOBoundCheckRegionList_3DIndex;

    std::vector<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPosition;

    Image3DSize      m_InputImageSize;

    Image3DPhysicalSize   m_InputImagePhysicalSize;

    Image3DPhysicalOrigin m_InputImagePhysicalOrigin;

    Image3DPixelSpacing   m_InputImagePixelSpacing;

protected:		
	NeighbourhoodImage3DFilter();
	virtual ~NeighbourhoodImage3DFilter();
 
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
	NeighbourhoodImage3DFilter(const NeighbourhoodImage3DFilter&); // Not implemented.
	void operator=(const NeighbourhoodImage3DFilter&);   // Not implemented.
};

}//end namespace mdk


#include "mdkNeighbourhoodImage3DFilter.hpp"

#endif