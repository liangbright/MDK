#ifndef __mdk3DImageVectorVoxelWithFixedSize_h
#define __mdk3DImageVectorVoxelWithFixedSize_h

#include <utility>
#include <array>

#include "mdkType.h"
#include "mdkObject.h"

namespace mdk
{
  
template<typename ElementType, uint64 Length>
class mdk3DImageVectorVoxelWithFixedSize : public mdkObject
{
private:
    std::array<ElementType, Length> m_ElementData;

    ElementType m_ZeroElement;

public:
    
    inline mdk3DImageVectorVoxelWithFixedSize();

    inline mdk3DImageVectorVoxelWithFixedSize(const ElementType& Element);

    inline mdk3DImageVectorVoxelWithFixedSize(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel) = delete;

    inline mdk3DImageVectorVoxelWithFixedSize(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>&& Voxel);

    inline ~mdk3DImageVectorVoxelWithFixedSize();

    inline void operator=(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    inline void operator=(mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>&& Voxel);

    inline void Reset();

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

    //------------ operator += -= *= /= -------------------//

    //---------------------------------------------------- Voxel {+= -= *= /=} Voxel ---------------------------------------------// 

    inline void operator+=(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    inline void operator-=(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    inline void operator*=(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    inline void operator/=(const mdk3DImageVectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    //---------------------------------------------------- Voxel {+= -= *= /=} Element ---------------------------------------------// 

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

};


}// namespace mdk

#include "mdk3DImageVectorVoxelWithFixedSize.hpp"

#endif