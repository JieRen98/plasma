/**
 *
 * @file qwrapper_zgetrf.c
 *
 *  PLASMA core_blas quark wrapper
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @precisions normal z -> c d s
 *
 **/
#include <lapacke.h>
#include "common.h"

/***************************************************************************//**
 *
 **/
void QUARK_CORE_zgetrf(Quark *quark, Quark_Task_Flags *task_flags,
                       int m, int n, int nb,
                       PLASMA_Complex64_t *A, int lda,
                       int *IPIV,
                       PLASMA_sequence *sequence, PLASMA_request *request,
                       PLASMA_bool check_info, int iinfo)
{
    plasma_profile_by_kernel( task_flags, GETRF );

    QUARK_Insert_Task(quark, CORE_zgetrf_quark, task_flags,
        sizeof(int),                        &m,             VALUE,
        sizeof(int),                        &n,             VALUE,
        sizeof(PLASMA_Complex64_t)*nb*nb,    A,                     INOUT | LOCALITY,
        sizeof(int),                        &lda,           VALUE,
        sizeof(int)*nb,                      IPIV,                  OUTPUT,
        sizeof(PLASMA_sequence*),           &sequence,      VALUE,
        sizeof(PLASMA_request*),            &request,       VALUE,
        sizeof(PLASMA_bool),                &check_info,    VALUE,
        sizeof(int),                        &iinfo,         VALUE,
        0);
}

/***************************************************************************//**
 *
 **/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_zgetrf_quark = PCORE_zgetrf_quark
#define CORE_zgetrf_quark PCORE_zgetrf_quark
#endif
void CORE_zgetrf_quark(Quark *quark)
{
    int m;
    int n;
    PLASMA_Complex64_t *A;
    int lda;
    int *IPIV;
    PLASMA_sequence *sequence;
    PLASMA_request *request;
    PLASMA_bool check_info;
    int iinfo;
    int info;

    quark_unpack_args_9(quark, m, n, A, lda, IPIV, sequence, request, check_info, iinfo);
    info = LAPACKE_zgetrf_work(LAPACK_COL_MAJOR, m, n, A, lda, IPIV );
    if (info != PLASMA_SUCCESS) {
        if (check_info || info < 0)
            plasma_sequence_flush(quark, sequence, request, iinfo+info);
        else{
            int i;
            for(i=info-1; i<min(m,n); i++)
                IPIV[i] = i+1;
        }
    }
}
