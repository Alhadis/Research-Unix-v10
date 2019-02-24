      INTEGER FUNCTION MAPCHR(CHAR, ERR)
C
C     MAPCHR RETURNS THE INTERNAL CODE OF THE CHARACTER CHAR
C
      INTEGER CHAR
      INTEGER LET(46), ICODE(46)
      LOGICAL ERR
C
      DATA LET(7), ICODE(7) /1H0,0/
      DATA LET(16), ICODE(16) /1H1,1/
      DATA LET(22), ICODE(22) /1H2,2/
      DATA LET(24), ICODE(24) /1H3,3/
      DATA LET(26), ICODE(26) /1H4,4/
      DATA LET(28), ICODE(28) /1H5,5/
      DATA LET(25), ICODE(25) /1H6,6/
      DATA LET(38), ICODE(38) /1H7,7/
      DATA LET(34), ICODE(34) /1H8,8/
      DATA LET(32), ICODE(32) /1H9,9/
      DATA LET(3), ICODE(3) /1HA,30/
      DATA LET(36), ICODE(36) /1HB,31/
      DATA LET(14), ICODE(14) /1HC,32/
      DATA LET(6), ICODE(6) /1HD,33/
      DATA LET(5), ICODE(5) /1HE,34/
      DATA LET(33), ICODE(33) /1HF,35/
      DATA LET(27), ICODE(27) /1HG,36/
      DATA LET(37), ICODE(37) /1HH,37/
      DATA LET(8), ICODE(8) /1HI,38/
      DATA LET(44), ICODE(44) /1HJ,39/
      DATA LET(46), ICODE(46) /1HK,40/
      DATA LET(13), ICODE(13) /1HL,41/
      DATA LET(12), ICODE(12) /1HM,42/
      DATA LET(4), ICODE(4) /1HN,43/
      DATA LET(2), ICODE(2) /1HO,44/
      DATA LET(18), ICODE(18) /1HP,45/
      DATA LET(43), ICODE(43) /1HQ,46/
      DATA LET(11), ICODE(11) /1HR,47/
      DATA LET(15), ICODE(15) /1HS,48/
      DATA LET(10), ICODE(10) /1HT,49/
      DATA LET(30), ICODE(30) /1HU,50/
      DATA LET(42), ICODE(42) /1HV,51/
      DATA LET(45), ICODE(45) /1HW,52/
      DATA LET(39), ICODE(39) /1HX,53/
      DATA LET(35), ICODE(35) /1HY,54/
      DATA LET(41), ICODE(41) /1HZ,55/
      DATA LET(31), ICODE(31) /1H+,60/
      DATA LET(29), ICODE(29) /1H-,61/
      DATA LET(19), ICODE(19) /1H),62/
      DATA LET(23), ICODE(23) /1H=,63/
      DATA LET(17), ICODE(17) /1H.,64/
      DATA LET(20), ICODE(20) /1H(,65/
      DATA LET(21), ICODE(21) /1H*,66/
      DATA LET(40), ICODE(40) /1H/,67/
      DATA LET(9), ICODE(9) /1H,,68/
      DATA LET(1), ICODE(1) /1H ,69/
C
      DO 10 I=1,46
        IF (CHAR.EQ.LET(I)) GO TO 20
   10 CONTINUE
C
C     UNKNOWN, RETURN BLANK AND SET ERR = .TRUE.
C
      MAPCHR = 69
      ERR = .TRUE.
      RETURN
C
C     KNOWN, RETURN INTERNAL CODE, LEAVE ERR ALONE
C
   20 MAPCHR = ICODE(I)
      RETURN
      END