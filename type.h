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
src/contigsc.c:byte _seg	*endscalermemory;
_BSEG
src/contigsc.c:	endscalermemory = (void _seg *)(seg+ofs/16);
_VSEG
src/contigsc.c:	t_compshape	_seg *shape;
src/contigsc.c:	t_compshape	_seg *shape;
_C_TSSEG
src/id_ca.c:byte 		_seg	*tinf;
src/id_ca.c:unsigned	_seg	*mapsegs[MAPPLANES];
_USEG
src/id_ca.c:maptype		_seg	*mapheaderseg[NUMMAPS];
_M_TSEG
src/id_ca.c:byte		_seg	*audiosegs[NUMSNDCHUNKS];
src/id_ca.c:void		_seg	*grsegs[NUMCHUNKS];
src/id_ca.c:long		_seg *grstarts;	// array of offsets in egagraph, -1 for sparse
_LSEG
src/id_ca.c:long		_seg *audiostarts;	// array of offsets in audio / audiot
src/id_ca.c:	grstarts = (long _seg *)FP_SEG(&EGAhead);
src/id_ca.c:	tinf = (byte _seg *)FP_SEG(&maphead);
src/id_ca.c:		pos = ((mapfiletype	_seg *)tinf)->headeroffsets[i];
_MF_TSEG
src/id_ca.c:	audiostarts = (long _seg *)FP_SEG(&audiohead);
src/id_ca.c:		((mapfiletype _seg *)tinf)->RLEWtag);
src/id_ca.c:		((mapfiletype _seg *)tinf)->RLEWtag);
src/id_ca.c:					source = (byte _seg *)bufferseg+(pos-bufferstart);
src/id_ca.h:extern	byte 		_seg	*tinf;
src/id_ca.h:extern	unsigned	_seg	*mapsegs[MAPPLANES];
src/id_ca.h:extern	maptype		_seg	*mapheaderseg[NUMMAPS];
src/id_ca.h:extern	byte		_seg	*audiosegs[NUMSNDCHUNKS];
src/id_ca.h:extern	void		_seg	*grsegs[NUMCHUNKS];
src/id_ca.h:extern long		_seg *grstarts;	// array of offsets in egagraph, -1 for sparse
src/id_ca.h:extern long		_seg *audiostarts;	// array of offsets in audio / audiot
src/id_in.c:		byte _seg	*DemoBuffer;
src/id_in.h:extern	byte _seg	*DemoBuffer;
src/id_mm.h:typedef void _seg * memptr;
src/id_pm.c:					_seg *PMSegPages;
src/id_pm.c:	void			_seg *buf;
src/id_pm.c:		MM_FreePtr(&(void _seg *)PMSegPages);
src/id_sd.c:	word		*SoundTable;	// Really * _seg *SoundTable, but that don't work
src/id_sd.c:		word			_seg *DigiList;
src/id_vh.c:pictabletype	_seg *pictable;
_PT_TSEG
src/id_vh.c:	byte		far *dest, far *srcline, _seg *temp;
src/id_vh.c:	, fontstruct _seg *font)
_F_TSEG
src/id_vh.c:	VWL_MeasureString(string,width,height,(fontstruct _seg *)grsegs[STARTFONT+fontnumber]);
src/id_vh.c:	VWL_MeasureString(string,width,height,(fontstruct _seg *)grsegs[STARTFONTM+fontnumber]);
src/id_vh.c:	src = (byte _seg *)grsegs[STARTTILE8];
src/id_vh.c:	src = (byte _seg *)grsegs[STARTTILE16];
src/id_vh.c:		src = (byte _seg *)grsegs[STARTTILE16+i];
src/id_vh.h:extern	pictabletype	_seg *pictable;
src/id_vh.h:extern	pictabletype	_seg *picmtable;
src/id_vh.h:extern	spritetabletype _seg *spritetable;
grep: src/obj: Is a directory
src/oldscale.c:t_compscale _seg *scaledirectory[MAXSCALEHEIGHT+1];
src/oldscale.c:t_compscale 	_seg *work;
src/oldscale.c:	_fmemcpy ((byte _seg *)(*finalspot),(byte _seg *)work,totalsize);
src/oldscale.c:	t_compshape	_seg *shape;
src/oldscale.c:	t_compscale _seg *comptable;
_C_CTSEG
src/oldscale.c:	t_compshape	_seg *shape;
src/oldscale.c:	t_compscale _seg *comptable;
_C_CTSEG
grep: src/readme: Is a directory
src/wl_debug.c:	spritetype _seg	*block;
src/wl_debug.c:extern	word	_seg *DigiList;
src/wl_def.h:extern	t_compscale _seg *scaledirectory[MAXSCALEHEIGHT+1];
src/wl_menu.c:	fontstruct _seg *font;
src/wl_scale.c:t_compscale _seg *scaledirectory[MAXSCALEHEIGHT+1];
src/wl_scale.c:t_compscale 	_seg *work;
src/wl_scale.c:	_fmemcpy ((byte _seg *)(*finalspot),(byte _seg *)work,totalsize);
src/wl_scale.c:	t_compshape	_seg *shape;
src/wl_scale.c:	t_compscale _seg *comptable;
_C_CTSEG
src/wl_scale.c:	t_compshape	_seg *shape;
src/wl_scale.c:	t_compscale _seg *comptable;
_CSEG
src/wl_text.c:	text = (char _seg *)grsegs[artnum];
src/wl_text.c:	text = (char _seg *)layout;
src/wl_text.c:	text = (char _seg *)grsegs[artnum];
src/wl_text.c:	text = (char _seg *)layout;

*/
