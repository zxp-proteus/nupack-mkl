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
!    vsLn  Example Program Text
!*******************************************************************************

      program MKL_VML_TEST

      include "mkl_vml.fi"
      include "func_interv.fi"

      real(kind=4) sA(VEC_LEN)
      real(kind=4) sB1(VEC_LEN)
      real(kind=4) sB2(VEC_LEN)
      real(kind=4) CurRMS,MaxRMS
      integer i

      MaxRMS=0.0
      do i=1,VEC_LEN
            sA(i)=VML_SLN_BEG+((VML_SLN_END-VML_SLN_BEG)*i)/VEC_LEN
            sB1(i)=0.0
            sB2(i)=log(sA(i))
      end do

      call VSLN(VEC_LEN,sA,sB1)

      print *,"vsLn test/example program"
      print *,""
      print *,""
      print *,"           Argument                     vsLn          ", &
     &        "            Log"
      print *,"======================================================", &
     &        "========================"
      do i=1,VEC_LEN
            print 10,sA(i),sB1(i),sB2(i)
            CurRMS=abs((sB1(i)-sB2(i))/(0.5*(sB1(i)+sB2(i))))
            if(MaxRMS<CurRMS) MaxRMS=CurRMS
      end do

      print *,""
      if(MaxRMS>=SEPS) then
            print 11,"Error! Relative accuracy is ",MaxRMS
            stop 1
      else
            print 11,"Relative accuracy is ",MaxRMS
      endif

10    format(F25.14,F25.14,F25.14)
11    format(A,F25.14)

      end
