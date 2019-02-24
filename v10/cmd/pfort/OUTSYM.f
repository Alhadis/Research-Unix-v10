      SUBROUTINE OUTSYM
      INTEGER HASH, STACK, BL, DSA, OUTUT, SYMHD, BNEXT, SYMLEN, ATT(8)
      INTEGER CODE(11), CC(30), C(4), Q(70), SYM, PDSA
      INTEGER OUTUT2, OUTUT3, OUTUT4
      LOGICAL OK
      LOGICAL OPT, P1ERR, COMM
      COMMON /CHASH/ LHASH, HASH(401)
      COMMON /OPTNS/ OPT(5), P1ERR
      COMMON /CTABL/ LDSA, PDSA, DSA(5000)
      COMMON /PARAMS/ INUT, OUTUT, NOCHAR, SYMLEN, OUTUT2, OUTUT3,
     *    OUTUT4
      COMMON /TABL/ NEXT, LAB, SYM, BNEXT
      COMMON /FACTS/ NAME, NOST, ITYP, IASF
      COMMON /CEXPRS/ LSTACK, STACK(620)
      COMMON /TRANS/ Q
      DATA BL /1H /
      DATA CODE(1) /1HD/, CODE(2) /1HR/, CODE(3) /1HI/, CODE(4) /1HC/,
     *    CODE(5) /1HL/, CODE(11) /1HE/, CODE(7) /1HA/, CODE(8) /1HS/,
     *    CODE(9) /1HF/, CODE(10) /1HN/, CODE(6) /1HH/, C(1) /4/, C(2)
     *    /11/, C(3) /7/, C(4) /8/
      DATA CC(2), CC(16), CC(20), CC(22), CC(26), CC(28) /6*1H /,
     *    CC(3), CC(5), CC(9), CC(11) /1HF,1HS,1HF,1HF/, CC(1), CC(4) /
     *    1HU,1HA/, CC(6), CC(8), CC(10), CC(12), CC(14) /1HF,4*1HN/,
     *    CC(7), CC(13) /2*1HS/, CC(15) /1HC/, CC(17) /1HG/, CC(18) /
     *    1HT/, CC(19) /1HL/, CC(21) /1HV/, CC(25) /1HM/, CC(23) /1HB/,
     *    CC(24) /1HD/, CC(27) /1HE/
      DATA CC(29) /1HI/, CC(30) /1HF/
C
C     ROUTINE PRINTS OUT SYMBOL TABLE FOR A PROGRAM UNIT
C
      IF (NAME.EQ.0 .OR. .NOT.OPT(1)) RETURN
      II = IGATT1(NAME,8)
      CALL S5UNPK(DSA(NAME+4), STACK(1), 6)
      WRITE (OUTUT,99999) (STACK(I),I=1,6)
99999 FORMAT (14H1PROGRAM UNIT , 5X, 6A1)
      IF (II.EQ.11 .OR. II.EQ.12 .OR. DSA(NAME+2).EQ.0) GO TO 60
C
C     PRINT FCN/SUBROUTINE ARGS
C
      KK = DSA(NAME+2)
      I = 0
   10 L = 20
      CALL RDLIST(KK, 9, M, 0)
      IF (M) 20, 60, 20
   20 DO 30 I1=1,M
        J = STACK(I1) + 4
        CALL S5UNPK(DSA(J), STACK(L), 6)
        L = L + 7
        STACK(L-1) = BL
   30 CONTINUE
      MM = M*7 + 19
      IF (I) 40, 40, 50
   40 WRITE (OUTUT,99998) (STACK(L),L=20,MM)
99998 FORMAT (//10H ARGUMENTS, 9X, 63A1)
      I = 1
      GO TO 10
   50 WRITE (OUTUT,99997) (STACK(L),L=20,MM)
99997 FORMAT (19X, 63A1)
      GO TO 10
C
C     PRINT SYMBOLS FOR PROGRAM UNIT
C
   60 CALL SORT(SYM, LBR)
      COMM = .FALSE.
      WRITE (OUTUT,99996)
99996 FORMAT (//1X, 4HNAME, 5X, 4HTYPE, 2X, 3HUSE, 1X, 10HATTRIBUTES,
     *    1X, 10HREFERENCES//)
      DO 230 JBR=1,LBR
        SYMHD = HASH(JBR)
        DO 70 I=20,35
          STACK(I) = BL
   70   CONTINUE
        DO 80 I=1,8
          ATT(I) = IGATT1(SYMHD,I)
   80   CONTINUE
C     SKIPS OVER SYMBOL TABLE ENTRY FOR MAIN, BLOCK DATA,
C     AND CURRENT SUBROUTINE NAME
        IF (SYMHD.EQ.NAME .AND. ATT(8).NE.4) GO TO 230
        IF (ATT(8).NE.7) GO TO 90
        COMM = .TRUE.
        GO TO 230
   90   CALL S5UNPK(DSA(SYMHD+4), STACK(20), 6)
        I1 = ATT(8)
        L = 2*(I1+1) - 1
        STACK(28) = CC(L)
        STACK(29) = CC(L+1)
C     LEAVE BLANK IRRELEVANT TYPE INFO FOR EXT SUBR, COMMON, EXT ENTS
      IF(ATT(8).EQ.6 .OR. ATT(8).EQ.7 .OR. ATT(8).EQ.13)
     1 GOTO 100
      I1 = MOD(ATT(1),8)
      IF (ATT(1).GE.8) STACK(26)=CODE(11)
      STACK(27) = CODE(I1 + 1)
  100   DO 110 I=1,4
          L = I + 29
          J = C(I)
          IF (ATT(I+1).EQ.1) STACK(L) = CODE(J)
  110   CONTINUE
        IF (ATT(8).EQ.7) STACK(30) = BL
        IF (ATT(8).NE.10 .AND. ATT(8).NE.8) GO TO 140
        IF (ATT(7)) 120, 130, 120
  120   STACK(34) = CODE(7)
        J = ATT(7) + 1
        STACK(35) = Q(J)
        GO TO 140
  130   STACK(34) = CODE(8)
C
C     XREF LIST
C
  140   IF (OPT(2)) GO TO 160
  150   WRITE (OUTUT,99995) (STACK(L),L=20,35)
        GO TO 230
  160   OK = .FALSE.
        N = DSA(SYMHD+1)
        IF (N.LE.0) GO TO 150
      N = DSA( N+1 )
 170  CALL RFLIST( N, M, J, DSA(SYMHD+1) )
C
C     FIRST TIME PRINT WHOLE LINE
C
        K = M
        IF (M.GE.57) K = 57
        WRITE (OUTUT,99995) (STACK(L),L=20,35), (STACK(L),L=50,K)
99995   FORMAT (1X, 6A1, 5X, 2A1, 3X, 2A1, 3X, 6A1, 2X, 8(I5, 1X))
        IF (M-57) 220, 220, 180
  180   L = (M-57)/8
        LL = 58
        IF (L) 220, 210, 190
  190   DO 200 K=1,L
          LK = LL + 7
          WRITE (OUTUT,99994) (STACK(I),I=LL,LK)
          LL = LK + 1
  200   CONTINUE
        IF (LK.EQ.M) GO TO 220
  210   WRITE (OUTUT,99994) (STACK(I),I=LL,M)
99994   FORMAT (30X, 8(I5, 1X))
C
C     MAY HAVE TO CALL REFLIST AGAIN
C
  220   IF (J) 230, 230, 170
  230 CONTINUE
C
C     PRINT LABELS
C
      IF (LAB.EQ.0) GO TO 320
      CALL SORT(LAB, LBR)
      DO 310 JBR=1,LBR
        LABHD = HASH(JBR)
        CALL S5UNPK(DSA(LABHD+4), STACK(20), 6)
        OK = .FALSE.
        IF (OPT(2)) GO TO 240
        WRITE (OUTUT,99993) (STACK(L),L=20,25)
        GO TO 310
  240   II = DSA(LABHD+1)
      II = DSA(II+1)
 250  CALL RFLIST(II, M, J, DSA(LABHD+1) )
        K = M
        IF (M.GE.57) K = 57
        WRITE (OUTUT,99993) (STACK(I),I=20,25), (STACK(I),I=50,K)
99993   FORMAT (1X, 6A1, 23X, 8(I5, 1X))
        IF (M-57) 300, 300, 260
  260   L = (M-57)/8
        LL = 58
        IF (L) 300, 290, 270
  270   DO 280 K=1,L
          LK = LL + 7
          WRITE (OUTUT,99992) (STACK(I),I=LL,LK)
          LL = LK + 1
  280   CONTINUE
        IF (LK.EQ.M) GO TO 300
  290   WRITE (OUTUT,99992) (STACK(I),I=LL,M)
99992   FORMAT (30X, 8(I5, 1X))
  300   IF (J) 310, 310, 250
  310 CONTINUE
  320 IF (.NOT.COMM) GO TO 390
      CALL SORT(SYM, LBR)
      WRITE (OUTUT,99991)
99991 FORMAT (//14H COMMON BLOCKS//)
      DO 380 JBR=1,LBR
        SYMHD = HASH(JBR)
        I = IGATT1(SYMHD,8)
        IF (I.NE.7) GO TO 380
        CALL S5UNPK(DSA(SYMHD+4), STACK(100), 6)
        N = 0
        II = DSA(SYMHD+2)
  330   L = 11
        CALL RDLIST(II, 10, M, 0)
        IF (M) 340, 380, 340
  340   DO 350 I=1,M
          J = STACK(I) + 4
          CALL S5UNPK(DSA(J), STACK(L), 6)
          L = L + 7
          STACK(L-1) = BL
  350   CONTINUE
        L = L - 1
        IF (N) 360, 360, 370
  360   WRITE (OUTUT,99990) (STACK(I),I=100,105), (STACK(I),I=11,L)
99990   FORMAT (1X, 6A1, 3X, 70A1)
        N = 1
        GO TO 330
  370   WRITE (OUTUT,99989) (STACK(K),K=11,L)
99989   FORMAT (10X, 70A1)
        GO TO 330
  380 CONTINUE
  390 RETURN
      END
