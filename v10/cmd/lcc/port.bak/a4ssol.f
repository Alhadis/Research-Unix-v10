      LOGICAL FUNCTION A4SSOL(E2, E, NX, M, KMAX, POW, LOGRND)
      INTEGER KMAX, NX
      INTEGER M
      REAL E2(NX, KMAX), E(NX), POW(KMAX), LOGRND
      INTEGER JHI, MIN0, I, J
      REAL ABS, V, ALOG, LOGE, AMIN1
      LOGICAL TEMP
C TO RETURN POW(J) TIMES THE LOGARITHM OF THE RATIO OF THE DESIRED
C TO THE ATTAINED ERROR, FOR EACH ELEMENT IN THE LOZENGE.
C
C A4SSOL = TRUE IF NOT SUCCESSFUL.
C A4SSOL = FALSE IF SUCCESSFUL.
      A4SSOL = .FALSE.
      JHI = MIN0(M-1, KMAX)
      DO  5 I = 1, NX
         TEMP = E(I) .EQ. 0.
         IF (TEMP) TEMP = E2(I, 1) .NE. 0.
         IF (.NOT. TEMP) TEMP = E(I) .LT. 0.
         IF (.NOT. TEMP) GOTO 1
C/6S
C           CALL SETERR(37H ESSOM - E(I).LE.0 RETURNED BY SERROR, 37,
C    1         19, 1)
C/7S
            CALL SETERR(' ESSOM - E(I).LE.0 RETURNED BY SERROR', 37,
     1         19, 1)
C/
            A4SSOL = .TRUE.
            GOTO  6
   1     IF (E(I) .GT. 0.) LOGE = ALOG(E(I))
         DO  4 J = 1, JHI
            TEMP = E2(I, J) .NE. 0.
            IF (TEMP) TEMP = E2(I, 1) .NE. 0.
            IF (.NOT. TEMP) GOTO 2
               V = AMIN1(LOGE-ALOG(ABS(E2(I, J))), (-LOGRND)-4.6)
               GOTO  3
   2           V = (-LOGRND)-4.6
   3        E2(I, J) = POW(J)*V
   4        CONTINUE
   5     CONTINUE
   6  RETURN
      END
