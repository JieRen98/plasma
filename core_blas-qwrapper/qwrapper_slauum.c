/**
 *
 * @file qwrapper_spotrf.c
 *
 *  PLASMA core_blas quark wrapper
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @author Jakub Kurzak
 * @date 2010-11-15
 * @generated s Fri Apr  1 11:02:40 2016
 *
 **/
#include <lapacke.h>
#include "common.h"

/***************************************************************************//**
 *
 **/
void QUARK_CORE_slauum(Quark *quark, Quark_Task_Flags *task_flags,
                       PLASMA_enum uplo, int n, int nb,
                       float *A, int lda)
{
    plasma_profile_by_kernel( task_flags, LAUUM );

    QUARK_Insert_Task(quark, CORE_slauum_quark, task_flags,
        sizeof(PLASMA_enum),                &uplo,  VALUE,
        sizeof(int),                        &n,     VALUE,
        sizeof(float)*nb*nb,    A,             INOUT,
        sizeof(int),                        &lda,   VALUE,
        0);
}

/***************************************************************************//**
 *
 **/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_slauum_quark = PCORE_slauum_quark
#define CORE_slauum_quark PCORE_slauum_quark
#endif
void CORE_slauum_quark(Quark *quark)
{
    PLASMA_enum uplo;
    int N;
    float *A;
    int LDA;

    quark_unpack_args_4(quark, uplo, N, A, LDA);
    LAPACKE_slauum_work(LAPACK_COL_MAJOR, lapack_const(uplo), N, A, LDA);
}
