#ifndef __mdkRotationTransform3D_hpp
#define __mdkRotationTransform3D_hpp

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
	m_Rotation = this->ComputeRotationMatrixByAngle(AngleX, AngleY, AngleZ)
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
const DenseMatrix<ScalarType>& RotationTransform3D<ScalarType>::GetRotationMatrix() const
{
	return m_Rotation;
}


template<typename ScalarType>
bool RotationTransform3D<ScalarType>::CheckInput()
{
	if (m_SourceControlPointSet != nullptr)
	{
		if (m_SourceControlPointSet->GetRowNumber() != 3)
		{
			MDK_Error("m_SourceControlPointSet empty or wrong size  @  RotationTransform3D::CheckInput()")
			return false;
		}
	}

	if (m_TargetControlPointSet != nullptr)
	{
		if (m_SourceControlPointSet->GetRowNumber() != 3)
		{
			MDK_Error("m_TargetControlPointSet empty or wrong size  @  RotationTransform3D::CheckInput()")
			return false;
		}
	}

	if (m_SourceControlPointSet != nullptr && m_TargetControlPointSet != nullptr)
	{
		if (m_SourceControlPointSet->GetColNumber() != m_TargetControlPointSet->GetColNumber())
		{
			MDK_Error("m_SourceControlPointSet size and m_TargetControlPointSet size do not match @  RotationTransform3D::CheckInput()")
			return false;
		}
	}
	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
bool RotationTransform3D<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return false;
	}

	this->EstimateRotationMatrixFromControlPointSet();
	return true;
}


template<typename ScalarType>
void RotationTransform3D<ScalarType>::EstimateRotationMatrixFromControlPointSet()
{
	if (m_SourceControlPointSet == nullptr || m_TargetControlPointSet == nullptr)
	{
		return;
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
	DenseVector<ScalarType, 3> NewPosition;
	auto Ptr = m_Rotation.GetElementPointer();
	NewPosition[0] = Ptr[0] * x + Ptr[3] * y + Ptr[6] * z;
	NewPosition[1] = Ptr[1] * x + Ptr[4] * y + Ptr[7] * z;
	NewPosition[2] = Ptr[2] * x + Ptr[5] * y + Ptr[8] * z;
	return NewPosition;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> RotationTransform3D<ScalarType>::TransformPoint(const DenseVector<ScalarType, 3>& SourcePosition) const
{
	return this->TransformPoint(SourcePosition[0], SourcePosition[1], SourcePosition[2]);
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rx_ByAngle(ScalarType AngleX)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosX = std::cos(AngleX);
	auto SinX = std::sin(AngleX);
	DenseMatrix<ScalarType> Rx = { { One, Zero, Zero },
								   { Zero, CosX, -SinX},
								   { Zero, SinX, CosX } };
	return Rx;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_Ry_ByAngle(ScalarType AngleY)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosY = std::cos(AngleY);
	auto SinY = std::sin(AngleY);
	DenseMatrix<ScalarType> Ry = { { CosY, Zero, SinY },
					               { Zero,  One, Zero },
								   { -SinY, Zero, CosY } };
	return Ry;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rz_ByAngle(ScalarType AngleZ)
{
	auto One = ScalarType(1);
	auto Zero = ScalarType(0);
	auto CosZ = std::cos(AngleZ);
	auto SinZ = std::sin(AngleZ);
	DenseMatrix<ScalarType> Rz = { { CosZ, -SinZ, Zero },
					               { SinZ,  CosZ, Zero },
					               { Zero, Zero, One } };

	return Rz;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrixByAngle(const DenseVector<ScalarType, 3>& AngleList)
{
	return RotationTransform3D<ScalarType>::ComputeRotationMatrix_ByAngle(AngleList[0], AngleList[1], AngleList[2]);
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::ComputeRotationMatrixByAngle(ScalarType AngleX, ScalarType AngleY, ScalarType AngleZ)
{
	auto Rx = RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rx_ByAngle(AngleList[0]);
	auto Ry = RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rx_ByAngle(AngleList[1]);
	auto Rz = RotationTransform3D<ScalarType>::ComputeRotationMatrix_Rx_ByAngle(AngleList[2]);

	DenseMatrix<ScalarType> R = Rz*Ry*Rx;
	return R;
}


template<typename ScalarType>
DenseMatrix<ScalarType> RotationTransform3D<ScalarType>::
ComputeRotationMatrixByAngleAndAxis(ScalarType Angle, const DenseVector<ScalarType, 3>& Axis)
{
	auto One = ScalarType(1);
	DenseMatrix<ScalarType> R(3, 3);
	auto CosA = std::cos(Angle);
	auto SinA = std::sin(Angle);
	R[0] = CosA + Axis[0] * Axis[0] * (One - CosA);           //R(0, 0) 
	R[3] = Axis[0] * Axis[1] * (One - CosA) - Axis[2] * SinA; //R(0, 1)
	R[6] = Axis[0] * Axis[2] * (One - CosA) + Axis[1] * SinA; //R(0, 2)
	R[1] = Axis[0] * Axis[1] * (One - CosA) + Axis[2] * SinA; //R(1, 0)
	R[4] = CosA + Axis[1] * Axis[1] * (One - CosA);           //R(1, 1)
	R[7]= Axis[1] * Axis[2] * (One - CosA) - Axis[0] * SinA;  //R(1, 2)
	R[2] = Axis[0] * Axis[2] * (One - CosA) - Axis[1] * SinA; //R(2, 0) 
	R[5] = Axis[1] * Axis[2] * (One - CosA) + Axis[0] * SinA; //R(2, 1)
	R[8] = CosA + Axis[2] * Axis[2] * (One - CosA);           //R(2, 2)
	return R;
}


}//namespace mdk

#endif
