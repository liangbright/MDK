#pragma once

namespace mdk
{
// ScalarType is float or double

template<typename ScalarType>
struct LocalCoordinateSystem2DWithUnitSpacing
{
	DenseVector<ScalarType, 2> Origin;

	DenseMatrix<ScalarType> Orientation;
	//Col-0: DirectionX
	//Col-1: DirectionY

	// Spacing is [1, 1]

//-----------------------------------------------------------------
	LocalCoordinateSystem2DWithUnitSpacing() { this->Clear(); }
	~LocalCoordinateSystem2DWithUnitSpacing() {}

	void operator=(const LocalCoordinateSystem2DWithUnitSpacing& InputSys)
	{
		Origin = InputSys.Origin;
		Orientation = InputSys.Orientation;
	}

	void Clear()
	{
		Origin.Fill(0);
		Orientation.Clear();
		Orientation.Resize(2, 2);
		Orientation.Fill(0);
		Orientation.FixSize();
	}
};

template<typename ScalarType>
struct LocalCoordinateSystem2D
{
	DenseVector<ScalarType, 2> Origin;

	DenseVector<ScalarType, 2> Orientation;
	//Col-0: DirectionX
	//Col-1: DirectionY
	//Col-2: DirectionZ

	DenseVector<ScalarType, 2> Spacing;
//------------------------------------------------------------
	LocalCoordinateSystem2D() { this->Clear(); }
	~LocalCoordinateSystem2D() {}

	void operator=(const LocalCoordinateSystem2D& InputSys)
	{
		Origin = InputSys.Origin;
		Orientation = InputSys.Orientation;
		Spacing = InputSys.Spacing;
	}

	void Clear()
	{
		Origin.Fill(0);
		Orientation.Clear();
		Orientation.Resize(2, 2);
		Orientation.Fill(0);
		Orientation.FixSize();
		Spacing.Fill(1);
	}
};

// same point, different coordinate in local/global coordinate System
template<typename ScalarType>
inline DenseVector<ScalarType, 2> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, const LocalCoordinateSystem2DWithUnitSpacing<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ConvertPointCoordinateFromLocalToGlobal(const DenseVector<ScalarType, 2>& LocalPosition, const LocalCoordinateSystem2DWithUnitSpacing<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ConvertPointCoordinateFromGlobalToLocal(ScalarType x, ScalarType y, const LocalCoordinateSystem2DWithUnitSpacing<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ConvertPointCoordinateFromGlobalToLocal(const DenseVector<ScalarType, 2>& GlobalPosition, const LocalCoordinateSystem2DWithUnitSpacing<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, const LocalCoordinateSystem2D<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ConvertPointCoordinateFromLocalToGlobal(const DenseVector<ScalarType, 2>& LocalPosition,  const LocalCoordinateSystem2D<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ConvertPointCoordinateFromGlobalToLocal(ScalarType x, ScalarType y, const LocalCoordinateSystem2D<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 2> ConvertPointCoordinateFromGlobalToLocal(const DenseVector<ScalarType, 2>& GlobalPosition, const LocalCoordinateSystem2D<ScalarType>& LocalSys);

}// namespace mdk

#include "mdkLocalCoordinateSystem2D.hpp"
