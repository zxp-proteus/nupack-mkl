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
!    vsTan  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include <math.h>
#include "func_interv.h"

#include "mkl_vml.h"

int main()
{
  float fA[VEC_LEN],fB1[VEC_LEN],fB2[VEC_LEN];

  MKL_INT i=0,vec_len=VEC_LEN;
  double CurRMS,MaxRMS=0.0;

  for(i=0;i<vec_len;i++) {
    fA[i]=(float)(__STAN_BEG+((__STAN_END-__STAN_BEG)*i)/vec_len);
    fB1[i]=0.0;
    fB2[i]=(float)tan(fA[i]);
  }

  vsTan(vec_len,fA,fB1);

  printf("vsTan test/example program\n\n");
  printf("           Argument                     vsTan                      Tanf\n");
  printf("===============================================================================\n");
  for(i=0;i<vec_len;i++) {
    printf("% 25.14f % 25.14f % 25.14f\n",fA[i],fB1[i],fB2[i]);
    CurRMS=fabs((fB1[i]-fB2[i])/(0.5*(fB1[i]+fB2[i])));
    if(MaxRMS<CurRMS) MaxRMS=CurRMS;
  }
  printf("\n");
  if(MaxRMS>=SEPS) {
    printf("Error! Relative accuracy is %.2f\n",MaxRMS);
    return 1;
  }
  else {
    printf("Relative accuracy is %.2f\n",MaxRMS);
  }

  return 0;
}
