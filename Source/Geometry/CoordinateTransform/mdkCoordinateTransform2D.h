#pragma once

#include "mdkDenseMatrix.h"

namespace mdk
{
template<typename Scalar_Type>
class CoordinateTransform2D : public Object
{
public:
	typedef Scalar_Type ScalarType;

protected:
	CoordinateTransform2D() {}
	virtual ~CoordinateTransform2D() {}

public:
	virtual DenseVector<ScalarType, 2> TransformPoint(ScalarType x, ScalarType y) const = 0;

	DenseVector<ScalarType, 2> TransformPoint(const DenseVector<ScalarType, 2>& Position) const
	{
		return this->TransformPoint(Position[0], Position[1]);
	}

	DenseVector<ScalarType, 2> TransformPoint(const ScalarType Position[2]) const
	{
		return this->TransformPoint(Position[0], Position[1]);
	}

private:
	CoordinateTransform2D(const CoordinateTransform2D&) = delete;
	void operator=(const CoordinateTransform2D&) = delete;
};

}
