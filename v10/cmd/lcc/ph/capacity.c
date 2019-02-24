/* The Plum Hall Validation Suite for C
 * Published copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The contents of this file may be shown to any persons, provided 
 * that no copies are transfered to any persons not licensed to receive such copies.
 * Successful compilation and execution of this program satisfies 2.2.4.1 of the C Standard
 */

/* b2241.c -- test translation limits (2.2.4.1) */
#define verify(ln, expr) { if (!(expr)) printf("execution fails, line %d\n", ln), ++errs; }

#ifndef FREESTANDING
extern int printf(const char *format, ...);
#else
/* FREESTANDING validation must provide some means of printing  string  */
static int Zero = 0; /* needed in sdutil.h */
#include "sdutil.h"
int printf(const char *string, int n)
	{
	pr_ok(string); /* bare-bones, adequate if no errors from  verify  */
	}
#endif /* FREESTANDING */

void cat31(char *s, int dummy,
	char  c0,char  c1,char  c2,char  c3,char  c4,char  c5,char  c6,char  c7,char  c8,char  c9,
	char c10,char c11,char c12,char c13,char c14,char c15,char c16,char c17,char c18,char c19,
	char c20,char c21,char c22,char c23,char c24,char c25,char c26,char c27,char c28)
	{
	s[0]=c0; s[1]=c1; s[2]=c2; s[3]=c3; s[4]=c4; s[5]=c5; s[6]=c6; s[7]=c7; s[8]=c8; s[9]=c9;
	s[10]=c10; s[11]=c11; s[12]=c12; s[13]=c13; s[14]=c14; s[15]=c15; s[16]=c16; s[17]=c17; s[18]=c18; s[19]=c19;
	s[20]=c20; s[21]=c21; s[22]=c22; s[23]=c23; s[24]=c24; s[25]=c25; s[26]=c26; s[27]=c27; s[28]=c28;
	s[29] = '\0';
	}

int str_cmp(const char *s1, const char *s2)
	{
	for (; *s1 == *s2; ++s1, ++s2)
		if (*s1 == '\0')
			return 0;
	return 1;
	}

/* #7: 6 significant characters in an external identifier */
short e23456 = 6;
short e2345  = 5;

/* str_len -- used here to avoid need for  size_t  definition */
int str_len(s)
	char *s;
	{
	int i;

	for (i = 0; s[i] != '\0'; ++i)
		;
	return i;
	}

/* #8: 511 external identifiers in one source file */
/* e2345 e23456 str_len main str_cmp cat31 printf errs, plus 503 here */
char i000=0,i001=0,i002=0,i003=0,i004=0,i005=0,i006=0,i007=0,i008=0,i009=0;
char i010=0,i011=0,i012=0,i013=0,i014=0,i015=0,i016=0,i017=0,i018=0,i019=0;
char i020=0,i021=0,i022=0,i023=0,i024=0,i025=0,i026=0,i027=0,i028=0,i029=0;
char i030=0,i031=0,i032=0,i033=0,i034=0,i035=0,i036=0,i037=0,i038=0,i039=0;
char i040=0,i041=0,i042=0,i043=0,i044=0,i045=0,i046=0,i047=0,i048=0,i049=0;
char i050=0,i051=0,i052=0,i053=0,i054=0,i055=0,i056=0,i057=0,i058=0,i059=0;
char i060=0,i061=0,i062=0,i063=0,i064=0,i065=0,i066=0,i067=0,i068=0,i069=0;
char i070=0,i071=0,i072=0,i073=0,i074=0,i075=0,i076=0,i077=0,i078=0,i079=0;
char i080=0,i081=0,i082=0,i083=0,i084=0,i085=0,i086=0,i087=0,i088=0,i089=0;
char i090=0,i091=0,i092=0,i093=0,i094=0,i095=0,i096=0,i097=0,i098=0,i099=0;
char i100=0,i101=0,i102=0,i103=0,i104=0,i105=0,i106=0,i107=0,i108=0,i109=0;
char i110=0,i111=0,i112=0,i113=0,i114=0,i115=0,i116=0,i117=0,i118=0,i119=0;
char i120=0,i121=0,i122=0,i123=0,i124=0,i125=0,i126=0,i127=0,i128=0,i129=0;
char i130=0,i131=0,i132=0,i133=0,i134=0,i135=0,i136=0,i137=0,i138=0,i139=0;
char i140=0,i141=0,i142=0,i143=0,i144=0,i145=0,i146=0,i147=0,i148=0,i149=0;
char i150=0,i151=0,i152=0,i153=0,i154=0,i155=0,i156=0,i157=0,i158=0,i159=0;
char i160=0,i161=0,i162=0,i163=0,i164=0,i165=0,i166=0,i167=0,i168=0,i169=0;
char i170=0,i171=0,i172=0,i173=0,i174=0,i175=0,i176=0,i177=0,i178=0,i179=0;
char i180=0,i181=0,i182=0,i183=0,i184=0,i185=0,i186=0,i187=0,i188=0,i189=0;
char i190=0,i191=0,i192=0,i193=0,i194=0,i195=0,i196=0,i197=0,i198=0,i199=0;
char i200=0,i201=0,i202=0,i203=0,i204=0,i205=0,i206=0,i207=0,i208=0,i209=0;
char i210=0,i211=0,i212=0,i213=0,i214=0,i215=0,i216=0,i217=0,i218=0,i219=0;
char i220=0,i221=0,i222=0,i223=0,i224=0,i225=0,i226=0,i227=0,i228=0,i229=0;
char i230=0,i231=0,i232=0,i233=0,i234=0,i235=0,i236=0,i237=0,i238=0,i239=0;
char i240=0,i241=0,i242=0,i243=0,i244=0,i245=0,i246=0,i247=0,i248=0,i249=0;
char i250=0,i251=0,i252=0,i253=0,i254=0,i255=0,i256=0,i257=0,i258=0,i259=0;
char i260=0,i261=0,i262=0,i263=0,i264=0,i265=0,i266=0,i267=0,i268=0,i269=0;
char i270=0,i271=0,i272=0,i273=0,i274=0,i275=0,i276=0,i277=0,i278=0,i279=0;
char i280=0,i281=0,i282=0,i283=0,i284=0,i285=0,i286=0,i287=0,i288=0,i289=0;
char i290=0,i291=0,i292=0,i293=0,i294=0,i295=0,i296=0,i297=0,i298=0,i299=0;
char i300=0,i301=0,i302=0,i303=0,i304=0,i305=0,i306=0,i307=0,i308=0,i309=0;
char i310=0,i311=0,i312=0,i313=0,i314=0,i315=0,i316=0,i317=0,i318=0,i319=0;
char i320=0,i321=0,i322=0,i323=0,i324=0,i325=0,i326=0,i327=0,i328=0,i329=0;
char i330=0,i331=0,i332=0,i333=0,i334=0,i335=0,i336=0,i337=0,i338=0,i339=0;
char i340=0,i341=0,i342=0,i343=0,i344=0,i345=0,i346=0,i347=0,i348=0,i349=0;
char i350=0,i351=0,i352=0,i353=0,i354=0,i355=0,i356=0,i357=0,i358=0,i359=0;
char i360=0,i361=0,i362=0,i363=0,i364=0,i365=0,i366=0,i367=0,i368=0,i369=0;
char i370=0,i371=0,i372=0,i373=0,i374=0,i375=0,i376=0,i377=0,i378=0,i379=0;
char i380=0,i381=0,i382=0,i383=0,i384=0,i385=0,i386=0,i387=0,i388=0,i389=0;
char i390=0,i391=0,i392=0,i393=0,i394=0,i395=0,i396=0,i397=0,i398=0,i399=0;
char i400=0,i401=0,i402=0,i403=0,i404=0,i405=0,i406=0,i407=0,i408=0,i409=0;
char i410=0,i411=0,i412=0,i413=0,i414=0,i415=0,i416=0,i417=0,i418=0,i419=0;
char i420=0,i421=0,i422=0,i423=0,i424=0,i425=0,i426=0,i427=0,i428=0,i429=0;
char i430=0,i431=0,i432=0,i433=0,i434=0,i435=0,i436=0,i437=0,i438=0,i439=0;
char i440=0,i441=0,i442=0,i443=0,i444=0,i445=0,i446=0,i447=0,i448=0,i449=0;
char i450=0,i451=0,i452=0,i453=0,i454=0,i455=0,i456=0,i457=0,i458=0,i459=0;
char i460=0,i461=0,i462=0,i463=0,i464=0,i465=0,i466=0,i467=0,i468=0,i469=0;
char i470=0,i471=0,i472=0,i473=0,i474=0,i475=0,i476=0,i477=0,i478=0,i479=0;
char i480=0,i481=0,i482=0,i483=0,i484=0,i485=0,i486=0,i487=0,i488=0,i489=0;
char i490=0,i491=0,i492=0,i493=0,i494=0,i495=0,i496=0,i497=0,i498=0,i499=0;
char i500=0,i501=0,i502=0;

int errs = 0;

int main()
{

/* verify the 6 significant chars on extern */
    verify(__LINE__, e2345 == 5);

/* verify that the externals are accessible and distinct */
	i450+=i451+=i452+=i453+=i454+=i455+=i456+=i457+=i458+=i459+=1;
	i460+=i461+=i462+=i463+=i464+=i465+=i466+=i467+=i468+=i469+=1;
	i470+=i471+=i472+=i473+=i474+=i475+=i476+=i477+=i478+=i479+=1;
	i480+=i481+=i482+=i483+=i484+=i485+=i486+=i487+=i488+=i489+=1;
	i490+=i491+=i492+=i493+=i494+=i495+=i496+=i497+=i498+=i499+=1;
    verify(__LINE__, i450 == 1);
    
/* #1: 15 nesting levels of compound statements and selection control */
    if(i451){if(i452){if(i453){if(i454){if(i455){
    if(i456){if(i457){if(i458){if(i459){if(i460){
    if(i461){if(i462){if(i463){if(i464){if(i465)
        i100 = 1;
    else
        i100 = 0;
        } } } } } } } } } } } } } } 
    /*  1 2 3 4 5 6 7 8 9 0 1 2 3 4 */
    verify(__LINE__, i100 == 1);
    
/* #2: 8 nesting levels in conditional compilation */
#define NEST_OK 0
#if 1
#if 2
#if 3
#if 4
#if 5
#if 6
#if 7
#if 8
#undef NEST_OK
#define NEST_OK 1
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif


    verify(__LINE__, NEST_OK == 1);

/* #3: 12 ptr, array, and fn declarators modifying a basic type */
    {
    char   c = 'a';
    char   *p1 = &c;
    char   **p2 = &p1;
    char   ***p3 = &p2;
    char   ****p4 = &p3;
    char   *****p5 = &p4;
    char   ******p6 = &p5;
    char   *******p7 = &p6;
    char   ********p8 = &p7;
    char   *********p9 = &p8;
    char   **********p10 = &p9;
    char   ***********p11 = &p10;
    char   ************p12 = &p11;

    verify(__LINE__, ************p12 == 'a');
    }

/* #4: 31 declarators nested by parentheses  */
    {
	/*  1234567890123456789012345678901 1234567890123456789012345678901 */
    int (((((((((((((((((((((((((((((((i))))))))))))))))))))))))))))))) = 2;

	verify(__LINE__, i == 2);
    }
/* #5: 32 expressions nested by parentheses (ANSI8709 reduced this from 127) */
    {
          /* 1  2  3  4  5  6  7  8  9  10 */
#define L(a) (1-(1-(1-(1-(1-(1-(1-(1-(1-(1-a
#define R    )  )  )  )  )  )  )  )  )  )  
    /* L(x) generates 10 left parens, 30 chars (plus length of x) */
    int i = 2;
    int j;
    

    j =
        L(L(L((1-(i-1)))))
         R R R;
	verify(__LINE__, j == 0);
    }

/* #6: 31 significant characters in an internal identifier or macro name */
#define M234567890123456789012345678901 1
#define M23456789012345678901234567890  0
    verify(__LINE__, M234567890123456789012345678901 == 1);

/* See top of file for #7 and #8 */

/* #9: 127 identifiers with block scope in one block */
    {
    auto char i000=0,i001=0,i002=0,i003=0,i004=0,i005=0,i006=0,i007=0,i008=0,i009=0;
	auto char i010=0,i011=0,i012=0,i013=0,i014=0,i015=0,i016=0,i017=0,i018=0,i019=0;
	auto char i020=0,i021=0,i022=0,i023=0,i024=0,i025=0,i026=0,i027=0,i028=0,i029=0;
	auto char i030=0,i031=0,i032=0,i033=0,i034=0,i035=0,i036=0,i037=0,i038=0,i039=0;
	auto char i040=0,i041=0,i042=0,i043=0,i044=0,i045=0,i046=0,i047=0,i048=0,i049=0;
    auto char i050=0,i051=0,i052=0,i053=0,i054=0,i055=0,i056=0,i057=0,i058=0,i059=0;
	auto char i060=0,i061=0,i062=0,i063=0,i064=0,i065=0,i066=0,i067=0,i068=0,i069=0;
	auto char i070=0,i071=0,i072=0,i073=0,i074=0,i075=0,i076=0,i077=0,i078=0,i079=0;
	auto char i080=0,i081=0,i082=0,i083=0,i084=0,i085=0,i086=0,i087=0,i088=0,i089=0;
	auto char i090=0,i091=0,i092=0,i093=0,i094=0,i095=0,i096=0,i097=0,i098=0,i099=0;
    auto char i100=0,i101=0,i102=0,i103=0,i104=0,i105=0,i106=0,i107=0,i108=0,i109=0;
	auto char i110=0,i111=0,i112=0,i113=0,i114=0,i115=0,i116=0,i117=0,i118=0,i119=0;
    auto char i120=0,i121=0,i122=0,i123=0,i124=0,i125=0,i126=0;

	i050+=i051+=i052+=i053+=i054+=i055+=i056+=i057+=i058+=i059+=1;
	i060+=i061+=i062+=i063+=i064+=i065+=i066+=i067+=i068+=i069+=1;
	i070+=i071+=i072+=i073+=i074+=i075+=i076+=i077+=i078+=i079+=1;
	i080+=i081+=i082+=i083+=i084+=i085+=i086+=i087+=i088+=i089+=1;
	i090+=i091+=i092+=i093+=i094+=i095+=i096+=i097+=i098+=i099+=1;
    verify(__LINE__, i050 == 1);
    }

/* #10: 1024 macro identifiers simultaneously defined in one source file */
/* 9 macros defined elsewhere by capacity.c, 1 in verify,
   and 1014 are defined in this section */
    {

#define A00 0
#define A01 1
#define A02 2
#define A03 3
#define A04 4
#define A05 5
#define A06 6
#define A07 7
#define A08 8
#define A09 9

#define A10 0
#define A11 1
#define A12 2
#define A13 3
#define A14 4
#define A15 5
#define A16 6
#define A17 7
#define A18 8
#define A19 9

#define A20 0
#define A21 1
#define A22 2
#define A23 3
#define A24 4
#define A25 5
#define A26 6
#define A27 7
#define A28 8
#define A29 9

#define A30 0
#define A31 1
#define A32 2
#define A33 3
#define A34 4
#define A35 5
#define A36 6
#define A37 7
#define A38 8
#define A39 9

#define A40 0
#define A41 1
#define A42 2
#define A43 3
#define A44 4
#define A45 5
#define A46 6
#define A47 7
#define A48 8
#define A49 9

#define A50 0
#define A51 1
#define A52 2
#define A53 3
#define A54 4
#define A55 5
#define A56 6
#define A57 7
#define A58 8
#define A59 9

#define A60 0
#define A61 1
#define A62 2
#define A63 3
#define A64 4
#define A65 5
#define A66 6
#define A67 7
#define A68 8
#define A69 9

#define A70 0
#define A71 1
#define A72 2
#define A73 3
#define A74 4
#define A75 5
#define A76 6
#define A77 7
#define A78 8
#define A79 9

#define A80 0
#define A81 1
#define A82 2
#define A83 3
#define A84 4
#define A85 5
#define A86 6
#define A87 7
#define A88 8
#define A89 9

#define A90 0
#define A91 1
#define A92 2
#define A93 3
#define A94 4
#define A95 5
#define A96 6
#define A97 7
#define A98 8
#define A99 9

#define B00 0
#define B01 1
#define B02 2
#define B03 3
#define B04 4
#define B05 5
#define B06 6
#define B07 7
#define B08 8
#define B09 9

#define B10 0
#define B11 1
#define B12 2
#define B13 3
#define B14 4
#define B15 5
#define B16 6
#define B17 7
#define B18 8
#define B19 9

#define B20 0
#define B21 1
#define B22 2
#define B23 3
#define B24 4
#define B25 5
#define B26 6
#define B27 7
#define B28 8
#define B29 9

#define B30 0
#define B31 1
#define B32 2
#define B33 3
#define B34 4
#define B35 5
#define B36 6
#define B37 7
#define B38 8
#define B39 9

#define B40 0
#define B41 1
#define B42 2
#define B43 3
#define B44 4
#define B45 5
#define B46 6
#define B47 7
#define B48 8
#define B49 9

#define B50 0
#define B51 1
#define B52 2
#define B53 3
#define B54 4
#define B55 5
#define B56 6
#define B57 7
#define B58 8
#define B59 9

#define B60 0
#define B61 1
#define B62 2
#define B63 3
#define B64 4
#define B65 5
#define B66 6
#define B67 7
#define B68 8
#define B69 9

#define B70 0
#define B71 1
#define B72 2
#define B73 3
#define B74 4
#define B75 5
#define B76 6
#define B77 7
#define B78 8
#define B79 9

#define B80 0
#define B81 1
#define B82 2
#define B83 3
#define B84 4
#define B85 5
#define B86 6
#define B87 7
#define B88 8
#define B89 9

#define B90 0
#define B91 1
#define B92 2
#define B93 3
#define B94 4
#define B95 5
#define B96 6
#define B97 7
#define B98 8
#define B99 9

#define C00 0
#define C01 1
#define C02 2
#define C03 3
#define C04 4
#define C05 5
#define C06 6
#define C07 7
#define C08 8
#define C09 9

#define C10 0
#define C11 1
#define C12 2
#define C13 3
#define C14 4
#define C15 5
#define C16 6
#define C17 7
#define C18 8
#define C19 9

#define C20 0
#define C21 1
#define C22 2
#define C23 3
#define C24 4
#define C25 5
#define C26 6
#define C27 7
#define C28 8
#define C29 9

#define C30 0
#define C31 1
#define C32 2
#define C33 3
#define C34 4
#define C35 5
#define C36 6
#define C37 7
#define C38 8
#define C39 9

#define C40 0
#define C41 1
#define C42 2
#define C43 3
#define C44 4
#define C45 5
#define C46 6
#define C47 7
#define C48 8
#define C49 9

#define C50 0
#define C51 1
#define C52 2
#define C53 3
#define C54 4
#define C55 5
#define C56 6
#define C57 7
#define C58 8
#define C59 9

#define C60 0
#define C61 1
#define C62 2
#define C63 3
#define C64 4
#define C65 5
#define C66 6
#define C67 7
#define C68 8
#define C69 9

#define C70 0
#define C71 1
#define C72 2
#define C73 3
#define C74 4
#define C75 5
#define C76 6
#define C77 7
#define C78 8
#define C79 9

#define C80 0
#define C81 1
#define C82 2
#define C83 3
#define C84 4
#define C85 5
#define C86 6
#define C87 7
#define C88 8
#define C89 9

#define C90 0
#define C91 1
#define C92 2
#define C93 3
#define C94 4
#define C95 5
#define C96 6
#define C97 7
#define C98 8
#define C99 9

#define D00 0
#define D01 1
#define D02 2
#define D03 3
#define D04 4
#define D05 5
#define D06 6
#define D07 7
#define D08 8
#define D09 9

#define D10 0
#define D11 1
#define D12 2
#define D13 3
#define D14 4
#define D15 5
#define D16 6
#define D17 7
#define D18 8
#define D19 9

#define D20 0
#define D21 1
#define D22 2
#define D23 3
#define D24 4
#define D25 5
#define D26 6
#define D27 7
#define D28 8
#define D29 9

#define D30 0
#define D31 1
#define D32 2
#define D33 3
#define D34 4
#define D35 5
#define D36 6
#define D37 7
#define D38 8
#define D39 9

#define D40 0
#define D41 1
#define D42 2
#define D43 3
#define D44 4
#define D45 5
#define D46 6
#define D47 7
#define D48 8
#define D49 9

#define D50 0
#define D51 1
#define D52 2
#define D53 3
#define D54 4
#define D55 5
#define D56 6
#define D57 7
#define D58 8
#define D59 9

#define D60 0
#define D61 1
#define D62 2
#define D63 3
#define D64 4
#define D65 5
#define D66 6
#define D67 7
#define D68 8
#define D69 9

#define D70 0
#define D71 1
#define D72 2
#define D73 3
#define D74 4
#define D75 5
#define D76 6
#define D77 7
#define D78 8
#define D79 9

#define D80 0
#define D81 1
#define D82 2
#define D83 3
#define D84 4
#define D85 5
#define D86 6
#define D87 7
#define D88 8
#define D89 9

#define D90 0
#define D91 1
#define D92 2
#define D93 3
#define D94 4
#define D95 5
#define D96 6
#define D97 7
#define D98 8
#define D99 9

#define E00 0
#define E01 1
#define E02 2
#define E03 3
#define E04 4
#define E05 5
#define E06 6
#define E07 7
#define E08 8
#define E09 9

#define E10 0
#define E11 1
#define E12 2
#define E13 3
#define E14 4
#define E15 5
#define E16 6
#define E17 7
#define E18 8
#define E19 9

#define E20 0
#define E21 1
#define E22 2
#define E23 3
#define E24 4
#define E25 5
#define E26 6
#define E27 7
#define E28 8
#define E29 9

#define E30 0
#define E31 1
#define E32 2
#define E33 3
#define E34 4
#define E35 5
#define E36 6
#define E37 7
#define E38 8
#define E39 9

#define E40 0
#define E41 1
#define E42 2
#define E43 3
#define E44 4
#define E45 5
#define E46 6
#define E47 7
#define E48 8
#define E49 9

#define E50 0
#define E51 1
#define E52 2
#define E53 3
#define E54 4
#define E55 5
#define E56 6
#define E57 7
#define E58 8
#define E59 9

#define E60 0
#define E61 1
#define E62 2
#define E63 3
#define E64 4
#define E65 5
#define E66 6
#define E67 7
#define E68 8
#define E69 9

#define E70 0
#define E71 1
#define E72 2
#define E73 3
#define E74 4
#define E75 5
#define E76 6
#define E77 7
#define E78 8
#define E79 9

#define E80 0
#define E81 1
#define E82 2
#define E83 3
#define E84 4
#define E85 5
#define E86 6
#define E87 7
#define E88 8
#define E89 9

#define E90 0
#define E91 1
#define E92 2
#define E93 3
#define E94 4
#define E95 5
#define E96 6
#define E97 7
#define E98 8
#define E99 9

#define F00 0
#define F01 1
#define F02 2
#define F03 3
#define F04 4
#define F05 5
#define F06 6
#define F07 7
#define F08 8
#define F09 9

#define F10 0
#define F11 1
#define F12 2
#define F13 3
#define F14 4
#define F15 5
#define F16 6
#define F17 7
#define F18 8
#define F19 9

#define F20 0
#define F21 1
#define F22 2
#define F23 3
#define F24 4
#define F25 5
#define F26 6
#define F27 7
#define F28 8
#define F29 9

#define F30 0
#define F31 1
#define F32 2
#define F33 3
#define F34 4
#define F35 5
#define F36 6
#define F37 7
#define F38 8
#define F39 9

#define F40 0
#define F41 1
#define F42 2
#define F43 3
#define F44 4
#define F45 5
#define F46 6
#define F47 7
#define F48 8
#define F49 9

#define F50 0
#define F51 1
#define F52 2
#define F53 3
#define F54 4
#define F55 5
#define F56 6
#define F57 7
#define F58 8
#define F59 9

#define F60 0
#define F61 1
#define F62 2
#define F63 3
#define F64 4
#define F65 5
#define F66 6
#define F67 7
#define F68 8
#define F69 9

#define F70 0
#define F71 1
#define F72 2
#define F73 3
#define F74 4
#define F75 5
#define F76 6
#define F77 7
#define F78 8
#define F79 9

#define F80 0
#define F81 1
#define F82 2
#define F83 3
#define F84 4
#define F85 5
#define F86 6
#define F87 7
#define F88 8
#define F89 9

#define F90 0
#define F91 1
#define F92 2
#define F93 3
#define F94 4
#define F95 5
#define F96 6
#define F97 7
#define F98 8
#define F99 9

#define G00 0
#define G01 1
#define G02 2
#define G03 3
#define G04 4
#define G05 5
#define G06 6
#define G07 7
#define G08 8
#define G09 9

#define G10 0
#define G11 1
#define G12 2
#define G13 3
#define G14 4
#define G15 5
#define G16 6
#define G17 7
#define G18 8
#define G19 9

#define G20 0
#define G21 1
#define G22 2
#define G23 3
#define G24 4
#define G25 5
#define G26 6
#define G27 7
#define G28 8
#define G29 9

#define G30 0
#define G31 1
#define G32 2
#define G33 3
#define G34 4
#define G35 5
#define G36 6
#define G37 7
#define G38 8
#define G39 9

#define G40 0
#define G41 1
#define G42 2
#define G43 3
#define G44 4
#define G45 5
#define G46 6
#define G47 7
#define G48 8
#define G49 9

#define G50 0
#define G51 1
#define G52 2
#define G53 3
#define G54 4
#define G55 5
#define G56 6
#define G57 7
#define G58 8
#define G59 9

#define G60 0
#define G61 1
#define G62 2
#define G63 3
#define G64 4
#define G65 5
#define G66 6
#define G67 7
#define G68 8
#define G69 9

#define G70 0
#define G71 1
#define G72 2
#define G73 3
#define G74 4
#define G75 5
#define G76 6
#define G77 7
#define G78 8
#define G79 9

#define G80 0
#define G81 1
#define G82 2
#define G83 3
#define G84 4
#define G85 5
#define G86 6
#define G87 7
#define G88 8
#define G89 9

#define G90 0
#define G91 1
#define G92 2
#define G93 3
#define G94 4
#define G95 5
#define G96 6
#define G97 7
#define G98 8
#define G99 9

#define H00 0
#define H01 1
#define H02 2
#define H03 3
#define H04 4
#define H05 5
#define H06 6
#define H07 7
#define H08 8
#define H09 9

#define H10 0
#define H11 1
#define H12 2
#define H13 3
#define H14 4
#define H15 5
#define H16 6
#define H17 7
#define H18 8
#define H19 9

#define H20 0
#define H21 1
#define H22 2
#define H23 3
#define H24 4
#define H25 5
#define H26 6
#define H27 7
#define H28 8
#define H29 9

#define H30 0
#define H31 1
#define H32 2
#define H33 3
#define H34 4
#define H35 5
#define H36 6
#define H37 7
#define H38 8
#define H39 9

#define H40 0
#define H41 1
#define H42 2
#define H43 3
#define H44 4
#define H45 5
#define H46 6
#define H47 7
#define H48 8
#define H49 9

#define H50 0
#define H51 1
#define H52 2
#define H53 3
#define H54 4
#define H55 5
#define H56 6
#define H57 7
#define H58 8
#define H59 9

#define H60 0
#define H61 1
#define H62 2
#define H63 3
#define H64 4
#define H65 5
#define H66 6
#define H67 7
#define H68 8
#define H69 9

#define H70 0
#define H71 1
#define H72 2
#define H73 3
#define H74 4
#define H75 5
#define H76 6
#define H77 7
#define H78 8
#define H79 9

#define H80 0
#define H81 1
#define H82 2
#define H83 3
#define H84 4
#define H85 5
#define H86 6
#define H87 7
#define H88 8
#define H89 9

#define H90 0
#define H91 1
#define H92 2
#define H93 3
#define H94 4
#define H95 5
#define H96 6
#define H97 7
#define H98 8
#define H99 9

#define I00 0
#define I01 1
#define I02 2
#define I03 3
#define I04 4
#define I05 5
#define I06 6
#define I07 7
#define I08 8
#define I09 9

#define I10 0
#define I11 1
#define I12 2
#define I13 3
#define I14 4
#define I15 5
#define I16 6
#define I17 7
#define I18 8
#define I19 9

#define I20 0
#define I21 1
#define I22 2
#define I23 3
#define I24 4
#define I25 5
#define I26 6
#define I27 7
#define I28 8
#define I29 9

#define I30 0
#define I31 1
#define I32 2
#define I33 3
#define I34 4
#define I35 5
#define I36 6
#define I37 7
#define I38 8
#define I39 9

#define I40 0
#define I41 1
#define I42 2
#define I43 3
#define I44 4
#define I45 5
#define I46 6
#define I47 7
#define I48 8
#define I49 9

#define I50 0
#define I51 1
#define I52 2
#define I53 3
#define I54 4
#define I55 5
#define I56 6
#define I57 7
#define I58 8
#define I59 9

#define I60 0
#define I61 1
#define I62 2
#define I63 3
#define I64 4
#define I65 5
#define I66 6
#define I67 7
#define I68 8
#define I69 9

#define I70 0
#define I71 1
#define I72 2
#define I73 3
#define I74 4
#define I75 5
#define I76 6
#define I77 7
#define I78 8
#define I79 9

#define I80 0
#define I81 1
#define I82 2
#define I83 3
#define I84 4
#define I85 5
#define I86 6
#define I87 7
#define I88 8
#define I89 9

#define I90 0
#define I91 1
#define I92 2
#define I93 3
#define I94 4
#define I95 5
#define I96 6
#define I97 7
#define I98 8
#define I99 9

#define J00 0
#define J01 1
#define J02 2
#define J03 3
#define J04 4
#define J05 5
#define J06 6
#define J07 7
#define J08 8
#define J09 9

#define J10 0
#define J11 1
#define J12 2
#define J13 3
#define J14 4
#define J15 5
#define J16 6
#define J17 7
#define J18 8
#define J19 9

#define J20 0
#define J21 1
#define J22 2
#define J23 3
#define J24 4
#define J25 5
#define J26 6
#define J27 7
#define J28 8
#define J29 9

#define J30 0
#define J31 1
#define J32 2
#define J33 3
#define J34 4
#define J35 5
#define J36 6
#define J37 7
#define J38 8
#define J39 9

#define J40 0
#define J41 1
#define J42 2
#define J43 3
#define J44 4
#define J45 5
#define J46 6
#define J47 7
#define J48 8
#define J49 9

#define J50 0
#define J51 1
#define J52 2
#define J53 3
#define J54 4
#define J55 5
#define J56 6
#define J57 7
#define J58 8
#define J59 9

#define J60 0
#define J61 1
#define J62 2
#define J63 3
#define J64 4
#define J65 5
#define J66 6
#define J67 7
#define J68 8
#define J69 9

#define J70 0
#define J71 1
#define J72 2
#define J73 3
#define J74 4
#define J75 5
#define J76 6
#define J77 7
#define J78 8
#define J79 9

#define J80 0
#define J81 1
#define J82 2
#define J83 3
#define J84 4
#define J85 5
#define J86 6
#define J87 7
#define J88 8
#define J89 9

#define J90 0
#define J91 1
#define J92 2
#define J93 3
#define J94 4
#define J95 5
#define J96 6
#define J97 7
#define J98 8
#define J99 9

#define K00 0
#define K01 1
#define K02 2
#define K03 3
#define K04 4
#define K05 5
#define K06 6
#define K07 7
#define K08 8
#define K09 9
#define K10 0
#define K11 1
#define K12 2
#define K13 3

    verify(__LINE__, A00 == 0 && K13 == 3);
    }

/* #11: 31 parameters in one function definition and call */
    {
    char buf[40];
    
    cat31(buf,  /* target + dummy + 29 args */
      0,
      '1','2','3','4','5','6','7','8','9','0',
      '1','2','3','4','5','6','7','8','9','0',
      '1','2','3','4','5','6','7','8','9');
    verify(__LINE__, 0 == str_cmp(buf, "12345678901234567890123456789"));
    }

/* #12: 31 parameters in one macro definition and invocation */
    {
#define MUL31(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E) \
              a*b*c*d*e*f*g*h*i*j*k*l*m*n*o*p*q*r*s*t*u*v*w*x*y*z*A*B*C*D*E

    verify(__LINE__, 0x40000000L ==
    MUL31(1L,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2));

    }

/* #13: 509 characters in a logical source line */
/* #14: 509 characters in a string literal (after concatenation) */
{
static char buf[510] =
"\
1234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890\
12345678901234567"
"89";
/* 2 quotes + 7x70 digits + 17 digits = 509 */
/* 507 catenated with 2 = 509 */

verify(__LINE__, str_len(buf) == 509 && buf[str_len(buf)-1] == '9');
} /* end #13 and #14 */
#ifndef FREESTANDING
/* #15: 32767 bytes in an object (hosted environment only) */
    {
    static unsigned char buf[32767] = {0};
    long i;
    
    for (i = 0; i < 32767; ++i)
    	buf[i] = i % 251;
    for (i = 0; i < 32767; ++i)
    	verify(__LINE__, buf[i] == i % 251);
    }
#endif	/* !FREESTANDING */
/* #16: 8 nesting levels for #included files */
/* b22411.h includes b22412.h, etc. to b22418.h:
 *    b22411.h:
 *    	#include "b22412.h"
 *    b22412.h:
 *    	#include "b22413.h"
 *    b22413.h:
 *    	#include "b22414.h"
 *    b22414.h:
 *    	#include "b22415.h"
 *    b22415.h:
 *    	#include "b22416.h"
 *    b22416.h:
 *    	#include "b22417.h"
 *    b22417.h:
 *    	#include "b22418.h"
 *    b22418.h:
 *    	#define b22418_INCLUDED 12345678
 */
#include "b22411.h"
    {
    verify(__LINE__, b22418_INCLUDED == 12345678);
    }

/* #17: 257 case labels in a switch statement (ANSI8703 changed from 255) */
    {
    short i = 101;
    
    switch (i) {
    case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:
        break;
    case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:
	case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:
	case 130:case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:
	case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:
        verify(__LINE__, 0);  /* fail, if reached here */
    case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:
	case 160:case 161:case 162:case 163:case 164:case 165:case 166:case 167:case 168:case 169:
	case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:
	case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:
	case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:
	   /* 100 cases */
        verify(__LINE__, 0);
    case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:case 209:
	case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:
	case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:
	case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:
	case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:
    case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:
	case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:
	case 270:case 271:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:
	case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:
	case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:
   /* 200 cases */
        verify(__LINE__, 0);
    case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:
	case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:
	case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:
	case 330:case 331:case 332:case 333:case 334:case 335:case 336:case 337:case 338:case 339:
	case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:
   /* 250 cases */
    case 350: case 351: case 352: case 353: case 354: case 355: case 356:
		/* 257 cases */
        verify(__LINE__, 0);
    } /* end of switch */
    } /* end of #17 */

/* #18: 127 members in a single structure or union */
	{
	struct s127 {
		int i00, i01, i02, i03, i04, i05, i06, i07, i08, i09;
		int i10, i11, i12, i13, i14, i15, i16, i17, i18, i19;
		int i20, i21, i22, i23, i24, i25, i26, i27, i28, i29;
		int i30, i31, i32, i33, i34, i35, i36, i37, i38, i39;
		int i40, i41, i42, i43, i44, i45, i46, i47, i48, i49;
		int i50, i51, i52, i53, i54, i55, i56, i57, i58, i59;
		int i60, i61, i62, i63, i64, i65, i66, i67, i68, i69;
		int i70, i71, i72, i73, i74, i75, i76, i77, i78, i79;
		int i80, i81, i82, i83, i84, i85, i86, i87, i88, i89;
		int i90, i91, i92, i93, i94, i95, i96, i97, i98, i99;
		int i100, i101, i102, i103, i104, i105, i106, i107, i108, i109;
		int i110, i111, i112, i113, i114, i115, i116, i117, i118, i119;
		int i120, i121, i122, i123, i124, i125, i126;
		} s127 = {0, 1, 2, 0};
	verify(__LINE__, s127.i01 == 1);
	verify(__LINE__, s127.i126 == 0);
	}


/* #19: 127 enumeration constants in a single enumeration */
	{
	enum e127 {
		EN00, EN01, EN02, EN03, EN04, EN05, EN06, EN07, EN08, EN09,
		EN10, EN11, EN12, EN13, EN14, EN15, EN16, EN17, EN18, EN19,
		EN20, EN21, EN22, EN23, EN24, EN25, EN26, EN27, EN28, EN29,
		EN30, EN31, EN32, EN33, EN34, EN35, EN36, EN37, EN38, EN39,
		EN40, EN41, EN42, EN43, EN44, EN45, EN46, EN47, EN48, EN49,
		EN50, EN51, EN52, EN53, EN54, EN55, EN56, EN57, EN58, EN59,
		EN60, EN61, EN62, EN63, EN64, EN65, EN66, EN67, EN68, EN69,
		EN70, EN71, EN72, EN73, EN74, EN75, EN76, EN77, EN78, EN79,
		EN80, EN81, EN82, EN83, EN84, EN85, EN86, EN87, EN88, EN89,
		EN90, EN91, EN92, EN93, EN94, EN95, EN96, EN97, EN98, EN99,
		EN100, EN101, EN102, EN103, EN104, EN105, EN106, EN107, EN108, EN109,
		EN110, EN111, EN112, EN113, EN114, EN115, EN116, EN117, EN118, EN119,
		EN120, EN121, EN122, EN123, EN124, EN125, EN126
		} e127 = {EN126};
	verify(__LINE__, e127 == 126);
	verify(__LINE__, EN125 == 125);
	}

/* #20: 15 levels of nested structure or union definitions in a single struct-declaraction list */
	{
	struct { /* 1 */
	 struct { /* 2 */
	  struct { /* 3 */
	   struct { /* 4 */
	    struct { /* 5 */
	     struct { /* 6 */
	      struct { /* 7 */
	       struct { /* 8 */
	        struct { /* 9 */
	         struct { /* 10 */
	          struct { /* 11 */
	           struct { /* 12 */
	            struct { /* 13 */
	             struct { /* 14 */
	              struct { /* 15 */
					int i;
			              } s15;
			             } s14;
			            } s13;
			           } s12;
			          } s11;
			         } s10;
			        } s9;
			       } s8;
			      } s7;
			     } s6;
			    } s5;
			   } s4;
			  } s3;
			 } s2;
			} s1 = { 99 };

	verify(__LINE__, s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.i == 99);
	}

	if (errs == 0)
		{}
	else
		{
		printf("***** 0 successful tests in CAPACITY *****\n", 0);
		printf("***** %d errors detected in CAPACITY *****\n", errs);
		printf("***** 0 skipped sections in CAPACITY *****\n", 0);
		}

	return 0;
} /* end of main */
