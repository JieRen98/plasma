/**
 *
 * @file zgelqf.c
 *
 *  PLASMA computational routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Jakub Kurzak
 * @author Dulceneia Becker
 * @date 2010-11-15
 * @precisions normal z -> s d c
 *
 **/
#include "common.h"

/***************************************************************************//**
 *
 * @ingroup PLASMA_Complex64_t
 *
 *  PLASMA_zgelqf - Computes the tile LQ factorization of a complex M-by-N matrix A: A = L * Q.
 *
 *******************************************************************************
 *
 * @param[in] M
 *          The number of rows of the matrix A. M >= 0.
 *
 * @param[in] N
 *          The number of columns of the matrix A. N >= 0.
 *
 * @param[in,out] A
 *          On entry, the M-by-N matrix A.
 *          On exit, the elements on and below the diagonal of the array contain the m-by-min(M,N)
 *          lower trapezoidal matrix L (L is lower triangular if M <= N); the elements above the
 *          diagonal represent the unitary matrix Q as a product of elementary reflectors, stored
 *          by tiles.
 *
 * @param[in] LDA
 *          The leading dimension of the array A. LDA >= max(1,M).
 *
 * @param[out] descT
 *          On exit, auxiliary factorization data, required by PLASMA_zgelqs to solve the system
 *          of equations.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *          \retval <0 if -i, the i-th argument had an illegal value
 *
 *******************************************************************************
 *
 * @sa PLASMA_zgelqf_Tile
 * @sa PLASMA_zgelqf_Tile_Async
 * @sa PLASMA_cgelqf
 * @sa PLASMA_dgelqf
 * @sa PLASMA_sgelqf
 * @sa PLASMA_zgelqs
 *
 ******************************************************************************/
int PLASMA_zgelqf(int M, int N,
                  PLASMA_Complex64_t *A, int LDA,
                  PLASMA_desc *descT)
{
    int NB;
    int status;
    plasma_context_t *plasma;
    PLASMA_sequence *sequence = NULL;
    PLASMA_request request = PLASMA_REQUEST_INITIALIZER;
    PLASMA_desc descA;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_zgelqf", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }

    /* Check input arguments */
    if (M < 0) {
        plasma_error("PLASMA_zgelqf", "illegal value of M");
        return -1;
    }
    if (N < 0) {
        plasma_error("PLASMA_zgelqf", "illegal value of N");
        return -2;
    }
    if (LDA < max(1, M)) {
        plasma_error("PLASMA_zgelqf", "illegal value of LDA");
        return -4;
    }

    /* Quick return */
    if (min(M, N) == 0)
        return PLASMA_SUCCESS;

    /* Tune NB & IB depending on M, N & NRHS; Set NBNBSIZE */
    status = plasma_tune(PLASMA_FUNC_ZGELS, M, N, 0);
    if (status != PLASMA_SUCCESS) {
        plasma_error("PLASMA_zgelqf", "plasma_tune() failed");
        return status;
    }

    /* Set NT */
    NB = PLASMA_NB;

    plasma_sequence_create(plasma, &sequence);
 
    if ( PLASMA_TRANSLATION == PLASMA_OUTOFPLACE ) {
        plasma_zooplap2tile( descA, A, NB, NB, LDA, N, 0, 0, M, N, sequence, &request,
                             plasma_desc_mat_free(&(descA)) );
    } else {
        plasma_ziplap2tile( descA, A, NB, NB, LDA, N, 0, 0, M, N,
                            sequence, &request);
    }

    /* Call the tile interface */
    PLASMA_zgelqf_Tile_Async(&descA, descT, sequence, &request);

    if ( PLASMA_TRANSLATION == PLASMA_OUTOFPLACE ) {
        plasma_zooptile2lap( descA, A, NB, NB, LDA, N,  sequence, &request);
        plasma_dynamic_sync();
        plasma_desc_mat_free(&descA);
    } else {
        plasma_ziptile2lap( descA, A, NB, NB, LDA, N,  sequence, &request);
        plasma_dynamic_sync();
    }

    status = sequence->status;
    plasma_sequence_destroy(plasma, sequence);
    return status;
}

/***************************************************************************//**
 *
 * @ingroup PLASMA_Complex64_t_Tile
 *
 *  PLASMA_zgelqf_Tile - Computes the tile LQ factorization of a matrix.
 *  Tile equivalent of PLASMA_zgelqf().
 *  Operates on matrices stored by tiles.
 *  All matrices are passed through descriptors.
 *  All dimensions are taken from the descriptors.
 *
 *******************************************************************************
 *
 * @param[in,out] A
 *          On entry, the M-by-N matrix A.
 *          On exit, the elements on and below the diagonal of the array contain the m-by-min(M,N)
 *          lower trapezoidal matrix L (L is lower triangular if M <= N); the elements above the
 *          diagonal represent the unitary matrix Q as a product of elementary reflectors, stored
 *          by tiles.
 *
 * @param[out] T
 *          On exit, auxiliary factorization data, required by PLASMA_zgelqs to solve the system
 *          of equations.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *
 *******************************************************************************
 *
 * @sa PLASMA_zgelqf
 * @sa PLASMA_zgelqf_Tile_Async
 * @sa PLASMA_cgelqf_Tile
 * @sa PLASMA_dgelqf_Tile
 * @sa PLASMA_sgelqf_Tile
 * @sa PLASMA_zgelqs_Tile
 *
 ******************************************************************************/
int PLASMA_zgelqf_Tile(PLASMA_desc *A, PLASMA_desc *T)
{
    plasma_context_t *plasma;
    PLASMA_sequence *sequence = NULL;
    PLASMA_request request = PLASMA_REQUEST_INITIALIZER;
    int status;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_zgelqf_Tile", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    plasma_sequence_create(plasma, &sequence);
    PLASMA_zgelqf_Tile_Async(A, T, sequence, &request);
    plasma_dynamic_sync();
    status = sequence->status;
    plasma_sequence_destroy(plasma, sequence);
    return status;
}

/***************************************************************************//**
 *
 * @ingroup PLASMA_Complex64_t_Tile_Async
 *
 *  PLASMA_zgelqf_Tile_Async - Computes the tile LQ factorization of a matrix.
 *  Non-blocking equivalent of PLASMA_zgelqf_Tile().
 *  May return before the computation is finished.
 *  Allows for pipelining of operations at runtime.
 *
 *******************************************************************************
 *
 * @param[in] sequence
 *          Identifies the sequence of function calls that this call belongs to
 *          (for completion checks and exception handling purposes).
 *
 * @param[out] request
 *          Identifies this function call (for exception handling purposes).
 *
 *******************************************************************************
 *
 * @sa PLASMA_zgelqf
 * @sa PLASMA_zgelqf_Tile
 * @sa PLASMA_cgelqf_Tile_Async
 * @sa PLASMA_dgelqf_Tile_Async
 * @sa PLASMA_sgelqf_Tile_Async
 * @sa PLASMA_zgelqs_Tile_Async
 *
 ******************************************************************************/
int PLASMA_zgelqf_Tile_Async(PLASMA_desc *A, PLASMA_desc *T,
                             PLASMA_sequence *sequence, PLASMA_request *request)
{
    PLASMA_desc descA;
    PLASMA_desc descT;
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_zgelqf_Tile", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        plasma_fatal_error("PLASMA_zgelqf_Tile", "NULL sequence");
        return PLASMA_ERR_UNALLOCATED;
    }
    if (request == NULL) {
        plasma_fatal_error("PLASMA_zgelqf_Tile", "NULL request");
        return PLASMA_ERR_UNALLOCATED;
    }
    /* Check sequence status */
    if (sequence->status == PLASMA_SUCCESS)
        request->status = PLASMA_SUCCESS;
    else
        return plasma_request_fail(sequence, request, PLASMA_ERR_SEQUENCE_FLUSHED);

    /* Check descriptors for correctness */
    if (plasma_desc_check(A) != PLASMA_SUCCESS) {
        plasma_error("PLASMA_zgelqf_Tile", "invalid first descriptor");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    } else {
        descA = *A;
    }
    if (plasma_desc_check(T) != PLASMA_SUCCESS) {
        plasma_error("PLASMA_zgelqf_Tile", "invalid second descriptor");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    } else {
        descT = *T;
    }
    /* Check input arguments */
    if (descA.nb != descA.mb) {
        plasma_error("PLASMA_zgelqf_Tile", "only square tiles supported");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    }
    /* Quick return */
/*
    if (min(M, N) == 0)
        return PLASMA_SUCCESS;
*/
    if (plasma->householder == PLASMA_FLAT_HOUSEHOLDER) {
        plasma_parallel_call_4(plasma_pzgelqf,
            PLASMA_desc, descA,
            PLASMA_desc, descT,
            PLASMA_sequence*, sequence,
            PLASMA_request*, request);
    }
    else {
        plasma_dynamic_call_5(plasma_pzgelqfrh,
            PLASMA_desc, descA,
            PLASMA_desc, descT,
            PLASMA_enum, PLASMA_RHBLK,
            PLASMA_sequence*, sequence,
            PLASMA_request*, request);
    }

    return PLASMA_SUCCESS;
}
