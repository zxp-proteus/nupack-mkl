!*******************************************************************************
!                              INTEL CONFIDENTIAL
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
!    vdPow  Example Program Text
!*******************************************************************************

      program MKL_VML_TEST

      include "mkl_vml.fi"
      include "func_interv.fi"

      real(kind=8) dA1(VEC_LEN)
      real(kind=8) dA2(VEC_LEN)
      real(kind=8) dB1(VEC_LEN)
      real(kind=8) dB2(VEC_LEN)
      real(kind=8) CurRMS,MaxRMS
      integer i

      MaxRMS=0.0
      do i=1,VEC_LEN
            dA1(i)=VML_DPOW_BEG+((VML_DPOW_END-VML_DPOW_BEG)*i)/VEC_LEN
            dA2(i)=VML_DPOW_BEG+((VML_DPOW_END-VML_DPOW_BEG)*i)/VEC_LEN
            dB1(i)=0.0
            dB2(i)=dA1(i)**dA2(i)
      end do

      call VDPOW(VEC_LEN,dA1,dA2,dB1)

      print *,"vdPow test/example program"
      print *,""
      print *,""
      print *,"           Argument                     vdPow         ", &
     &        "             Pow"
      print *,"======================================================", &
     &        "========================"
      do i=1,VEC_LEN
            print 10,dA1(i),dA2(i),dB1(i),dB2(i)
            CurRMS=abs((dB1(i)-dB2(i))/(0.5*(dB1(i)+dB2(i))))
            if(MaxRMS<CurRMS) MaxRMS=CurRMS
      end do

      print *,""
      if(MaxRMS>=DEPS) then
            print 11,"Error! Relative accuracy is ",MaxRMS
            stop 1
      else
            print 11,"Relative accuracy is ",MaxRMS
      endif

10    format(F13.6,F13.6,F25.14,F25.14)
11    format(A,F25.14)

      end