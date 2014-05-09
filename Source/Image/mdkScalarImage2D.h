#ifndef __mdkScalarImage2D_h
#define __mdkScalarImage2D_h

#include <vector>
#include <memory>
#include <cstdlib>


#include "mdkImage2D.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 2D Scalar Image Class
// --------------------------------------------------------------------------------------------------------//


template<typename PixelType>
class ScalarImage2D : public Image2D<PixelType>
{

public:

    ScalarImage2D();

    ScalarImage2D(ScalarImage2D&& InputImage);

    ~ScalarImage2D();

    void operator=(ScalarImage2D&& InputImage);

	//-------------------------- Get SubImage -------------------------------//

    ScalarImage2D GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e) const;

	//-------------------------- Pad, UnPad -------------------------------//

    ScalarImage2D  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly) const;

    ScalarImage2D  Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly) const;

    ScalarImage2D  UnPad(int_max Pad_Lx, int_max Pad_Ly) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    PixelType Sum() const;

    PixelType Mean() const;

    PixelType Max() const;

    PixelType Min() const;

private:
    ScalarImage2D(const ScalarImage2D& InputImage) = delete;
    void operator=(const ScalarImage2D& InputImage) = delete;

};


}//end namespace mdk

#include "mdkScalarImage2D.hpp"

#endif