      SUBROUTINE CHECKS(IROOT)
C
C     MAIN IS INDEX IN NODE OF SUPEROOT IN LAT
C
      LOGICAL ERR, SYSERR, ABORT, INSYM
      COMMON /PARAMS/ I1, I2, I3, I4, I5, I6, I7
      COMMON /DETECT/ ERR, SYSERR, ABORT
      CALL COMCHK(IROOT)
      CALL SCAN(IROOT)
      IF (SYSERR) GO TO 20
 10   IF(.NOT.INSYM(I6,0)) GOTO 30
      CALL UNSAFE
      GO TO 10
   20 WRITE (I2,99999)
99999 FORMAT (31H1UNSAFE REFERENCES NOT VERIFIED)
   30 RETURN
      END