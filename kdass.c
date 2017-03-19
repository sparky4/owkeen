#include "kdass.h"

#define	STATUS_REGISTER_1    0x3da

//*
int px,py;
byte		pdrawmode,fontcolor;
unsigned	bufferwidth,bufferheight;	// used by font drawing stuff
unsigned	**shifttabletable;

int rndindex = 0;

static byte rndtable[] = {
      0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
	 74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
	 95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
	 52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
	149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
	145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0,
	175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
	 25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
	 94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
	136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
	135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
	 80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
	 24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
	145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
	 28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
	 71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
	 17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
	197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
	120, 163, 236, 249 };

//========
//
// VW_WaitVBL (int number)
//
//========

void VW_WaitVBL(int num)
{
	__asm {
		mov	cx,num
		mov	dx,03dah
#ifdef __BORLANDC__
	}
#endif
VBLActive:
#ifdef __BORLANDC__
	__asm {
#endif
		in al,dx
		test al,8
		jnz VBLActive
#ifdef __BORLANDC__
	}
#endif
noVBL:
#ifdef __BORLANDC__
	__asm {
#endif
		in al,dx
		test al,8
		jz noVBL
		loop VBLActive
		//parm [cx]
		//modify exact [dx al cx]
	}
}

//===========================================================================

//==============
//
// VW_SetScreen
//
//==============

void	VW_SetScreen (unsigned crtc, unsigned pelpan)
{
// PROC	VL_SetScreen  crtc:WORD, pel:WORD
// PUBLIC	VL_SetScreen
	__asm {
		mov	cx,[TimeCount]		// if TimeCount goes up by two, the retrace
		add	cx,2				// period was missed (an interrupt covered it)

		mov	dx,STATUS_REGISTER_1

	// wait for a display signal to make sure the raster isn't in the middle
	// of a sync

#ifdef __BORLANDC__
	}
#endif
SetScreen_waitdisplay:
#ifdef __BORLANDC__
	__asm {
#endif
		in	al,dx
		test	al,1	//1 = display is disabled (HBL / VBL)
		jnz	SetScreen_waitdisplay

#ifdef __BORLANDC__
	}
#endif
SetScreen_loop:
#ifdef __BORLANDC__
	__asm {
#endif
		sti
		jmp	SetScreen_skip1
		cli
#ifdef __BORLANDC__
	}
#endif
SetScreen_skip1:
#ifdef __BORLANDC__
	__asm {
#endif
		cmp	[TimeCount],cx		// will only happen if an interrupt is
		jae	SetScreen_setcrtc			// straddling the entire retrace period

	// when several succesive display not enableds occur,
	// the bottom of the screen has been hit

		in	al,dx
		test	al,8
		jnz	SetScreen_waitdisplay
		test	al,1
		jz	SetScreen_loop

		in	al,dx
		test	al,8
		jnz	SetScreen_waitdisplay
		test	al,1
		jz	SetScreen_loop

		in	al,dx
		test	al,8
		jnz	SetScreen_waitdisplay
		test	al,1
		jz	SetScreen_loop

		in	al,dx
		test	al,8
		jnz	SetScreen_waitdisplay
		test	al,1
		jz	SetScreen_loop

		in	al,dx
		test	al,8
		jnz	SetScreen_waitdisplay
		test	al,1
		jz	SetScreen_loop

#ifdef __BORLANDC__
	}
#endif
SetScreen_setcrtc:
#ifdef __BORLANDC__
	__asm {
#endif
	// set CRTC start
	// for some reason, my XT's EGA card doesn't like word outs to the CRTC index...

		mov	cx,[crtc]
		mov	dx,CRTC_INDEX
		mov	al,0ch		//start address high register
		out	dx,al
		inc	dx
		mov	al,ch
		out	dx,al
		dec	dx
		mov	al,0dh		//start address low register
		out	dx,al
		mov	al,cl
		inc	dx
		out	dx,al


	// set horizontal panning

		mov	dx,ATR_INDEX
		mov	al,ATR_PELPAN or 20h
		out	dx,al
		jmp	SetScreen_done
		mov	al,[BYTE PTR pelpan]		//pel pan value
		out	dx,al
#ifdef __BORLANDC__
	}
#endif
SetScreen_done:
#ifdef __BORLANDC__
	__asm {
#endif
		sti
	}
}

/*void VW_MaskBlock(memptr segm,unsigned ofs,unsigned dest,
	unsigned width,unsigned height,unsigned planesize);
//============================================================================
//
// VW_MemToScreen
//
// Basic block drawing routine. Takes a block shape at segment pointer source
// of width by height data, and draws it to dest in the virtual screen,
// based on linewidth.  bufferofs is NOT accounted for.
// There are four drawing routines to provide the best optimized code while
// accounting for odd segment wrappings due to the floating screens.
//
// DONE
//
//============================================================================

//DATASEG

enum memtoscreentable {	eventoeven,eventoodd,oddtoeven,oddtoodd };

//CODESEG

void VW_MemToScreen(memptr source,unsigned dest,unsigned width,unsigned height)
{
//PROC	VW_MemToScreen	source:WORD, dest:WORD, width:WORD, height:WORD
//PUBLIC	VW_MemToScreen
//USES	SI,DI

	__asm {
	mov	es,[screenseg]

	mov	bx,[linewidth]
	sub	bx,[width]

	mov	ds,[source]

	xor	si,si					//block is segment aligned

	xor	di,di
	shr	[width],1				//change width to words, and see if carry is set
	rcl	di,1					//1 if width is odd
	mov	ax,[dest]
	shr	ax,1
	rcl	di,1					//shift a 1 in if destination is odd
	shl	di,1					//to index into a word width table
	mov	dx,[height]				//scan lines to draw
	mov	ax,[width]
	jmp	[ss:memtoscreentable+di]	//call the right routine

//==============
//
// Copy an even width block to an even destination address
//
//==============

eventoeven:
	mov	di,[dest]				//start at same place in all planes
EVEN
@@lineloopEE:
	mov	cx,ax
	rep	movsw
	add	di,bx
	dec	dx
	jnz	@@lineloopEE

	mov	ax,ss
	mov	ds,ax					//restore turbo's data segment

	ret

//==============
//
// Copy an odd width block to an even video address
//
//==============

oddtoeven:
	mov	di,[dest]				//start at same place in all planes
EVEN
@@lineloopOE:
	mov	cx,ax
	rep	movsw
	movsb						//copy the last byte
	add	di,bx
	dec	dx
	jnz	@@lineloopOE

	mov	ax,ss
	mov	ds,ax					//restore turbo's data segment

	ret

//==============
//
// Copy an even width block to an odd video address
//
//==============

eventoodd:
	mov	di,[dest]				//start at same place in all planes
	dec	ax						//one word has to be handled seperately
EVEN
@@lineloopEO:
	movsb
	mov	cx,ax
	rep	movsw
	movsb
	add	di,bx
	dec	dx
	jnz	@@lineloopEO

	mov	ax,ss
	mov	ds,ax					//restore turbo's data segment

	ret

//==============
//
// Copy an odd width block to an odd video address
//
//==============

oddtoodd:
	mov	di,[dest]				//start at same place in all planes
EVEN
@@lineloopOO:
	movsb
	mov	cx,ax
	rep	movsw
	add	di,bx
	dec	dx
	jnz	@@lineloopOO

	mov	ax,ss
	mov	ds,ax					//restore turbo's data segment
	//ret
	}
}*/

//===========================================================================
//
// VW_ScreenToMem
//
// Copies a block of video memory to main memory, in order from planes 0-3.
// This could be optimized along the lines of VW_MemToScreen to take advantage
// of word copies, but this is an infrequently called routine.
//
// DONE
//
//===========================================================================

void VW_ScreenToMem(unsigned source,memptr dest,unsigned width,unsigned height)
{
//PROC	VW_ScreenToMem	source:WORD, dest:WORD, width:WORD, height:WORD
//PUBLIC	VW_ScreenToMem
//USES	SI,DI

	__asm {
		mov	es,[dest]

		mov	bx,[linewidth]
		sub	bx,[width]

		mov	ds,[screenseg]

		xor	di,di

		mov	si,[source]
		mov	dx,[height]				//scan lines to draw

lineloop:
		mov	cx,[width]
		rep	movsb

		add	si,bx

		dec	dx
		jnz	lineloop

		mov	ax,ss
		mov	ds,ax					//restore turbo's data segment
	}
	//ret
}

//============================================================================
//
// VW_ScreenToScreen
//
// Basic block copy routine.  Copies one block of screen memory to another,
// bufferofs is NOT accounted for.
//
// DONE
//
//============================================================================

//PROC	VW_ScreenToScreen	source:WORD, dest:WORD, width:WORD, height:WORD
//PUBLIC	VW_ScreenToScreen
//USES	SI,DI
void VW_ScreenToScreen(unsigned source,unsigned dest,unsigned width,unsigned height)
{
	__asm {
		mov	bx,[linewidth]
		sub	bx,[width]

		mov	ax,[screenseg]
		mov	es,ax
		mov	ds,ax

		mov	si,[source]
		mov	di,[dest]				//start at same place in all planes
		mov	dx,[height]				//scan lines to draw
		mov	ax,[width]
//
// if the width, source, and dest are all even, use word moves
// This is allways the case in the CGA refresh
//
		test	ax,1
		jnz	bytelineloop
		test	si,1
		jnz	bytelineloop
		test	di,1
		jnz	bytelineloop

		shr	ax,1
wordlineloop:
		mov	cx,ax
		rep	movsw
		add	si,bx
		add	di,bx

		dec	dx
		jnz	wordlineloop

		mov	ax,ss
		mov	ds,ax					//restore turbo's data segment

		ret

bytelineloop:
		mov	cx,ax
		rep	movsb
		add	si,bx
		add	di,bx

		dec	dx
		jnz	bytelineloop

		mov	ax,ss
		mov	ds,ax					//restore turbo's data segment
	}
//	ret

}

//*/

///////////////////////////////////////////////////////////////////////////
//
// US_InitRndT - Initializes the pseudo random number generator.
//      If randomize is true, the seed will be initialized depending on the
//      current time
//
///////////////////////////////////////////////////////////////////////////
void US_InitRndT(boolean randomize)
{
	__asm {
		mov	ax,SEG rndtable
		mov	es,ax

		mov	ax,[randomize]
		or	ax,ax
		jne	timeit		//if randomize is true, really random

		mov	dx,0			//set to a definite value
		jmp	setit

	timeit:
		mov	ah,2ch
		int	21h			//GetSystemTime
		and	dx,0ffh

	setit:
		mov	[es:rndindex],dx
		ret
	}
    /*if(randomize)
        rndindex = TimeIt() & 0xff;
    else
        rndindex = 0;*/
}

///////////////////////////////////////////////////////////////////////////
//
// US_RndT - Returns the next 8-bit pseudo random number
//
///////////////////////////////////////////////////////////////////////////
int US_RndT()
{
    rndindex = (rndindex+1)&0xff;
    return rndtable[rndindex];
}

	/*void US_InitRndT(int randomize);
	#pragma aux US_InitRndT parm [AX] modify exact [ax cx edx]

	int US_RndT();
	#pragma aux US_RndT value [EAX] modify exact [eax ebx]*/
