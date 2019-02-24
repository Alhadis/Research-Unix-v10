      LOGICAL FUNCTION D4SSOM(WV, RV, IV, LV, DTE)
      INTEGER IV(40)
      REAL RV(30)
      LOGICAL LV(20)
      DOUBLE PRECISION WV(30), DTE
      COMMON /CSTAK/ DS
      DOUBLE PRECISION DS(500)
      INTEGER IDX, NERROR, MIN0, K, NERR, I1
      INTEGER I2, I3, IS(1000)
      REAL ABS, EXP, SNGL, AMIN1, AMAX1, RS(1000)
      REAL RTEMP
      LOGICAL LS(1000), D4SSOL
      DOUBLE PRECISION DABS, TEMP, DMIN1, WS(500), DSQRT
      INTEGER TEMP2, TEMP3, TEMP4, TEMP5, TEMP6, TEMP7
      INTEGER TEMP8
      LOGICAL TEMP1
      EQUIVALENCE (DS(1), WS(1), RS(1), IS(1), LS(1))
C THE BASIC STEP-SIZE AND ORDER MONITOR.
      IF (NERROR(NERR) .EQ. 0) GOTO 1
         D4SSOM = .TRUE.
         RETURN
   1     D4SSOM = .FALSE.
   2  IF (IV(3) .NE. 1) GOTO 3
         IF (LV(2)) CALL ISTKRL(1)
         D4SSOM = .FALSE.
         RETURN
   3  IF (.NOT. LV(7)) WV(11) = WV(10)
C SIGNAL A RESTART.
      TEMP1 = WV(10) .EQ. WV(11)
      IF (TEMP1) TEMP1 = WV(10) .NE. WV(1)
      IF (.NOT. TEMP1) GOTO 4
         IV(25) = IV(14)
         WV(12) = WV(10)+WV(6)
   4  TEMP1 = LV(2)
      IF (TEMP1) TEMP1 = .NOT. LV(4)
      IF (.NOT. TEMP1) GOTO 7
         IF (LV(5)) GOTO 6
            TEMP6 = IV(21)
C FIND THE OPTIMAL DT AND M FOR
C THE NEXT TIME-STEP.
            TEMP7 = IV(22)
            TEMP8 = IV(10)
            IF (.NOT. D4SSOL(RS(TEMP6), RS(TEMP7), IV(1), IV(18), IV(2),
     1         RS(TEMP8), RV(10))) GOTO 5
               D4SSOM = .TRUE.
               RETURN
   5        CONTINUE
   6     TEMP8 = IV(21)
         TEMP7 = IV(11)
         TEMP6 = IV(26)
         TEMP5 = IV(10)
         TEMP4 = IV(6)
         TEMP3 = IV(9)
         TEMP2 = IV(5)
         CALL D4SSOO(RS(TEMP8), IV(1), IV(18), IV(3), IV(2), WS(TEMP7)
     1      , WV(4), RS(TEMP6), RS(TEMP5), IV(16), WV(6), RS(TEMP4), RS(
     2      TEMP3), RS(TEMP2), RV(8), RV(9), RV(12))
C RELEASE THE LOZENGE ERROR ESTIMATES E2.
         CALL ISTKRL(1)
   7  IF (.NOT. LV(2)) GOTO 15
         IV(16) = MIN0(IV(16), IV(17)+1)
C GET THE DT FOR THE NEXT TIME-STEP.
         IF (WV(10) .NE. WV(11)) GOTO 8
            IV(28) = MIN0(IV(16), IV(17))
            GOTO  9
   8        IV(28) = IV(16)
   9     IDX = IV(6)+IV(28)
C ABS(HOPT(KOPTM+1))).
         WV(8) = (WV(6)/DABS(WV(6)))*RV(3)*AMIN1(ABS(SNGL(WV(6)))*EXP(
     1      RV(7)), ABS(RS(IDX)))
C   TWO RESTARTS IN A ROW CAUSE DT TO DECREASE BY AT LEAST A
C   FACTOR OF 10**3.
         TEMP1 = LV(3)
         IF (TEMP1) TEMP1 = WV(10) .EQ. WV(11)
         IF (TEMP1) WV(8) = (WV(6)/DABS(WV(6)))*DMIN1(DABS(WV(8)), 1D-3*
     1      DABS(WV(6)))
         IV(15) = MIN0(IV(18), IV(2)+1)
C   COMPUTE THE COST/UNIT TIME-STEP FOR EACH LOZENGE SIZE.
C COST(K) = WORK(K+1)/ABS(HOPT(K)).
         TEMP2 = IV(15)
         DO  12 K = 1, TEMP2
            I1 = IV(5)+K-1
            I2 = IV(8)+K
            I3 = IV(6)+K-1
            IF (RS(I3) .NE. 0.) GOTO 10
               RS(I1) = -1
               GOTO  11
  10           RS(I1) = RS(I2)/ABS(RS(I3))
  11        CONTINUE
  12        CONTINUE
         TEMP1 = WV(10) .NE. WV(1)
         IF (TEMP1) TEMP1 = WV(10) .NE. WV(11)
         IF (.NOT. TEMP1) GOTO 14
            IDX = MIN0(IV(16)+1, IV(19)-1, IV(18)-1)
C SEE IF SHOULD BE CAUTIOUS.
C     IF ABS(HOPT(NEW)) < ABS(HOPT(OLD)), BE CONSERVATIVE.
            I1 = IV(6)+IDX-1
            I2 = IV(7)+IDX-1
            IF (DABS(WV(8)) .GT. 1D-2*ABS(RS(I1)) .AND. ABS(RS(I1))
     1          .LT. ABS(RS(I2))) WV(8) = WV(8)*AMAX1(ABS(RS(I1)/RS(I2))
     2         , 1E-2)
            IF (IV(25) .LE. 0) GOTO 13
               IV(25) = IV(25)-1
C LOGARITHMIC BISECTION.
               IF (IV(25) .EQ. 0) RV(7) = 0.5*RV(5)
               TEMP = (WV(12)-WV(11))/WV(6)
               IF (TEMP .GT. 0.99) WV(8) = (WV(6)/DABS(WV(6)))*DMIN1(
     1            0.5*DABS(WV(12)-WV(11)), DABS(WV(8)), DSQRT(TEMP)*
     2            DABS(WV(6)))
  13        CONTINUE
  14     CONTINUE
  15  IF (WV(10) .EQ. WV(11)) LV(3) = .TRUE.
      WV(9) = WV(6)
      IF (LV(2)) WV(6) = WV(8)
      DTE = WV(6)
      IF (WV(11)+WV(6) .NE. WV(11)) GOTO 16
C/6S
C        CALL SETERR(13HDESSOM - DT=0, 13, 15, 1)
C/7S
         CALL SETERR('DESSOM - DT=0', 13, 15, 1)
C/
         D4SSOM = .TRUE.
  16  RETURN
      END
