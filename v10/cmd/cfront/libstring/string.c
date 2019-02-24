#include "String.h"
#include <ctype.h>

extern "C" {
	extern char	*memcpy(char *to, const char *from, int);
}

inline char*	// thanx to Jerry Schwarz
my_mcpy(char *to, const char *from, int n)
{
	if (n==1) {
		*to = *from;
		return to;
	} else return memcpy(to, from, n);
}

inline char*
my_Mcpy(char *to, const char *from, int n)
{
	if (n==1) {
		*to = *from;
		return to;
	} else return Memcpy(to, from, n);
}

String
String::operator+ (const String& oo) _const	/* catenate */
{
	register Rep	*sd = d;
	register Rep	*sood = oo.d;
	register	sdlen = sd->len;
	register	soodlen = sood->len;
	if (sdlen + soodlen > MAXSTRINGLENGTH)
		error( 1, "String.operator+: overflow" );
	if (sdlen == 0) return oo;
	else if (soodlen == 0) return *this;
	// try to make this a SubString of the result
	Rep	*temp;
	if ((temp = sd->canCat(soodlen)) == NULL) {
		temp = new Rep(sdlen + soodlen);
		my_mcpy(temp->start, sd->start, sdlen);
	}
	my_mcpy(temp->start+sdlen, sood->start, soodlen);
	return String(*temp);
}

String
String::operator+ (const char *p) _const	/* catenate */
{
	register Rep	*sd = d;
	register	sdlen = sd->len;
	register	len = strlen(p);
	if (sdlen + len > MAXSTRINGLENGTH)
		error(1, "String::operator+: overflow");
	if (sdlen == 0) return String(p);
	else if (len == 0) return *this;
	// try to make this a SubString of the result
	Rep	*temp;
	if ((temp = sd->canCat(len)) == NULL) {
		temp = new Rep(sdlen + len);
		my_mcpy(temp->start, sd->start, sdlen);
	}
	my_mcpy(temp->start+sdlen, p, len);
	return String(*temp);
}

String
String::operator+ (const char c) const	/* catenate */
{
	register Rep	*sd = d;
	register	sdlen = sd->len;
	if (sdlen > MAXSTRINGLENGTH - 1)
		error( 1, "String.operator+: overflow" );
	if (sdlen == 0) return String(c);
	// try to make this a SubString of the result
	Rep	*temp;
	if ((temp = sd->canCat(1)) == NULL) {
		temp = new Rep(sdlen + 1);
		my_mcpy(temp->start, sd->start, sdlen);
	}
	temp->start[sdlen] = c;
	return String(*temp);
}

String
operator+ (const char *p, const String& s)	/* catenate */
{
	register Rep	*ssd = s.d;
	register	ssdlen = ssd->len;
	register	len = strlen(p);
	if (ssdlen + len > MAXSTRINGLENGTH)
		s.error( 1, "operator+(char*,String): overflow" );
	if (ssdlen == 0) return String(p);
	else if (len == 0) return s;
	Rep	*temp = new Rep(ssdlen + len);
	my_mcpy(temp->start, p, len);
	my_mcpy(temp->start+len, ssd->start, ssdlen);
	return String(*temp);
}

String
operator+ (const char c, const String& s)	/* catenate */
{
	register Rep	*ssd = s.d;
	register	ssdlen = ssd->len;
	if (ssdlen > MAXSTRINGLENGTH - 1)
		s.error( 1, "operator+(char,String): overflow" );
	if (ssdlen == 0) return String(c);
	Rep	*temp = new Rep(ssdlen + 1);
	my_mcpy(temp->start+1, ssd->start, ssdlen);
	temp->start[0] = c;
	return String(*temp);
}

Subchar&
String::operator[](const unsigned i)	/* character selection */
{
	if ( i >= length())
		error(1, "String.operator[]: out of bounds");
	return *new Subchar(*this,i);
}

SubString&
String::operator()(const unsigned offset, const unsigned len)	/* SubString */
{
	if ( offset > length() )
		error(1, "String.operator(): offset out of bounds");
	if ( len > MAXSUBSTRINGLENGTH || offset + len > d->len)
		error(1, "String.operator(): length out of bounds");
	return *new SubString(*this, offset, len);
}

Subchar::Subchar(const Subchar&)
{
	error(1, "Subchar.Subchar(Subchar&): shouldn't call this" );
}

SubString::SubString(const SubString&)
{
	error(1, "SubString.SubString(SubString&): shouldn't call this" );
}

String&
String::operator=(const String& oo)
{
	register Rep	*sd = d;
	register Rep	*sood = oo.d;
	if ( sd != sood ) {
		sd->refDecr();
		sd = d = sood;
		sd->refIncr();
	}
	return *this;
}

String&
String::operator=(const char *s)
{
	register Rep	*sd = d;
	sd->refDecr();
	d = new Rep(s);
	return *this;
}

String&
String::operator= (const char c)
{
	register Rep	*sd = d;
	sd->refDecr();
	d = &oneChar[c];
	return *this;
}

char
Subchar::operator=(const char cc)
{
	register Rep	*sd = ss->d;
	if (sd->len == 1) {	/* one character String */
		sd = &oneChar[cc];
	} else if (sd->refCount == 1 && !sd->is_constant()) {
		/* modify in place */
		sd->start[oo] = cc;
	} else {
		Rep	*rr = new Rep(*sd);
		/* copy middle */
		rr->start[oo] = cc;
		sd->refDecr();
		ss->d = rr;
	}
	/* delete this;	it's used up */
	return cc;
}

void
SubString::operator=(const String& oss)
{
	register Rep	*sd = ss->d;
	register Rep	*osd = oss.d;
	int	itsOldLength = ss->length();
	int	newLength = itsOldLength + oss.length() - ll;
	if (newLength == 0) {
		sd->refDecr();
		sd = ss->d = nullRep;
	} else if (newLength == 1) {
		Rep	*rr;
		if (oss.length() == 1)
			rr = osd;
		else if (oo == 1)	/* the head of this */
			rr = &oneChar[*sd->start];
		else	/* the last character */
			rr = &oneChar[*(sd->start+ss->length()-1)];
		sd->refDecr();
		ss->d = rr;
	} else if (sd->refCount == 1 && !sd->is_constant() &&
	    (sd->extend(oss.length() - ll))) {
		/* modify in place */
		if (ll != oss.length())
			/* copy tail */
			my_Mcpy(sd->start + oo + oss.length(),
			    sd->start + oo + ll,
			    itsOldLength - (oo + ll));
		/* copy middle */
		my_mcpy(sd->start+oo, osd->start, oss.length());
	} else {
		Rep	*rr = new Rep(newLength);
		/* copy head */
		my_mcpy(rr->start, sd->start, oo);
		/* copy middle */
		my_mcpy(rr->start+oo, osd->start, oss.length());
		/* copy tail */
		my_mcpy(rr->start + oo + oss.length(),
		    sd->start + oo + ll,
		    ss->length() - (oo + ll));
		sd->refDecr();
		ss->d = rr;
	}
	/* delete this;	it's used up */
}

bit
String::getX(char& c)	/*  or lop */
{
	register Rep	*sd = d;
	if ( length() < 1 )
		return FALSE;
	c = *sd->start;
	if ( length() == 1 )
		d = nullRep;
	else  if ( length() == 2 ) {
		d = &oneChar[(unsigned char)sd->start[1]];
		sd->refDecr();
	} else {
		Charfield	*myF = sd->myField();
		if (sd->refCount == 1 && !sd->is_constant()) {
			sd->start++;
			sd->len--;
			myF->usedSpace -= 1;
		} else {
			Rep	*rr = myF->getMt();
			if (rr) {
				sd->immutable();
				rr->immutable();
				rr->start = sd->start + 1;
				myF->usedSpace += (rr->len = sd->len - 1);
			} else
				rr = new Rep (sd->start+1, sd->len - 1);
			sd->refDecr();
			d = rr;
		}
	}
	return TRUE;
}

bit
String::firstX(char &c) _const	// don't change the String
{
	return  length() ? (c = *d->start, TRUE) : FALSE;
}

bit
String::lastX(char &c) _const	// don't change the String
{
	return  length() ? (c = d->start[length()-1], TRUE) : FALSE;
}

bit
String::unputX(char& c)		/* remove from back */
{
	register Rep	*sd = d;
	if (length() < 1)
		return FALSE;
	c = sd->start[length()-1];
	if (length() == 1)
		d = nullRep;
	else  if (length() == 2) {
		d = &oneChar[(unsigned char)*sd->start];
		sd->refDecr();
	} else {
		Charfield	*myF = sd->myField();
		if (!myF) abort();	// internal string error
		if (sd->refCount == 1 && !sd->is_constant()) {
			sd->len--;
			myF->usedSpace -= 1;
		} else {
			Rep	*rr = myF->getMt();
			if (rr) {
				sd->immutable();
				rr->immutable();
				rr->start = sd->start;
				myF->usedSpace += (rr->len = sd->len - 1);
			} else
				rr = new Rep (sd->start, sd->len - 1);
			sd->refDecr();
			d = rr;
		}
	}
	return TRUE;
}

String&
String::put(const char c)	/* append or put */
{
	register Rep	*sd = d;
	register	sdlen = sd->len;
	switch (sdlen) {
	case 0:
		d = &oneChar[(unsigned char) c];
		break;
	case 1:
		Rep	*rr = new Rep(2);
		*rr->start = *sd->start;
		rr->start[1] = c;
		d = rr;
		break;
	case MAXSTRINGLENGTH:
		error (1, "String::put: String overflow");
		break;
	default:
		if (sd->refCount == 1 && !sd->is_constant() && sd->extend(1))
			sd->start[sdlen] = c;
		else {
			Rep	*rr = sd->canCat(1);
			if (rr)
				rr->start[rr->len-1] = c;
			else {
				rr = new Rep (sdlen + 1);
				my_mcpy(rr->start, sd->start, sdlen);
				rr->start[sdlen] = c;
			}
			sd->refDecr();
			d = rr;
		}
		break;
	}
	return *this;
}

String&
String::put(const char *p)	/* append */
{
	register Rep	*sd = d;
	register	sdlen = sd->len;
	register	soodlen = strlen(p);
	if (sdlen + soodlen > MAXSTRINGLENGTH)
		error(1, "String::put: overflow");
	if (sdlen == 0) return *this = p;
	if (soodlen == 0) return *this;
	if (sd->refCount == 1 && !sd->is_constant()) {
		char	*oldEnd = sd->start+sdlen;
		if (sd->extend(soodlen)) {
			my_mcpy(oldEnd, p, soodlen);
			return *this;
		}
	}
	Rep	*temp = new Rep(sdlen + soodlen);
	my_mcpy(temp->start, sd->start, sdlen);
	my_mcpy(temp->start+sdlen, p, soodlen);
	sd->refDecr();
	d = temp;
	return *this;
}

String&
String::put(const String& oo)	/* append */
{
	register Rep	*sd = d;
	register Rep	*sood = oo.d;
	register	sdlen = sd->len;
	register	soodlen = sood->len;
	if (sdlen + soodlen > MAXSTRINGLENGTH)
		error(1, "String::put: overflow");
	if (sdlen == 0) return *this = oo;
	if (soodlen == 0) return *this;
	if (sd->refCount == 1 && !sd->is_constant()) {
		char	*oldEnd = sd->start+sdlen;
		if (sd->extend(soodlen)) {
			my_mcpy(oldEnd, sood->start, soodlen);
			return *this;
		}
	}
	Rep	*temp = new Rep(sdlen + soodlen);
	my_mcpy(temp->start, sd->start, sdlen);
	my_mcpy(temp->start+sdlen, sood->start, soodlen);
	sd->refDecr();
	d = temp;
	return *this;
}

// unget(c) sticks c onto the front of the String
String&
String::unget (const char c)
{
	register Rep	*sd = d;
	register	sdlen = sd->len;
	switch (sdlen) {
	case 0:
		d = &oneChar[(unsigned char) c];
		break;
	case 1:
		Rep	*rr = new Rep (2);
		*rr->start = c;
		rr->start[1] = *sd->start;
		d = rr;
		break;
	case MAXSTRINGLENGTH:
		error(1, "String::unget: String overflow");
		break;
	default:
		if (sd->refCount == 1 && !sd->is_constant() && sd->extend(1)) {
			my_Mcpy(sd->start+1, sd->start, sdlen);
			*sd->start = c;
		} else {
			Rep	*rr;
			rr = new Rep (sdlen + 1);
			my_mcpy (rr->start+1, sd->start, sdlen);
			*rr->start = c;
			sd->refDecr();
			d = rr;
		}
	}
	return *this;
}

String&
String::unget (const String& oo)	/* prepend */
{
	register Rep	*sd = d;
	register Rep	*sood = oo.d;
	register	sdlen = sd->len;
	register	soodlen = sood->len;
	if (sdlen + soodlen > MAXSTRINGLENGTH)
		error(1, "String.operator-=: overflow");
	if (soodlen == 0) return *this;
	if (sdlen == 0) return *this = oo;
	if (sd->refCount == 1 && !sd->is_constant()) {
		if (sd->extend(soodlen)) {
			my_Mcpy(sd->start+soodlen, sd->start, sdlen);
			my_mcpy(sd->start, sood->start, soodlen);
			return *this;
		}
	}
	Rep	*temp = new Rep(sdlen + soodlen);
	my_mcpy(temp->start, sood->start, soodlen);
	my_mcpy(temp->start+soodlen, sd->start, sdlen);
	sd->refDecr();
	d = temp;
	return *this;
}

istream&
operator>>(istream& ii, String& ss)
{
	ss = "";
	if (!(ii >> WS))
		return ii;
	char	c;
	while (ii.get(c)) {
		if (isspace(c)) {
			ii.putback(c);
			break;
		}
		ss.put(c);
	}
	return ii;
}

String
sgets(istream& ii)
{
	char	c;
	String	ans;
	while (ii.get(c) && c != '\n')
		ans.put(c);
	return ans;
}

void
String::dump(char *s) _const
{
	my_mcpy(s, d->start, d->len);
	s[d->len] = '\0';
}

