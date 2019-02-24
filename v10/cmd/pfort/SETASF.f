      SUBROUTINE SETASF(PP, K)
      INTEGER PP, SYMLEN, PLAT, PDSA, SETARG, PNODE, DSA
      LOGICAL ERR, SYSERR, ABORT
      COMMON /GRAPH/ LLAT, PLAT, LAT(6000)
      COMMON /HEAD/ LNODE, PNODE, NODE(500)
      COMMON /CTABL/ LDSA, PDSA, DSA(5000)
      COMMON /PARAMS/ INUT, I1, NOCHAR, SYMLEN, I2, I3, I4
      COMMON /DETECT/ ERR, SYSERR, ABORT
C
C     SETUP ASF NODE;  IT HAS A NODE JUST LIKE A RTNE
C     EXCEPT ITS INDEX IN NODE IS NEGATIVE
C     PP-COM ADDRESS OF PARENT SUBPGM
C     K-DSA ADDRESS OF ASF ENTRY
C
      IF (PNODE+1.GT.LNODE) GO TO 40
      IF (PLAT+SYMLEN+11.GT.LLAT) GO TO 60
C
C     CREATE NEW NODE ENTRY
C
      NODE(PNODE) = -PLAT
      PNODE = PNODE + 1
C
C     ENTER NAME AND ZERO REST OF NODE
C
      DO 10 I=1,SYMLEN
        L = K + 3 + I
        LL = PLAT + I - 1
        LAT(LL) = DSA(L)
   10 CONTINUE
      DO 20 I=1,6
        L = LL + I
        LAT(L) = 0
   20 CONTINUE
C
C     SET LAST ELEMENT TO TYPE OF PGM UNIT
C     STORE IN SAME WORD ASF TYPE
C
      I = IGATT1(K,1)
      LAT(L+1) = 4 + 8*MOD(I,8)
C
C     SETUP PARENT'S LIST TO POINT TO PP IN ASF NODE
C
      L = PLAT + SYMLEN + 3
      LAT(L) = L + 4
      LAT(L+4) = PP
      LAT(L+5) = 0
      KQ = PLAT
      PLAT = L + 6
C
C     SETUP REFERENCE IN PP'S DESCENDENTS LIST
C
      II = PP + SYMLEN + 4
      LAT(PLAT) = KQ
      LAT(PLAT+1) = LAT(II)
      LAT(II) = PLAT
      PLAT = PLAT + 2
C
C     SETUP ARGUMENTS
C
      L = KQ + SYMLEN
      LAT(L) = SETARG(KQ,K)
   30 RETURN
   40 CALL ERROR1(34H IN SETASF, TABLE OVERFLOW OF NODE, 34)
   50 SYSERR = .TRUE.
      GO TO 30
   60 CALL ERROR1(33H IN SETASF, TABLE OVERFLOW OF LAT, 33)
      GO TO 50
      END
