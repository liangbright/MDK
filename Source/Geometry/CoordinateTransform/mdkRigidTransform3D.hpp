#pragma once

namespace mdk
{

template<typename ScalarType>
RigidTransform3D<ScalarType>::RigidTransform3D()
{
	this->Clear();
}

template<typename ScalarType>
RigidTransform3D<ScalarType>::~RigidTransform3D()
{
}

template<typename ScalarType>
void RigidTransform3D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Rotation.Clear();
	m_Rotation.Resize(3, 3);
	m_Rotation.Fill(0);
	m_Rotation.FillDiagonal(1);
	m_Rotation.FixSize();
	m_Translation_AfterRotation.Resize(3, 1);
	m_Translation_AfterRotation.Fill(0);
	m_Translation_AfterRotation.FixSize();
}

template<typename ScalarType>
bool RigidTransform3D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ RigidTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size  @ RigidTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ RigidTransform3D::CheckLandmarkPointSet()")
		return false;

	}
		
	if (m_TargetLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size  @ RigidTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ RigidTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void RigidTransform3D<ScalarType>::EstimateParameter()
{
	if (this->CheckLandmarkPointSet() == false)
	{
		return;
	}

	const auto& X = *m_SourceLandmarkPointSet;
	const auto& Y = *m_TargetLandmarkPointSet;

	auto Xc = X.MeanOfEachRow();
	auto Yc = Y.MeanOfEachRow();
	Yc.Reshape(1, 3);

	DenseMatrix<ScalarType> H(3,3);
	H.Fill(0);
	auto PointCount = X.GetColCount();
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto Xk = X.GetCol(k);
		Xk.Reshape(3, 1);
		Xk -= Xc;

		auto Yk = Y.GetCol(k);
		Yk.Reshape(1, 3);
		Yk -= Yc;

		H += Xk*Yk;
	}

	auto SVDResult = H.SVD();
	auto& U = SVDResult.U;
	auto& V = SVDResult.V;	
	auto Ut = SVDResult.U.Transpose();
	auto U_det = U.Det();
	auto V_det = V.Det();

	DenseMatrix<ScalarType> D(3, 3);
	D.Fill(0);
	D(0, 0) = 1;
	D(1, 1) = 1;
	if (U_det*V_det > 0)
	{
		D(2, 2) = 1;
	}
	else
	{
		D(2, 2) = -1;
	}

	m_Rotation = V * D * Ut;

	Yc.Reshape(3, 1);
	m_Translation_AfterRotation = Yc - m_Rotation*Xc;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> RigidTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{	
	const auto R = m_Rotation.GetElementPointer();

	DenseVector<ScalarType, 3> NewPosition;
	NewPosition[0] = m_Translation_AfterRotation[0] + R[0] * x + R[3] * y + R[6] * z;
	NewPosition[1] = m_Translation_AfterRotation[1] + R[1] * x + R[4] * y + R[7] * z;
	NewPosition[2] = m_Translation_AfterRotation[2] + R[2] * x + R[5] * y + R[8] * z;
	return NewPosition;
}

}//namespace mdk

