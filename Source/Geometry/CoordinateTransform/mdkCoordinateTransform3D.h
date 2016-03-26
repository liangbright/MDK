#pragma once

#include "mdkDenseMatrix.h"

namespace mdk
{
template<typename Scalar_Type>
class CoordinateTransform3D : public Object
{
public:
	typedef Scalar_Type ScalarType;

protected:
	CoordinateTransform3D() {}
	virtual ~CoordinateTransform3D() {}

public:
	virtual DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const = 0;

	DenseVector<ScalarType, 3> TransformPoint(const DenseVector<ScalarType, 3>& Position) const
	{
		return this->TransformPoint(Position[0], Position[1], Position[2]);
	}

	DenseVector<ScalarType, 3> TransformPoint(const ScalarType Position[3]) const
	{
		return this->TransformPoint(Position[0], Position[1], Position[2]);
	}

private:
	CoordinateTransform3D(const CoordinateTransform3D&) = delete;
	void operator=(const CoordinateTransform3D&) = delete;
};

}
