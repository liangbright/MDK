#include <iomanip>

#include "mdkDenseMatrix.h"
#include "mdkQuadraticProgrammingSolver.h"
#include "mdkLinearLeastSquaresProblemSolver.h"


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

void test_QProblem()
{
    using namespace qpOASES;

    /* Setup data of first QP. */
    double H[2 * 2] = { 1.0, 0.0, 0.0, 0.5 };

    double A[1 * 2] = { 1.0, 1.0 };
    double g[2] = { 1.5, 1.0 };
    double lb[2] = { 0.5, -2.0 };
    double ub[2] = { 5.0, 2.0 };
    double lbA[1] = { -1.0 };
    double ubA[1] = { 2.0 };

    /* Setup data of second QP. */
    double g_new[2] = { 1.0, 1.5 };
    double lb_new[2] = { 0.0, -1.0 };
    double ub_new[2] = { 5.0, -0.5 };
    double lbA_new[1] = { -2.0 };
    double ubA_new[1] = { 1.0 };


    /* Setting up QProblem object. */
    QProblem<double> example(2, 1);

    Options<double> options;
    example.setOptions(options);

    /* Solve first QP. */
    int_max nWSR = 10;
    example.init(H, g, A, lb, ub, lbA, ubA, nWSR);

    /* Get and print solution of first QP. */
    double xOpt[2];
    double yOpt[2 + 1];
    example.getPrimalSolution(xOpt);
    example.getDualSolution(yOpt);
    printf("\nxOpt = [ %e, %e ];  yOpt = [ %e, %e, %e ];  objVal = %e\n\n",
        xOpt[0], xOpt[1], yOpt[0], yOpt[1], yOpt[2], example.getObjVal());
    
    //change H
    
    H[0] = 1.0;
    H[1] = 100.0;
    H[2] = 100.0;
    H[3] = 1.5;
    
    /* Solve second QP. */
    nWSR = 10;
    example.hotstart(g_new, lb_new, ub_new, lbA_new, ubA_new, nWSR);

    /* Get and print solution of second QP. */
    example.getPrimalSolution(xOpt);
    example.getDualSolution(yOpt);
    printf("\nxOpt = [ %e, %e ];  yOpt = [ %e, %e, %e ];  objVal = %e\n\n",
        xOpt[0], xOpt[1], yOpt[0], yOpt[1], yOpt[2], example.getObjVal());

    example.printOptions();

    std::system("pause");
}

void test_SQProblem()
{
    USING_NAMESPACE_QPOASES

    /* Setup data of first QP. */
    double H[2 * 2] = { 1.0, 0.0, 0.0, 0.5 };
    double A[1 * 2] = { 1.0, 1.0 };
    double g[2] = { 1.5, 1.0 };
    double lb[2] = { 0.5, -2.0 };
    double ub[2] = { 5.0, 2.0 };
    double lbA[1] = { -1.0 };
    double ubA[1] = { 2.0 };

    /* Setup data of second QP. */
   // double H_new[2 * 2] = { 1.0, 0.5, 0.5, 0.5 };
    double A_new[1 * 2] = { 1.0, 5.0 };
    double g_new[2] = { 1.0, 1.5 };
    double lb_new[2] = { 0.0, -1.0 };
    double ub_new[2] = { 5.0, -0.5 };
    double lbA_new[1] = { -2.0 };
    double ubA_new[1] = { 1.0 };


    /* Setting up SQProblem object and solution analyser. */
    SQProblem<double> example(2, 1);
    SolutionAnalysis<double> analyser;

    /* Solve first QP ... */
    int_max nWSR = 10;
    example.init(H, g, A, lb, ub, lbA, ubA, nWSR, 0);

    double xOpt[2];
    double yOpt[2 + 1];
    example.getPrimalSolution(xOpt);
    example.getDualSolution(yOpt);
    printf("\nxOpt = [ %e, %e ];  yOpt = [ %e, %e, %e ];  objVal = %e\n\n",
            xOpt[0], xOpt[1], yOpt[0], yOpt[1], yOpt[2], example.getObjVal());


    double H_new[2 * 2] = { 1.0, 0.5, 0.5, 0.5 };
    /*
    auto H_new = H;
    H_new[0] = 1.0;
    H_new[1] = 0.5;
    H_new[2] = 0.5;
    H_new[3] = 0.5;
    */


    /* Solve second QP ... */
    nWSR = 10;
    example.hotstart(H_new, g_new, A_new, lb_new, ub_new, lbA_new, ubA_new, nWSR, 0);

    /* Get and print solution of second QP. */
    example.getPrimalSolution(xOpt);
    example.getDualSolution(yOpt);
    printf("\nxOpt = [ %e, %e ];  yOpt = [ %e, %e, %e ];  objVal = %e\n\n",
        xOpt[0], xOpt[1], yOpt[0], yOpt[1], yOpt[2], example.getObjVal());


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

    qpSolver.m_Option.MaxIterNumber = 10;

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


void test_qp_Online_Varing_H_A()
{
    using namespace mdk;

    DenseMatrix<double> H = { { 1.0, 0.0 },
                              { 0.0, 0.5 } };
    DenseMatrix<double> A = { 1.0, 1.0 };
    DenseMatrix<double> g = { 1.5, 1.0 };
    DenseMatrix<double> lb = { 0.5, -2.0 };
    DenseMatrix<double> ub = { 5.0, 2.0 };
    DenseMatrix<double> lbA = { -1.0 };
    DenseMatrix<double> ubA = { 2.0 };

    DenseMatrix<double> H_new = { { 1.0, 0.5 },
                                  { 0.5, 0.5 } };
    DenseMatrix<double> A_new =   { 1.0, 5.0 };
    DenseMatrix<double> g_new = { 1.0, 1.5 };
    DenseMatrix<double> lb_new = { 0.0, -1.0 };
    DenseMatrix<double> ub_new = { 5.0, -0.5 };
    DenseMatrix<double> lbA_new = { -2.0 };
    DenseMatrix<double> ubA_new = { 1.0 };

    mdk::QuadraticProgrammingSolver<double> qpSolver;

    qpSolver.m_Option.Mode = "Online_Varying_H_A";

    qpSolver.m_Option.MaxIterNumber = 10;

    qpSolver.SetInputData(&H, &g, &lb, &ub, &A, &lbA, &ubA);

    qpSolver.Update();

    auto Solution1 = qpSolver.GetSolution();

    DisplayMatrix("X1", Solution1->X);

    std::cout << "ObjVal_1: " << Solution1->ObjectiveFunctionValue << '\n';


    qpSolver.SetInputData(&H_new, &g_new, &lb_new, &ub_new, &A_new, &lbA_new, &ubA_new);

    qpSolver.Update();

    auto Solution2 = qpSolver.GetSolution();

    DisplayMatrix("X2", Solution2->X);

    std::cout << "ObjVal_2: " << Solution2->ObjectiveFunctionValue << '\n';

    std::system("pause");

}

void test_lsqlin_by_qp()
{
    using namespace mdk;
    
    DenseMatrix<double> D(2, 2);

    D= { { 1.0, 0.0 },
         { 0.0, 0.5 } };

    DenseMatrix<double> c(2, 1);
    c= { 1.5, 1.0 };
    
    DenseMatrix<double> lb(2, 1);
    lb= { 0.5, -2.0 };

    DenseMatrix<double> ub(2, 1);
    ub = { 5.0, 2.0 };

    DenseMatrix<double> A(1, 2);
    A= { 1.0, 1.0 };
    DenseMatrix<double> lbA = { -1.0 };
    DenseMatrix<double> ubA = { 2.0 };

    auto Solution = LinearLeastSquaresProblemSolver<double>::Apply(&D, &c, &lb, &ub, &A, &lbA, &ubA);

    DisplayMatrix("X", Solution.X);

    std::system("pause");
}


int main()
{

   // test_matrix();

    //test_QProblem();

   // test_qp();

   // test_SQProblem();

   // test_qp_Online_Varing_H_A();

    test_lsqlin_by_qp();

    std::system("pause");
}

