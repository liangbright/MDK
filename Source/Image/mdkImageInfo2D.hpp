#ifndef mdk_ImageInfo2D_hpp
#define mdk_ImageInfo2D_hpp

namespace mdk
{
//================================================ ImageCoordinateTransform ============================================================//

template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ImageCoordinateTransform_LinearIndexTo2DIndex(int_max LinearIndex, int_max ImageSizeX)
{
	auto divresult = std::div(LinearIndex, ImageSizeX);
	auto yIndex = divresult.quot; // y
	auto xIndex = divresult.rem;  // x

	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = ScalarType(xIndex);
	Index2D[1] = ScalarType(yIndex);
	return Index2D;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 2> ImageCoordinateTransform_LinearIndexTo2DIndex(int_max LinearIndex, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_LinearIndexTo2DIndex(LinearIndex, Info.Size[0]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ImageCoordinateTransform_LinearIndexTo2DPosition(int_max LinearIndex, int_max ImageSizeX, double SpacingX, double SpacingY)
{
	auto Index2D = ImageCoordinateTransform_LinearIndexTo2DIndex(LinearIndex, ImageSizeX);
	auto Position = ImageCoordinateTransform_2DIndexTo2DPosition<ScalarType>(Index2D[0], Index2D[1], SpacingX, SpacingY);
	return Position;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ImageCoordinateTransform_LinearIndexTo2DPosition(int_max LinearIndex, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_LinearIndexTo2DPosition(LinearIndex, Info.Size[0], Info.Spacing[0], Info.Spacing[1]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DWorldPosition(int_max LinearIndex, int_max ImageSizeX,
                                                                                 const DenseMatrix<double>& TransformMatrix_2DIndexTo3DWorld,
								                                                 double OriginX, double OriginY, double OriginZ)
{
	auto Index2D = ImageCoordinateTransform_LinearIndexTo2DIndex(LinearIndex, ImageSizeX);
	auto Position = ImageCoordinateTransform_2DIndexTo3DWorldPosition<ScalarType>(Index2D[0], Index2D[1], TransformMatrix_2DIndexTo3DWorld, OriginX, OriginY, OriginZ);
	return Position;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DWorldPosition(int_max LinearIndex, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_LinearIndexTo3DWorldPosition(LinearIndex, Info.Size[0], Info.TransformMatrix_2DIndexTo3DWorld, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


//2DIndex is discrete
inline int_max ImageCoordinateTransform_2DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max ImageSizeX)
{
	return yIndex*ImageSizeX + xIndex;
}


inline int_max ImageCoordinateTransform_2DIndexToLinearIndex(int_max xIndex, int_max yIndex, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_2DIndexToLinearIndex(xIndex, yIndex, Info.Size[0]);
}


template<typename ScalarType_Position, typename ScalarType_Index>
inline 
DenseVector<ScalarType_Position, 2>
ImageCoordinateTransform_2DIndexTo2DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, double SpacingX, double SpacingY)
{
	DenseVector<ScalarType_Position, 2> Position;
	Position[0] = ScalarType_Position(double(xIndex)*SpacingX);
	Position[1] = ScalarType_Position(double(yIndex)*SpacingY);
	return Position;
}


template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 2>
ImageCoordinateTransform_2DIndexTo2DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_2DIndexTo2DPosition<ScalarType_Position>(xIndex, yIndex, Info.Spacing[0], Info.Spacing[1]);
}


template<typename ScalarType_Position, typename ScalarType_Index>
inline 
DenseVector<ScalarType_Position, 3> 
ImageCoordinateTransform_2DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex,
							                      const DenseMatrix<double>& TransformMatrix_2DIndexTo3DWorld,
								                  double OriginX, double OriginY, double OriginZ)
{
	auto M = TransformMatrix_2DIndexTo3DWorld.GetElementPointer();
	DenseVector<ScalarType_Position, 3> Position;
	Position[0] = ScalarType_Position(OriginX + double(xIndex)*M[0] + double(yIndex)*M[3]);
	Position[1] = ScalarType_Position(OriginY + double(xIndex)*M[1] + double(yIndex)*M[4]);
	Position[2] = ScalarType_Position(OriginZ + double(xIndex)*M[2] + double(yIndex)*M[5]);
	return Position;
}


template<typename ScalarType_Position, typename ScalarType_Index>
inline
DenseVector<ScalarType_Position, 3>
ImageCoordinateTransform_2DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_2DIndexTo3DWorldPosition<ScalarType_Position>(xIndex, yIndex, Info.TransformMatrix_2DIndexTo3DWorld, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ImageCoordinateTransform_2DPositionTo2DIndex(ScalarType x, ScalarType y, double SpacingX, double SpacingY)
{
	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = ScalarType(double(x) / SpacingX);
	Index2D[1] = ScalarType(double(y) / SpacingY);
	return Index2D;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 2> ImageCoordinateTransform_2DPositionTo2DIndex(ScalarType x, ScalarType y, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_2DPositionTo2DIndex(x, y, Info.Spacing[0], Info.Spacing[1]);
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ImageCoordinateTransform_2DPositionTo3DWorldPosition(ScalarType x, ScalarType y,
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
DenseVector<ScalarType, 3> ImageCoordinateTransform_2DPositionTo3DWorldPosition(ScalarType x, ScalarType y, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_2DPositionTo3DWorldPosition(x, y, Info.Orientation, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ImageCoordinateTransform_3DWorldPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z,
																             const DenseMatrix<double>& TransformMatrix_3DWorldTo2DIndex,
	                                                                         double OriginX, double OriginY, double OriginZ)
{
	auto temp_x = double(x) - OriginX;
	auto temp_y = double(y) - OriginY;
	auto temp_z = double(z) - OriginZ;

	auto M = TransformMatrix_3DWorldTo2DIndex.GetElementPointer();

	DenseVector<ScalarType, 2> Index2D;
	Index2D[0] = ScalarType(double(temp_x)*M[0] + double(temp_y)*M[2] + double(temp_z)*M[4]);
	Index2D[1] = ScalarType(double(temp_x)*M[1] + double(temp_y)*M[3] + double(temp_z)*M[5]);
	return Index2D;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 2> ImageCoordinateTransform_3DWorldPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_3DWorldPositionTo2DIndex(x, y, z, Info.TransformMatrix_3DWorldTo2DIndex, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ImageCoordinateTransform_3DWorldPositionTo2DPosition(ScalarType x, ScalarType y, ScalarType z,
																			    const DenseMatrix<double>& Orientation,
	                                                                            double OriginX, double OriginY, double OriginZ)
{
	auto temp_x = double(x) - OriginX;
	auto temp_y = double(y) - OriginY;
	auto temp_z = double(z) - OriginZ;
	auto R = Orientation.GetElementPointer();
	DenseVector<ScalarType, 2> Position;
	Position[0] = ScalarType(double(temp_x)*R[0] + double(temp_y)*R[1] + double(temp_z)*R[2]);
	Position[1] = ScalarType(double(temp_x)*R[3] + double(temp_y)*R[4] + double(temp_z)*R[5]);
	return Position;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ImageCoordinateTransform_3DWorldPositionTo2DPosition(ScalarType x, ScalarType y, ScalarType z, const ImageInfo2D& Info)
{
	return ImageCoordinateTransform_3DWorldPositionTo2DPosition(x, y, z, Info.Orientation, Info.Origin[0], Info.Origin[1], Info.Origin[2]);
}

}//namespace mdk


#endif