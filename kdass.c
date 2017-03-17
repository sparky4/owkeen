#include "kdass.h"
/*
int px,py;
unsigned char		pdrawmode,fontcolor;

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
		VBLActive:
		in al,dx
		test al,8
		jnz VBLActive
		noVBL:
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
*/
