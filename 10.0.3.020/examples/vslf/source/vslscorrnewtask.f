!*******************************************************************************
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
!    vslsCorrNewTask  Example Program Text
!*******************************************************************************

      INCLUDE 'mkl_vsl.fi'

      PROGRAM MKL_VSL_TEST
      USE MKL_VSL_TYPE
      USE MKL_VSL
      IMPLICIT NONE
!
      TYPE(VSL_CORR_TASK) task
      INTEGER(KIND=4) status
      INTEGER mode,rank
      INTEGER xshape(1),yshape(1),zshape(1)
!
      PRINT *, 'EXAMPLE creating a new task'
!
      mode = VSL_CORR_MODE_AUTO
      rank = 1
      xshape(1) = 100
      yshape(1) = 1000
      zshape(1) = (xshape(1)-1) + (yshape(1)-1) + 1
      status = vslscorrnewtask(task,mode,rank,xshape,yshape,zshape)
!
      IF (status .NE. VSL_STATUS_OK) THEN
         PRINT *, 'ERROR: bad status: ',status
         PRINT *, 'EXAMPLE FAILED'
         STOP 1
      ELSE
         PRINT *, 'EXAMPLE PASSED'
         STOP
      END IF
!
      END
