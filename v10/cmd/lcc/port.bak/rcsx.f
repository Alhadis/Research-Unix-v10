      SUBROUTINE RCSX(X, N, M, R, RR, C, RC)
      INTEGER M, N
      INTEGER RR(N), RC(M)
      REAL X(N, M), R(N), C(M)
      INTEGER I, J, RD2, RD3
      REAL ABS, AXX, L, S, D2, D3
      REAL AMIN1, AMAX1, SL, XX, R1MACH
      LOGICAL BADNGE, OVELOW
      DATA S/0E0/
      DATA L/0E0/
C TO SCALE
C   X = (1/R) * X * C.
C/6S
C     IF (N .LT. 1) CALL SETERR(16H RCSX - N .LT. 1, 16, 1, 2)
C     IF (M .LT. 1) CALL SETERR(16H RCSX - M .LT. 1, 16, 2, 2)
C/7S
      IF (N .LT. 1) CALL SETERR(' RCSX - N .LT. 1', 16, 1, 2)
      IF (M .LT. 1) CALL SETERR(' RCSX - M .LT. 1', 16, 2, 2)
C/
      IF (S .NE. 0.) GOTO 1
         S = R1MACH(1)
         L = R1MACH(2)
   1  CONTINUE
      SL = S*L
      DO  2 I = 1, N
         IF (R(I) .EQ. 0.) GOTO  2
C/6S
C        IF (R(I) .LT. S .OR. R(I) .GT. L) CALL SETERR(
C    1      36H RCSX - MUST HAVE S .LE. R(I) .LE. L, 36, 3, 2)
C        IF (RR(I) .LT. (-1) .OR. RR(I) .GT. 1) CALL SETERR(
C    1      36H RCSX - MUST HAVE RR(I) IN (-1,0,+1), 36, 4, 2)
C/7S
         IF (R(I) .LT. S .OR. R(I) .GT. L) CALL SETERR(
     1      ' RCSX - MUST HAVE S .LE. R(I) .LE. L', 36, 3, 2)
         IF (RR(I) .LT. (-1) .OR. RR(I) .GT. 1) CALL SETERR(
     1      ' RCSX - MUST HAVE RR(I) IN (-1,0,+1)', 36, 4, 2)
C/
   2     CONTINUE
      DO  3 I = 1, M
         IF (C(I) .EQ. 0.) GOTO  3
C/6S
C        IF (C(I) .LT. S .OR. C(I) .GT. L) CALL SETERR(
C    1      36H RCSX - MUST HAVE S .LE. C(I) .LE. L, 36, 5, 2)
C        IF (RC(I) .LT. (-1) .OR. RC(I) .GT. 1) CALL SETERR(
C    1      36H RCSX - MUST HAVE RC(I) IN (-1,0,+1), 36, 6, 2)
C/7S
         IF (C(I) .LT. S .OR. C(I) .GT. L) CALL SETERR(
     1      ' RCSX - MUST HAVE S .LE. C(I) .LE. L', 36, 5, 2)
         IF (RC(I) .LT. (-1) .OR. RC(I) .GT. 1) CALL SETERR(
     1      ' RCSX - MUST HAVE RC(I) IN (-1,0,+1)', 36, 6, 2)
C/
   3     CONTINUE
C CHECK 1/(S*L) RANGE.
      BADNGE = .FALSE.
      IF (SL .GT. 1.) GOTO 4
         IF (1./L .GT. SL) BADNGE = .TRUE.
         GOTO  5
   4     IF (SL .GT. 1./S) BADNGE = .TRUE.
C S*L > 1.
C/6S
C  5  IF (BADNGE) CALL SETERR(
C    1   42H RCSX - MUST HAVE 1/(S*L) IN MACHINE RANGE, 42, 7, 1)
C/7S
   5  IF (BADNGE) CALL SETERR(
     1   ' RCSX - MUST HAVE 1/(S*L) IN MACHINE RANGE', 42, 7, 1)
C/
      OVELOW = .FALSE.
      DO  86 J = 1, M
         D3 = C(J)
         RD3 = RC(J)
C DO THE SCALING.
         DO  85 I = 1, N
            D2 = R(I)
            RD2 = RR(I)
            XX = X(I, J)
            AXX = ABS(XX)
            IF (D2 .EQ. 0.) GOTO  85
            BADNGE = .FALSE.
            IF (XX .NE. 0. .AND. D3 .NE. 0.) GOTO 6
               X(I, J) = 0
               GOTO  85
   6        IF (RD2 .NE. RD3) GOTO 26
               IF (D3 .LT. D2) GOTO 16
                  IF (D3 .GT. 1.) GOTO 7
                     IF (D3*AXX .GT. D2*L) BADNGE = .TRUE.
C WORRY ABOUT OVERFLOW.
                     GOTO  10
   7                 IF (D2 .GT. 1.) GOTO 8
                        IF (AXX .GT. D2*(L/D3)) BADNGE = .TRUE.
C D3 > 1.
                        GOTO  9
   8                    IF (AXX/D2 .GT. L/D3) BADNGE = .TRUE.
C D2 > 1 & D3 > 1.
   9              CONTINUE
  10              IF (BADNGE) GOTO 15
                     IF (D3 .GT. AMIN1(D2, 1E0)*L) GOTO 11
                        X(I, J) = X(I, J)*(D3/D2)
C IN-RANGE.
                        GOTO  14
  11                    IF (D3 .LT. 1. .AND. AXX .GT. D2*L) GOTO 12
                           X(I, J) = D3*(XX/D2)
C D3/D2 OVERFLOWS, IE, D3 > D2*L AND D2 < 1.
                           GOTO  13
  12                       X(I, J) = (D3*XX)/D2
  13                    CONTINUE
  14                 CONTINUE
  15              CONTINUE
                  GOTO  25
  16              IF (D3 .LT. 1.) GOTO 17
                     IF (AXX .LT. (D2*S)/D3) X(I, J) = 0
C D3 < D2, WORRY ABOUT UNDERFLOW.
C D2 > D3 >= 1.
                     GOTO  20
  17                 IF (D2 .LT. 1.) GOTO 18
                        IF (D3*AXX .LT. D2*S) X(I, J) = 0
C D3 < 1.
                        GOTO  19
  18                    IF (AXX .LT. (S/D3)*D2) X(I, J) = 0
C D2, D3 < 1.
  19              CONTINUE
  20              IF (X(I, J) .EQ. 0.) GOTO  85
C       IN-RANGE.
                  IF (D3 .LT. AMAX1(D2, 1E0)*S) GOTO 21
                     X(I, J) = X(I, J)*(D3/D2)
                     GOTO  24
  21                 IF (D3 .GT. 1. .AND. AXX .LT. D2*S) GOTO 22
                        X(I, J) = D3*(XX/D2)
C D3/D2 UNDERFLOWS, I.E., D3 < D2*S, D2 > 1 .
                        GOTO  23
  22                    X(I, J) = (D3*XX)/D2
  23                 CONTINUE
  24              CONTINUE
  25           CONTINUE
               GOTO  83
  26           IF (RD2 .LE. 0 .OR. RD3 .GE. 0) GOTO 27
                  X(I, J) = 0
                  GOTO  85
C UNDERFLOW.
  27              IF (RD2 .GE. 0 .OR. RD3 .LE. 0) GOTO 28
                     BADNGE = .TRUE.
C OVERFLOW.
                     GOTO  81
  28                 IF (RD2 .GE. 0 .OR. RD3 .NE. 0) GOTO 42
                        IF (D3 .GT. 1.) GOTO 29
                           IF (D3*AXX .GT. D2) BADNGE = .TRUE.
C WORRY ABOUT OVERFLOW.
                           GOTO  30
  29                       IF (AXX .GT. D2/D3) BADNGE = .TRUE.
C D3 > 1.
  30                    IF (BADNGE) GOTO 41
                           IF (D3 .GT. D2) GOTO 33
                              IF (D3 .GT. 1.) GOTO 31
                                 X(I, J) = ((D3*L)/D2)*XX
C IN-RANGE.
                                 GOTO  32
  31                             X(I, J) = ((D3/D2)*L)*XX
  32                          CONTINUE
                              GOTO  40
  33                          IF (D3 .LT. 1. .AND. AXX .GT. D2) GOTO 36
                                 IF (AXX .LT. D2*S) GOTO 34
                                    X(I, J) = D3*(L*(XX/D2))
C D3 > D2.
C D3*(X*L/D2).
                                    GOTO  35
  34                                X(I, J) = D3*((XX*L)/D2)
  35                             CONTINUE
                                 GOTO  39
  36                             IF (AXX .GT. D2*L) GOTO 37
                                    X(I, J) = (D3*(XX/D2))*L
C D3 < 1 & |X| > D2.
                                    GOTO  38
  37                                X(I, J) = ((D3/D2)*XX)*L
  38                             CONTINUE
  39                          CONTINUE
  40                       CONTINUE
  41                    CONTINUE
                        GOTO  80
  42                    IF (RD2 .LE. 0 .OR. RD3 .NE. 0) GOTO 55
                           IF (D3 .LT. 1.) GOTO 43
                              IF (AXX .LT. D2/D3) X(I, J) = 0
C WORRY ABOUT UNDERFLOW.
                              GOTO  44
  43                          IF (D3*AXX .LT. D2) X(I, J) = 0
  44                       IF (X(I, J) .EQ. 0.) GOTO  85
C       IN-RANGE.
                           IF (D3 .LT. D2) GOTO 47
                              IF (D2 .GT. 1.) GOTO 45
                                 X(I, J) = ((S/D2)*D3)*XX
                                 GOTO  46
  45                             X(I, J) = ((D3/D2)*S)*XX
  46                          CONTINUE
                              GOTO  54
  47                          IF (D3 .GT. 1. .AND. AXX .LT. D2) GOTO 50
                                 IF (D2 .GT. 1.) GOTO 48
                                    X(I, J) = D3*(XX*(S/D2))
C D3 < D2.
                                    GOTO  49
  48                                X(I, J) = D3*((XX/D2)*S)
  49                             CONTINUE
                                 GOTO  53
  50                             IF (D3 .LT. S*D2) GOTO 51
                                    X(I, J) = ((D3/D2)*XX)*S
C D2 > D3 > 1 AND |X| < D2.
                                    GOTO  52
  51                                X(I, J) = ((XX/D2)*D3)*S
  52                             CONTINUE
  53                          CONTINUE
  54                       CONTINUE
                           GOTO  79
  55                       IF (RD2 .NE. 0 .OR. RD3 .GE. 0) GOTO 66
                              IF (D2 .LT. 1.) GOTO 56
                                 IF (AXX/D2 .LT. SL/D3) X(I, J) = 0
C WORRY ABOUT UNDERFLOW.
                                 GOTO  57
  56                             IF (AXX .LT. (SL/D3)*D2) X(I, J) = 0
  57                          IF (X(I, J) .EQ. 0.) GOTO  85
C       IN-RANGE.
                              IF (D2 .GT. 1.) GOTO 62
                                 IF (D3 .LT. D2*SL) GOTO 58
                                    X(I, J) = ((D3/D2)/L)*XX
                                    GOTO  61
  58                                IF (AXX .LT. SL) GOTO 59
                                       X(I, J) = (D3/D2)*(XX/L)
                                       GOTO  60
  59                                   X(I, J) = ((D3/D2)*XX)/L
  60                             CONTINUE
  61                             CONTINUE
                                 GOTO  65
  62                             IF (AXX/D2 .LT. SL) GOTO 63
                                    X(I, J) = ((XX/D2)/L)*D3
C D2 > 1.
                                    GOTO  64
  63                                X(I, J) = ((XX/D2)*D3)/L
  64                             CONTINUE
  65                          CONTINUE
                              GOTO  78
  66                          IF (D2 .LT. 1.) GOTO 67
                                 IF (AXX/D2 .GT. SL/D3) BADNGE = .TRUE.
C RD2 == 0 & RD3 > 0, WORRY ABOUT OVERFLOW.
                                 GOTO  68
  67                             IF (AXX .GT. D2*(SL/D3)) BADNGE =
     1                              .TRUE.
  68                          IF (BADNGE) GOTO 77
                                 IF (D2 .LT. 1.) GOTO 73
                                    IF (D3/D2 .GT. SL) GOTO 69
                                       X(I, J) = ((D3/D2)/S)*XX
C IN-RANGE.
                                       GOTO  72
  69                                   IF (AXX .GT. SL) GOTO 70
                                          X(I, J) = (D3/D2)*(XX/S)
                                          GOTO  71
  70                                      X(I, J) = ((D3/D2)*XX)/S
  71                                CONTINUE
  72                                CONTINUE
                                    GOTO  76
  73                                IF (AXX .GT. D2*SL) GOTO 74
                                       X(I, J) = ((XX/D2)/S)*D3
C D2 < 1.
                                       GOTO  75
  74                                   X(I, J) = ((XX/D2)*D3)/S
  75                                CONTINUE
  76                             CONTINUE
  77                          CONTINUE
  78                    CONTINUE
  79                 CONTINUE
  80              CONTINUE
  81           CONTINUE
  82        CONTINUE
  83        IF (.NOT. BADNGE) GOTO 84
               X(I, J) = L*(XX/AXX)
               OVELOW = .TRUE.
  84        CONTINUE
  85        CONTINUE
  86     CONTINUE
      IF (.NOT. OVELOW) GOTO 87
         CALL ERROFF
C/6S
C        CALL SETERR(33H RCSX - SOLUTION X HAS OVERFLOWED, 33, 8, 1)
C/7S
         CALL SETERR(' RCSX - SOLUTION X HAS OVERFLOWED', 33, 8, 1)
C/
  87  RETURN
      END
