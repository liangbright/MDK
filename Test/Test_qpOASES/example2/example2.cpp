/*
 *	This file is part of qpOASES.
 *
 *	qpOASES -- An Implementation of the Online Active Set Strategy.
 *	Copyright (C) 2007-2014 by Hans Joachim Ferreau, Andreas Potschka,
 *	Christian Kirches et al. All rights reserved.
 *
 *	qpOASES is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation; either
 *	version 2.1 of the License, or (at your option) any later version.
 *
 *	qpOASES is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *	See the GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with qpOASES; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


/**
 *	\file examples/example2.cpp
 *	\author Boris Houska, Hans Joachim Ferreau
 *	\version 3.0beta
 *	\date 2008-2014
 *
 *	Very simple example for testing qpOASES in combination
 *  with the SolutionAnalysis class.
 */



#include <qpOASES.h>


/** Example for qpOASES main function using the SolutionAnalysis class. */
int main( )
{
	USING_NAMESPACE_QPOASES

	/* Setup data of first QP. */
	double H[2*2] = { 1.0, 0.0, 0.0, 0.5 };
	double A[1*2] = { 1.0, 1.0 };
	double g[2] = { 1.5, 1.0 };
	double lb[2] = { 0.5, -2.0 };
	double ub[2] = { 5.0, 2.0 };
	double lbA[1] = { -1.0 };
	double ubA[1] = { 2.0 };

	/* Setup data of second QP. */
	double H_new[2*2] = { 1.0, 0.5, 0.5, 0.5 };
	double A_new[1*2] = { 1.0, 5.0 };
	double g_new[2] = { 1.0, 1.5 };
	double lb_new[2] = { 0.0, -1.0 };
	double ub_new[2] = { 5.0, -0.5 };
	double lbA_new[1] = { -2.0 };
	double ubA_new[1] = { 1.0 };


	/* Setting up SQProblem object and solution analyser. */
	SQProblem<double> example( 2,1 );
    SolutionAnalysis<double> analyser;

	/* Solve first QP ... */
	int_max nWSR = 10;
	example.init( H,g,A,lb,ub,lbA,ubA, nWSR,0 );

	/* ... and analyse it. */
	double maxKKTviolation;
    analyser.getMaxKKTviolation( &example, maxKKTviolation );
    printf( "maxKKTviolation: %e\n", maxKKTviolation );

	/* Solve second QP ... */
	nWSR = 10;
	example.hotstart( H_new,g_new,A_new,lb_new,ub_new,lbA_new,ubA_new, nWSR,0 );

	/* ... and analyse it. */
	analyser.getMaxKKTviolation( &example, maxKKTviolation );
    printf( "maxKKTviolation: %e\n", maxKKTviolation );


//  ------------ VARIANCE-COVARIANCE EVALUATION --------------------

        double *Var              = new double[5*5];
        double *Primal_Dual_Var  = new double[5*5];

        int run1, run2;
        for (run1 = 0; run1 < 5 * 5; run1++)
        {
            Var[run1] = 0.0;
        }

        Var[0] = 1.0;
        Var[6] = 1.0;

//                  (  1   0   0   0   0   )
//                  (  0   1   0   0   0   )
//     Var     =    (  0   0   0   0   0   )
//                  (  0   0   0   0   0   )
//                  (  0   0   0   0   0   )


        analyser.getVarianceCovariance( &example, Var,Primal_Dual_Var );

        printf("\nPrimal_Dual_VAR = \n");
        for( run1 = 0; run1 < 5; run1++ ){
          for( run2 = 0; run2 < 5; run2++ ){
            printf(" %10f", Primal_Dual_Var[run1*5+run2]);
          }
          printf("\n");
        }

        delete[] Primal_Dual_Var;
        delete[] Var;


        std::system("pause");

	return 0;
}


/*
 *	end of file
 */
