#ifndef __mdkVectorImage3D_hpp
#define __mdkVectorImage3D_hpp


namespace mdk
{

template<typename VectorType>
VectorImage3D<VectorType>::VectorImage3D()
: Image3D()
{
}


template<typename VectorType>
VectorImage3D<VectorType>::VectorImage3D(VectorImage3D<VectorType>&& InputImage)
: Image3D(InputImage)
{
}


template<typename VectorType>
VectorImage3D<VectorType>::~VectorImage3D()
{
}


template<typename PixelType>
void VectorImage3D<PixelType>::operator=(VectorImage3D<PixelType>&& InputImage)
{
    this->Image3D::operator=(std::forward<VectorImage3D<PixelType>&&>(InputImage));
}



}//end namespace mdk

#endif