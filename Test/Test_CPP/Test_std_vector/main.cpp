#include <iostream>
#include <vector>
#include <string>
#include <memory>

#define noexcept
//throw()

static int global_int = 0;

class MatrixClass
{
public:
    int m_value;

    std::string m_str;

    std::shared_ptr<int> m_spt;

    MatrixClass()
    {
        m_value = global_int;

        global_int += 1;

        m_spt = std::make_shared<int>(10);

        std::cout << "(): " << m_value << '\n';
    }

    MatrixClass(int a)
    {
        m_value = global_int;

        global_int += 1;

        std::cout << "(int " <<  a << ") : " << m_value << '\n';
    }

    MatrixClass(const MatrixClass& A)
    {
        m_value = A.m_value;

        std::cout << "(const MatrixClass&) : " << m_value << '\n';
    }

    MatrixClass(MatrixClass&& A) noexcept
    {
        m_value = A.m_value;

        std::cout << "(MatrixClass&&) : " << m_value << '\n';
    }

    void operator=(const MatrixClass& A)
    {
        m_value = A.m_value;

        std::cout << " operator=(const MatrixClass&): " << m_value << '\n';
    }

    void operator=(MatrixClass&& A)
    {
        m_value = A.m_value;

        std::cout << " operator=(MatrixClass&&): " << m_value << '\n';
    }

    ~MatrixClass()
    {
        std::cout << "~() : " << m_value << '\n';
    }

};


void test_resize()
{

    std::vector<MatrixClass> List;

    //std::cout << "reserve(10)" << '\n';
    //List.reserve(10);

    List.clear();

    std::cout << "Capacity=" << List.capacity() << '\n';

    std::cout << "resize(1)"<< '\n';
    List.resize(1);

    std::cout << "Capacity=" << List.capacity() << '\n';

    std::cout << "resize(2)" << '\n';
    List.resize(2);

    std::cout << "Capacity=" << List.capacity() << '\n';

    std::cout << "resize(10)" << '\n';
    List.resize(10);

    std::cout << "Capacity=" << List.capacity() << '\n';

    std::cout << "end of test_resize()" << '\n';
}


void test_emplace_back()
{
    std::vector<MatrixClass> List;

    //std::cout << "reserve(10)" << '\n';
    //List.reserve(10);

    std::cout << "resize(1)" << '\n';
    List.resize(1);

    std::cout << "reserve(2)" << '\n';
    List.reserve(2);

    std::cout << "emplace_back()" << '\n';
    List.emplace_back();

    std::cout << "reserve(10)" << '\n';
    List.reserve(10);

    std::cout << "for {emplace_back(i)}" << '\n';

    for (int i = 0; i < 8; ++i)
    {
        List.emplace_back(i);
    }

    std::cout << "reserve(11)" << '\n';
    List.reserve(11);

    std::cout << "end of emplace_back()" << '\n';
}

void test_emplace()
{
    std::vector<MatrixClass> List;

    //std::cout << "reserve(10)" << '\n';
    //List.reserve(10);

    std::cout << "resize(1)" << '\n';
    List.resize(1);

    std::cout << "resize(2)" << '\n';
    List.resize(2);

    std::cout << "emplace(0, 11)" << '\n';
    List.emplace(List.begin(), 11);

    std::cout << "end of test_emplace()" << '\n';
}

void test_insert()
{
    std::vector<MatrixClass> List;

    //std::cout << "reserve(10)" << '\n';
    //List.reserve(10);

    std::cout << "resize(1)" << '\n';
    List.resize(1);

    std::cout << "resize(2)" << '\n';
    List.resize(2);

    std::cout << "insert(0, 11)" << '\n';
    List.insert(List.begin(), 11);

    std::cout << "end of test_insert()" << '\n';
}

void test_erase()
{
    std::vector<MatrixClass> List;

    //std::cout << "reserve(10)" << '\n';
    //List.reserve(10);

    std::cout << "resize(1)" << '\n';
    List.resize(1);

    std::cout << "resize(2)" << '\n';
    List.resize(2);

    std::cout << "erase(0)" << '\n';
    List.erase(List.begin());

    std::cout << "end of test_erase()" << '\n';
}


int main()
{

    //test_resize();

    test_emplace_back();

    //test_emplace();

    //test_insert();

    //test_erase();

    std::cout << "end of main" << '\n';

    std::system("pause");
}