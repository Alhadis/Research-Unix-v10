#include "c.h"
extern Interface mcInterface;
extern Interface nextInterface;
extern Interface mcInterface;
extern Interface mipsInterface;
extern Interface mipselInterface;
extern Interface sparcInterface;
extern Interface vaxInterface;
Interface *interfaces[] = {
	&mcInterface,
	&mcInterface,
	&nextInterface,
	&mcInterface,
	&mipsInterface,
	&mipselInterface,
	&sparcInterface,
	&vaxInterface,
	0
};
