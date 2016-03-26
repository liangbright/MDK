#pragma once

namespace mdk
{
// ScalarType is float or double

template<typename ScalarType>
struct LocalCoordinateSystem3DWithUnitSpacing
{
	DenseVector<ScalarType, 3> Origin;

	DenseMatrix<ScalarType> Orientation;
	//Col-0: DirectionX
	//Col-1: DirectionY
	//Col-2: DirectionZ

	// Spacing is [1, 1, 1]

//-----------------------------------------------------------------
	LocalCoordinateSystem3DWithUnitSpacing() { this->Clear(); }
	~LocalCoordinateSystem3DWithUnitSpacing() {}

	void operator=(const LocalCoordinateSystem3DWithUnitSpacing& InputSys)
	{
		Origin = InputSys.Origin;
		Orientation = InputSys.Orientation;
	}

	void Clear()
	{
		Origin.Fill(0);
		Orientation.Clear();
		Orientation.Resize(3, 3);
		Orientation.Fill(0);
		Orientation.FixSize();
	}
};

template<typename ScalarType>
struct LocalCoordinateSystem3D
{
	DenseVector<ScalarType, 3> Origin;

	DenseVector<ScalarType, 3> Orientation;
	//Col-0: DirectionX
	//Col-1: DirectionY
	//Col-2: DirectionZ

	DenseVector<ScalarType, 3> Spacing;
//------------------------------------------------------------
	LocalCoordinateSystem3D() { this->Clear(); }
	~LocalCoordinateSystem3D() {}

	void operator=(const LocalCoordinateSystem3D& InputSys)
	{
		Origin = InputSys.Origin;
		Orientation = InputSys.Orientation;
		Spacing = InputSys.Spacing;
	}

	void Clear()
	{
		Origin.Fill(0);
		Orientation.Clear();
		Orientation.Resize(3, 3);
		Orientation.Fill(0);
		Orientation.FixSize();
		Spacing.Fill(1);
	}
};

// same point, different coordinate in local/global coordinate System
template<typename ScalarType>
inline DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, ScalarType z,
	                                                                      const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(const DenseVector<ScalarType, 3>& LocalPosition,
	                                                                      const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ConvertPointCoordinateFromGlobalToLocal(ScalarType x, ScalarType y, ScalarType z,
																		  const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ConvertPointCoordinateFromGlobalToLocal(const DenseVector<ScalarType, 3>& GlobalPosition,
																		  const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, ScalarType z,
	                                                                      const LocalCoordinateSystem3D<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(const DenseVector<ScalarType, 3>& LocalPosition,
	                                                                      const LocalCoordinateSystem3D<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ConvertPointCoordinateFromGlobalToLocal(ScalarType x, ScalarType y, ScalarType z,
																		   const LocalCoordinateSystem3D<ScalarType>& LocalSys);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ConvertPointCoordinateFromGlobalToLocal(const DenseVector<ScalarType, 3>& GlobalPosition,
																		  const LocalCoordinateSystem3D<ScalarType>& LocalSys);

}// namespace mdk

#include "mdkLocalCoordinateSystem3D.hpp"
