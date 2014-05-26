#ifndef __mdkITKDataStructureConversion_h
#define __mdkITKDataStructureConversion_h

#include <itkImportImageFilter.h>
#include <itkImageDuplicator.h>

namespace mdk
{

//------------- forward declare-------------------//
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class SparseMatrix;

template<typename PixelType>
class Image2D;

template<typename PixelType>
class Image3D;

template<typename ScalarType>
class TriangleMesh;
//-------------------------------------------------//


//-------------------------------------- convert mdk image to itk image--------------------------------------------------------------------//

//copy or share data
template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(const Image3D<PixelType>& InputImage, bool SharePixelData);

//copy data
template<typename PixelType>
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(const Image3D<PixelType>& InputImage);

//-------------------------------------- convert itk image to mdk image--------------------------------------------------------------------//
//copy data
template<typename PixelType>
Image3D<PixelType> ConvertITK3DScalarImageToMDK3DScalarImage(const itk::Image<PixelType, 3>* ITKImage);

//--------------------------------------- convert mdk TriangleMesh to itk Mesh -----------------------------------------------//
//template<typename ScalarType>
//itk::SmartPointer<itk::Mesh> ConvertMDKTriangleMeshToITKMesh(const TriangleMesh<ScalarType>& InputMesh);

//--------------------------------------- convert itk Mesh to mdk TriangleMesh ------------------------------------------------//
//template<typename ScalarType>
//TriangleMesh<ScalarType> ConvertITKMeshToMDKTriangleMesh(const itk::Mesh* ITKMesh);


}// namespace mdk

#include "mdkITKDataStructureConversion.hpp"

#endif