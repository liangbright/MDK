#pragma once

namespace mdk
{

template<typename ScalarType>
RigidTransform2D<ScalarType>::RigidTransform2D()
{
	this->Clear();
}

template<typename ScalarType>
RigidTransform2D<ScalarType>::~RigidTransform2D()
{
}

template<typename ScalarType>
void RigidTransform2D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Rotation.Clear();
	m_Rotation.Resize(2, 2);
	m_Rotation.Fill(0);
	m_Rotation.FillDiagonal(1);
	m_Rotation.FixSize();
	m_Translation_AfterRotation.Resize(2, 1);
	m_Translation_AfterRotation.Fill(0);
	m_Translation_AfterRotation.FixSize();
}


template<typename ScalarType>
bool RigidTransform2D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ RigidTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size  @ RigidTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ RigidTransform2D::CheckLandmarkPointSet()")
		return false;

	}
		
	if (m_TargetLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size  @ RigidTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ RigidTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void RigidTransform2D<ScalarType>::EstimateParameter()
{
	if (this->CheckLandmarkPointSet() == false)
	{
		return;
	}

	const auto& X = *m_SourceLandmarkPointSet;
	const auto& Y = *m_TargetLandmarkPointSet;

	auto Xc = X.MeanOfEachRow();
	auto Yc = Y.MeanOfEachRow();
	Yc.Reshape(1, 2);

	DenseMatrix<ScalarType> H(2, 2);
	H.Fill(0);
	auto PointCount = X.GetColCount();
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto Xk = X.GetCol(k);
		Xk.Reshape(2, 1);
		Xk -= Xc;

		auto Yk = Y.GetCol(k);
		Yk.Reshape(1, 2);
		Yk -= Yc;

		H += Xk*Yk;
	}

	auto SVDResult = H.SVD();
	auto& U = SVDResult.U;
	auto& V = SVDResult.V;	
	auto Ut = SVDResult.U.Transpose();
	auto U_det = U.Det();
	auto V_det = V.Det();

	DenseMatrix<ScalarType> D(2, 2);
	D.Fill(0);
	D(0, 0) = 1;
	if (U_det*V_det > 0)
	{
		D(1, 1) = 1;
	}
	else
	{
		D(1, 1) = -1;
	}

	m_Rotation = V * D * Ut;

	Yc.Reshape(2, 1);
	m_Translation_AfterRotation = Yc - m_Rotation*Xc;	
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> RigidTransform2D<ScalarType>::TransformPoint(ScalarType x, ScalarType y) const
{
	const auto R = m_Rotation.GetElementPointer();

	DenseVector<ScalarType, 2> NewPosition;
	NewPosition[0] = m_Translation_AfterRotation[0] + R[0] * x + R[2] * y;
	NewPosition[1] = m_Translation_AfterRotation[1] + R[1] * x + R[3] * y;
	return NewPosition;
}

}//namespace mdk
