#ifndef __mdkLocalCoordinateSystem3D_hpp
#define __mdkLocalCoordinateSystem3D_hpp

namespace mdk
{
// same point, different coordinate in local/global coordinate System
template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, ScalarType z,
                                                                   const LocalCoordinateSystem3DWithUnitSpacing<ScalarType>& LocalSys)
{
	DenseVector<ScalarType, 3> GlobalPosition;
	GlobalPosition[0] = LocalSys.Origin[0] + x*LocalSys.DirectionX[0] + y*LocalSys.DirectionY[0] + z*LocalSys.DirectionZ[0];
	GlobalPosition[1] = LocalSys.Origin[1] + x*LocalSys.DirectionX[1] + y*LocalSys.DirectionY[1] + z*LocalSys.DirectionZ[1];
	GlobalPosition[2] = LocalSys.Origin[2] + x*LocalSys.DirectionX[2] + y*LocalSys.DirectionY[2] + z*LocalSys.DirectionZ[2];
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

	LocalPosition[0] = temp_x*LocalSys.DirectionX[0] + temp_y*LocalSys.DirectionX[1] + temp_z*LocalSys.DirectionX[2];
	LocalPosition[1] = temp_x*LocalSys.DirectionY[0] + temp_y*LocalSys.DirectionY[1] + temp_z*LocalSys.DirectionY[2];
	LocalPosition[2] = temp_x*LocalSys.DirectionZ[0] + temp_y*LocalSys.DirectionZ[1] + temp_z*LocalSys.DirectionZ[2];

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
	
	GlobalPosition[0] = LocalSys.Origin[0] 
		                + x*LocalSys.Spacing[0]*LocalSys.DirectionX[0] + y*LocalSys.Spacing[1]*LocalSys.DirectionY[0] + z*LocalSys.Spacing[2]*LocalSys.DirectionZ[0];

	GlobalPosition[1] = LocalSys.Origin[1]
		                + x*LocalSys.Spacing[0]*LocalSys.DirectionX[1] + y*LocalSys.Spacing[1]*LocalSys.DirectionY[1] + z*LocalSys.Spacing[2]*LocalSys.DirectionZ[1];

	GlobalPosition[2] = LocalSys.Origin[2]
						+ x*LocalSys.Spacing[0]*LocalSys.DirectionX[2] + y*LocalSys.Spacing[1]*LocalSys.DirectionY[2] + z*LocalSys.Spacing[2]*LocalSys.DirectionZ[2];
	
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

	LocalPosition[0] = temp_x*LocalSys.DirectionX[0] + temp_y*LocalSys.DirectionX[1] + temp_z*LocalSys.DirectionX[2];
	LocalPosition[0] /= LocalSys.Spacing[0];

	LocalPosition[1] = temp_x*LocalSys.DirectionY[0] + temp_y*LocalSys.DirectionY[1] + temp_z*LocalSys.DirectionY[2];
	LocalPosition[1] /= LocalSys.Spacing[1];

	LocalPosition[2] = temp_x*LocalSys.DirectionZ[0] + temp_y*LocalSys.DirectionZ[1] + temp_z*LocalSys.DirectionZ[2];
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