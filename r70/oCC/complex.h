
#include <stream.h>
#include <errno.h>

overload cos;
overload cosh;
overload exp;
overload log;
overload pow;
overload sin;
overload sinh;
overload sqrt;
overload abs;

#include <math.h>
inline double abs(double d) { return fabs(d); }

class complex {
	double	re, im;
public:
 	complex(double r, double i)	{ re=r; im=i; }
	complex(double r)		{ re=r; im=0; }
 	complex()			{ re=0; im=0; }
	 	
	friend	double	real(const complex& a ) { return a.re; }
	friend	double	imag(const complex& a ) { return a.im; }

	friend	double	abs(complex);
	friend  double  norm(complex);
	friend  double	arg(complex);
	friend  complex conj(complex a) { return complex(a.re, -a.im); }
	friend  complex cos(complex);
	friend  complex cosh(complex);
	friend	complex exp(complex);
	friend  complex log(complex);
	friend  complex pow(double, complex);
	friend	complex pow(complex, int);
	friend	complex pow(complex, double);
	friend	complex pow(complex, complex);
	friend  complex	polar(double, double = 0);
	friend  complex sin(complex);
	friend  complex sinh(complex);
	friend	complex sqrt(complex);

	friend complex  operator+(complex a1, complex a2)
	{
		return complex(a1.re+a2.re, a1.im+a2.im);
	}
	friend complex  operator-(complex a1, complex a2)
	{
		return complex(a1.re-a2.re, a1.im-a2.im);
	}
	friend complex  operator-(complex a) { return complex(-a.re, -a.im); }
	friend complex  operator*(complex, complex);
	friend complex  operator/(complex, complex);
	friend int	operator==(complex a, complex b)
	{
		return a.re==b.re && a.im==b.im;
	}
	friend int	operator!=(complex a, complex b)
	{
		return a.re!=b.re || a.im!=b.im;
	}
	friend int	operator==(complex, complex);
	friend int	operator!=(complex, complex);
	
	void operator+=(complex a) { re += a.re; im += a.im; }
	void operator-=(complex a) { re -= a.re; im -= a.im; }
	void operator*=(complex);
	void operator/=(complex);
};

ostream& operator<<(ostream&, complex);
istream& operator>>(istream&, complex&);

void complex_error(int,double);
extern int errno;

