#pragma once

#include "mdkDenseMatrix.h"

namespace mdk
{
template<typename Scalar_Type>
class CoordinateTransform2D : public Object
{
public:
	typedef Scalar_Type ScalarType;

protected:
	CoordinateTransform2D() {}
	virtual ~CoordinateTransform2D() {}

public:
	virtual DenseVector<ScalarType, 2> TransformPoint(ScalarType x, ScalarType y) const = 0;

	DenseVector<ScalarType, 2> TransformPoint(const DenseVector<ScalarType, 2>& Position) const
	{
		return this->TransformPoint(Position[0], Position[1]);
	}

	DenseVector<ScalarType, 2> TransformPoint(const ScalarType Position[2]) const
	{
		return this->TransformPoint(Position[0], Position[1]);
	}

	DenseMatrix<ScalarType> TransformPoint(const DenseMatrix<ScalarType>& Position) const
	{
		auto ColCount = Position.GetColCount();
		auto RowCount = Position.GetRowCount();
		DenseMatrix<ScalarType> NewPosition;
		NewPosition.Resize(RowCount, ColCount);
		if (ColCount == 1 && RowCount == 2 || ColCount == 2 && RowCount == 1)//vector
		{
			NewPosition = this->TransformPoint(Position[0], Position[1]);
		}
		else if (RowCount == 2 && ColCount > 1)
		{
			for (int_max k = 0; k < ColCount; ++k)
			{
				auto Pos_k = this->TransformPoint(Position(0, k), Position(1, k));
				NewPosition.SetCol(k, Pos_k);
			}
		}
		else
		{
			MDK_Error("Invalid input @ CoordinateTransform3D::TransformPoint(...)")
		}
		return NewPosition;
	}

private:
	CoordinateTransform2D(const CoordinateTransform2D&) = delete;
	void operator=(const CoordinateTransform2D&) = delete;
};

}
