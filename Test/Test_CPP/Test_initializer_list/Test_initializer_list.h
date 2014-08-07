#ifndef __TestImageFilter_h
#define __TestImageFilter_h

#include <ctime>
#include <cstdlib>
#include <vector>

#include "mdkMatrix.h"
#include "mdk3DImage.h"

using namespace mdk;

template<typename T>
void Test_PassToFunction(const std::initializer_list<T>& data)
{
    Test_PassToNextFunction(data);
}

template<typename T>
void Test_PassToNextFunction(const std::initializer_list<T>& data)
{
    std::vector<T> ok = data;

    for (int i = 0; i < ok.size(); ++i)
    {
        std::cout << "ok =" << ok[i] <<'\n';
    }

    Test_PassToNextNextFunction(data.begin(), data.size());
}

template<typename T>
void Test_PassToNextNextFunction(T* data, int L)
{
    for (int i = 0; i < L; ++i)
    {
        std::cout << "ok =" << data[i] << '\n';
    }
}

#endif