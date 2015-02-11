#ifndef mdk_AffineTransform3D_hpp
#define mdk_AffineTransform3D_hpp

namespace mdk
{

template<typename ScalarType>
AffineTransform3D<ScalarType>::AffineTransform3D()
{
}

template<typename ScalarType>
AffineTransform3D<ScalarType>::~AffineTransform3D()
{
}

template<typename ScalarType>
void AffineTransform3D<ScalarType>::Clear()
{
	m_SourceControlPointSet = nullptr;
	m_TargetControlPointSet = nullptr;
	m_Parameter.Clear();
	m_Parameter.Resize(3, 4);
	m_Parameter.Fill(0);
	m_Parameter(0, 0) = 1;
	m_Parameter(1, 1) = 1;
	m_Parameter(2, 2) = 1;
}

template<typename ScalarType>
void AffineTransform3D<ScalarType>::SetSourceControlPointSet(const DenseMatrix<ScalarType>* SourcePointSet)
{
	m_SourceControlPointSet = SourcePointSet;
}

template<typename ScalarType>
void AffineTransform3D<ScalarType>::SetTargetControlPointSet(const DenseMatrix<ScalarType>* TargetPointSet)
{
	m_TargetControlPointSet = TargetPointSet;
}


template<typename ScalarType>
bool AffineTransform3D<ScalarType>::CheckInput()
{
	if (m_SourceControlPointSet == nullptr)
	{
		MDK_Error("m_SourceControlPointSet is empty (nullptr) @  AffineTransform3D::CheckInput()")
		return false;
	}
	else
	{
		if (m_SourceControlPointSet->GetRowNumber() != 3)
		{
			MDK_Error("m_SourceControlPointSet empty or wrong size  @  AffineTransform3D::CheckInput()")
			return false;
		}
	}

	if (m_TargetControlPointSet == nullptr)
	{
		MDK_Error("m_TargetControlPointSet is empty (nullptr) @  AffineTransform3D::CheckInput()")
		return false;
	}
	else
	{
		if (m_SourceControlPointSet->GetRowNumber() != 3)
		{
			MDK_Error("m_TargetControlPointSet empty or wrong size  @  AffineTransform3D::CheckInput()")
			return false;
		}
	}

	if (m_SourceControlPointSet->GetColNumber() != m_TargetControlPointSet->GetColNumber())
	{
		MDK_Error("m_SourceControlPointSet size and m_TargetControlPointSet size do not match @  AffineTransform3D::CheckInput()")
		return false;
	}
	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void AffineTransform3D<ScalarType>::EstimateParameter()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	// Assemble SourceData Matrix
	DenseMatrix<ScalarType> SourceData;
	SourceData.Resize(4, m_SourceControlPointSet->GetColNumber());
	for (int_max k = 0; k < m_SourceControlPointSet->GetColNumber(); ++k)
	{
		ScalarType Pos[3];
		m_SourceControlPointSet->GetCol(k, Pos);
		SourceData(0, k) = Pos[0];//x
		SourceData(1, k) = Pos[1];//y
		SourceData(2, k) = Pos[2];//z
		SourceData(3, k) = 1;
	}

	m_Parameter = MatrixMultiply((*m_TargetControlPointSet), SourceData.PInv());
}


template<typename ScalarType>
void AffineTransform3D<ScalarType>::SetTransformationMatrix(const DenseMatrix<ScalarType>& TransformationMatrix)
{
	if (TransformationMatrix.GetRowNumber() != 4 || TransformationMatrix.GetColNumber() != 4)
	{
		MDK_Error("invalid input @ AffineTransform3D::SetTransformationMatrix(...)")
		return;
	}

	m_Parameter.Clear();
	m_Parameter.Resize(3, 4);
	for (int_max j = 0; j < 4; ++j)
	{
		for (int_max i = 0; i < 3; ++i)
		{
			m_Parameter(i, j) = TransformationMatrix(i, j);
		}
	}
}


template<typename ScalarType>
DenseMatrix<ScalarType> AffineTransform3D<ScalarType>::GetTransformationMatrix() const
{
	DenseMatrix<ScalarType> TransformationMatrix(4, 4);

	for (int_max j = 0; j < 4; ++j)
	{
		for (int_max i = 0; i < 3; ++i)
		{
			TransformationMatrix(i, j) = m_Parameter(i, j);
		}
	}

	TransformationMatrix(3, 0) = 0;
	TransformationMatrix(3, 1) = 0;
	TransformationMatrix(3, 2) = 0;
	TransformationMatrix(3, 3) = 1;

	return TransformationMatrix;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> AffineTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> NewPosition;
	//NewPosition[k] = m_Parameter(k, 0)*x + m_Parameter(k, 1)*y + m_Parameter(k, 2)*z + m_Parameter(k, 3);

	auto P = m_Parameter.GetElementPointer();
	NewPosition[0] = P[0] * x + P[3] * y + P[6] * z + P[9];
	NewPosition[1] = P[1] * x + P[4] * y + P[7] * z + P[10];
	NewPosition[2] = P[2] * x + P[5] * y + P[8] * z + P[11];
	return NewPosition;
}

}//namespace mdk

#endif
