#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include "mdkDataArray.h"

namespace mdk
{

template<typename ScalarType>
class mdkMatrix : public mdkDataArray<ScalarType>
{

public:		
	//uint64 RowNumber = ElementLength
	//uint64 ColumNumber = ElementNumber
	//store: column major, i.e., a column is an element in mdkDataArray
	mdkMatrix();
	~mdkMatrix();

	bool Initialize(uint64 RowNumber, uint64 ColumnNumber);
};

}//end namespace mdk

#include "mdkMatrix.hpp"

#endif