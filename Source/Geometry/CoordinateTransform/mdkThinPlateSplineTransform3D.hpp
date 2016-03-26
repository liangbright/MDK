#pragma once

namespace mdk
{
template<typename ScalarType>
ThinPlateSplineTransform3D<ScalarType>::ThinPlateSplineTransform3D()
{
	this->Clear();
}

template<typename ScalarType>
ThinPlateSplineTransform3D<ScalarType>::~ThinPlateSplineTransform3D()
{
}

template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Parameter.Clear();
}

template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet)
{
	m_SourceLandmarkPointSet = SourceLandmarkPointSet;
}

template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet)
{
	m_TargetLandmarkPointSet = TargetLandmarkPointSet;
}

template<typename ScalarType>
bool ThinPlateSplineTransform3D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ ThinPlateSplineTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size  @ ThinPlateSplineTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ ThinPlateSplineTransform3D::CheckLandmarkPointSet()")
		return false;

	}

	if (m_TargetLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size  @ ThinPlateSplineTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ ThinPlateSplineTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	return true;
}

template<typename ScalarType>
void ThinPlateSplineTransform3D<ScalarType>::EstimateParameter()
{
	// check input 
	if (this->CheckLandmarkPointSet() == false)
	{
		return;
	}
    //---------------------- input check done -------------------------------------------------------------------//
	
	//------------------- Construct L matrix as in the literature --------------------------------------------------//
	/*
	auto PointCount = m_SourceLandmarkPointSet->GetColCount();

    // compute K matrix K(i,j)= distance between point_i and point_j in SourceLandmarkPointSet
	DenseMatrix<ScalarType> K;
	K.Resize(PointCount, PointCount);
	for (int_max i = 0; i < PointCount - 1; ++i)
	{
		auto Point_i = m_SourceLandmarkPointSet->GetPointerOfCol(i);

		for (int_max j = i + 1; j < PointCount; ++j)
		{
			auto Point_j = m_SourceLandmarkPointSet->GetPointerOfCol(j);

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
	P.Resize(PointCount, 4);
	for (int_max i = 0; i < PointCount; ++i)
	{
		P(i, 0) = 1;
		P(i, 1) = (*m_SourceLandmarkPointSet)(0, i); // x
		P(i, 2) = (*m_SourceLandmarkPointSet)(1, i); // y
		P(i, 3) = (*m_SourceLandmarkPointSet)(2, i); // z
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

	auto PointCount = m_SourceLandmarkPointSet->GetColCount();

	DenseMatrix<ScalarType> L;
	L.Resize(PointCount + 4, PointCount + 4);

	// add K in L
	for (int_max i = 0; i < PointCount - 1; ++i)
	{
		auto Point_i = m_SourceLandmarkPointSet->GetPointerOfCol(i);

		for (int_max j = i + 1; j < PointCount; ++j)
		{
			auto Point_j = m_SourceLandmarkPointSet->GetPointerOfCol(j);

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
	for (int_max i = 0; i < PointCount; ++i)
	{
		L(i, PointCount) = 1;
		L(i, PointCount + 1) = (*m_SourceLandmarkPointSet)(0, i); // x
		L(i, PointCount + 2) = (*m_SourceLandmarkPointSet)(1, i); // y
		L(i, PointCount + 3) = (*m_SourceLandmarkPointSet)(2, i); // z
	}

	// add P' in L
	for (int_max j = 0; j < PointCount; ++j)
	{
		L(PointCount, j) = 1;
		L(PointCount + 1, j) = (*m_SourceLandmarkPointSet)(0, j); // x
		L(PointCount + 2, j) = (*m_SourceLandmarkPointSet)(1, j); // y
		L(PointCount + 3, j) = (*m_SourceLandmarkPointSet)(2, j); // z
	}

	// add ZeroMatrix in L
	for (int_max j = PointCount; j < PointCount + 4; ++j)
	{
		for (int_max i = PointCount; i < PointCount + 4; ++i)
		{		
			L(i, j) = 0;
		}
	}

	// assemble B matrix
	DenseMatrix<ScalarType> B;
	B.Resize(PointCount + 4, 3);
	for (int_max i = 0; i < PointCount; ++i)
	{
		B(i, 0) = (*m_TargetLandmarkPointSet)(0, i); // x
		B(i, 1) = (*m_TargetLandmarkPointSet)(1, i); // y
		B(i, 2) = (*m_TargetLandmarkPointSet)(2, i); // z
	}
	for (int_max i = PointCount; i < PointCount + 4; ++i)
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
void ThinPlateSplineTransform3D<ScalarType>::SetParameter(const DenseMatrix<ScalarType>& Parameter)
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ ThinPlateSplineTransform3D::SetParameter()")
		return;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size @ ThinPlateSplineTransform3D::SetParameter()")
		return;
	}

	if (Parameter.GetColCount() != 3)
	{
		MDK_Error("Parameter wrong size @ ThinPlateSplineTransform3D::SetParameter(...)")
		return;
	}

	if (Parameter.GetRowCount() != m_SourceLandmarkPointSet->GetColCount() + 4)
	{
		MDK_Error("Parameter wrong size @ ThinPlateSplineTransform3D::SetParameter(...)")
		return;
	}

	m_Parameter = Parameter;
}

template<typename ScalarType>
DenseMatrix<ScalarType> ThinPlateSplineTransform3D<ScalarType>::GetParameter() const
{
	return m_Parameter;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> ThinPlateSplineTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> OutputPosition;
	OutputPosition[0] = 0;
	OutputPosition[1] = 0;
	OutputPosition[2] = 0;

	auto PointCount = m_SourceLandmarkPointSet->GetColCount();
	
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto Point_k = m_SourceLandmarkPointSet->GetPointerOfCol(k);
		auto Distance = (x - Point_k[0])*(x - Point_k[0]) + (y - Point_k[1])*(y - Point_k[1]) + (z - Point_k[2])*(z - Point_k[2]);	
		Distance = std::sqrt(Distance);
		OutputPosition[0] += m_Parameter(k, 0)*Distance;
		OutputPosition[1] += m_Parameter(k, 1)*Distance;
		OutputPosition[2] += m_Parameter(k, 2)*Distance;
	}

	for (int_max k = 0; k < 3; ++k)
	{
		OutputPosition[k] +=  m_Parameter(PointCount, k) 
			                + m_Parameter(PointCount + 1, k)*x
		                    + m_Parameter(PointCount + 2, k)*y
		                    + m_Parameter(PointCount + 3, k)*z;
	}

	return OutputPosition;
}

}//namespace mdk
