      LOGICAL FUNCTION TOKLOG(K1, K2)
C
C     ROUTINE RETURNS TRUE IF FINDS .TRUE. OR .FALSE.
C     IN STMT(K1)-STMT(K2-1)
C
      INTEGER STMT, CONS(13), PSTMT
      COMMON /INPUT/ NSTMT, PSTMT, STMT(1327)
      DATA CONS(1) /64/, CONS(2) /35/, CONS(3) /30/, CONS(4) /41/,
     *    CONS(5) /48/, CONS(6) /34/, CONS(7) /64/, CONS(8) /64/,
     *    CONS(9) /49/, CONS(10) /47/, CONS(11) /50/, CONS(12) /34/,
     *    CONS(13) /64/
      TOKLOG = .FALSE.
      KCNT = 1
      L1 = 1
      L2 = 7
      K2 = K1
   10 DO 20 I=L1,L2
        IF (STMT(K2).NE.CONS(I)) GO TO 30
        K2 = K2 + 1
   20 CONTINUE
      TOKLOG = .TRUE.
      RETURN
   30 KCNT = KCNT + 1
      IF (KCNT.GE.3) RETURN
      K2 = K1
      L1 = 8
      L2 = 13
      GO TO 10
      END
