#include <iostream>

#include <string>

class MatrixClass
{
public:
    int m_value;

    std::string m_str;

    MatrixClass(int a)
    {
        m_value = a;
        std::cout << "hello: " << m_value << '\n';
    }

    MatrixClass(const MatrixClass& A)
    {
        m_value = A.m_value;

        std::cout << "hello: " << m_value;

        if (A.m_str == "temp_in_*")
        {
            std::cout << ", string: " << "copied from temp_in_*";
        }
        else
        {
            std::cout << ", string: " << A.m_str;
        }

        std::cout << '\n';
    }

    MatrixClass(MatrixClass&& A)
    {
        m_value = A.m_value;

        std::cout << "hello: " << m_value;

        if (A.m_str == "temp_in_*")
        {
            std::cout << ", string: " << "copied from temp_in_*";
        }
        else
        {
            std::cout << ", string: " << A.m_str;
        }

        std::cout << '\n';
    }

    void operator=(const MatrixClass& A)
    {
        m_value = A.m_value;

        std::cout << " = hello: " << m_value;

        if (A.m_str == "temp_in_*")
        {
            std::cout << ", string: " << "copied from temp_in_*";
        }

        std::cout << '\n';

    }

    ~MatrixClass()
    {
        std::cout << "bye: " << m_value;

        if (m_str == "temp_in_*")
        {
            std::cout << ", string: " << "temp_in_*";
        }

        std::cout << '\n';
    }

};

MatrixClass& operator+(MatrixClass& tempClassA, MatrixClass& tempClassB)
{
    tempClassA.m_value += tempClassB.m_value;
    return tempClassA;
}

MatrixClass operator*(MatrixClass& tempClassA, MatrixClass& tempClassB)
{
    MatrixClass Temp(101010);
    Temp.m_value = tempClassA.m_value * tempClassB.m_value;

    Temp.m_str = "temp_in_*";

    return Temp;
}

MatrixClass FunctionA(MatrixClass A)
{
    A.m_value += 1;
    return A;
}

MatrixClass FunctionA1(const MatrixClass& A)
{
    MatrixClass B(1010);
    B.m_value += A.m_value;
    return B;
}


MatrixClass FunctionA2(MatrixClass&& A)
{
    MatrixClass B(1010);
    B.m_value += A.m_value;
    return B;
}


MatrixClass FunctionB()
{
    MatrixClass B(101010);

    return std::move(B);
}


int main()
{
    //FunctionA(FunctionB());

    //FunctionA1(FunctionB());

    FunctionA2(FunctionB());

    /*
    MatrixClass A1(2);
    MatrixClass A2(3);
    MatrixClass A3(10);
    MatrixClass A4(11);
    MatrixClass A5(12);

    std::cout << "start of expression " << '\n';

    MatrixClass A6(0);

    A6 = A1 * A2 + A3 * A4 + A5 + A6;

    std::cout << "end of expression " << '\n';

    std::cout << "A6.m_value: " << A6.m_value << '\n';
    */

    std::system("pause");
}