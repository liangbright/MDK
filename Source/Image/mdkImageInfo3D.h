#ifndef mdk_ImageInfo3D_h
#define mdk_ImageInfo3D_h

namespace mdk
{
struct BoxRegionOf3DIndexInImage3D
{ 
	double x_min;
	double y_min;
	double z_min;

	double x_max;
	double y_max;
	double z_max;

//-------------------------------------
    BoxRegionOf3DIndexInImage3D()
    {
		x_min = 0;
		y_min = 0;
		z_min = 0;
		x_max = 0;
		y_max = 0;
		z_max = 0;
    };

	double Lx() const
    {
		return x_max - x_min + 1;
    }

	double Ly() const
    {
		return y_max - y_min + 1;
    }

	double Lz() const
    {
		return z_max - z_min + 1;
    }
};


struct BoxRegionOf3DPositionInImage3D
{
	double x_min;
	double y_min;
	double z_min;

	double x_max;
	double y_max;
	double z_max;

//-------------------------------------

	BoxRegionOf3DPositionInImage3D()
    {
		x_min = 0;
		y_min = 0;
		z_min = 0;
		x_max = 0;
		y_max = 0;
		z_max = 0;
    };

	double Lx() const
    {
		return x_max - x_min;
    }

	double Ly() const
    {
		return y_max - y_min;
    }

	double Lz() const
    {
		return z_max - z_min;
    }
};


struct BoxRegionOf3DWorldPositionInImage3D
{
	double x_min;
	double y_min;
	double z_min;

	double x_max;
	double y_max;
	double z_max;

	//-------------------------------------

	BoxRegionOf3DWorldPositionInImage3D()
	{
		x_min = 0;
		y_min = 0;
		z_min = 0;
		x_max = 0;
		y_max = 0;
		z_max = 0;
	};

	double Lx() const
	{
		return x_max - x_min;
	}

	double Ly() const
	{
		return y_max - y_min;
	}

	double Lz() const
	{
		return z_max - z_min;
	}
};


struct ImageInfo3D
{
	DenseVector<int_max, 3> Size;       // {Lx, Ly, Lz} number of Pixels in each direction
	DenseVector<double, 3>  Spacing;    // Pixel Spacing of DICOM DenseImage in world coordinate system {Sx, Sy, Sz} (unit: mm)
	DenseVector<double, 3>  Origin;     // Origin of DICOM DenseImage in world coordinate system (x,y,z) (unit: mm)

	DenseMatrix<double> Orientation;    // 3x3 Matrix
	// column-0: DirectionX
	// column-1: DirectionY
	// Column-2: DirectionZ

	DenseMatrix<double> TransformMatrix_3DIndexTo3DWorld;
	DenseMatrix<double> TransformMatrix_3DWorldTo3DIndex;
	// column-0: DirectionX * SpacingX
	// column-1: DirectionY * SpacingY
	// Column-2: DirectionZ * SpacingZ

//-------------------------------------------
	ImageInfo3D() { this->Clear(); }
	~ImageInfo3D() {}

	ImageInfo3D(const ImageInfo3D& Info)
	{
		(*this) = Info;
	}

	void operator=(const ImageInfo3D& Info)
	{
		Size = Info.Size;
		Spacing = Info.Spacing;
		Origin = Info.Origin;
		Orientation = Info.Orientation;
		TransformMatrix_3DIndexTo3DWorld = Info.TransformMatrix_3DIndexTo3DWorld;
		TransformMatrix_3DWorldTo3DIndex = Info.TransformMatrix_3DWorldTo3DIndex;
	}

	void Clear()
	{
		Size.Fill(0);
		Spacing.Fill(0);
		Origin.Fill(0);
		Orientation.Clear();
		Orientation.Resize(3, 3);
		Orientation.FixSize();
		Orientation.FillDiagonal(1.0);
	}
};

//================================================ ImageCoordinateTransform ============================================================//

//--------------------------- transform between image space and world space -----------------------------

template<typename ScalarType = int_max>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DIndex(int_max LinearIndex, int_max ImageSizeX, int_max ImageSizeY);

template<typename ScalarType = int_max>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DIndex(int_max LinearIndex, const Image3DInfo& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DPosition(int_max LinearIndex, double SpacingX, double SpacingY, double SpacingZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DPosition(int_max LinearIndex, const Image3DInfo& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransforme_LinearIndexTo3DWorldPosition(int_max LinearIndex, int_max ImageSizeX, int_max ImageSizeY,
                                                                                         const DenseMatrix<double>& TransformMatrix_3DIndexTo3DWorld,
								                                                         double OriginX, double OriginY, double OriginZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransforme_LinearIndexTo3DWorldPosition(int_max LinearIndex, const Image3DInfo& Info);

//3DIndex is discrete
inline int_max ImageCoordinateTransform_3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex, int_max ImageSizeX, int_max ImageSizeY);

inline int_max ImageCoordinateTransform_3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex, const Image3DInfo& Info);

// 3DIndex may be continuous
template<typename ScalarType_Position, typename ScalarType_Index>
inline DenseVector<ScalarType_Position, 3> 
ImageCoordinateTransform_3DIndexTo3DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex, 
											 double SpacingX, double SpacingY, double SpacingZ);

template<typename ScalarType_Position, typename ScalarType_Index>
inline DenseVector<ScalarType_Position, 3> 
ImageCoordinateTransform_3DIndexTo3DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex, const Image3DInfo& Info);

template<typename ScalarType_Position, typename ScalarType_Index>
inline DenseVector<ScalarType_Position, 3> 
ImageCoordinateTransform_3DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex,
												  const DenseMatrix<double>& TransformMatrix_3DIndexTo3DWorld,
											      double OriginX, double OriginY, double OriginZ);

template<typename ScalarType_Position, typename ScalarType_Index>
inline DenseVector<ScalarType_Position, 3> 
ImageCoordinateTransform_3DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex, const Image3DInfo& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_3DPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z, 
																			   double SpacingX, double SpacingY, double SpacingZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_3DPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z, const Image3DInfo& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_3DPositionTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z,
																	                   const DenseMatrix<double>& Oirentation,
																	                   double OriginX, double OriginY, double OriginZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_3DPositionTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z, const Image3DInfo& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_3DWorldPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z,
																		            const DenseMatrix<double>& TransformMatrix_3DWorldTo3DIndex,
	                                                                                double OriginX, double OriginY, double OriginZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_3DWorldPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z, const Image3DInfo& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_3DWorldPositionTo3DPosition(ScalarType x, ScalarType y, ScalarType z,
																					   const DenseMatrix<double>& Orientation,
	                                                                                   double OriginX, double OriginY, double OriginZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_3DWorldPositionTo3DPosition(ScalarType x, ScalarType y, ScalarType z, const Image3DInfo& Info);

//--------------------------- transform between two image space, assume the same world space -----------------------------
template<typename ScalarType>
inline DenseVector<ScalarType, 3>
ImageCoordinateTransform_3DPositionInImageATo3DPositionInImageB(ScalarType x, ScalarType y, ScalarType z, const Image3DInfo& InfoA, const Image3DInfo& InfoB);


}//namespace mdk

#include "mdkImageInfo3D.hpp"

#endif