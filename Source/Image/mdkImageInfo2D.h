#ifndef mdk_ImageInfo2D_h
#define mdk_ImageInfo2D_h

#include <memory>

#include "mdkZeroPixel.h"

namespace mdk
{
struct BoxRegionOf2DIndexInImage2D
{ 
	double x_min;
	double y_min;

	double x_max;
	double y_max;

//-------------------------------------
    BoxRegionOf2DIndexInImage2D()
    {
		x_min = 0;
		y_min = 0;
		x_max = 0;
		y_max = 0;
    };

	double Lx() const
    {
		return x_max - x_min + 1;
    }

	double Ly() const
    {
		return y_max - y_min + 1;
    }
};


struct BoxRegionOf2DPositionInImage2D
{
	double x_min;
	double y_min;

	double x_max;
	double y_max;
//-------------------------------------

	BoxRegionOf2DPositionInImage2D()
    {
		x_min = 0;
		y_min = 0;
		x_max = 0;
		y_max = 0;
    };

	double Lx() const
    {
		return x_max - x_min;
    }

	double Ly() const
    {
		return y_max - y_min;
    }
};


struct BoxRegionOf3DWorldPositionInImage2D
{
	double x_min;
	double y_min;
	double z_min;

	double x_max;
	double y_max;
	double z_max;
	//-------------------------------------

	BoxRegionOf3DWorldPositionInImage2D()
	{
		x_min = 0;
		y_min = 0;
		x_max = 0;
		y_max = 0;
		z_min = 0;
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


struct ImageInfo2D
{// 2D Image in 3D space, e.g., 2D Slice of 3D image
	DenseVector<int_max, 2> Size;       // {Lx, Ly} number of Pixels in each direction
	DenseVector<double, 2>  Spacing;    // Pixel Spacing (unit: mm)
	DenseVector<double, 3>  Origin;     // position at Image(0,0) in world coordinate system (x,y,z)

	DenseMatrix<double> Orientation;    // Orientation in world coordinate system
	// column-0: DirectionX
	// column-1: DirectionY
	// column-2: DirectionZ

	DenseMatrix<double> TransformMatrix_2DIndexTo3DWorld; // 3x2
	// column-0: DirectionX*Spacing[0]
	// column-1: DirectionY*Spacing[1]

	DenseMatrix<double> TransformMatrix_3DWorldTo2DIndex; // 2x3

//-------------------------------------------
	ImageInfo2D() { this->Clear(); }
	~ImageInfo2D() {}

	void operator=(const ImageInfo2D& Info)
	{
		Size = Info.Size;
		Spacing = Info.Spacing;
		Origin = Info.Origin;
		Orientation = Info.Orientation;
		TransformMatrix_2DIndexTo3DWorld = Info.TransformMatrix_2DIndexTo3DWorld;
		TransformMatrix_3DWorldTo2DIndex = Info.TransformMatrix_3DWorldTo2DIndex;
	}

	void operator=(ImageInfo2D&& Info)
	{
		Size = Info.Size;
		Spacing = Info.Spacing;
		Origin = Info.Origin;
		Orientation = std::move(Info.Orientation);
		TransformMatrix_2DIndexTo3DWorld = std::move(Info.TransformMatrix_2DIndexTo3DWorld);
		TransformMatrix_3DWorldTo2DIndex = std::move(Info.TransformMatrix_3DWorldTo2DIndex);
	}

	void Clear()
	{
		Size = { 0, 0};
		Origin = { 0.0, 0.0, 0.0 };
		Spacing = { 1.0, 1.0};

		Orientation = { { 1.0, 0.0, 0.0 },
						{ 0.0, 1.0, 0.0 },
						{ 0.0, 0.0, 1.0 } };
		Orientation.FixSize();

		TransformMatrix_2DIndexTo3DWorld = { { 1.0, 0.0 },
											 { 0.0, 1.0 },
											 { 0.0, 0.0 } };
		TransformMatrix_2DIndexTo3DWorld.FixSize();

		TransformMatrix_3DWorldTo2DIndex = { { 1.0, 0.0, 0.0 },
											 { 0.0, 1.0, 0.0 }};
		TransformMatrix_3DWorldTo2DIndex.FixSize();
	}

	void UpdateTransformMatrix()
	{
		auto R = Orientation.GetElementPointer();
		TransformMatrix_2DIndexTo3DWorld.SetCol(0, { Spacing[0] * R[0], Spacing[0] * R[1], Spacing[0] * R[2] });
		TransformMatrix_2DIndexTo3DWorld.SetCol(1, { Spacing[1] * R[3], Spacing[1] * R[4], Spacing[1] * R[5] });
		// inverse transform
		double S0 = 1.0 / Spacing[0];
		double S1 = 1.0 / Spacing[1];
		TransformMatrix_3DWorldTo2DIndex.SetCol(0, { S0*R[0], S1*R[3]});
		TransformMatrix_3DWorldTo2DIndex.SetCol(1, { S0*R[1], S1*R[4]});
		TransformMatrix_3DWorldTo2DIndex.SetCol(2, { S0*R[2], S1*R[5]});
	}
};


//================================================ ImageCoordinateTransform ============================================================//

//--------------------------- transform between image space and world space -----------------------------

template<typename ScalarType = int_max>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_LinearIndexTo2DIndex(int_max LinearIndex, int_max ImageSizeX);

template<typename ScalarType = int_max>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_LinearIndexTo2DIndex(int_max LinearIndex, const ImageInfo2D& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_LinearIndexTo2DPosition(int_max LinearIndex, int_max ImageSizeX, double SpacingX, double SpacingY);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_LinearIndexTo2DPosition(int_max LinearIndex, const ImageInfo2D& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DWorldPosition(int_max LinearIndex, int_max ImageSizeX,
                                                                                        const DenseMatrix<double>& TransformMatrix_2DIndexTo3DWorld,
								                                                        double OriginX, double OriginY, double OriginZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_LinearIndexTo3DWorldPosition(int_max LinearIndex, const ImageInfo2D& Info);

//2DIndex is discrete
inline int_max ImageCoordinateTransform_2DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max ImageSizeX);

inline int_max ImageCoordinateTransform_2DIndexToLinearIndex(int_max xIndex, int_max yIndex, const ImageInfo2D& Info);

// 2DIndex may be continuous
template<typename ScalarType_Position, typename ScalarType_Index>
inline DenseVector<ScalarType_Position, 2> 
ImageCoordinateTransform_2DIndexTo2DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, double SpacingX, double SpacingY);

template<typename ScalarType_Position, typename ScalarType_Index>
inline DenseVector<ScalarType_Position, 2> 
ImageCoordinateTransform_2DIndexTo2DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, const ImageInfo2D& Info);

template<typename ScalarType_Position, typename ScalarType_Index>
inline DenseVector<ScalarType_Position, 3> 
ImageCoordinateTransform_2DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex,
												  const DenseMatrix<double>& TransformMatrix_2DIndexTo3DWorld,
											      double OriginX, double OriginY, double OriginZ);

template<typename ScalarType_Position, typename ScalarType_Index>
inline DenseVector<ScalarType_Position, 3> 
ImageCoordinateTransform_2DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, const ImageInfo2D& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_2DPositionTo2DIndex(ScalarType x, ScalarType y, double SpacingX, double SpacingY);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_2DPositionTo2DIndex(ScalarType x, ScalarType y, const ImageInfo2D& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_2DPositionTo3DWorldPosition(ScalarType x, ScalarType y,
																	                   const DenseMatrix<double>& Oirentation,
																	                   double OriginX, double OriginY, double OriginZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ImageCoordinateTransform_2DPositionTo3DWorldPosition(ScalarType x, ScalarType y, const ImageInfo2D& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_3DWorldPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z,
																		            const DenseMatrix<double>& TransformMatrix_3DWorldTo2DIndex,
	                                                                                double OriginX, double OriginY, double OriginZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_3DWorldPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z, const ImageInfo2D& Info);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_3DWorldPositionTo2DPosition(ScalarType x, ScalarType y, ScalarType z,
																					   const DenseMatrix<double>& Orientation,
	                                                                                   double OriginX, double OriginY, double OriginZ);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ImageCoordinateTransform_3DWorldPositionTo2DPosition(ScalarType x, ScalarType y, ScalarType z, const ImageInfo2D& Info);

}//namespace mdk

#include "mdkImageInfo2D.hpp"

#endif