#include <iostream>
#include "Test_DenseImageResampler3D.h"

using namespace mdk;

int main()
{
	unsigned int n = std::thread::hardware_concurrency();
	std::cout << n << " concurrent threads are supported.\n";

    test_a();

	//test_b();

	//test_c();
}