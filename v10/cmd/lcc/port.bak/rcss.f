      SUBROUTINE RCSS(A, M, N, R, C, RC)
      INTEGER M, N
      INTEGER RC(N)
      REAL A(M, N), R(M), C(N)
      INTEGER I, J, RD2
      REAL L, S, D1, D2, R1MACH
      LOGICAL BADNGE
      DATA S/0E0/
      DATA L/0E0/
C TO SCALE ((1/R)*A)*(1/C).
C/6S
C     IF (M .LT. 1) CALL SETERR(16H RCSS - M .LT. 1, 16, 1, 2)
C     IF (N .LT. 1) CALL SETERR(16H RCSS - N .LT. 1, 16, 2, 2)
C/7S
      IF (M .LT. 1) CALL SETERR(' RCSS - M .LT. 1', 16, 1, 2)
      IF (N .LT. 1) CALL SETERR(' RCSS - N .LT. 1', 16, 2, 2)
C/
      IF (S .NE. 0.) GOTO 1
         S = R1MACH(1)
         L = R1MACH(2)
   1  DO  2 I = 1, M
         IF (R(I) .EQ. 0.) GOTO  2
C/6S
C        IF (R(I) .LT. S .OR. R(I) .GT. L) CALL SETERR(
C    1      36H RCSS - MUST HAVE S .LE. R(I) .LE. L, 36, 3, 2)
C/7S
         IF (R(I) .LT. S .OR. R(I) .GT. L) CALL SETERR(
     1      ' RCSS - MUST HAVE S .LE. R(I) .LE. L', 36, 3, 2)
C/
   2     CONTINUE
      DO  3 I = 1, N
         IF (C(I) .EQ. 0.) GOTO  3
C/6S
C        IF (C(I) .LT. S .OR. C(I) .GT. L) CALL SETERR(
C    1      36H RCSS - MUST HAVE S .LE. C(I) .LE. L, 36, 4, 2)
C        IF (RC(I) .LT. (-1) .OR. RC(I) .GT. 1) CALL SETERR(
C    1      36H RCSS - MUST HAVE RC(I) IN (-1,0,+1), 36, 5, 2)
C/7S
         IF (C(I) .LT. S .OR. C(I) .GT. L) CALL SETERR(
     1      ' RCSS - MUST HAVE S .LE. C(I) .LE. L', 36, 4, 2)
         IF (RC(I) .LT. (-1) .OR. RC(I) .GT. 1) CALL SETERR(
     1      ' RCSS - MUST HAVE RC(I) IN (-1,0,+1)', 36, 5, 2)
C/
   3     CONTINUE
C CHECK 1/(S*L) RANGE.
      BADNGE = .FALSE.
      IF (S*L .GT. 1.) GOTO 4
         IF (1./L .GT. S*L) BADNGE = .TRUE.
         GOTO  5
   4     IF (S*L .GT. 1./S) BADNGE = .TRUE.
C S*L > 1.
C/6S
C  5  IF (BADNGE) CALL SETERR(
C    1   42H RCSX - MUST HAVE 1/(S*L) IN MACHINE RANGE, 42, 6, 1)
C/7S
   5  IF (BADNGE) CALL SETERR(
     1   ' RCSX - MUST HAVE 1/(S*L) IN MACHINE RANGE', 42, 6, 1)
C/
      DO  33 I = 1, M
         D1 = R(I)
         IF (D1 .EQ. 0.) GOTO  33
         DO  32 J = 1, N
            D2 = C(J)
            RD2 = RC(J)
            IF (A(I, J) .NE. 0. .AND. D2 .NE. 0.) GOTO 6
               GOTO  32
   6           IF (D1 .LT. 1.) GOTO 19
                  IF (RD2 .LE. 0) GOTO 11
                     IF (D2 .LT. 1.) GOTO 7
                        A(I, J) = S*((A(I, J)/D1)/D2)
C D2 OVERFLOWED.
                        GOTO  10
   7                    IF (D1*D2 .LT. 1.) GOTO 8
                           A(I, J) = S*(A(I, J)/(D1*D2))
C D2 < 1.
                           GOTO  9
   8                       A(I, J) = A(I, J)*(S/(D1*D2))
   9                    CONTINUE
  10                 CONTINUE
                     GOTO  18
  11                 IF (D2 .LT. 1.) GOTO 12
                        A(I, J) = (A(I, J)/D1)/D2
                        GOTO  17
  12                    IF (RD2 .GE. 0) GOTO 15
                           IF (D2 .LT. 1./D1) GOTO 13
                              A(I, J) = A(I, J)*((L/D1)/D2)
C D2 UNDERFLOWED.
                              GOTO  14
  13                          A(I, J) = L*(A(I, J)/(D1*D2))
  14                       CONTINUE
                           GOTO  16
  15                       A(I, J) = A(I, J)/(D1*D2)
C D2 < 1.
  16                 CONTINUE
  17              CONTINUE
  18              CONTINUE
                  GOTO  30
  19              IF (RD2 .LE. 0) GOTO 22
                     IF (D1*D2 .LT. 1.) GOTO 20
                        A(I, J) = S*(A(I, J)/(D1*D2))
C D1 < 1.
C D2 OVERFLOWED.
                        GOTO  21
  20                    A(I, J) = A(I, J)*((S/D1)/D2)
  21                 CONTINUE
                     GOTO  29
  22                 IF (D2 .LT. 1.) GOTO 23
                        A(I, J) = A(I, J)/(D1*D2)
                        GOTO  28
  23                    IF (RD2 .GE. 0) GOTO 26
                           IF (D1*D2 .GT. 1.) GOTO 24
                              A(I, J) = L*(A(I, J)/(D1*D2))
C D2 UNDERFLOWED.
                              GOTO  25
  24                          A(I, J) = A(I, J)*(L/(D1*D2))
  25                       CONTINUE
                           GOTO  27
  26                       A(I, J) = (A(I, J)/D1)/D2
C D2 < 1.
  27                 CONTINUE
  28              CONTINUE
  29              CONTINUE
  30        CONTINUE
  31        CONTINUE
  32        CONTINUE
  33     CONTINUE
      RETURN
      END
