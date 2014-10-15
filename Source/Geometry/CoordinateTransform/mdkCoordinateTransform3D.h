#ifndef __mdkCoordinateTransform3D_h
#define __mdkCoordinateTransform3D_h

#include "mdkProcessObject.h"

namespace mdk
{
template<typename Scalar_Type>
class CoordinateTransform3D : public ProcessObject
{
public:
	typedef Scalar_Type ScalarType;

protected:
	CoordinateTransform3D() {}
	virtual ~CoordinateTransform3D() {}

	virtual DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const = 0;
	virtual DenseVector<ScalarType, 3> TransformPoint(const DenseVector<ScalarType, 3>& PhysicalPosition) const = 0;

private:
	CoordinateTransform3D(const CoordinateTransform3D&) = delete;
	void operator=(const CoordinateTransform3D&) = delete;
};

}

#endif