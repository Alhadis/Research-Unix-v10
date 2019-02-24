	Filename = "c3134.h";
	checkthat(__LINE__, sizeof(L'x') == sizeof(wchar_t));
	iequals(__LINE__, L'a', 'a');
	iequals(__LINE__, L'\0', 0);
	Filename = "c31.c";
