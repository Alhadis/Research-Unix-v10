      LOGICAL FUNCTION TOKLOP(K1, K2, KCODE)
C
C     ROUTINE RETURNS TRUE IF FINDS LOGICAL OR RELATIONAL
C     OPERATORS IN STMT(K1)-STMT(K2-1);  RETURNS OPERATOR CODE
C     IN KCODE (SEE EXPR FOR CODES)
C
      INTEGER C(20), CC(9), CODE(9), PSTMT, STMT
      COMMON /INPUT/ NSTMT, PSTMT, STMT(1327)
      DATA C(1) /30/, C(2) /43/, C(3) /33/, C(4) /43/, C(5) /44/, C(6)
     *    /49/, C(7) /44/, C(8) /47/, C(9) /34/, C(10) /46/, C(11)
     *    /43/, C(12) /34/, C(13) /36/, C(14) /49/, C(15) /36/, C(16)
     *    /34/, C(17) /41/, C(18) /49/, C(19) /41/, C(20) /34/
      DATA CC(1), CC(2) /2*3/, CC(3), CC(4), CC(5), CC(6), CC(7),
     *    CC(8), CC(9) /7*2/, CODE(1) /19/, CODE(2) /14/, CODE(3) /20/,
     *    CODE(4), CODE(5), CODE(6), CODE(7), CODE(8), CODE(9) /6*21/
      TOKLOP = .FALSE.
      IF (STMT(K1).NE.64) RETURN
      J = 1
      DO 30 I=1,9
        KK = J + CC(I) - 1
        K2 = K1 + 1
        DO 10 L=J,KK
          IF (STMT(K2).NE.C(L)) GO TO 20
          K2 = K2 + 1
   10   CONTINUE
        KCODE = CODE(I)
        IF (STMT(K2).NE.64) RETURN
        K2 = K2 + 1
        TOKLOP = .TRUE.
        RETURN
   20   J = J + CC(I)
   30 CONTINUE
      RETURN
      END
