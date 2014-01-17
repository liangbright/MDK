#include "mdkPointSet.h"

namespace mdk
{

mdkPointSet::mdkPointSet()
{
}

mdkPointSet::~mdkPointSet()
{
}

bool mdkPointSet::SetPointDimension(uint32 Dim)
{
	if (Dim > 4)
	{
		return false;
	}

	this->SetElementDimension(Dim);
}


bool mdkPointSet::SetPointNumber(uint64 Number)
{
	this->SetElementNumber(Number);
}

bool mdkPointSet::SetPoint(uint64 PointIndex, double x)
{
	return this->SetElement(PointIndex, x);
}

bool mdkPointSet::SetPoint(uint64 PointIndex, double x, double y)
{
	return this->SetElement(PointIndex, x, y);
}

bool mdkPointSet::SetPoint(uint64 PointIndex, double x, double y, double z)
{
	return this->SetElement(PointIndex, x, y, z);
}

bool mdkPointSet::SetPoint(uint64 PointIndex, double x, double y, double z, double t)
{
	return this->SetElement(PointIndex, x, y, z, t);
}

bool mdkPointSet::InsertPoint(uint64 PointIndex, double x)
{
	return this->InsertElement(PointIndex, x);
}

bool mdkPointSet::InsertPoint(uint64 PointIndex, double x, double y)
{
	return this->InsertElement(PointIndex, x, y);
}

bool mdkPointSet::InsertPoint(uint64 PointIndex, double x, double y, double z)
{
	return this->InsertElement(PointIndex, x, y, z);
}

bool mdkPointSet::InsertPoint(uint64 PointIndex, double x, double y, double z, double t)
{
	return this->InsertElement(PointIndex, x, y, z, t);
}

}//end namespace mdk