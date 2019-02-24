      LOGICAL FUNCTION ARDECL(K2, KK)
C
C     K2 IS INDEX OF END OF ARRAY DECLARATOR IN STMT
C     KK IS SYMBOL TABLE INDEX FOR THIS ARRAY
C     PROCESSES ARRAY DECLARATOR AND DECLARATOR CONSTRUCTS.
C     CAN EXPECT ARRAY DECLARATOR, ARRAY ELEMENT; ARRAY, VARIABLE.
C     ENTERS INTO SYMBOL TABLE AND TYPES ID;  SETS USAGE ON ARRAY
C     DECLARATOR
C     CHECKS SYNTAX OF BOUNDS; IF VARIABLY DIMENSIONED, BOUNDS
C     VARIABLE AND ARRAY ITSELF MUST BE DUMMY ARGUMENTS.
C     ACCUMULATES TOTAL LENGTH OF ARRAY WITH CONSTANT BOUNDS AND STORES
C     IT OFF ARRAY SYMBOL TABLE ENTRY.  -1 LENGTH INDICATES VARIABLE
C     DIMENSION
C     CALLED BY DIMENSION, TYPE, COMMON, EQUIVALENCE, DATA STMT.
C
C     ARRY IS TRUE FOR ARRAY ELEMENTS/ARRAY DECLARATORS
C     FALSE FOR ARRAYS AND VARIABLES
C     CORNER IS TRUE FOR ARRAY ELEMENTS WITH (1,1,1)--NEEDED IN EQUIV.
C     STMT ;  IF SUCH AN ELEMENT IS RECOGNIZED KK IS SENT AS ITS
C     NEGATIVE.
C
      LOGICAL ERR, SYSERR, ABORT, TOKPNO, VAR, CORNER, ARRY, FLUSH
      INTEGER STMT, PSTMT, PDSA, BNEXT, SYMHD, DSA
      COMMON /INPUT/ NSTMT, PSTMT, STMT(1327)
      COMMON /TABL/ NEXT, LABHD, SYMHD, BNEXT
      COMMON /CTABL/ LDSA, PDSA, DSA(5000)
      COMMON /FACTS/ NAME, NOST, ITYP, IASF
      COMMON /DETECT/ ERR, SYSERR, ABORT
      ERR = .FALSE.
      FLUSH = .FALSE.
      ARRY = .FALSE.
      CORNER = .TRUE.
      ARDECL = .FALSE.
C
C     CHECK NAME; CAN'T HAVE BEEN USED PREVIOUSLY AS A NONVAR;
C     CHECK TO SEE IF HAVE ARRAY ELEMENT/ARRAY DECLARATOR. IF SO
C     ARRY=.TRUE.
C
      ICNT = 0
      CALL NEXTOK(PSTMT, K2, I1)
      IF (I1.EQ.0) GO TO 10
      CALL ERROR1(20H ILLEGAL PUNCTUATION, 20)
      ERR = .TRUE.
      GO TO 280
   10 IF (STMT(K2).EQ.65) ARRY = .TRUE.
      KK = LOOKUP(K2,.FALSE.)
      IF (SYSERR) GO TO 70
      ARDECL = .TRUE.
      L = IGATT1(KK,8)
      IF (L.EQ.0 .OR. L.EQ.10) GO TO 30
      IF (ITYP.LT.6 .AND. L.EQ.13) GO TO 30
   20 CALL ERROR1(45H ILLEGAL USE OF PREVIOUSLY DEFINED IDENTIFIER, 45)
      ERR = .TRUE.
      GO TO 280
C
C     SET TYPE (EXPLICITLY FOR TYPE STMTS)
C
   30 I1 = IGATT1(KK,1)
      IF (ITYP.GE.6) GO TO 50
C
C     TYPE EXPLICITLY
C
      IF (I1.GE.8) GO TO 40
      CALL SATT1(KK, 1, ITYP+7)
      GO TO 60
   40 CALL ERROR1(34H IDENTIFIER TYPED EXPLICITLY TWICE, 34)
      GO TO 60
C
C     TYPE IMPLICITLY
C
   50 IF (I1.GT.0) GO TO 60
      I1 = 1
      IF (STMT(PSTMT).GE.38 .AND. STMT(PSTMT).LE.43) I1 = 2
      CALL SATT1(KK, 1, I1)
C
C     IF NOT ARRAY ELEMENT/ARAY DECLARATOR--RECOGNITION COMPLETE
C
C     CHECK NOT A DUMMY ARG IN COMMON, DATA, EQUIV STMT
C
   60 IF (ARRY) GO TO 80
      IF (IGATT1(KK,7).EQ.0) GO TO 65
      IF (ITYP.EQ.12)
     1 CALL ERROR1(46H WARNING - ILLEGAL USE OF ARRAY IN EQUIVALENCE
     2  , 46)
      IF (ITYP.EQ.13)
     1 CALL ERROR1(39H WARNING - ILLEGAL USE OF ARRAY IN DATA, 39)
   65 CONTINUE
      I1 = IGATT1(KK,4)
      IF (.NOT.(I1.EQ.1 .AND. (ITYP.EQ.8 .OR. ITYP.EQ.12 .OR.
     *    ITYP.EQ.13))) GO TO 70
      ERR = .TRUE.
      CALL ERROR1(32H ILLEGAL USAGE OF DUMMY ARGUMENT, 32)
   70 RETURN
   80 ISIZ = 1
      VAR = .FALSE.
      IF (L.EQ.0) CALL SATT1(KK, 8, 10)
C
C     LOOP TO FIND BOUNDS;  CHECK THAT VARIABLE BOUNDS ARE DUMMY ARGS
C     SET ADJUSTIBLE DIMENSION VARIABLE BIT; SET TYPE IMPLICITLY IF NOT
C     ALREADY SET
C     ACCUMULATE LENGTH IF IN DIMENSION, COMMON, OR TYPE STMT.
C     CHECK FOR REPEAT DIMENSIONING IN THOSE STMTS
C
      L = IGATT1(KK,7)
      IF (ITYP.EQ.13 .OR. ITYP.EQ.12) GO TO 90
      IF (L.EQ.0) GO TO 100
      CALL ERROR1(44H ILLEGAL USE OF PREVIOUSLY DIMENSIONED ARRAY, 44)
      ERR = .TRUE.
      GO TO 270
   90 IF (L.EQ.0) CALL ERROR1(
     *    44H ILLEGAL USE OF ARRAY NOT PREVIOUSLY DEFINED, 44)
  100 IF (K2+1.LT.NSTMT) GO TO 120
  110 CALL ERROR1(28H ILLEGAL ARRAY BOUNDS SYNTAX, 28)
      GO TO 270
C
C     CHECK FOR POSITIVE INTEGER BOUND
C
  120 PSTMT = K2 + 1
      IF (.NOT.TOKPNO(PSTMT,I1,LL)) GO TO 130
      IF (ITYP.EQ.7 .OR. ITYP.EQ.8 .OR. ITYP.LT.6) ISIZ = ISIZ*LL
      IF (ITYP.NE.12) GO TO 170
      IF (I1-K2.NE.2 .OR. STMT(PSTMT).NE.1) CORNER = .FALSE.
      GO TO 170
C
C     SEEK A VARIABLE BOUND
C
  130 CALL NEXTOK(PSTMT, I1, L)
      IF (L.NE.0) GO TO 110
      IF (ITYP.LT.6 .OR. ITYP.EQ.7) GO TO 140
      CALL ERROR1(32H VARIABLE DIMENSION ILLEGAL HERE, 32)
      ERR = .TRUE.
      GO TO 270
  140 VAR = .TRUE.
      L = LOOKUP(I1,.FALSE.)
      IF (SYSERR) GO TO 70
      N = IGATT1(L,8)
      IF (N.NE.0 .AND. N.NE.10) GO TO 20
      I2 = IGATT1(L,4)
      IF (I2.EQ.1) GO TO 150
      CALL ERROR1(42H ILLEGAL USAGE OF VARIABLE IN ARRAY BOUNDS, 42)
      ERR = .TRUE.
      GO TO 270
  150 I2 = IGATT1(KK,4)
      IF (I2.EQ.1) GO TO 160
      CALL ERROR1(50H VARIABLY DIMENSIONED ARRAY MUST BE DUMMY ARGUMENT,
     *    50)
      ERR = .TRUE.
      GO TO 270
  160 CALL SATT1(L, 6, 1)
      CALL SATT1(L, 8, 10)
      N = IGATT1(L,1)
      IF (N.GT.0) GO TO 170
      N = 1
      IF (STMT(K2+1).GE.38 .AND. STMT(K2+1).LE.43) N = 2
      CALL SATT1(L, 1, N)
      GO TO 170
C
C     FIND "," AND ACCUMULATE LENGTH
C
  170 ICNT = ICNT + 1
      IF (ICNT.LE.3) GO TO 180
      ISIZ = ISIZ/LL
      CALL ERROR1(30H WARNING - TOO MANY SUBSCRIPTS, 30)
      ICNT = 3
      FLUSH = .TRUE.
      GO TO 190
  180 K2 = I1
      IF (STMT(K2).EQ.68) GO TO 100
C
C     FIND ")" STORE LENGTH OR -1 INTO ARRAY SYMBOL TABLE ELEMENT
C
      IF (STMT(K2).NE.62) GO TO 110
  190 IF (ITYP.EQ.13 .OR. ITYP.EQ.12) GO TO 260
      CALL SATT1(KK, 7, ICNT)
C
C     STORE LENGTH OF ARRAY
C
      IF (VAR) GO TO 240
      IF (DSA(KK+2).EQ.0) GO TO 200
      N = DSA(KK+2)
      DSA(N) = ISIZ
      GO TO 220
  200 IF (NEXT+2.GE.BNEXT) GO TO 210
      DSA(KK+2) = NEXT
      DSA(NEXT) = ISIZ
      DSA(NEXT+1) = 0
      NEXT = NEXT + 2
      GO TO 220
  210 SYSERR = .TRUE.
      CALL ERROR1(33H IN ARDECL, TABLE OVERFLOW OF DSA,33)
  220 IF (FLUSH) GO TO 270
  230 K2 = K2 + 1
      GO TO 70
C
C     FIXUP FOR VARIABLY DIMENSIONED ARRAYS
C
  240 IF (DSA(KK+2).EQ.0) GO TO 250
      N = DSA(KK+2)
      DSA(N) = -1
      GO TO 220
  250 IF (NEXT+2.GE.BNEXT) GO TO 210
      DSA(KK+2) = NEXT
      DSA(NEXT) = -1
      DSA(NEXT+1) = 0
      NEXT = NEXT + 2
      GO TO 220
C
C     CHECK FORCORNER ELEMENT IN EQUIVALENCE STMT
C
  260 IF (ITYP.NE.12) GO TO 220
      IF (CORNER) KK = -KK
      GO TO 220
C
C     CODE TO FLUSH CONSTRUCT--TO NEXT ")"
C
  270 IF (K2.EQ.NSTMT) GO TO 70
      IF (STMT(K2).EQ.62) GO TO 230
      K2 = K2 + 1
      GO TO 270
C
C     CODE TO FLUSH TO NEXT CONSTRUCT ")",",", "/"
C
  280 K = 90
      IF (ITYP.EQ.8 .OR. ITYP.EQ.13) K = 67
  290 IF (K2.EQ.NSTMT) GO TO 70
      L = STMT(K2)
      IF (L.EQ.65) GO TO 270
      IF (L.EQ.68 .OR. L.EQ.K) GO TO 70
      K2 = K2 + 1
      GO TO 290
      END