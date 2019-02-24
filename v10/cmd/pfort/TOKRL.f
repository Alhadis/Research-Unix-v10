      LOGICAL FUNCTION TOKRL(K1, K2, CODE)
      INTEGER STMT, CODE, PSTMT
      LOGICAL TOKLOP
      COMMON /INPUT/ NSTMT, PSTMT, STMT(1327)
C
C     ROUTINE RETURNS TRUE IF FINDS A REAL CONSTANT IN
C     STMT(K1)-STMT(K2-1) . ELSE IT RETURNS FALSE.
C     BASIC CONSTRUCT IS: <INT> . <INT>
C     <INT> .
C     . <INT>
C     EACH OF THESE MAY BE FOLLOWED BY <D,E> <+,-> <INT>
C     ALSO LEGAL IS IN-CONST FOLLOWED BY EXPONENT CONSTRUCT
C
      TOKRL = .FALSE.
      CALL NEXTOK(K1, K2, K)
      IF (K.EQ.3 .AND. STMT(K1).EQ.64) GO TO 10
      IF (K.EQ.1 .AND. STMT(K2).EQ.64) GO TO 40
C     IF HAVE INT-CONST NEED EXPONENT FOR THIS TO BE A REAL-CONST
      IF (K-1) 80, 50, 80
C
C     FIND BASIC REAL CONSTANT
C
C     (. INT-CONST) CONSTRUCT
   10 CALL NEXTOK(K2, K3, K)
      IF (K.NE.1) GO TO 80
   20 K2 = K3
   30 TOKRL = .TRUE.
      CODE = 1
      GO TO 50
C     (INT-CONST .) CONSTRUCT; CHECK FOR (INT . INT )
   40 K2 = K2 + 1
      CALL NEXTOK(K2, K3, K)
      IF (K.EQ.1) GO TO 20
      IF (TOKLOP(K2-1,K4,K)) GO TO 80
      GO TO 30
C
C     CHECK FOR EXPONENT
C
   50 IF (STMT(K2).NE.33) GO TO 60
      CODE = 0
      GO TO 70
   60 IF (STMT(K2).NE.34) GO TO 80
      CODE = 1
   70 K3 = K2 + 1
      IF (K3.EQ.NSTMT) GO TO 80
      IF ((STMT(K3).EQ.60) .OR. (STMT(K3).EQ.61)) K3 = K3 + 1
      CALL NEXTOK(K3, K4, K)
      IF (K.NE.1) GO TO 80
      K2 = K4
      TOKRL = .TRUE.
   80 RETURN
      END
