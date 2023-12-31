/**
 *
 * @file core_dlaswp.c
 *
 *  PLASMA core_blas kernel
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated d Fri Apr  1 11:02:30 2016
 *
 **/
#include <lapacke.h>
#include "common.h"

#define A(m, n) BLKADDR(descA, double, m, n)

/***************************************************************************//**
 *
 * @ingroup CORE_double
 *
 *  CORE_dlaswp performs a series of row interchanges on the matrix A.
 *  One row interchange is initiated for each of rows I1 through I2 of A.
 *
 *******************************************************************************
 *
 *  @param[in] N
 *          The number of columns in the matrix A. N >= 0.
 *
 *  @param[in,out] A
 *         On entry, the matrix of column dimension N to which the row
 *         interchanges will be applied.
 *         On exit, the permuted matrix.
 *
 *  @param[in] LDA
 *         The leading dimension of the array A.  LDA >= max(1,max(IPIV[I1..I2])).
 *
 *  @param[in] I1
 *          The first element of IPIV for which a row interchange will
 *          be done.
 *
 *  @param[in] I2
 *          The last element of IPIV for which a row interchange will
 *          be done.
 *
 *  @param[in] IPIV
 *          The pivot indices; Only the element in position i1 to i2
 *          are accessed. The pivot are offset by A.i.
 *
 *  @param[in] INC
 *          The increment between successive values of IPIV.  If IPIV
 *          is negative, the pivots are applied in reverse order.
 *
 *******************************************************************************
 */
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_dlaswp = PCORE_dlaswp
#define CORE_dlaswp PCORE_dlaswp
#endif
void CORE_dlaswp(int N, double *A, int LDA, int I1, int I2, const int *IPIV, int INC)
{
    LAPACKE_dlaswp_work( LAPACK_COL_MAJOR, N, A, LDA, I1, I2, IPIV, INC );
}

/***************************************************************************//**
 *
 * @ingroup CORE_double
 *
 *  CORE_dlaswp_ontile apply the dlaswp function on a matrix stored in
 *  tile layout
 *
 *******************************************************************************
 *
 *  @param[in,out] descA
 *          The descriptor of the matrix A to permute.
 *
 *  @param[in] i1
 *          The first element of IPIV for which a row interchange will
 *          be done.
 *
 *  @param[in] i2
 *          The last element of IPIV for which a row interchange will
 *          be done.
 *
 *  @param[in] ipiv
 *          The pivot indices; Only the element in position i1 to i2
 *          are accessed. The pivot are offset by A.i.
 *
 *  @param[in] inc
 *          The increment between successive values of IPIV.  If IPIV
 *          is negative, the pivots are applied in reverse order.
 *
 *******************************************************************************
 *
 * @return
 *         \retval PLASMA_SUCCESS successful exit
 *         \retval <0 if INFO = -k, the k-th argument had an illegal value
 *
 *******************************************************************************
 */
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_dlaswp_ontile = PCORE_dlaswp_ontile
#define CORE_dlaswp_ontile PCORE_dlaswp_ontile
#endif
int CORE_dlaswp_ontile(PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc)
{
    int i, j, ip, it;
    double *A1;
    int lda1, lda2;

    /* Change i1 to C notation */
    i1--;

    /* Check parameters */
    if ( descA.nt > 1 ) {
        coreblas_error(1, "Illegal value of descA.nt");
        return -1;
    }
    if ( i1 < 0 ) {
        coreblas_error(2, "Illegal value of i1");
        return -2;
    }
    if ( (i2 <= i1) || (i2 > descA.m) ) {
        coreblas_error(3, "Illegal value of i2");
        return -3;
    }
    if ( ! ( (i2 - i1 - i1%descA.mb -1) < descA.mb ) ) {
        coreblas_error(2, "Illegal value of i1,i2. They have to be part of the same block.");
        return -3;
    }

    if (inc > 0) {
        it = i1 / descA.mb;
        A1 = A(it, 0);
        lda1 = BLKLDD(descA, 0);

        for (j = i1; j < i2; ++j, ipiv+=inc) {
            ip = (*ipiv) - descA.i - 1;
            if ( ip != j )
            {
                it = ip / descA.mb;
                i  = ip % descA.mb;
                lda2 = BLKLDD(descA, it);
                cblas_dswap(descA.n, A1       + j, lda1,
                                     A(it, 0) + i, lda2 );
            }
        }
    }
    else
    {
        it = (i2-1) / descA.mb;
        A1 = A(it, 0);
        lda1 = BLKLDD(descA, it);

        i1--;
        ipiv = &ipiv[(1-i2)*inc];
        for (j = i2-1; j > i1; --j, ipiv+=inc) {
            ip = (*ipiv) - descA.i - 1;
            if ( ip != j )
            {
                it = ip / descA.mb;
                i  = ip % descA.mb;
                lda2 = BLKLDD(descA, it);
                cblas_dswap(descA.n, A1       + j, lda1,
                                     A(it, 0) + i, lda2 );
            }
        }
    }

    return PLASMA_SUCCESS;
}

/***************************************************************************//**
 *
 * @ingroup CORE_double
 *
 *  CORE_dswptr_ontile apply the dlaswp function on a matrix stored in
 *  tile layout, followed by a dtrsm on the first tile of the panel.
 *
 *******************************************************************************
 *
 *  @param[in,out] descA
 *          The descriptor of the matrix A to permute.
 *
 *  @param[in] i1
 *          The first element of IPIV for which a row interchange will
 *          be done.
 *
 *  @param[in] i2
 *          The last element of IPIV for which a row interchange will
 *          be done.
 *
 *  @param[in] ipiv
 *          The pivot indices; Only the element in position i1 to i2
 *          are accessed. The pivot are offset by A.i.
 *
 *  @param[in] inc
 *          The increment between successive values of IPIV.  If IPIV
 *          is negative, the pivots are applied in reverse order.
 *
 *  @param[in] Akk
 *          The triangular matrix Akk. The leading descA.nb-by-descA.nb
 *          lower triangular part of the array Akk contains the lower triangular matrix, and the
 *          strictly upper triangular part of A is not referenced. The
 *          diagonal elements of A are also not referenced and are assumed to be 1.
 *
 * @param[in] ldak
 *          The leading dimension of the array Akk. ldak >= max(1,descA.nb).
 *
 *******************************************************************************
 *
 * @return
 *         \retval PLASMA_SUCCESS successful exit
 *         \retval <0 if INFO = -k, the k-th argument had an illegal value
 *
 *******************************************************************************
 */
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_dswptr_ontile = PCORE_dswptr_ontile
#define CORE_dswptr_ontile PCORE_dswptr_ontile
#endif
int CORE_dswptr_ontile(PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc,
                       const double *Akk, int ldak)
{
    double zone  = 1.0;
    int lda;
    int m = descA.mt == 1 ? descA.m : descA.mb;

    if ( descA.nt > 1 ) {
        coreblas_error(1, "Illegal value of descA.nt");
        return -1;
    }
    if ( i1 < 1 ) {
        coreblas_error(2, "Illegal value of i1");
        return -2;
    }
    if ( (i2 < i1) || (i2 > m) ) {
        coreblas_error(3, "Illegal value of i2");
        return -3;
    }

    CORE_dlaswp_ontile(descA, i1, i2, ipiv, inc);

    lda = BLKLDD(descA, 0);
    cblas_dtrsm( CblasColMajor, CblasLeft, CblasLower,
                 CblasNoTrans, CblasUnit,
                 m, descA.n, (zone),
                 Akk,     ldak,
                 A(0, 0), lda );

    return PLASMA_SUCCESS;
}

/***************************************************************************//**
 *
 * @ingroup CORE_double
 *
 *  CORE_dlaswpc_ontile apply the dlaswp function on a matrix stored in
 *  tile layout
 *
 *******************************************************************************
 *
 *  @param[in,out] descA
 *          The descriptor of the matrix A to permute.
 *
 *  @param[in] i1
 *          The first element of IPIV for which a column interchange will
 *          be done.
 *
 *  @param[in] i2
 *          The last element of IPIV for which a column interchange will
 *          be done.
 *
 *  @param[in] ipiv
 *          The pivot indices; Only the element in position i1 to i2
 *          are accessed. The pivot are offset by A.i.
 *
 *  @param[in] inc
 *          The increment between successive values of IPIV.  If IPIV
 *          is negative, the pivots are applied in reverse order.
 *
 *******************************************************************************
 *
 * @return
 *         \retval PLASMA_SUCCESS successful exit
 *         \retval <0 if INFO = -k, the k-th argument had an illegal value
 *
 *******************************************************************************
 */
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_dlaswpc_ontile = PCORE_dlaswpc_ontile
#define CORE_dlaswpc_ontile PCORE_dlaswpc_ontile
#endif
int CORE_dlaswpc_ontile(PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc)
{
    int i, j, ip, it;
    double *A1;
    int lda;

    /* Change i1 to C notation */
    i1--;

    /* Check parameters */
    if ( descA.mt > 1 ) {
        coreblas_error(1, "Illegal value of descA.mt");
        return -1;
    }
    if ( i1 < 0 ) {
        coreblas_error(2, "Illegal value of i1");
        return -2;
    }
    if ( (i2 <= i1) || (i2 > descA.n) ) {
        coreblas_error(3, "Illegal value of i2");
        return -3;
    }
    if ( ! ( (i2 - i1 - i1%descA.nb -1) < descA.nb ) ) {
        coreblas_error(2, "Illegal value of i1,i2. They have to be part of the same block.");
        return -3;
    }

    lda = BLKLDD(descA, 0);

    if (inc > 0) {
        it = i1 / descA.nb;
        A1 = A(0, it);

        for (j = i1-1; j < i2; ++j, ipiv+=inc) {
            ip = (*ipiv) - descA.j - 1;
            if ( ip != j )
            {
                it = ip / descA.nb;
                i  = ip % descA.nb;
                cblas_dswap(descA.m, A1       + j*lda, 1,
                                     A(0, it) + i*lda, 1 );
            }
        }
    }
    else
    {
        it = (i2-1) / descA.mb;
        A1 = A(0, it);

        i1--;
        ipiv = &ipiv[(1-i2)*inc];
        for (j = i2-1; j > i1; --j, ipiv+=inc) {
            ip = (*ipiv) - descA.j - 1;
            if ( ip != j )
            {
                it = ip / descA.nb;
                i  = ip % descA.nb;
                cblas_dswap(descA.m, A1       + j*lda, 1,
                                     A(0, it) + i*lda, 1 );
            }
        }
    }

    return PLASMA_SUCCESS;
}
