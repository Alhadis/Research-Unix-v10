extern double fabs(), floor(), ceil(), fmod();
extern double frexp(), ldexp(), modf();
extern double sqrt(), hypot(), atof(), strtod();
extern double sin(), cos(), tan(), asin(), acos(), atan(), atan2();
extern double exp(), log(), log10(), pow();
extern double sinh(), cosh(), tanh();
extern double gamma();
extern double j0(), j1(), jn(), y0(), y1(), yn();
extern double frand();
extern double erf(), erfc();
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[120];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
struct _iobuf	*popen();
long	ftell();
char	*fgets();
typedef double real;
realfft ( log2n, x, y, sign )
int     sign ;
real   log2n, x[], y[] ;       
{
	static  int     prevn   =  0 ;
	static  double  *cost, *sint ;
	int     s[13], u[13] ;
	int     j1, j2, j3, j4, jt, n, m4 ;
	int     a, b, c, d, e, f, g, h, i, j, k, l, m ;
	double  arg, c1, c2, c3, s1, s2, s3, tpim4 ;
	double  r1, r2, r3, r4, r5, r6, r7, r8 ;
	real   *w, *z, t ;
	w = &x[0] ;
	z = &x[1] ;
	n = pow(2.,log2n -1.) + 0.2 ;  
	
	if ( n / 4  !=  prevn )     {
		if ( prevn != 0 )  {
			free ((char *)cost ) ;
			free ((char *)sint ) ;
		}
		prevn = n / 4 ;
		
		if ((( cost = (double *)calloc((unsigned)prevn,sizeof(*cost)))
				== (double *)0 ) ||
		    (( sint = (double *)calloc((unsigned)prevn,sizeof(*sint)))
				== (double *)0 )   ) {
			fprintf((&_iob[2]), " alloc error in realfft\n");
			exit(1);
		}
		tpim4 =   6.283185307179586476925287 / ( 2 * n ) ;
		for ( j = 1 ; j < prevn ; j++ )   {
			arg = tpim4 * j ;
			cost[j] = cos ( arg ) ;
			sint[j] = sin ( arg ) ;
		}
		cost[0] = 1. ;
		sint[0] = 0. ;
        }       
	if (log2n > 2.1 )  {
		for (m = n/4; m >= 1; m /= 4)    {
			m4 = 4 * m ;
			e = n / m4 ;
			for (j = 0; j < m; j++ ) {
				f = e * j ;
				c1 = cost[f] * cost[f] * 2. - 1. ;
				s1 = cost[f] * sint[f] * 2.      ;
				c2 = c1 * c1 * 2. - 1. ;     
				s2 = c1 * s1 * 2.      ;     
				c3 = c2 * c1 - s2 * s1 ;     
				s3 = c2 * s1 + s2 * c1 ;     
				for (i = m4; i <= n; i += m4)  {
					j1 = (i + j - m4) * 2 ;
					a  = m  * 2  ;
					j2 = j1 + a  ;
					j3 = j2 + a  ;
					j4 = j3 + a  ;
					r1 = w[j1] + w[j3] ;
					r2 = w[j1] - w[j3] ;
					r3 = z[j1] + z[j3] ;
					r4 = z[j1] - z[j3] ;
					r5 = w[j2] + w[j4] ;
					r6 = w[j2] - w[j4] ;
					r7 = z[j2] + z[j4] ;
					r8 = z[j2] - z[j4] ;
					w[j1] = r1 + r5 ;
					z[j1] = r3 + r7 ;
					if (arg != 0. )  {
					      w[j3] = (r2+r8)*c1 + (r4-r6)*s1 ;
					      z[j3] = (r4-r6)*c1 - (r2+r8)*s1 ;
					      w[j2] = (r1-r5)*c2 + (r3-r7)*s2 ;
					      z[j2] = (r3-r7)*c2 - (r1-r5)*s2 ;
					      w[j4] = (r2-r8)*c3 + (r4+r6)*s3 ;
					      z[j4] = (r4+r6)*c3 - (r2-r8)*s3 ;
					}
					else  {
						w[j3] = r2+r8 ;
						z[j3] = r4-r6 ;
						w[j2] = r1-r5 ;
						z[j2] = r3-r7 ;
						w[j4] = r2-r8 ;
						z[j4] = r4+r6 ;
					}
				}
			}
		}
	}
	
	if (((int)(log2n+.2)-1) % 2)  {
		m = n * 2 ;
		for (j=2, i = 0; i < m ; i += 4, j += 4 )   {
			r1 = w[i]+w[j] ;
			r2 = w[i]-w[j] ;
			r3 = z[i]+z[j] ;
			r4 = z[i]-z[j] ;
			w[i] = r1 ;
			z[i] = r3 ;
			w[j] = r2 ;
			z[j] = r4 ;
		}
	}
	
	s[12] = n     ;
	u[12] = n * 2 ;
	for (j = 11; j > 0; --j)   {
		s[j] = (s[j+1] > 2) ? s[j+1] / 2 : 2 ;
		u[j] = s[j+1]  ;
	}
	u[0] = s[1] ;
	jt = -2 ;
	for (a = 0; a < u[0];  a +=  2  )
	 for (b = a; b < u[1];  b += s[1])
	  for (c = b; c < u[2];  c += s[2])
	   for (d = c; d < u[3];  d += s[3])
	    for (e = d; e < u[4];  e += s[4])
	     for (f = e; f < u[5];  f += s[5])
	      for (g = f; g < u[6];  g += s[6])
	       for (h = g; h < u[7];  h += s[7])
		for (i = h; i < u[8];  i += s[8])
		 for (j = i; j < u[9];  j += s[9])
		  for (k = j; k < u[10]; k += s[10])
		   for (l = k; l < u[11]; l += s[11])
		    for (m = l; m < u[12]; m += s[12])  {
				jt += 2 ;
				if ( jt > m)  {
					t     = w[jt] ;
					w[jt] = w[m]  ;
					w[m]  = t     ;
					t     = z[jt] ;
					z[jt] = z[m]  ;
					z[m]  = t     ;
				}
			}
	
	n *= 2    ;
	tpim4 = w[0] - z[0] ;
	x[0]  = w[0] + z[0] ;
	y[0] = 0. ;
	m = n / 8 ;
	for (l=n/2-1, i=n-2, j=2, g=n/4-1, f=n/2-2, e=n/2+2, d=n/4+1, k=1;
		k < m  ;
		--l, i -= 2, j += 2, --g, f -= 2, e += 2, d++, k++ ) {
		
		r1 = z[j] + z[i] ;
		r2 = w[i] - w[j] ;
		r3 = cost[ k ] ;
		r4 = sint[ k ] ;
		r5 = r3 * r2 - r4 * r1 ;
		y[k] = .5 * ((z[j] - z[i]) + r5) ;
		y[l] = .5 * ((z[i] - z[j]) + r5) ;
		r5 = r3 * r1 + r4 * r2 ;
		r6 = w[j] + w[i] ;
		x[k] = .5 * (r6 + r5) ;
		x[i] = .5 * (r6 - r5) ; 
		
		r1 = z[f] + z[e] ;
		r2 = w[e] - w[f] ;
		r5 = r4 * r2 - r3 * r1 ;
		y[g] = .5 * ((z[f] - z[e]) + r5) ;
		y[d] = .5 * ((z[e] - z[f]) + r5) ;
		r5 = r4 * r1 + r3 * r2 ;
		r6 = w[f] + w[e] ;
		x[f] = .5 * (r6 + r5) ; 
		x[e] = .5 * (r6 - r5) ; 
	}
	
	r1 = ( z[j] + z[i] ) *   0.70710678118654752440 ;
	r2 = ( w[i] - w[j] ) *   0.70710678118654752440 ;
	r5 = r2 - r1 ;
	y[k] = .5 * ((z[j] - z[i]) + r5) ;
	y[l] = .5 * ((z[i] - z[j]) + r5) ;
	r5 = r1 + r2 ;
	r6 = w[j] + w[i] ;
	x[k] = .5 * (r6 + r5) ;
	x[i] = .5 * (r6 - r5) ; 
	
	m = n / 2 ;
	for (i=n/4+2, l=n/8+1; l< m ; i += 2, l++)
		x[l] = x[i] ;
	
	y[n/4] = -z[n/2] ;
	x[n/2] = tpim4   ;
	
	if ( sign <= 0 ) {      
		r1 = 1. / (double) n ;
		r2 = - r1 ;
		m = n / 2 ;
		for (i=0; i<m ; i++)  {
			x[i] *= r1 ;
			y[i] *= r2 ;
		}
		x[n/2] *= r1 ;
        }
        return ;
}
