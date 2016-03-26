#pragma once

namespace mdk
{
template<typename ScalarType>
ThinPlateSplineTransform2D<ScalarType>::ThinPlateSplineTransform2D()
{
	this->Clear();
}

template<typename ScalarType>
ThinPlateSplineTransform2D<ScalarType>::~ThinPlateSplineTransform2D()
{
}

template<typename ScalarType>
void ThinPlateSplineTransform2D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Parameter.Clear();
	m_Flag_Use_TPS3D = false;
}

template<typename ScalarType>
void ThinPlateSplineTransform2D<ScalarType>::SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet)
{
	m_SourceLandmarkPointSet = SourceLandmarkPointSet;
}

template<typename ScalarType>
void ThinPlateSplineTransform2D<ScalarType>::SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet)
{
	m_TargetLandmarkPointSet = TargetLandmarkPointSet;
}

template<typename ScalarType>
void ThinPlateSplineTransform2D<ScalarType>::UseTPS3D()
{
	m_Flag_Use_TPS3D = true;
}

template<typename ScalarType>
void ThinPlateSplineTransform2D<ScalarType>::UseStandardTPS2D()
{
	m_Flag_Use_TPS3D = false;
}


template<typename ScalarType>
bool ThinPlateSplineTransform2D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ ThinPlateSplineTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size @ ThinPlateSplineTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ ThinPlateSplineTransform2D::CheckLandmarkPointSet()")
		return false;

	}

	if (m_TargetLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size @ ThinPlateSplineTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ ThinPlateSplineTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	return true;
}

template<typename ScalarType>
void ThinPlateSplineTransform2D<ScalarType>::EstimateParameter()
{
	// check input 
	if (this->CheckLandmarkPointSet() == false)
	{
		return;
	}

	if (m_Flag_Use_TPS3D == true)
	{
		auto PointCount = m_SourceLandmarkPointSet->GetColCount();

		DenseMatrix<ScalarType> L;
		L.Resize(PointCount + 3, PointCount + 3);

		// add K in L
		for (int_max i = 0; i < PointCount - 1; ++i)
		{
			auto Point_i = m_SourceLandmarkPointSet->GetPointerOfCol(i);

			for (int_max j = i + 1; j < PointCount; ++j)
			{
				auto Point_j = m_SourceLandmarkPointSet->GetPointerOfCol(j);

				auto Distance = (Point_i[0] - Point_j[0])*(Point_i[0] - Point_j[0]) + (Point_i[1] - Point_j[1])*(Point_i[1] - Point_j[1]);
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
		}

		// add P' in L
		for (int_max j = 0; j < PointCount; ++j)
		{
			L(PointCount, j) = 1;
			L(PointCount + 1, j) = (*m_SourceLandmarkPointSet)(0, j); // x
			L(PointCount + 2, j) = (*m_SourceLandmarkPointSet)(1, j); // y
		}

		// add ZeroMatrix in L
		for (int_max j = PointCount; j < PointCount + 3; ++j)
		{
			for (int_max i = PointCount; i < PointCount + 3; ++i)
			{
				L(i, j) = 0;
			}
		}

		// assemble V matrix
		DenseMatrix<ScalarType> V;
		V.Resize(PointCount + 3, 2);
		for (int_max i = 0; i < PointCount; ++i)
		{
			V(i, 0) = (*m_TargetLandmarkPointSet)(0, i); // x
			V(i, 1) = (*m_TargetLandmarkPointSet)(1, i); // y
		}
		for (int_max i = PointCount; i < PointCount + 3; ++i)
		{
			V(i, 0) = 0;
			V(i, 1) = 0;
		}

		m_Parameter = MatrixMultiply(L.PInv(), V);
	}
	else// standard TPS2D
	{
		auto PointCount = m_SourceLandmarkPointSet->GetColCount();

		DenseMatrix<ScalarType> L;
		L.Resize(PointCount + 3, PointCount + 3);

		// add K in L
		ScalarType EPS = std::numeric_limits<ScalarType>::epsilon();
		for (int_max i = 0; i < PointCount - 1; ++i)
		{
			auto Point_i = m_SourceLandmarkPointSet->GetPointerOfCol(i);

			for (int_max j = i + 1; j < PointCount; ++j)
			{
				auto Point_j = m_SourceLandmarkPointSet->GetPointerOfCol(j);

				auto Distance_sq = (Point_i[0] - Point_j[0])*(Point_i[0] - Point_j[0]) + (Point_i[1] - Point_j[1])*(Point_i[1] - Point_j[1]);
				ScalarType U = 0;
				if (Distance_sq > EPS)
				{
					U = Distance_sq*std::log(Distance_sq);
				}
				L(i, j) = U;
				L(j, i) = U;
			}
			L(i, i) = 0;
		}

		// add P in L
		for (int_max i = 0; i < PointCount; ++i)
		{
			L(i, PointCount) = 1;
			L(i, PointCount + 1) = (*m_SourceLandmarkPointSet)(0, i); // x
			L(i, PointCount + 2) = (*m_SourceLandmarkPointSet)(1, i); // y
		}

		// add P' in L
		for (int_max j = 0; j < PointCount; ++j)
		{
			L(PointCount, j) = 1;
			L(PointCount + 1, j) = (*m_SourceLandmarkPointSet)(0, j); // x
			L(PointCount + 2, j) = (*m_SourceLandmarkPointSet)(1, j); // y
		}

		// add ZeroMatrix in L
		for (int_max j = PointCount; j < PointCount + 3; ++j)
		{
			for (int_max i = PointCount; i < PointCount + 3; ++i)
			{
				L(i, j) = 0;
			}
		}

		// assemble V matrix
		DenseMatrix<ScalarType> V;
		V.Resize(PointCount + 3, 2);
		for (int_max i = 0; i < PointCount; ++i)
		{
			V(i, 0) = (*m_TargetLandmarkPointSet)(0, i); // x
			V(i, 1) = (*m_TargetLandmarkPointSet)(1, i); // y
		}
		for (int_max i = PointCount; i < PointCount + 3; ++i)
		{
			V(i, 0) = 0;
			V(i, 1) = 0;
		}

		m_Parameter = MatrixMultiply(L.PInv(), V);
	}
}


template<typename ScalarType>
bool ThinPlateSplineTransform2D<ScalarType>::Is_TPS3D_used() const
{
	return m_Flag_Use_TPS3D;
}

template<typename ScalarType>
bool ThinPlateSplineTransform2D<ScalarType>::Is_StandardTPS2D_used() const
{
	return (m_Flag_Use_TPS3D==false);
}

template<typename ScalarType>
void ThinPlateSplineTransform2D<ScalarType>::SetParameter(const DenseMatrix<ScalarType>& Parameter)
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ ThinPlateSplineTransform2D::SetParameter()")
		return;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size @ ThinPlateSplineTransform2D::SetParameter()")
		return;
	}

	if (Parameter.GetColCount() != 2)
	{
		MDK_Error("Parameter wrong size @ ThinPlateSplineTransform2D::SetParameter(...)")
		return;
	}

	if (Parameter.GetRowCount() != m_SourceLandmarkPointSet->GetColCount() + 4)
	{
		MDK_Error("Parameter wrong size @ ThinPlateSplineTransform2D::SetParameter(...)")
		return;
	}

	m_Parameter = Parameter;
}

template<typename ScalarType>
DenseMatrix<ScalarType> ThinPlateSplineTransform2D<ScalarType>::GetParameter() const
{
	return m_Parameter;
}

template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> ThinPlateSplineTransform2D<ScalarType>::TransformPoint(ScalarType x, ScalarType y) const
{
	DenseVector<ScalarType, 2> OutputPosition;
	OutputPosition[0] = 0;
	OutputPosition[1] = 0;

	auto PointCount = m_SourceLandmarkPointSet->GetColCount();
	
	if (m_Flag_Use_TPS3D == true)
	{
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto Point_k = m_SourceLandmarkPointSet->GetPointerOfCol(k);
			auto Distance = std::sqrt((x - Point_k[0])*(x - Point_k[0]) + (y - Point_k[1])*(y - Point_k[1]));
			OutputPosition[0] += m_Parameter(k, 0)*Distance;
			OutputPosition[1] += m_Parameter(k, 1)*Distance;
		}

		for (int_max k = 0; k < 2; ++k)
		{
			OutputPosition[k] += m_Parameter(PointCount, k)	+ m_Parameter(PointCount + 1, k)*x + m_Parameter(PointCount + 2, k)*y;
		}
	}
	else
	{
		ScalarType EPS = std::numeric_limits<ScalarType>::epsilon();
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto Point_k = m_SourceLandmarkPointSet->GetPointerOfCol(k);
			auto Distance_sq = (x - Point_k[0])*(x - Point_k[0]) + (y - Point_k[1])*(y - Point_k[1]);
			ScalarType U = 0;
			if (Distance_sq > EPS)
			{
				U = Distance_sq*std::log(Distance_sq);
			}
			OutputPosition[0] += m_Parameter(k, 0)*U;
			OutputPosition[1] += m_Parameter(k, 1)*U;
		}

		for (int_max k = 0; k < 2; ++k)
		{
			OutputPosition[k] += m_Parameter(PointCount, k) + m_Parameter(PointCount + 1, k)*x + m_Parameter(PointCount + 2, k)*y;
		}
	}

	return OutputPosition;
}

}//namespace mdk
