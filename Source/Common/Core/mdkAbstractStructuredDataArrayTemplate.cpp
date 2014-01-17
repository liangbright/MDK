#include <typeinfo>

#include <cstdlib>
#include <cstring>

#include "mdkDebug.h"
#include "mdkAbstractStructuredDataArrayTemplate.h"

namespace mdk
{

mdkAbstractStructuredDataArrayTemplate::mdkAbstractStructuredDataArrayTemplate()
{	
}


mdkAbstractStructuredDataArrayTemplate::~mdkAbstractStructuredDataArrayTemplate()
{
}


uint64 mdkAbstractStructuredDataArrayTemplate::CalScalarNumberInElement(uint64 ElementDimension, const uint64* ElementSize)
{
	if (ElementDimension == 0)
	{
		return 0;
	}

	uint64 ScalarNumber = 1;

	for (uint64 i = 0; i < ElementDimension; ++i)
	{
		ScalarNumber *= ElementSize[i];
	}

	return ScalarNumber;
}


uint64 mdkAbstractStructuredDataArrayTemplate::CalElementNumberInArray(uint64 ArrayDimension, const uint64* ArraySize)
{
	if (ArrayDimension == 0)
	{
		return 0;
	}

	uint64 ElementNumber = 1;

	for (uint64 i = 0; i < ArrayDimension; ++i)
	{
		ElementNumber *= ArraySize[i];
	}

	return ElementNumber;
}


uint64 mdkAbstractStructuredDataArrayTemplate::CalScalarNumberInArray(uint64 ElementDimension, const uint64* ElementSize,
	                                                          uint64 ArrayDimension, const uint64* ArraySize)
{
	auto ScalarNumberInElement = mdkAbstractStructuredDataArrayTemplate::CalScalarNumberInElement(ElementDimension, ElementSize);

	auto ElementNumber = mdkAbstractStructuredDataArrayTemplate::CalElementNumberInArray(ArrayDimension, ArraySize);

	return ElementNumber*ScalarNumberInElement;
}


uint64 mdkAbstractStructuredDataArrayTemplate::CalElementLinearIndex(const uint64* Position, uint64 ArrayDimension, const uint64* ArraySize)
{
	uint64 Index = 0;

	switch (ArrayDimension)
	{
	case 1:
		Index = Position[0];
		break;

	case 2:
		Index = Position[0] + Position[1] * ArraySize[0];
		break;

	case 3:
		Index = Position[0] + Position[1] * ArraySize[0] + Position[2] * ArraySize[1] * ArraySize[0];
		break;

	case 4:
		Index = Position[0] + Position[1] * ArraySize[0] + Position[2] * ArraySize[1] * ArraySize[0]
			+ Position[3] * ArraySize[2] * ArraySize[1] * ArraySize[0];
		break;

	default:

		Index = 0;

		auto tempValue = 1;

		for (uint64 i = 0; i < ArrayDimension; ++i)
		{
			Index += Position[i] * tempValue;

			tempValue *= ArraySize[i];
		}
	}

	return Index;
}


void mdkAbstractStructuredDataArrayTemplate::CalElementPosition(uint64 Index, uint64 ArrayDimension, const uint64* ArraySize, uint64* Position)
{
	switch (ArrayDimension)
	{
	case 1:
		Position[0] = Index;

		break;

	case 2:
		std::lldiv_t divresult;
		divresult.rem = Index;

		divresult = div(divresult.rem, ArraySize[0]);

		Position[1] = divresult.quot;
		Position[0] = divresult.rem;

		break;

	case 3:
		std::lldiv_t divresult;
		divresult.rem = Index;

		divresult = div(divresult.rem, ArraySize[1] * ArraySize[0]);

		Position[2] = divresult.quot;

		divresult = div(divresult.rem, ArraySize[0]);

		Position[1] = divresult.quot;

		Position[0] = divresult.rem;

		break;

	case 4:
		std::lldiv_t divresult;
		divresult.rem = Index;

		divresult = div(divresult.rem, ArraySize[2] * ArraySize[1] * ArraySize[0]);

		Position[3] = divresult.quot;

		divresult = div(divresult.rem, ArraySize[1] * ArraySize[0]);

		Position[2] = divresult.quot;

		divresult = div(divresult.rem, ArraySize[0]);

		Position[1] = divresult.quot;

		Position[0] = divresult.rem;

		break;

	default:
		std::lldiv_t divresult;
		divresult.rem = Index;

		uint64 tempValue = 1;
		for (uint64 i = 0; i < ArrayDimension - 1; ++i)
		{
			tempValue *= ArraySize[i];
		}

		for (uint64 i = ArrayDimension - 1; i >= 1; --i)
		{
			divresult = div(divresult.rem, tempValue);

			Position[i] = divresult.quot;

			tempValue /= ArraySize[i];
		}

		Position[0] = divresult.rem;
	}

}


}//end namespace mdk