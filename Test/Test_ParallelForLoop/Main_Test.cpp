#include <iostream>
#include <chrono>

#include "mdkDenseMatrix.h"
#include "mdkParallelForLoop.h"


using namespace mdk;

int main()
{
    auto L = 1000000;

    DenseMatrix<double> A(1, L);
    A.Fill(1);

    DenseMatrix<double> B(1, L);
    B.Fill(1);

    auto Function = [&](int_max i){ A[i] *= B[i]; };
   
    int_max MaxNumberOfThreads = 1;

    auto t0 = std::chrono::system_clock::now();

    for (int_max k = 0; k < 10000; ++k)
    {
        ParallelForLoop(Function, 0, L - 1, MaxNumberOfThreads);
    }

    auto t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> raw_time = t1 - t0;
    std::cout << "ParallelForLoop  time " << raw_time.count() << '\n';

    //DisplayMatrix("A", A);

    std::system("pause");

    return 1;
}