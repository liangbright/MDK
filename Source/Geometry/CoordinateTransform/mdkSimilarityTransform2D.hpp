#pragma once

namespace mdk
{

template<typename ScalarType>
SimilarityTransform2D<ScalarType>::SimilarityTransform2D()
{
	this->Clear();
}

template<typename ScalarType>
SimilarityTransform2D<ScalarType>::~SimilarityTransform2D()
{
}

template<typename ScalarType>
void SimilarityTransform2D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Scale = 1;
	m_Rotation.Clear();
	m_Rotation.Resize(2, 2);
	m_Rotation.Fill(0);
	m_Rotation.FillDiagonal(1);
	m_Rotation.FixSize();
	m_Translation_After_Scale_Rotation.Resize(2, 1);
	m_Translation_After_Scale_Rotation.Fill(0);
	m_Translation_After_Scale_Rotation.FixSize();
}


template<typename ScalarType>
bool SimilarityTransform2D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ SimilarityTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size @ SimilarityTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ SimilarityTransform2D::CheckLandmarkPointSet()")
		return false;

	}
		
	if (m_TargetLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size @ SimilarityTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ SimilarityTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void SimilarityTransform2D<ScalarType>::EstimateParameter()
{
	if (this->CheckLandmarkPointSet() == false)
	{
		return;
	}

	const auto& X = *m_SourceLandmarkPointSet;
	const auto& Y = *m_TargetLandmarkPointSet;

	auto PointCount = X.GetColCount();

	auto Xc = X.MeanOfEachRow();
	Xc.Reshape(1, 2);
	auto Yc = Y.MeanOfEachRow();
	Yc.Reshape(2, 1);

	DenseMatrix<ScalarType> H(2, 2);
	H.Fill(0);

	ScalarType SigmaX_sq = 0;

	for (int_max k = 0; k < PointCount; ++k)
	{
		auto Xk = X.GetCol(k);
		Xk.Reshape(1, 2);
		Xk -= Xc;

		auto Yk = Y.GetCol(k);
		Yk.Reshape(2, 1);
		Yk -= Yc;

		SigmaX_sq += Xk[0] * Xk[0] + Xk[1] * Xk[1];

		H += Yk*Xk;
	}
	SigmaX_sq /= ScalarType(PointCount);
	H /= ScalarType(PointCount);

	auto SVDResult = H.SVD();
	auto& U = SVDResult.U;
	auto& S = SVDResult.S;
	auto& V = SVDResult.V;	
	auto Vt = SVDResult.V.Transpose();
	//auto H_det = H.Det();
	
	DenseMatrix<ScalarType> D(2, 2);
	D.Fill(0);
	D(0, 0) = 1;
	auto U_det = U.Det();
	auto V_det = V.Det();
	if (U_det*V_det > 0)
	{
		D(1, 1) = 1;
	}
	else
	{
		D(1, 1) = -1;
	}

	// this is useless
	/*
	if (H_det > 0)
	{
		D(1, 1) = 1;
	}
	else
	{
		D(1, 1) = -1;
	}
	*/

	DenseMatrix<ScalarType> S_Matrix(S.GetElementCount(), S.GetElementCount());
	S_Matrix.Fill(0);
	S_Matrix.SetDiagonal(S);

	DenseMatrix<ScalarType> R = U * D * Vt;
	DenseMatrix<ScalarType> SD = S_Matrix*D;
	ScalarType trace_SD = SD.GetDiagonal().Sum();
	ScalarType s = trace_SD / SigmaX_sq;

	Xc.Reshape(2, 1);
	DenseMatrix<ScalarType> Traslation = Yc - s*R*Xc;

	//output
	m_Rotation = R;
	m_Scale = s;
	m_Translation_After_Scale_Rotation = Traslation;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 2> SimilarityTransform2D<ScalarType>::TransformPoint(ScalarType x, ScalarType y) const
{
	const auto& s = m_Scale;
	const auto R = m_Rotation.GetElementPointer();
	const auto& T = m_Translation_After_Scale_Rotation;

	DenseVector<ScalarType, 2> NewPosition;
	NewPosition[0] = T[0] + s*(R[0] * x + R[2] * y);
	NewPosition[1] = T[1] + s*(R[1] * x + R[3] * y);
	return NewPosition;
}

}//namespace mdk
