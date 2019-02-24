      SUBROUTINE DRCSB(B, M, N, R, C, RC)
      INTEGER M, N
      INTEGER RC(N)
      DOUBLE PRECISION B(M, N), R(M), C(N)
      INTEGER NERROR, I, NERR, IROLD
      DOUBLE PRECISION L, S, D1MACH
      DATA S/0D0/
      DATA L/0D0/
C TO GET THE COLUMN SCALED MATRIX (1/R)*B*(1/C),
C GIVEN THE ROW SCALE FACTOR, AND RETURN THE COLUMN FACTOR.
C/6S
C     IF (M .LT. 1) CALL SETERR(16HDRCSB - M .LT. 1, 16, 1, 2)
C     IF (N .LT. 1) CALL SETERR(16HDRCSB - N .LT. 1, 16, 2, 2)
C/7S
      IF (M .LT. 1) CALL SETERR('DRCSB - M .LT. 1', 16, 1, 2)
      IF (N .LT. 1) CALL SETERR('DRCSB - N .LT. 1', 16, 2, 2)
C/
      IF (S .NE. 0D0) GOTO 1
         S = D1MACH(1)
         L = D1MACH(2)
   1  DO  2 I = 1, M
         IF (R(I) .EQ. 0D0) GOTO  2
C/6S
C        IF (R(I) .LT. S .OR. R(I) .GT. L) CALL SETERR(
C    1      36HDRCSB - MUST HAVE S .LE. R(I) .LE. L, 36, 3, 2)
C/7S
         IF (R(I) .LT. S .OR. R(I) .GT. L) CALL SETERR(
     1      'DRCSB - MUST HAVE S .LE. R(I) .LE. L', 36, 3, 2)
C/
   2     CONTINUE
C TURN ERROR RECOVERY ON AND SAVE OLD VALUE.
      CALL ENTSRC(IROLD, 1)
C GET COLUMN SCALE FACTOR.
      CALL DRCSC(B, M, N, R, C, RC)
C APPLY THEM.
      CALL DRCSS(B, M, N, R, C, RC)
      IF (NERROR(NERR) .EQ. 0) GOTO 3
         CALL ERROFF
C/6S
C        CALL SETERR(42HDRCSB - MUST HAVE 1/(S*L) IN MACHINE RANGE, 42
C    1      , 4, 1)
C/7S
         CALL SETERR('DRCSB - MUST HAVE 1/(S*L) IN MACHINE RANGE', 42
     1      , 4, 1)
C/
         RETURN
C RESTORE OLD RECOVERY VALUE.
   3  CALL RETSRC(IROLD)
      RETURN
      END
