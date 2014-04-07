#include <iomanip>

#include "mdkDenseMatrix.h"
#include "mdkQuadraticProgrammingSolver.h"


template<typename ElementType>
void DisplayMatrix(const std::string& Name, const mdk::DenseMatrix<ElementType>& InputMatrix, mdk::int_max precision = 6, bool Flag_scientific = false)
{
    std::cout << Name << " = " << '\n';

    if (Flag_scientific == true)
    {
        std::cout << std::scientific << std::setprecision(precision);
    }
    else
    {
        std::cout << std::setprecision(precision) << std::fixed;
    }

    for (mdk::int_max i = 0; i < InputMatrix.GetRowNumber(); ++i)
    {
        for (mdk::int_max j = 0; j < InputMatrix.GetColNumber(); ++j)
        {
            std::cout << std::setw(6 + precision) << InputMatrix(i, j) << ' ';
        }
        std::cout << '\n';
    }
}

void test_matrix()
{

    mdk::DenseMatrix<double> H = { { 1.0, 2.0 }, 
                                    { 3.0, 4.0 } };

    mdk::DenseMatrix<double> A = { { 1.0, 2.0 }, 
                                   { 3.0, 4.0 },
                                   { 5.0, 6.0 } };

    H.TransposeInPlace();

    qpOASES::SymDenseMat<double> SymH(2, 2, 2, H.GetElementPointer());

    qpOASES::DenseMatrix<double> denseA(3, 2, 2, A.GetElementPointer());


    SymH.print();

    denseA.print();
}

void test_qpOASES()
{
    /* Setup data of first QP. */
    double H[2 * 2] = { 1.0, 2.0, 3.0, 4.0 };
    double A[1 * 2] = { 1.0, 2.0 };
    double g[2] = { 1.5, 1.0 };
    double lb[2] = { 0.5, -2.0 };
    double ub[2] = { 5.0, 2.0 };
    double lbA[1] = { -1.0 };
    double ubA[1] = { 2.0 };


	/* Setting up QProblem object. */
    qpOASES::QProblem<double> example(2, 1);

    qpOASES::Options<double> options;
	example.setOptions( options );

	/* Solve first QP. */
    qpOASES::int_max nWSR = 10;
	example.init( H,g,A,lb,ub,lbA,ubA, nWSR );

	/* Get and print solution of second QP. */
	double xOpt[2];
	double yOpt[2+1];
	example.getPrimalSolution( xOpt );
	example.getDualSolution( yOpt );
	printf( "\nxOpt = [ %e, %e ];  yOpt = [ %e, %e, %e ];  objVal = %e\n\n", 
			xOpt[0],xOpt[1],yOpt[0],yOpt[1],yOpt[2],example.getObjVal() );
	
	example.printOptions();

    //--------------------------------------------------------------------

    std::system("pause");

}

void test_qp()
{
    using namespace mdk;

    DenseMatrix<double> H = { { 1.0, 2.0 }, 
                              { 2.0, 4.0 } };
    DenseMatrix<double> A = { 1.0, 1.0 };
    DenseMatrix<double> g = { 1.5, 1.0 };
    DenseMatrix<double> lb = { 0.5, -2.0 };
    DenseMatrix<double> ub = { 5.0, 2.0 };
    DenseMatrix<double> lbA = { -1.0 };
    DenseMatrix<double> ubA = { 2.0 };
    
    DenseMatrix<double> x0 = { 0.0 , 0.0};

    mdk::QuadraticProgrammingSolver<double> qpSolver;

    qpSolver.m_Option.IsOnlineMode = false;

    qpSolver.m_Option.MaxWSR = 10;

    qpSolver.SetInputData(&H, &g, &lb, &ub, &A, &lbA, &ubA, &x0);

    qpSolver.Update();

    auto Solution = qpSolver.GetSolution();

    DisplayMatrix("X", Solution->X);

    std::cout << "ObjVal: " << Solution->ObjectiveFunctionValue << '\n';


    auto OutputSolution = mdk::QuadraticProgrammingSolver<double>::Apply(&H, &g, &lb, &ub, &A, &lbA, &ubA);

    DisplayMatrix("X", OutputSolution.X);

    std::cout << "ObjVal: " << OutputSolution.ObjectiveFunctionValue << '\n';


    std::system("pause");
    
}


int main()
{
    //test_qpOASES();

   // test_matrix();

    test_qp();

    std::system("pause");
}
