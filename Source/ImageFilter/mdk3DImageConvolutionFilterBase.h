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

struct RegionOfNOBoundCheck_mdk3DImageConvolutionFilter
{
    bool IsEmpty;

    uint64 x0;
    uint64 y0;
    uint64 z0;

    uint64 x1;
    uint64 y1;
    uint64 z1;
};

class mdk3DImageConvolutionFilterBase : public mdkObject
{
protected:
    std::vector<mdkMatrix<double>> m_MaskList;
	// each coloum is [dx; dy; dz; w]
	// w is the coefficient at (dx, dy, dz)

    RegionOfNOBoundCheck_mdk3DImageConvolutionFilter m_RegionOfNOBoundCheck;

public:		
	mdk3DImageConvolutionFilterBase();
	~mdk3DImageConvolutionFilterBase();
  
	bool LoadMask(const std::string& FilePathAndName);

	bool SaveMask(const std::string& FilePathAndName);

	bool SetMask(const std::vector<mdkMatrix<double>>& MaskList);

	bool SetMask(const mdkMatrix<double>& Mask);

    void ComputeRegionOfNOBoundCheck(const uint64 ImageSize[3]);

private:
	mdk3DImageConvolutionFilterBase(const mdk3DImageConvolutionFilterBase&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilterBase&);   // Not implemented.
};

}//end namespace mdk

#endif