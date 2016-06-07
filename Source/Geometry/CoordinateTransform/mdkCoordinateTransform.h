#pragma once

#include "mdkDenseMatrix.h"

namespace mdk
{
template<typename Scalar_Type>
class CoordinateTransform : public Object
{
public:
	typedef Scalar_Type ScalarType;

protected:
	CoordinateTransform() {}
	virtual ~CoordinateTransform() {}

public:
	virtual DenseVector<ScalarType, 2> TransformPoint(ScalarType x, ScalarType y) const
	{
		DenseVector<ScalarType, 2> Empty;
		Empty[0] = 0;
		Empty[1] = 0;		
		return Empty;
	}

	virtual DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const
	{
		DenseVector<ScalarType, 3> Empty;
		Empty[0] = 0;
		Empty[1] = 0;
		Empty[2] = 0;
		return Empty;
	}

	DenseVector<ScalarType, 2> TransformPoint(const DenseVector<ScalarType, 2>& Position) const
	{
		return this->TransformPoint(Position[0], Position[1]);
	}

	DenseVector<ScalarType, 3> TransformPoint(const DenseVector<ScalarType, 3>& Position) const
	{
		return this->TransformPoint(Position[0], Position[1], Position[2]);
	}

	DenseVector<ScalarType> TransformPoint(const DenseVector<ScalarType>& Position) const
	{
		auto Dim = Position.GetLength();
		DenseVector<ScalarType> NewPosition;
		NewPosition.Resize(Dim);
		if (Dim == 2)//2D vector
		{
			NewPosition = this->TransformPoint(Position[0], Position[1]);
		}
		else if (Dim == 3)
		{		
			NewPosition = this->TransformPoint(Position[0], Position[1], Position[2]);
		}
		else
		{
			MDK_Error("Invalid input @ CoordinateTransform::TransformPoint(...)")
		}
		return NewPosition;
	}

	DenseMatrix<ScalarType> TransformPoint(const DenseMatrix<ScalarType>& Position) const
	{
		auto Dim = Position.GetElementCount();
		DenseMatrix<ScalarType> NewPosition;
		NewPosition.Resize(Position.GetSize());
		if (Dim == 2)//2D vector
		{
			NewPosition = this->TransformPoint(Position[0], Position[1]);
		}
		else if (Dim == 3)
		{
			NewPosition = this->TransformPoint(Position[0], Position[1], Position[2]);
		}
		else
		{
			MDK_Error("Invalid input @ CoordinateTransform::TransformPoint(...)")
		}
		return NewPosition;
	}

private:
	CoordinateTransform(const CoordinateTransform&) = delete;
	void operator=(const CoordinateTransform&) = delete;
};

}
