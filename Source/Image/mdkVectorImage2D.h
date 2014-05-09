#ifndef __mdkVectorImage2D_h
#define __mdkVectorImage2D_h

#include <vector>
#include <memory>
#include <cstdlib>


#include "mdkImage2D.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 2D Vector Image Class
// example:
// multi-channel 2D microscopy image, RGB image
// --------------------------------------------------------------------------------------------------------//

template<typename PixelType>
class VectorImage2D : public Image2D<PixelType>
{

public:

    VectorImage2D();

    VectorImage2D(VectorImage2D&& InputImage);

    ~VectorImage2D();

    void operator=(VectorImage2D&& InputImage);

	//-------------------------- Get SubImage -------------------------------//

    VectorImage2D GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e) const;

	//-------------------------- Pad, UnPad -------------------------------//

    VectorImage2D  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly) const;

    VectorImage2D  Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly) const;

    VectorImage2D  UnPad(int_max Pad_Lx, int_max Pad_Ly) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    PixelType Sum() const;

    PixelType Mean() const;

    PixelType Max() const;

    PixelType Min() const;

private:
    VectorImage2D(const VectorImage2D& InputImage) = delete;
    void operator=(const VectorImage2D& InputImage) = delete;

};


}//end namespace mdk

#include "mdkVectorImage2D.hpp"

#endif