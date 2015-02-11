#ifndef mdk_Geometry3D_Function_Part_1_hpp
#define mdk_Geometry3D_Function_Part_1_hpp

namespace mdk
{
template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const DenseMatrix<ScalarType>& Point,
													 const DenseMatrix<ScalarType>& PlaneOrigin,
													 const DenseMatrix<ScalarType>& PlaneNormal)
{
	if (Point.IsVector() == false || PlaneOrigin.IsVector() == false || PlaneNormal.IsVector() == false)
	{
		MDK_Error("Input is not vector @ mdkGeometry3D ComputeSignedDistanceBetweenPointAndPlane(...)")
		return GetNaNElement<ScalarType>();
	}
	if (Point.GetElementNumber() != 3 || PlaneOrigin.GetElementNumber() != 3 || PlaneNormal.GetElementNumber() != 3)
	{
		MDK_Error("Input Vector length is not 3 @ mdkGeometry3D ComputeSignedDistanceBetweenPointAndPlane(...)")
		return GetNaNElement<ScalarType>();
	}

	return ComputeSignedDistanceBetweenPointAndPlane(Point.GetElementPointer(), PlaneOrigin.GetElementPointer(), PlaneNormal.GetElementPointer());
}


template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const DenseVector<ScalarType, 3>& Point,
													 const DenseVector<ScalarType, 3>& PlaneOrigin,
													 const DenseVector<ScalarType, 3>& PlaneNormal)
{
	return ComputeSignedDistanceBetweenPointAndPlane(Point.GetElementPointer(), PlaneOrigin.GetElementPointer(), PlaneNormal.GetElementPointer());
}

template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const ScalarType* Point, const ScalarType* PlaneOrigin, const ScalarType* PlaneNormal)
{ // PlaneNormal must have unit length

	if (Point == nullptr || PlaneOrigin == nullptr || PlaneNormal == nullptr)
	{
		MDK_Error("Input is nullptr @ mdkGeometry3D ComputeSignedDistanceBetweenPointAndPlane(...)")
		return GetNaNElement<ScalarType>();
	}
	 
	DenseVector<ScalarType, 3> Displacement;
	Displacement[0] = Point[0] - PlaneOrigin[0];
	Displacement[1] = Point[1] - PlaneOrigin[1];
	Displacement[2] = Point[2] - PlaneOrigin[2];

	// dot product between Displacement and PlaneNormal
	auto DotProduct = Displacement[0] * PlaneNormal[0] + Displacement[1] * PlaneNormal[1] + Displacement[2] * PlaneNormal[2];	
	return DotProduct;
}


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToPlane(const DenseVector<ScalarType, 3>& Point,
											const DenseMatrix<ScalarType>& PlaneOrigin,
										    const DenseMatrix<ScalarType>& PlaneNormal)
{
	if (Point.IsVector() == false || PlaneOrigin.IsVector() == false || PlaneNormal.IsVector() == false)
	{
		MDK_Error("Input is not vector @ mdkGeometry3D ProjectPointToPlane(...)")
		return GetNaNElement<ScalarType>();
	}
	if (Point.GetElementNumber() != 3 || PlaneOrigin.GetElementNumber() != 3 || PlaneNormal.GetElementNumber() != 3)
	{
		MDK_Error("Input Vector length is not 3 @ mdkGeometry3D ProjectPointToPlane(...)")
		return GetNaNElement<ScalarType>();
	}

	return ProjectPointToPlane(Point.GetElementPointer(), PlaneOrigin.GetElementPointer(), PlaneNormal.GetElementPointer());
}

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToPlane(const DenseVector<ScalarType, 3>& Point,
											   const DenseVector<ScalarType, 3>& PlaneOrigin,
											   const DenseVector<ScalarType, 3>& PlaneNormal)
{
	return ProjectPointToPlane(Point.GetElementPointer(), PlaneOrigin.GetElementPointer(), PlaneNormal.GetElementPointer());
}

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToPlane(const ScalarType* Point, const ScalarType* PlaneOrigin, const ScalarType* PlaneNormal)
{
	DenseVector<ScalarType, 3> ProjectedPoint;

	if (Point == nullptr || PlaneOrigin == nullptr || PlaneNormal == nullptr)
	{
		MDK_Error("Input is nullptr @ mdkGeometry3D ProjectPointToPlane(...)")
		ProjectedPoint.Fill(GetNaNElement<ScalarType>());
		return ProjectedPoint;
	}

	DenseVector<ScalarType, 3> Displacement;
	Displacement[0] = Point[0] - PlaneOrigin[0];
	Displacement[1] = Point[1] - PlaneOrigin[1];
	Displacement[2] = Point[2] - PlaneOrigin[2];

	// dot product between Displacement and PlaneNormal
	auto DotProduct = Displacement[0] * PlaneNormal[0] + Displacement[1] * PlaneNormal[1] + Displacement[2] * PlaneNormal[2];

	ProjectedPoint[0] = Point[0] - DotProduct*PlaneNormal[0];
	ProjectedPoint[1] = Point[1] - DotProduct*PlaneNormal[1];
	ProjectedPoint[2] = Point[2] - DotProduct*PlaneNormal[2];

	return ProjectedPoint;
}

}// namespace mdk

#endif