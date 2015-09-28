#ifndef mdk_RotationTransform3D_hpp
#define mdk_RotationTransform3D_hpp

namespace mdk
{

template<typename ScalarType>
RotationTransform3D<ScalarType>::RotationTransform3D()
{
	this->Clear();
}

template<typename ScalarType>
RotationTransform3D<ScalarType>::~RotationTransform3D()
{
}

template<typename ScalarType>
void RotationTransform3D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Rotation.Clear();
	m_Rotation.Resize(3, 3);
	m_Rotation.Fill(0);
	m_Rotation.FillDiagonal(1);
	m_Rotation.FixSize();
	m_RotationCenter.Fill(0);
}

template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet)
{
	m_SourceLandmarkPointSet = SourceLandmarkPointSet;
}

template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet)
{
	m_TargetLandmarkPointSet = TargetLandmarkPointSet;
}


template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetRotationMatrix(const DenseMatrix<ScalarType>& Rotation)
{
	if (Rotation.GetColCount() != 3 || Rotation.GetRowCount() != 3)
	{
		MDK_Error("Invalid size @ RotationTransform3D::SetRotationMatrix(...)")
		return;
	}
	m_Rotation = Rotation;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::GetRotationMatrix() const
{
	return m_Rotation;
}


template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetRotationCenter(const DenseVector<ScalarType, 3>& Center)
{
	m_RotationCenter = Center;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> RotationTransform3D<ScalarType>::GetRotationCenter() const
{
	return m_RotationCenter;
}


template<typename ScalarType>
bool RotationTransform3D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ RotationTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size  @ RotationTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ RotationTransform3D::CheckLandmarkPointSet()")
		return false;

	}
		
	if (m_TargetLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size  @ RotationTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ RotationTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void RotationTransform3D<ScalarType>::EstimateParameter()
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

	// get Rotation Center: C = inv(I-R)*(Yc-R*Xc)
	DenseMatrix<ScalarType> ImiusR(3, 3);// I - R
	ImiusR = m_Rotation;
	ImiusR *= ScalarType(-1);
	ImiusR(0, 0) += 1;
	ImiusR(1, 1) += 1;
	ImiusR(2, 2) += 1;
	Yc.Reshape(3, 1);
	DenseMatrix<ScalarType> Translation_AfterRotation = Yc - m_Rotation*Xc;
	m_RotationCenter = ImiusR.PInv()*Translation_AfterRotation;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 3> RotationTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{
	auto temp_x = x - m_RotationCenter[0];
	auto temp_y = y - m_RotationCenter[1];
	auto temp_z = z - m_RotationCenter[2];
	
	const auto R = m_Rotation.GetElementPointer();

	DenseVector<ScalarType, 3> NewPosition;
	NewPosition[0] = m_RotationCenter[0] + R[0] * temp_x + R[3] * temp_y + R[6] * temp_z;
	NewPosition[1] = m_RotationCenter[1] + R[1] * temp_x + R[4] * temp_y + R[7] * temp_z;
	NewPosition[2] = m_RotationCenter[2] + R[2] * temp_x + R[5] * temp_y + R[8] * temp_z;
	return NewPosition;
}

//----------------------------------- static function -----------------------------------------------------------//

template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_around_X_axis(ScalarType Angle)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosX = std::cos(Angle);
	auto SinX = std::sin(Angle);
	DenseMatrix<ScalarType> Rx = { { One,  Zero,  Zero },
								   { Zero, CosX, -SinX },
								   { Zero, SinX,  CosX } };
	return Rx;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_around_Y_axis(ScalarType Angle)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosY = std::cos(Angle);
	auto SinY = std::sin(Angle);
	DenseMatrix<ScalarType> Ry = { { CosY,  Zero, SinY },
					               { Zero,   One, Zero },
								   {-SinY,  Zero, CosY } };
	return Ry;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_around_Z_axis(ScalarType Angle)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosZ = std::cos(Angle);
	auto SinZ = std::sin(Angle);
	DenseMatrix<ScalarType> Rz = { { CosZ, -SinZ, Zero },
					               { SinZ,  CosZ, Zero },
					               { Zero,  Zero, One } };
	return Rz;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_around_Direction(ScalarType Angle, const DenseVector<ScalarType, 3>& Direction)
{
	DenseMatrix<ScalarType> R(3, 3);
	auto C = std::cos(Angle);
	auto S = std::sin(Angle);
	auto t = ScalarType(1) - C;
	auto x = Direction[0];
	auto y = Direction[1];
	auto z = Direction[2];

	R[0] = t*x*x + C;    //R(0, 0) 
	R[1] = t*x*y + S*z;  //R(1, 0)
	R[2] = t*x*z - S*y;  //R(2, 0) 
	R[3] = t*x*y - S*z;  //R(0, 1)
	R[4] = t*y*y + C;    //R(1, 1)
	R[5] = t*y*z + S*x;  //R(2, 1)
	R[6] = t*x*z + S*y;  //R(0, 2)
	R[7] = t*y*z - S*x;  //R(1, 2)
	R[8] = t*z*z + C;    //R(2, 2)
	return R;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> RotationTransform3D<ScalarType>::RotatePoint(const DenseVector<ScalarType, 3>& PointPosition,																		
																		const DenseMatrix<ScalarType>&    RotationMatrix,
																		const DenseVector<ScalarType, 3>& RotationCenter)
{	
	auto x = PointPosition[0] - RotationCenter[0];
	auto y = PointPosition[1] - RotationCenter[1];
	auto z = PointPosition[2] - RotationCenter[2];

	const auto P = RotationMatrix.GetElementPointer();

	DenseVector<ScalarType, 3> NewPosition;
	NewPosition[0] = RotationCenter[0] + P[0] * x + P[3] * y + P[6] * z;
	NewPosition[1] = RotationCenter[1] + P[1] * x + P[4] * y + P[7] * z;
	NewPosition[2] = RotationCenter[2] + P[2] * x + P[5] * y + P[8] * z;
	return NewPosition;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> RotationTransform3D<ScalarType>::RotatePoint(const DenseVector<ScalarType, 3>& PointPosition,
																		const DenseMatrix<ScalarType>& RotationMatrix)
{//RotationCenter is [0, 0, 0]
	const auto& x = PointPosition[0];
	const auto& y = PointPosition[1];
	const auto& z = PointPosition[2];

	const auto R = RotationMatrix.GetElementPointer();

	DenseVector<ScalarType, 3> NewPosition;
	NewPosition[0] = R[0] * x + R[3] * y + R[6] * z;
	NewPosition[1] = R[1] * x + R[4] * y + R[7] * z;
	NewPosition[2] = R[2] * x + R[5] * y + R[8] * z;
	return NewPosition;
}

}//namespace mdk

#endif
