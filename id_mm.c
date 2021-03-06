/* Keen Dreams Source Code
 * Copyright (C) 2014 Javier M. Chavez
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

// NEWMM.C

/*
=============================================================================

		   ID software memory manager
		   --------------------------

Primary coder: John Carmack

RELIES ON
---------
Quit (char *error) function


WORK TO DO
----------
Soft error on out of memory

EMS 64k frame, upper memory block mapping

MM_SizePtr to change the size of a given pointer

Multiple purge levels utilized

EMS / XMS unmanaged routines

=============================================================================
*/

#include "ID_HEADS.H"
#pragma hdrstop

/*
=============================================================================

							LOCAL INFO

=============================================================================
*/

//#define EMSINT		0x67

#define LOCKBIT		0x80	// if set in attributes, block cannot be moved
#define PURGEBITS	3		// 0-3 level, 0= unpurgable, 3= purge first
#define PURGEMASK	0xfffc
#define BASEATTRIBUTES	0	// unlocked, non purgable

typedef struct mmblockstruct
{
	unsigned	start,length;
	unsigned	attributes;
	memptr		*useptr;	// pointer to the segment start
	struct mmblockstruct far *next;
} mmblocktype;


//#define GETNEWBLOCK {if(!(mmnew=mmfree))Quit("MM_GETNEWBLOCK: No free blocks!") ;mmfree=mmfree->next;}
//

#define GETNEWBLOCK {if(!mmfree)MML_ClearBlock();mmnew=mmfree;mmfree=mmfree->next;}

#define FREEBLOCK(x) {*x->useptr=NULL;x->next=mmfree;mmfree=x;}

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

mminfotype	mminfo;
memptr		bufferseg;
boolean		bombonerror;

void		(* beforesort) (void);
void		(* aftersort) (void);

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

boolean		mmstarted;

void far	*farheap;
void		*nearheap;

mmblocktype	far mmblocks[MAXBLOCKS]
			,far *mmhead,far *mmfree,far *mmrover,far *mmnew;

//==========================================================================

//
// local prototypes
//

boolean		MML_CheckForEMS (void);
void 		MML_ShutdownEMS (void);
void 		MM_MapEMS (void);
boolean 	MML_CheckForXMS (void);
void 		MML_ShutdownXMS (void);
void		MML_ClearBlock (void);

//==========================================================================
#ifndef __ID_PM__
/*
======================
=
= MML_CheckForEMS
=
= Routine from p36 of Extending DOS
=
=======================
*/

boolean MML_CheckForEMS (void)
{
	boolean	emmcfems = false;
	static char	emmname[9] = "EMMXXXX0";

	__asm {
		mov	dx,OFFSET emmname
		mov	ax,0x3d00
		int	0x21		// try to open EMMXXXX0 device
		jc	error

		mov	bx,ax
		mov	ax,0x4400

		int	0x21		// get device info
		jc	error

		and	dx,0x80
		jz	error

		mov	ax,0x4407

		int	0x21		// get status
		jc	error
		or	al,al
		jz	error

		mov	ah,0x3e
		int	0x21		// close handle
		jc	error

		//
		// EMS is good
		//
		mov	emmcfems,1
		jmp	End
#ifdef __BORLANDC__
	}
#endif
error:
#ifdef __BORLANDC__
	__asm {
#endif
		//
		// EMS is bad
		//
		mov	emmcfems,0
#ifdef __BORLANDC__
	}
#endif
		End:
#ifdef __WATCOMC__
	}
#endif

	return emmcfems;

}

/*
======================
=
= MML_ShutdownEMS
=
=======================
*/

void MML_ShutdownEMS (void)
{

}

/*
====================
=
= MM_MapEMS
=
= Maps the 64k of EMS used by memory manager into the page frame
= for general use.  This only needs to be called if you are keeping
= other things in EMS.
=
====================
*/

void MM_MapEMS (void)
{

}

//==========================================================================

/*
======================
=
= MML_CheckForXMS
=
= Try to allocate an upper memory block
=
=======================
*/

boolean MML_CheckForXMS (void)
{

	return false;
}


/*
======================
=
= MML_ShutdownXMS
=
=======================
*/

void MML_ShutdownXMS (void)
{

}
#endif
//==========================================================================

/*
======================
=
= MML_UseSpace
=
= Marks a range of paragraphs as usable by the memory manager
= This is used to mark space for the near heap, far heap, ems page frame,
= and upper memory blocks
=
======================
*/

/*
	extra = oldend - (segstart+seglength);

	segmlen=extra;

	//++++emsver stuff!
	if(segm>1)/// || extra>=0x10000lu)
	//if(extra>0xfffflu)
	{
		scan->blob=segm;

		//MML_UseSpace(segstart, seglength);

		printf("MML_UseSpace: Segment spans two blocks!\n");
	//}
	printf("segm=%u		", segm);
	printf("ex=%lu	", extra);
	printf("old=%u	", oldend);
	printf("start+seglen=%lu\n", segstart+seglength);
	printf("segsta=%x	", segstart);
	printf("len=%lu	", scan->length);
	printf("seglen=%lu	", seglength);
	printf("segmlen=%lu\n", segmlen);
	}
//++++todo: linked list of segment!
*/
void MML_UseSpace (unsigned segstart, unsigned seglength)
{
	mmblocktype far *scan,far *last;
	unsigned	oldend;
	sdword		extra;
	//word segm=1;

	scan = last = mmhead;
	mmrover = mmhead;		// reset rover to start of memory

//
// search for the block that contains the range of segments
//
	while (scan->start+scan->length < segstart)
	{
		last = scan;
		scan = scan->next;
	}

//
// find out how many blocks it spans!
//
	/*for(;seglength>=0x10000;seglength-=0xFFFF)
	{
		//printf("	seglen=%lu\n", segmlen);
		segm++;
	}*/

//
// take the given range out of the block
//
	oldend = scan->start + scan->length;
	extra = oldend - (segstart+seglength);
	if (extra < 0)
#ifdef __DEBUG_MM__
	{
		printf("========================================\n");
		printf("start=%x	", scan->start);
		printf("old=%u	", oldend);
		printf("start+seglen=%lu\n", segstart+seglength);
		printf("segsta=%x	", segstart);
		printf("len=%lu	", scan->length);
		printf("seglen=%lu	", seglength);
		printf("\n");
		printf("MML_UseSpace: Segment spans two blocks!	%d\n", extra);
		printf("========================================\n");
		//return;
	}
#else
		Quit ("MML_UseSpace: Segment spans two blocks!");
#endif

	if (segstart == scan->start)
	{
		last->next = scan->next;			// unlink block
		FREEBLOCK(scan);
		scan = last;
	}
	else
		scan->length = segstart-scan->start;	// shorten block

	if (extra > 0)
	{
		GETNEWBLOCK;
		mmnew->useptr = NULL;

		mmnew->next = scan->next;
		scan->next = mmnew;
		mmnew->start = segstart+seglength;
		mmnew->length = extra;
		mmnew->attributes = LOCKBIT;
	}//else if(segm>0) goto segu;

}

//==========================================================================

/*
====================
=
= MML_ClearBlock
=
= We are out of blocks, so free a purgable block
=
====================
*/

void MML_ClearBlock ()
{
	mmblocktype far *scan;//,far *last;

	scan = mmhead->next;

	while(scan)
	{
		if(!(scan->attributes&LOCKBIT) && (scan->attributes&PURGEBITS) )
		{
			MM_FreePtr (scan->useptr);
			return;
		}
		scan = scan->next;
	}

	Quit ("MM_ClearBlock: No purgable blocks!\n");
}


//==========================================================================

/*
===================
=
= MM_Reset
=
===================
*/

void MM_Reset ()
{
	//has to be 16
	if(sizeof(mmblocktype)!=16)
		return;
/*
#ifdef __BORLANDC__
	strcpy(datadumpfilename, "mmdump.16b");
#endif
#ifdef __WATCOMC__
	strcpy(datadumpfilename, "mmdump.16w");
#endif

#ifdef __BORLANDC__
	strcpy(heapdumpfilename, "heap.16b");
#endif
#ifdef __WATCOMC__
	strcpy(heapdumpfilename, "heap.16w");
#endif*/
}


//==========================================================================

/*
===================
=
= MM_Startup
=
= Grabs all space from turbo with malloc/farmalloc
= Allocates bufferseg misc buffer
=
===================
*/

void MM_Startup (void)
{
	int i;
	unsigned 	long length;
	void far 	*start;
	unsigned 	segstart,seglength,endfree;

	if (mmstarted)
		MM_Shutdown ();

	mmstarted = true;
	bombonerror = true;

//
// set up the linked list (everything in the free list;
//
	mmhead = NULL;
	mmfree = &mmblocks[0];
	for (i=0;i<MAXBLOCKS-1;i++)
		mmblocks[i].next = &mmblocks[i+1];
	mmblocks[i].next = NULL;

//
// locked block of all memory until we punch out free space
//
	GETNEWBLOCK;
	mmhead = mmnew;				// this will allways be the first node
	mmnew->start = 0;
	mmnew->length = 0xffff;
	mmnew->attributes = LOCKBIT;
	mmnew->next = NULL;
	mmrover = mmhead;


//
// get all available near conventional memory segments
//
	length=coreleft();
	start = (void far *)(nearheap = _nmalloc(length));

	length -= 16-(FP_OFF(start)&15);
	length -= SAVENEARHEAP;
	seglength = length / 16;			// now in paragraphs
	segstart = FP_SEG(start)+(FP_OFF(start)+15)/16;
	mminfo.nearheap = length;

	// locked block of unusable low memory
	// from 0 to start of near heap
	GETNEWBLOCK;
	mmhead = mmnew;				// this will allways be the first node
	mmnew->start = 0;
	mmnew->length = segstart;
	mmnew->attributes = LOCKBIT;
	endfree = segstart+seglength;
	mmrover = mmhead;

//
// get all available far conventional memory segments
//
	length=farcoreleft();
	start = farheap = farmalloc(length);

	length -= 16-(FP_OFF(start)&15);
	length -= SAVEFARHEAP;
	seglength = length / 16;			// now in paragraphs
	segstart = FP_SEG(start)+(FP_OFF(start)+15)/16;
	mminfo.farheap = length;
	mminfo.mainmem = mminfo.nearheap + mminfo.farheap;

	// locked block of unusable near heap memory (usually just the stack)
	// from end of near heap to start of far heap
	GETNEWBLOCK;
	mmnew->start = endfree;
	mmnew->length = segstart-endfree;
	mmnew->attributes = LOCKBIT;
	mmrover->next = mmnew;
	endfree = segstart+seglength;
	mmrover = mmnew;


#ifndef __ID_PM__
//
// detect EMS and allocate 64K at page frame
//
	if (MML_CheckForEMS())
	{
		MM_MapEMS();					// map in used pages
		mminfo.EMSmem = 0x10000l;
	}
	else
	{
		mminfo.EMSmem = 0;
	}

//
// detect XMS and get upper memory blocks
//
	if (MML_CheckForXMS())
	{

	}
	else
	{
		mminfo.XMSmem = 0;
	}
#endif


//
// cap off the list
//
	// locked block of high memory (video, rom, etc)
	// from end of far heap or EMS/XMS to 0xffff
	GETNEWBLOCK;
	mmnew->start = endfree;
	mmnew->length = 0xffff-endfree;
	mmnew->attributes = LOCKBIT;
	mmnew->next = NULL;
	mmrover->next = mmnew;

//
// allocate the misc buffer
//
	mmrover = mmhead;		// start looking for space after low block

	MM_GetPtr (&bufferseg,BUFFERSIZE);
}

//==========================================================================

/*
====================
=
= MM_Shutdown
=
= Frees all conventional, EMS, and XMS allocated
=
====================
*/

void MM_Shutdown (void)
{
	if (!mmstarted)
		return;

	farfree (farheap);
	free (nearheap);
#ifndef __ID_PM__
	MML_ShutdownEMS ();
	MML_ShutdownXMS ();
#endif
}

//==========================================================================

/*
====================
=
= MM_GetPtr
=
= Allocates an unlocked, unpurgable block
=
====================
*/

void MM_GetPtr (memptr *baseptr,unsigned long size)
{
	mmblocktype far *scan,far *lastscan,far *endscan
				,far *purge,far *next;
	int			search;
	unsigned	needed,startseg;

	needed = (size+15)/16;		// convert size from bytes to paragraphs

	GETNEWBLOCK;				// fill in start and next after a spot is found
	mmnew->length = needed;
	mmnew->useptr = baseptr;
	mmnew->attributes = BASEATTRIBUTES;
//	if(mmnew->useptr==NULL){
#ifdef __DEBUG_MM__
	printf("===============================================================================\n");
	printf("		MM_GetPtr\n");
	printf("===============================================================================\n");
		//%04x
		printf("	baseptr=%Fp	", baseptr); printf("useptr=%Fp\n", mmnew->useptr);
		//printf("	*baseptr=%Fp	", *baseptr); printf("*useptr=%Fp\n", *(mmnew->useptr));
		printf("	&baseptr=%Fp	", &baseptr); printf("&useptr=%Fp\n", &(mmnew->useptr));

		printf("	baseptr=%04x	", baseptr); printf("useptr=%04x\n", mmnew->useptr);
		//printf("	*baseptr=%04x	", *baseptr); printf("*useptr=%04x\n", *(mmnew->useptr));
		printf("	&baseptr=%04u	", &baseptr); printf("&useptr=%04u\n", &(mmnew->useptr));

		printf("	size is %lu\n", size);
#endif
	//Quit ("mmnew->useptr==NULL"); }

	for (search = 0; search<3; search++)
	{
	//
	// first search:	try to allocate right after the rover, then on up
	// second search: 	search from the head pointer up to the rover
	// third search:	compress memory, then scan from start
		if (search == 1 && mmrover == mmhead)
			search++;

		switch (search)
		{
		case 0:
			lastscan = mmrover;
			scan = mmrover->next;
			endscan = NULL;
			break;
		case 1:
			lastscan = mmhead;
			scan = mmhead->next;
			endscan = mmrover;
			break;
		case 2:
			MM_SortMem ();
			lastscan = mmhead;
			scan = mmhead->next;
			endscan = NULL;
			break;
		}

		startseg = lastscan->start + lastscan->length;

		while (scan != endscan)
		{
			if (scan->start - startseg >= needed)
			{
			//
			// got enough space between the end of lastscan and
			// the start of scan, so throw out anything in the middle
			// and allocate the new block
			//
				purge = lastscan->next;
				lastscan->next = mmnew;
				mmnew->start = *(unsigned *)baseptr = startseg;
				mmnew->next = scan;
				while ( purge != scan)
				{	// free the purgable block
					next = purge->next;
					FREEBLOCK(purge);
					purge = next;		// purge another if not at scan
				}
				mmrover = mmnew;
				return;	// good allocation!
			}

			//
			// if this block is purge level zero or locked, skip past it
			//
			if ( (scan->attributes & LOCKBIT)
				|| !(scan->attributes & PURGEBITS) )
			{
				lastscan = scan;
				startseg = lastscan->start + lastscan->length;
			}


			scan=scan->next;		// look at next line
		}
	}

	Quit ("Out of memory!  Please make sure you have enough free memory.");
}

//==========================================================================

/*
====================
=
= MM_FreePtr
=
= Allocates an unlocked, unpurgable block
=
====================
*/

void MM_FreePtr (memptr *baseptr)
{
	mmblocktype far *scan,far *last;

	last = mmhead;
	scan = last->next;

	if (baseptr == mmrover->useptr)	// removed the last allocated block
		mmrover = mmhead;

	while (scan->useptr != baseptr && scan)
	{
		last = scan;
		scan = scan->next;
	}

	if (!scan)
		Quit ("MM_FreePtr: Block not found!");

	last->next = scan->next;

	FREEBLOCK(scan);
}
//==========================================================================

/*
=====================
=
= MM_SetPurge
=
= Sets the purge level for a block (locked blocks cannot be made purgable)
=
=====================
*/

void MM_SetPurge (memptr *baseptr, int purge)
{
	mmblocktype far *start;

	start = mmrover;

	do
	{
		if (mmrover->useptr == baseptr)
			break;

		mmrover = mmrover->next;

		if (!mmrover)
			mmrover = mmhead;
		else if (mmrover == start)
			Quit ("MM_SetPurge: Block not found!");

	} while (1);

	mmrover->attributes &= ~PURGEBITS;
	mmrover->attributes |= purge;
}

//==========================================================================

/*
=====================
=
= MM_SetLock
=
= Locks / unlocks the block
=
=====================
*/

void MM_SetLock (memptr *baseptr, boolean locked)
{
	mmblocktype far *start;

	start = mmrover;

	do
	{
		if (mmrover->useptr == baseptr)
			break;

		mmrover = mmrover->next;

		if (!mmrover)
			mmrover = mmhead;
		else if (mmrover == start)
			Quit ("MM_SetLock: Block not found!");

	} while (1);

	mmrover->attributes &= ~LOCKBIT;
	mmrover->attributes |= locked*LOCKBIT;
}

//==========================================================================

/*
=====================
=
= MM_SortMem
=
= Throws out all purgable stuff and compresses movable blocks
=
=====================
*/

void MM_SortMem (void)
{
	mmblocktype far *scan,far *last,far *next;
	unsigned	start,length,source,dest;

	VW_ColorBorder (15);

	if (beforesort)
		beforesort();

	scan = mmhead;

	while (scan)
	{
		if (scan->attributes & LOCKBIT)
		{
		//
		// block is locked, so try to pile later blocks right after it
		//
			start = scan->start + scan->length;
		}
		else
		{
			if (scan->attributes & PURGEBITS)
			{
			//
			// throw out the purgable block
			//
				next = scan->next;
				FREEBLOCK(scan);
				last->next = next;
				scan = next;
				continue;
			}
			else
			{
			//
			// push the non purgable block on top of the last moved block
			//
				if (scan->start != start)
				{
					length = scan->length;
					source = scan->start;
					dest = start;
					while (length > 0xf00)
					{
						movedata(source,0,dest,0,0xf00*16);
						length -= 0xf00;
						source += 0xf00;
						dest += 0xf00;
					}
					movedata(source,0,dest,0,length*16);

					scan->start = start;
					*(unsigned *)scan->useptr = start;
				}
				start = scan->start + scan->length;
			}
		}

		last = scan;
		scan = scan->next;		// go to next block
	}

	mmrover = mmhead;

	if (aftersort)
		aftersort();

	VW_ColorBorder (0);
}


//==========================================================================

/*
=====================
=
= MM_ShowMemory
=
=====================
*/

void MM_ShowMemory (void)
{
	mmblocktype far *scan;
	unsigned color,temp;
	long	end;

	VW_SetLineWidth(40);
	temp = bufferofs;
	bufferofs = 0;
	VW_SetScreen (0,0);

	scan = mmhead;

	end = -1;

	while (scan)
	{
		if (scan->attributes & PURGEBITS)
			color = 5;		// dark purple = purgable
		else
			color = 9;		// medium blue = non purgable
		if (scan->attributes & LOCKBIT)
			color = 12;		// red = locked
		if (scan->start<=end)
			Quit ("MM_ShowMemory: Memory block order currupted!");
		end = scan->start+scan->length-1;
		VW_Hlin(scan->start,(unsigned)end,0,color);
		VW_Plot(scan->start,0,15);
		if (scan->next->start > end+1)
			VW_Hlin(end+1,scan->next->start,0,0);	// black = free
		scan = scan->next;
	}

	IN_Ack();
	VW_SetLineWidth(64);
	bufferofs = temp;
}

//==========================================================================


/*
======================
=
= MM_UnusedMemory
=
= Returns the total free space without purging
=
======================
*/

long MM_UnusedMemory (void)
{
	unsigned free;
	mmblocktype far *scan;

	free = 0;
	scan = mmhead;

	while (scan->next)
	{
		free += scan->next->start - (scan->start + scan->length);
		scan = scan->next;
	}

	return free*16l;
}

//==========================================================================


/*
======================
=
= MM_TotalFree
=
= Returns the total free space with purging
=
======================
*/

long MM_TotalFree (void)
{
	unsigned free;
	mmblocktype far *scan;

	free = 0;
	scan = mmhead;

	while (scan->next)
	{
		if ((scan->attributes&PURGEBITS) && !(scan->attributes&LOCKBIT))
			free += scan->length;
		free += scan->next->start - (scan->start + scan->length);
		scan = scan->next;
	}

	return free*16l;
}

//==========================================================================

/*
=====================
=
= MM_BombOnError
=
=====================
*/

void MM_BombOnError (boolean bomb)
{
	bombonerror = bomb;
}


