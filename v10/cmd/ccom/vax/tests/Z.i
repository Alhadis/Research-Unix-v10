struct text {
    int fd; 
    char file;
    short cnt,extra;
    unsigned char *pnt;
    unsigned char buf[1];
};
int blankline(tex)
struct text *tex;
{
    int c, i, n, found;
    n = found + ((i)?*tex->pnt++:get());
    for (;;) {
       c= (tex->cnt>0?tex->pnt[0]:peek(0,tex));
       if (c == ' ' || c == '\n' || c == '\t')  ((0<tex->cnt--)?*tex->pnt++:get(tex));
       else return(1);
       }
}
