/**
 *
 * @file core_cgetrf_rectil.c
 *
 *  PLASMA core_blas kernel
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @author Piotr Luszczek
 * @date 2009-11-15
 *
 * @generated c Fri Apr  1 11:02:30 2016
 *
 **/

#include <math.h>
#include <cblas.h>
#include <lapacke.h>
#include "common.h"

#define A(m, n) BLKADDR(A, PLASMA_Complex32_t, m, n)

struct CORE_cgetrf_data_s {
    volatile PLASMA_Complex32_t *CORE_camax;
    volatile int                *CORE_cstep;
};

static inline void
CORE_cgetrf_rectil_rec(CORE_cgetrf_data_t *data,
                       const PLASMA_desc A, int *IPIV, int *info,
                       PLASMA_Complex32_t *pivot,
                       int thidx,  int thcnt,
                       int column, int width,
                       int ft,     int lt);
static inline void
CORE_cgetrf_rectil_update(CORE_cgetrf_data_t *data,
                          const PLASMA_desc A, int *IPIV,
                          int column, int n1,     int n2,
                          int thidx,  int thcnt,
                          int ft,     int lt);

/***************************************************************************//**
 *
 * @ingroup CORE_PLASMA_Complex32_t
 *
 *  CORE_cgetrf_rectil computes a LU factorization of a general M-by-N
 *  matrix A stored in CCRB layout using partial pivoting with row
 *  interchanges.
 *
 *  The factorization has the form
 *
 *    A = P * L * U
 *
 *  where P is a permutation matrix, L is lower triangular with unit
 *  diagonal elements (lower trapezoidal if m > n), and U is upper
 *  triangular (upper trapezoidal if m < n).
 *
 *  This is the recursive version of the algorithm applied on tile layout.
 *
 *  WARNINGS:
 *     - The function CORE_cgetrf_rectil_init has to be called prior
 *     to any call to this function.
 *     - You cannot call this kernel on different matrices at the same
 *     time.
 *     - The matrix A cannot be more than one tile wide.
 *     - The number of threads calling this function has to be excatly
 *     the number defined by info[2] with each one of them a different
 *     index between 0 included and info[2] excluded.
 *
 *******************************************************************************
 *
 *  @param[in] data
 *          Common data structure to all threads initialized by
 *          CORE_cgetrf_rectil_init() that contains information for thread
 *          synchronization and maximum reductions. All threads working on a
 *          given matrix A must provide the same data structure.
 *
 *  @param[in,out] A
 *          PLASMA descriptor of the matrix A to be factorized.
 *          On entry, the M-by-N matrix to be factorized.
 *          On exit, the factors L and U from the factorization
 *          A = P*L*U; the unit diagonal elements of L are not stored.
 *
 *  @param[out] IPIV
 *          The pivot indices; for 0 <= i < min(M,N) stored in Fortran
 *          mode (starting at 1), row i of the matrix was interchanged
 *          with row IPIV(i).
 *          On exit, each value IPIV[i] for 0 <= i < min(M,N) is
 *          increased by A.i, which means A.i < IPIV[i] <= A.i+M.
 *
 *  @param[in,out] info
 *          Array of 3 integers.
 *          - info[0], see returned value
 *          - info[1], is the thread index 0 <= info[0] < info[2]
 *          - info[2], on entry is the number of threads trying to
 *                     participate to the factorization,
 *                     on exit is the real number of threads used to
 *                     perform the factorization.
 *    Info[2] threads, and exactly info[2], have to call this function
 *    to avoid dead lock.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *          \retval -k, the k-th argument had an illegal value
 *          \retval k if U(k,k) is exactly zero. The factorization
 *                  has been completed, but the factor U is exactly
 *                  singular, and division by zero will occur if it is used
 *                  to solve a system of equations.
 *
 */
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_cgetrf_rectil = PCORE_cgetrf_rectil
#define CORE_cgetrf_rectil PCORE_cgetrf_rectil
#endif
int CORE_cgetrf_rectil(CORE_cgetrf_data_t *data,
                       const PLASMA_desc A, int *IPIV, int *info)
{
    int ft, lt;
    int thidx = info[1];
    int thcnt = min( info[2], A.mt );
    int minMN = min( A.m, A.n );
    PLASMA_Complex32_t pivot;

    info[0] = 0;
    info[2] = thcnt;

    if ( A.nt > 1 ) {
        coreblas_error(1, "Illegal value of A.nt");
        info[0] = -1;
        return -1;
    }

    if ( thidx >= thcnt )
      return 0;

    int q = A.mt / thcnt;
    int r = A.mt % thcnt;

    if (thidx < r) {
        q++;
        ft = thidx * q;
        lt = ft + q;
    } else {
        ft = r * (q + 1) + (thidx - r) * q;
        lt = ft + q;
        lt = min( lt, A.mt );
    }

    CORE_cgetrf_rectil_rec( data, A, IPIV, info, &pivot,
                            thidx, thcnt, 0, minMN, ft, lt);

    if ( A.n > minMN ) {
        CORE_cgetrf_rectil_update( data, A, IPIV,
                                   0, minMN, A.n-minMN,
                                   thidx, thcnt,
                                   ft, lt);
    }

    return info[0];
}

/*******************************************************************
 *   Additional routines
 */
static float sfmin = 0.;

CORE_cgetrf_data_t *
CORE_cgetrf_rectil_init(int nbthrd)
{
    int i;
    CORE_cgetrf_data_t *data;

    data = (CORE_cgetrf_data_t*)malloc( nbthrd * (sizeof(PLASMA_Complex32_t)+sizeof(int))
                                        + 2 * sizeof(void*) );

    data->CORE_camax = (PLASMA_Complex32_t*)((char*)data + 2 * sizeof(void*));
    data->CORE_cstep = (int*)((char*)data + 2 * sizeof(void*) + nbthrd * sizeof(PLASMA_Complex32_t));

    for (i = 0; i < nbthrd; ++i) {
        data->CORE_camax[i] = 0.;
        data->CORE_cstep[i] = -1;
    }
    if ( sfmin == 0. ) {
        sfmin = LAPACKE_slamch_work('S');
    }
    return data;
}

static inline void
CORE_camax1_thread(CORE_cgetrf_data_t *data,
                   PLASMA_Complex32_t localamx,
                   int thidx, int thcnt, int *thwinner,
                   PLASMA_Complex32_t *diagvalue,
                   PLASMA_Complex32_t *globalamx,
                   int pividx, int *ipiv)
{
    volatile PLASMA_Complex32_t *CORE_camax = data->CORE_camax;
    volatile int                *CORE_cstep = data->CORE_cstep;

    if (thidx == 0) {
        int i, j = 0;
        PLASMA_Complex32_t curval = localamx, tmp;
        float curamx = cabsf(localamx);

        /* make sure everybody filled in their value */
        for (i = 1; i < thcnt; ++i) {
            while (CORE_cstep[i] == -1) { /* wait for thread i to store its value */
            }
        }

        /* better not fuse the loop above and below to make sure data is sync'd */

        for (i = 1; i < thcnt; ++i) {
            tmp = CORE_camax[i];
            if (cabsf(tmp) > curamx) {
                curamx = cabsf(tmp);
                curval = tmp;
                j = i;
            }
        }

        if (0 == j)
            ipiv[0] = pividx;

        /* make sure everybody knows the amax value */
        for (i = 1; i < thcnt; ++i)
            CORE_camax[ i ] = curval;

        CORE_cstep[0] = -j - 2; /* set the index of the winning thread */
        CORE_camax[0] = *diagvalue; /* set the index of the winning thread */

        *thwinner = j;
        *globalamx = curval;

        for (i = 1; i < thcnt; ++i)
            CORE_cstep[i] = -3;

        /* make sure everybody read the max value */
        for (i = 1; i < thcnt; ++i) {
            while (CORE_cstep[i] != -1) {
            }
        }

        CORE_cstep[0] = -1;
    } else {
        CORE_camax[thidx] = localamx;
        CORE_cstep[thidx] = -2;  /* announce to thread 0 that local amax was stored */
        while (CORE_cstep[0] == -1) { /* wait for thread 0 to finish calculating the global amax */
        }
        while (CORE_cstep[thidx] != -3) { /* wait for thread 0 to store amax */
        }
        *thwinner  = -CORE_cstep[0] - 2;
        *diagvalue = CORE_camax[0];
        *globalamx = CORE_camax[thidx];     /* read the amax from the location adjacent to the one in the above loop */
        CORE_cstep[thidx] = -1;  /* signal thread 0 that this thread is done reading */

        if (thidx == *thwinner)
            ipiv[0] = pividx;

        while (CORE_cstep[0] != -1) { /* wait for thread 0 to finish */
        }
    }
}

static inline void
CORE_cbarrier_thread(CORE_cgetrf_data_t *data,
                     int thidx, int thcnt)
{
    int idum1, idum2;
    PLASMA_Complex32_t ddum1 = 0.;
    PLASMA_Complex32_t ddum2 = 0.;
    /* it's probably faster to implement a dedicated barrier */
    CORE_camax1_thread( data, 1.0, thidx, thcnt, &idum1, &ddum1, &ddum2, 0, &idum2 );
}

static inline void
CORE_cgetrf_rectil_update(CORE_cgetrf_data_t *data,
                          const PLASMA_desc A, int *IPIV,
                          int column, int n1,     int n2,
                          int thidx,  int thcnt,
                          int ft,     int lt)
{
    int ld, lm, tmpM;
    int ip, j, it, i, ldft;
    PLASMA_Complex32_t zone  = 1.0;
    PLASMA_Complex32_t mzone = -1.0;
    PLASMA_Complex32_t *Atop, *Atop2, *U, *L;
    int offset = A.i;

    ldft = BLKLDD(A, 0);
    Atop  = A(0, 0) + column * ldft;
    Atop2 = Atop    + n1     * ldft;

    if (thidx == 0)
    {
        /* Swap to the right */
        int *lipiv = IPIV+column;
        int idxMax = column+n1;
        for (j = column; j < idxMax; ++j, ++lipiv) {
            ip = (*lipiv) - offset - 1;
            if ( ip != j )
            {
                it = ip / A.mb;
                i  = ip % A.mb;
                ld = BLKLDD(A, it);
                cblas_cswap(n2, Atop2                     + j, ldft,
                            A(it, 0) + (column+n1)*ld + i, ld   );
            }
        }

        /* Trsm on the upper part */
        U = Atop2 + column;
        cblas_ctrsm( CblasColMajor, CblasLeft, CblasLower,
                     CblasNoTrans, CblasUnit,
                     n1, n2, CBLAS_SADDR(zone),
                     Atop  + column, ldft,
                     U,              ldft );

        /* Signal to other threads that they can start update */
        CORE_cbarrier_thread( data, thidx, thcnt );

        /* First tile */
        L = Atop + column + n1;
        tmpM = min(ldft, A.m) - column - n1;

        /* Apply the GEMM */
        cblas_cgemm( CblasColMajor, CblasNoTrans, CblasNoTrans,
                     tmpM, n2, n1,
                     CBLAS_SADDR(mzone), L,      ldft,
                     U,      ldft,
                     CBLAS_SADDR(zone),  U + n1, ldft );

    }
    else
    {
        ld = BLKLDD( A, ft );
        L  = A( ft, 0 ) + column * ld;
        lm = ft == A.mt-1 ? A.m - ft * A.mb : A.mb;

        U = Atop2 + column;

        /* Wait for pivoting and triangular solve to be finished
         * before to really start the update */
        CORE_cbarrier_thread( data, thidx, thcnt );

        /* First tile */
        /* Apply the GEMM */
        cblas_cgemm( CblasColMajor, CblasNoTrans, CblasNoTrans,
                     lm, n2, n1,
                     CBLAS_SADDR(mzone), L,         ld,
                                         U,         ldft,
                     CBLAS_SADDR(zone),  L + n1*ld, ld );
    }

    /* Update the other blocks */
    for( it = ft+1; it < lt; it++)
    {
        ld = BLKLDD( A, it );
        L  = A( it, 0 ) + column * ld;
        lm = it == A.mt-1 ? A.m - it * A.mb : A.mb;

        /* Apply the GEMM */
        cblas_cgemm( CblasColMajor, CblasNoTrans, CblasNoTrans,
                     lm, n2, n1,
                     CBLAS_SADDR(mzone), L,          ld,
                                         U,          ldft,
                     CBLAS_SADDR(zone),  L + n1*ld,  ld );
    }
}

static void
CORE_cgetrf_rectil_rec(CORE_cgetrf_data_t *data,
                       const PLASMA_desc A, int *IPIV, int *info,
                       PLASMA_Complex32_t *pivot,
                       int thidx,  int thcnt,
                       int column, int width,
                       int ft,     int lt)
{
    int ld, jp, n1, n2, lm, tmpM, piv_sf;
    int ip, j, it, i, ldft;
    int max_i, max_it, thwin;
    PLASMA_Complex32_t zone  = 1.0;
    PLASMA_Complex32_t mzone = -1.0;
    PLASMA_Complex32_t tmp1;
    PLASMA_Complex32_t tmp2 = 0.;
    PLASMA_Complex32_t pivval;
    PLASMA_Complex32_t *Atop, *Atop2, *U, *L;
    float             abstmp1;
    int offset = A.i;

    ldft = BLKLDD(A, 0);
    Atop = A(0, 0) + column * ldft;

    if ( width > 1 ) {
        /* Assumption: N = min( M, N ); */
        n1 = width / 2;
        n2 = width - n1;

        Atop2 = Atop + n1 * ldft;

        CORE_cgetrf_rectil_rec( data, A, IPIV, info, pivot,
                                thidx, thcnt, column, n1, ft, lt );

        if ( *info != 0 )
            return;

        /*
         * Check if need to return due to 0 on diagonal,
         * And compute the best pivval to choose between multiply and divide
         */
        pivval = *pivot;
        if ( pivval == 0.0 ) {
            *info = column+n1;
            return;
        } else {
            if ( cabsf(pivval) >= sfmin ) {
                piv_sf = 1;
                pivval = 1.0 / pivval;
            } else {
                piv_sf = 0;
            }
        }

        if (thidx == 0)
        {
            /* Swap to the right */
            int *lipiv = IPIV+column;
            int idxMax = column+n1;
            for (j = column; j < idxMax; ++j, ++lipiv) {
                ip = (*lipiv) - offset - 1;
                if ( ip != j )
                {
                    it = ip / A.mb;
                    i  = ip % A.mb;
                    ld = BLKLDD(A, it);
                    cblas_cswap(n2, Atop2                     + j, ldft,
                                    A(it, 0) + (column+n1)*ld + i, ld   );
                }
            }
            /* Trsm on the upper part */
            U = Atop2 + column;
            cblas_ctrsm( CblasColMajor, CblasLeft, CblasLower,
                         CblasNoTrans, CblasUnit,
                         n1, n2, CBLAS_SADDR(zone),
                         Atop  + column, ldft,
                         U,              ldft );

            /* SIgnal to other threads that they can start update */
            CORE_cbarrier_thread( data, thidx, thcnt );

            /* First tile */
            {
                L = Atop + column + n1;
                tmpM = min(ldft, A.m) - column - n1;

                /* Scale last column of L */
                if ( piv_sf ) {
                    cblas_cscal( tmpM, CBLAS_SADDR(pivval), L+(n1-1)*ldft, 1 );
                } else {
                    int i;
                    Atop2 = L+(n1-1)*ldft;
                    for( i=0; i < tmpM; i++, Atop2++)
                        *Atop2 = *Atop2 / pivval;
                }

                /* Apply the GEMM */
                cblas_cgemm( CblasColMajor, CblasNoTrans, CblasNoTrans,
                             tmpM, n2, n1,
                             CBLAS_SADDR(mzone), L,      ldft,
                                                 U,      ldft,
                             CBLAS_SADDR(zone),  U + n1, ldft );

                /* Search Max in first column of U+n1 */
                tmp2    = U[n1];
                max_it  = ft;
                max_i   = cblas_icamax( tmpM, U+n1, 1 ) + n1;
                tmp1    = U[max_i];
                abstmp1 = cabsf(tmp1);
                max_i   += column;
            }
        }
        else
        {
            ld = BLKLDD( A, ft );
            L  = A( ft, 0 ) + column * ld;
            lm = ft == A.mt-1 ? A.m - ft * A.mb : A.mb;

            U = Atop2 + column;

            /* First tile */
            /* Scale last column of L */
            if ( piv_sf ) {
                cblas_cscal( lm, CBLAS_SADDR(pivval), L+(n1-1)*ld, 1 );
            } else {
                int i;
                Atop2 = L+(n1-1)*ld;
                for( i=0; i < lm; i++, Atop2++)
                    *Atop2 = *Atop2 / pivval;
            }

            /* Wait for pivoting and triangular solve to be finished
             * before to really start the update */
            CORE_cbarrier_thread( data, thidx, thcnt );

            /* Apply the GEMM */
            cblas_cgemm( CblasColMajor, CblasNoTrans, CblasNoTrans,
                         lm, n2, n1,
                         CBLAS_SADDR(mzone), L,         ld,
                                             U,         ldft,
                         CBLAS_SADDR(zone),  L + n1*ld, ld );

            /* Search Max in first column of L+n1*ld */
            max_it  = ft;
            max_i   = cblas_icamax( lm, L+n1*ld, 1 );
            tmp1    = L[n1*ld+max_i];
            abstmp1 = cabsf(tmp1);
        }

        /* Update the other blocks */
        for( it = ft+1; it < lt; it++)
        {
            ld = BLKLDD( A, it );
            L  = A( it, 0 ) + column * ld;
            lm = it == A.mt-1 ? A.m - it * A.mb : A.mb;

            /* Scale last column of L */
            if ( piv_sf ) {
                cblas_cscal( lm, CBLAS_SADDR(pivval), L+(n1-1)*ld, 1 );
            } else {
                int i;
                Atop2 = L+(n1-1)*ld;
                for( i=0; i < lm; i++, Atop2++)
                    *Atop2 = *Atop2 / pivval;
            }

            /* Apply the GEMM */
            cblas_cgemm( CblasColMajor, CblasNoTrans, CblasNoTrans,
                         lm, n2, n1,
                         CBLAS_SADDR(mzone), L,          ld,
                                             U,          ldft,
                         CBLAS_SADDR(zone),  L + n1*ld,  ld );

            /* Search the max on the first column of L+n1*ld */
            jp = cblas_icamax( lm, L+n1*ld, 1 );
            if ( cabsf( L[n1*ld+jp] ) > abstmp1 ) {
                tmp1 = L[n1*ld+jp];
                abstmp1 = cabsf(tmp1);
                max_i  = jp;
                max_it = it;
            }
        }

        jp = offset + max_it*A.mb + max_i;
        CORE_camax1_thread( data, tmp1, thidx, thcnt, &thwin,
                            &tmp2, pivot, jp + 1, IPIV + column + n1 );

        if ( thidx == 0 ) {
            U[n1] = *pivot; /* all threads have the pivot element: no need for synchronization */
        }
        if (thwin == thidx) { /* the thread that owns the best pivot */
            if ( jp-offset != column+n1 ) /* if there is a need to exchange the pivot */
            {
                ld = BLKLDD(A, max_it);
                Atop2 = A( max_it, 0 ) + (column + n1 )* ld + max_i;
                *Atop2 = tmp2;
            }
        }

        CORE_cgetrf_rectil_rec( data, A, IPIV, info, pivot,
                                thidx, thcnt, column+n1, n2, ft, lt );
        if ( *info != 0 )
            return;

        if ( thidx == 0 )
        {
            /* Swap to the left */
            int *lipiv = IPIV+column+n1;
            int idxMax = column+width;
            for (j = column+n1; j < idxMax; ++j, ++lipiv) {
                ip = (*lipiv) - offset - 1;
                if ( ip != j )
                {
                    it = ip / A.mb;
                    i  = ip % A.mb;
                    ld = BLKLDD(A, it);
                    cblas_cswap(n1, Atop + j,                 ldft,
                                    A(it, 0) + column*ld + i, ld  );
                }
            }
        }

    } else if ( width == 1 ) {

        /* Search maximum for column 0 */
        if ( column == 0 )
        {
            if ( thidx == 0 )
              tmp2 = Atop[column];

            /* First tmp1 */
            ld = BLKLDD(A, ft);
            Atop2   = A( ft, 0 );
            lm      = ft == A.mt-1 ? A.m - ft * A.mb : A.mb;
            max_it  = ft;
            max_i   = cblas_icamax( lm, Atop2, 1 );
            tmp1    = Atop2[max_i];
            abstmp1 = cabsf(tmp1);

            /* Update */
            for( it = ft+1; it < lt; it++)
            {
                Atop2= A( it, 0 );
                lm   = it == A.mt-1 ? A.m - it * A.mb : A.mb;
                jp   = cblas_icamax( lm, Atop2, 1 );
                if (  cabsf(Atop2[jp]) > abstmp1 ) {
                    tmp1 = Atop2[jp];
                    abstmp1 = cabsf(tmp1);
                    max_i  = jp;
                    max_it = it;
                }
            }

            jp = offset + max_it*A.mb + max_i;
            CORE_camax1_thread( data, tmp1, thidx, thcnt, &thwin,
                                &tmp2, pivot, jp + 1, IPIV + column );

            if ( thidx == 0 ) {
                Atop[0] = *pivot; /* all threads have the pivot element: no need for synchronization */
            }
            if (thwin == thidx) { /* the thread that owns the best pivot */
                if ( jp-offset != 0 ) /* if there is a need to exchange the pivot */
                {
                    Atop2 = A( max_it, 0 ) + max_i;
                    *Atop2 = tmp2;
                }
            }
        }

        CORE_cbarrier_thread( data, thidx, thcnt );

        /* If it is the last column, we just scale */
        if ( column == (min(A.m, A.n))-1 ) {

            pivval = *pivot;
            if ( pivval != 0.0 ) {
                if ( thidx == 0 ) {
                    if ( cabsf(pivval) >= sfmin ) {
                        pivval = 1.0 / pivval;

                        /*
                         * We guess than we never enter the function with m == A.mt-1
                         * because it means that there is only one thread
                         */
                        lm = ft == A.mt-1 ? A.m - ft * A.mb : A.mb;
                        cblas_cscal( lm - column - 1, CBLAS_SADDR(pivval), Atop+column+1, 1 );

                        for( it = ft+1; it < lt; it++)
                        {
                            ld = BLKLDD(A, it);
                            Atop2 = A( it, 0 ) + column * ld;
                            lm = it == A.mt-1 ? A.m - it * A.mb : A.mb;
                            cblas_cscal( lm, CBLAS_SADDR(pivval), Atop2, 1 );
                        }
                    } else {
                        /*
                         * We guess than we never enter the function with m == A.mt-1
                         * because it means that there is only one thread
                         */
                        int i;
                        Atop2 = Atop + column + 1;
                        lm = ft == A.mt-1 ? A.m - ft * A.mb : A.mb;

                        for( i=0; i < lm-column-1; i++, Atop2++)
                            *Atop2 = *Atop2 / pivval;

                        for( it = ft+1; it < lt; it++)
                            {
                                ld = BLKLDD(A, it);
                                Atop2 = A( it, 0 ) + column * ld;
                                lm = it == A.mt-1 ? A.m - it * A.mb : A.mb;

                                for( i=0; i < lm; i++, Atop2++)
                                    *Atop2 = *Atop2 / pivval;
                            }
                    }
                }
                else
                {
                    if ( cabsf(pivval) >= sfmin ) {
                        pivval = 1.0 / pivval;

                        for( it = ft; it < lt; it++)
                        {
                            ld = BLKLDD(A, it);
                            Atop2 = A( it, 0 ) + column * ld;
                            lm = it == A.mt-1 ? A.m - it * A.mb : A.mb;
                            cblas_cscal( lm, CBLAS_SADDR(pivval), Atop2, 1 );
                        }
                    } else {
                        /*
                         * We guess than we never enter the function with m == A.mt-1
                         * because it means that there is only one thread
                         */
                        int i;
                        for( it = ft; it < lt; it++)
                        {
                            ld = BLKLDD(A, it);
                            Atop2 = A( it, 0 ) + column * ld;
                            lm = it == A.mt-1 ? A.m - it * A.mb : A.mb;

                            for( i=0; i < lm; i++, Atop2++)
                                *Atop2 = *Atop2 / pivval;
                        }
                    }
                }
            }
            else {
                *info = column + 1;
                return;
            }
        }
    }
}
