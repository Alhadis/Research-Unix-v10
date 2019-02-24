      FUNCTION DOT(N,X,Y)
      INTEGER N
      REAL X(N),Y(N)
      DOT = 0
      DO 10 I = 1, N
  10    DOT = DOT + X(I)*Y(I)
      END
