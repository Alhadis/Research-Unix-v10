      SUBROUTINE INVOKE
C
C      PGM UNIT STEPS THROUGH NODES IN INVOCATION ORDER
C      PUSHING ACTUAL PROC ARGS DOWN LATTICE WHERE NECESSARY
C      AND READJUSTING LEVEL IF NECESSARY
C
      INTEGER PNODE, PLAT, PDSA, DSA, SYMLEN, PREF, REF, FIND, CHK1,
     *  ZERO(1), FINDND
      LOGICAL ERR, SYSERR, ABORT, OK, AOK
      COMMON /PARAMS/ I1, I2, I3, SYMLEN, I4, I5, I6
      COMMON /DETECT/ ERR, SYSERR, ABORT
      COMMON /HEAD/ LNODE, PNODE, NODE(500)
      COMMON /SCR1/ LINODE, INODE(500)
      COMMON /GRAPH/ LLAT, PLAT, LAT(6000)
      COMMON /CTABL/ LDSA, PDSA, DSA(5000)
      COMMON /CREF/ LREF, PREF, REF(100)
      COMMON /FACTS/ NAME, NOST, ITYPE, IASF
      DATA ZERO(1)/0/
C      NC IS CURRENT NODE, NP IS PREVIOUS NODE PROCESSED
      NC = 0
   10 NP = NC
      OK = .TRUE.
      AOK = .TRUE.
C
C      SEARCH FOR NEXT NODE TO DO, NODE WITH LOWEST POSIT LEVEL IN INODE
C      UPON ENTRY TO INVOKE, SUPEROOT IS -1, ASFS AND BLOCK DATA
C      ARE -2. IF CANT FIND A POSITIVE LEVEL ARE DONE
C
      L = PNODE - 1
      DO 20 I=1,L
        IF (INODE(I).LT.0) GO TO 20
        NC = I
        GO TO 40
   20 CONTINUE
   30 RETURN
   40 J = NC
      DO 50 I=J,L
        IF (INODE(I).GE.0 .AND. INODE(I).LT.INODE(NC)) NC = I
   50 CONTINUE
C      READ IN SYMBOL TABLE FOR NODE(NC) AND POSITION REFS CORRECTLY
      CALL RDSYM(NC, NP)
   60 IF (INREF(I6)) 140, 140, 70
C      HAVE A REFERENCE TO PROCESS
   70 IF (IGATT1(REF(2),4).EQ.1) GO TO 80
C      PROCESSING A DIRECT REFERENCE
C      NEED ONLY PROCESS REF TEMPLATE IF ANY PROC ACTUAL ARGS IN REF
      L = REF(2)
      L = FINDND(DSA(L+4),K)
      CALL PROC(NODE(NC), L, K, AOK)
      IF (SYSERR .OR. ABORT) GO TO 30
      GO TO 60
C      PROCESSING AN INDIRECT REF
   80 K = NODE(NC) + SYMLEN + 5
      K = LAT(K)
      IF (K.EQ.0) GO TO 150
C      K PTS TO A TEMPLATE OF ACTUALS AT LAT(NODE(NC))
C      L GIVES REL POSIT AMONG PROC DUMMIES IN LAT(NODE(NC))
C      OF PROC DUMMY BEING CALLED
      L = REF(2)
      L = DSA(L+2)
      I = NODE(NC) + SYMLEN + 1
      I = LAT(I)
      IF (L.EQ.1) GO TO 100
      DO 90 M=2,L
        I = LAT(I+3)
   90 CONTINUE
C
C      I PTS TO DUMMY PROC ARG ENTRY IN LAT
C
  100 M = LAT(I+1) + K
      M = LAT(M)
C      M PTS TO ACTUAL SUBSTITUTABLE FOR I
      L = FIND(M)
C      RECURSION DUE TO INDIRECT REF COMPLETING THE LOOP
      IF (-1.NE.INODE(L)) GO TO 110
      ABORT = .TRUE.
      CALL ERROR2(26H RECURSIVE LOOP INVOLVING , 26, DSA(NAME+4), 1,1,0)
      CALL ERROR2(14H AND POSSIBLY , 14, LAT(M), 1, 0, 1)
      GO TO 30
C      NOTE NEED NOT WORRY ABOUT MISSING SUBPGM SINCE
C      THEN ITS LAT INDEX COULDNOT BE IN TEMPLATE
  110 IF (CHK1(NODE(NC),M).EQ.0) GO TO 130
C      PROCESSED A LEGAL INDIRET REF
      CALL SETPD(M, NODE(NC))
      IF (SYSERR) GO TO 30
      IF (-2.EQ.INODE(L) .OR. INODE(NC).LT.INODE(L)) GO TO 120
      INODE(L) = INODE(NC) + 1
      CALL ASLEV(L)
      IF (ABORT .OR. SYSERR) GO TO 30
C      LOOK FOR MORE ACTUALS
 120  CALL PROC(NODE(NC), M, L, AOK)
      IF (SYSERR .OR. ABORT) GO TO 30
  130 K = LAT(K) + K
      K = LAT(K)
      IF (K) 60, 60, 100
C      MARK CURRENT NODE DONE
  140 INODE(NC) = -1
      GO TO 10
  150 IF (.NOT.OK) GO TO 60
      K = NODE(NC)
      CALL ERROR2(20H NO ACTUAL PROCS IN ,20, LAT(K), 1, 1, 0)
      CALL ERROR2(28H CANNOT PROCESS FORMAL REFS , 28,
     *  ZERO, -3, 0, 1)
      OK = .FALSE.
      GO TO 60
      END