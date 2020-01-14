#ifndef __TYPE_H__
#define __TYPE_H__

//bcc
#ifdef __BORLANDC__
#define MEMPTRANDPERCONV	&
//#define BYTEFARPTRCONV		(unsigned char far *)
//#define SMDPTRANDPERCONV	&
//#define SDFPTRANDPERCONV	&
//#define OBTPTRANDPERCONV	&
//#define SDTPTRANDPERCONV	&

#define _nmalloc malloc
#define _nfree free

#define _nheapwalk heapwalk
#define _fheapwalk farheapwalk

//heapinfo vars
#define _heapinfo heapinfo
#define _pentry	ptr
#define _size	size
#define _useflag	in_use
#endif
//

#ifdef __WATCOMC__
/*
 * open watcom definitions for keen dreams
 * by sparky4
 */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>	//for argc and argv
#include <graph.h>
#include <malloc.h>
#include <i86.h>

#define VERSION __DATE__ " " __TIME__

static union REGS CPURegs;
static struct SREGS	CPUSTKRegs;

#define _AX CPURegs.x.ax
#define _BX CPURegs.x.bx
#define _CX CPURegs.x.cx
#define _DX CPURegs.x.dx

#define _SI CPURegs.x.si
#define _DI CPURegs.x.di

#define _AH CPURegs.h.ah
#define _AL CPURegs.h.al
#define _BH CPURegs.h.bh
#define _BL CPURegs.h.bl
#define _CH CPURegs.h.ch
#define _CL CPURegs.h.cl
#define _DH CPURegs.h.dh
#define _DL CPURegs.h.dl

#define _ES	CPUSTKRegs.es
#define _CS	CPUSTKRegs.cs
#define _SS	CPUSTKRegs.ss
#define _DS	CPUSTKRegs.ds

#define geninterrupt(n) int86(n,&CPURegs,&CPURegs);

#define peekb(segm,ofs) (*(byte far*)MK_FP((segm),(ofs)))
#define peek(segm,ofs) (*(word far*)MK_FP((segm),(ofs)))
#define pokeb(segm,ofs,value) (peekb((segm),(ofs)) = (byte)(value))
#define poke(segm,ofs,value) (peek((segm),(ofs)) = (word)(value))

#define inportb(port)         inp(port)
#define inport(port)         inpw(port)
#define outportb(port, val)   outp(port, val)
#define outport(port, val)   outpw(port, val)

#define asm __asm
#define getvect _dos_getvect
#define setvect _dos_setvect

#define farmalloc _fmalloc
#define farfree _ffree
#define nearmalloc _nmalloc
#define nearfree _nfree

/*inline unsigned long farcoreleft()
{
//	_fheapgrow();
	return 0x90000UL+16UL;
}
inline unsigned long coreleft()
{
	_nheapgrow();
	return _memavl();//(dword)GetFreeSize();
}*/


//from http://www.verycomputer.com/3_65d875cc818b54ec_1.htm
void clrscr(void);
#pragma aux clrscr= \
  " mov ah, 0fh " \
  " int 10h " \
  " xor ah, ah " \
  " int 10h " ;
//from http://stackoverflow.com/questions/18425748/gotoxy-function-using-printf-s-position
inline void gotoxy(int x,int y)
{
	printf("\x1B[%d;%df", y, x);
	fflush(stdout);  // @jxh
}

#define ffblk        find_t
#define ff_name      name
#define ff_attrib    attrib
#define ff_fsize     size
#define ff_ftime     wr_time
#define ff_fdate     wr_date

#define findfirst(name, dta, attrib)   _dos_findfirst(name, attrib, dta)
#define findnext(dta)                  _dos_findnext(dta)

#define MAXLONG LONG_MAX
#define EINVFMT EMFILE
#define FA_ARCH	0x20
#define PI	3.141592657
#define M_PI PI

typedef union REGPACK	regs_t;

/*typedef	enum	{false,true}	boolean;
typedef	unsigned	char		byte;
typedef	unsigned	int		word;
typedef	signed	short		sword;
typedef	unsigned	long		dword; typedef dword longword;
typedef	byte *					Ptr;

typedef	struct
		{
			int	x,y;
		} Point;
typedef	struct
		{
			Point	ul,lr;
		} Rect;*/

//typedef void __based( void ) * memptr;	////old //----typedef void __based(__self) * memptr;
//#define _seg __based( void )
#define _seg __based(__self)
typedef long fixed;

#define _argv __argv
#define _argc __argc
#endif

#define MEMPTRCONV			(memptr *)&

//#define	__ID_PM__

#define BAKAPEEQUIT 	getch(); VL_SetTextMode (); printf("\n_fmemcpy() and VW_Startup breaks. I do not know how to fix this.\n"); MM_Shutdown (); exit(1);
#endif
