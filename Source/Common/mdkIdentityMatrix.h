#ifndef __mdkIdentityMatrix_h
#define __mdkIdentityMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"

namespace mdk
{

template<typename ScalarType>
class mdkIdentityMatrix : public mdkObject
{

private:

	uint64 m_Length; // Col x  Row = m_Length x  m_Length
	
	mdkScalarTypeEnum m_ScalarType;

public:		
	
	inline mdkIdentityMatrix();
	inline ~mdkIdentityMatrix();

	inline mdkScalarTypeEnum GetScalarType();

	inline mdkScalarTypeEnum FindScalarType();

	//---------------------- Matrix Size ----------------------------------------//

	inline bool SetSize(uint64 Length);

	inline void GetSize(uint64* RowNumber, uint64* ColNumber);

	inline uint64 GetSize();

	inline bool IsEmpty();
	//---------------------- Matrix + - * / ----------------------------------------//

	template<typename ScalarType_target>
	inline	mdkMatrix operator+(mdkIdentityMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator-(mdkIdentityMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator*(mdkIdentityMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator/(mdkIdentityMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator+(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator-(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator*(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator/(mdkMatrix<ScalarType_target>& targetMatrix);

    //---------------------- scalar + - * / ----------------------------------------//

	template<typename ScalarType_target>
	inline	mdkMatrix operator+(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline	mdkMatrix operator-(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline	mdkMatrix operator*(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline	mdkMatrix operator/(ScalarType_target Scalar);

};

}//end namespace mdk

#include "mdkIdentityMatrix.hpp"

#endif