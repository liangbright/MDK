#include <iostream>

#include "Test_initializer_list.h"

using namespace mdk;

int main()
{
   
    // initializer_list sytax can be used for this!
    //double a;
    //a = { 1 };

    //error
    //std::vector<double> a;
    //std::initializer_list<double> b=a;

    
    Test_PassToFunction({1, 2 , 3});

    Test_PassToFunction({ "11111", "22222", "33333" });
}