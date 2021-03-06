/*******************************************************************************
!                              INTEL CONFIDENTIAL
!   Copyright(C) 2006-2008 Intel Corporation. All Rights Reserved.
!   The source code contained  or  described herein and all documents related to
!   the source code ("Material") are owned by Intel Corporation or its suppliers
!   or licensors.  Title to the  Material remains with  Intel Corporation or its
!   suppliers and licensors. The Material contains trade secrets and proprietary
!   and  confidential  information of  Intel or its suppliers and licensors. The
!   Material  is  protected  by  worldwide  copyright  and trade secret laws and
!   treaty  provisions. No part of the Material may be used, copied, reproduced,
!   modified, published, uploaded, posted, transmitted, distributed or disclosed
!   in any way without Intel's prior express written permission.
!   No license  under any  patent, copyright, trade secret or other intellectual
!   property right is granted to or conferred upon you by disclosure or delivery
!   of the Materials,  either expressly, by implication, inducement, estoppel or
!   otherwise.  Any  license  under  such  intellectual property  rights must be
!   express and approved by Intel in writing.
!
!*******************************************************************************
!   Content:
!       MKL DFTI interface example program (C-interface)
!
!       Real-to-complex and complex-to-real 2D transform
!       for single precision data not inplace
!       which are allocated in three-dimension array
!
!       Perform multiple two-dimensional transform along the 1-st dimension of 3D array
!       Note that data are not unit-stride
!
!       CCS packed format for complex conjugate-symmetric data
!
!*******************************************************************************
!  Configuration parameters:
!           DFTI_FORWARD_DOMAIN = DFTI_REAL                 (obligatory)
!           DFTI_PRECISION      = DFTI_SINGLE               (obligatory)
!           DFTI_DIMENSION      = 2                         (obligatory)
!           DFTI_LENGTHS        = { m, n}                   (obligatory)
!           DFTI_PACKED_FORMAT  = DFTI_CCS_FORMAT           (default)
!           DFTI_PLACEMENT      = DFTI_NOT_INPLACE          (default = DFTI_INPLACE)
!           DFTI_INPUT_STRIDES  = {first_index, stride_in_m, stride_in_n}   (default={0, n, 1})
!           DFTI_NUMBER_OF_TRANSFORMS   = multiple                          (default = 1)
!           DFTI_INPUT_DISTANCE         = dist_in    (obligatory, if NUMBER_OF_TRANSFORMS >1)
!
!           DFTI_FORWARD_SCALE  = 1.0                       (default)
!           DFTI_BACKWARD_SCALE = 1.0/(float)(m*n)         (default=1.0)
!
!           DFTI_REAL_STORAGE   =  DFTI_REAL_REAL           (default)
!           DFTI_CONJUGATE_EVEN_STORAGE = DFTI_COMPLEX_REAL (default)
!
! Other default configuration parameters are in the mkl_dfti.h interface file
!*******************************************************************************
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mkl_dfti.h"

#include "mkl_dfti_examples.h"

int main(int argc, char *argv[])  /* REAL_2D_CCS_SINGLE_EX4 */
{
    float  x_in [M_MAX][N_MAX][K_MAX];
    float  x_out[M_MAX][N_MAX+2][K_MAX+2];
    float  x_exp[M_MAX][N_MAX][K_MAX];

    DFTI_DESCRIPTOR_HANDLE Desc_Handle = 0;
    MKL_LONG m;
    MKL_LONG n;
    MKL_LONG first_index;
    MKL_LONG multiple;
    MKL_LONG dist_in;
    MKL_LONG dist_out;

    MKL_LONG Status;
    float   Scale;
    MKL_LONG lengths[2];
    MKL_LONG strides_in[3];
    MKL_LONG strides_out[3];

    float  maxerr;
    float  eps = SINGLE_EPS;
    MKL_LONG i;

    /*
    /   Read input data from input file
    /   n - size of transform
    */
    if(read_data_file_2d_mult(argc, argv, &m, &n, &first_index, &multiple)) return 1;

    /*
    /  Put transform parameters
    */
    lengths[0] = m;
    lengths[1] = n;

    dist_in  = N_MAX*K_MAX;
    dist_out = (N_MAX+2)*(K_MAX+2);

    strides_in[0] = first_index;
    strides_in[1] = K_MAX;
    strides_in[2] = 1;

    strides_out[0] = first_index;
    strides_out[1] = K_MAX+2;
    strides_out[2] = 1;

    if(LEGEND_PRINT) {
        printf(" \n\n REAL_2D_CCS_SINGLE_EX4                \n");
        printf(" Forward-Backward 2D real transform for single precision data\n\n");
        printf(" Multiple 2D transform along the 1-st dimension of 3D array             \n");
        printf(" Configuration parameters:                  \n\n");
        printf(" DFTI_FORWARD_DOMAIN = DFTI_REAL            \n");
        printf(" DFTI_PRECISION      = DFTI_SINGLE          \n");
        printf(" DFTI_DIMENSION      = 2                    \n");
        printf(" DFTI_LENGTHS        = {%d,%d)              \n", m, n);
        printf(" DFTI_PACKED_FORMAT  = DFTI_CCS_FORMAT      \n");
        printf(" DFTI_PLACEMENT      = DFTI_NOT_INPLACE     \n");
        printf(" DFTI_INPUT_STRIDES  = {%d, %d, %d}\n", first_index,
                                        strides_in[1], strides_in[2]);
        printf(" DFTI_FORWARD_SCALE  = 1.0                  \n");
        printf(" DFTI_BACKWARD_SCALE = 1.0/(float)(m*n)    \n\n");

        printf(" DFTI_NUMBER_OF_TRANSFORMS  = %d         \n", multiple);
        if(multiple > 1)
        printf(" DFTI_INPUT_DISTANCE        = %d         \n", dist_in);
   }

    /*
    /   Check test input parameters
    */
    if((m > N_MAX)||(n > K_MAX)) {
        printf(" Error input parameters: (m > N_MAX)||(n > K_MAX) \n");
        printf(" Please see mkl_dfti_examples.h file\n");
        printf(" TEST FAIL\n");  goto END_OF_TEST;
    }

    if( first_index + multiple*m*n > M_MAX*N_MAX*K_MAX) {
        printf(" Error input parameters: first_index + multiple*m*n > M_MAX*N_MAX*K_MAX \n");
        printf(" Please see mkl_dfti_examples.h file\n");
        printf(" TEST FAIL\n");  goto END_OF_TEST;
    }

    /*
    /   put input data and expected result
    */
    zero_init_s(x_in,  M_MAX*N_MAX*K_MAX);
    zero_init_s(x_out, M_MAX*(N_MAX+2)*(K_MAX+2));

    init_real_mult_2d_s(x_in, m, n, multiple, dist_in, strides_in);

    cblas_scopy(M_MAX*N_MAX*K_MAX, (const float*)x_in, 1, (float*)x_exp, 1);

    if(ADVANCED_DATA_PRINT){
        printf("\n INPUT X, 3 first columns \n");
        for(i=0; i< multiple; i++) {
            printf("\n Transform Serial=%d \n", i);
            print_three_2d_columns_s(&x_in[i][0][0], m, n, strides_in);
        }
    }

    /*
    /   Create Dfti descriptor for 2D single precision  transform
    */
    Status = DftiCreateDescriptor( &Desc_Handle, DFTI_SINGLE,
                                    DFTI_REAL, 2, lengths);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n");  goto END_OF_TEST;
    }

    /*
    /   Set placement of result DFTI_NOT_INPLACE
    */
    Status = DftiSetValue( Desc_Handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n");  goto FREE_DESCRIPTOR;
    }

    /*
    /   Set Forward transform parameters for multiple transform mode
    */
    if(multiple > 1){
        Status = DftiSetValue( Desc_Handle, DFTI_NUMBER_OF_TRANSFORMS, multiple );
        if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
            dfti_example_status_print(Status);
            printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
        }

        Status = DftiSetValue( Desc_Handle, DFTI_INPUT_DISTANCE, dist_in);
        if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
            dfti_example_status_print(Status);
            printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
        }

        Status = DftiSetValue( Desc_Handle, DFTI_OUTPUT_DISTANCE, dist_out);
        if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
            dfti_example_status_print(Status);
            printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
        }
    }

    /*
    /    Set Forward transform strides parameters
    */

    Status = DftiSetValue(Desc_Handle, DFTI_INPUT_STRIDES, strides_in);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    Status = DftiSetValue(Desc_Handle, DFTI_OUTPUT_STRIDES, strides_out);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    /*
    /   Commit Dfti descriptor
    */
    Status = DftiCommitDescriptor( Desc_Handle );
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    /*
    /   Compute Forward transform
    */
    printf("\n Compute DftiComputeForward\n\n");
    Status = DftiComputeForward( Desc_Handle, x_in, x_out);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    if(ADVANCED_DATA_PRINT){
        printf("\n Forward result X, 3 first columns and 3 last columns \n\n");
        for(i=0; i< multiple; i++) {
            printf("\n Transform Serial=%d \n", i);
            print_three_first_and_three_last_columns_s(&x_out[i][0][0], m+2, n+2, strides_out);
        }
    }

    /*
    /   Set Scale number for Backward transform
    */
    Scale = 1.0/(float)(m*n);
    printf(" \n\n DFTI_BACKWARD_SCALE  = 1/(m*n) \n");

    Status = DftiSetValue(Desc_Handle, DFTI_BACKWARD_SCALE, Scale);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    /*
    /   Set Backward transform parameters for multiple transform mode
    */
    if(multiple > 1){

        Status = DftiSetValue( Desc_Handle, DFTI_INPUT_DISTANCE, dist_out);
        if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
            dfti_example_status_print(Status);
            printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
        }

        Status = DftiSetValue( Desc_Handle, DFTI_OUTPUT_DISTANCE, dist_in);
        if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
            dfti_example_status_print(Status);
            printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
        }
    }

    /*
    /    Set Backward transform strides parameters
    */

    Status = DftiSetValue(Desc_Handle, DFTI_INPUT_STRIDES, strides_out);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    Status = DftiSetValue(Desc_Handle, DFTI_OUTPUT_STRIDES, strides_in);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    /*
    /   Commit Dfti descriptor
    */
    Status = DftiCommitDescriptor( Desc_Handle );
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    /*
    /   Compute Backward transform
    */
    printf("\n Compute DftiComputeBackward\n\n");
    Status = DftiComputeBackward( Desc_Handle, x_out, x_in);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
    }

    if(ADVANCED_DATA_PRINT){
        printf("\n Backward result X, 3 first columns \n\n");
        for(i=0; i< multiple; i++) {
            printf("\n Transform Serial=%d \n", i);
            print_three_2d_columns_s(&x_in[i][0][0], m, n, strides_in);
        }
    }

    /*
    /   Check result
    */
    maxerr = check_result_mult_2d_s(x_in, x_exp, m, n, multiple, dist_in, strides_in);

    if(ACCURACY_PRINT)
        printf("\n ACCURACY = %g\n\n", maxerr);

    if(maxerr < eps){
        printf(" TEST PASSED\n");
    } else {
        printf(" TEST FAIL\n");
    }

    /*
    /   Free DFTI descriptor
    */
FREE_DESCRIPTOR:
    Status = DftiFreeDescriptor(&Desc_Handle);
    if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
        dfti_example_status_print(Status);
        printf(" TEST FAIL\n");
    }

END_OF_TEST:
    return 0;
}


