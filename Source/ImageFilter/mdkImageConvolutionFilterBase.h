#ifndef __mdkImageConvolutionFilterBase_h
#define __mdkImageConvolutionFilterBase_h

#include <vector>
#include <array>
#include <string>

#include "mdkObject.h"
#include "mdkDenseMatrix.h"
#include "mdkImage.h"
#include "mdkImageFilter.h"

namespace mdk
{

class ImageConvolutionFilterBase : public Object
{
protected:
    std::vector<DenseMatrix<double>> m_MaskList_3DIndex;
	// each coloum is [dx_Index; dy_Index; dz_Index; w]
	// w is the coefficient at (dx_Index, dy_Index, dz_Index)

    std::vector<DenseMatrix<double>> m_MaskList_3DPosition;
    // each coloum is [dx; dy; dz; w]
    // w is the coefficient at (dx, dy, dz)

    std::vector<ImageBoxRegionOf3DIndex> m_NOBoundCheckRegionList_3DIndex;

    std::vector<ImageBoxRegionOf3DPosition> m_NOBoundCheckRegionList_3DPosition;

    ImageDimension m_InputImageDimension;

    ImagePhysicalSize m_InputImagePhysicalSize;

    ImagePhysicalOrigin m_InputImagePhysicalOrigin;

    ImageVoxelSpacing m_InputVoxelPhysicalSize;

public:		
	ImageConvolutionFilterBase();
	~ImageConvolutionFilterBase();
  
	bool LoadMask(const std::string& FilePathAndName);

	bool SaveMask(const std::string& FilePathAndName);

    bool SetMaskOf3DIndex(const std::vector<DenseMatrix<double>>& MaskList);

    bool SetMaskOf3DPosition(const std::vector<DenseMatrix<double>>& MaskList);

    bool SetMaskOf3DIndex(const DenseMatrix<double>& Mask);

    bool SetMaskOf3DPosition(const DenseMatrix<double>& Mask);

    void ComputeRegionOfNOBoundCheck_3DIndex();

    void ComputeRegionOfNOBoundCheck_3DPosition();

    bool IsMaskOf3DIndexEmpty();

    bool IsMaskOf3DPositionEmpty();

private:
	ImageConvolutionFilterBase(const ImageConvolutionFilterBase&); // Not implemented.
	void operator=(const ImageConvolutionFilterBase&);   // Not implemented.
};

}//end namespace mdk

#endif