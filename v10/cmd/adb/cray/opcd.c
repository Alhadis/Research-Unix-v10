/*       opcd.c               */
# include "mode.h"
# include "extern.h"
# include "functs.h"
# include "defs.h"

/* The following array contains the number of
   extra parcels for the instruction getter. It
   contains the assembler format for the
   instruction printer. It is machine dependent.
*/

struct itable itable[ ] = {
	{"ERR     %d%d%d",	0,	0 },
	{(char *)0,             0,      0200},
	{(char *)0,             0,      0222},
	{(char *)0,             0,      0233},
	{"EX	  %d%d%d",	0,	0 },
	{"J	B%w%d%d",	0,	0 },
	{"J	%d%d%d%8O",	1,	0 },
	{"R	%d%d%d%8O",	1,	0 },
/* 010 starting the conditional jumps */
	{(char *)0,             1,      0240},
	{(char *)0,             1,      0242},
	{(char *)0,             1,      0244},
	{(char *)0,             1,      0246},
	{(char *)0,             1,      0250},
	{(char *)0,             1,      0252},
	{(char *)0,             1,      0254},
	{(char *)0,             1,      0256},
/* 020 transmits                      */
	{"A%d      %d%d%8O",    1,	0 },
	{"A%d	   #%d%d%8O",	1,	0 },
	{"A%d      %d%d", 	0,	0 },
  	{(char *)0,             0,      0260},
	{"A%d	B%d%d",		0,	0 },
	{"B%w%d%d     A%b%b%b%d", 0,	0 },
	{(char *)0,             0,      0262},
	{(char *)0,             0,      0265},
/* 030 a-register arithmetic and register load   */
	{(char *)0,             0,      0267},
	{(char *)0,             0,      0272},
	{"A%d      A%d*A%d",    0,	0 },
	{(char *)0,             0,      0276},
	{"B%w%d%d,A%b%b%b%d  0,A0", 0,  0 },
	{"0,A0    B%w%d%d,A%b%b%b%d",0, 0 },
	{"T%w%d%d,A%b%b%b%d  0,A0", 0,  0 },
	{"0,A0    T%w%d%d,A%b%b%b%d",0, 0 },
/* 040           */
	{"S%d      %d%d%8O",    1,	0 },
	{"S%d      #%d%d%8O",   1,	0 },
	{(char *)0,             0,      0301},
	{(char *)0,             0,      0304},
	{(char *)0,             0,      0306},
	{(char *)0,             0,      0311},
	{(char *)0,             0,      0313},
	{(char *)0,             0,      0316},
/* 050 logical arithmetic and shifting       */
	{(char *)0,             0,      0323},
	{(char *)0,             0,      0325},
	{"S0      S%d<%d%d",	0,      0 },
	{"S0      S%d>0100-%d%d",0,     0 },
	{"S%d      S%b%d<%d%d", 0,      0 },
	{"S%d      S%b%d>0100-%d%d",0,  0 },
	{(char *)0,             0,      0332},
	{(char *)0,             0,      0335},
/* 060 floating-point arithmetic          */
        {"S%d      S%d+S%d",    0,      0 },
	{(char *)0,             0,      0340},
	{(char *)0,             0,      0342},
	{(char *)0,             0,      0344},
        {"S%d      S%d*FS%d",   0,      0 },
        {"S%d      S%d*HS%d",   0,      0 },
        {"S%d      S%d*RS%d",   0,      0 },
        {"S%d      S%d*IS%d",   0,      0 },
/* 070 transmits                          */
        {"S%d      \\HS%d",     0,      0 },
	{(char *)0,             0,      0346},
	{(char *)0,             0,      0356},
	{(char *)0,             0,      0361},
	{"S%d      T%d%d",      0,      0 },
	{"T%w%d%d     S%b%b%b%d", 0,    0 },
	{"S%d      V%d,A%d",    0,      0 },
	{(char *)0,             0,      0370},
/* 0100 read from memory to a-register  */
	{(char *)0,             1,      0372},
	{(char *)0,             1,      0372},
	{(char *)0,             1,      0372},
	{(char *)0,             1,      0372},
	{(char *)0,             1,      0372},
	{(char *)0,             1,      0372},
	{(char *)0,             1,      0372},
	{(char *)0,             1,      0372},
/* 0110 write from a-register to memory */
	{(char *)0,             1,      0375},
	{(char *)0,             1,      0375},
	{(char *)0,             1,      0375},
	{(char *)0,             1,      0375},
	{(char *)0,             1,      0375},
	{(char *)0,             1,      0375},
	{(char *)0,             1,      0375},
	{(char *)0,             1,      0375},
/* 0120 read from memory to s-register     */
	{(char *)0,             1,      0400},
	{(char *)0,             1,      0400},
	{(char *)0,             1,      0400},
	{(char *)0,             1,      0400},
	{(char *)0,             1,      0400},
	{(char *)0,             1,      0400},
	{(char *)0,             1,      0400},
	{(char *)0,             1,      0400},
/* 0130 write from s-register to memory     */
	{(char *)0,             1,      0403},
	{(char *)0,             1,      0403},
	{(char *)0,             1,      0403},
	{(char *)0,             1,      0403},
	{(char *)0,             1,      0403},
	{(char *)0,             1,      0403},
	{(char *)0,             1,      0403},
	{(char *)0,             1,      0403},
/* 0140 v-register logical instructions  */
	{"V%d      S%d&V%d",    0, 	0 },
	{"V%d      V%d&V%d",    0, 	0 },
	{(char *)0,             0,      0406},
	{"V%d      V%d!V%d",    0, 	0 },
	{"V%d      S%d\\V%d",   0, 	0 },
	{(char *)0,             0,      0410},
	{(char *)0,             0,      0412},
	{"V%d      V%d!V%d&VM", 0,  	0 },
/* 0150 shifting and v-register integer arithmetic */
	{(char *)0,             0,      0414},
	{(char *)0,             0,      0416},
	{(char *)0,             0,      0420},
	{(char *)0,             0,      0422},
	{"V%d      S%d+V%d",	0,      0 },
	{"V%d      V%d+V%d",	0,      0 },
	{(char *)0,             0,      0424},
	{"V%d      V%d-V%d",	0,      0 },
/* 0160 v-register floating-point arithmetic */
	{"V%d      S%d*FV%d",	0,      0 },
	{"V%d      V%d*FV%d",	0,	0 },
	{"V%d      S%d*HV%d",	0,	0 },
	{"V%d      V%d*HV%d",	0,	0 },
	{"V%d      S%d*RV%d",	0,	0 },
	{"V%d      V%d*RV%d",	0,	0 },
	{"V%d      S%d*IV%d",	0,	0 },
	{"V%d      V%d*IV%d",	0,	0 },
/* 0170 v-register floating-point, vector masking,etc.   */     
	{(char *)0,             0,      0426},
	{"V%d      V%d+FV%d",	0,	0 },
	{(char *)0,             0,      0430},
	{"V%d      V%d-FV%d",	0,	0 },
	{(char *)0,             0,      0432},
	{(char *)0,             0,      0435},
	{(char *)0,             0,      0445},
	{(char *)0,             0,      0450},

/*  Subinstructions for instruction type 001   */
   	{"CA,A%w%d   A%d",      0,  	0 },  
   	{"CL,A%w%d   A%d",      0,  	0 },  
   	{"CI,A%w%d",	        0,  	0 },  
   	{"MC,A%w%d",	        0,  	0 },  
   	{"XA      A%w%d",	0,	0 },  
   	{"RT      S%w%d",	0,	0 },  
   	{"SIPI    %w%d",	0,	0 },  
   	{"CIPI",		0,	0 },  
   	{"CLN     %w%d",	0,	0 },  
   	{"PCI     S%w%d",	0,	0 },  
   	{"CCI",			0,	0 },  
   	{"ECI", 		0,	0 },  
   	{"DCI",	   		0,	0 },  
   	{"select perf mon",	0,	0 },  
   	{"set maint read",	0,	0 },  
   	{"load diag byte",	0,	0 },  
   	{"set maint write 1",	0,	0 },  
   	{"set maint write 2",	0,	0 },  
/*  Subinstructions for instruction type 002   */
        {"VL      A%w%w%d",	0,	0 },
        {"VL      1",		0,	0 },
   	{"EFI",	   		0,	0 },  
   	{"DFI",	   		0,	0 },  
   	{"ERI",	   		0,	0 },  
   	{"DRI",	   		0,	0 },  
   	{"DBM",	   		0,	0 },  
   	{"EBM",	   		0,	0 },  
   	{"CMR",	   		0,	0 },  
/*  Subinstructions for instruction type 003   */
   	{"VM      S%w%d",	0,	0 },
   	{"VM      0",		0,	0 },
   	{"SM%w%d%d    1,TS",	0,	0 },
   	{"SM%w%d%d    0",	0,	0 },
   	{"SM%w%d%d    1",	0,	0 },
/*  Subinstructions for instruction type 010   */
	{"JAZ     %d%d%d%8O",   1,      0 },
	{"A0      %d%d%d%8O",   1,      0 },
/*  Subinstructions for instruction type 011   */
        {"JAN     %d%d%d%8O",   1,      0 },
	{"A1      %d%d%d%8O",   1,      0 },
/*  Subinstructions for instruction type 012   */
        {"JAP     %d%d%d%8O",   1,      0 },
	{"A2      %d%d%d%8O",   1,      0 },
/*  Subinstructions for instruction type 013   */
        {"JAM     %d%d%d%8O",   1,      0 },
	{"A3      %d%d%d%8O",   1,      0 },
/*  Subinstructions for instruction type 014   */
        {"JSZ     %d%d%d%8O",   1,      0 },
	{"A4      %d%d%d%8O",   1,      0 },
/*  Subinstructions for instruction type 015   */
        {"JSN     %d%d%d%8O",   1,      0 },
	{"A5      %d%d%d%8O",   1,      0 },
/*  Subinstructions for instruction type 016   */
        {"JSP     %d%d%d%8O",   1,      0 },
	{"A6      %d%d%d%8O",   1,      0 },
/*  Subinstructions for instruction type 017   */
        {"JSM     %d%d%d%8O",   1,      0 },
	{"A7      %d%d%d%8O",   1,      0 },
/*  Subinstructions for instruction type 023   */
	{"A%d      S%d", 	0,	0 },
	{"A%d      VL",  	0,	0 },
/*  Subinstructions for instruction type 026   */
	{"A%d      PS%d",	0,	0 },
	{"A%d      QS%d",	0,	0 },
	{"A%d      SB%d",	0,	0 },
/*  Subinstructions for instruction type 027   */
	{"A%d      ZS%d",	0,	0 },
	{"SB%w%d     A%b%b%d",	0,	0 },
/*  Subinstructions for instruction type 030   */
	{"A%d      A%d+A%d",	0,	0 },
	{"A%d      A%w%d",  	0,	0 },
	{"A%d      A%d+1",	0,	0 },
/*  Subinstructions for instruction type 031   */
	{"A%d      A%d-A%d",	0,	0 },
	{"A%d      -1",         0,	0 },
	{"A%d      -A%w%d",  	0,	0 },
	{"A%d      A%d-1",  	0,	0 },
/*  Subinstructions for instruction type 033   */
	{"A%d      CI",     	0,	0 },
	{"A%d      CA,A%d",    	0,	0 },
	{"A%d      CE,A%d",    	0,	0 },
/*  Subinstructions for instruction type 042   */
	{"S%d      <0100-%d%d", 0,	0 },
	{"S%d      1",          0,	0 },
	{"S%d      -1",         0,	0 },
/*  Subinstructions for instruction type 043 */
	{"S%d      >%d%d",	0,	0 },
	{"S%d      0",          0,	0 },
/*  Subinstructions for instruction type 044 */
	{"S%d      S%d&S%d",    0,	0 },
	{"S%d      S%d&SB",     0,	0 },
	{"S%d      SB&S%d",     0,	0 },
/*  Subinstructions for instruction type 045 */
	{"S%d      #S%w%d&S%b%b%d",0,	0 },
	{"S%d      #SB&S%d",    0,	0 },
/*  Subinstructions for instruction type 046 */
	{"S%d      S%d\\S%d",	0,	0 },
	{"S%d      S%d\\SB",	0,	0 },
	{"S%d      SB\\S%d",	0,	0 },
/*  Subinstructions for instruction type 047 */
	{"S%d      #S%d\\S%d",	0,	0 },
	{"S%d      #S%w%d",	0,	0 },
	{"S%d      #S%d\\SB",	0,	0 },
	{"S%d      #SB\\S%d",	0,	0 },
	{"S%d      #SB",	0,	0 },
/*  Subinstructions for instruction type 050 */
	{"S%d      S%d!S%b%b%d&S%w%d",0,0 },
	{"S%d      S%d!S%b%b%d&SB",0,	0 },
/*  Subinstructions for instruction type 051 */
	{"S%d      S%d!S%d",	0,	0 },
	{"S%d      S%w%d",	0,	0 },
	{"S%d      S%d!SB",	0,	0 },
	{"S%d      SB!S%d",	0,	0 },
	{"S%d      SB",		0,	0 },
/*  Subinstructions for instruction type 056 */
	{"S%d      S%b%d,S%d<A%d",0,	0 },
	{"S%d      S%b%d,S%d<1",0,	0 },
	{"S%d      S%b%d<A%d",  0,	0 },
/*  Subinstructions for instruction type 057 */
	{"S%d      S%d,S%b%b%d>A%w%d",0,0 },
	{"S%d      S%d,S%b%b%d>1",0,	0 },
	{"S%d      S%b%d>A%w%d",0,	0 },
/*  Subinstructions for instruction type 061 */
	{"S%d      S%d-S%d",	0,	0 },
	{"S%d      -S%w%d",	0,	0 },
/*  Subinstructions for instruction type 062 */
	{"S%d      S%d+FS%d",	0,	0 },
	{"S%d      +FS%w%d",	0,	0 },
/*  Subinstructions for instruction type 063 */
	{"S%d      S%d-FS%d",	0,	0 },
	{"S%d      -FS%w%d",	0,	0 },
/*  Subinstructions for instruction type 071 */
	{"S%d      A%w%d",	0,	0 },
	{"S%d      +A%w%d",	0,	0 },
	{"S%d      +FA%w%d",	0,	0 },
	{"S%d      0.6",  	0,	0 },
	{"S%d      0.4",  	0,	0 },
	{"S%d      1.",  	0,	0 },
	{"S%d      2.",  	0,	0 },
	{"S%d      4.",  	0,	0 },
/*  Subinstructions for instruction type 072 */
	{"S%d      RT",  	0,	0 },
	{"S%d      SM",  	0,	0 },
	{"S%d      ST%d",  	0,	0 },
/*  Subinstructions for instruction type 073 */
	{"S%d      VM",  	0,	0 },
	{"S%d      SR%d",  	0,	0 },
	{"read perf count",	0,	0 },
	{"incr perf count",	0,	0 },
	{"clear maint modes",	0,	0 },
	{"SM      S%d", 	0,	0 },
	{"ST%w%d     S%b%b%d", 	0,	0 },
/*  Subinstructions for instruction type 077 */
	{"V%d,A%w%d   S%b%b%d",	0,	0 },
	{"V%d,A%w%d   0",	0,	0 },
/*  Subinstructions for instruction type 10h */
	{"A%d      %d%d%8O,A%d", 1, 	0 },
	{"A%d      %d%d%8O,0",  1, 	0 },
	{"A%d      0,A%w%w%w%d", 1, 	0 },
/*  Subinstructions for instruction type 11h */
	{"%w%d%d%8O,A%d  A%b%b%b%b%b%d",1,0 },
	{"%w%d%d%8O,0   A%b%b%b%b%d",1,0 },
	{"0,A%w%w%w%w%d    A%b%b%b%b%b%d",1,0 },
/*  Subinstructions for instruction type 12h */
	{"S%d      %d%d%8O,A%d", 1, 	0 },
	{"S%d      %d%d%8O,0",  1, 	0 },
	{"S%d      0,A%w%w%w%d", 1, 	0 },
/*  Subinstructions for instruction type 13h */
	{"%w%d%d%8O,A%d  S%b%b%b%b%b%d",1,0 },
	{"%w%d%d%8O,0   S%b%b%b%b%d",1,0 },
	{"0,A%w%w%w%w%d    S%b%b%b%b%b%d",1,0 },
/*  Subinstructions for instruction type 142 */
	{"V%d      S%d!V%d",	0,	0 },
	{"V%d      V%w%d",	0,	0 },
/*  Subinstructions for instruction type 145 */
	{"V%d      V%d\\V%d", 	0,	0 },
	{"V%d      0",	 	0,	0 },
/*  Subinstructions for instruction type 146 */
	{"V%d      S%d!V%d&VM",	0,	0 },
	{"V%d      #VM&V%w%d",	0,	0 },
/*  Subinstructions for instruction type 150 */
	{"V%d      V%d<A%d",	0,	0 },
	{"V%d      V%d<1",	0,	0 },
/*  Subinstructions for instruction type 151 */
	{"V%d      V%d>A%d",	0,	0 },
	{"V%d      V%d>1",	0,	0 },
/*  Subinstructions for instruction type 152 */
	{"V%d      V%d,V%b%d<A%d",0,	0 },
	{"V%d      V%d,V%b%d<1",0,	0 },
/*  Subinstructions for instruction type 153 */
	{"V%d      V%d,V%b%d>A%d",0,	0 },
	{"V%d      V%d,V%b%d>1",0,	0 },
/*  Subinstructions for instruction type 156 */
	{"V%d      S%d-V%d",	0,	0 },
	{"V%d      -V%w%d",	0,	0 },
/*  Subinstructions for instruction type 170 */
	{"V%d      S%d+FV%d",	0,	0 },
	{"V%d      +FV%w%d",	0,	0 },
/*  Subinstructions for instruction type 172 */
	{"V%d      S%d-FV%d",	0,	0 },
	{"V%d      -FV%w%d",	0,	0 },
/*  Subinstructions for instruction type 174 */
	{"V%d      \\HV%d",	0,	0 },
	{"V%d      PV%d",	0,	0 },
	{"V%d      QV%d",	0,	0 },
/*  Subinstructions for instruction type 175 */
	{"VM       V%w%d,Z",	0,	0 },
	{"VM       V%w%d,N",	0,	0 },
	{"VM       V%w%d,P",	0,	0 },
	{"VM       V%w%d,M",	0,	0 },
	{"V%d,VM   V%d,Z",	0,	0 },
	{"V%d,VM   V%d,N",	0,	0 },
	{"V%d,VM   V%d,P",	0,	0 },
	{"V%d,VM   V%d,M",	0,	0 },
/*  Subinstructions for instruction type 176 */
	{"V%d      ,A0,A%w%d",	0,	0 },
	{"V%d      ,A0,1",	0,	0 },
	{"V%d      ,A0,V%w%d",	0,	0 },
/*  Subinstructions for instruction type 177 */
	{",A0,A%w%w%d  V%b%b%d",0,	0 },
	{",A0,1   V%w%d",	0,	0 },
	{",A0,V%w%w%d  V%b%b%d",0,	0 },
};

/* The second parameter ins is the numeric op code for the instruction.
*/
void printins(idsp, ins)
	int idsp;
	long ins;
{
	long ops[5];
	long blank = -99;
	int itaboffs;
	if (dot & 01) {
		adbpr("not on parcel boundary\n");
		return;
	}
	ins = ins >> 48;
	ins = ins & 0177777;
	dotinc = setins ( (unsigned long)ins, dot , idsp);
	ops[0] = instr.opi;
	ops[1] = instr.opj;
	ops[2] = instr.opk;
	ops[3] = instr.opother;
	ops[4] = instr.oph;
	if (itable[instr.opcode].iformat == 0) {
		itaboffs = getoffs (itable[instr.opcode].ioffset);
		adbpr (itable[itaboffs].iformat, blank, ops);
	}
	else
		adbpr ( itable[instr.opcode].iformat , blank, ops );
}

/* Routine setins is used by the disassembler and by
   the instruction stepper to build the global instr
   structure and to set dotinc to the size of the 
   instruction. Upon entry, the first parcel of the
   instruction is in the unsigned long w.
*/
int setins ( w, mydot, idsp)
	register unsigned long w;
	long mydot;
	int idsp;
{
	instr.opcode = (w >> 9) & 0177;
	instr.oph = (w >> 9) & 07;
	instr.opi = (w >> 6) & 07;
	instr.opj = (w >> 3) & 07;
	instr.opk = (w) & 07;
	parcels = itable[instr.opcode].iparcels;
	if (parcels == 1)
         	 instr.opother = (unsigned long)(get(mydot+2,idsp)>>48);
	return(2 * (1+parcels));
}

/* Routine getoffs is what is used to handle those opcodes that could 
   represent more than one instruction.  This routine is simply one large
   switch with a manual lookup into the subinstruction part of the table
*/
int
getoffs (ioffs)
int ioffs;
{
	switch (ioffs) {
/*001*/	case 0200:	switch(instr.opi) {
			case 0:	break;
			case 1: ioffs += 01; break;
			case 2: if(instr.opk == 0) ioffs += 02;
				else ioffs += 03; 
				break;
			case 3: ioffs += 04; break;
			case 4:	switch(instr.opk) {
				case 0: if (instr.opj == 2) ioffs += 07;
					else ioffs += 05;
					break;
				case 1: ioffs += 06; break;
				case 3:
				case 4:
				case 5:
				case 6:
				case 7: 
					ioffs = ioffs + 05 + instr.opk;
					break;
				}
			case 5: if (instr.opk == 0) ioffs += 015;
				else ioffs = ioffs + 016 + instr.opj;
				break;
			}
/*002*/	case 0222:	if (instr.opk != 0) break;
			else ioffs = ioffs + 01 + instr.opi;
			break;
/*003*/	case 0233:	switch(instr.opi) {
			case 0: if (instr.opj != 0) break;
				else ioffs += 01; 
				break;
			case 4: ioffs += 02; break;
			case 6: ioffs += 03; break;
			case 7: ioffs += 04; break;
			}
/*010*/	case 0240:
/*011*/	case 0242:
/*012*/	case 0244:
/*013*/	case 0246:
/*014*/	case 0250:
/*015*/	case 0252:
/*016*/	case 0254:
/*017*/	case 0256:	if ((instr.opi>>2)&01 == 1) ioffs += 01; 
			break;
/*023*/	case 0260:	if (instr.opk == 1) 
				 ioffs += 01;
			break;
/*026*/	case 0262:	switch(instr.opk) {
			case 0: break;
			case 1: ioffs += 01; break;
			case 7: ioffs += 02; break;
			}
/*027*/	case 0265:     	if (instr.opk == 7) 
				 ioffs += 01;
			break;
/*030*/	case 0267:	if (instr.opj == 0) ioffs += 01;	
			if (instr.opk == 0) ioffs += 02;	
			break;
/*031*/	case 0272: 	if (instr.opj == 0) {
				if (instr.opk == 0) ioffs += 01;
				else ioffs += 02;
				break;
			}
			if (instr.opk == 0) ioffs += 03;
			break;
/*033*/	case 0276: 	if (instr.opj == 0) break;
			if (instr.opk == 0) ioffs += 01;
			else ioffs += 02;
			break;
/*042*/ case 0301:	if ((instr.opj==0)&(instr.opk == 0)) ioffs += 02;
			if ((instr.opj==7)&(instr.opk == 7)) ioffs += 01;
			break;
/*043*/	case 0304:	if ((instr.opj == 0) & (instr.opk == 0)) ioffs += 01;
			break;
/*044*/	case 0306:	switch (instr.opk) {
			case 0: if (instr.opj == 0) ioffs += 01;
				else ioffs += 02;
				break;
			default: break;
			}
/*045*/	case 0311:
/*050*/	case 0323:
/*150*/	case 0414:
/*151*/ case 0416:
/*152*/	case 0420:
/*153*/	case 0422:
		        if (instr.opk == 0) ioffs += 01;
			break;
/*046*/	case 0313:      switch (instr.opk) {
                        case 0: if (instr.opj == 0) ioffs += 01;
                                else ioffs += 02;
                                break;
                        default: break;
			}
/*047*/	case 0316:
/*051*/	case 0325:
			if (instr.opk == 0) {
				if (instr.opj == 0) ioffs += 04;
				else ioffs += 03;
				break;
			}
			else if (instr.opj == 0) ioffs += 01;
           		break;
/*056*/	case 0332:
/*057*/	case 0335:
			if (instr.opj == 0) ioffs += 02;       
                        if (instr.opk == 0) ioffs += 01;       
                        break;
/*061*/	case 0340:
/*062*/	case 0342:	
/*063*/	case 0344:	
/*077*/	case 0370:	
/*142*/	case 0406:	
/*146*/	case 0412:	
/*156*/	case 0424:	
/*170*/	case 0426:	
/*172*/	case 0430:	
	                if (instr.opj == 0) ioffs += 01;
			break;
/*071*/	case 0346:	ioffs += instr.opj;
			break;
/*072*/	case 0356:	switch (instr.opk) {
			case 0: break;
			case 2: ioffs += 01; break;
			case 3: ioffs += 02; break;
			}
/*073*/	case 0361:	switch (instr.opk) {
			case 0: break;
			case 1: ioffs = ioffs + 01 + instr.opj; break;
			case 2: ioffs += 05; break;
			case 3: ioffs += 06; break;
			}
/*10h*/	case 0372:
/*11h*/	case 0375:
/*12h*/	case 0400:
/*13h*/	case 0403:
			if (instr.oph == 0) ioffs += 01;
		       else if((instr.opj==0)&(instr.opk==0)&(instr.opother==0))
				ioffs += 02;
			break;	
/*145*/	case 0410:	if ((instr.opi == instr.opj) == instr.opk)	
				ioffs += 01;
			break;
/*174*/	case 0432:
/*175*/	case 0435:
			ioffs += instr.opk;
			break;
/*176*/	case 0445:	if (instr.opj == 1) ioffs += 02;
			else if (instr.opk == 0) ioffs += 01;
			break;
/*177*/	case 0450:	if (instr.opi == 1) {ioffs += 02; break;}
			if (instr.opk == 0) ioffs += 01;
			break;
	}
	return (ioffs);
}	
