      LOGICAL FUNCTION A4SSOE(WV, RV, IV, LV, X, TSTOPE, DTE)
      INTEGER IV(40)
      REAL WV(30), RV(30), X(1), TSTOPE, DTE
      LOGICAL LV(20)
      COMMON /CSTAK/ DS
      DOUBLE PRECISION DS(500)
      INTEGER MIN0, I, K, I1, I2, IS(1000)
      REAL ABS, TEMP, AMIN1, RS(1000), WS(500), RTEMP
      LOGICAL LS(1000)
      INTEGER TEMP2
      LOGICAL TEMP1
      EQUIVALENCE (DS(1), WS(1), RS(1), IS(1), LS(1))
C WIND THINGS UP AT THE END OF THE TIME-STEP.
      WV(2) = TSTOPE
      WV(6) = DTE
      A4SSOE = .FALSE.
C RELEASE E.
      CALL ISTKRL(1)
      IF (WV(11)+WV(6) .NE. WV(11)) GOTO 1
C/6S
C        CALL SETERR(30H ESSOM - DT=0 RETURNED BY SOUT, 30, 16, 1)
C/7S
         CALL SETERR(' ESSOM - DT=0 RETURNED BY SOUT', 30, 16, 1)
C/
         A4SSOE = .TRUE.
         RETURN
   1  IF ((WV(6)/ABS(WV(6)))*(WV(2)-WV(11)) .GE. 0.) GOTO 2
C/6S
C        CALL SETERR(47H ESSOM - DT RETURNED BY SOUT HAS THE WRONG SIGN,
C    1      47, 17, 1)
C/7S
         CALL SETERR(' ESSOM - DT RETURNED BY SOUT HAS THE WRONG SIGN',
     1      47, 17, 1)
C/
         A4SSOE = .TRUE.
         RETURN
   2  IF (.NOT. LV(2)) GOTO 9
         IF (WV(10) .NE. WV(11)) GOTO 3
            IV(17) = MIN0(IV(16), IV(17))
            GOTO  8
   3        TEMP2 = IV(1)
C CONVERGED, SO UPDATE X0, HOPT, HUP, ETC.
            DO  4 I = 1, TEMP2
               I1 = IV(12)+I-1
               X(I) = WS(I1)
   4           CONTINUE
C X=X1.
            K = 1
               GOTO  6
   5           K = K+1
   6           IF (K .GT. IV(15)) GOTO  7
C HOPTO = HOPT.
               I1 = IV(7)+K-1
               I2 = IV(6)+K-1
               RS(I1) = RS(I2)
               GOTO  5
   7        RV(7) = AMIN1(2.*RV(7), RV(6))
            IV(19) = IV(18)
            IV(17) = IV(16)
            LV(3) = .FALSE.
   8     CONTINUE
         GOTO  12
   9     IF (ABS(WV(9)) .GE. ABS(WV(6))) GOTO 10
C/6S
C           CALL SETERR(42H ESSOM - DT RAISED BY SOUT WHEN OK = FALSE,
C    1         42, 18, 1)
C/7S
            CALL SETERR(' ESSOM - DT RAISED BY SOUT WHEN OK = FALSE',
     1         42, 18, 1)
C/
            A4SSOE = .TRUE.
            RETURN
C   THE DEFAULT RESPONSE IS TO LOWER DT BY 10**3.
  10     IF (ABS(WV(6)) .EQ. ABS(WV(9))) WV(6) = WV(6)/1E+3
         IF (WV(11)+WV(6) .NE. WV(11)) GOTO 11
C/6S
C           CALL SETERR(13H ESSOM - DT=0, 13, 15, 1)
C/7S
            CALL SETERR(' ESSOM - DT=0', 13, 15, 1)
C/
            A4SSOE = .TRUE.
            RETURN
  11     CONTINUE
  12  WV(10) = WV(11)
      WV(11) = WV(10)+WV(6)
      TEMP1 = (WV(6)/ABS(WV(6)))*(WV(11)-WV(2)) .GE. 0.
      IF (.NOT. TEMP1) TEMP1 = ABS(WV(11)-WV(2)) .LE. WV(7)*ABS(WV(6))
      IF (TEMP1) WV(11) = WV(2)
      IF (WV(10) .NE. WV(2)) WV(6) = WV(11)-WV(10)
      DTE = WV(6)
      IF (WV(11)+WV(6) .NE. WV(11)) GOTO 13
C/6S
C        CALL SETERR(13H ESSOM - DT=0, 13, 15, 1)
C/7S
         CALL SETERR(' ESSOM - DT=0', 13, 15, 1)
C/
         A4SSOE = .TRUE.
  13  RETURN
      END
