/*******************************************************************************
!                              INTEL CONFIDENTIAL
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
!   function    : fftw_plan_dft_c2r_1d - interface FFTW initialization function
!   Content     : wrapper from this function to DFTI MKL initialization functions
!                 for double precision complex data
!   input data  : parameters for DFT
!   output data : pointer to FFTW plan
!*******************************************************************************
*/

#include "fftw3.h"
#include "fftw3_mkl.h"

fftw_plan fftw_plan_dft_c2r_1d(int n, fftw_complex *in, double *out, unsigned flags) {

    MKL_LONG Status;
    fftw_plan_mkl*  fftw_desc_mkl;
    fftw_desc_mkl = (fftw_plan_mkl*)malloc(sizeof(fftw_plan_mkl));
    if (fftw_desc_mkl == NULL) return NULL;

    fftw_desc_mkl->in = (void*)in;
    fftw_desc_mkl->out = (void*)out;
    fftw_desc_mkl->tt_fftw_mkl = NULL;
    fftw_desc_mkl->sign = FFTW_BACKWARD;

    Status = DftiCreateDescriptor(&(fftw_desc_mkl->mkl_desc), DFTI_DOUBLE, DFTI_REAL, 1, (MKL_LONG)n);
    if (! DftiErrorClass(Status, DFTI_NO_ERROR)) {
        free(fftw_desc_mkl);
        return NULL;
    }

    Status = DftiSetValue( fftw_desc_mkl->mkl_desc, DFTI_CONJUGATE_EVEN_STORAGE, DFTI_COMPLEX_COMPLEX);
    if (! DftiErrorClass(Status, DFTI_NO_ERROR)) {
        Status = DftiFreeDescriptor(&(fftw_desc_mkl->mkl_desc));
        free(fftw_desc_mkl);
        return NULL;
	}

	if ((double*)in != out) {
        Status = DftiSetValue( fftw_desc_mkl->mkl_desc, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
        if (! DftiErrorClass(Status, DFTI_NO_ERROR)) {
            Status = DftiFreeDescriptor(&(fftw_desc_mkl->mkl_desc));
            free(fftw_desc_mkl);
            return NULL;
		}
	}

    Status = DftiCommitDescriptor(fftw_desc_mkl->mkl_desc);
    if (! DftiErrorClass(Status, DFTI_NO_ERROR)) {
        free(fftw_desc_mkl);
        Status = DftiFreeDescriptor(&(fftw_desc_mkl->mkl_desc));
        return NULL;
    }
    return (fftw_plan)fftw_desc_mkl;
}
