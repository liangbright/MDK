#ifndef __mdkDiagonalMatrix_h
#define __mdkDiagonalMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"

namespace mdk
{

template<typename ScalarType>
class mdkDiagonalMatrix : public mdkObject
{

private:

	uint64 m_Length; // Col x  Row = m_Length x  m_Length
	
	mdkScalarTypeEnum m_ScalarType;

public:		
	
	inline mdkDiagonalMatrix();
	inline ~mdkDiagonalMatrix();

	inline mdkScalarTypeEnum GetScalarType();

	inline mdkScalarTypeEnum FindScalarType();

	//---------------------- Matrix Size ----------------------------------------//

	inline bool SetSize(uint64 Length);

	inline void GetSize(uint64* RowNumber, uint64* ColNumber);

	inline uint64 GetSize();

	inline bool IsEmpty();

	//---------------------- fill Matrix  ----------------------------------------//

	inline bool Fill(ScalarType Scalar);

	//---------------------- Matrix + - * / ----------------------------------------//

	template<typename ScalarType_target>
	inline	mdkMatrix operator+(mdkDiagonalMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator-(mdkDiagonalMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator*(mdkDiagonalMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline	mdkMatrix operator/(mdkDiagonalMatrix<ScalarType_target>& targetMatrix);

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

#include "mdkDiagonalMatrix.hpp"

#endif