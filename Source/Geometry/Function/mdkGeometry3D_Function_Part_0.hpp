#ifndef __mdkGeometry3D_Function_Part_0_hpp
#define __mdkGeometry3D_Function_Part_0_hpp

namespace mdk
{

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeVectorCrossProductIn3D(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    DenseVector<ElementType, 3> Vector_AxB;

    if (VectorA.GetElementNumber() != 3 || VectorB.GetElementNumber() != 3)
    {
        MDK_Error("Input is not 3D vector @ mdkGeometry3D ComputeVectorCrossProductIn3D(...)")
        Vector_AxB.Fill(0);
        return Vector_AxB();
    }

    Vector_AxB = ComputeVectorCrossProductIn3D(VectorA.GetElementPointer(), VectorB.GetElementPointer());
    return Vector_AxB;
}

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeVectorCrossProductIn3D(const DenseVector<ElementType, 3>& VectorA, const DenseVector<ElementType, 3>& VectorB)
{
    return ComputeVectorCrossProductIn3D(VectorA.GetElementPointer(), VectorB.GetElementPointer());
}

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeVectorCrossProductIn3D(const ElementType* VectorA, const ElementType* VectorB)
{
    DenseVector<ElementType, 3> Vector_AxB;
    if (VectorA == nullptr || VectorB == nullptr)
    {
        MDK_Error("Input is nullptr @ mdkGeometry3D ComputeVectorCrossProductIn3D(...)")
        Vector_AxB.Fill(0);
        return Vector_AxB;
    }

    Vector_AxB[0] = VectorA[1] * VectorB[2] - VectorA[2] * VectorB[1];
    Vector_AxB[1] = VectorA[2] * VectorB[0] - VectorA[0] * VectorB[2];
    Vector_AxB[2] = VectorA[0] * VectorB[1] - VectorA[1] * VectorB[0];
    return Vector_AxB;
}


template<typename ElementType>
inline
ElementType ComputeAngleBetweenTwoVectorIn3D(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    if (VectorA.GetElementNumber() != 3 || VectorB.GetElementNumber() != 3)
    {
        MDK_Error("Input is not 3D vector @ mdkGeometry3D ComputeAngleBetweenTwoVectorIn3D(...)")
        return 0;
    }

    return ComputeAngleBetweenTwoVectorIn3D(VectorA.GetElementPointer(), VectorB.GetElementPointer());
}

template<typename ElementType>
inline
ElementType ComputeAngleBetweenTwoVectorIn3D(const DenseVector<ElementType, 3>& VectorA, const DenseVector<ElementType, 3>& VectorB)
{
    return ComputeAngleBetweenTwoVectorIn3D(VectorA.GetElementPointer(), VectorB.GetElementPointer());
}

template<typename ElementType>
inline
ElementType ComputeAngleBetweenTwoVectorIn3D(const ElementType* VectorA, const ElementType* VectorB)
{
    if (VectorA == nullptr || VectorB == nullptr)
    {
        MDK_Error("Input is nullptr @ mdkGeometry3D ComputeAngleBetweenTwoVectorIn3D(...)")
        return 0;
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

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeTriangleNormalIn3D(const DenseMatrix<ElementType>& PointPositionA, 
                                                      const DenseMatrix<ElementType>& PointPositionB, 
                                                      const DenseMatrix<ElementType>& PointPositionC)
{
    DenseVector<ElementType, 3> Normal;

    if (PointPositionA.GetElementNumber() != 3 || PointPositionB.GetElementNumber() != 3 || PointPositionC.GetElementNumber() != 3)
    {
        MDK_Error("Input is not position vector in 3D @ mdkGeometry3D ComputeTriangleNormalIn3D(...)")
        Normal.Fill(0);
        return Normal;
    }

    Normal = ComputeTriangleNormalIn3D(PointPositionA.GetElementPointer(), PointPositionB.GetElementPointer(), PointPositionC.GetElementPointer());
    return Normal;
}

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeTriangleNormalIn3D(const DenseVector<ElementType, 3>& PointPositionA, 
                                                      const DenseVector<ElementType, 3>& PointPositionB, 
                                                      const DenseVector<ElementType, 3>& PointPositionC)
{
    return ComputeTriangleNormalIn3D(PointPositionA.GetElementPointer(), PointPositionB.GetElementPointer(), PointPositionC.GetElementPointer());
}

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeTriangleNormalIn3D(const ElementType* PointPositionA, const ElementType* PointPositionB, const ElementType* PointPositionC)
{
    DenseVector<ElementType, 3> Normal;
    if (PointPositionA == nullptr || PointPositionB == nullptr || PointPositionC == nullptr)
    {
        MDK_Error("Input is nullptr @ mdkGeometry3D ComputeTriangleNormalIn3D(...)")
        Normal.Fill(0);
        return Normal;
    }

    ElementType VectorAB[3]; // vector U
    VectorAB[0] = PointPositionB[0] - PointPositionA[0];
    VectorAB[1] = PointPositionB[1] - PointPositionA[1];
    VectorAB[2] = PointPositionB[2] - PointPositionA[2];

    ElementType VectorAC[3]; // vector V
    VectorAC[0] = PointPositionC[0] - PointPositionA[0];
    VectorAC[1] = PointPositionC[1] - PointPositionA[1];
    VectorAC[2] = PointPositionC[2] - PointPositionA[2];

    Normal = ComputeVectorCrossProductIn3D(VectorAB, VectorAC);

    ElementType L2Norm_ABxAC = std::sqrt(Normal[0] * Normal[0] + Normal[1] * Normal[1] + Normal[2] * Normal[2]);

    Normal[0] = Normal[0] / L2Norm_ABxAC;
    Normal[1] = Normal[1] / L2Norm_ABxAC;
    Normal[2] = Normal[2] / L2Norm_ABxAC;

    return Normal;
}

template<typename ElementType>
inline
ElementType ComputeTriangleAreaIn3D(const DenseMatrix<ElementType>& PointPositionA,
                                    const DenseMatrix<ElementType>& PointPositionB,
                                    const DenseMatrix<ElementType>& PointPositionC)
{
    if (PointPositionA.GetElementNumber() != 3 || PointPositionB.GetElementNumber() != 3 || PointPositionC.GetElementNumber() != 3)
    {
        MDK_Error("Input is not position vector in 3D @ mdkGeometry3D ComputeTriangleAreaIn3D(...)")
        return 0;
    }

    return ComputeTriangleAreaIn3D(PointPositionA.GetElementPointer(), PointPositionB.GetElementPointer(), PointPositionC.GetElementPointer());
}

template<typename ElementType>
inline
ElementType ComputeTriangleAreaIn3D(const DenseVector<ElementType, 3>& PointPositionA,
                                    const DenseVector<ElementType, 3>& PointPositionB,
                                    const DenseVector<ElementType, 3>& PointPositionC)
{
    return ComputeTriangleAreaIn3D(PointPositionA.GetElementPointer(), PointPositionB.GetElementPointer(), PointPositionC.GetElementPointer());
}

template<typename ElementType>
ElementType ComputeTriangleAreaIn3D(const ElementType* PointPositionA, const ElementType* PointPositionB, const ElementType* PointPositionC)
{
    ElementType VectorAB[3]; // vector U
    VectorAB[0] = PointPositionB[0] - PointPositionA[0];
    VectorAB[1] = PointPositionB[1] - PointPositionA[1];
    VectorAB[2] = PointPositionB[2] - PointPositionA[2];

    ElementType VectorAC[3]; // vector V
    VectorAC[0] = PointPositionC[0] - PointPositionA[0];
    VectorAC[1] = PointPositionC[1] - PointPositionA[1];
    VectorAC[2] = PointPositionC[2] - PointPositionA[2];

    auto Normal = ComputeVectorCrossProductIn3D(VectorAB, VectorAC);

    ElementType L2Norm_ABxAC = std::sqrt(Normal[0] * Normal[0] + Normal[1] * Normal[1] + Normal[2] * Normal[2]);

    ElementType Area = L2Norm_ABxAC / ElementType(2);
    return Area;
}



}// namespace mdk

#endif