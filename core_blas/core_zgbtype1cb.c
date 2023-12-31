/**
 *
 * @file core_zgbtype1cb.c
 *
 *  PLASMA core_blas kernel
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Azzam Haidar
 * @date 2012-12-15
 * @precisions normal z -> c d s
 *
 **/
#include <lapacke.h>
#include "common.h"

//#define AU(m,n) &(A[(m) + LDA*(n)])
//#define AL(m,n) &(A[(m) + LDA*(n)])
#define AL(m_, n_) (A + NB + LDA * (n_) + ((m_)-(n_)))
#define AU(m_, n_) (A + NB + LDA * (n_) + ((m_)-(n_)+NB))
#define VQ(m)     (VQ + (m))
#define VP(m)     (VP + (m))
#define TAUQ(m)   (TAUQ + (m))
#define TAUP(m)   (TAUP + (m))

/**
 *****************************************************************************
 *
 * @ingroup CORE_PLASMA_Complex64_t
 *
 *  CORE_zgbtype1cb is a kernel that will operate on a region (triangle) of data
 *  bounded by st and ed. This kernel eliminate a column by an column-wise
 *  annihiliation, then it apply a left+right update on the hermitian triangle.
 *  Note that the column to be eliminated is located at st-1.
 *
 *  All detail are available on technical report or SC11 paper.
 *  Azzam Haidar, Hatem Ltaief, and Jack Dongarra. 2011.
 *  Parallel reduction to condensed forms for symmetric eigenvalue problems
 *  using aggregated fine-grained and memory-aware kernels. In Proceedings
 *  of 2011 International Conference for High Performance Computing,
 *  Networking, Storage and Analysis (SC '11). ACM, New York, NY, USA, ,
 *  Article 8 , 11 pages.
 *  http://doi.acm.org/10.1145/2063384.2063394
 *
 *******************************************************************************
 *
 * @param[in] uplo
 *          = PlasmaUpper: Upper triangle of A is stored;
 *          = PlasmaLower: Lower triangle of A is stored.
 *
 * @param[in] N
 *          The order of the matrix A.
 *
 * @param[in] NB
 *          The size of the band.
 *
 * @param[in, out] A
 *          A pointer to the matrix A of size (3*NB+1)-by-N.
 *
 * @param[in] LDA
 *          The leading dimension of the matrix A. LDA >= max(1,3*NB+1)
 *
 * @param[out] VP
 *          TODO: Check and fix doc
 *          PLASMA_Complex64_t array, dimension N if eigenvalue only
 *          requested or (LDV*blkcnt*Vblksiz) if Eigenvectors requested
 *          The Householder reflectors are stored in this array.
 *
 * @param[out] TAUP
 *          TODO: Check and fix doc
 *          PLASMA_Complex64_t array, dimension (N).
 *          The scalar factors of the Householder reflectors are stored
 *          in this array.
 *
 * @param[out] VQ
 *          TODO: Check and fix doc
 *          PLASMA_Complex64_t array, dimension N if eigenvalue only
 *          requested or (LDV*blkcnt*Vblksiz) if Eigenvectors requested
 *          The Householder reflectors are stored in this array.
 *
 * @param[out] TAUQ
 *          TODO: Check and fix doc
 *          PLASMA_Complex64_t array, dimension (N).
 *          The scalar factors of the Householder reflectors are stored
 *          in this array.
 *
 * @param[in] st
 *          A pointer to the start index where this kernel will operate.
 *
 * @param[in] ed
 *          A pointer to the end index where this kernel will operate.
 *
 * @param[in] sweep
 *          The sweep number that is eliminated. it serve to calculate the
 *          pointer to the position where to store the Vs and Ts.
 *
 * @param[in] Vblksiz
 *          constant which correspond to the blocking used when applying the Vs.
 *          it serve to calculate the pointer to the position where to store the
 *          Vs and Ts.
 *
 * @param[in] WANTZ
 *          constant which indicate if Eigenvalue are requested or both
 *          Eigenvalue/Eigenvectors.
 *
 * @param[in] WORK
 *          Workspace of size nb.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *          \retval <0 if -i, the i-th argument had an illegal value
 *
 ******************************************************************************/
/***************************************************************************
 *          TYPE 1-BAND-bidiag Lower/Upper columnwise-Householder
 ***************************************************************************/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_zgbtype1cb = PCORE_zgbtype1cb
#define CORE_zgbtype1cb PCORE_zgbtype1cb
#endif
void CORE_zgbtype1cb(PLASMA_enum uplo, int N, int NB,
                     PLASMA_Complex64_t *A, int LDA,
                     PLASMA_Complex64_t *VQ, PLASMA_Complex64_t *TAUQ,
                     PLASMA_Complex64_t *VP, PLASMA_Complex64_t *TAUP,
                     int st, int ed, int sweep, int Vblksiz, int WANTZ,
                     PLASMA_Complex64_t *WORK)
{
    PLASMA_Complex64_t ctmp;
    int i, len, LDX, lenj;
    int blkid, vpos, taupos, tpos;
    /* find the pointer to the Vs and Ts as stored by the bulgechasing
     * note that in case no eigenvector required V and T are stored
     * on a vector of size N
     * */
     if( WANTZ == 0 ) {
         vpos   = ((sweep+1)%2)*N + st;
         taupos = ((sweep+1)%2)*N + st;
     } else {
         findVTpos(N, NB, Vblksiz, sweep, st,
                   &vpos, &taupos, &tpos, &blkid);
     }

    LDX = LDA-1;
    len = ed-st+1;

    if( uplo == PlasmaUpper ) {
        /* ========================
         *       UPPER CASE
         * ========================*/
        /* Eliminate the row  at st-1 */
        *VP(vpos) = 1.;
        for(i=1; i<len; i++){
            *VP(vpos+i)     = conj(*AU(st-1, st+i));
            *AU(st-1, st+i) = 0.;
        }
        ctmp = conj(*AU(st-1, st));
        LAPACKE_zlarfg_work(len, &ctmp, VP(vpos+1), 1, TAUP(taupos) );
        *AU(st-1, st) = ctmp;
        /* Apply right on A(st:ed,st:ed) */
        ctmp = *TAUP(taupos);
        LAPACKE_zlarfx_work(LAPACK_COL_MAJOR, lapack_const(PlasmaRight),
                            len, len, VP(vpos), ctmp, AU(st, st), LDX, WORK);

        /* Eliminate the created col at st */
        *VQ(vpos) = 1.;
        memcpy( VQ(vpos+1), AU(st+1, st), (len-1)*sizeof(PLASMA_Complex64_t) );
        memset( AU(st+1, st), 0, (len-1)*sizeof(PLASMA_Complex64_t) );
        LAPACKE_zlarfg_work(len, AU(st, st), VQ(vpos+1), 1, TAUQ(taupos) );
        lenj = len-1;
        ctmp = conj(*TAUQ(taupos));
        LAPACKE_zlarfx_work(LAPACK_COL_MAJOR, lapack_const(PlasmaLeft),
                            len, lenj, VQ(vpos), ctmp, AU(st, st+1), LDX, WORK);
    }else{
        /* ========================
         *       LOWER CASE
         * ========================*/
        /* Eliminate the col  at st-1 */
        *VQ(vpos) = 1.;
        memcpy( VQ(vpos+1), AL(st+1, st-1), (len-1)*sizeof(PLASMA_Complex64_t) );
        memset( AL(st+1, st-1), 0, (len-1)*sizeof(PLASMA_Complex64_t) );
        LAPACKE_zlarfg_work(len, AL(st, st-1), VQ(vpos+1), 1, TAUQ(taupos) );
        /* Apply left on A(st:ed,st:ed) */
        ctmp = conj(*TAUQ(taupos));
        LAPACKE_zlarfx_work(LAPACK_COL_MAJOR, lapack_const(PlasmaLeft),
                            len, len, VQ(vpos), ctmp, AL(st, st), LDX, WORK);
        /* Eliminate the created row at st */
        *VP(vpos) = 1.;
        for(i=1; i<len; i++){
            *VP(vpos+i)     = conj(*AL(st, st+i));
            *AL(st, st+i)   = 0.;
        }
        ctmp = conj(*AL(st, st));
        LAPACKE_zlarfg_work(len, &ctmp, VP(vpos+1), 1, TAUP(taupos) );
        *AL(st, st) = ctmp;
        lenj = len-1;
        ctmp = (*TAUP(taupos));
        LAPACKE_zlarfx_work(LAPACK_COL_MAJOR, lapack_const(PlasmaRight),
                            lenj, len, VP(vpos), ctmp, AL(st+1, st), LDX, WORK);
    }
    /* end of uplo case */
    return;
}
/***************************************************************************/
#undef AU
#undef AL
#undef VQ
#undef VP
#undef TAUQ
#undef TAUP
