#ifndef __mdk3DIntegralImageBuilder_h
#define __mdk3DIntegralImageBuilder_h

#include "mdk3DImageFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class mdk3DIntegralImageBuilder : public mdk3DImageFilter<VoxelType_Input, VoxelType_Output>
{

public:		
    mdk3DIntegralImageBuilder();
    ~mdk3DIntegralImageBuilder();
  
    void Clear();

    bool Run();

private:

    bool CheckInput();

    void Compute2DIntegralImage(int64 z_Index_start, int64 z_Index_end);

    void ComputeSumInZDirection(int64 xy_LinearIndex_start, int64 xy_LinearIndex_end);

private:
    mdk3DIntegralImageBuilder(const mdk3DIntegralImageBuilder&); // Not implemented.
    void operator=(const mdk3DIntegralImageBuilder&);            // Not implemented.
};

}//end namespace mdk

#include "mdk3DIntegralImageBuilder.hpp"

#endif