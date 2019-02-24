      SUBROUTINE OUT2(ISR)
      INTEGER SYMLEN, PNODE, BL, PLAT, STACK, Q(3), C(12),
     *    OUTLAT, OUTCOM, OUTUT
      LOGICAL ERR, SYSERR, ABORT
      EXTERNAL EXCH
      COMMON /PARAMS/ INUT, OUTUT, NOCHAR, SYMLEN, I1, I2, II1
      COMMON /DETECT/ ERR, SYSERR, ABORT
      COMMON /GRAPH/ LLAT, PLAT, LAT(6000)
      COMMON /CEXPRS/ LSTACK, STACK(620)
      COMMON /HEAD/ LNODE, PNODE, NODE(500)
      COMMON /SCR1/ LINODE, INODE(500)
      DATA C(1) /1HE/, C(2) /1HD/, C(3) /1HR/, C(4) /1HI/, C(5) /1HC/,
     *    C(6) /1HL/, C(7) /1HS/, C(8) /1HA/, C(9) /1HF/, IP /1HP/, IBL
     *    /1H-/, C(12) /1HU/, C(10) /1HB/, C(11) /1HN/
      DATA BL /1H /
C
C     ROUTINE PRINTS CALLING GRAPH
C
      IF (PNODE.LE.2) GO TO 110
C
C     GRAPH
C
      I3 = PNODE - 1
      IF (ISR.NE.0) I3 = I3 - 1
C
C     SORT LATTICE
C
      DO 10 I=1,I3
        INODE(I) = IABS(NODE(I))
   10 CONTINUE
      CALL SSORT(EXCH, LAT, LLAT, INODE, I3, 0)
      DO 100 IA=1,I3
        I = INODE(IA)
        L = I + SYMLEN + 6
        IF (MOD(LAT(L),8).EQ.4) GO TO 100
        CALL S5UNPK(LAT(I), STACK(1), 6)
        WRITE (OUTUT,99999) (STACK(L),L=1,6)
99999   FORMAT (///1X, 6A1//)
C
C     GET ARGS IF ANY
C
        IS = 1
        K = SYMLEN + I
        L = LAT(K)
        IF (L) 70, 70, 20
   20   K = K + 1
        K = LAT(K)
        IF (L*8.GT.LSTACK) GO TO 120
        DO 60 LL=1,L
          Q(1) = IGATT2(K,8)
          IF (Q(1).EQ.5 .OR. Q(1).EQ.6 .OR. Q(1).EQ.13) GO TO 30
          Q(1) = IGATT2(K,1)
          Q(2) = IGATT2(K,5)
          Q(3) = IGATT2(K,7)
          STACK(IS) = IBL
          STACK(IS+2) = IBL
          IF (Q(1).GE.8) STACK(IS) = C(1)
          L1 = MOD(Q(1),8) + 2
          STACK(IS+1) = C(L1)
          IF (Q(2).EQ.1) STACK(IS+2) = C(7)
          STACK(IS+3) = C(7)
          IF (Q(3).NE.0) STACK(IS+3) = C(8)
          GO TO 40
   30     STACK(IS) = IP
          STACK(IS+1) = IBL
          STACK(IS+2) = IBL
          STACK(IS+3) = IBL
   40     DO 50 LK=4,7
            L1 = LK + IS
            STACK(L1) = BL
   50     CONTINUE
          IS = IS + 8
          K = LAT(K+3)
   60   CONTINUE
      IS = IS - 1
C     PRINT ARGUMENTS
      K = 48
      IF (K.GT.IS) K = IS
      WRITE(OUTUT,99998)(STACK(LK), LK=1,K)
99998 FORMAT(20H ARGUMENT ATTRIBUTES ,5X,6(8A1,1X))
      IF( K.EQ.IS ) GOTO 70
 65   LK = K + 1
      K = LK + 47
      IF(K.GT.IS) K = IS
      WRITE(OUTUT,99997) (STACK(L1),L1=LK,K)
99997 FORMAT(25X,6(8A1,1X))
      IF(K.LT.IS) GOTO 65
C
C     GET COMMON NAMES
C
   70   K = I + SYMLEN + 2
        K = OUTCOM(LAT(K),IS)
        IF (SYSERR) GO TO 110
        IF (K.EQ.0) GO TO 80
      CALL OUT2A(14H COMMON BLOCKS, 14, IS, 2)
C
C     FIND PARENTS
C
   80   K = I + SYMLEN + 3
        K = OUTLAT(LAT(K),IS,ISR)
        IF (SYSERR) GO TO 110
        IF (K.EQ.0) GO TO 90
      CALL OUT2A(22H CALLED BY SUBPROGRAMS, 22, IS, 1)
C
C     FIND DESCENDENTS
C
   90   K = I + SYMLEN + 4
        K = OUTLAT(LAT(K),IS,ISR)
        IF (SYSERR) GO TO 110
        IF (K.EQ.0) GO TO 100
      CALL OUT2A(18H CALLS SUBPROGRAMS, 18, IS, 1)
  100 CONTINUE
  110 RETURN
  120 SYSERR = .TRUE.
      CALL ERROR1(33H IN OUT2, TABLE OVERFLOW OF STACK, 33)
      GO TO 110
      END
