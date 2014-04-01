#ifndef __mdkVectorVoxelWithFixedSize_h
#define __mdkVectorVoxelWithFixedSize_h

#include <utility>
#include <array>

#include "mdkType.h"
#include "mdkObject.h"

namespace mdk
{
  
template<typename ElementType, int_max Length>
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

    inline int_max GetLength();

    //-----------element access------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

    inline ElementType& operator[](int_max Index);

    inline const ElementType& operator[](int_max Index) const;

    ElementType& operator()(int_max Index);

    const ElementType& operator()(int_max Index) const;

    ElementType& at(int_max Index);

    const ElementType& at(int_max Index) const;

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