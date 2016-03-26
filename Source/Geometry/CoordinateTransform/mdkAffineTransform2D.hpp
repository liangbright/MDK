#pragma once

namespace mdk
{

template<typename ScalarType>
AffineTransform2D<ScalarType>::AffineTransform2D()
{
	this->Clear();
}

template<typename ScalarType>
AffineTransform2D<ScalarType>::~AffineTransform2D()
{
}

template<typename ScalarType>
void AffineTransform2D<ScalarType>::Clear()
{
	m_SourceLandmarkPointSet = nullptr;
	m_TargetLandmarkPointSet = nullptr;
	m_Parameter.Clear();
	m_Parameter.Resize(2, 3);
	m_Parameter.FixSize();
	m_Parameter.Fill(0);
	m_Parameter(0, 0) = 1;
	m_Parameter(1, 1) = 1;
}

template<typename ScalarType>
void AffineTransform2D<ScalarType>::SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet)
{
	m_SourceLandmarkPointSet = SourceLandmarkPointSet;
}

template<typename ScalarType>
void AffineTransform2D<ScalarType>::SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet)
{
	m_TargetLandmarkPointSet = TargetLandmarkPointSet;
}


template<typename ScalarType>
bool AffineTransform2D<ScalarType>::CheckLandmarkPointSet()
{
	if (m_SourceLandmarkPointSet == nullptr)
	{
		MDK_Error("m_SourceLandmarkPointSet is nullptr @ AffineTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_SourceLandmarkPointSet wrong size @ AffineTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_TargetLandmarkPointSet == nullptr)
	{
		MDK_Error("m_TargetLandmarkPointSet is nullptr @ AffineTransform2D::CheckLandmarkPointSet()")
		return false;

	}

	if (m_TargetLandmarkPointSet->GetRowCount() != 2)
	{
		MDK_Error("m_TargetLandmarkPointSet wrong size @ AffineTransform2D::CheckLandmarkPointSet()")
		return false;
	}

	if (m_SourceLandmarkPointSet->GetColCount() != m_TargetLandmarkPointSet->GetColCount())
	{
		MDK_Error("m_SourceLandmarkPointSet size and m_TargetLandmarkPointSet size do not match @ AffineTransform2D::CheckLandmarkPointSet()")
		return false;
	}
	//-------------------------------------------------------------------------------------------
	return true;
}


template<typename ScalarType>
void AffineTransform2D<ScalarType>::EstimateParameter()
{
	if (this->CheckLandmarkPointSet() == false)
	{
		return;
	}

	// Assemble SourceData Matrix
	DenseMatrix<ScalarType> SourceData;
	SourceData.Resize(3, m_SourceLandmarkPointSet->GetColCount());
	for (int_max k = 0; k < m_SourceLandmarkPointSet->GetColCount(); ++k)
	{
		ScalarType Pos[3];
		m_SourceLandmarkPointSet->GetCol(k, Pos);
		SourceData(0, k) = Pos[0];//x
		SourceData(1, k) = Pos[1];//y
		SourceData(2, k) = 1;
	}

	m_Parameter = MatrixMultiply((*m_TargetLandmarkPointSet), SourceData.PInv());
}


template<typename ScalarType>
void AffineTransform2D<ScalarType>::SetTransformationMatrix(const DenseMatrix<ScalarType>& TransformationMatrix)
{
	if (TransformationMatrix.GetRowCount() != 3 || TransformationMatrix.GetColCount() != 3)
	{
		MDK_Error("invalid input @ AffineTransform2D::SetTransformationMatrix(...)")
		return;
	}

	m_Parameter.Clear();
	m_Parameter.Resize(2, 3);
	for (int_max j = 0; j < 3; ++j)
	{
		for (int_max i = 0; i < 2; ++i)
		{
			m_Parameter(i, j) = TransformationMatrix(i, j);
		}
	}
}


template<typename ScalarType>
DenseMatrix<ScalarType> AffineTransform2D<ScalarType>::GetTransformationMatrix() const
{
	DenseMatrix<ScalarType> TransformationMatrix(3, 3);

	for (int_max j = 0; j < 3; ++j)
	{
		for (int_max i = 0; i < 2; ++i)
		{
			TransformationMatrix(i, j) = m_Parameter(i, j);
		}
	}

	TransformationMatrix(2, 0) = 0;
	TransformationMatrix(2, 1) = 0;
	TransformationMatrix(2, 2) = 1;
	
	return TransformationMatrix;
}


template<typename ScalarType>
inline 
DenseVector<ScalarType, 2> AffineTransform2D<ScalarType>::TransformPoint(ScalarType x, ScalarType y) const
{
	DenseVector<ScalarType, 2> NewPosition;
	const auto P = m_Parameter.GetElementPointer();
	NewPosition[0] = P[0] * x + P[2] * y + P[4];
	NewPosition[1] = P[1] * x + P[3] * y + P[5];
	return NewPosition;
}

}//namespace mdk

