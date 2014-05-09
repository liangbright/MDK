#ifndef __mdkScalarImage3D_h
#define __mdkScalarImage3D_h

#include <vector>
#include <memory>
#include <cstdlib>


#include "mdkImage3D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 3D Scalar Image Class
// Pixel is scalar
// PixelType is double/float/int ...
// --------------------------------------------------------------------------------------------------------//

template<typename PixelType>
class ScalarImage3D : public Image3D<PixelType>
{
public:		
	
    ScalarImage3D();

    ScalarImage3D(ScalarImage3D&& InputImage);

    ~ScalarImage3D();

    void operator=(ScalarImage3D&& InputImage);

    //-------------------------- Get SubImage -------------------------------//

    ScalarImage3D GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e, int_max zIndex_s, int_max zIndex_e) const;

    //-------------------------- Pad, UnPad -------------------------------//

    ScalarImage3D  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    ScalarImage3D  Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    ScalarImage3D  UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    PixelType Sum() const;

    PixelType Mean() const;

    PixelType Max() const;

    PixelType Min() const;

private:
    ScalarImage3D(const ScalarImage3D& InputImage) = delete;
    void operator=(const ScalarImage3D& InputImage) = delete;

};


}//end namespace mdk

#include "mdkScalarImage3D.hpp"

#endif