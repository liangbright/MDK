#ifndef mdk_LocalCoordinateSystem3D_h
#define mdk_LocalCoordinateSystem3D_h

namespace mdk
{
// ScalarType is float or double

template<typename ScalarType>
struct LocalCoordinateSystem3DWithUnitSpacing
{
	DenseVector<ScalarType, 3> Origin;
	DenseVector<ScalarType, 3> DirectionX;
	DenseVector<ScalarType, 3> DirectionY;
	DenseVector<ScalarType, 3> DirectionZ;
	// Spacing is [1, 1, 1]

//-----------------------------------------------------------------
	LocalCoordinateSystem3DWithUnitSpacing() { this->Clear(); }
	~LocalCoordinateSystem3DWithUnitSpacing() {}

	void operator=(const LocalCoordinateSystem3DWithUnitSpacing& InputSys)
	{
		Origin = InputSys.Origin;
		DirectionX = InputSys.DirectionX;
		DirectionY = InputSys.DirectionY;
		DirectionZ = InputSys.DirectionZ;
	}

	void Clear()
	{
		Origin = { 0, 0, 0 };
		DirectionX = { 1, 0, 0 };
		DirectionY = { 0, 1, 0 };
		DirectionZ = { 0, 0, 1 };
	}
};

template<typename ScalarType>
struct LocalCoordinateSystem3D
{
	DenseVector<ScalarType, 3> Origin;
	DenseVector<ScalarType, 3> DirectionX;
	DenseVector<ScalarType, 3> DirectionY;
	DenseVector<ScalarType, 3> DirectionZ;
	DenseVector<ScalarType, 3> Spacing;
//------------------------------------------------------------
	LocalCoordinateSystem3D() { this->Clear(); }
	~LocalCoordinateSystem3D() {}

	void operator=(const LocalCoordinateSystem3D& InputSys)
	{
		Origin = InputSys.Origin;
		DirectionX = InputSys.DirectionX;
		DirectionY = InputSys.DirectionY;
		DirectionZ = InputSys.DirectionZ;
		Spacing = InputSys.Spacing;
	}

	void Clear()
	{
		Origin = { 0, 0, 0 };
		DirectionX = { 1, 0, 0 };
		DirectionY = { 0, 1, 0 };
		DirectionZ = { 0, 0, 1 };
		Spacing = { 1, 1, 1 };
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

#endif