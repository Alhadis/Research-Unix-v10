struct  INODE   {
        int     MODE;
        char    NLINKS;
        char    UID;
        char    GID;
        char    FILL;
        long    SIZE;
        struct  {
                unsigned        STBLK;
                unsigned        NCBLKS;
        }       EXTENTS[27];
        int     ACTIME[2];
        int     MODTIME[2];
        int     CHKSUM;
};

struct  DIRECT  {
        int     INUM;
        char    NAME[14];
};


int     dev;
long    offset = 1802240l;


char    fname[1000];
main(argc,argv)
int     argc;
char    **argv;
{
        int     inum;
        char    *s;

        if(argc != 4)   {
                printf("fcopy: device inode directory\n");
                return(1);
        }
        if((dev = open(argv[1],0)) < 0) {
                printf("can't open device %s\n",argv[1]);
                return(1);
        }
        for(s = fname ; *argv[3] ; *s++ = *argv[3]++);
        *s = 0;
        inum = atoi(argv[2]);
        copy(inum);
}


copy(inum)
{
        struct  INODE   ino;
        struct  DIRECT  entry;
        static  char    block[512];

        if(lseek(dev, ((long)(inum+7)*128)+offset, 0) < 0 || read(dev, &ino, 128) != 128)       {
                message("Cant read inode",inum,fname);
                return;
        }
        if(!(ino.MODE & 0100000))       {
                message("unallocated",inum,fname);
                return;
        }
        switch(ino.MODE & 070000)       {
            case 020000:
                message("character special file",inum,fname);
                return;
            case 060000:
                message("block special file",inum,fname);
                return;
            case 070000:
                message("record special file",inum,fname);
                return;
            case 000000:
            case 010000:
            {
                /* copy the regular file to fname */
                int     e,i;
                int     ofile;
/*
message("copying file",inum,fname);
*/
                if((ofile = creat(fname, 0777)) < 0)    {
                        message("cant creat file",inum,fname);
                        return;
                }
                if(ino.NLINKS != 1)     {
                        message("dropping %d links",inum,fname,ino.NLINKS-1);
                }
                for(e = 0 ; ino.SIZE > 0 && e < 27 ; )  {
                        if(ino.EXTENTS[e].NCBLKS-- > 0) {
                                if(lseek(dev, (long)(ino.EXTENTS[e].STBLK++)*512 + offset, 0) < 0)      {
                                        message("Cant seek",inum,fname);
                                        return;
                                }
                                if(read(dev, block, 512) != 512)        {
                                        message("cant read",inum,fname);
                                        return;
                                }
                                i = (ino.SIZE >= 512) ? 512 : ino.SIZE;
                                if(write(ofile, block, i) != i) {
                                        message("bad size",inum,fname);
                                        close(ofile);
                                        return;
                                }
                                ino.SIZE -= i;
                        }
                        else
                                e++;
                }
                close(ofile);
                break;
            }
            case 040000:
            case 050000:
            {
                /* copy each directory entry */
                int     e,i,j;
                long    b;
                int     flag;
                int     status;
/*
        message("directory",inum,fname);
*/
                if(fork() == 0) {
                        execl("/bin/mkdir","mkdir",fname,0);
                        exit(1);
                }
                wait(&status);
                if(status)      {
                        message("can't creat directory",inum,fname);
                }
                flag = 1;
                for(e = 0 ; ino.SIZE > 0 && e < 27 ; )  {
                        if(ino.EXTENTS[e].NCBLKS-- > 0) {
                                i = (ino.SIZE >= 512) ? 512 : ino.SIZE;
                                ino.SIZE -= i;
                                b = (long)(ino.EXTENTS[e].STBLK++);
                                for(j = (flag?32:0) ; j < i ; j += 16)  {
                                        if(lseek(dev, (b*512)+j+offset, 0) < 0) {
                                                message("cant seek",inum,fname);
                                                return;
                                        }
                                        read(dev, &entry, 16);
                                        if(entry.INUM != 0 && entry.NAME[0])    {
                                                addname(entry.NAME);
                                                copy(entry.INUM);
                                                subname();
                                        }
                                }
                                flag = 0;
                        }
                        else
                                e++;
                }
                break;
            }
            default:
                message("funny file type (%o)",inum,fname,ino.MODE);
                return;
        }
        /* change the mode and owner of fname */
        if(ino.MODE & 07000)    {
                message("funny file mode (%o)",inum,fname,ino.MODE);
        }
        chmod(fname, ino.MODE & 0777);
        chown(fname, ino.UID & 0377, ino.GID & 0377);
}


addname(s)
char    *s;
{
        char    *f;
        int     i;

        f = fname;
        while(*f++);
        f--;
        *f++ = '/';
        for(i = 14 ; *s && i-- ; *f++ = *s++);
        *f = 0;
}


subname()
{
        char    *f;

        f = fname;
        while(*f++);
        while(*--f != '/')
                *f = 0;
        *f = 0;
}


message(s,i,f,x)
char    *s,*f;
{

        printf("[%s] (%d) : ",f,i);
        printf(s,x);
        printf("\n");
}
