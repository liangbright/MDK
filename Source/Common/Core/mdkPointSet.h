#ifndef __mdkPointSet_h
#define __mdkPointSet_h

#include "DataArray/mdkDoubleDataArray.h"

namespace mdk
{

// points in 1D, 2D, 3D, 3D+t
// DataType: double
class mdkPointSet : public mdkDoubleDataArray
{

public:		
	mdkPointSet();
	~mdkPointSet();

	bool SetPointDimension(uint32 Dim);

	bool SetPointNumber(uint64 Number);

	uint32 GetPointDimension();

	uint64 GetPointNumber();

	bool SetPoint(uint64 PointIndex, double x);
	bool SetPoint(uint64 PointIndex, double x, double y);
	bool SetPoint(uint64 PointIndex, double x, double y, double z);
	bool SetPoint(uint64 PointIndex, double x, double y, double z, double t);
	bool SetPoint(uint64 PointIndex, const double* Pos);

	bool InsertPoint(uint64 PointIndex, double x);
	bool InsertPoint(uint64 PointIndex, double x, double y);
	bool InsertPoint(uint64 PointIndex, double x, double y, double z);
	bool InsertPoint(uint64 PointIndex, double x, double y, double z, double t);
	bool InsertPoint(uint64 PointIndex, const double* Pos);

	bool GetPoint(uint64 PointIndex, double* Pos);
};

}//end namespace mdk

#endif