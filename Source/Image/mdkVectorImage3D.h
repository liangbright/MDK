#ifndef __mdkVectorImage3D_h
#define __mdkVectorImage3D_h

#include <vector>
#include <memory>
#include <cstdlib>


#include "mdkImageConfig.h"
#include "mdkImage3D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 3D Vector Image Class
// Pixel is Vector
// --------------------------------------------------------------------------------------------------------//

template<typename VectorType>
class VectorImage3D : public Image3D<VectorType>
{
public:

    typedef VectorType::ElementType ElementTypeInVector;

public:		
	
    VectorImage3D();

    VectorImage3D(VectorImage3D&& InputImage);

    ~VectorImage3D();

    void operator=(VectorImage3D&& InputImage);

private:
    VectorImage3D(const VectorImage3D& InputImage) = delete;

    void operator=(const VectorImage3D& InputImage) = delete;
};


}//end namespace mdk

#include "mdkVectorImage3D.hpp"

#endif