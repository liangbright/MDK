#ifndef mdk_RotationTransform3D_hpp
#define mdk_RotationTransform3D_hpp

namespace mdk
{

template<typename ScalarType>
RotationTransform3D<ScalarType>::RotationTransform3D()
{
}

template<typename ScalarType>
RotationTransform3D<ScalarType>::~RotationTransform3D()
{
}

template<typename ScalarType>
void RotationTransform3D<ScalarType>::Clear()
{
	m_SourceControlPointSet = nullptr;
	m_TargetControlPointSet = nullptr;
	m_Rotation.Clear();
	m_RotationCenter.Fill(0);
}

template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetSourceControlPointSet(const DenseMatrix<ScalarType>* SourcePointSet)
{
	m_SourceControlPointSet = SourcePointSet;
}

template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetTargetControlPointSet(const DenseMatrix<ScalarType>* TargetPointSet)
{
	m_TargetControlPointSet = TargetPointSet;
}

template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetRotationMatrixByAngle(const DenseVector<ScalarType, 3>& AngleList)
{
	this->SetRotationMatrixByAngle(AngleList[0], AngleList[1], AngleList[2]);
}

template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetRotationMatrixByAngle(ScalarType AngleX, ScalarType AngleY, ScalarType AngleZ)
{
	m_Rotation = this->ComputeRotationMatrixByAngleXYZ(AngleX, AngleY, AngleZ)
}


template<typename ScalarType>
void RotationTransform3D<ScalarType>::SetRotationMatrix(const DenseMatrix<ScalarType>& Rotation)
{
	if (Rotation.GetColNumber() != 3 || Rotation.GetRowNumber() != 3)
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
bool RotationTransform3D<ScalarType>::CheckControlPointSet()
{
	if (m_SourceControlPointSet != nullptr)
	{
		if (m_SourceControlPointSet->GetRowNumber() != 3)
		{
			MDK_Error("m_SourceControlPointSet empty or wrong size  @  RotationTransform3D::CheckControlPointSet()")
			return false;
		}
	}

	if (m_TargetControlPointSet != nullptr)
	{
		if (m_SourceControlPointSet->GetRowNumber() != 3)
		{
			MDK_Error("m_TargetControlPointSet empty or wrong size  @  RotationTransform3D::CheckControlPointSet()")
			return false;
		}
	}

	if (m_SourceControlPointSet != nullptr && m_TargetControlPointSet != nullptr)
	{
		if (m_SourceControlPointSet->GetColNumber() != m_TargetControlPointSet->GetColNumber())
		{
			MDK_Error("m_SourceControlPointSet size and m_TargetControlPointSet size do not match @  RotationTransform3D::CheckControlPointSet()")
			return false;
		}
	}
	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void RotationTransform3D<ScalarType>::EstimateParameter()
{
	if (this->CheckControlPointSet() == false)
	{
		return
	}

	DenseMatrix<ScalarType> H(3,3);
	H.Fill(0);
	auto PointNumber = m_SourceControlPointSet->GetColNumber();
	for (int_max k = 0; k < PointNumber; ++k)
	{
		auto SourcePosition = m_SourceControlPointSet->GetCol(k);
		SourcePosition.Reshape(3, 1);
		auto TargetPosition = m_TargetControlPointSet->GetCol(k);
		TargetPosition.Reshape(1, 3);
		H += SourcePosition*TargetPosition;
	}

	auto SVDResult = H.SVD();
	
	DenseMatrix<ScalarType> UV = SVDResult.U*SVDResult.V.Transpose();
	auto UV_det = UV.Det();

	DenseMatrix<ScalarType> D(3, 3);
	D.SetDiagonal({ ScalarType(1), ScalarType(1), UV_det });

	m_Rotation = SVDResult.U * D * SVDResult.V.Transpose();
}


template<typename ScalarType>
DenseVector<ScalarType, 3> RotationTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{
	auto temp_x = x - m_RotationCenter[0];
	auto temp_y = y - m_RotationCenter[1];
	auto temp_z = z - m_RotationCenter[2];
	
	auto P = m_Rotation.GetElementPointer();

	DenseVector<ScalarType, 3> NewPosition;
	NewPosition[0] = m_RotationCenter[0] + P[0] * temp_x + P[3] * temp_y + P[6] * temp_z;
	NewPosition[1] = m_RotationCenter[1] + P[1] * temp_x + P[4] * temp_y + P[7] * temp_z;
	NewPosition[2] = m_RotationCenter[2] + P[2] * temp_x + P[5] * temp_y + P[8] * temp_z;
	return NewPosition;
}

//----------------------------------- static function -----------------------------------------------------------//

template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rx_ByAngleX(ScalarType AngleX)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosX = std::cos(AngleX);
	auto SinX = std::sin(AngleX);
	DenseMatrix<ScalarType> Rx = { { One,  Zero,  Zero },
								   { Zero, CosX, -SinX },
								   { Zero, SinX,  CosX } };
	return Rx;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_Ry_ByAngleY(ScalarType AngleY)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosY = std::cos(AngleY);
	auto SinY = std::sin(AngleY);
	DenseMatrix<ScalarType> Ry = { { CosY,  Zero, SinY },
					               { Zero,   One, Zero },
								   {-SinY,  Zero, CosY } };
	return Ry;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rz_ByAngleZ(ScalarType AngleZ)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosZ = std::cos(AngleZ);
	auto SinZ = std::sin(AngleZ);
	DenseMatrix<ScalarType> Rz = { { CosZ, -SinZ, Zero },
					               { SinZ,  CosZ, Zero },
					               { Zero,  Zero, One } };
	return Rz;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrixByAngleXYZ(const DenseVector<ScalarType, 3>& AngleList)
{
	return RotationTransform3D<ScalarType>::ComputeRotationMatrixByAngleXYZ(AngleList[0], AngleList[1], AngleList[2]);
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrixByAngleXYZ(ScalarType AngleX, ScalarType AngleY, ScalarType AngleZ)
{
	auto Rx = RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rx_ByAngleX(AngleX);
	auto Ry = RotationTransform3D<ScalarType>::ComputeRotationMatrix_Ry_ByAngleY(AngleY);
	auto Rz = RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rz_ByAngleZ(AngleZ);

	DenseMatrix<ScalarType> R = Rz*Ry*Rx;
	return R;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrixByAngleAlongAxis(ScalarType Angle, const DenseVector<ScalarType, 3>& Axis)
{
	DenseMatrix<ScalarType> R(3, 3);
	auto C = std::cos(Angle);
	auto S = std::sin(Angle);
	auto t = ScalarType(1) - C;
	auto x = Axis[0];
	auto y = Axis[1];
	auto z = Axis[2];

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
																		const DenseMatrix<ScalarType>& RotationMatrix,
																		const DenseVector<ScalarType, 3>& RotationCenter)
{	
	auto x = PointPosition[0] - RotationCenter[0];
	auto y = PointPosition[1] - RotationCenter[1];
	auto z = PointPosition[2] - RotationCenter[2];

	auto P = RotationMatrix.GetElementPointer();

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

	auto P = RotationMatrix.GetElementPointer();

	DenseVector<ScalarType, 3> NewPosition;
	NewPosition[0] = P[0] * x + P[3] * y + P[6] * z;
	NewPosition[1] = P[1] * x + P[4] * y + P[7] * z;
	NewPosition[2] = P[2] * x + P[5] * y + P[8] * z;
	return NewPosition;
}

}//namespace mdk

#endif
