/**
 *
 * @file cunglq.c
 *
 *  PLASMA computational routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Hatem Ltaief
 * @author Jakub Kurzak
 * @date 2010-11-15
 * @generated c Fri Apr  1 11:02:55 2016
 *
 **/
#include "common.h"

/***************************************************************************//**
 *
 * @ingroup PLASMA_Complex32_t
 *
 *  PLASMA_cunglq - Generates an M-by-N matrix Q with orthonormal rows, which is
 *  defined as the first M rows of a product of the elementary reflectors
 *  returned by PLASMA_cgelqf.
 *
 *******************************************************************************
 *
 * @param[in] M
 *          The number of rows of the matrix Q. M >= 0.
 *
 * @param[in] N
 *          The number of columns of the matrix Q. N >= M.
 *
 * @param[in] K
 *          The number of rows of elementary tile reflectors whose product
 *          defines the matrix Q.
 *          M >= K >= 0.
 *
 * @param[in] A
 *          Details of the LQ factorization of the original matrix A as returned
 *          by PLASMA_cgelqf.
 *
 * @param[in] LDA
 *          The leading dimension of the array A. LDA >= max(1,M).
 *
 * @param[in] descT
 *          Auxiliary factorization data, computed by PLASMA_cgelqf.
 *
 * @param[out] Q
 *          On exit, the M-by-N matrix Q.
 *
 * @param[in] LDQ
 *          The leading dimension of the array Q. LDQ >= max(1,M).
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *          \retval PLASMA_SUCCESS <0 if -i, the i-th argument had an illegal
 *          value
 *
 *******************************************************************************
 *
 * @sa PLASMA_cunglq_Tile
 * @sa PLASMA_cunglq_Tile_Async
 * @sa PLASMA_cunglq
 * @sa PLASMA_dorglq
 * @sa PLASMA_sorglq
 * @sa PLASMA_cgelqf
 *
 ******************************************************************************/
int PLASMA_cunglq(int M, int N, int K,
                  PLASMA_Complex32_t *A, int LDA,
                  PLASMA_desc *descT,
                  PLASMA_Complex32_t *Q, int LDQ)
{
    int NB;
    int status;
    plasma_context_t *plasma;
    PLASMA_sequence *sequence = NULL;
    PLASMA_request request = PLASMA_REQUEST_INITIALIZER;
    PLASMA_desc descA, descQ;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_cunglq", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    /* Check input arguments */
    if (M < 0) {
        plasma_error("PLASMA_cunglq", "illegal value of M");
        return -1;
    }
    if (N < M) {
        plasma_error("PLASMA_cunglq", "illegal value of N");
        return -2;
    }
    if (K < 0 || K > M) {
        plasma_error("PLASMA_cunglq", "illegal value of K");
        return -3;
    }
    if (LDA < max(1, M)) {
        plasma_error("PLASMA_cunglq", "illegal value of LDA");
        return -5;
    }
    if (LDQ < max(1, M)) {
        plasma_error("PLASMA_cunglq", "illegal value of LDQ");
        return -8;
    }
    /* Quick return - currently NOT equivalent to LAPACK's:
     * CALL DLASET( 'Full', MAX( M, N ), NRHS, ZERO, ZERO, B, LDQ ) */
    if (min(M, min(N, K)) == 0)
        return PLASMA_SUCCESS;

    /* Tune NB & IB depending on M, N & NRHS; Set NBNB */
    status = plasma_tune(PLASMA_FUNC_CGELS, M, N, 0);
    if (status != PLASMA_SUCCESS) {
        plasma_error("PLASMA_cunglq", "plasma_tune() failed");
        return status;
    }

    /* Set NT */
    NB = PLASMA_NB;

    plasma_sequence_create(plasma, &sequence);

    if ( PLASMA_TRANSLATION == PLASMA_OUTOFPLACE ) {
        plasma_cooplap2tile( descA, A, NB, NB, LDA, N, 0, 0, K, N, sequence, &request,
                             plasma_desc_mat_free(&(descA)) );
        plasma_cooplap2tile( descQ, Q, NB, NB, LDQ, N, 0, 0, M, N, sequence, &request,
                             plasma_desc_mat_free(&(descA)); plasma_desc_mat_free(&(descQ)));
    } else {
        plasma_ciplap2tile( descA, A, NB, NB, LDA, N, 0, 0, K, N,
                            sequence, &request);
        plasma_ciplap2tile( descQ, Q, NB, NB, LDQ, N, 0, 0, M, N,
                            sequence, &request);
    }

    /* Call the tile interface */
    PLASMA_cunglq_Tile_Async(&descA, descT, &descQ, sequence, &request);

    if ( PLASMA_TRANSLATION == PLASMA_OUTOFPLACE ) {
        plasma_cooptile2lap( descQ, Q, NB, NB, LDQ, N,  sequence, &request);
        plasma_dynamic_sync();
        plasma_desc_mat_free(&descA);
        plasma_desc_mat_free(&descQ);
    } else {
        plasma_ciptile2lap( descA, A, NB, NB, LDA, N,  sequence, &request);
        plasma_ciptile2lap( descQ, Q, NB, NB, LDQ, N,  sequence, &request);
        plasma_dynamic_sync();
    }

    status = sequence->status;
    plasma_sequence_destroy(plasma, sequence);
    return status;
}

/***************************************************************************//**
 *
 * @ingroup PLASMA_Complex32_t_Tile
 *
 * PLASMA_cunglq_Tile - Generates an M-by-N matrix Q with orthonormal rows,
 * which is defined as the first M rows of a product of the elementary
 * reflectors returned by PLASMA_cgelqf.
 * All matrices are passed through descriptors. All dimensions are taken from
 * the descriptors.
 *
 *******************************************************************************
 *
 * @param[in] A
 *          Details of the LQ factorization of the original matrix A as returned by PLASMA_cgelqf.
 *
 * @param[in] T
 *          Auxiliary factorization data, computed by PLASMA_cgelqf.
 *
 * @param[out] Q
 *          On exit, the M-by-N matrix Q.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *
 *******************************************************************************
 *
 * @sa PLASMA_cunglq
 * @sa PLASMA_cunglq_Tile_Async
 * @sa PLASMA_cunglq_Tile
 * @sa PLASMA_dorglq_Tile
 * @sa PLASMA_sorglq_Tile
 * @sa PLASMA_cgelqf_Tile
 *
 ******************************************************************************/
int PLASMA_cunglq_Tile(PLASMA_desc *A, PLASMA_desc *T, PLASMA_desc *Q)
{
    plasma_context_t *plasma;
    PLASMA_sequence *sequence = NULL;
    PLASMA_request request = PLASMA_REQUEST_INITIALIZER;
    int status;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_cunglq_Tile", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    plasma_sequence_create(plasma, &sequence);
    PLASMA_cunglq_Tile_Async(A, T, Q, sequence, &request);
    plasma_dynamic_sync();
    status = sequence->status;
    plasma_sequence_destroy(plasma, sequence);
    return status;
}

/***************************************************************************//**
 *
 * @ingroup PLASMA_Complex32_t_Tile_Async
 *
 *  Non-blocking equivalent of PLASMA_cunglq_Tile().
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
 * @sa PLASMA_cunglq
 * @sa PLASMA_cunglq_Tile
 * @sa PLASMA_cunglq_Tile_Async
 * @sa PLASMA_dorglq_Tile_Async
 * @sa PLASMA_sorglq_Tile_Async
 * @sa PLASMA_cgelqf_Tile_Async
 *
 ******************************************************************************/
int PLASMA_cunglq_Tile_Async(PLASMA_desc *A, PLASMA_desc *T, PLASMA_desc *Q,
                             PLASMA_sequence *sequence, PLASMA_request *request)
{
    PLASMA_desc descA;
    PLASMA_desc descT;
    PLASMA_desc descQ;
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_cunglq_Tile", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        plasma_fatal_error("PLASMA_cunglq_Tile", "NULL sequence");
        return PLASMA_ERR_UNALLOCATED;
    }
    if (request == NULL) {
        plasma_fatal_error("PLASMA_cunglq_Tile", "NULL request");
        return PLASMA_ERR_UNALLOCATED;
    }
    /* Check sequence status */
    if (sequence->status == PLASMA_SUCCESS)
        request->status = PLASMA_SUCCESS;
    else
        return plasma_request_fail(sequence, request, PLASMA_ERR_SEQUENCE_FLUSHED);

    /* Check descriptors for correctness */
    if (plasma_desc_check(A) != PLASMA_SUCCESS) {
        plasma_error("PLASMA_cunglq_Tile", "invalid first descriptor");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    } else {
        descA = *A;
    }
    if (plasma_desc_check(T) != PLASMA_SUCCESS) {
        plasma_error("PLASMA_cunglq_Tile", "invalid second descriptor");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    } else {
        descT = *T;
    }
    if (plasma_desc_check(Q) != PLASMA_SUCCESS) {
        plasma_error("PLASMA_cunglq_Tile", "invalid third descriptor");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    } else {
        descQ = *Q;
    }
    /* Check input arguments */
    if (descA.nb != descA.mb || descQ.nb != descQ.mb) {
        plasma_error("PLASMA_cunglq_Tile", "only square tiles supported");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    }
    /* Quick return - currently NOT equivalent to LAPACK's:
     * CALL DLASET( 'Full', MAX( M, N ), NRHS, ZERO, ZERO, Q, LDQ ) */
/*
    if (min(M, N) == 0)
        return PLASMA_SUCCESS;
*/
    plasma_dynamic_call_6(plasma_pclaset,
        PLASMA_enum, PlasmaUpperLower,
        PLASMA_Complex32_t, 0.,
        PLASMA_Complex32_t, 1.,
        PLASMA_desc, descQ,
        PLASMA_sequence*, sequence,
        PLASMA_request*, request);

    if (plasma->householder == PLASMA_FLAT_HOUSEHOLDER) {
        plasma_dynamic_call_5(plasma_pcunglq,
            PLASMA_desc, descA,
            PLASMA_desc, descQ,
            PLASMA_desc, descT,
            PLASMA_sequence*, sequence,
            PLASMA_request*, request);
    }
    else {
        plasma_dynamic_call_6(plasma_pcunglqrh,
            PLASMA_desc, descA,
            PLASMA_desc, descQ,
            PLASMA_desc, descT,
            PLASMA_enum, PLASMA_RHBLK,
            PLASMA_sequence*, sequence,
            PLASMA_request*, request);
    }

    return PLASMA_SUCCESS;
}
