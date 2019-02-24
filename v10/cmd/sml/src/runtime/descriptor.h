/* needs tags.h */

#define is_ptr(x) (!((int)(x)&1))
#define mak_int(x) ((int)(x)*2+1)
#define mask_tags (power_tags-1)
#define get_len(x) (*(int *)(x)>>width_tags)
#define get_lenz(x) ((((*(int*)(x))&(power_tags-1))==tag_suspension)?1:get_len(x))
#define get_strlen(x) (((*(int *)(x)>>width_tags)+3)/4)
#define get_tag(x) (*(int *)(x)&mask_tags)

typedef char * mlstring;

extern char *get_str();
