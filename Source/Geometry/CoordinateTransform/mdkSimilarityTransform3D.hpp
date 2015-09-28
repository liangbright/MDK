#ifndef mdk_SimilarityTransform3D_hpp
#define mdk_SimilarityTransform3D_hpp

namespace mdk
{

template<typename ScalarType>
SimilarityTransform3D<ScalarType>::SimilarityTransform3D()
{
	this->Clear();
}

template<typename ScalarType>
SimilarityTransform3D<ScalarType>::~SimilarityTransform3D()
{
}

template<typename ScalarType>
void SimilarityTransform3D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Scale = 1;
	m_Rotation.Clear();
	m_Rotation.Resize(3, 3);
	m_Rotation.Fill(0);
	m_Rotation.FillDiagonal(1);
	m_Rotation.FixSize();
	m_Translation_After_Scale_Rotation.Fill(0);
}


template<typename ScalarType>
bool SimilarityTransform3D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ SimilarityTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size @ SimilarityTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ SimilarityTransform3D::CheckLandmarkPointSet()")
		return false;

	}
		
	if (m_TargetLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size @ SimilarityTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ SimilarityTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void SimilarityTransform3D<ScalarType>::EstimateParameter()
{
	if (this->CheckLandmarkPointSet() == false)
	{
		return;
	}

	const auto& X = *m_SourceLandmarkPointSet;
	const auto& Y = *m_TargetLandmarkPointSet;

	auto PointCount = X.GetColCount();

	auto Xc = X.MeanOfEachRow();
	Xc.Reshape(1, 3);
	auto Yc = Y.MeanOfEachRow();
	Yc.Reshape(3, 1);

	DenseMatrix<ScalarType> H(3,3);
	H.Fill(0);

	ScalarType SigmaX_sq = 0;

	for (int_max k = 0; k < PointCount; ++k)
	{
		auto Xk = X.GetCol(k);
		Xk.Reshape(1, 3);
		Xk -= Xc;

		auto Yk = Y.GetCol(k);
		Yk.Reshape(3, 1);
		Yk -= Yc;

		SigmaX_sq += Xk[0] * Xk[0] + Xk[1] * Xk[1] + Xk[2] * Xk[2];

		H += Yk*Xk;
	}
	SigmaX_sq /= ScalarType(PointCount);
	H /= ScalarType(PointCount);

	auto SVDResult = H.SVD();
	auto& U = SVDResult.U;
	auto& S = SVDResult.S;
	auto& V = SVDResult.V;	
	auto Vt = SVDResult.V.Transpose();
	auto H_det = H.Det();
	
	DenseMatrix<ScalarType> D(3, 3);
	D.Fill(0);
	D(0, 0) = 1;
	D(1, 1) = 1;
	if (H_det > 0)
	{
		D(2, 2) = 1;
	}
	else
	{
		D(2, 2) = -1;
	}

	DenseMatrix<ScalarType> R = U * D * Vt;

	DenseMatrix<ScalarType> SD = S*D;
	ScalarType trace_SD = SD.GetDiagonal().Sum();
	ScalarType s = trace_SD / SigmaX_sq;

	Xc.Reshape(3, 1);
	DenseMatrix<ScalarType> Traslation = Yc - s*R*Xc;

	//output
	m_Rotation = R;
	m_Scale = s;
	m_Translation_After_Scale_Rotation = Traslation;
}


template<typename ScalarType>
inline
DenseVector<ScalarType, 3> SimilarityTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{
	const auto& s = m_Scale;
	const auto R = m_Rotation.GetElementPointer();
	const auto& T = m_Translation_After_Scale_Rotation;

	DenseVector<ScalarType, 3> NewPosition;
	NewPosition[0] = T[0] + s*(R[0] * x + R[3] * y + R[6] * z);
	NewPosition[1] = T[1] + s*(R[1] * x + R[4] * y + R[7] * z);
	NewPosition[2] = T[2] + s*(R[2] * x + R[5] * y + R[8] * z);
	return NewPosition;
}


}//namespace mdk

#endif
