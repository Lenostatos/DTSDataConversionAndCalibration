//      LAPACK++ (V. 1.1)
//      (C) 1992-1996 All Rights Reserved.

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "arch.h"
#include "lapack.h"
#include "f2c.h"
#include "lapackpp.h"

#ifdef _LA_TRIDIAG_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaTridiagMatDouble &A)
{
    // the lapack dtimmg() routine assumes that the three
    // diagonals of A are stored contiguously.  This is not
    // a typical requirement of tridiagonal matrices, so
    // we first create a tmp vector hold the contiguous values
    // to be initalized, and then copy this back into A.

    integer N = A.size();
    LaVectorDouble tmp(3 * N - 2);
    integer iflag = -12, ku = 0, kl = 0;
    integer m = 3 * N - 2, n = 1;

    F77NAME(dtimmg)(&iflag, &m, &n, &tmp(0), &m, &kl, &ku);

    A.diag(-1) = tmp(LaIndex(0, N - 2));
    A.diag(0)  = tmp(LaIndex(N - 1, 2 * N - 2));
    A.diag(1)  = tmp(LaIndex(2 * N - 1, 3 * N - 3));
}
#endif

#ifdef _LA_SYMM_TRIDIAG_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaSymmTridiagMatDouble &A)
{
    // the lapack dtimmg() routine assumes that the
    // diagonals of A are stored contiguously.
    // We first create a tmp vector hold the contiguous values
    // to be initalized, and then copy this back into A.

    integer N = A.size();
    LaVectorDouble tmp(2 * N - 1);
    integer iflag = -13, ku = 0, kl = 0;
    integer m = 2 * N - 1, n = 1;

    F77NAME(dtimmg)(&iflag, &m, &n, &tmp(0), &m, &kl, &ku);

    //std::cout << tmp << std::endl;
    //std::cout << tmp(LaIndex(0,N-2)) << std::endl;

    A.diag(-1) = tmp(LaIndex(0, N - 2));
    A.diag(0)  = tmp(LaIndex(N - 1, 2 * N - 2));
}
#endif

#ifdef _LA_GEN_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaGenMatDouble &A)
{
    integer m = A.size(0), n = A.size(1), lda = A.gdim(0);
    integer iflag = 0, ku = 0, kl = 0;

    F77NAME(dtimmg)(&iflag, &m, &n, &A(0, 0), &lda, &ku, &kl);
}

#if !(defined(OS_WIN32) || LAPACK_OS_WIN32)
extern "C" double drand48(void) throw ();
#endif

LaGenMatDouble& LaRandUniform(LaGenMatDouble &A,
                              double low, double high)
{
    int M = A.size(0), N = A.size(1);
    int i, j;

    double scale = high - low;
    for (j = 0; j < N; j++)
        for (i = 0; i < M; i++)
            A(i, j) = low + scale *
#if defined(OS_WIN32) || LAPACK_OS_WIN32
                      double(rand()) / double(RAND_MAX)
#else
                      drand48()
#endif
                      ;
    return A;
}

#ifdef _LA_GEN_MAT_COMPLEX_H_
LaGenMatComplex& LaRandUniform(LaGenMatComplex &A,
                               double low, double high)
{
    int M = A.size(0), N = A.size(1);
    int i, j;

    double scale = high - low;
    for (j = 0; j < N; j++)
        for (i = 0; i < M; i++)
            A(i, j) = LaComplex(low + scale *
#if defined(OS_WIN32) || LAPACK_OS_WIN32
                                double(rand()) / double(RAND_MAX)
#else
                                drand48()
#endif
                                , low + scale *
#if defined(OS_WIN32) || LAPACK_OS_WIN32
                                double(rand()) / double(RAND_MAX)
#else
                                drand48()
#endif
                               );
    return A;
}
#endif // _LA_GEN_MAT_COMPLEX_H_

#endif // _LA_GEN_MAT_DOUBLE_H_

#ifdef _LA_UPPER_TRIANG_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaUpperTriangMatDouble &A)
{
    integer m = A.size(0), n = A.size(1), lda = A.gdim(0);
    integer iflag = 9, ku = 0, kl = 0;

    F77NAME(dtimmg)(&iflag, &m, &n, &A(0, 0), &lda, &ku, &kl);
}
#endif

#ifdef _LA_LOWER_TRIANG_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaLowerTriangMatDouble &A)
{
    integer m = A.size(0), n = A.size(1), lda = A.gdim(0);
    integer iflag = -9, ku = 0, kl = 0;

    F77NAME(dtimmg)(&iflag, &m, &n, &A(0, 0), &lda, &ku, &kl);
}
#endif


#ifdef _LA_SYMM_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaSymmMatDouble &A)
{
    integer m = A.size(0), n = A.size(1), lda = A.gdim(0);
    integer iflag = -6, ku = 0, kl = 0;

    F77NAME(dtimmg)(&iflag, &m, &n, &A(0, 0), &lda, &ku, &kl);
}
#endif

#ifdef _LA_SPD_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaSpdMatDouble &A)
{
    integer m = A.size(0), n = A.size(1), lda = A.gdim(0);
    integer iflag = -3;
    integer ku = 0;
    integer kl = 0;

    F77NAME(dtimmg)(&iflag, &m, &n, &A(0, 0), &lda, &ku, &kl);
}
#endif

#ifdef _LA_SPD_BAND_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaSpdBandMatDouble &A)
{
    integer m = A.size(0), n = A.size(1), lda = A.gdim(0);
    integer iflag = -5;
    integer ku = A.subdiags();
    integer kl = A.subdiags();

    F77NAME(dtimmg)(&iflag, &m, &n, &A(0, 0), &lda, &ku, &kl);
}
#endif


#ifdef _LA_BAND_MAT_DOUBLE_H_
void LaGenerateMatDouble(LaBandMatDouble &A)
{
    integer iflag = 2, ku = A.superdiags(), kl = A.subdiags();
    integer m = A.size(1), n = A.size(1), lda = A.gdim(0);  // changed 11/8

    F77NAME(dtimmg)(&iflag, &m, &n, &A(-kl, 0), &lda, &ku, &kl);
}
#endif



