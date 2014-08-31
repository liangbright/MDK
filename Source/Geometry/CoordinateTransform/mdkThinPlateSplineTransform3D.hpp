#ifndef __mdkThinPlateSplineTransform3D_hpp
#define __mdkThinPlateSplineTransform3D_hpp

namespace mdk
{
template<typename ScalarType>
ThinPlateSplineTransform3D<ScalarType>::ThinPlateSplineTransform3D()
{
}

template<typename ScalarType>
ThinPlateSplineTransform3D<ScalarType>::~ThinPlateSplineTransform3D()
{
}

template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::Clear()
{
	m_SourceControlPointSet = nullptr;
	m_TargetControlPointSet = nullptr;
	m_Parameter.Clear();
}

template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::SetSourceControlPointSet(const DenseMatrix<ScalarType>* SourcePointSet)
{
	m_SourceControlPointSet = SourcePointSet;
}

template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::SetTargetControlPointSet(const DenseMatrix<ScalarType>* TargetPointSet)
{
	m_TargetControlPointSet = TargetPointSet;
}

template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::SetParameter(const DenseMatrix<ScalarType>& Parameter)
{
	m_Parameter = Parameter;
}

template<typename ScalarType>
const DenseMatrix<ScalarType>& ThinPlateSplineTransform3D<ScalarType>::GetParameter() const
{
	return m_Parameter;
}


template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::UpdateParameter()
{
	// check input 
	if (m_SourceControlPointSet == nullptr || m_TargetControlPointSet == nullptr)
	{
		MDK_Error("SourcePointSet or TargetPointSet is empty (nullptr) @ ThinPlateSplineTransform3D::UpdateParameter()")
	    return;
	}

	if (m_SourceControlPointSet->GetRowNumber() != 3 || m_TargetControlPointSet->GetRowNumber() != 3)
	{
		MDK_Error("RowNumber of SourcePointSet or TargetPointSet is not 3 @ ThinPlateSplineTransform3D::UpdateParameter()")
	    return;
	}

	if (m_SourceControlPointSet->GetColNumber() != m_TargetControlPointSet->GetColNumber())
	{
		MDK_Error("ControlPointNumber is not the same @ ThinPlateSplineTransform3D::UpdateParameter()")
	    return;
	}
    //---------------------- input check done -------------------------------------------------------------------//
	
	//------------------- Construct L matrix as in the literature --------------------------------------------------//
	/*
	auto ControlPointNumber = m_SourceControlPointSet->GetColNumber();

    // compute K matrix K(i,j)= distance between point_i and point_j in SourcePointSet
	DenseMatrix<ScalarType> K;
	K.Resize(ControlPointNumber, ControlPointNumber);
	for (int_max i = 0; i < ControlPointNumber - 1; ++i)
	{
		auto Point_i = m_SourceControlPointSet->GetPointerOfCol(i);

		for (int_max j = i + 1; j < ControlPointNumber; ++j)
		{
			auto Point_j = m_SourceControlPointSet->GetPointerOfCol(j);

			auto Distance =  (Point_i[0] - Point_j[0])*(Point_i[0] - Point_j[0]) 
				            +(Point_i[1] - Point_j[1])*(Point_i[1] - Point_j[1])
 			                +(Point_i[2] - Point_j[2])*(Point_i[2] - Point_j[2]);
			Distance = std::sqrt(Distance);
			K(i, j) = Distance;
			K(j, i) = Distance;
		}
		K(i, i) = 0;
	}

	// assemble P matrix
	DenseMatrix<ScalarType> P;
	P.Resize(ControlPointNumber, 4);
	for (int_max i = 0; i < ControlPointNumber; ++i)
	{
		P(i, 0) = 1;
		P(i, 1) = (*m_SourceControlPointSet)(0, i); // x
		P(i, 2) = (*m_SourceControlPointSet)(1, i); // y
		P(i, 3) = (*m_SourceControlPointSet)(2, i); // z
	}

	// get transpose of P
	DenseMatrix<ScalarType> Pt=P.Transpose();

	// assemble zero matrix 4x4
	DenseMatrix<ScalarType> ZeroMatrix(4,4);
	ZeroMatrix.Fill(0);

	// assemble L matrix
	DenseMatrix<ScalarType> L;
    L={{&K, &P}, {&Pt, &ZeroMatrix}}
	*/
	//----------------------------------------------------------------------------------------------------------//

	// construct L matrix directly

	auto ControlPointNumber = m_SourceControlPointSet->GetColNumber();

	DenseMatrix<ScalarType> L;
	L.Resize(ControlPointNumber + 4, ControlPointNumber + 4);

	// add K in L
	for (int_max i = 0; i < ControlPointNumber - 1; ++i)
	{
		auto Point_i = m_SourceControlPointSet->GetPointerOfCol(i);

		for (int_max j = i + 1; j < ControlPointNumber; ++j)
		{
			auto Point_j = m_SourceControlPointSet->GetPointerOfCol(j);

			auto Distance =  (Point_i[0] - Point_j[0])*(Point_i[0] - Point_j[0]) 
				            +(Point_i[1] - Point_j[1])*(Point_i[1] - Point_j[1])
 			                +(Point_i[2] - Point_j[2])*(Point_i[2] - Point_j[2]);
			Distance = std::sqrt(Distance);
			L(i, j) = Distance;
			L(j, i) = Distance;
		}
		L(i, i) = 0;
	}

	// add P in L
	for (int_max i = 0; i < ControlPointNumber; ++i)
	{
		L(i, ControlPointNumber) = 1;
		L(i, ControlPointNumber + 1) = (*m_SourceControlPointSet)(0, i); // x
		L(i, ControlPointNumber + 2) = (*m_SourceControlPointSet)(1, i); // y
		L(i, ControlPointNumber + 3) = (*m_SourceControlPointSet)(2, i); // z
	}

	// add P' in L
	for (int_max j = 0; j < ControlPointNumber; ++j)
	{
		L(ControlPointNumber, j) = 1;
		L(ControlPointNumber + 1, j) = (*m_SourceControlPointSet)(0, j); // x
		L(ControlPointNumber + 2, j) = (*m_SourceControlPointSet)(1, j); // y
		L(ControlPointNumber + 3, j) = (*m_SourceControlPointSet)(2, j); // z
	}

	// add ZeroMatrix in L
	for (int_max j = ControlPointNumber; j < ControlPointNumber + 4; ++j)
	{
		for (int_max i = ControlPointNumber; i < ControlPointNumber + 4; ++i)
		{		
			L(i, j) = 0;
		}
	}

	// assemble B matrix
	DenseMatrix<ScalarType> B;
	B.Resize(ControlPointNumber + 4, 3);
	for (int_max i = 0; i < ControlPointNumber; ++i)
	{
		B(i, 0) = (*m_TargetControlPointSet)(0, i); // x
		B(i, 1) = (*m_TargetControlPointSet)(1, i); // y
		B(i, 2) = (*m_TargetControlPointSet)(2, i); // z
	}
	for (int_max i = ControlPointNumber; i < ControlPointNumber + 4; ++i)
	{
		B(i, 0) = 0;
		B(i, 1) = 0;
		B(i, 2) = 0;
	}

	// computer parameter
	//m_Parameter = SolveMatrixLinearEquation(L, B); // this is not good
	m_Parameter = MatrixMultiply(L.PInv(), B); // use Pseudo Inverse, this is good

	//DisplayMatrix("L", L, 4);
	//DisplayMatrix("B", B, 4);
	//DisplayMatrix("m_Parameter", m_Parameter, 4);
}

template<typename ScalarType>
DenseVector<ScalarType, 3> ThinPlateSplineTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> OutputPosition;
	OutputPosition[0] = 0;
	OutputPosition[1] = 0;
	OutputPosition[2] = 0;

	auto ControlPointNumber = m_SourceControlPointSet->GetColNumber();
	
	for (int_max k = 0; k < ControlPointNumber; ++k)
	{
		auto Point_k = m_SourceControlPointSet->GetPointerOfCol(k);
		auto Distance = (x - Point_k[0])*(x - Point_k[0]) + (y - Point_k[1])*(y - Point_k[1]) + (z - Point_k[2])*(z - Point_k[2]);	
		Distance = std::sqrt(Distance);
		OutputPosition[0] += m_Parameter(k, 0)*Distance;
		OutputPosition[1] += m_Parameter(k, 1)*Distance;
		OutputPosition[2] += m_Parameter(k, 2)*Distance;
	}

	for (int_max k = 0; k < 3; ++k)
	{
		OutputPosition[k] +=  m_Parameter(ControlPointNumber, k) 
			                + m_Parameter(ControlPointNumber + 1, k)*x
		                    + m_Parameter(ControlPointNumber + 2, k)*y
		                    + m_Parameter(ControlPointNumber + 3, k)*z;
	}

	return OutputPosition;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> ThinPlateSplineTransform3D<ScalarType>::TransformPoint(const DenseVector<ScalarType, 3>& SourcePosition) const
{
	return this->TransformPoint(SourcePosition[0], SourcePosition[1], SourcePosition[2]);
}

}//namespace mdk


#endif