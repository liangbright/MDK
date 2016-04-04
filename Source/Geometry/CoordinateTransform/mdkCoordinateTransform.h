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

	DenseMatrix<ScalarType> TransformPoint(const DenseMatrix<ScalarType>& Position) const
	{
		auto ColCount = Position.GetColCount();
		auto RowCount = Position.GetRowCount();
		DenseMatrix<ScalarType> NewPosition;
		NewPosition.Resize(RowCount, ColCount);
		if (ColCount == 1 && RowCount == 2 || ColCount == 2 && RowCount == 1)//2D vector
		{
			NewPosition = this->TransformPoint(Position[0], Position[1], Position[2]);
		}
		else if (RowCount == 2 && ColCount > 1)
		{
			for (int_max k = 0; k < ColCount; ++k)
			{
				auto Pos_k = this->TransformPoint(Position(0, k), Position(1, k));
				NewPosition.SetCol(k, Pos_k);
			}
		}
		else if (ColCount == 1 && RowCount == 3 || ColCount == 3 && RowCount == 1)//3D vector
		{
			NewPosition = this->TransformPoint(Position[0], Position[1], Position[2]);
		}
		else if(RowCount == 3 && ColCount > 1)
		{
			for (int_max k = 0; k < ColCount; ++k)
			{
				auto Pos_k = this->TransformPoint(Position(0, k), Position(1, k), Position(2, k));
				NewPosition.SetCol(k, Pos_k);
			}
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
