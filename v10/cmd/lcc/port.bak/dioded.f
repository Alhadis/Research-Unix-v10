      LOGICAL FUNCTION DIODED(V, VT, NV, T, DT, D, DV, TJ, DTJ,
     1   GETJAC, SEPATE, USENGJ, USENNS, USENFD, NES, NFS, FNUM, THETA
     2   , IRCS, KEEJAC)
      INTEGER NV
      EXTERNAL D
      INTEGER NES, NFS, FNUM, IRCS, KEEJAC
      LOGICAL GETJAC, SEPATE, USENGJ, USENNS, USENFD
      DOUBLE PRECISION V(NV), VT(NV), T, DT, DV(NV), TJ
      DOUBLE PRECISION DTJ, THETA
      COMMON /CSTAK/ DS
      DOUBLE PRECISION DS(500)
      COMMON /D1ODES/ IMEM
      INTEGER IMEM
      COMMON /DIODEJ/ AJ, BJ, GETACC, NEEUMC
      LOGICAL GETACC, NEEUMC
      DOUBLE PRECISION AJ, BJ
      COMMON /DIODEF/ FAILED
      LOGICAL FAILED
      INTEGER ID43, IDV, LDV, IDELVT, ISTKGT, NERROR
      INTEGER MIN0, MAX0, I, J, NERR, IDVT
      INTEGER LDVT, I0, I1, IS(1000), IDELV, ID4(2)
      INTEGER ID1
      REAL RS(1000)
      LOGICAL DIODEG, GETDVT, GETACT, NEESUM, LS(1000)
      DOUBLE PRECISION DABS, TEMP, DMAX1, WS(500), DUMMY(100)
      INTEGER TEMP1, TEMP2, TEMP3, TEMP4
      LOGICAL TEMP5, TEMP6
      EQUIVALENCE (DS(1), WS(1), RS(1), IS(1), LS(1))
C SCRATCH SPACE ALLOCATED - S(DIODED) =
C     NV**2 + NV*(2*NV+1) +
C     MAX ( S(D), 2*NV + NV INTEGER )
C LONG REAL WORDS.
C A DUMMY ARRAY FOR VOID DV AND DVT.
C THE PORT LIBRARY STACK AND ITS ALIASES.
      LDV = NV**2
C THE LENGTHS OF THE DV AND DVT ARRAYS.
      LDVT = NV**2
      IF (IMEM .LE. 0) GOTO 1
         ID4(1) = IS(IMEM+3)
         ID4(2) = IS(IMEM+4)
   1  TEMP5 = NFS .EQ. (-1)
      IF (TEMP5) TEMP5 = KEEJAC .GT. 1
      IF (TEMP5) GOTO 2
         TEMP6 = NFS .EQ. (-2)
         IF (TEMP6) TEMP6 = KEEJAC .EQ. 1
         TEMP5 = TEMP6
   2  IF (.NOT. TEMP5) GOTO 3
         IMEM = ISTKGT(10, 2)
C INITIALIZE.
C D, DIAG, PIVOT, D41, D42, R, CA, RCA, CB AND RCB.
         IS(IMEM) = ISTKGT(LDV+3*NV+1, 4)
         IS(IMEM+1) = IS(IMEM)+LDV
         IS(IMEM+3) = 0
         IS(IMEM+4) = 0
         IS(IMEM+5) = IS(IMEM+1)+NV
         IS(IMEM+6) = IS(IMEM+5)+NV
         IS(IMEM+8) = IS(IMEM+6)+NV
         IS(IMEM+2) = ISTKGT(2*NV+1, 2)
         IS(IMEM+7) = IS(IMEM+2)+NV
         IS(IMEM+9) = IS(IMEM+7)+NV
         GOTO  4
   3     IF (IMEM .EQ. 0) IMEM = 1
C SIGNAL THAT KNOW ABOUT D1ODES.
   4  TEMP5 = NFS .EQ. (-1)
      IF (TEMP5) TEMP5 = KEEJAC .GT. 1
      IF (.NOT. TEMP5) GOTO 5
         IS(IMEM+3) = ISTKGT(LDV+LDVT, 4)
C THE JACOBIAN PARTS ARE GLOBAL.
         IS(IMEM+4) = IS(IMEM+3)+LDV
   5  IF (NFS .GE. 0) GOTO 6
         DIODED = .TRUE.
         RETURN
   6  CALL ENTER(1)
      NEESUM = DTJ .NE. DT
      NEESUM = NEESUM .AND. SEPATE
      IF (KEEJAC .NE. 0) GOTO 11
         IMEM = ISTKGT(2*NV+11, 2)
C NEED STACK FRAME.
         IF (USENNS) GOTO 7
            ID4(1) = ISTKGT(LDV+LDVT+3*NV+1, 4)
            ID4(2) = ID4(1)+LDV
            GOTO  8
   7        ID4(1) = ISTKGT(LDV+3*NV+1, 4)
            ID4(2) = 0
   8     IS(IMEM) = ID4(1)
         ID43 = ID4(1)
         IF (USENNS) GOTO 9
            IS(IMEM+1) = ID4(2)+LDVT
            GOTO  10
   9        IS(IMEM+1) = ID4(1)+LDV
  10     IS(IMEM+5) = IS(IMEM+1)+NV
         IS(IMEM+6) = IS(IMEM+5)+NV
         IS(IMEM+8) = IS(IMEM+6)+NV
         IS(IMEM+2) = IMEM+10
         IS(IMEM+7) = IS(IMEM+2)+NV
         IS(IMEM+9) = IS(IMEM+7)+NV
         GOTO  20
  11     IF (KEEJAC .NE. 1) GOTO 16
            IF (.NOT. GETJAC) GOTO 14
               ID4(1) = IS(IMEM)
               ID43 = ID4(1)
               IF (.NOT. USENNS) GOTO 12
                  ID4(2) = 0
                  GOTO  13
  12              ID4(2) = ISTKGT(LDVT, 4)
  13           CONTINUE
               GOTO  15
  14           ID4(1) = 0
               ID4(2) = 0
               ID43 = 0
  15        CONTINUE
            GOTO  19
  16        TEMP5 = GETJAC
            IF (.NOT. TEMP5) TEMP5 = NEESUM
            IF (.NOT. TEMP5) GOTO 17
               ID43 = IS(IMEM)
               GOTO  18
  17           ID43 = 0
  18        CONTINUE
  19  CONTINUE
  20  NEESUM = DT .NE. DTJ
      NEESUM = NEESUM .AND. SEPATE
      TEMP5 = GETJAC
      IF (TEMP5) TEMP5 = .NOT. SEPATE
      NEESUM = NEESUM .OR. TEMP5
      IF ((.NOT. SEPATE) .OR. (.NOT. GETJAC)) GOTO 27
         AJ = 1
         BJ = 1
         GETACC = GETJAC
         NEEUMC = .FALSE.
C DEFAULT VALUES.
         CALL SETD(NV, 0D0, DV)
         TEMP4 = ID4(1)
         CALL SETD(LDV, 0D0, WS(TEMP4))
         TEMP4 = ID4(2)
         CALL SETD(LDVT, 0D0, WS(TEMP4))
         IF (USENFD) GOTO 21
            FAILED = .FALSE.
            NES = NES+1
            TEMP4 = ID4(1)
            TEMP3 = ID4(2)
            CALL D(TJ, V, VT, NV, DV, WS(TEMP4), WS(TEMP3))
            GOTO  23
  21        GETACC = .FALSE.
            IDELV = ISTKGT(3*NV, 4)
            IDELVT = IDELV+NV
            ID1 = IDELVT+NV
            TEMP3 = ID4(1)
            TEMP4 = ID4(2)
            IF (DIODEG(D, TJ, V, VT, NV, WS(IDELV), WS(IDELVT), DV, NES,
     1         FNUM, NV, LDV, LDVT, WS(TEMP3), WS(TEMP4), .TRUE., DUMMY,
     2         100, WS(ID1))) GOTO 22
               CALL LEAVE
               DIODED = .TRUE.
               RETURN
  22        CALL ISTKRL(1)
  23     IF (.NOT. FAILED) GOTO 24
            FNUM = 1
            CALL LEAVE
            DIODED = .TRUE.
            RETURN
  24     IF (THETA .EQ. 1D0) GOTO 26
            TEMP4 = ID4(1)
            TEMP3 = ID4(1)+LDV-1
            DO  25 I = TEMP4, TEMP3
               WS(I) = WS(I)*THETA
  25           CONTINUE
  26     CONTINUE
  27  GETACT = GETJAC .AND. (.NOT. SEPATE)
      TEMP5 = USENNS
      IF (.NOT. TEMP5) TEMP5 = USENFD
      IF (.NOT. TEMP5) GOTO 28
         AJ = THETA
         BJ = DT
         GOTO  29
  28     AJ = 1
         BJ = 1
  29  GETACC = GETACT
      TEMP5 = NEESUM
      IF (TEMP5) TEMP5 = USENNS
      NEEUMC = TEMP5
      IF (.NOT. USENFD) GOTO 30
         GETDVT = KEEJAC .GT. 1
         GOTO  31
  30     GETDVT = .TRUE.
  31  CALL SETD(NV, 0D0, DV)
      IF (.NOT. GETACT) GOTO 43
         CALL SETD(LDV, 0D0, WS(ID43))
         IF (ID4(2) .LE. 0) GOTO 32
            TEMP3 = ID4(2)
            CALL SETD(LDVT, 0D0, WS(TEMP3))
            GOTO  33
  32        CALL SETD(MIN0(100, LDVT), 0D0, DUMMY)
  33     IF (SEPATE) NEEUMC = .FALSE.
         FAILED = .FALSE.
         IF (ID4(2) .LE. 0) GOTO 37
            IF (USENFD) GOTO 34
               NES = NES+1
               TEMP3 = ID4(2)
               CALL D(T, V, VT, NV, DV, WS(ID43), WS(TEMP3))
               GOTO  36
  34           GETACC = .FALSE.
               IDELV = ISTKGT(3*NV, 4)
               IDELVT = IDELV+NV
               ID1 = IDELVT+NV
               TEMP3 = ID4(2)
               IF (DIODEG(D, T, V, VT, NV, WS(IDELV), WS(IDELVT), DV,
     1            NES, FNUM, NV, LDV, LDVT, WS(ID43), WS(TEMP3), GETDVT,
     2            DUMMY, 100, WS(ID1))) GOTO 35
                  CALL LEAVE
                  DIODED = .TRUE.
                  RETURN
  35           CALL ISTKRL(1)
  36        CONTINUE
            GOTO  42
  37        IF (USENFD) GOTO 39
               NES = NES+1
               CALL D(T, V, VT, NV, DV, WS(ID43), DUMMY)
               TEMP3 = MIN0(100, LDVT)
               DO  38 I = 1, TEMP3
C/6S
C                 IF (DUMMY(I) .NE. 0D0) CALL SETERR(
C    1               38HDIODED - D USED DVT WHEN USENNS = TRUE, 38, 1, 2
C    2               )
C/7S
                  IF (DUMMY(I) .NE. 0D0) CALL SETERR(
     1               'DIODED - D USED DVT WHEN USENNS = TRUE', 38, 1, 2
     2               )
C/
  38              CONTINUE
               GOTO  41
  39           GETACC = .FALSE.
               IDELV = ISTKGT(2*NV, 4)
               ID1 = IDELV+NV
               IF (DIODEG(D, T, V, VT, NV, WS(IDELV), DUMMY, DV, NES,
     1            FNUM, NV, LDV, LDVT, WS(ID43), DUMMY, GETDVT, DUMMY,
     2            100, WS(ID1))) GOTO 40
                  CALL LEAVE
                  DIODED = .TRUE.
                  RETURN
  40           CONTINUE
  41        CONTINUE
  42     CONTINUE
         GOTO  51
  43     IF (.NOT. USENGJ) GOTO 44
            IDV = 0
            IDVT = 0
            CALL SETD(MIN0(100, MAX0(LDV, LDVT)), 0D0, DUMMY)
            GOTO  47
  44        IF (USENNS) GOTO 45
               IDV = ISTKGT(LDV+LDVT, 4)
               IDVT = IDV+LDV
C DEFAULT VALUES.
               CALL SETD(LDV+LDVT, 0D0, WS(IDV))
               GOTO  46
  45           IDV = ISTKGT(MAX0(LDV, LDVT), 4)
               IDVT = IDV
C DEFAULT VALUES.
               CALL SETD(MAX0(LDV, LDVT), 0D0, WS(IDV))
  46        CONTINUE
  47     FAILED = .FALSE.
         NES = NES+1
         IF (IDV .LE. 0) GOTO 48
            CALL D(T, V, VT, NV, DV, WS(IDV), WS(IDVT))
            GOTO  50
  48        CALL D(T, V, VT, NV, DV, DUMMY, DUMMY)
            TEMP3 = MIN0(100, MAX0(LDV, LDVT))
            DO  49 I = 1, TEMP3
C/6S
C              IF (DUMMY(I) .NE. 0D0) CALL SETERR(
C    1            48HDIODED - D USED DV AND/OR DVT WHEN USENGJ = TRUE,
C    2            48, 2, 2)
C/7S
               IF (DUMMY(I) .NE. 0D0) CALL SETERR(
     1            'DIODED - D USED DV AND/OR DVT WHEN USENGJ = TRUE',
     2            48, 2, 2)
C/
  49           CONTINUE
  50     IF (.NOT. USENGJ) CALL ISTKRL(1)
  51  IF (.NOT. FAILED) GOTO 52
         FNUM = 1
         CALL LEAVE
         DIODED = .TRUE.
         RETURN
  52  DO  53 I = 1, NV
         DV(I) = -DV(I)
  53     CONTINUE
      TEMP5 = GETACT
      IF (TEMP5) TEMP5 = .NOT. USENNS
      IF (TEMP5) TEMP5 = .NOT. USENFD
      IF (TEMP5) TEMP5 = THETA .NE. 1D0
      IF (.NOT. TEMP5) GOTO 55
         TEMP3 = ID43+LDV-1
         DO  54 I = ID43, TEMP3
            WS(I) = WS(I)*THETA
  54        CONTINUE
  55  TEMP5 = NEESUM
      IF (.NOT. TEMP5) GOTO 56
         TEMP6 = .NOT. USENNS
         IF (TEMP6) TEMP6 = .NOT. USENFD
         IF (.NOT. TEMP6) TEMP6 = SEPATE
         TEMP5 = TEMP6
  56  IF (.NOT. TEMP5) GOTO 58
         I0 = ID4(1)
         I1 = ID4(2)
         TEMP3 = ID43+LDV-1
         DO  57 I = ID43, TEMP3
            WS(I) = WS(I0)+WS(I1)/DT
            I0 = I0+1
            I1 = I1+1
  57        CONTINUE
  58  IF ((.NOT. NEESUM) .AND. (.NOT. GETJAC)) GOTO 64
         NFS = NFS+1
         IF (IRCS .NE. 1) GOTO 62
            DO  60 I = 1, NV
C SCALE THE JACOBIAN.
C FIRST, GET THE ROW SCALE FACTORS.
               TEMP = 0
               TEMP3 = IS(IMEM)+I-1
               TEMP4 = IS(IMEM)+NV**2-1
               DO  59 J = TEMP3, TEMP4, NV
                  TEMP = DMAX1(TEMP, DABS(WS(J)))
  59              CONTINUE
               TEMP4 = IS(IMEM+5)+I
               WS(TEMP4-1) = TEMP
  60           CONTINUE
C COLUMN SCALE FACTORS.
            TEMP4 = IS(IMEM)
            TEMP3 = IS(IMEM+5)
            TEMP2 = IS(IMEM+6)
            TEMP1 = IS(IMEM+7)
            CALL DRCSC(WS(TEMP4), NV, NV, WS(TEMP3), WS(TEMP2), IS(
     1         TEMP1))
C SCALE THE JACBOBIAN.
            TEMP1 = IS(IMEM)
            TEMP2 = IS(IMEM+5)
            TEMP3 = IS(IMEM+6)
            TEMP4 = IS(IMEM+7)
            CALL DRCSS(WS(TEMP1), NV, NV, WS(TEMP2), WS(TEMP3), IS(
     1         TEMP4))
            IF (NERROR(NERR) .EQ. 0) GOTO 61
               CALL ERROFF
C/6S
C              CALL SETERR(
C    1            43HDIODED - MUST HAVE 1/(S*L) IN MACHINE RANGE, 43, 3,
C    2            2)
C/7S
               CALL SETERR(
     1            'DIODED - MUST HAVE 1/(S*L) IN MACHINE RANGE', 43, 3,
     2            2)
C/
  61        CONTINUE
C FACTOR JACOBIAN.
  62     TEMP4 = IS(IMEM)
         TEMP3 = IS(IMEM+1)
         TEMP2 = IS(IMEM+2)
         CALL DQRD(NV, NV, WS(TEMP4), WS(TEMP3), IS(TEMP2))
         IF (NERROR(NERR) .EQ. 0) GOTO 63
            CALL ERROFF
            FNUM = 2
            CALL LEAVE
            DIODED = .TRUE.
            RETURN
  63     CONTINUE
  64  IF (IRCS .NE. 1) GOTO 65
         TEMP2 = IS(IMEM+5)
C SCALE FACTORS FOR THE RHS.
         TEMP3 = IS(IMEM+8)
         TEMP4 = IS(IMEM+9)
         CALL DRCSC(DV, NV, 1, WS(TEMP2), WS(TEMP3), IS(TEMP4))
C SCALE THE RHS.
         TEMP4 = IS(IMEM+5)
         TEMP3 = IS(IMEM+8)
         TEMP2 = IS(IMEM+9)
         CALL DRCSB(DV, NV, 1, WS(TEMP4), WS(TEMP3), IS(TEMP2))
C SOLVE JACOBIAN * DV = RHS.
  65  TEMP2 = IS(IMEM)
      TEMP3 = IS(IMEM+1)
      TEMP4 = IS(IMEM+2)
      CALL DQRQTB(NV, NV, WS(TEMP2), WS(TEMP3), IS(TEMP4), 1, DV, DV)
C/6S
C     IF (NERROR(NERR) .NE. 0) CALL SETERR(
C    1   32HDIODED - SINGULAR DJAC IN DQRQTB, 32, 4, 2)
C/7S
      IF (NERROR(NERR) .NE. 0) CALL SETERR(
     1   'DIODED - SINGULAR DJAC IN DQRQTB', 32, 4, 2)
C/
      IF (IRCS .NE. 1) GOTO 68
         TEMP4 = IS(IMEM+6)
C UN-SCALE THE SOLUTION.
         TEMP3 = IS(IMEM+7)
         TEMP2 = IS(IMEM+8)
         TEMP1 = IS(IMEM+9)
         CALL DRCSX(DV, NV, 1, WS(TEMP4), IS(TEMP3), WS(TEMP2), IS(
     1      TEMP1))
         IF (NERROR(NERR) .EQ. 0) GOTO 67
            IF (NERR .NE. 8) GOTO 66
               CALL ERROFF
C/6S
C              CALL SETERR(26HDIODED - DV HAS OVERFLOWED, 26, 5, 2)
C/7S
               CALL SETERR('DIODED - DV HAS OVERFLOWED', 26, 5, 2)
C/
  66        CONTINUE
  67     CONTINUE
  68  CALL LEAVE
      DIODED = .FALSE.
      RETURN
      END
