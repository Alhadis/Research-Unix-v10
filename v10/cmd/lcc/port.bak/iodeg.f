      LOGICAL FUNCTION IODEG(D, T, V, VT, NV, DELV, DELVT, D0,
     1   NES, FNUM, LDIM, LDV, LDVT, DV, DVT, GETDVT, DUMMY, LDUMMY, D1)
      INTEGER LDUMMY, LDIM, NV
      EXTERNAL D
      INTEGER NES, FNUM, LDV, LDVT
      REAL T, V(NV), VT(NV), DELV(NV), DELVT(NV), D0(NV)
      REAL DV(LDIM, 1), DVT(LDIM, 1), DUMMY(LDUMMY), D1(NV)
      LOGICAL GETDVT
      COMMON /IODEJ/ AJ, BJ, GETJAC, NEESUM
      REAL AJ, BJ
      LOGICAL GETJAC, NEESUM
      COMMON /IODEF/ FAILED
      LOGICAL FAILED
      INTEGER MIN0, MAX0, I, J
      REAL BIG, ABS, DELVTI, VTSAVE, SQRT, DELVI
      REAL VSAVE, SQRTR, R1MACH
      INTEGER TEMP1
      LOGICAL TEMP
      DATA SQRTR/0E0/
      DATA BIG/0E0/
C THE FINITE-DIFFERENCE JACOBIAN GETTER FOR IODE.
      CALL SETR(LDUMMY, 0E0, DUMMY)
      IF (SQRTR .EQ. 0.) SQRTR = SQRT(R1MACH(4))
C SQRT( MACHINE PRECISION ).
      IF (BIG .EQ. 0.) BIG = R1MACH(2)
C THE BIGGEST FP NUMBER.
C GET INITIAL D VALUE AND DELV, DELVT TERMS.
      CALL SETR(NV, 0E0, D0)
      FAILED = .FALSE.
      NES = NES+1
      DO  1 I = 1, NV
         DELV(I) = SQRTR*ABS(V(I))
         IF (DELV(I) .EQ. 0.) DELV(I) = SQRTR
   1     CONTINUE
      IF (.NOT. GETDVT) GOTO 3
         DO  2 I = 1, NV
            DELVT(I) = SQRTR*ABS(VT(I))
            IF (DELVT(I) .EQ. 0.) DELVT(I) = SQRTR
   2        CONTINUE
   3  CALL D(T, V, VT, NV, D0, DELV, DELVT)
      DO  4 J = 1, NV
C/6S
C        IF (DELV(J) .EQ. 0.) CALL SETERR(
C    1      50H IODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT, 50, 1
C    2      , 2)
C/7S
         IF (DELV(J) .EQ. 0.) CALL SETERR(
     1      ' IODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT', 50, 1
     2      , 2)
C/
   4     CONTINUE
      IF (.NOT. GETDVT) GOTO 6
         DO  5 J = 1, NV
C/6S
C           IF (DELVT(J) .EQ. 0.) CALL SETERR(
C    1         50H IODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT,
C    2         50, 1, 2)
C/7S
            IF (DELVT(J) .EQ. 0.) CALL SETERR(
     1         ' IODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT',
     2         50, 1, 2)
C/
   5        CONTINUE
         GOTO  8
   6     TEMP1 = MIN0(LDUMMY, LDVT)
         DO  7 J = 1, TEMP1
C/6S
C           IF (DUMMY(J) .NE. 0.) CALL SETERR(
C    1         29H IODEG - D USED DV AND/OR DVT, 29, 2, 2)
C/7S
            IF (DUMMY(J) .NE. 0.) CALL SETERR(
     1         ' IODEG - D USED DV AND/OR DVT', 29, 2, 2)
C/
   7        CONTINUE
   8  IF (.NOT. FAILED) GOTO 9
         FNUM = 1
         IODEG = .FALSE.
         RETURN
   9  IF (.NOT. GETDVT) GOTO 22
         DO  15 I = 1, NV
C USE FDS TO GET DV AND DVT.
            DELVI = ABS(DELV(I))
            IF (V(I) .GT. 0.) DELVI = -DELVI
C GUARD AGAINST OVERFLOW.
            VSAVE = V(I)
            V(I) = V(I)+DELVI
            CALL SETR(NV, 0E0, D1)
            FAILED = .FALSE.
            NES = NES+1
            CALL D(T, V, VT, NV, D1, DUMMY, DUMMY)
            TEMP1 = MIN0(LDUMMY, MAX0(LDV, LDVT))
            DO  10 J = 1, TEMP1
C/6S
C              IF (DUMMY(J) .NE. 0.) CALL SETERR(
C    1            29H IODEG - D USED DV AND/OR DVT, 29, 2, 2)
C/7S
               IF (DUMMY(J) .NE. 0.) CALL SETERR(
     1            ' IODEG - D USED DV AND/OR DVT', 29, 2, 2)
C/
  10           CONTINUE
            IF (.NOT. FAILED) GOTO 11
               FNUM = 1
               IODEG = .FALSE.
               RETURN
  11        DO  14 J = 1, NV
               TEMP = ABS(DELVI) .LT. 1.
               IF (TEMP) TEMP = ABS(D1(J)-D0(J)) .GT. ABS(DELVI)*BIG
               IF (.NOT. TEMP) GOTO 12
                  DV(J, I) = BIG
C OVERFLOW.
                  GOTO  13
  12              DV(J, I) = (D1(J)-D0(J))/DELVI
  13           CONTINUE
  14           CONTINUE
            V(I) = VSAVE
  15        CONTINUE
         DO  21 I = 1, NV
            DELVTI = ABS(DELVT(I))
            IF (VT(I) .GT. 0.) DELVTI = -DELVTI
C GUARD AGAINST OVERFLOW.
            VTSAVE = VT(I)
            VT(I) = VT(I)+DELVTI
            CALL SETR(NV, 0E0, D1)
            FAILED = .FALSE.
            NES = NES+1
            CALL D(T, V, VT, NV, D1, DUMMY, DUMMY)
            TEMP1 = MIN0(LDUMMY, MAX0(LDV, LDVT))
            DO  16 J = 1, TEMP1
C/6S
C              IF (DUMMY(J) .NE. 0.) CALL SETERR(
C    1            29H IODEG - D USED DV AND/OR DVT, 29, 2, 2)
C/7S
               IF (DUMMY(J) .NE. 0.) CALL SETERR(
     1            ' IODEG - D USED DV AND/OR DVT', 29, 2, 2)
C/
  16           CONTINUE
            IF (.NOT. FAILED) GOTO 17
               FNUM = 1
               IODEG = .FALSE.
               RETURN
  17        DO  20 J = 1, NV
               TEMP = ABS(DELVTI) .LT. 1.
               IF (TEMP) TEMP = ABS(D1(J)-D0(J)) .GT. ABS(DELVTI)*BIG
               IF (.NOT. TEMP) GOTO 18
                  DVT(J, I) = BIG
C OVERFLOW.
                  GOTO  19
  18              DVT(J, I) = (D1(J)-D0(J))/DELVTI
  19           CONTINUE
  20           CONTINUE
            VT(I) = VTSAVE
  21        CONTINUE
         GOTO  29
  22     DO  28 I = 1, NV
C NOT GETTING DVT.
            VSAVE = V(I)
            VTSAVE = VT(I)
            DELVI = ABS(DELV(I))
            V(I) = V(I)+DELVI*AJ
            VT(I) = VT(I)+DELVI/BJ
            CALL SETR(NV, 0E0, D1)
            FAILED = .FALSE.
            NES = NES+1
            CALL D(T, V, VT, NV, D1, DUMMY, DUMMY)
            TEMP1 = MIN0(LDUMMY, MAX0(LDV, LDVT))
            DO  23 J = 1, TEMP1
C/6S
C              IF (DUMMY(J) .NE. 0.) CALL SETERR(
C    1            29H IODEG - D USED DV AND/OR DVT, 29, 2, 2)
C/7S
               IF (DUMMY(J) .NE. 0.) CALL SETERR(
     1            ' IODEG - D USED DV AND/OR DVT', 29, 2, 2)
C/
  23           CONTINUE
            IF (.NOT. FAILED) GOTO 24
               FNUM = 1
               IODEG = .FALSE.
               RETURN
  24        DO  27 J = 1, NV
               TEMP = ABS(DELVI) .LT. 1.
               IF (TEMP) TEMP = ABS(D1(J)-D0(J)) .GT. BIG*ABS(DELVI)
               IF (.NOT. TEMP) GOTO 25
                  DV(J, I) = BIG
C OVERFLOW.
                  GOTO  26
  25              DV(J, I) = (D1(J)-D0(J))/DELVI
  26           CONTINUE
  27           CONTINUE
            V(I) = VSAVE
            VT(I) = VTSAVE
  28        CONTINUE
  29  IODEG = .TRUE.
      RETURN
      END
