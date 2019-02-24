case 'a':
	if (rcp[0] == 'u'
	&&  rcp[1] == 't'
	&&  rcp[2] == 'o'
	&& !(map[rcp[3]]&(DIGIT|LETTER))) {
		cp = rcp + 3;
		return AUTO;
	}
	goto id;
case 'b':
	if (rcp[0] == 'r'
	&&  rcp[1] == 'e'
	&&  rcp[2] == 'a'
	&&  rcp[3] == 'k'
	&& !(map[rcp[4]]&(DIGIT|LETTER))) {
		cp = rcp + 4;
		return BREAK;
	}
	goto id;
case 'c':
	if (rcp[0] == 'a'
	&&  rcp[1] == 's'
	&&  rcp[2] == 'e'
	&& !(map[rcp[3]]&(DIGIT|LETTER))) {
		cp = rcp + 3;
		return CASE;
	}
	if (rcp[0] == 'h'
	&&  rcp[1] == 'a'
	&&  rcp[2] == 'r'
	&& !(map[rcp[3]]&(DIGIT|LETTER))) {
		cp = rcp + 3;
		tsym = chartype->u.sym;
		return CHAR;
	}
	if (rcp[0] == 'o'
	&&  rcp[1] == 'n'
	&&  rcp[2] == 's'
	&&  rcp[3] == 't'
	&& !(map[rcp[4]]&(DIGIT|LETTER))) {
		cp = rcp + 4;
		return CONST;
	}
	if (rcp[0] == 'o'
	&&  rcp[1] == 'n'
	&&  rcp[2] == 't'
	&&  rcp[3] == 'i'
	&&  rcp[4] == 'n'
	&&  rcp[5] == 'u'
	&&  rcp[6] == 'e'
	&& !(map[rcp[7]]&(DIGIT|LETTER))) {
		cp = rcp + 7;
		return CONTINUE;
	}
	goto id;
case 'd':
	if (rcp[0] == 'e'
	&&  rcp[1] == 'f'
	&&  rcp[2] == 'a'
	&&  rcp[3] == 'u'
	&&  rcp[4] == 'l'
	&&  rcp[5] == 't'
	&& !(map[rcp[6]]&(DIGIT|LETTER))) {
		cp = rcp + 6;
		return DEFAULT;
	}
	if (rcp[0] == 'o'
	&&  rcp[1] == 'u'
	&&  rcp[2] == 'b'
	&&  rcp[3] == 'l'
	&&  rcp[4] == 'e'
	&& !(map[rcp[5]]&(DIGIT|LETTER))) {
		cp = rcp + 5;
		tsym = doubletype->u.sym;
		return DOUBLE;
	}
	if (rcp[0] == 'o'
	&& !(map[rcp[1]]&(DIGIT|LETTER))) {
		cp = rcp + 1;
		return DO;
	}
	goto id;
case 'e':
	if (rcp[0] == 'l'
	&&  rcp[1] == 's'
	&&  rcp[2] == 'e'
	&& !(map[rcp[3]]&(DIGIT|LETTER))) {
		cp = rcp + 3;
		return ELSE;
	}
	if (rcp[0] == 'n'
	&&  rcp[1] == 'u'
	&&  rcp[2] == 'm'
	&& !(map[rcp[3]]&(DIGIT|LETTER))) {
		cp = rcp + 3;
		return ENUM;
	}
	if (rcp[0] == 'x'
	&&  rcp[1] == 't'
	&&  rcp[2] == 'e'
	&&  rcp[3] == 'r'
	&&  rcp[4] == 'n'
	&& !(map[rcp[5]]&(DIGIT|LETTER))) {
		cp = rcp + 5;
		return EXTERN;
	}
	goto id;
case 'f':
	if (rcp[0] == 'l'
	&&  rcp[1] == 'o'
	&&  rcp[2] == 'a'
	&&  rcp[3] == 't'
	&& !(map[rcp[4]]&(DIGIT|LETTER))) {
		cp = rcp + 4;
		tsym = floattype->u.sym;
		return FLOAT;
	}
	if (rcp[0] == 'o'
	&&  rcp[1] == 'r'
	&& !(map[rcp[2]]&(DIGIT|LETTER))) {
		cp = rcp + 2;
		return FOR;
	}
	goto id;
case 'g':
	if (rcp[0] == 'o'
	&&  rcp[1] == 't'
	&&  rcp[2] == 'o'
	&& !(map[rcp[3]]&(DIGIT|LETTER))) {
		cp = rcp + 3;
		return GOTO;
	}
	goto id;
case 'i':
	if (rcp[0] == 'f'
	&& !(map[rcp[1]]&(DIGIT|LETTER))) {
		cp = rcp + 1;
		return IF;
	}
	if (rcp[0] == 'n'
	&&  rcp[1] == 't'
	&& !(map[rcp[2]]&(DIGIT|LETTER))) {
		cp = rcp + 2;
		tsym = inttype->u.sym;
		return INT;
	}
	goto id;
case 'l':
	if (rcp[0] == 'o'
	&&  rcp[1] == 'n'
	&&  rcp[2] == 'g'
	&& !(map[rcp[3]]&(DIGIT|LETTER))) {
		cp = rcp + 3;
		tsym = longtype->u.sym;
		return LONG;
	}
	goto id;
case 'r':
	if (rcp[0] == 'e'
	&&  rcp[1] == 'g'
	&&  rcp[2] == 'i'
	&&  rcp[3] == 's'
	&&  rcp[4] == 't'
	&&  rcp[5] == 'e'
	&&  rcp[6] == 'r'
	&& !(map[rcp[7]]&(DIGIT|LETTER))) {
		cp = rcp + 7;
		return REGISTER;
	}
	if (rcp[0] == 'e'
	&&  rcp[1] == 't'
	&&  rcp[2] == 'u'
	&&  rcp[3] == 'r'
	&&  rcp[4] == 'n'
	&& !(map[rcp[5]]&(DIGIT|LETTER))) {
		cp = rcp + 5;
		return RETURN;
	}
	goto id;
case 's':
	if (rcp[0] == 'h'
	&&  rcp[1] == 'o'
	&&  rcp[2] == 'r'
	&&  rcp[3] == 't'
	&& !(map[rcp[4]]&(DIGIT|LETTER))) {
		cp = rcp + 4;
		tsym = shorttype->u.sym;
		return SHORT;
	}
	if (rcp[0] == 'i'
	&&  rcp[1] == 'g'
	&&  rcp[2] == 'n'
	&&  rcp[3] == 'e'
	&&  rcp[4] == 'd'
	&& !(map[rcp[5]]&(DIGIT|LETTER))) {
		cp = rcp + 5;
		return SIGNED;
	}
	if (rcp[0] == 'i'
	&&  rcp[1] == 'z'
	&&  rcp[2] == 'e'
	&&  rcp[3] == 'o'
	&&  rcp[4] == 'f'
	&& !(map[rcp[5]]&(DIGIT|LETTER))) {
		cp = rcp + 5;
		return SIZEOF;
	}
	if (rcp[0] == 't'
	&&  rcp[1] == 'a'
	&&  rcp[2] == 't'
	&&  rcp[3] == 'i'
	&&  rcp[4] == 'c'
	&& !(map[rcp[5]]&(DIGIT|LETTER))) {
		cp = rcp + 5;
		return STATIC;
	}
	if (rcp[0] == 't'
	&&  rcp[1] == 'r'
	&&  rcp[2] == 'u'
	&&  rcp[3] == 'c'
	&&  rcp[4] == 't'
	&& !(map[rcp[5]]&(DIGIT|LETTER))) {
		cp = rcp + 5;
		return STRUCT;
	}
	if (rcp[0] == 'w'
	&&  rcp[1] == 'i'
	&&  rcp[2] == 't'
	&&  rcp[3] == 'c'
	&&  rcp[4] == 'h'
	&& !(map[rcp[5]]&(DIGIT|LETTER))) {
		cp = rcp + 5;
		return SWITCH;
	}
	goto id;
case 't':
	if (rcp[0] == 'y'
	&&  rcp[1] == 'p'
	&&  rcp[2] == 'e'
	&&  rcp[3] == 'd'
	&&  rcp[4] == 'e'
	&&  rcp[5] == 'f'
	&& !(map[rcp[6]]&(DIGIT|LETTER))) {
		cp = rcp + 6;
		return TYPEDEF;
	}
	goto id;
case 'u':
	if (rcp[0] == 'n'
	&&  rcp[1] == 'i'
	&&  rcp[2] == 'o'
	&&  rcp[3] == 'n'
	&& !(map[rcp[4]]&(DIGIT|LETTER))) {
		cp = rcp + 4;
		return UNION;
	}
	if (rcp[0] == 'n'
	&&  rcp[1] == 's'
	&&  rcp[2] == 'i'
	&&  rcp[3] == 'g'
	&&  rcp[4] == 'n'
	&&  rcp[5] == 'e'
	&&  rcp[6] == 'd'
	&& !(map[rcp[7]]&(DIGIT|LETTER))) {
		cp = rcp + 7;
		return UNSIGNED;
	}
	goto id;
case 'v':
	if (rcp[0] == 'o'
	&&  rcp[1] == 'i'
	&&  rcp[2] == 'd'
	&& !(map[rcp[3]]&(DIGIT|LETTER))) {
		cp = rcp + 3;
		tsym = voidtype->u.sym;
		return VOID;
	}
	if (rcp[0] == 'o'
	&&  rcp[1] == 'l'
	&&  rcp[2] == 'a'
	&&  rcp[3] == 't'
	&&  rcp[4] == 'i'
	&&  rcp[5] == 'l'
	&&  rcp[6] == 'e'
	&& !(map[rcp[7]]&(DIGIT|LETTER))) {
		cp = rcp + 7;
		return VOLATILE;
	}
	goto id;
case 'w':
	if (rcp[0] == 'h'
	&&  rcp[1] == 'i'
	&&  rcp[2] == 'l'
	&&  rcp[3] == 'e'
	&& !(map[rcp[4]]&(DIGIT|LETTER))) {
		cp = rcp + 4;
		return WHILE;
	}
case 'h':
case 'j':
case 'k':
case 'm':
case 'n':
case 'o':
case 'p':
case 'q':
case 'x':
case 'y':
case 'z':
case 'A':
case 'B':
case 'C':
case 'D':
case 'E':
case 'F':
case 'G':
case 'H':
case 'I':
case 'J':
case 'K':
case 'M':
case 'N':
case 'O':
case 'P':
case 'Q':
case 'R':
case 'S':
case 'T':
case 'U':
case 'V':
case 'W':
case 'X':
case 'Y':
case 'Z':
case '_':
