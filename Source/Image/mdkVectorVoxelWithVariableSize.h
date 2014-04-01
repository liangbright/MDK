#ifndef __mdkVectorVoxelWithVariableSize_h
#define __mdkVectorVoxelWithVariableSize_h

#include <utility>
#include <vector>

#include "mdkType.h"
#include "mdkObject.h"

namespace mdk
{
  
template<typename ElementType>
class VectorVoxelWithVariableSize : public Object
{
private:
    std::vector<ElementType> m_ElementData;

    ElementType m_ZeroElement;

public:
    
    inline VectorVoxelWithVariableSize();

    inline VectorVoxelWithVariableSize(const ElementType& Element, int_max Length = 1);

    inline VectorVoxelWithVariableSize(const VectorVoxelWithVariableSize<ElementType>& Voxel) = delete;

    inline VectorVoxelWithVariableSize(VectorVoxelWithVariableSize<ElementType, Length>&& Voxel);

    inline ~VectorVoxelWithVariableSize();

    inline void operator=(const VectorVoxelWithVariableSize<ElementType, Length>& Voxel);

    inline void operator=(VectorVoxelWithVariableSize<ElementType, Length>&& Voxel);

    inline void Clear(); // set to empty

    inline void Fill(const ElementType& Element);

    inline int_max GetLength();

    //-----------element access------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

    inline ElementType& operator[](uint_max Index);

    inline const ElementType& operator[](uint_max Index) const;

    ElementType& operator()(uint_max Index);

    const ElementType& operator()(uint_max Index) const;

    ElementType& at(uint_max Index);

    const ElementType& at(uint_max Index) const;

    // ------------------------------------------------------------------------------------------------------------//

    inline void operator+=(const VectorVoxelWithVariableSize<ElementType>& Voxel);

    inline void operator-=(const VectorVoxelWithVariableSize<ElementType>& Voxel);

    inline void operator*=(const VectorVoxelWithVariableSize<ElementType>& Voxel);

    inline void operator/=(const VectorVoxelWithVariableSize<ElementType>& Voxel);

    //---------------------------------------------------------------------------------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

};


}// namespace mdk

#include "mdkVectorVoxelWithVariableSize.hpp"

#endif