#ifndef mdk_ImageInfo3D_hpp
#define mdk_ImageInfo3D_hpp

namespace mdk
{

template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DIndex(int_max LinearIndex, int_max ImageSizeX, int_max ImageSizeY)
{
	auto divresult = std::div(LinearIndex, ImageSizeX*ImageSizeY);

	auto zIndex = divresult.quot; // z

	divresult = std::div(divresult.rem, ImageSizeX);

	auto yIndex = divresult.quot; // y
	auto xIndex = divresult.rem; // x

	DenseVector<ScalarType, 3> Index3D;
	Index3D[0] = ScalarType(xIndex);
	Index3D[1] = ScalarType(yIndex);
	Index3D[2] = ScalarType(zIndex);
	return Index3D;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DIndex(int_max LinearIndex, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_LinearIndexTo3DIndex(LinearIndex, Info.Size[0], Info.Size[1]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DPosition(int_max LinearIndex, double SpacingX, double SpacingY, double SpacingZ)
{
	auto Index3D = ImageCoordinateTransform_LinearIndexTo3DIndex(LinearIndex);
	auto Position = ImageCoordinateTransform_3DIndexTo3DPosition<ScalarType>(Index3D[0], Index3D[1], Index3D[2], SpacingX, SpacingY, SpacingZ);
	return Position;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DPosition(int_max LinearIndex, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_LinearIndexTo3DPosition(LinearIndex, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DWorldPosition(int_max LinearIndex, int_max ImageSizeX, int_max ImageSizeY,
                                                                                 const DenseMatrix<double>& TransformMatrix_3DIndexTo3DWorld,
								                                                 double OriginX, double OriginY, double OriginZ)
{
	auto Index3D = ImageCoordinateTransform_LinearIndexTo3DIndex(LinearIndex, ImageSizeX, ImageSizeY);
	auto Position = ImageCoordinateTransform_3DIndexTo3DWorldPosition<ScalarType>(Index3D[0], Index3D[1], Index3D[2], TransformMatrix_3DIndexTo3DWorld,
		                                                                          OriginX, OriginY, OriginZ);
	return Position;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransforme_LinearIndexTo3DWorldPosition(int_max LinearIndex, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_LinearIndexTo3DWorldPosition(LinearIndex, Info.TransformMatrix_3DIndexTo3DWorld, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


//3DIndex is discrete
inline int_max ImageCoordinateTransform_3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex, int_max ImageSizeX, int_max ImageSizeY)
{
	return zIndex*ImageSizeX*ImageSizeY + yIndex*ImageSizeX + xIndex;
}


inline int_max ImageCoordinateTransform_3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_3DIndexToLinearIndex(xIndex, yIndex, zIndex, Info.Size[0], Info.Size[1]);
}


template<typename ScalarType_Position, typename ScalarType_Index>
inline 
DenseVector<ScalarType_Position, 3>
ImageCoordinateTransform_3DIndexTo3DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex, 
                                             double SpacingX, double SpacingY, double SpacingZ)
{
	DenseVector<ScalarType_Position, 3> Position;
	Position[0] = ScalarType_Position(double(xIndex)*SpacingX);
	Position[1] = ScalarType_Position(double(yIndex)*SpacingY);
	Position[2] = ScalarType_Position(double(zIndex)*SpacingZ);
	return Position;
}


template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3>
ImageCoordinateTransform_3DIndexTo3DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_3DIndexTo3DPosition<ScalarType_Position>(xIndex, yIndex, zIndex, Info.Spacing[0], Info.Spacing[1], Info.Spacing[2]);
}


template<typename ScalarType_Position, typename ScalarType_Index>
inline 
DenseVector<ScalarType_Position, 3> 
ImageCoordinateTransform_3DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex,
							                      const DenseMatrix<double>& TransformMatrix_3DIndexTo3DWorld,
								                  double OriginX, double OriginY, double OriginZ)
{
	auto M = TransformMatrix_3DIndexTo3DWorld.GetElementPointer();
	DenseVector<ScalarType_Position, 3> Position;
	Position[0] = ScalarType_Position(OriginX + double(xIndex)*M[0] + double(yIndex)*M[3] + double(zIndex)*M[6]);
	Position[1] = ScalarType_Position(OriginY + double(xIndex)*M[1] + double(yIndex)*M[4] + double(zIndex)*M[7]);
	Position[2] = ScalarType_Position(OriginZ + double(xIndex)*M[2] + double(yIndex)*M[5] + double(zIndex)*M[8]);
	return Position;
}


template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3>
ImageCoordinateTransform_3DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_3DIndexTo3DWorldPosition<ScalarType_Position>(xIndex, yIndex, zIndex, Info.TransformMatrix_3DIndexTo3DWorld, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_3DPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z,
															            double SpacingX, double SpacingY, double SpacingZ)
{
	DenseVector<ScalarType, 3> Index3D;
	Index3D[0] = ScalarType(double(x) / SpacingX);
	Index3D[1] = ScalarType(double(y) / SpacingY);
	Index3D[2] = ScalarType(double(z) / SpacingZ);
	return Index3D;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ImageCoordinateTransform_3DPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_3DPositionTo3DIndex(x, y, z, Info.Spacing[0], Info.Spacing[1], Info.Spacing[2]);
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ImageCoordinateTransform_3DPositionTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z,
																	            const DenseMatrix<double>& Orientation,
																	            double OriginX, double OriginY, double OriginZ)
{
	auto R = Orientation.GetElementPointer();
	DenseVector<ScalarType, 3> Position;
	Position[0] = ScalarType(OriginX + double(x)*R[0] + double(y)*R[3] + double(z)*R[6]);
	Position[1] = ScalarType(OriginY + double(x)*R[1] + double(y)*R[4] + double(z)*R[7]);
	Position[2] = ScalarType(OriginZ + double(x)*R[2] + double(y)*R[5] + double(z)*R[8]);
	return Position;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ImageCoordinateTransform_3DPositionTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_3DPositionTo3DWorldPosition(x, y, z, Info.Orientation, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_3DWorldPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z,
																             const DenseMatrix<double>& TransformMatrix_3DWorldTo3DIndex,
	                                                                         double OriginX, double OriginY, double OriginZ)
{
	auto temp_x = double(x) - OriginX;
	auto temp_y = double(y) - OriginY;
	auto temp_z = double(z) - OriginZ;

	auto M = TransformMatrix_3DWorldTo3DIndex.GetElementPointer();

	DenseVector<ScalarType, 3> Index3D;
	Index3D[0] = ScalarType(double(temp_x)*M[0] + double(temp_y)*M[3] + double(temp_z)*M[6]);
	Index3D[1] = ScalarType(double(temp_x)*M[1] + double(temp_y)*M[4] + double(temp_z)*M[7]);
	Index3D[2] = ScalarType(double(temp_x)*M[2] + double(temp_y)*M[5] + double(temp_z)*M[8]);
	return Index3D;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ImageCoordinateTransform_3DWorldPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_3DWorldPositionTo3DIndex(x, y, z, Info.TransformMatrix_3DWorldTo3DIndex, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_3DWorldPositionTo3DPosition(ScalarType x, ScalarType y, ScalarType z,
																			    const DenseMatrix<double>& Orientation,
	                                                                            double OriginX, double OriginY, double OriginZ)
{
	auto temp_x = double(x) - OriginX;
	auto temp_y = double(y) - OriginY;
	auto temp_z = double(z) - OriginZ;
	auto R = Orientation.GetElementPointer();
	DenseVector<ScalarType, 3> Position;
	Position[0] = ScalarType(double(temp_x)*R[0] + double(temp_y)*R[1] + double(temp_z)*R[2]);
	Position[1] = ScalarType(double(temp_x)*R[3] + double(temp_y)*R[4] + double(temp_z)*R[5]);
	Position[2] = ScalarType(double(temp_x)*R[6] + double(temp_y)*R[7] + double(temp_z)*R[8]);
	return Position;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_3DWorldPositionTo3DPosition(ScalarType x, ScalarType y, ScalarType z, const ImageInfo3D& Info)
{
	return ImageCoordinateTransform_3DWorldPositionTo3DPosition(x, y, z, Info.Orientation, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


}//namespace mdk


#endif