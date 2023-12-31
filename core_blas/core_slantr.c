/**
 *
 * @file core_slantr.c
 *
 *  PLASMA core_blas kernel
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated s Fri Apr  1 11:02:29 2016
 *
 **/
#include <lapacke.h>
#include <math.h>
#include "common.h"

#define LAPACKE_CORRECT_DLANTR

/***************************************************************************//**
 *
 * @ingroup CORE_float
 *
 *  CORE_slantr returns the value
 *
 *     slantr = ( max(abs(A(i,j))), NORM = PlasmaMaxNorm
 *              (
 *              ( norm1(A),         NORM = PlasmaOneNorm
 *              (
 *              ( normI(A),         NORM = PlasmaInfNorm
 *              (
 *              ( normF(A),         NORM = PlasmaFrobeniusNorm
 *
 *  where norm1 denotes the one norm of a matrix (maximum column sum),
 *  normI denotes the infinity norm of a matrix (maximum row sum) and
 *  normF denotes the Frobenius norm of a matrix (square root of sum
 *  of squares). Note that max(abs(A(i,j))) is not a consistent matrix
 *  norm.
 *
 *******************************************************************************
 *
 * @param[in] norm
 *          = PlasmaMaxNorm: Max norm
 *          = PlasmaOneNorm: One norm
 *          = PlasmaInfNorm: Infinity norm
 *          = PlasmaFrobeniusNorm: Frobenius norm
 *
 * @param[in] uplo
 *          Specifies whether the matrix A is upper triangular or lower triangular:
 *          = PlasmaUpper: Upper triangle of A is stored;
 *          = PlasmaLower: Lower triangle of A is stored.
 *
 * @param[in] diag
 *          Specifies whether or not A is unit triangular:
 *          = PlasmaNonUnit: A is non unit;
 *          = PlasmaUnit:    A us unit.
 *
 * @param[in] M
 *          The number of rows of the matrix A. M >= 0.
 *          If uplo == PlasmaUpper, M <= N. When M = 0, CORE_slantr returns 0.
 *
 * @param[in] N
 *          The number of columns of the matrix A. N >= 0.
 *          If uplo == PlasmaLower, N <= M. When N = 0, CORE_slantr returns 0.
 *
 * @param[in] A
 *          The LDA-by-N matrix A.
 *
 * @param[in] LDA
 *          The leading dimension of the array A. LDA >= max(1,M).
 *
 * @param[in,out] work
 *          Array of dimension (MAX(1,LWORK)), where LWORK >= M when norm =
 *          PlasmaInfNorm, or LWORK >= N when norm = PlasmaOneNorm; otherwise,
 *          work is not referenced.
 *
 * @param[out] normA
 *          On exit, normA is the norm of matrix A.
 *
 ******************************************************************************/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_slantr = PCORE_slantr
#define CORE_slantr PCORE_slantr
#endif
void CORE_slantr(PLASMA_enum norm, PLASMA_enum uplo, PLASMA_enum diag,
                 int M, int N,
                 const float *A, int LDA,
                 float *work, float *normA)
{
#if defined(LAPACKE_CORRECT_DLANTR)
    *normA = LAPACKE_slantr_work(
        LAPACK_COL_MAJOR,
        lapack_const(norm),
        lapack_const(uplo),
        lapack_const(diag),
        M, N, A, LDA, work);
#else
    const float *tmpA;
    float value;
    int i, j, imax;
    int idiag = (diag == PlasmaUnit) ? 1 : 0;

    if ( min(M, N) == 0 ) {
        *normA = 0;
        return;
    }

    switch ( norm ) {
    case PlasmaMaxNorm:
        if ( diag == PlasmaUnit ) {
            *normA = 1.;
        } else {
            *normA = 0.;
        }

        if ( uplo == PlasmaUpper ) {
            M = min(M, N);
            for (j = 0; j < N; j++) {
                tmpA = A+(j*LDA);
                imax = min(j+1-idiag, M);

                for (i = 0; i < imax; i++) {
                    value = fabsf( *tmpA );
                    *normA = ( value > *normA ) ? value : *normA;
                    tmpA++;
                }
            }
        } else {
            N = min(M, N);
            for (j = 0; j < N; j++) {
                tmpA = A + j * (LDA+1) + idiag;

                for (i = j+idiag; i < M; i++) {
                    value = fabsf( *tmpA );
                    *normA = ( value > *normA ) ? value : *normA;
                    tmpA++;
                }
            }
        }
        break;

    case PlasmaOneNorm:
        CORE_strasm( PlasmaColumnwise, uplo, diag, M, N,
                     A, LDA, work );
        if ( uplo == PlasmaLower )
            N = min(M,N);

        *normA = 0;
        for (i = 0; i < N; i++) {
            *normA = ( work[i] > *normA ) ? work[i] : *normA;
        }
        break;

    case PlasmaInfNorm:
        CORE_strasm( PlasmaRowwise, uplo, diag, M, N,
                     A, LDA, work );
        if ( uplo == PlasmaUpper )
            M = min(M,N);

        *normA = 0;
        for (i = 0; i < M; i++) {
            *normA = ( work[i] > *normA ) ? work[i] : *normA;
        }
        break;

    case PlasmaFrobeniusNorm:
    {
        float scale = 0.;
        float sumsq = 1.;
        CORE_strssq( uplo, diag, M, N,
                     A, LDA, &scale, &sumsq );

        *normA = scale * sqrt( sumsq );
    }
    break;
    default:
        coreblas_error(1, "Illegal value of norm");
        return;
    }
#endif
}
