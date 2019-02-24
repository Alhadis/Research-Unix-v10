      LOGICAL FUNCTION D4SSOR(WV, RV, IV, LV, ERRARE)
      INTEGER IV(40)
      REAL RV(30), ERRARE(2)
      LOGICAL LV(20)
      DOUBLE PRECISION WV(30)
      COMMON /CSTAK/ DS
      DOUBLE PRECISION DS(500)
      INTEGER IDX, IFIX, I1, I2, I3, IS(1000)
      REAL RS(1000), RTEMP
      LOGICAL LS(1000), D4SSOD, D4SSOL
      DOUBLE PRECISION TEMP, WS(500)
      INTEGER TEMP1, TEMP2, TEMP3, TEMP4, TEMP5, TEMP6
      INTEGER TEMP7
      EQUIVALENCE (DS(1), WS(1), RS(1), IS(1), LS(1))
C SEE IF A RESTART IS IN ORDER.
      RV(1) = ERRARE(1)
C UPDATE ERRPAR.
      RV(2) = ERRARE(2)
      IF (.NOT. LV(7)) GOTO 1
         D4SSOR = .TRUE.
         RETURN
   1     D4SSOR = .FALSE.
   2  IF (IV(18) .LE. IV(17)) GOTO 8
         LV(5) = .TRUE.
C SEE IF A RE-START IS NECESSARY.
         TEMP6 = IV(21)
         TEMP5 = IV(22)
         TEMP4 = IV(10)
         IF (.NOT. D4SSOL(RS(TEMP6), RS(TEMP5), IV(1), IV(18), IV(2),
     1      RS(TEMP4), RV(10))) GOTO 3
            D4SSOR = .TRUE.
            RETURN
C   IF WILL NOT CONVERGE IN THIS LOZENGE, RESTART.
   3     TEMP4 = IV(21)
         TEMP5 = IV(11)
         TEMP6 = IV(26)
         TEMP7 = IV(10)
         LV(6) = D4SSOD(RS(TEMP4), IV(1), IV(18), IV(3), IV(2), WS(
     1      TEMP5), WV(3), WV(4), WV(5), RS(TEMP6), RS(TEMP7), IV(24),
     2      IV(23), IV(17))
         IF (.NOT. LV(6)) GOTO 4
            D4SSOR = .TRUE.
            RETURN
   4     IF (IV(18) .LE. IV(17)+1) GOTO 7
            TEMP7 = IV(21)
C SEE IF A RE-START WOULD BE MORE EFFICIENT.
            TEMP6 = IV(11)
            TEMP5 = IV(26)
            TEMP4 = IV(10)
            TEMP3 = IV(6)
            TEMP2 = IV(9)
            TEMP1 = IV(5)
            CALL D4SSOO(RS(TEMP7), IV(1), IV(18), IV(3), IV(2), WS(
     1         TEMP6), WV(4), RS(TEMP5), RS(TEMP4), IV(16), WV(6), RS(
     2         TEMP3), RS(TEMP2), RS(TEMP1), RV(8), RV(9), RV(12))
            IDX = IV(6)+IV(16)
            IF (WV(11)+RS(IDX) .NE. WV(11)) GOTO 5
C/6S
C              CALL SETERR(13HDESSOM - DT=0, 13, 15, 1)
C/7S
               CALL SETERR('DESSOM - DT=0', 13, 15, 1)
C/
               D4SSOR = .TRUE.
               RETURN
C = SNGL(DT/HOPT(KOPT+1)).
   5        RTEMP = WV(6)/RS(IDX)
            IF (RTEMP .LE. 1E+3) RTEMP = IFIX(RTEMP+0.99)
C     IF ( WORK(M) + WORK(KOPT+2)*RTEMP <= WORK(L) )
            I1 = IV(8)+IV(18)-1
            I2 = IV(8)+IV(16)+1
            I3 = IV(8)+IV(24)-1
            IF (RS(I1)+RS(I2)*RTEMP .GT. RS(I3)) GOTO 6
               CALL ISTKRL(1)
               LV(4) = .TRUE.
               D4SSOR = .TRUE.
               RETURN
   6        CONTINUE
   7     CONTINUE
   8  IF (IV(18) .LT. IV(3)) CALL ISTKRL(2)
      RETURN
      END
