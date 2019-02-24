#include "decl.h"

ostream& mel::
print (ostream& o)
{
	o << p;
	if (s.length() != 0)
		o << " " << s;
	o << "\n";

	String_list_iterator si (q);
	String x;

	while (si.nextX (x))
		o << "\t" << x << "\n";

	return o;
}

ostream& map::
print (ostream& o)
{
	mel_list_iterator mi (l);
	mel m;

	while (mi.nextX (m))
		m.print (o);
	return o;
		
}

istream& map::
read (istream& i)
{
	l = mel_list();
	mel m;
	String s;

	while (s = sgets (i), i.good()) {
		if (s.length() != 0) {
			if (s[0] == '\t')
				m.q.put (s (1, s.length() - 1));
			else {
				if (m.p.length() != 0)
					l.put (m);
				int n = s.strchr (' ');
				if (n >= 0) {
					m.s = s (n + 1, s.length() - n - 1);
					s = s (0, n);
				} else
					m.s = "";
				m.p = s;
				m.q = String_list();
			}
		}
	}

	if (m.p.length() != 0)
		l.put (m);

	return i;
}

String
logname()
{
	char buf[64];
	for (int i = 0; i < sizeof (buf); i++)
		buf[i] = '\0';
	if (getlogname (buf) == 0)
		return buf;
	return "";
}
