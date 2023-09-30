/**
 *
 * @file coreblas_s.c
 *
 *  PLASMA core_blas tracing kernel
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 *  This file provides the wrapper for each function of the
 *  core_blas library which will generate an event before and
 *  after the execution of the kernel.
 *  This file is automatically generated with convert2eztrace.pl
 *  script. DO NOT MANUALLY EDIT THIS FILE.
 *
 * @version 2.8.0
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated s Fri Apr  1 11:03:14 2016
 *
 **/
#include <eztrace.h>
#include <ev_codes.h>
#include "common.h"
#include "coreblas_ev_codes.h"
#include "coreblas_macros.h"
#undef COMPLEX
#define REAL
#ifdef REAL
#endif
#ifdef REAL
FUNCTION_VOID( CORE_slaed2_compressq, LAED2_COMPRESSQ, void ,
          (int n, int n1, const int *INDX, const int *ctot, const float *Q, int LDQ, float *Q2, int start, int end),
          (n, n1, INDX, ctot, Q, LDQ, Q2, start, end) )
FUNCTION_VOID( CORE_slaed2_copydef, LAED2_COPYDEF, void ,
          (int n, int n1, int K, const int *ctot, float *Q, int LDQ, const float *Q2, int start, int end ),
          (n, n1, K, ctot, Q, LDQ, Q2, start, end) )
#endif
#ifdef REAL
FUNCTION_VOID( CORE_slaed2_computeK, LAED2_COMPUTEK, void ,
          (int *Kptr, int n, int n1, float *betaptr, float *D, float *Q, int LDQ, float *Z, float *DLAMBDA, float *W, int *INDX, int *INDXC, int *INDXP, int *INDXQ, int *COLTYP),
          (Kptr, n, n1, betaptr, D, Q, LDQ, Z, DLAMBDA, W, INDX, INDXC, INDXP, INDXQ, COLTYP) )
#endif
#ifdef REAL
FUNCTION_VOID( CORE_slaed3_computevectors, LAED3_COMPVEC, void ,
          (int K, int il_nondef, int iu_nondef, float *Q, int LDQ, float *W, float *S, const int *INDXC, int start, int end),
          (K, il_nondef, iu_nondef, Q, LDQ, W, S, INDXC, start, end) )
#endif
#ifdef REAL
FUNCTION_VOID( CORE_slaed3_computeW, LAED3_COMPW, void ,
          (int n, int K, const float *Q, int LDQ, const float *DLAMBDA, float *W, const int *INDX, int start, int end),
          (n, K, Q, LDQ, DLAMBDA, W, INDX, start, end) )
#endif
#ifdef REAL
#endif
#ifdef REAL
FUNCTION_VOID( CORE_slaed3_reduceW, LAED3_REDUCEW, void ,
          (int n, int n1, int K, int l, const float *Q, int LDQ, const float *Wred, float *W),
          (n, n1, K, l, Q, LDQ, Wred, W) )
#endif
#ifdef REAL
FUNCTION_VOID( CORE_slaed3_updatevectors, LAED3_UPDATEVECTORS, void ,
          ( int op, int wsmode, int n, int n1, int K, int il_nondef, int iu_nondef, float *Q, int ldq, float *Q2, const int *ctot, float *W, int start, int end),
          (op, wsmode, n, n1, K, il_nondef, iu_nondef, Q, ldq, Q2, ctot, W, start, end) )
#endif
#ifdef REAL
FUNCTION_TYPE( CORE_slaed4, LAED4, int ,
          (int n, int K, float *D, float beta, float *Q, int LDQ, const float *D0, const float *Z, const int *INDX, int start, int end ),
          (n, K, D, beta, Q, LDQ, D0, Z, INDX, start, end) )
#endif
#ifdef COMPLEX
FUNCTION_TYPE( CORE_slag2c, LAG2Z, int ,
          ( int m, int n, const float *R, int ldr, float *Z, int ldz ),
          (m, n, R, ldr, Z, ldz) )
#endif
#ifdef REAL
#endif
FUNCTION_VOID( CORE_sasum, ASUM, void ,
          (PLASMA_enum storev, PLASMA_enum uplo, int M, int N, const float *A, int lda, float *work),
          (storev, uplo, M, N, A, lda, work) )
FUNCTION_VOID( CORE_sbrdalg1, BRDALG, void ,
          ( PLASMA_enum uplo, int n, int nb, float *A, int lda, float *VQ, float *TAUQ, float *VP, float *TAUP, int Vblksiz, int wantz, int i, int sweepid, int m, int grsiz, float *work),
          (uplo, n, nb, A, lda, VQ, TAUQ, VP, TAUP, Vblksiz, wantz, i, sweepid, m, grsiz, work) )
FUNCTION_VOID( CORE_sgbtype1cb, TYPE1, void ,
          (PLASMA_enum uplo, int N, int NB, float *A, int LDA, float *VQ, float *TAUQ, float *VP, float *TAUP, int st, int ed, int sweep, int Vblksiz, int WANTZ, float *WORK),
          (uplo, N, NB, A, LDA, VQ, TAUQ, VP, TAUP, st, ed, sweep, Vblksiz, WANTZ, WORK) )
FUNCTION_VOID( CORE_sgbtype2cb, TYPE2, void ,
          (PLASMA_enum uplo, int N, int NB, float *A, int LDA, float *VQ, float *TAUQ, float *VP, float *TAUP, int st, int ed, int sweep, int Vblksiz, int WANTZ, float *WORK),
          (uplo, N, NB, A, LDA, VQ, TAUQ, VP, TAUP, st, ed, sweep, Vblksiz, WANTZ, WORK) )
FUNCTION_VOID( CORE_sgbtype3cb, TYPE3, void ,
          (PLASMA_enum uplo, int N, int NB, float *A, int LDA, float *VQ, float *TAUQ, float *VP, float *TAUP, int st, int ed, int sweep, int Vblksiz, int WANTZ, float *WORK),
          (uplo, N, NB, A, LDA, VQ, TAUQ, VP, TAUP, st, ed, sweep, Vblksiz, WANTZ, WORK) )
FUNCTION_TYPE( CORE_sgeadd, GEADD, int ,
          (PLASMA_enum trans, int M, int N, float alpha, const float *A, int LDA, float beta, float *B, int LDB),
          (trans, M, N, alpha, A, LDA, beta, B, LDB) )
FUNCTION_TYPE( CORE_sgelqt, GELQT, int ,
          (int M, int N, int IB, float *A, int LDA, float *T, int LDT, float *TAU, float *WORK),
          (M, N, IB, A, LDA, T, LDT, TAU, WORK) )
FUNCTION_VOID( CORE_sgemm, GEMM, void ,
          (PLASMA_enum transA, int transB, int M, int N, int K, float alpha, const float *A, int LDA, const float *B, int LDB, float beta, float *C, int LDC),
          (transA, transB, M, N, K, alpha, A, LDA, B, LDB, beta, C, LDC) )
FUNCTION_VOID( CORE_sgemv, GEMV, void ,
          (PLASMA_enum trans, int m, int n, float alpha, const float *A, int lda, const float *x, int incx, float beta, float *y, int incy),
          (trans, m, n, alpha, A, lda, x, incx, beta, y, incy) )
FUNCTION_VOID( CORE_sgeqp3_init, GEQP3_INIT, void ,
          ( int n, int *jpvt ),
          (n, jpvt) )
FUNCTION_VOID( CORE_sgeqp3_larfg, LARFG, void ,
          ( PLASMA_desc A, int ii, int jj, int i, int j, float *tau, float *beta ),
          (A, ii, jj, i, j, tau, beta) )
FUNCTION_VOID( CORE_sgeqp3_norms, GEQP3_NORMS, void ,
          ( PLASMA_desc A, int ioff, int joff, float *norms1, float *norms2 ),
          (A, ioff, joff, norms1, norms2) )
FUNCTION_VOID( CORE_sgeqp3_pivot, GEQP3_PIVOT, void ,
          ( PLASMA_desc A, float *F, int ldf, int jj, int k, int *jpvt, float *norms1, float *norms2, int *info ),
          (A, F, ldf, jj, k, jpvt, norms1, norms2, info) )
FUNCTION_TYPE( CORE_sgeqp3_tntpiv, GEQRT, int ,
          (int m, int n, float *A, int lda, int *IPIV, float *tau, int *iwork),
          (m, n, A, lda, IPIV, tau, iwork) )
FUNCTION_VOID( CORE_sgeqp3_update, GEQP3_UPDATE, void ,
          ( const float *Ajj, int lda1, float *Ajk, int lda2, const float *Fk, int ldf, int joff, int k, int koff, int nb, float *norms1, float *norms2, int *info ),
          (Ajj, lda1, Ajk, lda2, Fk, ldf, joff, k, koff, nb, norms1, norms2, info) )
FUNCTION_TYPE( CORE_sgeqrt, GEQRT, int ,
          (int M, int N, int IB, float *A, int LDA, float *T, int LDT, float *TAU, float *WORK),
          (M, N, IB, A, LDA, T, LDT, TAU, WORK) )
FUNCTION_TYPE( CORE_sgessm, GESSM, int ,
          (int M, int N, int K, int IB, const int *IPIV, const float *L, int LDL, float *A, int LDA),
          (M, N, K, IB, IPIV, L, LDL, A, LDA) )
FUNCTION_TYPE( CORE_sgessq, LASSQ, int ,
          (int M, int N, const float *A, int LDA, float *scale, float *sumsq),
          (M, N, A, LDA, scale, sumsq) )
FUNCTION_TYPE( CORE_sgetrf, GETRF, int ,
          (int m, int n, float *A, int lda, int *IPIV, int *info),
          (m, n, A, lda, IPIV, info) )
FUNCTION_TYPE( CORE_sgetrf_incpiv, GETRF, int ,
          (int M, int N, int IB, float *A, int LDA, int *IPIV, int *INFO),
          (M, N, IB, A, LDA, IPIV, INFO) )
FUNCTION_TYPE( CORE_sgetrf_nopiv, GETRF, int ,
          (int M, int N, int IB, float *A, int LDA),
          (M, N, IB, A, LDA) )
FUNCTION_TYPE( CORE_sgetrf_reclap, GETRF, int,
          (CORE_sgetrf_data_t *data, int M, int N, float *A, int LDA, int *IPIV, int *info),
          (data, M, N, A, LDA, IPIV, info) )
FUNCTION_TYPE( CORE_sgetrf_rectil, GETRF, int,
          (CORE_sgetrf_data_t *data, const PLASMA_desc A, int *IPIV, int *info),
          (data, A, IPIV, info) )
FUNCTION_VOID( CORE_sgetrip, GETRIP, void ,
          (int m, int n, float *A, float *W) ,
          (m, n, A, W)  )
FUNCTION_VOID( CORE_ssbtype1cb, TYPE1, void ,
          (int N, int NB, float *A, int LDA, float *V, float *TAU, int st, int ed, int sweep, int Vblksiz, int WANTZ, float *WORK),
          (N, NB, A, LDA, V, TAU, st, ed, sweep, Vblksiz, WANTZ, WORK) )
FUNCTION_VOID( CORE_ssbtype2cb, TYPE2, void ,
          (int N, int NB, float *A, int LDA, float *V, float *TAU, int st, int ed, int sweep, int Vblksiz, int WANTZ, float *WORK),
          (N, NB, A, LDA, V, TAU, st, ed, sweep, Vblksiz, WANTZ, WORK) )
FUNCTION_VOID( CORE_ssbtype3cb, TYPE3, void ,
          (int N, int NB, float *A, int LDA, const float *V, const float *TAU, int st, int ed, int sweep, int Vblksiz, int WANTZ, float *WORK),
          (N, NB, A, LDA, V, TAU, st, ed, sweep, Vblksiz, WANTZ, WORK) )
FUNCTION_VOID( CORE_ssygst, HEGST, void ,
          (int itype, PLASMA_enum uplo, int N, float *A, int LDA, float *B, int LDB, int *INFO),
          (itype, uplo, N, A, LDA, B, LDB, INFO) )
#ifdef COMPLEX
FUNCTION_VOID( CORE_ssymm, HEMM, void ,
          (PLASMA_enum side, PLASMA_enum uplo, int M, int N, float alpha, const float *A, int LDA, const float *B, int LDB, float beta, float *C, int LDC),
          (side, uplo, M, N, alpha, A, LDA, B, LDB, beta, C, LDC) )
#endif
#ifdef COMPLEX
FUNCTION_VOID( CORE_ssyr2k, HER2K, void ,
          (PLASMA_enum uplo, PLASMA_enum trans, int N, int K, float alpha, const float *A, int LDA, const float *B, int LDB, float beta, float *C, int LDC),
          (uplo, trans, N, K, alpha, A, LDA, B, LDB, beta, C, LDC) )
#endif
FUNCTION_TYPE( CORE_ssyrfb, HERFB, int ,
          ( PLASMA_enum uplo, int n, int k, int ib, int nb, const float *A, int lda, const float *T, int ldt, float *C, int ldc, float *WORK, int ldwork ),
          (uplo, n, k, ib, nb, A, lda, T, ldt, C, ldc, WORK, ldwork) )
#ifdef COMPLEX
FUNCTION_VOID( CORE_ssyrk, HERK, void ,
          (PLASMA_enum uplo, PLASMA_enum trans, int N, int K, float alpha, const float *A, int LDA, float beta, float *C, int LDC),
          (uplo, trans, N, K, alpha, A, LDA, beta, C, LDC) )
#endif
#ifdef COMPLEX
FUNCTION_TYPE( CORE_shessq, LASSQ, int ,
          (PLASMA_enum uplo, int N, const float *A, int LDA, float *scale, float *sumsq),
          (uplo, N, A, LDA, scale, sumsq) )
#endif
FUNCTION_VOID( CORE_slacpy, LACPY, void ,
          (PLASMA_enum uplo, int M, int N, const float *A, int LDA, float *B, int LDB),
          (uplo, M, N, A, LDA, B, LDB) )
FUNCTION_TYPE( CORE_slacpy_pivot, LACPY, int ,
          ( const PLASMA_desc descA, PLASMA_enum direct, int k1, int k2, const int *ipiv, int *rankin, int *rankout, float *A, int lda, int init),
          (descA, direct, k1, k2, ipiv, rankin, rankout, A, lda, init) )
FUNCTION_VOID( CORE_slange, LANGE, void ,
          (int norm, int M, int N, const float *A, int LDA, float *work, float *normA),
          (norm, M, N, A, LDA, work, normA) )
#ifdef COMPLEX
FUNCTION_VOID( CORE_slansy, LANHE, void ,
          (int norm, PLASMA_enum uplo, int N, const float *A, int LDA, float *work, float *normA),
          (norm, uplo, N, A, LDA, work, normA) )
#endif
FUNCTION_VOID( CORE_slansy, LANSY, void ,
          (int norm, PLASMA_enum uplo, int N, const float *A, int LDA, float *work, float *normA),
          (norm, uplo, N, A, LDA, work, normA) )
FUNCTION_VOID( CORE_slantr, LANGE, void ,
          (PLASMA_enum norm, PLASMA_enum uplo, PLASMA_enum diag, int M, int N, const float *A, int LDA, float *work, float *normA),
          (norm, uplo, diag, M, N, A, LDA, work, normA) )
FUNCTION_TYPE( CORE_slarfb_gemm, LARFB, int ,
          (PLASMA_enum side, PLASMA_enum trans, int direct, int storev, int M, int N, int K, const float *V, int LDV, const float *T, int LDT, float *C, int LDC, float *WORK, int LDWORK),
          (side, trans, direct, storev, M, N, K, V, LDV, T, LDT, C, LDC, WORK, LDWORK) )
FUNCTION_VOID( CORE_slaset2, LASET, void ,
          (PLASMA_enum uplo, int M, int N, float alpha, float *A, int LDA),
          (uplo, M, N, alpha, A, LDA) )
FUNCTION_VOID( CORE_slaset, LASET, void ,
          (PLASMA_enum uplo, int M, int N, float alpha, float beta, float *A, int LDA),
          (uplo, M, N, alpha, beta, A, LDA) )
FUNCTION_VOID( CORE_slaswp, LASWP, void ,
          (int N, float *A, int LDA, int I1, int I2, const int *IPIV, int INC),
          (N, A, LDA, I1, I2, IPIV, INC) )
FUNCTION_TYPE( CORE_slaswp_ontile, LASWP, int ,
          (PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc),
          (descA, i1, i2, ipiv, inc) )
FUNCTION_TYPE( CORE_sswptr_ontile, TRSM, int ,
          (PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc, const float *Akk, int ldak),
          (descA, i1, i2, ipiv, inc, Akk, ldak) )
FUNCTION_TYPE( CORE_slaswpc_ontile, LASWP, int ,
          (PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc),
          (descA, i1, i2, ipiv, inc) )
FUNCTION_TYPE( CORE_slatro, LATRO, int ,
          (PLASMA_enum uplo, PLASMA_enum trans, int M, int N, const float *A, int LDA, float *B, int LDB),
          (uplo, trans, M, N, A, LDA, B, LDB) )
FUNCTION_VOID( CORE_slauum, LAUUM, void ,
          (PLASMA_enum uplo, int N, float *A, int LDA),
          (uplo, N, A, LDA) )
FUNCTION_TYPE( CORE_spemv, PEMV, int ,
          (PLASMA_enum trans, int storev, int M, int N, int L, float ALPHA, const float *A, int LDA, const float *X, int INCX, float BETA, float *Y, int INCY, float *WORK),
          (trans, storev, M, N, L, ALPHA, A, LDA, X, INCX, BETA, Y, INCY, WORK) )
#ifdef COMPLEX
FUNCTION_VOID( CORE_splgsy, PLGHE, void ,
          ( float bump, int m, int n, float *A, int lda, int gM, int m0, int n0, unsigned long long int seed ),
          (bump, m, n, A, lda, gM, m0, n0, seed) )
#endif
FUNCTION_VOID( CORE_splgsy, PLGSY, void ,
          ( float bump, int m, int n, float *A, int lda, int gM, int m0, int n0, unsigned long long int seed ),
          (bump, m, n, A, lda, gM, m0, n0, seed) )
FUNCTION_VOID( CORE_splrnt, PLRNT, void ,
          ( int m, int n, float *A, int lda, int gM, int m0, int n0, unsigned long long int seed ),
          (m, n, A, lda, gM, m0, n0, seed) )
FUNCTION_TYPE( CORE_spltmg, PLRNT, int ,
          ( PLASMA_enum mtxtype, int M, int N, float *A, int LDA, int gM, int gN, int m0, int n0, unsigned long long int seed ),
          (mtxtype, M, N, A, LDA, gM, gN, m0, n0, seed) )
FUNCTION_TYPE( CORE_spltmg_chebvand, PLRNT, int ,
          ( int M, int N, float *A, int LDA, int gN, int m0, int n0, float *W ),
          (M, N, A, LDA, gN, m0, n0, W) )
FUNCTION_TYPE( CORE_spltmg_circul, PLRNT, int ,
          ( int M, int N, float *A, int LDA, int gM, int m0, int n0, const float *V ),
          (M, N, A, LDA, gM, m0, n0, V) )
FUNCTION_VOID( CORE_spltmg_condexq, PLRNT, void ,
          ( int M, int N, float *Q, int LDQ ),
          (M, N, Q, LDQ) )
FUNCTION_VOID( CORE_spltmg_fiedler, PLRNT, void ,
          ( int M, int N, const float *X, int incX, const float *Y, int incY, float *A, int LDA ),
          (M, N, X, incX, Y, incY, A, LDA) )
FUNCTION_TYPE( CORE_spltmg_hankel, PLRNT, int ,
          ( PLASMA_enum uplo, int M, int N, float *A, int LDA, int m0, int n0, int nb, const float *V1, const float *V2 ),
          (uplo, M, N, A, LDA, m0, n0, nb, V1, V2) )
FUNCTION_VOID( CORE_spltmg_toeppd1, PLRNT, void ,
          ( int gM, int m0, int M, float *W, unsigned long long int seed ),
          (gM, m0, M, W, seed) )
FUNCTION_VOID( CORE_spltmg_toeppd2, PLRNT, void ,
          ( int M, int N, int K, int m0, int n0, const float *W, float *A, int LDA ),
          (M, N, K, m0, n0, W, A, LDA) )
FUNCTION_VOID( CORE_spotrf, POTRF, void ,
          (PLASMA_enum uplo, int N, float *A, int LDA, int *info),
          (uplo, N, A, LDA, info) )
FUNCTION_VOID( CORE_ssetvar, SETVAR, void ,
          (const float *alpha, float *x),
          (alpha, x) )
FUNCTION_VOID( CORE_sshiftw, SHIFTW, void ,
          (int s, int cl, int m, int n, int L, float *A, float *W) ,
          (s, cl, m, n, L, A, W)  )
FUNCTION_VOID( CORE_sshift, SHIFT, void ,
          (int s, int m, int n, int L, float *A) ,
          (s, m, n, L, A)  )
FUNCTION_TYPE( CORE_sssssm, SSSSM, int ,
          (int M1, int N1, int M2, int N2, int K, int IB, float *A1, int LDA1, float *A2, int LDA2, const float *L1, int LDL1, const float *L2, int LDL2, const int *IPIV),
          (M1, N1, M2, N2, K, IB, A1, LDA1, A2, LDA2, L1, LDL1, L2, LDL2, IPIV) )
#ifdef COMPLEX
FUNCTION_TYPE( CORE_sstedc, STEDC, int ,
          ( PLASMA_enum compz, int n, float *D, float *E, float *Z, int LDZ, float *WORK, int LWORK, float *RWORK, int LRWORK, int *IWORK, int LIWORK ),
          (compz, n, D, E, Z, LDZ, WORK, LWORK, RWORK, LRWORK, IWORK, LIWORK) )
#else
FUNCTION_TYPE( CORE_sstedc, STEDC, int ,
          ( PLASMA_enum compz, int n, float *D, float *E, float *Z, int LDZ, float *WORK, int LWORK, int *IWORK, int LIWORK ),
          (compz, n, D, E, Z, LDZ, WORK, LWORK, IWORK, LIWORK) )
#endif
FUNCTION_TYPE( CORE_ssteqr, STEQR, int ,
          ( PLASMA_enum compz, int n, float *D, float *E, float *Z, int LDZ, float *WORK ),
          (compz, n, D, E, Z, LDZ, WORK) )
FUNCTION_VOID( CORE_sswap, SWAP, void ,
          (int m, int n, float *Q, int ldq, const float *work, const int *perm, int start, int end),
          (m, n, Q, ldq, work, perm, start, end) )
FUNCTION_VOID( CORE_sswpab, SWPAB, void ,
          (int i, int n1, int n2, float *A, float *work) ,
          (i, n1, n2, A, work)  )
FUNCTION_VOID( CORE_ssymm, SYMM, void ,
          (PLASMA_enum side, PLASMA_enum uplo, int M, int N, float alpha, const float *A, int LDA, const float *B, int LDB, float beta, float *C, int LDC),
          (side, uplo, M, N, alpha, A, LDA, B, LDB, beta, C, LDC) )
FUNCTION_VOID( CORE_ssyr2k, SYR2K, void ,
          (PLASMA_enum uplo, PLASMA_enum trans, int N, int K, float alpha, const float *A, int LDA, const float *B, int LDB, float beta, float *C, int LDC),
          (uplo, trans, N, K, alpha, A, LDA, B, LDB, beta, C, LDC) )
FUNCTION_VOID( CORE_ssyrk, SYRK, void ,
          (PLASMA_enum uplo, PLASMA_enum trans, int N, int K, float alpha, const float *A, int LDA, float beta, float *C, int LDC),
          (uplo, trans, N, K, alpha, A, LDA, beta, C, LDC) )
FUNCTION_TYPE( CORE_ssyssq, LASSQ, int ,
          (PLASMA_enum uplo, int N, const float *A, int LDA, float *scale, float *sumsq),
          (uplo, N, A, LDA, scale, sumsq) )
FUNCTION_TYPE( CORE_stradd, GEADD, int ,
          (PLASMA_enum uplo, PLASMA_enum trans, int M, int N, float alpha, const float *A, int LDA, float beta, float *B, int LDB),
          (uplo, trans, M, N, alpha, A, LDA, beta, B, LDB) )
FUNCTION_VOID( CORE_strasm, ASUM, void ,
          (PLASMA_enum storev, PLASMA_enum uplo, PLASMA_enum diag, int M, int N, const float *A, int lda, float *work),
          (storev, uplo, diag, M, N, A, lda, work) )
FUNCTION_VOID( CORE_strdalg1, TRDALG, void ,
          ( int n, int nb, float *A, int lda, float *V, float *TAU, int Vblksiz, int wantz, int i, int sweepid, int m, int grsiz, float *work),
          (n, nb, A, lda, V, TAU, Vblksiz, wantz, i, sweepid, m, grsiz, work) )
FUNCTION_VOID( CORE_strmm, TRMM, void ,
          (PLASMA_enum side, PLASMA_enum uplo, PLASMA_enum transA, PLASMA_enum diag, int M, int N, float alpha, const float *A, int LDA, float *B, int LDB),
          (side, uplo, transA, diag, M, N, alpha, A, LDA, B, LDB) )
FUNCTION_VOID( CORE_strsm, TRSM, void ,
          (PLASMA_enum side, PLASMA_enum uplo, PLASMA_enum transA, PLASMA_enum diag, int M, int N, float alpha, const float *A, int LDA, float *B, int LDB),
          (side, uplo, transA, diag, M, N, alpha, A, LDA, B, LDB) )
FUNCTION_TYPE( CORE_strssq, LASSQ, int ,
          (PLASMA_enum uplo, PLASMA_enum diag, int M, int N, const float *A, int LDA, float *scale, float *sumsq),
          (uplo, diag, M, N, A, LDA, scale, sumsq) )
FUNCTION_VOID( CORE_strtri, TRTRI, void ,
          (PLASMA_enum uplo, PLASMA_enum diag, int N, float *A, int LDA, int *info),
          (uplo, diag, N, A, LDA, info) )
FUNCTION_TYPE( CORE_stslqt, TSLQT, int ,
          (int M, int N, int IB, float *A1, int LDA1, float *A2, int LDA2, float *T, int LDT, float *TAU, float *WORK),
          (M, N, IB, A1, LDA1, A2, LDA2, T, LDT, TAU, WORK) )
FUNCTION_TYPE( CORE_stsmlq, TSMLQ, int ,
          (PLASMA_enum side, PLASMA_enum trans, int M1, int N1, int M2, int N2, int K, int IB, float *A1, int LDA1, float *A2, int LDA2, const float *V, int LDV, const float *T, int LDT, float *WORK, int LDWORK),
          (side, trans, M1, N1, M2, N2, K, IB, A1, LDA1, A2, LDA2, V, LDV, T, LDT, WORK, LDWORK) )
FUNCTION_TYPE( CORE_stsmlq_corner, TSMLQ, int ,
          ( int m1, int n1, int m2, int n2, int m3, int n3, int k, int ib, int nb, float *A1, int lda1, float *A2, int lda2, float *A3, int lda3, const float *V, int ldv, const float *T, int ldt, float *WORK, int ldwork),
          (m1, n1, m2, n2, m3, n3, k, ib, nb, A1, lda1, A2, lda2, A3, lda3, V, ldv, T, ldt, WORK, ldwork) )
FUNCTION_TYPE( CORE_stsmlq_sytra1, TSMLQ, int ,
          ( PLASMA_enum side, PLASMA_enum trans, int m1, int n1, int m2, int n2, int k, int ib, float *A1, int lda1, float *A2, int lda2, const float *V, int ldv, const float *T, int ldt, float *WORK, int ldwork),
          (side, trans, m1, n1, m2, n2, k, ib, A1, lda1, A2, lda2, V, ldv, T, ldt, WORK, ldwork) )
FUNCTION_TYPE( CORE_stsmqr, TSMQR, int ,
          (PLASMA_enum side, PLASMA_enum trans, int M1, int N1, int M2, int N2, int K, int IB, float *A1, int LDA1, float *A2, int LDA2, const float *V, int LDV, const float *T, int LDT, float *WORK, int LDWORK),
          (side, trans, M1, N1, M2, N2, K, IB, A1, LDA1, A2, LDA2, V, LDV, T, LDT, WORK, LDWORK) )
FUNCTION_TYPE( CORE_stsmqr_corner, TSMQR, int ,
          ( int m1, int n1, int m2, int n2, int m3, int n3, int k, int ib, int nb, float *A1, int lda1, float *A2, int lda2, float *A3, int lda3, const float *V, int ldv, const float *T, int ldt, float *WORK, int ldwork),
          (m1, n1, m2, n2, m3, n3, k, ib, nb, A1, lda1, A2, lda2, A3, lda3, V, ldv, T, ldt, WORK, ldwork) )
FUNCTION_TYPE( CORE_stsmqr_sytra1, TSMQR, int ,
          ( PLASMA_enum side, PLASMA_enum trans, int m1, int n1, int m2, int n2, int k, int ib, float *A1, int lda1, float *A2, int lda2, const float *V, int ldv, const float *T, int ldt, float *WORK, int ldwork),
          (side, trans, m1, n1, m2, n2, k, ib, A1, lda1, A2, lda2, V, ldv, T, ldt, WORK, ldwork) )
FUNCTION_TYPE( CORE_stsqrt, TSQRT, int ,
          (int M, int N, int IB, float *A1, int LDA1, float *A2, int LDA2, float *T, int LDT, float *TAU, float *WORK),
          (M, N, IB, A1, LDA1, A2, LDA2, T, LDT, TAU, WORK) )
FUNCTION_TYPE( CORE_ststrf, TSTRF, int ,
          (int M, int N, int IB, int NB, float *U, int LDU, float *A, int LDA, float *L, int LDL, int *IPIV, float *WORK, int LDWORK, int *INFO),
          (M, N, IB, NB, U, LDU, A, LDA, L, LDL, IPIV, WORK, LDWORK, INFO) )
FUNCTION_TYPE( CORE_sttlqt, TTLQT, int ,
          (int M, int N, int IB, float *A1, int LDA1, float *A2, int LDA2, float *T, int LDT, float *TAU, float *WORK),
          (M, N, IB, A1, LDA1, A2, LDA2, T, LDT, TAU, WORK) )
FUNCTION_TYPE( CORE_sttmlq, TTMLQ, int ,
          (PLASMA_enum side, PLASMA_enum trans, int M1, int N1, int M2, int N2, int K, int IB, float *A1, int LDA1, float *A2, int LDA2, const float *V, int LDV, const float *T, int LDT, float *WORK, int LDWORK),
          (side, trans, M1, N1, M2, N2, K, IB, A1, LDA1, A2, LDA2, V, LDV, T, LDT, WORK, LDWORK) )
FUNCTION_TYPE( CORE_sttmqr, TTMQR, int ,
          (PLASMA_enum side, PLASMA_enum trans, int M1, int N1, int M2, int N2, int K, int IB, float *A1, int LDA1, float *A2, int LDA2, const float *V, int LDV, const float *T, int LDT, float *WORK, int LDWORK),
          (side, trans, M1, N1, M2, N2, K, IB, A1, LDA1, A2, LDA2, V, LDV, T, LDT, WORK, LDWORK) )
FUNCTION_TYPE( CORE_sttqrt, TTQRT, int ,
          (int M, int N, int IB, float *A1, int LDA1, float *A2, int LDA2, float *T, int LDT, float *TAU, float *WORK),
          (M, N, IB, A1, LDA1, A2, LDA2, T, LDT, TAU, WORK) )
FUNCTION_TYPE( CORE_sormlq, UNMLQ, int ,
          (PLASMA_enum side, PLASMA_enum trans, int M, int N, int K, int IB, const float *A, int LDA, const float *T, int LDT, float *C, int LDC, float *WORK, int LDWORK),
          (side, trans, M, N, K, IB, A, LDA, T, LDT, C, LDC, WORK, LDWORK) )
FUNCTION_TYPE( CORE_sormqr, UNMQR, int ,
          (PLASMA_enum side, PLASMA_enum trans, int M, int N, int K, int IB, const float *A, int LDA, const float *T, int LDT, float *C, int LDC, float *WORK, int LDWORK),
          (side, trans, M, N, K, IB, A, LDA, T, LDT, C, LDC, WORK, LDWORK) )
