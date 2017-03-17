#ifndef __TYPE_H__
#define __TYPE_H__
#ifdef __WATCOMC__
/*
 * open watcom definitions for wolf3d
 * by sparky4
 */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>	//for argc and argv
#include <graph.h>
#include <malloc.h>

#define VERSION __DATE__ " " __TIME__
#define _FCORELEFT 0x90000UL+16UL

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

inline unsigned long farcoreleft()
{
	_fheapgrow();
	return _FCORELEFT;
}
inline unsigned long coreleft()
{
	_nheapgrow();
	return _memavl();//(dword)GetFreeSize();
}


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
#define _seg __based( void )
typedef long fixed;

#define _argv __argv
#define _argc __argc
#endif

#define MEMPTRCONV			(memptr *)&

#define	__ID_PM__

#define BAKAPEEQUIT 	getch(); VL_SetTextMode (); printf("\n_fmemcpy() and VW_Startup breaks. I do not know how to fix this.\n"); MM_Shutdown (); exit(1);
#endif

/*
./id_ca.c:	if ((handle = open(GREXT"HEAD."EXTENSION,
./id_ca.c:		Quit ("Can't open "GREXT"HEAD."EXTENSION"!");
./id_ca.c://	grhandle = open(GREXT"GRAPH."EXTENSION, O_RDONLY | O_BINARY); NOLAN
./id_ca.c://	if ((handle = open("MAPHEAD."EXTENSION,
./id_ca.c:	if ((maphandle = open("MAPTEMP."EXTENSION,
./id_ca.c:		Quit ("Can't open MAPTEMP."EXTENSION"!");
./id_ca.c:	if ((handle = open("AUDIOHED."EXTENSION,
./id_ca.c:		Quit ("Can't open AUDIOHED."EXTENSION"!");
./id_ca.c:	if ((audiohandle = open("AUDIOT."EXTENSION,
./id_ca.c:		Quit ("Can't open AUDIOT."EXTENSION"!");
./id_ca.c://	if ((audiohandle = open("AUDIO."EXTENSION,	NOLAN

./id_us.c:	for (s = "."EXTENSION;*s;)
./id_us.c:			&&	(!strcmp(game->signature,EXTENSION))
./id_us.c:			strcpy(game->signature,EXTENSION);
./id_us.c:			name = "GAMETEXT."EXTENSION;
./id_us.c:			name = "CONTEXT."EXTENSION;
./id_us.c:			name = "STORY."EXTENSION;
*/
