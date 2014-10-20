#ifndef __mdkAffineTransform3D_hpp
#define __mdkAffineTransform3D_hpp

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
void AffineTransform3D<ScalarType>::SetTransformationMatrix(const DenseMatrix<ScalarType>& TransformationMatrix)
{
	m_Parameter.FastResize(4, 3);

	m_Parameter(0, 0) = TransformationMatrix(0, 3);
	m_Parameter(1, 0) = TransformationMatrix(0, 0);
	m_Parameter(2, 0) = TransformationMatrix(0, 1);
	m_Parameter(3, 0) = TransformationMatrix(0, 2);

	m_Parameter(0, 1) = TransformationMatrix(1, 3);
	m_Parameter(1, 1) = TransformationMatrix(1, 0);
	m_Parameter(2, 1) = TransformationMatrix(1, 1);
	m_Parameter(3, 1) = TransformationMatrix(1, 2);

	m_Parameter(0, 2) = TransformationMatrix(2, 3);
	m_Parameter(1, 2) = TransformationMatrix(2, 0);
	m_Parameter(2, 2) = TransformationMatrix(2, 1);
	m_Parameter(3, 2) = TransformationMatrix(2, 2);
}


template<typename ScalarType>
DenseMatrix<ScalarType> AffineTransform3D<ScalarType>::GetTransformationMatrix() const
{
	DenseMatrix<ScalarType> TransformationMatrix(4, 4);

	TransformationMatrix(0, 3) = m_Parameter(0, 0);
	TransformationMatrix(0, 0) = m_Parameter(1, 0);
	TransformationMatrix(0, 1) = m_Parameter(2, 0);
	TransformationMatrix(0, 2) = m_Parameter(3, 0);

	TransformationMatrix(1, 3) = m_Parameter(0, 1);
	TransformationMatrix(1, 0) = m_Parameter(1, 1);
	TransformationMatrix(1, 1) = m_Parameter(2, 1);
	TransformationMatrix(1, 2) = m_Parameter(3, 1);

	TransformationMatrix(2, 3) = m_Parameter(0, 2);
	TransformationMatrix(2, 0) = m_Parameter(1, 2);
	TransformationMatrix(2, 1) = m_Parameter(2, 2);
	TransformationMatrix(2, 2) = m_Parameter(3, 2);

	TransformationMatrix(3, 0) = 0;
	TransformationMatrix(3, 1) = 0;
	TransformationMatrix(3, 2) = 0;
	TransformationMatrix(3, 3) = 1;

	return TransformationMatrix;
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
void AffineTransform3D<ScalarType>::UpdateParameter()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	// Assemble Data Matrix
	DenseMatrix<ScalarType> Data;
	Data.Resize(m_SourceControlPointSet->GetColNumber(), 4);
	for (int_max k = 0; k < m_SourceControlPointSet->GetColNumber(); ++k)
	{
		ScalarType Pos[3];
		m_SourceControlPointSet->GetCol(k, Pos);
		Data(k, 0) = 1;
		Data(k, 1) = Pos[0];
		Data(k, 2) = Pos[1];
		Data(k, 3) = Pos[2];
	}

	m_Parameter = MatrixMultiply(Data.PInv(), m_TargetControlPointSet->Transpose());

	/*
	// use LinearLeastSquaresProblemSolver
	// Get XData_new, YData_new, ZData_new and estimate m_Parameter
	m_Parameter.Resize(4, 3);
	for (int_max i = 0; i < 3; ++i)
	{
		auto DataVector_new = m_TargetControlPointSet->GetSubMatrix(i, ALL);
		DataVector_new.Reshape(DataVector_new.GetElementNumber(), 1);

		LinearLeastSquaresProblemSolver<ScalarType> LsqSolver;
		LsqSolver.m_Option.MethodType = LinearLeastSquaresProblemSolver<ScalarType>::MethodTypeEnum::SVD;
		LsqSolver.SetInputData(&Data, &DataVector_new);
		LsqSolver.Update();
		auto Solution = LsqSolver.GetSolution();

		m_Parameter(ALL, i) = Solution->X;
	}
	*/
}


template<typename ScalarType>
DenseVector<ScalarType, 3> AffineTransform3D<ScalarType>::TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
{
	DenseVector<ScalarType, 3> NewPosition;
	//NewPosition[0] = m_Parameter(0, 0) + m_Parameter(1, 0)*x + m_Parameter(2, 0)*y + m_Parameter(3, 0)*z;
	//NewPosition[1] = m_Parameter(0, 1) + m_Parameter(1, 1)*x + m_Parameter(2, 1)*y + m_Parameter(3, 1)*z;
	//NewPosition[2] = m_Parameter(0, 2) + m_Parameter(1, 2)*x + m_Parameter(2, 2)*y + m_Parameter(3, 2)*z;
	auto Ptr = m_Parameter.GetElementPointer();
	NewPosition[0] = Ptr[0] + Ptr[1] * x + Ptr[2] * y + Ptr[3] * z;
	NewPosition[1] = Ptr[4] + Ptr[5] * x + Ptr[6] * y + Ptr[7] * z;
	NewPosition[2] = Ptr[8] + Ptr[9] * x + Ptr[10] * y + Ptr[11] * z;
	return NewPosition;
}


template<typename ScalarType>
DenseVector<ScalarType, 3> AffineTransform3D<ScalarType>::TransformPoint(const DenseVector<ScalarType, 3>& SourcePosition) const
{
	return this->TransformPoint(SourcePosition[0], SourcePosition[1], SourcePosition[2]);
}

}//namespace mdk

#endif
