#ifndef __mdkScalarImage2D_hpp
#define __mdkScalarImage2D_hpp


namespace mdk
{

template<typename PixelType>
ScalarImage2D<PixelType>::ScalarImage2D()
: Image2D()
{
}


template<typename PixelType>
ScalarImage2D<PixelType>::~ScalarImage2D()
{
}


template<typename PixelType>
ScalarImage2D<PixelType>::ScalarImage2D(ScalarImage2D<PixelType>&& InputImage)
: Image2D(std::forward<ScalarImage2D<PixelType>&&>(InputImage))
{
}


template<typename PixelType>
void ScalarImage2D<PixelType>::operator=(ScalarImage2D<PixelType>&& InputImage)
{
    this->Image2D::operator=(std::forward<ScalarImage2D<PixelType>&&>(InputImage));
}


template<typename PixelType>
ScalarImage2D<PixelType> ScalarImage2D<PixelType>::GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e) const
{
    ScalarImage2D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 2DImage::GetSubImage()")
        return tempImage;
    }

    auto Size = this->GetSize();

    if (xIndex_s >= Size[0] || xIndex_s < 0 || xIndex_e >= Size[0] || xIndex_e < 0  || xIndex_s > xIndex_e
        || yIndex_s >= Size[1] || yIndex_s < 0 || yIndex_e >= Size[1] || yIndex_e < 0  || yIndex_s > yIndex_e)
	{
        MDK_Error("Invalid input @ 2DImage::GetSubImage() const")
		return tempImage;
	}

	Lx = xIndex_e - xIndex_s + 1;
	Ly = yIndex_e - yIndex_s + 1;

    tempImage.SetSize(Lx, Ly);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());
    tempImage.SetOrientation(this->GetOrientation());

    auto tempRawPtr = tempImage.GetPixelPointer();

    auto RawPtr = this->GetPixelPointer();

	int_max Index_j = yIndex_s;

	for (int_max j = yIndex_s; j <= yIndex_e; ++j)
	{
        auto temp = j*Size.Lx;

		for (int_max i = xIndex_s; i <= xIndex_e; ++i)
		{
            tempRawPtr[0] = RawPtr[temp + i];
            
            ++tempRawPtr;
		}
	}
    
	return tempImage;
}


template<typename PixelType>
ScalarImage2D<PixelType> 
ScalarImage2D<PixelType>::Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly) const
{
    ScalarImage2D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 2DImage::Pad")
        return tempImage;
    }

    if (Option != "replicate")
	{
		MDK_Error("Invalid Option @ 2DImage::Pad")
		return tempImage;
	}

    if (Pad_Lx <= 0 && Pad_Ly <= 0)
	{
		MDK_Warning("Invalid Pad Size @ 2DImage::Pad")

		tempImage = (*this);

		return tempImage;
	}

    auto Size = this->GetSize();

    auto Lx = Size.Lx + Pad_Lx;
    auto Ly = Size.Ly + Pad_Ly;

    tempImage.SetSize(Lx, Ly);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());

	if (Option == "zero")
	{
        tempImage.Fill(PixelType(0));

		for (int_max j = 0; j <= Ly; ++j)
		{
			for (int_max i = 0; i <= Lx; ++i)
			{
				auto temp_i = i + Pad_Lx;
				auto temp_j = j + Pad_Ly;
		
				tempImage(temp_i, temp_j) = (*this)(i, j, k);
			}
		}
	}
	else if (Option == "replicate")
	{
		for (int_max temp_j = 0; temp_j <= Ly; ++temp_j)
		{
			for (int_max temp_i = 0; temp_i <= Lx; ++temp_i)
			{
				auto i = std::min(std::max(temp_i - Pad_Lx, 0), Size[0] - 1);

				auto j = std::min(std::max(temp_j - Pad_Ly, 0), Size[1] - 1);
		
				tempImage(temp_i, temp_j) = (*this)(i, j);
			}
		}		
	}    

    return tempImage;
}


template<typename PixelType>
ScalarImage2D<PixelType>
ScalarImage2D<PixelType>::Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly) const
{
    ScalarImage2D<PixelType> tempImage; // empty image
    
    if (this->IsEmpty() == true)
    {
        MDK_Warning("Image is empty @ 2DImage::Pad")
        return tempImage;
    }

	if (Pad_Lx <= 0 && Pad_Ly <= 0)
	{
		MDK_Warning("Invalid Pad Size @ 2DImage::Pad")

		tempImage = (*this);

		return tempImage;
	}

    auto Size = this->GetSize();

    auto Lx = Size.Lx + Pad_Lx;
    auto Ly = Size.Ly + Pad_Ly;

    tempImage.SetSize(Lx, Ly);
    tempImage.SetPixelSpacing(this->GetPixelSpacing());
    tempImage.SetPhysicalOrigin(this->GetPhysicalOrigin());

    tempImage.Fill(Pixel);

    for (int_max j = 0; j <= Ly; ++j)
    {
        for (int_max i = 0; i <= Lx; ++i)
        {
            auto temp_i = i + Pad_Lx;
            auto temp_j = j + Pad_Ly;

            tempImage(temp_i, temp_j) = (*this)(i, j);
        }
    }
    
	return tempImage;
}


template<typename PixelType>
ScalarImage2D<PixelType> 
ScalarImage2D<PixelType>::UnPad(int_max Pad_Lx, int_max Pad_Ly) const
{
    ScalarImage2D<PixelType> tempImage; // empty image
   
    auto Size = this->GetSize();

    if (Pad_Lx > Size.Lx || Pad_Lx  < 0 || Pad_Ly > Size.Ly || Pad_Ly < 0)
	{
		MDK_Error("Invalid Pad Size @ 2DImage::UnPad")
		return tempImage;
	}

    if (Pad_Lx == Size.Lx || Pad_Ly == Size.Ly)
	{
		MDK_Warning("Output is empty @ 2DImage::UnPad")
		return tempImage;
	}

	if (Pad_Lx == 0 && Pad_Ly == 0)
	{
		MDK_Warning("Input Pad Size is [0, 0] @ 2DImage::UnPad")

		tempImage = (*this);

		return tempImage;
	}

    return this->GetSubImage(Pad_Lx, Size.Lx - 1 - Pad_Lx,
                             Pad_Ly, Size.Ly - 1 - Pad_Ly);
                             

}


}//end namespace mdk

#endif