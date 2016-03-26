#pragma once

namespace mdk
{

template<typename ScalarType>
AffineTransform3D<ScalarType>::AffineTransform3D()
{
	this->Clear();
}

template<typename ScalarType>
AffineTransform3D<ScalarType>::~AffineTransform3D()
{
}

template<typename ScalarType>
void AffineTransform3D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Parameter.Clear();
	m_Parameter.Resize(3, 4);
	m_Parameter.FixSize();
	m_Parameter.Fill(0);
	m_Parameter(0, 0) = 1;
	m_Parameter(1, 1) = 1;
	m_Parameter(2, 2) = 1;
}

template<typename ScalarType>
void AffineTransform3D<ScalarType>::SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet)
{
	m_SourceLandmarkPointSet = SourceLandmarkPointSet;
}

template<typename ScalarType>
void AffineTransform3D<ScalarType>::SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet)
{
	m_TargetLandmarkPointSet = TargetLandmarkPointSet;
}


template<typename ScalarType>
bool AffineTransform3D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ AffineTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size  @ AffineTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ AffineTransform3D::CheckLandmarkPointSet()")
		return false;

	}

	if (m_TargetLandmarkPointSet->GetRowCount() != 3)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size  @ AffineTransform3D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ AffineTransform3D::CheckLandmarkPointSet()")
		return false;
	}
	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void AffineTransform3D<ScalarType>::EstimateParameter()
{
	if (this->CheckLandmarkPointSet() == false)
	{
		return;
	}

	// Assemble SourceData Matrix
	DenseMatrix<ScalarType> SourceData;
	SourceData.Resize(4, m_SourceLandmarkPointSet->GetColCount());
	for (int_max k = 0; k < m_SourceLandmarkPointSet->GetColCount(); ++k)
	{
		ScalarType Pos[3];
		m_SourceLandmarkPointSet->GetCol(k, Pos);
		SourceData(0, k) = Pos[0];//x
		SourceData(1, k) = Pos[1];//y
		SourceData(2, k) = Pos[2];//z
		SourceData(3, k) = 1;
	}

	m_Parameter = MatrixMultiply((*m_TargetLandmarkPointSet), SourceData.PInv());
}


template<typename ScalarType>
void AffineTransform3D<ScalarType>::SetTransformationMatrix(const DenseMatrix<ScalarType>& TransformationMatrix)
{
	if (TransformationMatrix.GetRowCount() != 4 || TransformationMatrix.GetColCount() != 4)
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
inline 
DenseVector<ScalarType, 3> AffineTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> NewPosition;
	//NewPosition[k] = m_Parameter(k, 0)*x + m_Parameter(k, 1)*y + m_Parameter(k, 2)*z + m_Parameter(k, 3);

	const auto P = m_Parameter.GetElementPointer();
	NewPosition[0] = P[0] * x + P[3] * y + P[6] * z + P[9];
	NewPosition[1] = P[1] * x + P[4] * y + P[7] * z + P[10];
	NewPosition[2] = P[2] * x + P[5] * y + P[8] * z + P[11];
	return NewPosition;
}

}//namespace mdk

