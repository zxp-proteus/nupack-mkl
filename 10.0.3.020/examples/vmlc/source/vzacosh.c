/*******************************************************************************
!                             INTEL CONFIDENTIAL
!  Copyright(C) 2001-2008 Intel Corporation. All Rights Reserved.
!  The source code contained  or  described herein and all documents related to
!  the source code ("Material") are owned by Intel Corporation or its suppliers
!  or licensors.  Title to the  Material remains with  Intel Corporation or its
!  suppliers and licensors. The Material contains trade secrets and proprietary
!  and  confidential  information of  Intel or its suppliers and licensors. The
!  Material  is  protected  by  worldwide  copyright  and trade secret laws and
!  treaty  provisions. No part of the Material may be used, copied, reproduced,
!  modified, published, uploaded, posted, transmitted, distributed or disclosed
!  in any way without Intel's prior express written permission.
!  No license  under any  patent, copyright, trade secret or other intellectual
!  property right is granted to or conferred upon you by disclosure or delivery
!  of the Materials,  either expressly, by implication, inducement, estoppel or
!  otherwise.  Any  license  under  such  intellectual property  rights must be
!  express and approved by Intel in writing.
!
!*******************************************************************************
!  Content:
!    vzAcosh  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include <math.h>
#include "func_interv.h"

#include "mkl_vml.h"

int main()
{
  MKL_Complex16 zA[VEC_LEN],zB1[VEC_LEN];

  MKL_INT i=0,vec_len=VEC_LEN;

  for(i=0;i<vec_len;i++) {
    zA[i].real=(double)(__DACOSH_BEG+((__DACOSH_END-__DACOSH_BEG)*i)/vec_len);
    zA[i].imag=(double)(__DACOSH_END-((__DACOSH_END-__DACOSH_BEG)*i)/vec_len);
    zB1[i].real=0.0;
    zB1[i].imag=0.0;
  }

  vzAcosh(vec_len,zA,zB1);

  printf("vzAcosh test/example program\n\n");
  printf("           Argument                           vzAcosh\n");
  printf("===============================================================================\n");
  for(i=0;i<vec_len;i++) {
    printf("   % .10f %+.10f*i      %.10f %+.10f*i\n",zA[i].real,zA[i].imag,zB1[i].real,zB1[i].imag);
  }

  return 0;
}
