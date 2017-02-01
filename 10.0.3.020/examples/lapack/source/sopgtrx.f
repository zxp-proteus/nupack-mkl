*     SOPGTR (F08GFE) Example Program Text
*     Mark 16 Release. NAG Copyright 1992.
*     .. Parameters ..
      INTEGER          NIN, NOUT
      PARAMETER        (NIN=5,NOUT=6)
      INTEGER          NMAX, LDZ
      PARAMETER        (NMAX=8,LDZ=NMAX)
*     .. Local Scalars ..
      INTEGER          I, IFAIL, INFO, J, N
      CHARACTER        UPLO
*     .. Local Arrays ..
      REAL             AP(NMAX*(NMAX+1)/2), D(NMAX), E(NMAX), TAU(NMAX),
     +                 WORK(2*NMAX-2), Z(LDZ,NMAX)
*     .. External Subroutines ..
      EXTERNAL         SOPGTR, SSPTRD, SSTEQR, X04CAE
*     .. Executable Statements ..
      WRITE (NOUT,*) 'SOPGTR Example Program Results'
*     Skip heading in data file
      READ (NIN,*)
      READ (NIN,*) N
      IF (N.LE.NMAX) THEN
*
*        Read A from data file
*
         READ (NIN,*) UPLO
         IF (UPLO.EQ.'U') THEN
            READ (NIN,*) ((AP(I+J*(J-1)/2),J=I,N),I=1,N)
         ELSE IF (UPLO.EQ.'L') THEN
            READ (NIN,*) ((AP(I+(2*N-J)*(J-1)/2),J=1,I),I=1,N)
         END IF
*
*        Reduce A to tridiagonal form T = (Q**T)*A*Q
*
         CALL SSPTRD(UPLO,N,AP,D,E,TAU,INFO)
*
*        Form Q explicitly, storing the result in Z
*
         CALL SOPGTR(UPLO,N,AP,TAU,Z,LDZ,WORK,INFO)
*
*        Calculate all the eigenvalues and eigenvectors of A
*
         CALL SSTEQR('V',N,D,E,Z,LDZ,WORK,INFO)
*
         WRITE (NOUT,*)
         IF (INFO.GT.0) THEN
            WRITE (NOUT,*) 'Failure to converge.'
         ELSE
*
*           Print eigenvalues and eigenvectors
*
            WRITE (NOUT,*) 'Eigenvalues'
            WRITE (NOUT,99999) (D(I),I=1,N)
            WRITE (NOUT,*)
            IFAIL = 0
*
            CALL X04CAE('General',' ',N,N,Z,LDZ,'Eigenvectors',IFAIL)
*
         END IF
      END IF
      STOP
*
99999 FORMAT (3X,(8F8.4))
      END