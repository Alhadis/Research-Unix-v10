#define abtsize 71
#define abp1 61
#define abp2 67
extern struct dict abbrev_d[];
struct hnode abroot[abtsize];
char
abbrev(a0,a1,ptr)
	char *a0;
	struct dict *ptr;
{
	return(aahash(a0,a1,abtsize,abp1,abp2,abroot,ptr));
}
getab(){
	struct dict *ptr;
	ptr = abbrev_d;
	while(ptr->entry != 0){
		abbrev(ptr->entry,0,ptr);
		ptr++;
	}
}
struct dict abbrev_d[] = {
"St",'N',
"Dr",'N',
"Drs",'N',
"Mr",'N',
"Mrs",'N',
"Ms",'N',
"Rev",'N',
"No",'Y',
"Nos",'Y',
"NO",'Y',
"NOs",'Y',
"no",'Y',
"Fig",'Y',
"Figs",'Y',
"Dept",'Y',
"Depts",'Y',
"dept",'Y',
"depts",'Y',
"Eq",'Y',
"Eqs",'Y',
"eq",'Y',
"eqs",'Y',
"dB",'Y',
"vs",'P',
"in",'Y',
"ft",'Y',
"yr",'Y',
"ckts",'Y',
"mi",'Y',
"Jr",'J',
"jr",'J',
"Ch",'Y',
"ch",'Y',
"Ref",'Y',
"Refs",'Y',
"ref",'Y',
"refs",'Y',
"Inc",'J',
"Co",'N',
"Corp",'N',
"Jan",'N',
"Feb",'N',
"Mar",'N',
"Apr",'N',
"Jun",'N',
"Aug",'N',
"Sept",'N',
"Oct",'N',
"Nov",'N',
"Dec",'N',
"Sen",'Y',
"Sens",'Y',
"Rep",'Y',
"Hon",'Y',
"Gov",'Y',
"Lt",'Y',
"Col",'Y',
"Comdr",'Y',
"Cmdr",'Y',
"Capt",'Y',
"Calif",'N',
"Ky",'N',
"Va",'N',
0,0
};
