      SUBROUTINE DQRD(M, N, A, ALFA, PIVOT)
      INTEGER M, N
      INTEGER PIVOT(N)
      DOUBLE PRECISION A(M, N), ALFA(N)
      COMMON /CSTAK/ DS
      DOUBLE PRECISION DS(500)
      INTEGER ISTKGT, MIN0, I, J, K, JBAR
      INTEGER ISUM, IS(1000)
      REAL RS(1000), FLOAT
      LOGICAL LS(1000)
      DOUBLE PRECISION AKK, EPS, RNDING, DBLE, BETA, DDOT
      DOUBLE PRECISION ALFAK, SIGMA, WS(500), D1MACH, DSQRT
      INTEGER TEMP, TEMP1
      EQUIVALENCE (DS(1), WS(1), RS(1), IS(1), LS(1))
      DATA RNDING/0D0/
C TO OBTAIN THE QR DECOMPOSITION OF A MATRIX.
C MNEMONIC - DOUBLE PRECISION QR DECOMPOSITION OF A MATRIX.
C INPUT -
C   M - THE NUMBER OF ROWS IN THE MATRIX A.
C   N - THE NUMBER OF COLUMNS IN THE MATRIX A.
C   A - THE MATRIX.
C OUTPUT -
C   A     - THE UPPER TRIANGULAR PORTION OF A ABOVE THE DIAGONAL
C           IS THE R OF THE QR DECOMPOSITION, WITH THE DIAGONAL
C           ELEMENTS OF R IN ALFA.
C           THE LOWER TRIANGULAR PORTION OF A STORES THE Q IN
C           FACTORED HOUSEHOLDER FORM. Q*A = R.
C   ALFA  - THE DIAGONAL OF R.
C   PIVOT - PIVOT(J) IS THE POSITION OF THE J-TH VARIABLE, J = 1,...,N,
C           CHOSEN SO THAT THE MAXIMAL COLUMN IS ELIMINATED AT EACH
C           STEP.
C SCRATCH SPACE ALLOCATED - N*MU WORDS.
C ERROR STATES -
C   1 - M.LT.1.
C   2 - N.LT.1.
C   3 - SINGULAR MATRIX. (RECOVERABLE)
C ALFA(MIN(M,N)).
C THE PORT LIBRARY STACK AND ITS ALIASES.
C DEFINE SUM(J) WS(ISUM-1+J)
C CHECK THE INPUT FOR ERRORS.
C/6S
C     IF (M .LT. 1) CALL SETERR(13HDQRD - M.LT.1, 13, 1, 2)
C     IF (N .LT. 1) CALL SETERR(13HDQRD - N.LT.1, 13, 2, 2)
C/7S
      IF (M .LT. 1) CALL SETERR('DQRD - M.LT.1', 13, 1, 2)
      IF (N .LT. 1) CALL SETERR('DQRD - N.LT.1', 13, 2, 2)
C/
      IF (RNDING .EQ. 0D0) RNDING = D1MACH(4)
      EPS = (RNDING*DBLE(FLOAT(MIN0(M, N)**3)))**2
      ISUM = ISTKGT(N, 4)
      J = 1
         GOTO  2
   1     J = J+1
   2     IF (J .GT. N) GOTO  3
C GET THE L2 NORM OF THE J-TH COLUMN.
         TEMP1 = ISUM-1+J
         WS(TEMP1) = DDOT(M, A(1, J), 1, A(1, J), 1)
         PIVOT(J) = J
         GOTO  1
   3  K = 1
         GOTO  5
   4     K = K+1
   5     IF (K .GT. MIN0(M, N)) GOTO  19
C ELIMINATE K-TH COLUMN.
         TEMP1 = ISUM-1+K
         SIGMA = WS(TEMP1)
C FIND THE PIVOT COLUMN.
         JBAR = K
         J = K+1
            GOTO  7
   6        J = J+1
   7        IF (J .GT. N) GOTO  9
            TEMP1 = ISUM-1+J
            IF (SIGMA .GE. WS(TEMP1)) GOTO 8
               TEMP = ISUM-1+J
               SIGMA = WS(TEMP)
               JBAR = J
   8        CONTINUE
            GOTO  6
   9     IF (JBAR .EQ. K) GOTO 11
            I = PIVOT(K)
C NEED TO INTERCHANGE THE COLUMNS.
            PIVOT(K) = PIVOT(JBAR)
            PIVOT(JBAR) = I
            TEMP1 = ISUM-1+JBAR
            TEMP = ISUM-1+K
            WS(TEMP1) = WS(TEMP)
            TEMP = ISUM-1+K
            WS(TEMP) = SIGMA
            DO  10 I = 1, M
               SIGMA = A(I, K)
               A(I, K) = A(I, JBAR)
               A(I, JBAR) = SIGMA
  10           CONTINUE
  11     SIGMA = DDOT(M-K+1, A(K, K), 1, A(K, K), 1)
         IF (SIGMA .GT. EPS*WS(ISUM)) GOTO 12
C/6S
C           CALL SETERR(22HDQRD - SINGULAR MATRIX, 22, 3, 1)
C/7S
            CALL SETERR('DQRD - SINGULAR MATRIX', 22, 3, 1)
C/
            GOTO  19
  12     AKK = A(K, K)
         IF (AKK .GE. 0D0) GOTO 13
            ALFAK = DSQRT(SIGMA)
            GOTO  14
  13        ALFAK = -DSQRT(SIGMA)
  14     ALFA(K) = ALFAK
         BETA = 1D0/(SIGMA-AKK*ALFAK)
         A(K, K) = AKK-ALFAK
         J = K+1
            GOTO  16
  15        J = J+1
  16        IF (J .GT. N) GOTO  18
            SIGMA = BETA*DDOT(M+1-K, A(K, K), 1, A(K, J), 1)
            DO  17 I = K, M
               A(I, J) = A(I, J)-A(I, K)*SIGMA
  17           CONTINUE
            TEMP = ISUM-1+J
            WS(TEMP) = WS(TEMP)-A(K, J)**2
            GOTO  15
  18     CONTINUE
         GOTO  4
  19  CALL ISTKRL(1)
      RETURN
      END
