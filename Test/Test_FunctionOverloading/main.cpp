#include <iostream>

#include <string>

void FunctionA(const int& a)
{
    std::cout << a;
}


void FunctionA(const int* a)
{
    std::cout << *a;
}

class TestFunctionInClass
{
public:
    TestFunctionInClass() {};
    ~TestFunctionInClass() {};

    void FunctionA(const int& a)
    {
        std::cout << a;
    }


    void FunctionA(const int* a)
    {
        std::cout << *a;
    }

};

template<typename T>
class TestFunctionInTemplateClass
{
public:
    TestFunctionInTemplateClass() {};
    ~TestFunctionInTemplateClass() {};

    void FunctionA(const T& a)
    {
        std::cout << a;
    }


    void FunctionA(const T* a)
    {
        std::cout << *a;
    }

};


int main()
{
    int a = 1;

    FunctionA(a);

    FunctionA(&a);

    TestFunctionInClass Test1;

    Test1.FunctionA(a);

    Test1.FunctionA(&a);


    TestFunctionInTemplateClass<int> Test2;

    Test1.FunctionA(a);

    Test1.FunctionA(&a);


    std::system("pause");
}