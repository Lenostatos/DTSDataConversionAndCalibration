/*      LAPACK++ (V. 1.1)                                       */
/*      (C) 1992-1996 All Rights Reserved.                          */

#ifndef _BLAS3_H
#define _BLAS3_H

#include "arch.h"
#include "f2c.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    void F77NAME(dgemm)(char *transa, char *transb, integer *m, integer *n, integer *k,
                        double *alpha, const double *a, integer *lda, const double *b,
                        integer *ldb, double *beta, double *c, integer *ldc);

    void F77NAME(dtrsm)(char *side, char *uplo, char *transa, char *diag,
                        integer *m, integer *n, double *alpha, const double *A, integer *lda,
                        const double *B, integer *ldb);

    void F77NAME(dtrmm)(char *side, char *uplo, char *transa, char *diag,
                        integer *m, integer *n, double *alpha, const double *A, integer *lda,
                        const double *B, integer *ldb);

    void F77NAME(dsymm)(char *side, char *uplo, integer *m, integer *n,
                        double *alpha, const double *A, integer *lda, const double *B,
                        integer *ldb, double *beta, double *C, integer *ldc);

    void F77NAME(dsyrk)(char *uplo, char *transa, integer *n, integer *k,
                        double *alpha, double *A, integer *lda, double *beta, double *C,
                        integer *ldc);

    void F77NAME(dsyr2k)(char *uplo, char *transa, integer *n, integer *k,
                         double *alpha, double *A, integer *lda, double *B, integer *ldb,
                         double *beta, double *C, integer *ldc);

#ifdef LA_COMPLEX_SUPPORT
    void F77NAME(zgemm)(char *transa, char *transb, integer *m, integer *n, integer *k,
                        doublecomplex *alpha, const doublecomplex *a, integer *lda, const doublecomplex *b,
                        integer *ldb, doublecomplex *beta, doublecomplex *c, integer *ldc);
#endif /* LA_COMPLEX_SUPPORT */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _BLAS3_H */
