#pragma once

namespace mdk
{
// same point, different coordinate in local/global coordinate System
template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, const LocalCoordinateSystem2DWithUnitSpacing<ScalarType>& LocalSys)
{
	auto DirectionX = LocalSys.Orientation.GetPointerOfCol(0);
	auto DirectionY = LocalSys.Orientation.GetPointerOfCol(1);
	DenseVector<ScalarType, 2> GlobalPosition;
	GlobalPosition[0] = LocalSys.Origin[0] + x*DirectionX[0] + y*DirectionY[0];
	GlobalPosition[1] = LocalSys.Origin[1] + x*DirectionX[1] + y*DirectionY[1];
	return GlobalPosition;
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ConvertPointCoordinateFromLocalToGlobal(const DenseVector<ScalarType, 2>& LocalPosition, const LocalCoordinateSystem2DWithUnitSpacing<ScalarType>& LocalSys)
{
	return ConvertPointCoordinateFromLocalToGlobal(LocalPosition[0], LocalPosition[1], LocalSys);
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 2> ConvertPointCoordinateFromGlobalToLocal(ScalarType x, ScalarType y, const LocalCoordinateSystem2DWithUnitSpacing<ScalarType>& LocalSys)

{
	DenseVector<ScalarType, 2> LocalPosition;
	auto temp_x = x - LocalSys.Origin[0];
	auto temp_y = y - LocalSys.Origin[1];
	auto DirectionX = LocalSys.Orientation.GetPointerOfCol(0);
	auto DirectionY = LocalSys.Orientation.GetPointerOfCol(1);
	LocalPosition[0] = temp_x*DirectionX[0] + temp_y*DirectionX[1];
	LocalPosition[1] = temp_x*DirectionY[0] + temp_y*DirectionY[1];
	return LocalPosition;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 2> ConvertPointCoordinateFromGlobalToLocal(const DenseVector<ScalarType, 2>& GlobalPosition, const LocalCoordinateSystem2DWithUnitSpacing<ScalarType>& LocalSys)
{
	return ConvertPointCoordinateFromGlobalToLocal(GlobalPosition[0], GlobalPosition[1], LocalSys);
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 2> ConvertPointCoordinateFromLocalToGlobal(ScalarType x, ScalarType y, const LocalCoordinateSystem2D<ScalarType>& LocalSys)
{
	DenseVector<ScalarType, 2> GlobalPosition;
	auto temp_x = x*LocalSys.Spacing[0];
	auto temp_y = y*LocalSys.Spacing[1];
	auto DirectionX = LocalSys.Orientation.GetPointerOfCol(0);
	auto DirectionY = LocalSys.Orientation.GetPointerOfCol(1);	
	GlobalPosition[0] = LocalSys.Origin[0] + temp_x*DirectionX[0] + temp_y*DirectionY[0];
	GlobalPosition[1] = LocalSys.Origin[1] + temp_x*DirectionX[1] + temp_y*DirectionY[1];	
	return GlobalPosition;
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ConvertPointCoordinateFromLocalToGlobal(const DenseVector<ScalarType, 2>& LocalPosition, const LocalCoordinateSystem2D<ScalarType>& LocalSys)
{
	return ConvertPointCoordinateFromLocalToGlobal(LocalPosition[0], LocalPosition[1], LocalSys);
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ConvertPointCoordinateFromGlobalToLocal(ScalarType x, ScalarType y, const LocalCoordinateSystem2D<ScalarType>& LocalSys)
{
	DenseVector<ScalarType, 2> LocalPosition;
	auto temp_x = x - LocalSys.Origin[0];
	auto temp_y = y - LocalSys.Origin[1];
	auto DirectionX = LocalSys.Orientation.GetPointerOfCol(0);
	auto DirectionY = LocalSys.Orientation.GetPointerOfCol(1);
	LocalPosition[0] = temp_x*DirectionX[0] + temp_y*DirectionX[1];
	LocalPosition[0] /= LocalSys.Spacing[0];
	LocalPosition[1] = temp_x*DirectionY[0] + temp_y*DirectionY[1];
	LocalPosition[1] /= LocalSys.Spacing[1];
	return LocalPosition;
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ConvertPointCoordinateFromGlobalToLocal(const DenseVector<ScalarType, 2>& GlobalPosition, const LocalCoordinateSystem2D<ScalarType>& LocalSys)
{
	return ConvertPointCoordinateFromGlobalToLocal(GlobalPosition[0], GlobalPosition[1], LocalSys);
}

}// namespace mdk
