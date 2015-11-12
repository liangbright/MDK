#pragma once

namespace mdk
{

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const DenseMatrix<ScalarType>& VectorA, const DenseMatrix<ScalarType>& VectorB)
{
    DenseVector<ScalarType, 3> Vector_AxB;

    if (VectorA.GetElementNumber() != 3 || VectorB.GetElementNumber() != 3)
    {
        MDK_Error("Input is not 3D vector @ mdkGeometry3D ComputeVectorCrossProductIn3D(...)")
        Vector_AxB.Fill(GetNaNElement<ScalarType>());
        return Vector_AxB();
    }

    Vector_AxB = ComputeVectorCrossProductIn3D(VectorA.GetElementPointer(), VectorB.GetElementPointer());
    return Vector_AxB;
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const DenseVector<ScalarType, 3>& VectorA, const DenseVector<ScalarType, 3>& VectorB)
{
    return ComputeVectorCrossProductIn3D(VectorA.GetElementPointer(), VectorB.GetElementPointer());
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const ScalarType* VectorA, const ScalarType* VectorB)
{
    DenseVector<ScalarType, 3> Vector_AxB;
    if (VectorA == nullptr || VectorB == nullptr)
    {
        MDK_Error("Input is nullptr @ mdkGeometry3D ComputeVectorCrossProductIn3D(...)")
        Vector_AxB.Fill(GetNaNElement<ScalarType>());
        return Vector_AxB;
    }

    Vector_AxB[0] = VectorA[1] * VectorB[2] - VectorA[2] * VectorB[1];
    Vector_AxB[1] = VectorA[2] * VectorB[0] - VectorA[0] * VectorB[2];
    Vector_AxB[2] = VectorA[0] * VectorB[1] - VectorA[1] * VectorB[0];
    return Vector_AxB;
}


template<typename ScalarType>
inline ScalarType ComputeVectorDotProductIn3D(const DenseMatrix<ScalarType>& VectorA, const DenseMatrix<ScalarType>& VectorB)
{
	if (VectorA.GetElementNumber() != 3 || VectorB.GetElementNumber() != 3)
	{
		MDK_Error("Input is not 3D vector @ mdkGeometry3D ComputeVectorDotProductIn3D(...)")			
		return 0;
	}

	return VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2];
}

template<typename ScalarType>
inline ScalarType ComputeVectorDotProductIn3D(const DenseVector<ScalarType, 3>& VectorA, const DenseVector<ScalarType, 3>& VectorB)
{
	return VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2];
}

template<typename ScalarType>
inline ScalarType ComputeVectorDotProductIn3D(const ScalarType* VectorA, const ScalarType* VectorB)
{
	return VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2];
}


template<typename ScalarType>
inline
ScalarType ComputeAngleBetweenTwoVectorIn3D(const DenseMatrix<ScalarType>& VectorA, const DenseMatrix<ScalarType>& VectorB)
{
    if (VectorA.GetElementNumber() != 3 || VectorB.GetElementNumber() != 3)
    {
        MDK_Error("Input is not 3D vector @ mdkGeometry3D ComputeAngleBetweenTwoVectorIn3D(...)")
        return GetNaNElement<ScalarType>();
    }

    return ComputeAngleBetweenTwoVectorIn3D(VectorA.GetElementPointer(), VectorB.GetElementPointer());
}

template<typename ScalarType>
inline
ScalarType ComputeAngleBetweenTwoVectorIn3D(const DenseVector<ScalarType, 3>& VectorA, const DenseVector<ScalarType, 3>& VectorB)
{
    return ComputeAngleBetweenTwoVectorIn3D(VectorA.GetElementPointer(), VectorB.GetElementPointer());
}

template<typename ScalarType>
inline
ScalarType ComputeAngleBetweenTwoVectorIn3D(const ScalarType* VectorA, const ScalarType* VectorB)
{
    if (VectorA == nullptr || VectorB == nullptr)
    {
        MDK_Error("Input is nullptr @ mdkGeometry3D ComputeAngleBetweenTwoVectorIn3D(...)")
        return GetNaNElement<ScalarType>();
    }

	auto eps_value = std::numeric_limits<ScalarType>::epsilon();

    auto L2Norm_A = std::sqrt(VectorA[0] * VectorA[0] + VectorA[1] * VectorA[1] + VectorA[2] * VectorA[2]);
    auto L2Norm_B = std::sqrt(VectorB[0] * VectorB[0] + VectorB[1] * VectorB[1] + VectorB[2] * VectorB[2]);
    if (L2Norm_A > eps_value && L2Norm_B > eps_value)
    {
        auto CosTheta = (VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2]) / (L2Norm_A*L2Norm_B);
        return std::acos(CosTheta); // [0, pi], acos(-1) = pi
    }
    else
    {
        return 0;
    }
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const DenseMatrix<ScalarType>& PointA, const DenseMatrix<ScalarType>& PointB, const DenseMatrix<ScalarType>& PointC)
{
    DenseVector<ScalarType, 3> Normal;

    if (PointA.GetElementNumber() != 3 || PointB.GetElementNumber() != 3 || PointC.GetElementNumber() != 3)
    {
        MDK_Error("Input is not position vector in 3D @ mdkGeometry3D ComputeTriangleNormalIn3D(...)")
        Normal.Fill(GetNaNElement<ScalarType>());
        return Normal;
    }

    Normal = ComputeTriangleNormalIn3D(PointA.GetElementPointer(), PointB.GetElementPointer(), PointC.GetElementPointer());
    return Normal;
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const DenseVector<ScalarType, 3>& PointA, const DenseVector<ScalarType, 3>& PointB, const DenseVector<ScalarType, 3>& PointC)
{
	return ComputeTriangleNormalIn3D(PointA.GetElementPointer(), PointB.GetElementPointer(), PointC.GetElementPointer());
}

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const ScalarType* PointA, const ScalarType* PointB, const ScalarType* PointC)
{
    DenseVector<ScalarType, 3> Normal;
    if (PointA == nullptr || PointB == nullptr || PointC == nullptr)
    {
        MDK_Error("Input is nullptr @ mdkGeometry3D ComputeTriangleNormalIn3D(...)")
        Normal.Fill(GetNaNElement<ScalarType>());
        return Normal;
    }

    ScalarType VectorAB[3]; // vector U
    VectorAB[0] = PointB[0] - PointA[0];
    VectorAB[1] = PointB[1] - PointA[1];
    VectorAB[2] = PointB[2] - PointA[2];

    ScalarType VectorAC[3]; // vector V
    VectorAC[0] = PointC[0] - PointA[0];
    VectorAC[1] = PointC[1] - PointA[1];
    VectorAC[2] = PointC[2] - PointA[2];

    Normal = ComputeVectorCrossProductIn3D(VectorAB, VectorAC);

    ScalarType L2Norm_ABxAC = std::sqrt(Normal[0] * Normal[0] + Normal[1] * Normal[1] + Normal[2] * Normal[2]);

    Normal[0] = Normal[0] / L2Norm_ABxAC;
    Normal[1] = Normal[1] / L2Norm_ABxAC;
    Normal[2] = Normal[2] / L2Norm_ABxAC;

    return Normal;
}

template<typename ScalarType>
inline
ScalarType ComputeTriangleAreaIn3D(const DenseMatrix<ScalarType>& PointA, const DenseMatrix<ScalarType>& PointB, const DenseMatrix<ScalarType>& PointC)
{
    if (PointA.GetElementNumber() != 3 || PointB.GetElementNumber() != 3 || PointC.GetElementNumber() != 3)
    {
        MDK_Error("Input is not position vector in 3D @ mdkGeometry3D ComputeTriangleAreaIn3D(...)")
        return GetNaNElement<ScalarType>();
    }

    return ComputeTriangleAreaIn3D(PointA.GetElementPointer(), PointB.GetElementPointer(), PointC.GetElementPointer());
}

template<typename ScalarType>
inline
ScalarType ComputeTriangleAreaIn3D(const DenseVector<ScalarType, 3>& PointA, const DenseVector<ScalarType, 3>& PointB, const DenseVector<ScalarType, 3>& PointC)
{
    return ComputeTriangleAreaIn3D(PointA.GetElementPointer(), PointB.GetElementPointer(), PointC.GetElementPointer());
}

template<typename ScalarType>
ScalarType ComputeTriangleAreaIn3D(const ScalarType* PointA, const ScalarType* PointB, const ScalarType* PointC)
{
    ScalarType VectorAB[3]; // vector U
    VectorAB[0] = PointB[0] - PointA[0];
    VectorAB[1] = PointB[1] - PointA[1];
    VectorAB[2] = PointB[2] - PointA[2];

    ScalarType VectorAC[3]; // vector V
    VectorAC[0] = PointC[0] - PointA[0];
    VectorAC[1] = PointC[1] - PointA[1];
    VectorAC[2] = PointC[2] - PointA[2];

    auto Normal = ComputeVectorCrossProductIn3D(VectorAB, VectorAC);

    ScalarType L2Norm_ABxAC = std::sqrt(Normal[0] * Normal[0] + Normal[1] * Normal[1] + Normal[2] * Normal[2]);

    ScalarType Area = L2Norm_ABxAC / ScalarType(2);
    return Area;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> ComputeCenterOfCircumcircleOfTriangleIn3D(const DenseMatrix<ScalarType>& PointA, const DenseMatrix<ScalarType>& PointB, const DenseMatrix<ScalarType>& PointC)
{
	if (PointA.GetElementNumber() != 3 || PointB.GetElementNumber() != 3 || PointC.GetElementNumber() != 3)
	{
		MDK_Error("Input is not position vector in 3D @ mdkGeometry3D ComputeCenterOfCircumcircleOfTriangleIn3D(...)")
		return GetNaNElement<ScalarType>();
	}

	return ComputeCenterOfCircumcircleOfTriangleIn3D(PointA.GetElementPointer(), PointB.GetElementPointer(), PointC.GetElementPointer());
}

template<typename ScalarType>
DenseVector<ScalarType, 3> ComputeCenterOfCircumcircleOfTriangleIn3D(const DenseVector<ScalarType, 3>& PointA, const DenseVector<ScalarType, 3>& PointB, const DenseVector<ScalarType, 3>& PointC)
{
	return ComputeCenterOfCircumcircleOfTriangleIn3D(PointA.GetElementPointer(), PointB.GetElementPointer(), PointC.GetElementPointer());
}

template<typename ScalarType>
DenseVector<ScalarType, 3> ComputeCenterOfCircumcircleOfTriangleIn3D(const ScalarType* PointA, const ScalarType* PointB, const ScalarType* PointC)
{   // ref: http://mathworld.wolfram.com/Circumcircle.html
	//choose PointA as origin
	//
	// Y
	// ^
	// |   C 
	// |  /|   
	// | / | 
	// A---D-----B ------> X
	//
	//      Y
	//      ^
	//  C   |
	//  | \ |   
	//  |  \| 
	//  D---A--------B ------> X


	DenseVector<ScalarType, 3> AB, DirectionX;
	AB[0] = PointB[0] - PointA[0];
	AB[1] = PointB[1] - PointA[1];
	AB[2] = PointB[2] - PointA[2];	
	auto L_AB = std::sqrt(AB[0] * AB[0] + AB[1] * AB[1] + AB[2] * AB[2]);
	DirectionX[0] = AB[0] / L_AB;
	DirectionX[1] = AB[1] / L_AB;
	DirectionX[2] = AB[2] / L_AB;

	DenseVector<ScalarType, 3> AC;
	AC[0] = PointC[0] - PointA[0];
	AC[1] = PointC[1] - PointA[1];
	AC[2] = PointC[2] - PointA[2];

	ScalarType Dot_AC_X = DirectionX[0] * AC[0] + DirectionX[1] * AC[1] + DirectionX[2] * AC[2];
	DenseVector<ScalarType, 3> AD;// PointD is on AB
	AD[0] = Dot_AC_X*DirectionX[0];
	AD[1] = Dot_AC_X*DirectionX[1];
	AD[2] = Dot_AC_X*DirectionX[2];
	auto L_AD = std::sqrt(AD[0] * AD[0] + AD[1] * AD[1] + AD[2] * AD[2]);

	DenseVector<ScalarType, 3> DirectionY, DC;
	DC[0] = AC[0] - AD[0];
	DC[1] = AC[1] - AD[1];
	DC[2] = AC[2] - AD[2];
	auto L_DC = std::sqrt(DC[0] * DC[0] + DC[1] * DC[1] + DC[2] * DC[2]);
	DirectionY[0] = DC[0] / L_DC;
	DirectionY[1] = DC[1] / L_DC;
	DirectionY[2] = DC[2] / L_DC;

	//------------- special case when A, B, C in a line ---------------------------

	if (L_DC <= std::numeric_limits<ScalarType>::epsilon())
	{		
		auto inf = std::numeric_limits<ScalarType>::infinity();
		DenseVector<ScalarType, 3> Center = { inf, inf, inf };
		return Center;
	}

	//-------------- general case when A, B, C not in a line --------------------

	// convter PointA PointB PointC to the plane
	// PointA -> x1, y1; PointB->x2, y2; PointC->x3,y3
	// x0, y0 is the center
	ScalarType x0, y0, x1, y1, x2, y2, x3, y3;
	x1 = 0;
	y1 = 0;
	x2 = L_AB;
	y2 = 0;
	if (Dot_AC_X > 0)
	{
		x3 = L_AD;
	}
	else
	{
		x3 = -L_AD;
	}
	y3 = L_DC;

	DenseMatrix<ScalarType> a, bx, by;
	a = { {x1, y1, 1},
	      {x2, y2, 1},
		  {x3, y3, 1} };
	bx = { {x1*x1+y1*y1, y1, 1},
	       {x2*x2+y2*y2, y2, 1},
		   {x3*x3+y3*y3, y3, 1} };
	by = { {x1*x1+y1*y1, x1, 1},
	       {x2*x2+y2*y2, x2, 1},
		   {x3*x3+y3*y3, x3, 1} };

	auto a_det = a.Det();
	auto bx_det = bx.Det();
	auto by_det = by.Det();
	x0 = -bx_det / (2 * a_det);
	y0 = -by_det / (2 * a_det);

	DenseVector<ScalarType, 3> Center;
	Center[0] = PointA[0] + x0*DirectionX[0] + y0*DirectionY[0];
	Center[1] = PointA[1] + x0*DirectionX[1] + y0*DirectionY[1];
	Center[2] = PointA[2] + x0*DirectionX[2] + y0*DirectionY[2];

	return Center;
}

}// namespace mdk
