#ifndef __mdk3DImageConvolutionFilter_h
#define __mdk3DImageConvolutionFilter_h

#include <vector>
#include <array>

#include "mdkObject.h"
#include "mdkMatrix.h"
#include "mdk3DImage.h"
#include "mdk3DImageFilter.h"
#include "mdk3DImageConvolutionFilterBase.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output = 1>
class mdk3DImageConvolutionFilter : public mdk3DImageFilter<VoxelType_Input, VoxelType_Output>, public mdk3DImageConvolutionFilterBase
{
private:
    mdkMatrix<VoxelType_Input>* m_OutputVoxelMatrix;

public:		
	mdk3DImageConvolutionFilter();
	~mdk3DImageConvolutionFilter();
  
    void SetOutputVoxelMatrix(const mdkMatrix<uint64>* VoxelMatrix);

    bool Preprocess();

	inline void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel);

    inline void OutputFunction(uint64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel);


private:
	mdk3DImageConvolutionFilter(const mdk3DImageConvolutionFilter&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilter&);              // Not implemented.
};


template<typename VoxelType_Input, typename VoxelType_Output>
class mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1> : public mdk3DImageFilter<VoxelType_Input, VoxelType_Output>, public mdk3DImageConvolutionFilterBase
{
public:
	mdk3DImageConvolutionFilter();
	~mdk3DImageConvolutionFilter();

    bool Preprocess();

    inline void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel);

private:
	mdk3DImageConvolutionFilter(const mdk3DImageConvolutionFilter&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilter&);              // Not implemented.

};

}//end namespace mdk

#include "mdk3DImageConvolutionFilter.hpp"

#endif