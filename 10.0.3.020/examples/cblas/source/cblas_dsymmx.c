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
!      C B L A S _ D S Y M M  Example Program Text ( C Interface )
!******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "mkl_example.h"

int main(int argc, char *argv[])
{
      FILE *in_file;
      char *in_file_name;

      MKL_INT         m, n;
      MKL_INT         lda, ldb, ldc;
      MKL_INT         rmaxa, cmaxa, rmaxb, cmaxb, rmaxc, cmaxc;
      double          alpha, beta;
      double         *a, *b, *c;
      CBLAS_ORDER     order;
      CBLAS_SIDE      side;
      CBLAS_UPLO      uplo;
      MKL_INT         ma, na, typeA;

      printf("\n     C B L A S _ D S Y M M  EXAMPLE PROGRAM\n");

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
      if( GetIntegerParameters(in_file, &m, &n) != 2 ) {
          printf("\n ERROR of m, n reading\n");
          return 0;
      }
      if( GetScalarsD(in_file, &alpha, &beta) != 2 ) {
          printf("\n ERROR of alpha, beta reading\n");
          return 0;
      }
      if( GetCblasCharParameters(in_file, &side, &uplo, &order) != 3 ) {
          printf("\n ERROR of side, uplo, order reading\n");
          return 0;
      }

      if( side == CblasLeft ) {
          rmaxa = m + 1;
          cmaxa = m;
          ma    = m;
          na    = m;
      } else {
          rmaxa = n + 1;
          cmaxa = n;
          ma    = n;
          na    = n;
      }
      rmaxb = m + 1;
      cmaxb = n;
      rmaxc = m + 1;
      cmaxc = n;
      a = (double *)calloc( rmaxa*cmaxa, sizeof( double ) );
      b = (double *)calloc( rmaxb*cmaxb, sizeof( double ) );
      c = (double *)calloc( rmaxc*cmaxc, sizeof( double ) );
      if( a == NULL || b == NULL || c == NULL ) {
          printf( "\n Can't allocate memory for arrays\n");
          return 0;
      }
      if( order == CblasRowMajor ) {
         lda=cmaxa;
         ldb=cmaxb;
         ldc=cmaxc;
      } else {
         lda=rmaxa;
         ldb=rmaxb;
         ldc=rmaxc;
      }
      if (uplo == CblasUpper) typeA = UPPER_MATRIX;
      else                    typeA = LOWER_MATRIX;

      if( GetArrayD(in_file, &order, typeA, &ma, &na, a, &lda) != 0 ) {
        printf("\n ERROR of array A reading\n");
        return 0;
      }
      if( GetArrayD(in_file, &order, GENERAL_MATRIX, &m, &n, b, &ldb) != 0 ) {
        printf("\n ERROR of array B reading\n");
        return 0;
      }
      if( GetArrayD(in_file, &order, GENERAL_MATRIX, &m, &n, c, &ldc) != 0 ) {
        printf("\n ERROR of array C reading\n");
        return 0;
      }
      fclose(in_file);

/*       Print input data                                      */

      printf("\n     INPUT DATA");
      printf("\n       M=%d  N=%d", m, n);
      printf("\n       ALPHA=%6.2f  BETA=%6.2f", alpha, beta);
      PrintParameters("SIDE, UPLO", side, uplo);
      PrintParameters("ORDER", order);
      PrintArrayD(&order, FULLPRINT, typeA, &ma, &na, a, &lda, "A");
      PrintArrayD(&order, FULLPRINT, GENERAL_MATRIX, &m, &n, b, &ldb, "B");
      PrintArrayD(&order, FULLPRINT, GENERAL_MATRIX, &m, &n, c, &ldc, "C");

/*      Call CBLAS_DSYMM subroutine ( C Interface )            */

      cblas_dsymm(order, side, uplo, m, n, alpha, a, lda,
                  b, ldb, beta, c, ldc);

/*       Print output data                                     */

      printf("\n\n     OUTPUT DATA");
      PrintArrayD(&order, FULLPRINT, GENERAL_MATRIX, &m, &n, c, &ldc, "C");

      free(a);
      free(b);
      free(c);

      return 0;
}

