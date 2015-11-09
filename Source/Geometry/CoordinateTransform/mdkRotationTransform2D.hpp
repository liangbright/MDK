#ifndef mdk_RotationTransform2D_hpp
#define mdk_RotationTransform2D_hpp

namespace mdk
{

template<typename ScalarType>
RotationTransform2D<ScalarType>::RotationTransform2D()
{
	this->Clear();
}

template<typename ScalarType>
RotationTransform2D<ScalarType>::~RotationTransform2D()
{
}

template<typename ScalarType>
void RotationTransform2D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Rotation.Clear();
	m_Rotation.Resize(2, 2);
	m_Rotation.Fill(0);
	m_Rotation.FillDiagonal(1);
	m_Rotation.FixSize();
	m_RotationCenter.Fill(0);
	m_Translation_AfterRotation.Fill(0);
}

template<typename ScalarType>
void RotationTransform2D<ScalarType>::SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet)
{
	m_SourceLandmarkPointSet = SourceLandmarkPointSet;
}

template<typename ScalarType>
void RotationTransform2D<ScalarType>::SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet)
{
	m_TargetLandmarkPointSet = TargetLandmarkPointSet;
}


template<typename ScalarType>
void RotationTransform2D<ScalarType>::SetRotationMatrix(const DenseMatrix<ScalarType>& Rotation)
{
	if (Rotation.GetColCount() != 2 || Rotation.GetRowCount() != 2)
	{
		MDK_Error("Invalid size @ RotationTransform2D::SetRotationMatrix(...)")
		return;
	}
	m_Rotation = Rotation;

	//T=C-R*C
	const auto& C = Center;
	const auto R = m_Rotation.GetElementPointer();
	m_Translation_AfterRotation[0] = C[0] - (R[0] * C[0] + R[2] * C[1]);
	m_Translation_AfterRotation[1] = C[1] - (R[1] * C[0] + R[3] * C[1]);
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform2D<ScalarType>::GetRotationMatrix() const
{
	return m_Rotation;
}


template<typename ScalarType>
void RotationTransform2D<ScalarType>::SetRotationCenter(const DenseVector<ScalarType, 2>& Center)
{
	m_RotationCenter = Center;

	//T=C-R*C
	const auto& C = Center;
	const auto R = m_Rotation.GetElementPointer();
	m_Translation_AfterRotation[0] = C[0] - (R[0] * C[0] + R[2] * C[1]);
	m_Translation_AfterRotation[1] = C[1] - (R[1] * C[0] + R[3] * C[1]);
}


template<typename ScalarType>
DenseVector<ScalarType, 2> RotationTransform2D<ScalarType>::GetRotationCenter() const
{
	return m_RotationCenter;
}


template<typename ScalarType>
bool RotationTransform2D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ RotationTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size  @ RotationTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ RotationTransform2D::CheckLandmarkPointSet()")
		return false;

	}
		
	if (m_TargetLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size  @ RotationTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ RotationTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void RotationTransform2D<ScalarType>::EstimateParameter()
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

	// get Rotation Center: C = inv(I-R)*(Yc-R*Xc)
	DenseMatrix<ScalarType> ImiusR(2, 2);// I - R
	ImiusR = m_Rotation;
	ImiusR *= ScalarType(-1);
	ImiusR(0, 0) += 1;
	ImiusR(1, 1) += 1;
	Yc.Reshape(2, 1);
	DenseMatrix<ScalarType> Translation = Yc - m_Rotation*Xc;
	m_RotationCenter = ImiusR.PInv()*Translation;
	m_Translation_AfterRotation = Translation;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> RotationTransform2D<ScalarType>::TransformPoint(ScalarType x, ScalarType y) const
{
	const auto R = m_Rotation.GetElementPointer();

	DenseVector<ScalarType, 2> NewPosition;
	NewPosition[0] = m_Translation_AfterRotation[0] + R[0] * x + R[2] * y;
	NewPosition[1] = m_Translation_AfterRotation[1] + R[1] * x + R[3] * y;
	return NewPosition;
}

//----------------------------------- static function -----------------------------------------------------------//

template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform2D<ScalarType>::ComputeRotationMatrix_around_Z_axis(ScalarType Angle)
{
	auto CosZ = std::cos(Angle);
	auto SinZ = std::sin(Angle);
	DenseMatrix<ScalarType> Rz = { { CosZ, -SinZ},
					               { SinZ,  CosZ}};
	return Rz;
}


template<typename ScalarType>
DenseVector<ScalarType, 2> RotationTransform2D<ScalarType>::RotatePoint(const DenseVector<ScalarType, 2>& PointPosition,																		
																		const DenseMatrix<ScalarType>&    RotationMatrix,
																		const DenseVector<ScalarType, 2>& RotationCenter)
{	
	auto x = PointPosition[0] - RotationCenter[0];
	auto y = PointPosition[1] - RotationCenter[1];

	const auto P = RotationMatrix.GetElementPointer();

	DenseVector<ScalarType, 2> NewPosition;
	NewPosition[0] = RotationCenter[0] + P[0] * x + P[2] * y;
	NewPosition[1] = RotationCenter[1] + P[1] * x + P[3] * y;
	return NewPosition;
}


template<typename ScalarType>
DenseVector<ScalarType, 2> RotationTransform2D<ScalarType>::RotatePoint(const DenseVector<ScalarType, 2>& PointPosition,
																		const DenseMatrix<ScalarType>& RotationMatrix)
{//RotationCenter is [0, 0, 0]
	const auto& x = PointPosition[0];
	const auto& y = PointPosition[1];

	const auto R = RotationMatrix.GetElementPointer();

	DenseVector<ScalarType, 2> NewPosition;
	NewPosition[0] = R[0] * x + R[2] * y;
	NewPosition[1] = R[1] * x + R[3] * y;
	return NewPosition;
}

}//namespace mdk

#endif
