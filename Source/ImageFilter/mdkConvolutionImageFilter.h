#ifndef __mdkConvolutionImageFilter_h
#define __mdkConvolutionImageFilter_h

#include <algorithm>

#include "mdkNeighbourhoodImageFilter.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, int_max VectorVoxelLength_Output = 1>
class ConvolutionImageFilter : public NeighbourhoodImageFilter<VoxelType_Input, VoxelType_Output>
{
protected:
    DenseMatrix<VoxelType_Input>* m_OutputVoxelMatrix;

public:		
	ConvolutionImageFilter();
	~ConvolutionImageFilter();

    void SetOutputVoxelMatrix(const DenseMatrix<VoxelType_Input>* VoxelMatrix);

    inline void FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, VoxelType_Output& OutputVoxel);

    inline void FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel);    

protected:
    inline void OutputFunction(int_max OutputVoxelIndex, const VoxelType_Output& OutputVoxel);

private:
    ConvolutionImageFilter(const ConvolutionImageFilter&) = delete;
    void operator=(const ConvolutionImageFilter&) = delete;
};


template<typename VoxelType_Input, typename VoxelType_Output>
class ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, 1> : public NeighbourhoodImageFilter<VoxelType_Input, VoxelType_Output>
{
public:
	ConvolutionImageFilter();
	~ConvolutionImageFilter();

    inline void FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, VoxelType_Output& OutputVoxel);

    inline void FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel);

private:
	ConvolutionImageFilter(const ConvolutionImageFilter&) = delete;
	void operator=(const ConvolutionImageFilter&) = delete;

};

}//end namespace mdk

#include "mdkConvolutionImageFilter.hpp"

#endif