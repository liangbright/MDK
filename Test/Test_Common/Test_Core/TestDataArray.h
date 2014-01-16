#ifndef __TestDataArray_h
#define __TestDataArray_h

#include "mdkDataArray.h"

#define ElementLength_in 10

#define ElementNumber_in 10

namespace mdk
{

void TestDataArray()
{

	mdkDataArray<double> DataArray;

	DataArray.SetElementLength(ElementLength_in);

	DataArray.SetElementNumber(ElementNumber_in);

	DataArray.Allocate();

	auto ElementNumber = DataArray.GetElementNumber();

	auto ElementLength = DataArray.GetElementLength();

	for (uint64 i = 0; i < ElementNumber; ++i)
	{
		double Element_in[ElementLength_in];

		double Element_out[ElementLength_in];

		DataArray.GetElement(i, Element_out);

		for (uint64 j = 0; j < ElementLength; ++j)
		{
			std::cout << Element_out[j] << ", ";

			Element_in[j] = i;
		}
		std::cout << '\n';

		DataArray.SetElement(i, Element_in);
	}


	std::cout << "out put" << '\n';

	for (uint64 i = 0; i < ElementNumber; ++i)
	{
		double Element_out[ElementLength_in];

		DataArray.GetElement(i, Element_out);

		for (uint64 j = 0; j < ElementLength; ++j)
		{
			std::cout << Element_out[j] << ", ";
		}
		std::cout << '\n';
	}

	for (uint64 i = ElementNumber; i < 2*ElementNumber; ++i)
	{
		double Element_in[ElementLength_in];

		for (uint64 j = 0; j < ElementLength; ++j)
		{
			Element_in[j] = i;
		}

		DataArray.InsertElement(i, Element_in);
	}


	for (uint64 i = 0; i < 2*ElementNumber; ++i)
	{
		double Element_out[ElementLength_in];

		DataArray.GetElement(i, Element_out);

		for (uint64 j = 0; j < ElementLength; ++j)
		{
			std::cout << Element_out[j] << ", ";
		}
		std::cout << '\n';
	}


	std::cout << "ElementNumber before Squeeze " << DataArray.GetElementNumber() << '\n';

	std::cout << "EffectiveElementNumber before Squeeze " << DataArray.GetEffectiveElementNumber() << '\n';

	DataArray.Squeeze();

	std::cout << "ElementNumber after Squeeze " << DataArray.GetElementNumber() << '\n';

	std::cout << "EffectiveElementNumber after Squeeze " << DataArray.GetEffectiveElementNumber() << '\n';

}


}//end of namespace

#endif