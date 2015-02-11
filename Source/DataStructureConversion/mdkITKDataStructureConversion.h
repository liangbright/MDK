#ifndef mdk_ITKDataStructureConversion_h
#define mdk_ITKDataStructureConversion_h

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
class DenseImage3D;

template<typename ScalarType>
class TriangleMesh;

template<typename ScalarType>
class PolygonMesh;
//-------------------------------------------------//


//-------------------------------------- convert mdk image to itk image--------------------------------------------------------------------//

//copy or share data
template<typename PixelType>
itk::SmartPointer<itk::ImportImageFilter<PixelType, 3>> ConvertMDK3DScalarImageByITKImportImageFilter(const DenseImage3D<PixelType>& MDKImage, bool SharePixelData);

//copy data
template<typename PixelType>
itk::SmartPointer<itk::Image<PixelType, 3>> ConvertMDK3DScalarImageToITK3DScalarImage(const DenseImage3D<PixelType>& MDKImage);

template<typename PixelType>
bool ConvertMDK3DScalarImageToITK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, itk::Image<PixelType, 3>& ITKImage);

template<typename PixelType>
bool ConvertMDK3DScalarImageToITK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, itk::Image<PixelType, 3>* ITKImage);

//-------------------------------------- convert itk image to mdk image (copy data) ------------------------------------------------------------//
template<typename PixelType>
bool ConvertITK3DScalarImageToMDK3DScalarImage(const itk::Image<PixelType, 3>& ITKImage, DenseImage3D<PixelType>& MDKImage);

template<typename PixelType>
bool ConvertITK3DScalarImageToMDK3DScalarImage(const itk::Image<PixelType, 3>* ITKImage, DenseImage3D<PixelType>& MDKImage);

//--------------------------------------- convert mdk TriangleMesh to itk Mesh -----------------------------------------------//
//template<typename ScalarType>
//itk::SmartPointer<itk::Mesh> ConvertMDKTriangleMeshToITKMesh(const TriangleMesh<ScalarType>& InputMesh);

//--------------------------------------- convert itk Mesh to mdk TriangleMesh ------------------------------------------------//
//template<typename ScalarType>
//TriangleMesh<ScalarType> ConvertITKMeshToMDKTriangleMesh(const itk::Mesh* ITKMesh);


}// namespace mdk

#include "mdkITKDataStructureConversion.hpp"

#endif