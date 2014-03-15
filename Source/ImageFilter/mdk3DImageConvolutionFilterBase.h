#ifndef __mdk3DImageConvolutionFilterBase_h
#define __mdk3DImageConvolutionFilterBase_h

#include <vector>
#include <array>
#include <string>

#include "mdkObject.h"
#include "mdkMatrix.h"
#include "mdk3DImage.h"
#include "mdk3DImageFilter.h"

namespace mdk
{

struct RegionOfNOBoundCheck_3DIndex
{
    bool IsEmpty;

    int64 x0_Index;
    int64 y0_Index;
    int64 z0_Index;

    int64 x1_Index;
    int64 y1_Index;
    int64 z1_Index;
};

struct RegionOfNOBoundCheck_3DPosition
{
    bool IsEmpty;

    double x0;
    double y0;
    double z0;

    double x1;
    double y1;
    double z1;
};

class mdk3DImageConvolutionFilterBase : public mdkObject
{
protected:
    std::vector<mdkMatrix<double>> m_MaskList_3DIndex;
	// each coloum is [dx_Index; dy_Index; dz_Index; w]
	// w is the coefficient at (dx_Index, dy_Index, dz_Index)

    std::vector<mdkMatrix<double>> m_MaskList_3DPosition;
    // each coloum is [dx; dy; dz; w]
    // w is the coefficient at (dx, dy, dz)

    std::vector<RegionOfNOBoundCheck_3DIndex> m_NOBoundCheckRegionList_3DIndex;

    std::vector<RegionOfNOBoundCheck_3DPosition> m_NOBoundCheckRegionList_3DPosition;

    int64 m_InputImageDimension[3];

    double m_InputImagePhysicalSize[3];

    double m_InputImagePhysicalOrigin[3];

    double m_InputVoxelPhysicalSize[3];

public:		
	mdk3DImageConvolutionFilterBase();
	~mdk3DImageConvolutionFilterBase();
  
	bool LoadMask(const std::string& FilePathAndName);

	bool SaveMask(const std::string& FilePathAndName);

    bool SetMaskOf3DIndex(const std::vector<mdkMatrix<double>>& MaskList);

    bool SetMaskOf3DPosition(const std::vector<mdkMatrix<double>>& MaskList);

    bool SetMaskOf3DIndex(const mdkMatrix<double>& Mask);

    bool SetMaskOf3DPosition(const mdkMatrix<double>& Mask);

    void ComputeRegionOfNOBoundCheck_3DIndex();

    void ComputeRegionOfNOBoundCheck_3DPosition();

    bool IsMaskOf3DIndexEmpty();

    bool IsMaskOf3DPositionEmpty();

private:
	mdk3DImageConvolutionFilterBase(const mdk3DImageConvolutionFilterBase&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilterBase&);   // Not implemented.
};

}//end namespace mdk

#endif