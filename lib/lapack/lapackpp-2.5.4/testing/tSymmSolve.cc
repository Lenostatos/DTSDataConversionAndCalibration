//
//              LAPACK++ 1.1 Linear Algebra Package 1.1
//               University of Tennessee, Knoxvilee, TN.
//            Oak Ridge National Laboratory, Oak Ridge, TN.
//        Authors: J. J. Dongarra, E. Greaser, R. Pozo, D. Walker
//                 (C) 1992-1996 All Rights Reserved
//
//                             NOTICE
//
// Permission to use, copy, modify, and distribute this software and
// its documentation for any purpose and without fee is hereby granted
// provided that the above copyright notice appear in all copies and
// that both the copyright notice and this permission notice appear in
// supporting documentation.
//
// Neither the Institutions (University of Tennessee, and Oak Ridge National
// Laboratory) nor the Authors make any representations about the suitability
// of this software for any purpose.  This software is provided ``as is''
// without express or implied warranty.
//
// LAPACK++ was funded in part by the U.S. Department of Energy, the
// National Science Foundation and the State of Tennessee.



#include "lafnames.h"
#include "lapack.h"
#include LA_GEN_MAT_DOUBLE_H
#include LA_VECTOR_DOUBLE_H
#include LA_SYMM_MAT_DOUBLE_H
#include LA_SOLVE_DOUBLE_H
#include LA_GENERATE_MAT_DOUBLE_H
#include LA_EXCEPTION_H
#include "blaspp.h"
#include LA_UTIL_H




double residual(LaSymmMatDouble &A, LaVectorDouble &x,
                const LaVectorDouble& b)
{
    // Symm matrices are always square
    int N = A.size(0);

    std::cout << "\tNorm_Inf(A*x-b)" << Norm_Inf(A * x - b) << std::endl;
    std::cout << "\tNorm_Inf(A) " << Norm_Inf(A) << std::endl;
    std::cout << "\tNorm_Inf(x) " << Norm_Inf(x) << std::endl;
    std::cout << "\tMacheps :" << Mach_eps_double() << std::endl;

    return Norm_Inf(A * x - b ) /
           ( Norm_Inf(A) * Norm_Inf(x) * N * Mach_eps_double());
}

int TestSymmLinearSolve(int N)
{
    LaSymmMatDouble A(N, N);
    LaVectorDouble x(N), b(N);

    const char fname[] = "TestSymmLinearSolve(LaGenMat, x, b) ";


    LaGenerateMatDouble(A);

    // save a snapshot of what A looked like before the solution
    LaSymmMatDouble old_A(A);


    b = 1.1;

    std::cerr << fname << ": testing LaLinearSolve(Symm,...) \n";
    LaLinearSolve(A, x, b);
    double norm;


    if ( (norm = Norm_Inf( old_A - A)) >  0.0)  // this is a hard test, not
        // necessary to worry about
        // round-off issues.  We
        // are testing to see A was
        // overwritten.
    {
        std::cerr << fname << ": overwrote 1st arg.\n";
        std::cerr << "       error norm: " << norm << std::endl;
        exit(1);
    }

#if 0
    std::cout << "A\n" << A << std::endl;
    std::cout << "old_A\n" << old_A << std::endl;
    std::cout << "x \n" << x << std::endl;
#endif

    double res = residual(A, x, b);
    if (res > 1)
    {
        std::cerr << fname << "resdiual " << res << " is to too high.\n";
        exit(1);
    }
    std::cerr << fname << ": LaLinearSolve(Symm) success.\n\n";

    // now try the in-place solver

    std::cerr << fname << ": testing LaLinearSolveIP(Symm,...) \n";
    LaLinearSolveIP(A, x, b);
    res = residual(old_A, x, b);
    if (res > 1)
    {
        std::cerr << fname << "resdiual " << res << " is to too high.\n";
        exit(1);
    }
    std::cerr << fname << ": LaLinearSolveIP(Symm) success.\n\n";
    return 0;
}


int main(int argc, char **argv)
{

    std::cout.precision(8);
    std::cout.setf(std::ios::scientific, std::ios::floatfield);

    if (argc < 2)
    {
        std::cerr << "Usage " << argv[0] << " N " << std::endl;
        exit(1);
    }

    int N = atoi(argv[1]);
    //int N = atoi(argv[2]);

#ifdef __x86_64
    // LaGenerateMatDouble doesn't work on amd64, so ignore this test
    exit(77);
#endif

    TestSymmLinearSolve(N);

}

