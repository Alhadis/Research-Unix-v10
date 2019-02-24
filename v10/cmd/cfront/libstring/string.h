// <string.h>: the C string functions, UNIX Programmer's Manual vol 1 section 3

extern "C" {
	extern char* strcat(char*, const char*);
	extern char* strncat(char*, const char*, int);
	extern int strcmp(const char*, const char*);
	extern int strncmp(const char*, const char*, int);
	extern char* strcpy(char*, const char*);
	extern char* strncpy(char*, const char*, int);
	extern int strlen(const char*);
	// bsd:
	//extern char* index(char*, int);
	//extern char* rindex(char*, int);
	extern char* strchr(const char*, int);
	extern char* strrchr(const char*, int);
	extern char* strpbrk(const char*, const char*);
	extern int strspn(const char*, const char*);
	extern int strcspn(const char*, const char*);
	extern char* strtok(char*, const char*);
	extern char* strdup(const char*);
}
