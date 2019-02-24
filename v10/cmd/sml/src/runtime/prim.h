#ifndef MIPS
#define _div_e (_div_e0+4)
#define _real_e (_real_e0+4)
#define _interrupt_e (_interrupt_e0+4)
#define _overflow_e (_overflow_e0+4)
#define _systemcall_e (_systemcall_e0+4)
#else
#define div_e div_e0+4
#define real_e real_e0+4
#define interrupt_e interrupt_e0+4
#define overflow_e overflow_e0+4
#define systemcall_e systemcall_e0+4
#endif

#ifdef BSD
#define FIONREAD 0x4004667f
#define TIOCGETP 0x40067408
#endif
#ifdef V9
#define FIONREAD 0x667f
#define TIOCGETP 0x7408
#endif
