#define nop 		0x00000000
#define ea_base(_wd) 	((_wd >> 21) & 0x0000001f)
#define ea_offset(_wd)	(((short)_wd) & 0x0000ffff)
#define ea_addr(_offset,_base)	(((_base << 21) & 0x03e00000) | ea_offset(_offset))

#ifdef BINARY
typedef struct segment{
	char 	name[6]; 
	unsigned baseaddr;
	int	cursize;
	int	totsize; 
	MTable	*data;
}Segment;	
extern Segment segments[];
extern int segmentmap[];

proto(unsigned b_addrcon,(int, int));
proto(void b_address,(Symbol, Symbol, int, int, int));
proto(void b_align,(int));
proto(unsigned b_arg,(Node, unsigned int));
proto(unsigned b_arith2,(int, int, int, int, int));
proto(unsigned b_asgnb,(Node, unsigned int, unsigned int));
proto(void b_blkfetch,(int, int, int, int, int));
proto(void b_blkloop,(char *, int, char *, int, int, char *[]));
proto(void b_blkstore,(int, int, int, int, int));
proto(unsigned b_branch1,(int, int, int, int, Symbol));
proto(unsigned b_branch2,(int, int, int, Symbol));
proto(void b_byte,(unsigned));
proto(void b_char,(char *, char *, unsigned));
proto(unsigned b_cop,(int, int, int, int, int));
proto(unsigned b_copmv,(int, int, int, int));
proto(unsigned b_cvid,(int, int)) ;
proto(unsigned b_cvt,(int, int, int));
proto(unsigned b_def,(Symbol));
proto(void b_double,(unsigned, unsigned));
proto(unsigned b_eaddr,(int, int));
proto(void b_eol,(void));
proto(unsigned b_end,(Symbol));
proto(unsigned b_entry,(Symbol));
proto(unsigned b_fbranch,(int, Symbol, int, int, int, int));
proto(unsigned b_frame,(int, int));
proto(void b_half,(int));
proto(unsigned b_immed,(int, int, int, int));
proto(unsigned b_jump,(int, unsigned));
proto(unsigned b_la,(int, unsigned));
proto(unsigned b_ldst,(int, int, unsigned, int));
proto(unsigned b_move,(int, int));
proto(void b_seginit,(int []));
proto(void b_segment,(int));
proto(unsigned b_shift,(int, int, int, int));
proto(void b_space,(int, char *));
proto(unsigned b_special,(int, int, int, int));
proto(unsigned b_stackvar,(Symbol));
proto(void b_string,(char *, char *, int));
proto(unsigned b_sub,(int, int, int)) ;
proto(void b_symbol,(int, Symbol));
proto(unsigned b_trunc,(int, int, int, int));
proto(unsigned b_use,(Symbol));
proto(void b_word,(int));
#endif
