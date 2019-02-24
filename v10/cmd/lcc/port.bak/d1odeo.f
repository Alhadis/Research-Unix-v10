      LOGICAL FUNCTION D1ODEO(V, NV, T, DT, D, DA, ERROR, INMI,
     1   ERRPAR, VTHETA, VT, VOLD, EV, EV1, EV2, DV, THETA, MINIT,
     2   MAXIT, KEEJAC, IRCS, EGIVE)
      INTEGER NV
      EXTERNAL D, DA, ERROR, INMI
      INTEGER MINIT, MAXIT, KEEJAC, IRCS
      REAL ERRPAR(2), EV(NV), EV1(NV), EV2(NV), EGIVE
      LOGICAL DA, ERROR
      DOUBLE PRECISION V(NV), T, DT, VTHETA(NV), VT(NV), VOLD(NV)
      DOUBLE PRECISION DV(NV), THETA
      COMMON /D10DER/ NJS, NFS, NTSS, NSSS, NES, NNITS, NNDS, NNFS, NRS
      INTEGER NJS, NFS, NTSS, NSSS, NES, NNITS
      INTEGER NNDS, NNFS, NRS
      COMMON /D1ODEG/ TJ, DTJ, GETJAC, SEPATE, USENGJ, USENNS, USENFD
      LOGICAL GETJAC, SEPATE, USENGJ, USENNS, USENFD
      DOUBLE PRECISION TJ, DTJ
      COMMON /D1ODEL/ ERPUTS
      LOGICAL ERPUTS
      COMMON /D1ODEF/ FNUM
      INTEGER FNUM
      INTEGER I, J, ITER
      REAL RHO, PROD, TEMP, POWER
      LOGICAL DONE
      DOUBLE PRECISION DABS
      INTEGER TEMP1
      LOGICAL TEMP2
      CALL MOVEFD(NV, V, VOLD)
      CALL SETD(NV, 0D0, VT)
C GET INITIAL NEWTON METHOD GUESS.
      CALL INMI(NV, T, DT, VOLD, V, VT)
      DO  23 ITER = 1, MAXIT
         IF (KEEJAC .NE. 0) GOTO 1
            GETJAC = .TRUE.
            TJ = T
   1     IF (GETJAC) NJS = NJS+1
         NNITS = NNITS+1
         DO  2 I = 1, NV
            VTHETA(I) = THETA*(V(I)-VOLD(I))+VOLD(I)
   2        CONTINUE
         IF (GETJAC) DTJ = 0
         DONE = DA(VTHETA, VT, NV, T, DT, D, DV, TJ, DTJ, GETJAC,
     1      SEPATE, USENGJ, USENNS, USENFD, NES, NFS, FNUM, THETA, IRCS,
     2      KEEJAC)
         IF (.NOT. DONE) GOTO 3
            DONE = .FALSE.
            GOTO  24
   3     GETJAC = .FALSE.
         DTJ = DT
         DO  4 I = 1, NV
            V(I) = V(I)+DV(I)
            EV(I) = EGIVE*DABS(DV(I))
   4        CONTINUE
         IF (MAXIT .NE. 1) GOTO 5
            DONE = .TRUE.
            GOTO  24
   5     CALL MOVEFR(NV, EV, EV2)
         DONE = ERROR(V, NV, T, DT, ERRPAR, ERPUTS, EV)
C CHECK FOR NEGATIVE ERROR REQUESTS.
         DO  7 I = 1, NV
            TEMP2 = EV(I) .EQ. 0.
            IF (TEMP2) TEMP2 = EV2(I) .NE. 0.
            IF (.NOT. TEMP2) TEMP2 = EV(I) .LT. 0.
            IF (.NOT. TEMP2) GOTO 6
C/6S
C              CALL SETERR(37HDESSOM - E(I).LE.0 RETURNED BY SERROR, 37,
C    1            19, 1)
C/7S
               CALL SETERR('DESSOM - E(I).LE.0 RETURNED BY SERROR', 37,
     1            19, 1)
C/
               D1ODEO = .FALSE.
               RETURN
   6        CONTINUE
   7        CONTINUE
         IF (.NOT. DONE) GOTO 8
            GOTO  24
   8        IF (ITER .NE. MAXIT) GOTO 9
               FNUM = 4
               NNFS = NNFS+1
               GOTO  24
   9     CONTINUE
  10     DO  21 I = 1, NV
            IF (ITER .LE. MINIT .OR. EV(I) .GE. EV2(I)) GOTO 20
               IF (EV1(I) .LE. EV2(I)) GOTO 11
                  RHO = EV2(I)/EV1(I)
C CAN CHECK CONVERGENCE RATE.
                  GOTO  12
  11              RHO = 1
  12           IF (RHO .LT. 1.) GOTO 13
                  NNDS = NNDS+1
C DIVERGING.
                  FNUM = 3
                  D1ODEO = DONE
                  RETURN
  13           IF (KEEJAC .NE. 0) GOTO 17
                  PROD = 1
C CHECK QUADRATIC CONVERGENCE RATE.
                  POWER = RHO**2
C < 1.
                  TEMP = EV(I)/EV2(I)
                  TEMP1 = MAXIT-ITER
                  DO  14 J = 1, TEMP1
                     PROD = PROD*POWER
                     POWER = POWER**2
                     IF (PROD .LE. TEMP) GOTO  15
  14                 CONTINUE
  15              IF (PROD .LE. TEMP) GOTO 16
                     NNDS = NNDS+1
C SLOW CONVERGENCE.
                     FNUM = 3
                     D1ODEO = DONE
                     RETURN
  16              CONTINUE
                  GOTO  19
  17              IF (RHO**(MAXIT-ITER)*EV2(I) .LE. EV(I)) GOTO 18
                     NNDS = NNDS+1
C CHECK LINEAR CONVERGENCE RATE.
C SLOW CONVERGENCE.
                     FNUM = 3
                     D1ODEO = DONE
                     RETURN
  18              CONTINUE
  19           CONTINUE
  20        EV1(I) = EV2(I)
  21        CONTINUE
         DO  22 I = 1, NV
            VT(I) = VT(I)+DV(I)/DT
  22        CONTINUE
  23     CONTINUE
  24  D1ODEO = DONE
      RETURN
      END
