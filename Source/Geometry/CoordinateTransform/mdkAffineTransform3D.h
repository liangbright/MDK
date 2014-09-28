#ifndef __mdkAffineTransform3D_h
#define __mdkAffineTransform3D_h

#include "mdkProcessObject.h"
#include "mdkDenseMatrix.h"
#include "mdkLinearLeastSquaresProblemSolver.h"

namespace mdk
{
// TargetPoint = AffineTransform(SourcePoint)
// 
//	x_new = a0 + a1 * x + a2 * y + a3 * z;
//	y_new = b0 + b1 * x + b2 * y + b3 * z;
//	z_new = c0 + c1 * x + c2 * y + c3 * z;
//
// TransformationMatrix =  [a1 a2 a3 a0
//                          b1 b2 b3 b0
//                          c1 c2 c3 c0
//                          0   0  0  1];
//
// Parameter = [a0, b0, c0 
//              a1, b1, c1
//              a2, b2, c2
//              a3, b3, c3]
//
//  Data = [1 x_1 y_1 z_1
//          1 x_2 y_2 z_2
//          .............
//          1 x_N y_N z_N];
//
//  Data_new=[XData YData ZData];
//  XData=[x_1_new, x_2_new, ..., x_N_new ]'   (col vector)
//  YData=[y_1_new, y_2_new, ..., t_N_new ]'   (col vector)
//  ZData=[z_1_new, z_2_new, ..., z_N_new ]'   (col vector)
//
//  [XData, YData, ZData] = Data * Parameter;
//  XData=Data*Parameter(:, 1)
//  YData=Data*Parameter(:, 2)
//  ZData=Data*Parameter(:, 3)
//
//  Parameter = pinv(Data)*Data_new;

// ScalarType is float or double
template<typename Scalar_Type>
class AffineTransform3D : public ProcessObject
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceControlPointSet and m_TargetControlPointSet must be valid when UpdateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceControlPointSet; // 3 x N
	const DenseMatrix<ScalarType>* m_TargetControlPointSet; // 3 x N

	DenseMatrix<ScalarType> m_Parameter;  // 4 x 3

public:
	AffineTransform3D();
	~AffineTransform3D();

	void Clear();

	void SetSourceControlPointSet(const DenseMatrix<ScalarType>* SourcePointSet);

	void SetTargetControlPointSet(const DenseMatrix<ScalarType>* TargetPointSet);

	void SetTransformationMatrix(const DenseMatrix<ScalarType>& TransformationMatrix);
	DenseMatrix<ScalarType> GetTransformationMatrix() const;

	bool Update() { this->UpdateParameter(); return true; }
	void UpdateParameter();

	DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const;
	DenseVector<ScalarType, 3> TransformPoint(const DenseVector<ScalarType, 3>& SourcePosition) const;

private:
	bool CheckInput();

private:
	AffineTransform3D(const AffineTransform3D&) = delete;
	void operator=(const AffineTransform3D&) = delete;
};

}//namespace mdk

#include "mdkAffineTransform3D.hpp"

#endif
