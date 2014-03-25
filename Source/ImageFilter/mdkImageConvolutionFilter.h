#ifndef __mdkImageConvolutionFilter_h
#define __mdkImageConvolutionFilter_h

#include <vector>
#include <array>
#include <algorithm>


#include "mdkDebugConfig.h"
#include "mdkObject.h"
#include "mdkDenseMatrix.h"
#include "mdkImage.h"
#include "mdkImageFilter.h"
#include "mdkImageConvolutionFilterBase.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output = 1>
class ImageConvolutionFilter : public ImageFilter<VoxelType_Input, VoxelType_Output>, public ImageConvolutionFilterBase
{
protected:
    DenseMatrix<VoxelType_Input>* m_OutputVoxelMatrix;

public:		
	ImageConvolutionFilter();
	~ImageConvolutionFilter();

    void SetOutputVoxelMatrix(const DenseMatrix<VoxelType_Input>* VoxelMatrix);

    inline void FilterFunctionAt3DIndex(int64 x_Index, int64 y_Index, int64 z_Index, VoxelType_Output& OutputVoxel);

    inline void FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel);

protected:
    bool Preprocess();

    virtual void BuildMaskOf3DIndex();

    virtual void BuildMaskOf3DPosition();

    inline void OutputFunction(int64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel);

private:
	ImageConvolutionFilter(const ImageConvolutionFilter&); // Not implemented.
	void operator=(const ImageConvolutionFilter&);              // Not implemented.
};


template<typename VoxelType_Input, typename VoxelType_Output>
class ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1> : public ImageFilter<VoxelType_Input, VoxelType_Output>, public ImageConvolutionFilterBase
{
public:
	ImageConvolutionFilter();
	~ImageConvolutionFilter();

    inline void FilterFunctionAt3DIndex(int64 x_Index, int64 y_Index, int64 z_Index, VoxelType_Output& OutputVoxel);

    inline void FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel);

protected:
    bool Preprocess();

    virtual void BuildMaskOf3DIndex();

    virtual void BuildMaskOf3DPosition();

private:
	ImageConvolutionFilter(const ImageConvolutionFilter&); // Not implemented.
	void operator=(const ImageConvolutionFilter&);         // Not implemented.

};

}//end namespace mdk

#include "mdkImageConvolutionFilter.hpp"

#endif