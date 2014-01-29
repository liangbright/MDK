#ifndef __mdkLinearAlgebra_h
#define __mdkLinearAlgebra_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkMatrix.h"

namespace mdk
{

class mdkLinearAlgebra : public mdkObject
{

private:		
	
	mdkLinearAlgebra();
	~mdkLinearAlgebra();

public:

	template<typename ScalarType>
	inline static mdkMatrix<ScalarType> Transpose(mdkMatrix<ScalarType>& Matrix);

	template<typename ScalarType>
	inline static mdkMatrix<ScalarType> ElementOperation(std::string Operator,
		                                                 mdkMatrix<ScalarType>& MatrixA,
		                                                 mdkMatrix<ScalarType>& MatrixB);

	template<typename OperatorType, typename ScalarType>
	inline static mdkMatrix<ScalarType> ElementOperation(OperatorType Operator,
		                                                 mdkMatrix<ScalarType>& MatrixA,
		                                                 mdkMatrix<ScalarType>& MatrixB);

	template<typename ScalarType>
	inline static mdkMatrix<ScalarType> Convolution(mdkMatrix<ScalarType>& MatrixA, mdkMatrix<ScalarType>& MatrixB);

	template<typename ScalarType>
	inline	static mdkMatrix<ScalarType> LinearCombine(double alpha1, mdkMatrix<ScalarType>& Matrix1, double alpha2, mdkMatrix<ScalarType>& Matrix2);

	template<typename ScalarType>
	inline	static mdkMatrix<ScalarType> LinearCombine(double alpha1, mdkMatrix<ScalarType>& Matrix1, double alpha2, mdkMatrix<ScalarType>& Matrix2,
		                                               double alpha3, mdkMatrix<ScalarType>& Matrix3);

	template<typename ScalarType>
	inline	static mdkMatrix<ScalarType> LinearCombine(double alpha1, mdkMatrix<ScalarType>& Matrix1, double alpha2, mdkMatrix<ScalarType>& Matrix2,
		                                               double alpha3, mdkMatrix<ScalarType>& Matrix3, double alpha4, mdkMatrix<ScalarType>& Matrix4);

};

}//end namespace mdk

#endif