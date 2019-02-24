      SUBROUTINE DIODEV(J, F, R, I, L)
      INTEGER J, I
      REAL R
      LOGICAL L
      DOUBLE PRECISION F
      INTEGER MAX0, K, M, N(100), IABS, KEEJAC
      INTEGER KMAX, IRCS, MMAX, KINIT, MINIT, MAXIT
      REAL HFRACT, EGIVE, FLOAT
      LOGICAL USENFD, USENGJ, USENNS, ERPUTS, XPOLY
      DOUBLE PRECISION BETA, DBLE, GAMMA, DELTA, THETA, DSQRT
      INTEGER TEMP
      LOGICAL TEMP1
      DATA THETA/1D0/
      DATA BETA/1D0/
      DATA GAMMA/1D0/
      DATA DELTA/0D0/
      DATA HFRACT/1E0/
      DATA EGIVE/1E+2/
      DATA KEEJAC/0/
      DATA MINIT/10/
      DATA MAXIT/50/
      DATA KMAX/10/
      DATA KINIT/4/
      DATA MMAX/15/
      DATA IRCS/1/
      DATA XPOLY/.FALSE./
      DATA ERPUTS/.FALSE./
      DATA USENGJ/.FALSE./
      DATA USENNS/.FALSE./
      DATA USENFD/.FALSE./
      DATA N(1)/1/, N(2)/0/, N(3)/0/
C THE PARAMETER SETTING ROUTINE FOR IODE.
C THE VARIABLES ARE
C J = 1.
C J = 2.
C J = 3.
C J = 4.
C J = 1001.
C J = 1002.
C J = 2001.
C J = 2002.
C J = 2003.
C J = 2004.
C J = 2005.
C J = 2006.
C J = 2007. 0 DO NOT SCALE, +1 SCALE (DEFAULT).
C J = 3001.
C J = 3002.
C J = 3003.
C J = 3004.
C J = 3005.
C J = 4001, ... , 4100.
      GOTO  58
C   EXPORT THE VARIABLES.
   1     F = THETA
         GOTO  59
   2     F = BETA
         GOTO  59
   3     F = GAMMA
         GOTO  59
   4     F = DELTA
         GOTO  59
   5     R = HFRACT
         GOTO  59
   6     R = EGIVE
         GOTO  59
   7     I = KEEJAC
         GOTO  59
   8     I = MINIT
         GOTO  59
   9     I = MAXIT
         GOTO  59
  10     I = KMAX
         GOTO  59
  11     I = KINIT
         GOTO  59
  12     I = MMAX
         GOTO  59
  13     I = IRCS
         GOTO  59
  14     L = XPOLY
         GOTO  59
  15     L = ERPUTS
         GOTO  59
  16     L = USENGJ
         GOTO  59
  17     L = USENNS
         GOTO  59
  18     L = USENFD
         GOTO  59
C IODE VERSION NUMBER.
  19     F = 3D0
         GOTO  59
C SET THE VARIABLES TO THE DEFAULTS.
  20     THETA = 1D0
         BETA = 1
         GAMMA = 1
         DELTA = 0
         HFRACT = 1
         EGIVE = 1E+2
         KEEJAC = 0
         MINIT = 10
         MAXIT = 50
         KMAX = 10
         KINIT = 4
         MMAX = 15
         IRCS = 1
         XPOLY = .FALSE.
         ERPUTS = .FALSE.
         USENGJ = .FALSE.
         USENNS = .FALSE.
         USENFD = .FALSE.
         CALL SETI(100, 0, N)
         N(1) = 1
C   IMPORT THE VARIABLES.
         GOTO  59
  21     THETA = F
         IF (THETA .EQ. 0.5) GOTO 22
            GAMMA = 1
            HFRACT = 1
            GOTO  26
  22        GAMMA = 2
            HFRACT = 0.5
            N(1) = 2
            N(2) = 4
            N(3) = 6
            M = 4
               GOTO  24
  23           M = M+1
  24           IF (M .GT. MMAX) GOTO  25
               N(M) = 2*N(M-2)
               GOTO  23
  25        CONTINUE
  26     GOTO  59
  27     BETA = F
         GOTO  59
  28     GAMMA = F
         GOTO  59
  29     DELTA = F
         GOTO  59
  30     HFRACT = R
         GOTO  59
  31     EGIVE = R
         GOTO  59
  32     KEEJAC = I
         GOTO  59
  33     MINIT = I
         GOTO  59
  34     MAXIT = I
         GOTO  59
  35     KMAX = I
         MMAX = KMAX+5
         GOTO  59
  36     KINIT = I
         GOTO  59
  37     MMAX = I
         GOTO  59
  38     IRCS = I
         GOTO  59
  39     XPOLY = L
         GOTO  59
  40     ERPUTS = L
         IF (.NOT. ERPUTS) GOTO 41
            DELTA = 1
            GOTO  42
  41        DELTA = 0
  42     GOTO  59
  43     USENGJ = L
         GOTO  59
  44     USENNS = L
         GOTO  59
  45     USENFD = L
         GOTO  59
  46     TEMP1 = IABS(J) .GT. 4100
         IF (.NOT. TEMP1) TEMP1 = IABS(J) .LT. 4001
C/6S
C        IF (TEMP1) CALL SETERR(24HDIODEV - J OUT OF BOUNDS, 24, 1, 2)
C/7S
         IF (TEMP1) CALL SETERR('DIODEV - J OUT OF BOUNDS', 24, 1, 2)
C/
         IF (J .GE. 0) GOTO 56
            IF (N(2) .NE. 0) GOTO 47
               N(2) = DSQRT(2D0)*DBLE(FLOAT(N(1)))
C EXPORT N(ABS(J)-4000)
C ONLY N(1) IS GIVEN, USE SQRT(2) INCREASE.
               IF (N(2) .EQ. N(1)) N(2) = N(2)+1
               N(3) = DSQRT(2D0)*DBLE(FLOAT(N(2)))
               IF (N(3) .EQ. N(2)) N(3) = N(3)+1
               N(4) = 0
  47        TEMP = IABS(J)
            IF (N(TEMP-4000) .NE. 0) GOTO 55
               DO  53 K = 1, MMAX
C FILL IN THE MISSING N(M).
                  IF (N(K) .NE. 0) GOTO 52
                     IF (K .NE. 3) GOTO 49
                        DO  48 M = K, MMAX
                           N(M) = (N(2)*N(M-1))/MAX0(1, N(1))
  48                       CONTINUE
                        GOTO  51
  49                    DO  50 M = K, MMAX
                           N(M) = 2*N(M-2)
  50                       CONTINUE
  51                 GOTO  54
  52              CONTINUE
  53              CONTINUE
  54           CONTINUE
  55        TEMP = IABS(J)
            I = N(TEMP-4000)
            GOTO  57
  56        N(J-4000) = I
C IMPORT N(J-4000)
            IF (J-4000 .LT. 100) N(J-3999) = 0
  57     CONTINUE
         GOTO  59
  58     IF (J .EQ. 3005) GOTO  45
         IF (J .EQ. 3004) GOTO  44
         IF (J .EQ. 3003) GOTO  43
         IF (J .EQ. 3002) GOTO  40
         IF (J .EQ. 3001) GOTO  39
         IF (J .EQ. 2007) GOTO  38
         IF (J .EQ. 2006) GOTO  37
         IF (J .EQ. 2005) GOTO  36
         IF (J .EQ. 2004) GOTO  35
         IF (J .EQ. 2003) GOTO  34
         IF (J .EQ. 2002) GOTO  33
         IF (J .EQ. 2001) GOTO  32
         IF (J .EQ. 1002) GOTO  31
         IF (J .EQ. 1001) GOTO  30
         IF (J .EQ. 4) GOTO  29
         IF (J .EQ. 3) GOTO  28
         IF (J .EQ. 2) GOTO  27
         IF (J .EQ. 1) GOTO  21
         IF (J .EQ. 0) GOTO  20
         IF (J .EQ. (-6000)) GOTO  19
         IF (J .EQ. (-3005)) GOTO  18
         IF (J .EQ. (-3004)) GOTO  17
         IF (J .EQ. (-3003)) GOTO  16
         IF (J .EQ. (-3002)) GOTO  15
         IF (J .EQ. (-3001)) GOTO  14
         IF (J .EQ. (-2007)) GOTO  13
         IF (J .EQ. (-2006)) GOTO  12
         IF (J .EQ. (-2005)) GOTO  11
         IF (J .EQ. (-2004)) GOTO  10
         IF (J .EQ. (-2003)) GOTO  9
         IF (J .EQ. (-2002)) GOTO  8
         IF (J .EQ. (-2001)) GOTO  7
         IF (J .EQ. (-1002)) GOTO  6
         IF (J .EQ. (-1001)) GOTO  5
         IF (J .EQ. (-4)) GOTO  4
         IF (J .EQ. (-3)) GOTO  3
         IF (J .EQ. (-2)) GOTO  2
         IF (J .EQ. (-1)) GOTO  1
         GOTO  46
  59  RETURN
      END
