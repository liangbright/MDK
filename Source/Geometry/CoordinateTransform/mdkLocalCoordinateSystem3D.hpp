#ifndef mdk_LocalCoordinateSystem3D_hpp
#define mdk_LocalCoordinateSystem3D_hpp

namespace mdk
{
// same point, different coordinate in local/global coordinate System
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, ScalarType z,
                                                                   const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys)
{
	auto DirectionX = LocalSys.Orientation.GetPointerOfCol(0);
	auto DirectionY = LocalSys.Orientation.GetPointerOfCol(1);
	auto DirectionZ = LocalSys.Orientation.GetPointerOfCol(2);

	DenseVector<ScalarType, 3> GlobalPosition;
	GlobalPosition[0] = LocalSys.Origin[0] + x*DirectionX[0] + y*DirectionY[0] + z*DirectionZ[0];
	GlobalPosition[1] = LocalSys.Origin[1] + x*DirectionX[1] + y*DirectionY[1] + z*DirectionZ[1];
	GlobalPosition[2] = LocalSys.Origin[2] + x*DirectionX[2] + y*DirectionY[2] + z*DirectionZ[2];
	return GlobalPosition;
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(const DenseVector<ScalarType, 3>& LocalPosition,
                                                                   const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys)
{
	return ConvertPointCoordinateFromLocalToGlobal(LocalPosition[0], LocalPosition[1], LocalPosition[2], LocalSys);
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ConvertPointCoordinateFromGlobalToLocal(ScalarType x, ScalarType y, ScalarType z,
																   const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys)

{
	DenseVector<ScalarType, 3> LocalPosition;

	auto temp_x = x - LocalSys.Origin[0];
	auto temp_y = y - LocalSys.Origin[1];
	auto temp_z = z - LocalSys.Origin[2];

	auto DirectionX = LocalSys.Orientation.GetPointerOfCol(0);
	auto DirectionY = LocalSys.Orientation.GetPointerOfCol(1);
	auto DirectionZ = LocalSys.Orientation.GetPointerOfCol(2);

	LocalPosition[0] = temp_x*DirectionX[0] + temp_y*DirectionX[1] + temp_z*DirectionX[2];
	LocalPosition[1] = temp_x*DirectionY[0] + temp_y*DirectionY[1] + temp_z*DirectionY[2];
	LocalPosition[2] = temp_x*DirectionZ[0] + temp_y*DirectionZ[1] + temp_z*DirectionZ[2];

	return LocalPosition;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ConvertPointCoordinateFromGlobalToLocal(const DenseVector<ScalarType, 3>& GlobalPosition,
																   const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys)
{
	return ConvertPointCoordinateFromGlobalToLocal(GlobalPosition[0], GlobalPosition[1], GlobalPosition[2], LocalSys);
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, ScalarType z,
	                                                               const LocalCoordinateSystem3D<ScalarType>& LocalSys)
{
	DenseVector<ScalarType, 3> GlobalPosition;
	
	auto temp_x = x*LocalSys.Spacing[0];
	auto temp_y = y*LocalSys.Spacing[1];
	auto temp_z = z*LocalSys.Spacing[2];

	auto DirectionX = LocalSys.Orientation.GetPointerOfCol(0);
	auto DirectionY = LocalSys.Orientation.GetPointerOfCol(1);
	auto DirectionZ = LocalSys.Orientation.GetPointerOfCol(2);

	GlobalPosition[0] = LocalSys.Origin[0] + temp_x*DirectionX[0] + temp_y*DirectionY[0] + temp_z*DirectionZ[0];
	GlobalPosition[1] = LocalSys.Origin[1] + temp_x*DirectionX[1] + temp_y*DirectionY[1] + temp_z*DirectionZ[1];
	GlobalPosition[2] = LocalSys.Origin[2] + temp_x*DirectionX[2] + temp_y*DirectionY[2] + temp_z*DirectionZ[2];

	return GlobalPosition;
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(const DenseVector<ScalarType, 3>& LocalPosition,
	                                                               const LocalCoordinateSystem3D<ScalarType>& LocalSys)
{
	return ConvertPointCoordinateFromLocalToGlobal(LocalPosition[0], LocalPosition[1], LocalPosition[2], LocalSys);
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ConvertPointCoordinateFromGlobalToLocal(ScalarType x, ScalarType y, ScalarType z,
																   const LocalCoordinateSystem3D<ScalarType>& LocalSys)
{
	DenseVector<ScalarType, 3> LocalPosition;

	auto temp_x = x - LocalSys.Origin[0];
	auto temp_y = y - LocalSys.Origin[1];
	auto temp_z = z - LocalSys.Origin[2];

	auto DirectionX = LocalSys.Orientation.GetPointerOfCol(0);
	auto DirectionY = LocalSys.Orientation.GetPointerOfCol(1);
	auto DirectionZ = LocalSys.Orientation.GetPointerOfCol(2);

	LocalPosition[0] = temp_x*DirectionX[0] + temp_y*DirectionX[1] + temp_z*DirectionX[2];
	LocalPosition[0] /= LocalSys.Spacing[0];

	LocalPosition[1] = temp_x*DirectionY[0] + temp_y*DirectionY[1] + temp_z*DirectionY[2];
	LocalPosition[1] /= LocalSys.Spacing[1];

	LocalPosition[2] = temp_x*DirectionZ[0] + temp_y*DirectionZ[1] + temp_z*DirectionZ[2];
	LocalPosition[2] /= LocalSys.Spacing[2];

	return LocalPosition;
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ConvertPointCoordinateFromGlobalToLocal(const DenseVector<ScalarType, 3>& GlobalPosition,
																   const LocalCoordinateSystem3D<ScalarType>& LocalSys)
{
	return ConvertPointCoordinateFromGlobalToLocal(GlobalPosition[0], GlobalPosition[1], GlobalPosition[2], LocalSys);
}

}// namespace mdk

#endif