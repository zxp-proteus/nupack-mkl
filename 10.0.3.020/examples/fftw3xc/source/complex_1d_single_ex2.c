/*******************************************************************************
!                             INTEL CONFIDENTIAL
!   Copyright(C) 2005-2008 Intel Corporation. All Rights Reserved.
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
!          MKL DFTI implementation through FFTW interface (via wrappers) example
!          program (C-interface)
!
!   Complex-to-complex 1D transform for single precision data not inplace.
!
!   Configuration parameters for MKL DFTI:
!           DFTI_FORWARD_DOMAIN = DFTI_COMPLEX  	 (obligatory)
!           DFTI_PRECISION      = DFTI_SINGLE   	 (obligatory)
!           DFTI_DIMENSION      = 1             	 (obligatory)
!           DFTI_LENGTHS        = n             	 (obligatory)
! 	    DFTI_PLACEMENT      = DFTI_NOT_INPLACE       (default= DFTI_INPLACE)
!           DFTI_FORWARD_SCALE  = 1.0           	 (default)
!           DFTI_BACKWARD_SCALE = 1.0/(float)n  	 (default=1.0)
!
!  Other default configuration parameters are in the mkl_dfti.h interface file
!******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "fftw3.h"

#include "mkl_dfti_examples.h"

int main()  /* COMPLEX_1D_SINGLE_EX2 */
{
    /*
    /   DFT input parameters
    */
    long     n=5;
    fftwf_plan Desc_Handle;
    fftwf_complex* x_in;
    fftwf_complex* x_out;
    /* */
    fftwf_complex* x_exp;
    float  Scale;
    float  maxerr;
    float  eps= SINGLE_EPS;

    if(LEGEND_PRINT) {
        printf(" \n\n COMPLEX_1D_SINGLE_EX2 \n");
        printf(" Forward-Backward 1D complex transform for single precision data\n\n");
        printf(" Configuration parameters:            \n\n");
        printf(" DFTI_FORWARD_DOMAIN = DFTI_COMPLEX   \n");
        printf(" DFTI_PRECISION      = DFTI_SINGLE    \n");
        printf(" DFTI_DIMENSION      = 1              \n");
        printf(" DFTI_LENGTHS        = %d             \n", n);
        printf(" DFTI_PLACEMENT      = DFTI_NOT_INPLACE   \n");
        printf(" DFTI_FORWARD_SCALE  = 1.0            \n");
        printf(" DFTI_BACKWARD_SCALE = 1.0/(float)n   \n\n");
    }

    /*
    /   Allocate array for input data
    */
    x_in  = (fftwf_complex*)malloc(2*n*sizeof(float));
    x_out = (fftwf_complex*)malloc(2*n*sizeof(float));
    x_exp  = (fftwf_complex*)malloc(2*n*sizeof(float));
    /*
    /   initialize x_in and copy to expected x_exp
    */
    init_input_and_expected_vectors_c(x_in, x_exp, n);

    /*
    /   Create Dfti descriptor for 1D single precision forward transform
    */
    Desc_Handle = fftwf_plan_dft_1d( n, x_in, x_out , FFTW_FORWARD, FFTW_ESTIMATE);
    /*
    /   Compute DFT
    */
    fftwf_execute(Desc_Handle);
    /*
    /   Destroy Dfti descriptor
    */
    fftwf_destroy_plan(Desc_Handle);

    /*
    /   Set Scale number for Backward transform
    */
    Scale = 1.0/(float)n;
    /*
    /   Create Dfti descriptor for 1D single precision backward transform
    */
    Desc_Handle = fftwf_plan_dft_1d( n, x_out , x_in, FFTW_BACKWARD, FFTW_ESTIMATE);
    /*
    /   Compute DFT
    */
    fftwf_execute(Desc_Handle);
    /*
    /   Destroy Dfti descriptor
    */
    fftwf_destroy_plan(Desc_Handle);
    /*
    /   Result scaling
    */
    scaling_f(x_in, Scale, n);

    /*
    /   Check result
    */
    maxerr = check_result_c(x_in, x_exp, n);

    if(ACCURACY_PRINT)
        printf("\n ACCURACY = %g\n\n", maxerr);

    if(maxerr < eps){
        printf(" TEST PASSED\n");
    } else {
        printf(" TEST FAIL\n");
    }

    free(x_exp);
    free(x_in);
    free(x_out);

    return 0;
}