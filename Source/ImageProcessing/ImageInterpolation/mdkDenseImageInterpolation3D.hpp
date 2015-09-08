#ifndef mdk_DenseImageInterpolation3D_hpp
#define mdk_DenseImageInterpolation3D_hpp

namespace mdk
{
template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex(const DenseImage3D<InputPixelType>& InputImage,
                                          ScalarType x, ScalarType y, ScalarType z, 
										  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
										  bool EnableBoundCheck)
{
	switch (Option.MethodType)
    {
	case MethodEnum_Of_Image3DInterpolation::Nearest:
		return InterpolateImageAt3DIndex_Nearest<OutputPixelType>(InputImage, x, y, z, Option, EnableBoundCheck);

	case MethodEnum_Of_Image3DInterpolation::Linear:
		return InterpolateImageAt3DIndex_Linear<OutputPixelType>(InputImage, x, y, z, Option, EnableBoundCheck);

    //case MethodEnum_Of_Image3DInterpolation::Cubic:
    //    return InterpolateImageAt3DIndex_Cubic<OutputPixelType(InputImage, x, y, z, Option, EnableBoundCheck);

    default:
        MDK_Error("InterpolationMethod is not supported @ mdkDenseImageImageInterpolation3D InterpolateImageAt3DIndex(...)")
		return OutputPixelType(Option.Pixel_OutsideImage);
    }
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DPosition(const DenseImage3D<InputPixelType>& InputImage,
                                             ScalarType x, ScalarType y, ScalarType z,
                                             const Option_Of_Image3DInterpolation<InputPixelType>& Option,
											 bool EnableBoundCheck)
{
	auto Index3D = InputImage.Transform3DPositionTo3DIndex(x, y, z);
	return InterpolateImageAt3DIndex<OutputPixelType>(InputImage, Index3D[0], Index3D[1], Index3D[2], Option, EnableBoundCheck);
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DWorldPosition(const DenseImage3D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y, ScalarType z, 
											      const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck)
{
	auto Index3D = InputImage.Transform3DWorldPositionTo3DIndex(x, y, z);
	return InterpolateImageAt3DIndex<OutputPixelType>(InputImage, Index3D[0], Index3D[1], Index3D[2], Option, EnableBoundCheck);
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Nearest(const DenseImage3D<InputPixelType>& InputImage,
                                                  ScalarType x, ScalarType y, ScalarType z, 
												  const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												  bool EnableBoundCheck)
{
    auto Size = InputImage.GetSize();

	auto x0 = int_max(std::round(x));
	auto y0 = int_max(std::round(y));
	auto z0 = int_max(std::round(z));

	if (EnableBoundCheck == true)
	{
		if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image3DInterpolation::Constant)
		{
			if (x0 < 0 || x0 >= Size[0])
			{
				return OutputPixelType(Option.Pixel_OutsideImage);
			}

			if (y0 < 0 || y0 >= Size[1])
			{
				return OutputPixelType(Option.Pixel_OutsideImage);
			}

			if (z0 < 0 || z0 >= Size[2])
			{
				return OutputPixelType(Option.Pixel_OutsideImage);
			}
		}
		else if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image3DInterpolation::Replicate)
		{
			if (x0 < 0)
			{
				x0 = 0;
			}
			else if (x0 >= Size[0])
			{
				x0 = Size[0] - 1;
			}

			if (y0 < 0)
			{
				y0 = 0;
			}
			else if (y0 >= Size[1])
			{
				y0 = Size[1] - 1;
			}

			if (z0 < 0)
			{
				z0 = 0;
			}
			else if (z0 >= Size[2])
			{
				z0 = Size[2] - 1;
			}
		}
	}

	return OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y0, z0));
}


template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Linear(const DenseImage3D<InputPixelType>& InputImage,
                                                 ScalarType x, ScalarType y, ScalarType z, 
												 const Option_Of_Image3DInterpolation<InputPixelType>& Option,
												 bool EnableBoundCheck)
{// no input check, InputImage must have at least 8 pixel

    auto Size = InputImage.GetSize();
   
	// only keep integer part
	auto x0 = int_max(x);
	auto y0 = int_max(y);
	auto z0 = int_max(z);

	//---------------------------------------------
	if (EnableBoundCheck == true)
	{
		if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image3DInterpolation::Constant)
		{
			if (x0 < 0 || x0 >= Size[0])
			{
				return OutputPixelType(Option.Pixel_OutsideImage);
			}

			if (y0 < 0 || y0 >= Size[1])
			{
				return OutputPixelType(Option.Pixel_OutsideImage);
			}

			if (z0 < 0 || z0 >= Size[2])
			{
				return OutputPixelType(Option.Pixel_OutsideImage);
			}

			if (x0 == Size[0] - 1 || y0 == Size[1] - 1 || z0 == Size[2] - 1)// on edge
			{
				return OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y0, z0));
			}
		}
		else if (Option.BoundaryOption == BoundaryOptionEnum_Of_Image3DInterpolation::Replicate)
		{
			bool Flag_return = false;

			if (x0 < 0)
			{
				x0 = 0;
				Flag_return = true;
			}
			else if (x0 >= Size[0] - 1) // on the edge or outside
			{
				x0 = Size[0] - 1;
				Flag_return = true;
			}

			if (y0 < 0)
			{
				y0 = 0;
				Flag_return = true;
			}
			else if (y0 >= Size[1] - 1)// on the edge or outside
			{
				y0 = Size[1] - 1;
				Flag_return = true;
			}

			if (z0 < 0)
			{
				z0 = 0;
				Flag_return = true;
			}
			else if (z0 >= Size[2] - 1)// on the edge or outside
			{
				z0 = Size[2] - 1;
				Flag_return = true;
			}

			if (Flag_return == true)
			{
				return OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y0, z0));
			}
		}
	}
	else // assume (x0, y0, z0) is in the image (may be on edge)
	{//if on edge, then return
		if (x0 == Size[0] - 1)
		{
			return OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y0, z0));
		}

		if (y0 == Size[1] - 1)
		{
			return OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y0, z0));
		}

		if (z0 == Size[2] - 1)
		{
			return OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y0, z0));
		}
	}
	//---------------------------------------------
	// (x0, y0, z0) is now inside the image, NOT on edge

    int_max x1 = x0 + 1;
    int_max y1 = y0 + 1;
    int_max z1 = z0 + 1;

	auto Pixel_x0_y0_z0 = OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y0, z0));
	auto Pixel_x0_y1_z0 = OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y1, z0));
	auto Pixel_x0_y0_z1 = OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y0, z1));
	auto Pixel_x0_y1_z1 = OutputPixelType(InputImage.GetPixelAt3DIndex(x0, y1, z1));
	auto Pixel_x1_y0_z0 = OutputPixelType(InputImage.GetPixelAt3DIndex(x1, y0, z0));
	auto Pixel_x1_y1_z0 = OutputPixelType(InputImage.GetPixelAt3DIndex(x1, y1, z0));
	auto Pixel_x1_y0_z1 = OutputPixelType(InputImage.GetPixelAt3DIndex(x1, y0, z1));
	auto Pixel_x1_y1_z1 = OutputPixelType(InputImage.GetPixelAt3DIndex(x1, y1, z1));

	auto dx = x - ScalarType(x0);
	auto dy = y - ScalarType(y0);
	auto dz = z - ScalarType(z0);

	auto One = ScalarType(1);

	auto c00 = Pixel_x0_y0_z0*(One - dx) + Pixel_x1_y0_z0*dx;
	auto c10 = Pixel_x0_y1_z0*(One - dx) + Pixel_x1_y1_z0*dx;
	auto c01 = Pixel_x0_y0_z1*(One - dx) + Pixel_x1_y0_z1*dx;
	auto c11 = Pixel_x0_y1_z1*(One - dx) + Pixel_x1_y1_z1*dx;

	auto c0 = c00*(One - dy) + c10*dy;
	auto c1 = c01*(One - dy) + c11*dy;

	auto c = c0*(One - dz) + c1*dz;

	return OutputPixelType(c);
}

/*
template<typename OutputPixelType, typename InputPixelType, typename ScalarType>
inline
OutputPixelType InterpolateImageAt3DIndex_Cubic(const DenseImage3D<InputPixelType>& InputImage,
                                                ScalarType x, ScalarType y, ScalarType z, 
												const Option_Of_Image3DInterpolation<InputPixelType>& Option)
{   

}
*/

}// namespace mdk

#endif