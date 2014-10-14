#include <iostream>
#include "Test_DenseImageResampler3D.h"

using namespace mdk;

class TestBoolClass
{
public:
	bool m_Flag;
	TestBoolClass() {}
	~TestBoolClass() {}
	void TestBool(bool Flag = true)
	{
		m_Flag = Flag;
	}

};

int main()
{
    test_a();

	TestBoolClass Test;
	Test.TestBool();

	if (Test.m_Flag == true)
	{
		std::cout << "true" << '\n';
	}
	else if (Test.m_Flag == false)
	{
		std::cout << "false" << '\n';
	}
	else
	{
		std::cout << "other" << '\n';
	}

    std::system("pause");
}