#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include "mdkStructuredDataArray.h"

namespace mdk
{

// 2D Matrix Class, each entry is a scalar
// column major, i.e., a column is an element in mdkStructuredDataArray
// RowNumber  =  ElementSize[0]
// ColumNumber = ElementNumberInArray
// a matrix can dynamicly expand in column direction: via member function InsertColumn implemented in this class
//
// linear algebra library Eigen works better with column major matrix
// If heavy linear algebra is required, then convert mdkMatrix to Eigen matrix, do something, and convert back
//
class mdkMatrix : mdkStructuredDataArray
{

private:

	uint64 m_RowNumber;
	
	uint64 m_ColumnNumber;

public:		
	
	mdkMatrix();
	~mdkMatrix();

	bool Initialize(mdkScalarTypeEnum ScalarType, uint64 RowNumber, uint64 ColumnNumber);


	mdkMatrix operator+(mdkMatrix& SourceMatrix);

	mdkMatrix operator-(mdkMatrix& SourceMatrix);

	mdkMatrix operator*(mdkMatrix& SourceMatrix);

	mdkMatrix operator/(mdkMatrix& SourceMatrix);

	mdkMatrix& operator*=(mdkMatrix& SourceMatrix);

	mdkMatrix& operator-=(mdkMatrix& SourceMatrix);
};

}//end namespace mdk

#endif