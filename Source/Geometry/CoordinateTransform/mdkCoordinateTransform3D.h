#pragma once

#include "mdkDenseMatrix.h"

namespace mdk
{
template<typename Scalar_Type>
class CoordinateTransform3D : public Object
{
public:
	typedef Scalar_Type ScalarType;

protected:
	CoordinateTransform3D() {}
	virtual ~CoordinateTransform3D() {}

public:
	virtual DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const = 0;

	DenseVector<ScalarType, 3> TransformPoint(const DenseVector<ScalarType, 3>& Position) const
	{
		return this->TransformPoint(Position[0], Position[1], Position[2]);
	}

	DenseVector<ScalarType, 3> TransformPoint(const ScalarType Position[3]) const
	{
		return this->TransformPoint(Position[0], Position[1], Position[2]);
	}

	DenseMatrix<ScalarType> TransformPoint(const DenseMatrix<ScalarType>& Position) const
	{
		auto ColCount = Position.GetColCount();
		auto RowCount = Position.GetRowCount();
		DenseMatrix<ScalarType> NewPosition;
		NewPosition.Resize(RowCount, ColCount);
		if (ColCount == 1 && RowCount == 3 || ColCount == 3 && RowCount == 1)//vector
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
			MDK_Error("Invalid input @ CoordinateTransform3D::TransformPoint(...)")
		}
		return NewPosition;
	}

private:
	CoordinateTransform3D(const CoordinateTransform3D&) = delete;
	void operator=(const CoordinateTransform3D&) = delete;
};

}
