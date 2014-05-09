#ifndef __mdkScalarImage3D_hpp
#define __mdkScalarImage3D_hpp


namespace mdk
{

template<typename PixelType>
ScalarImage3D<PixelType>::ScalarImage3D()
: Image3D()
{

}


template<typename PixelType>
ScalarImage3D<PixelType>::ScalarImage3D(ScalarImage3D<PixelType>&& InputImage)
: Image3D(std::forward<ScalarImage3D<PixelType>&&>(InputImage))
{
}


template<typename PixelType>
ScalarImage3D<PixelType>::~ScalarImage3D()
{

}


template<typename PixelType>
void ScalarImage3D<PixelType>::operator=(ScalarImage3D&& InputImage)
{
    this->Image3D::operator=(std::forward<ScalarImage3D<PixelType>&&>(InputImage));
}


template<typename PixelType>
ScalarImage3D<PixelType> 
ScalarImage3D<PixelType>::GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e, int_max zIndex_s, int_max zIndex_e) const
{
    ScalarImage3D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 3DImage::GetSubImage()")
        return tempImage;
    }

    auto Size = this->GetSize();

    if (xIndex_s >= Size[0] || xIndex_s < 0
        || xIndex_e >= Size[0] || xIndex_e < 0
        || xIndex_s > xIndex_e
        || yIndex_s >= Size[1] || yIndex_s < 0
        || yIndex_e >= Size[1] || yIndex_e < 0
        || yIndex_s > yIndex_e
        || zIndex_s >= Size[2] || zIndex_s < 0
        || zIndex_e >= Size[2] || zIndex_e < 0
        || zIndex_s > zIndex_e)
	{
        MDK_Error("Invalid input @ 3DImage::GetSubImage() const")
		return tempImage;
	}

	Lx = xIndex_e - xIndex_s + 1;
	Ly = yIndex_e - yIndex_s + 1;
	Lz = zIndex_e - zIndex_s + 1;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());

    auto tempRawPtr = tempImage.GetPixelPointer();

    auto RawPtr = this->GetPixelPointer();

    PixelNumberPerZSlice = m_ImageData->PixelNumberPerZSlice;

	int_max temp_k   temp_k = zIndex_s;

	for (int_max k = zIndex_s; k <= zIndex_e; ++k)
	{
        auto temp_k = k*PixelNumberPerZSlice;

		for (int_max j = yIndex_s; j <= yIndex_e; ++j)
		{
            auto temp_j = j*Size.Lx;

			for (int_max i = xIndex_s; i <= xIndex_e; ++i)
			{
                tempRawPtr[0] = RawPtr[temp_k + temp_j + i];

				++tempRawPtr;
			}
		}
	}
    
	return tempImage;
}


template<typename PixelType>
ScalarImage3D<PixelType>
ScalarImage3D<PixelType>::Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    ScalarImage3D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 3DImage::Pad")
        return tempImage;
    }

    if (Option != "replicate")
	{
		MDK_Error("Invalid Option @ 3DImage::Pad")
		return tempImage;
	}

    if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
	{
		MDK_Warning("Invalid Pad Size @ 3DImage::Pad")

		tempImage = (*this);

		return tempImage;
	}

    auto Size = this->GetSize();

    auto Lx = Size.Lx + Pad_Lx;
    auto Ly = Size.Ly + Pad_Ly;
    auto Lz = Size.Lz + Pad_Lz;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());


	if (Option == "zero")
	{
        tempImage.Fill(PixelType(0));

		for (int_max k = 0; k <= Lz; ++k)
		{
			for (int_max j = 0; j <= Ly; ++j)
			{
				for (int_max i = 0; i <= Lx; ++i)
				{
					auto temp_i = i + Pad_Lx;
					auto temp_j = j + Pad_Ly;
					auto temp_k = k + Pad_Lz;

					tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
				}
			}
		}
	}
	else if (Option == "replicate")
	{
		for (int_max temp_k = 0; temp_k <= Lz; ++temp_k)
		{
			for (int_max temp_j = 0; temp_j <= Ly; ++temp_j)
			{
				for (int_max temp_i = 0; temp_i <= Lx; ++temp_i)
				{
					auto i = std::min(std::max(temp_i - Pad_Lx, 0), Size[0] - 1);

					auto j = std::min(std::max(temp_j - Pad_Ly, 0), Size[1] - 1);

					auto k = std::min(std::max(temp_k - Pad_Lz, 0), Size[2] - 1);

					tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
				}
			}
		}
	}
    

    return tempImage;
}


template<typename PixelType>
ScalarImage3D<PixelType>
ScalarImage3D<PixelType>::Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    ScalarImage3D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 3DImage::Pad")
        return tempImage;
    }

	if (Pad_Lx <= 0 && Pad_Ly <= 0 && Pad_Lz <= 0)
	{
		MDK_Warning("Invalid Pad Size @ 3DImage::Pad")

		tempImage = (*this);

		return tempImage;
	}

    auto Size = this->GetSize();

    auto Lx = Size.Lx + Pad_Lx;
    auto Ly = Size.Ly + Pad_Ly;
    auto Lz = Size.Lz + Pad_Lz;

    tempImage.SetSize(Lx, Ly, Lz);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());

    tempImage.Fill(Pixel);

    for (int_max k = 0; k <= Lz; ++k)
    {
        for (int_max j = 0; j <= Ly; ++j)
        {
            for (int_max i = 0; i <= Lx; ++i)
            {
                auto temp_i = i + Pad_Lx;
                auto temp_j = j + Pad_Ly;
                auto temp_k = k + Pad_Lz;

                tempImage(temp_i, temp_j, temp_k) = (*this)(i, j, k);
            }
        }
    }
    
	return tempImage;

}


template<typename PixelType>
ScalarImage3D<PixelType>
ScalarImage3D<PixelType>::UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const
{
    Image3D<PixelType> tempImage; // empty image
   
    auto Size = this->GetSize();

    if (Pad_Lx > Size.Lx || Pad_Lx  < 0 || Pad_Ly > Size.Ly || Pad_Ly < 0 || Pad_Lz > Size.Lz || Pad_Lz < 0)
	{
		MDK_Error("Invalid Pad Size @ 3DImage::UnPad")
		return tempImage;
	}

    if (Pad_Lx == Size.Lx || Pad_Ly == Size.Ly || Pad_Lz == Size.Lz)
	{
		MDK_Warning("Output is empty @ 3DImage::UnPad")
		return tempImage;
	}

	if (Pad_Lx == 0 && Pad_Ly == 0 && Pad_Lz == 0)
	{
		MDK_Warning("Input Pad Size is [0, 0, 0] @ 3DImage::UnPad")

		tempImage = (*this);

		return tempImage;
	}

    return this->GetSubImage(Pad_Lx, Size.Lx - 1 - Pad_Lx,
                             Pad_Ly, Size.Ly - 1 - Pad_Ly,
                             Pad_Lz, Size.Lz - 1 - Pad_Lz);
                             

}

}//end namespace mdk

#endif