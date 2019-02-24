typedef int (*fn)(struct x *);
extern int fn1(struct x *);
extern fn *fp = fn1;
