      SUBROUTINE SCAN(MAINND)
      INTEGER PLAT, SYMLEN, PNODE, STACK
      LOGICAL ERR, SYSERR, ABORT
      COMMON /DETECT/ ERR, SYSERR, ABORT
      COMMON /GRAPH/ LLAT, PLAT, LAT(6000)
      COMMON /PARAMS/ I1, I2, I3, SYMLEN, I4, I5, I6
      COMMON /CEXPRS/ LSTACK, STACK(620)
      COMMON /HEAD/ LNODE, PNODE, NODE(500)
      COMMON/ SCR1/ LINODE, INODE(500)
      COMMON /SCR2/ LICOM, ICOM(500)
C
C     SUBROUTINE PERCOLATES SETTING INFO ABOUT ARGUMENTS AND COMMON
C     UP THE LATTICE---IN ORDER THAT UNSAFE REFS CAN BE CHECKED
C
C
C     STACK(1)-(LSTACK) KEEPS TRACK OF PATH FROM CURRENT TERMINAL NODE
C     TO SUPEROOT NODE
C     INODE(J) IS 0 IF A NODE IS UNVISITED SO FAR ON ALL PATHS
C               1 IF A NODE HAS BEEN VISITED ON AT LEAST ONE PATH
C     SYSERR IS SET BY SCAN
C
      DO 10 I=1,PNODE
        INODE(I) = 0
   10 CONTINUE
      INODE(MAINND) = 1
      MAIN = NODE(MAINND)
      NUM = 0
C
C     CYCLE THROUGH ALL TERMINAL NODES
C
   20 NUM = NUM + 1
      IF (NUM.GT.PNODE-1) GO TO 240
C
C      CHECK IF AN NODE IS ASF OR IF IT HAS DESC
C     OR IF IT HAS NO PARENTS
C
      IF (NODE(NUM).LE.0) GO TO 20
      I = NODE(NUM) + SYMLEN + 4
C
C     NO PARENTS
C
      IF (LAT(I-1).EQ.0) GO TO 20
C
C     TEST DESC FOR BEING ALL ASFS
C
      IF (LAT(I).EQ.0) GO TO 40
      L = LAT(I)
   30 K = LAT(L) + SYMLEN + 6
      IF (MOD(LAT(K),8).NE.4) GO TO 20
      L = LAT(I+1)
      IF (L) 40, 40, 30
C
C     HAVE A TERMINAL NODE;NOW CAN START RECURSIVE TRAVERSE OF ALL
C     PATHS UPWARDS FROM IT  TO ROOT
C     ILEN--POINTER TO TOP OF CURRENT PATH
C     JNODE--CURRENT NODE
C
   40 INODE(NUM) = 1
      ILEN = 2
      STACK(2) = NODE(NUM)
      STACK(1) = 0
C
C     STACK ENTRY IS 1ST WORD-POINTER TO NODE ON LIST OF PARS OFPREV
C     NODE; 2ND WORD-NODE INDEX
C     PROCESS NODE
C     1. CHECK EACH ARG. IF NOT SET OR IF PARENTS ARGLINKS NONEXISTANT
C      SKIP TO NEXT ARG (IF NO ARGS GOTO 2); ELSE MARK EACH PARENT
C      ARGLIST ENTRY AS SET FOR A SET ARG.
C     2. ADD EACH COMMON REGION TO PARENTS' LIST OF COMMON REGIONS
C     3. GET NEW NODE
C
   50 J = STACK(ILEN) + SYMLEN + 1
C
C     ARG PROCESSING
C
      J = LAT(J)
   60 IF (J.EQ.0) GO TO 90
      I = IGATT2(J,5)
      IF (I.NE.1 .OR. LAT(J+2).EQ.0) GO TO 80
      L = LAT(J+2)
   70 IF (L.EQ.0) GO TO 80
C
C     SET PARENT ARGS
C
      CALL SATT2(LAT(L), 5, 1)
      L = LAT(L+1)
      GO TO 70
C
C     GO ON TO NEXT ARG
C
   80 J = LAT(J+3)
      GO TO 60
C
C     COMMON PROCESSING
C
   90 J = STACK(ILEN) + SYMLEN + 2
      II = 0
      J = LAT(J)
C
C     ACCUMULATE COMMON REGIONS
C
  100 IF (J.EQ.0) GO TO 110
      ICOM(II+1) = LAT(J)
      IF (LAT(J+1).NE.0) ICOM(II+1) = -ICOM(II+1)
      II = II + 1
      J = LAT(J+2)
      GO TO 100
  110 IF (II.EQ.0) GO TO 150
C
C     GET PARENT NODE AND ADD COMMON REGIONS TO IT
C
      K = STACK(ILEN) + SYMLEN + 3
      K = LAT(K)
  120 L = LAT(K) + SYMLEN + 2
      DO 140 I=1,II
        LL = MATCH(LAT(L),2,IABS(ICOM(I)))
        IF (LL.EQ.0) GO TO 130
        IF (ICOM(I).LT.0) LAT(LL+1) = 1
        GO TO 140
C
C     COPY COMMONNODE ENTRIES ONTO PARENTS LIST
C
  130   IF (PLAT+3.GT.LLAT) GO TO 270
        LAT(PLAT+2) = LAT(L)
        LAT(PLAT+1) = 0
        LAT(PLAT) = IABS(ICOM(I))
        IF (ICOM(I).LT.0) LAT(PLAT+1) = 1
        LAT(L) = PLAT
        PLAT = PLAT + 3
  140 CONTINUE
C
C     GOONTO NEW PARENT
C
      K = LAT(K+1)
      IF (K.NE.0) GO TO 120
C
C     FIND A PARENT OF THIS NODE AND TRY TO VISIT IT NEXT
C     I CONTAINS POINTER TO PARENT LIST POSITION OF THE PARENT;
C     J CONTAINS PARENTS INDEX IN LAT
C     IF NO MORE PARENTS, MUST BACKUP A LEVEL
C
  150 I = STACK(ILEN) + SYMLEN + 3
  160 IF (LAT(I).EQ.0) GO TO 200
      I = LAT(I)
  170 J = LAT(I)
C
C     CHECK THAT NEW ENTRY HAS PARENTS
C     AND THAT IT IS NOT THE SUPEROOT
C
      K = J + SYMLEN + 3
      IF (LAT(K).GT.0) GO TO 210
C
C     IF THIS PARENT UNACCEPTIBLE GO ONTO NEXT PARENT
C     MARK UNACCEPTIBLE AS VISITED SO WONT BE RECURSIVE
C
      LL = PNODE - 1
      DO 180 L=1,LL
        IF (J.NE.NODE(L)) GO TO 180
        INODE(L) = 1
        GO TO 190
  180 CONTINUE
  190 I = I + 1
      GO TO 160
C
C     MUST BACK DOWN THE PATH TO THE NEXT JUNCTURE WITH
C     AN UNTRIED PATH;  CHECK FIRST FOR DONE WITH ENTIRE PATH
C
  200 IF (STACK(ILEN-1).EQ.0) GO TO 20
      ILEN = ILEN - 2
      J = STACK(ILEN+1)
      IF (LAT(J+1).EQ.0) GO TO 200
C
C     FOUND AN UNTRIED PATH ON THE STACK
C
      I = LAT(J+1)
      GO TO 170
C
C     MARK ENTRY AS VISITED
C
  210 LL = PNODE - 1
      DO 220 L=1,LL
        IF (J.NE.NODE(L)) GO TO 220
        INODE(L) = 1
        GO TO 230
  220 CONTINUE
C
C     ENTER ON STACK
C
  230 IF (ILEN+2.GT.LSTACK) GO TO 260
      STACK(ILEN+1) = I
      STACK(ILEN+2) = J
      ILEN = ILEN + 2
      GO TO 50
  240 RETURN
  250 SYSERR = .TRUE.
      GO TO 240
  260 CALL ERROR1(33H IN SCAN, TABLE OVERFLOW OF STACK, 33)
      GO TO 250
  270 CALL ERROR1(31H IN SCAN, TABLE OVERFLOW OF LAT, 31)
      GO TO 250
      END
