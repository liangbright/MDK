#ifndef __mdk3DImageVectorVoxelWithVariableSize_h
#define __mdk3DImageVectorVoxelWithVariableSize_h

#include <utility>
#include <vector>

#include "mdkType.h"
#include "mdkObject.h"

namespace mdk
{
  
template<typename ElementType>
class mdk3DImageVectorVoxelWithVariableSize : public mdkObject
{
private:
    std::vector<ElementType> m_ElementData;

    ElementType m_ZeroElement;

public:
    
    inline mdk3DImageVectorVoxelWithVariableSize();

    inline mdk3DImageVectorVoxelWithVariableSize(const ElementType& Element, int64 Length = 1);

    inline mdk3DImageVectorVoxelWithVariableSize(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel) = delete;

    inline mdk3DImageVectorVoxelWithVariableSize(mdk3DImageVectorVoxelWithVariableSize<ElementType, Length>&& Voxel);

    inline ~mdk3DImageVectorVoxelWithVariableSize();

    inline void operator=(const mdk3DImageVectorVoxelWithVariableSize<ElementType, Length>& Voxel);

    inline void operator=(mdk3DImageVectorVoxelWithVariableSize<ElementType, Length>&& Voxel);

    inline void Reset(); // set to empty

    inline void Fill(const ElementType& Element);

    inline uint64 GetLength();

    //-----------element access------------------//

    inline ElementType* GetElementDataRawPointer();

    inline const ElementType* GetElementDataRawPointer() const;

    inline ElementType& operator[](uint64 Index);

    inline const ElementType& operator[](uint64 Index) const;

    ElementType& operator()(uint64 Index);

    const ElementType& operator()(uint64 Index) const;

    ElementType& at(uint64 Index);

    const ElementType& at(uint64 Index) const;

    // ------------------------------------------------------------------------------------------------------------//

    inline void operator+=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);

    inline void operator-=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);

    inline void operator*=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);

    inline void operator/=(const mdk3DImageVectorVoxelWithVariableSize<ElementType>& Voxel);

    //---------------------------------------------------------------------------------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

};


}// namespace mdk

#include "mdk3DImageVectorVoxelWithVariableSize.hpp"

#endif