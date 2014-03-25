#ifndef __mdkVectorVoxelWithFixedSize_h
#define __mdkVectorVoxelWithFixedSize_h

#include <utility>
#include <array>

#include "mdkType.h"
#include "mdkObject.h"

namespace mdk
{
  
template<typename ElementType, int64 Length>
class VectorVoxelWithFixedSize : public Object
{
private:
    std::array<ElementType, Length> m_ElementData;

    ElementType m_ZeroElement;

public:
    
    inline VectorVoxelWithFixedSize();

    inline VectorVoxelWithFixedSize(const ElementType& Element);

    inline VectorVoxelWithFixedSize(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel) = delete;

    inline VectorVoxelWithFixedSize(VectorVoxelWithFixedSize<ElementType, Length>&& Voxel);

    inline ~VectorVoxelWithFixedSize();

    inline void operator=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    inline void operator=(VectorVoxelWithFixedSize<ElementType, Length>&& Voxel);

    inline void Clear();

    inline void Fill(const ElementType& Element);

    inline uint64 GetLength();

    //-----------element access------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

    inline ElementType& operator[](uint64 Index);

    inline const ElementType& operator[](uint64 Index) const;

    ElementType& operator()(uint64 Index);

    const ElementType& operator()(uint64 Index) const;

    ElementType& at(uint64 Index);

    const ElementType& at(uint64 Index) const;

    //------------ operator += -= *= /= -------------------//

    //---------------------------------------------------- Voxel {+= -= *= /=} Voxel ---------------------------------------------// 

    inline void operator+=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    inline void operator-=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    inline void operator*=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    inline void operator/=(const VectorVoxelWithFixedSize<ElementType, Length>& Voxel);

    //---------------------------------------------------- Voxel {+= -= *= /=} Element ---------------------------------------------// 

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

};


}// namespace mdk

#include "mdkVectorVoxelWithFixedSize.hpp"

#endif