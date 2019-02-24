      REAL FUNCTION FLR(X)
C
C  FLR RETURNS FLR(X)
C
      FLR = FLOAT( INT(X) )
      IF (X .GE. 0.0) RETURN
      IF (FLR .NE. X) FLR = FLR - 1.0
C
      RETURN
      END
