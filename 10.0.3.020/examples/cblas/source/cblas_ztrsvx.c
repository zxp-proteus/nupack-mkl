/*******************************************************************************
!                             INTEL CONFIDENTIAL
!   Copyright(C) 1999-2008 Intel Corporation. All Rights Reserved.
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
!  Content:
!      C B L A S _ Z T R S V  Example Program Text ( C Interface )
!******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "mkl_example.h"

int main(int argc, char *argv[])
{
      FILE *in_file;
      char *in_file_name;

      MKL_INT          n, lda, incx;
      MKL_INT          rmaxa, cmaxa;
      MKL_Complex16   *a, *x;
      CBLAS_ORDER      order;
      CBLAS_UPLO       uplo;
      CBLAS_TRANSPOSE  trans;
      CBLAS_DIAG       diag;
      MKL_INT          len_x, typeA;

      printf("\n     C B L A S _ Z T R S V  EXAMPLE PROGRAM\n");

/*       Get input parameters                                  */

      if( argc == 1 ) {
         printf("\n You must specify in_file data file as 1-st parameter");
         return 0;
      }
      in_file_name = argv[1];

/*       Get input data                                        */

      if( (in_file = fopen( in_file_name, "r" )) == NULL ) {
         printf("\n ERROR on OPEN '%s' with mode=\"r\"\n", in_file_name);
         return 0;
      }
      if( GetIntegerParameters(in_file, &n) != 1 ) {
          printf("\n ERROR of n reading\n");
          return 0;
      }
      if( GetIntegerParameters(in_file, &incx) != 1 ) {
          printf("\n ERROR of incx reading\n");
          return 0;
      }
      if( GetCblasCharParameters(in_file, &uplo, &trans, &diag, &order) != 4 ) {
          printf("\n ERROR of uplo, trans, diag, order reading\n");
          return 0;
      }

      rmaxa = n + 1;
      cmaxa = n;
      len_x = 1+(n-1)*abs(incx);
      a = (MKL_Complex16 *)calloc( rmaxa*cmaxa, sizeof(MKL_Complex16) );
      x = (MKL_Complex16 *)calloc( len_x, sizeof(MKL_Complex16) );
      if( a == NULL || x == NULL ) {
          printf("\n Can't allocate memory for arrays\n");
          return 0;
      }
      if( order == CblasRowMajor )
         lda=cmaxa;
      else
         lda=rmaxa;

      if( GetVectorZ(in_file, n, x, incx) != len_x ) {
        printf("\n ERROR of vector X reading\n");
        return 0;
      }
      if (uplo == CblasUpper) typeA = UPPER_MATRIX;
      else                    typeA = LOWER_MATRIX;
      if( GetArrayZ(in_file, &order, typeA, &n, &n, a, &lda) != 0 ) {
        printf("\n ERROR of array A reading\n");
        return 0;
      }
      fclose(in_file);

/*       Print input data                                      */

      printf("\n     INPUT DATA");
      printf("\n       N=%d", n);
      PrintParameters("UPLO, DIAG", uplo, diag);
      PrintParameters("TRANS", trans);
      PrintParameters("ORDER", order);
      PrintVectorZ(FULLPRINT, n, x, incx, "X");
      PrintArrayZ(&order, FULLPRINT, typeA, &n, &n, a, &lda, "A");

/*      Call CBLAS_ZTRSV subroutine ( C Interface )            */

      cblas_ztrsv(order, uplo, trans, diag, n, a, lda, x, incx);

/*       Print output data                                     */

      printf("\n\n     OUTPUT DATA");
      PrintVectorZ(FULLPRINT, n, x, incx, "X");

      free(a);
      free(x);

      return 0;
}
