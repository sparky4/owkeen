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

// ID_CA.C

/*
=============================================================================

Id Software Caching Manager
---------------------------

Must be started BEFORE the memory manager, because it needs to get the headers
loaded into the data segment

=============================================================================
*/

#include "ID_HEADS.H"
#pragma hdrstop

/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

typedef struct
{
  unsigned bit0,bit1;	// 0-255 is a character, > is a pointer to a node
} huffnode;


typedef struct
{
	unsigned	RLEWtag;
	long		headeroffsets[100];
	byte		headersize[100];		// headers are very small
	byte		tileinfo[];
} mapfiletype;


/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

byte 		_seg	*tinf;
int			mapon;

unsigned	_seg	*mapsegs[3];
maptype		_seg	*mapheaderseg[NUMMAPS];
byte		_seg	*audiosegs[NUMSNDCHUNKS];
void		_seg	*grsegs[NUMCHUNKS];

byte		grneeded[NUMCHUNKS];
byte		ca_levelbit,ca_levelnum;

char		*titleptr[8];

int			profilehandle;

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

extern	long	far	CGAhead;
extern	long	far	EGAhead;
extern	byte	CGAdict;
extern	byte	EGAdict;
extern	byte	far	maphead;
extern	byte	mapdict;
extern	byte	far	audiohead;
extern	byte	audiodict;


long		_seg *grstarts;	// array of offsets in egagraph, -1 for sparse
long		_seg *audiostarts;	// array of offsets in audio / audiot

#ifdef GRHEADERLINKED
huffnode	*grhuffman;
#else
huffnode	grhuffman[255];
#endif

#ifdef MAPHEADERLINKED
huffnode	*maphuffman;
#else
huffnode	maphuffman[255];
#endif

#ifdef AUDIOHEADERLINKED
huffnode	*audiohuffman;
#else
huffnode	audiohuffman[255];
#endif


int			grhandle;		// handle to EGAGRAPH
int			maphandle;		// handle to MAPTEMP / GAMEMAPS
int			audiohandle;	// handle to AUDIOT / AUDIO

long		chunkcomplen,chunkexplen;

SDMode		oldsoundmode;

/*
=============================================================================

					   LOW LEVEL ROUTINES

=============================================================================
*/

/*
============================
=
= CAL_GetGrChunkLength
=
= Gets the length of an explicit length chunk (not tiles)
= The file pointer is positioned so the compressed data can be read in next.
=
============================
*/

void CAL_GetGrChunkLength (int chunk)
{
	lseek(grhandle,grstarts[chunk],SEEK_SET);
	read(grhandle,&chunkexplen,sizeof(chunkexplen));
	chunkcomplen = grstarts[chunk+1]-grstarts[chunk]-4;
}


/*
==========================
=
= CA_FarRead
=
= Read from a file to a far pointer
=
==========================
*/

boolean CA_FarRead (int handle, byte far *dest, long length)
{
	boolean flag=false;
	if (length>0xffffl)
		Quit ("CA_FarRead doesn't support 64K reads yet!");

	__asm {
		push	ds
		mov	bx,[handle]
		mov	cx,[WORD PTR length]
		mov	dx,[WORD PTR dest]
		mov	ds,[WORD PTR dest+2]
		mov	ah,0x3f				// READ w/handle
		int	21h
		pop	ds
		jnc	good
		mov	errno,ax
		mov	flag,0
		jmp End
#ifdef __BORLANDC__
	}
#endif
good:
#ifdef __BORLANDC__
	__asm {
#endif
		cmp	ax,[WORD PTR length]
		je	done
//		errno = EINVFMT;			// user manager knows this is bad read
		mov	flag,0
		jmp End
#ifdef __BORLANDC__
	}
#endif
done:
#ifdef __BORLANDC__
	__asm {
#endif
		mov	flag,1
#ifdef __BORLANDC__
	}
#endif
End:
#ifdef __WATCOMC__
	}
#endif
	return flag;
}


/*
==========================
=
= CA_SegWrite
=
= Write from a file to a far pointer
=
==========================
*/

boolean CA_FarWrite (int handle, byte far *source, long length)
{
	boolean flag=false;
	if (length>0xffffl)
		Quit ("CA_FarWrite doesn't support 64K reads yet!");

	__asm {
		push	ds
		mov	bx,[handle]
		mov	cx,[WORD PTR length]
		mov	dx,[WORD PTR source]
		mov	ds,[WORD PTR source+2]
		mov	ah,0x40			// WRITE w/handle
		int	21h
		pop	ds
		jnc	good
		mov	errno,ax
		mov flag,0
		jmp End
#ifdef __BORLANDC__
	}
#endif
good:
#ifdef __BORLANDC__
	__asm {
#endif
		cmp	ax,[WORD PTR length]
		je	done
//		errno = ENOMEM;				// user manager knows this is bad write
		mov	flag,0
		jmp End
#ifdef __BORLANDC__
	}
#endif
done:
#ifdef __BORLANDC__
	__asm {
#endif
		mov	flag,1
#ifdef __BORLANDC__
	}
#endif
End:
#ifdef __WATCOMC__
	}
#endif
	return flag;
}


/*
==========================
=
= CA_ReadFile
=
= Reads a file into an allready allocated buffer
=
==========================
*/

boolean CA_ReadFile (char *filename, memptr *ptr)
{
	int handle;
	long size;

	if ((handle = open(filename,O_RDONLY | O_BINARY, S_IREAD)) == -1)
		return false;

	size = filelength (handle);
	if (!CA_FarRead (handle,*ptr,size))
	{
		close (handle);
		return false;
	}
	close (handle);
	return true;
}


/*
==========================
=
= CA_WriteFile
=
= Writes a file from a memory buffer
=
==========================
*/

boolean CA_WriteFile (char *filename, void far *ptr, long length)
{
	int handle;
	long size;

	handle = open(filename,O_CREAT | O_BINARY | O_WRONLY,
				S_IREAD | S_IWRITE | S_IFREG);

	if (handle == -1)
		return false;

	if (!CA_FarWrite (handle,ptr,length))
	{
		close (handle);
		return false;
	}
	close (handle);
	return true;
}



/*
==========================
=
= CA_LoadFile
=
= Allocate space for and load a file
=
==========================
*/

boolean CA_LoadFile (char *filename, memptr *ptr)
{
	int handle;
	long size;

	if ((handle = open(filename,O_RDONLY | O_BINARY, S_IREAD)) == -1)
		return false;

	size = filelength (handle);
	MM_GetPtr (ptr,size);
	if (!CA_FarRead (handle,*ptr,size))
	{
		close (handle);
		return false;
	}
	close (handle);
	return true;
}

/*
============================================================================

		COMPRESSION routines, see JHUFF.C for more

============================================================================
*/



/*
===============
=
= CAL_OptimizeNodes
=
= Goes through a huffman table and changes the 256-511 node numbers to the
= actular address of the node.  Must be called before CAL_HuffExpand
=
===============
*/

void CAL_OptimizeNodes (huffnode *table)
{
  huffnode *node;
  int i;

  node = table;

  for (i=0;i<255;i++)
  {
	if (node->bit0 >= 256)
	  node->bit0 = (unsigned)(table+(node->bit0-256));
	if (node->bit1 >= 256)
	  node->bit1 = (unsigned)(table+(node->bit1-256));
	node++;
  }
}



/*
======================
=
= CAL_HuffExpand
=
= Length is the length of the EXPANDED data
=
======================
*/

void CAL_HuffExpand (byte huge *source, byte huge *dest,
	long length,huffnode *hufftable)
{
	unsigned bit,byte,node,code;
	unsigned sourceseg,sourceoff,destseg,destoff,endoff;
	huffnode *nodeon,*headptr;

	headptr = hufftable+254;	// head node is allways node 254

	source++;	// normalize
	source--;
	dest++;
	dest--;

	sourceseg = FP_SEG(source);
	sourceoff = FP_OFF(source);
	destseg = FP_SEG(dest);
	destoff = FP_OFF(dest);
	endoff = destoff+length;

//
// ds:si source
// es:di dest
// ss:bx node pointer
//

	if (length <0xfff0)
	{

//--------------------------
// expand less than 64k of data
//--------------------------

		__asm {
			mov	bx,[word ptr headptr]

			mov	si,[sourceoff]
			mov	di,[destoff]
			mov	es,[destseg]
			mov	ds,[sourceseg]
			mov	ax,[endoff]

			mov	ch,[si]				// load first byte
			inc	si
			mov	cl,1
#ifdef __BORLANDC__
		}
#endif
expandshort:
#ifdef __BORLANDC__
		__asm {
#endif
			test	ch,cl			// bit set?
			jnz	bit1short
			mov	dx,[ss:bx]			// take bit0 path from node
			shl	cl,1				// advance to next bit position
			jc	newbyteshort
			jnc	sourceupshort
#ifdef __BORLANDC__
		}
#endif
bit1short:
#ifdef __BORLANDC__
		__asm {
#endif
			mov	dx,[ss:bx+2]		// take bit1 path
			shl	cl,1				// advance to next bit position
			jnc	sourceupshort
#ifdef __BORLANDC__
		}
#endif
newbyteshort:
#ifdef __BORLANDC__
		__asm {
#endif
			mov	ch,[si]				// load next byte
			inc	si
			mov	cl,1				// back to first bit
#ifdef __BORLANDC__
		}
#endif
sourceupshort:
#ifdef __BORLANDC__
	__asm {
#endif
			or	dh,dh				// if dx<256 its a byte, else move node
			jz	storebyteshort
			mov	bx,dx				// next node = (huffnode *)code
			jmp	expandshort
#ifdef __BORLANDC__
		}
#endif
storebyteshort:
#ifdef __BORLANDC__
		__asm {
#endif
			mov	[es:di],dl
			inc	di					// write a decopmpressed byte out
			mov	bx,[word ptr headptr]		// back to the head node for next bit

			cmp	di,ax				// done?
			jne	expandshort
		}
	}
	else
	{

//--------------------------
// expand more than 64k of data
//--------------------------

	length--;

		__asm {
			mov	bx,[word ptr headptr]
			mov	cl,1

			mov	si,[sourceoff]
			mov	di,[destoff]
			mov	es,[destseg]
			mov	ds,[sourceseg]

			lodsb			// load first byte
#ifdef __BORLANDC__
		}
#endif
expand:
#ifdef __BORLANDC__
		__asm {
#endif
			test	al,cl		// bit set?
			jnz	bit1
			mov	dx,[ss:bx]	// take bit0 path from node
			jmp	gotcode
#ifdef __BORLANDC__
		}
#endif
bit1:
#ifdef __BORLANDC__
		__asm {
#endif
			mov	dx,[ss:bx+2]	// take bit1 path
#ifdef __BORLANDC__
		}
#endif
gotcode:
#ifdef __BORLANDC__
		__asm {
#endif
			shl	cl,1		// advance to next bit position
			jnc	sourceup
			lodsb
			cmp	si,0x10		// normalize ds:si
			jb	sinorm
			mov	cx,ds
			inc	cx
			mov	ds,cx
			xor	si,si
#ifdef __BORLANDC__
		}
#endif
sinorm:
#ifdef __BORLANDC__
		__asm {
#endif
			mov	cl,1		// back to first bit
#ifdef __BORLANDC__
		}
#endif
sourceup:
#ifdef __BORLANDC__
		__asm {
#endif
			or	dh,dh		// if dx<256 its a byte, else move node
			jz	storebyte
			mov	bx,dx		// next node = (huffnode *)code
			jmp	expand
#ifdef __BORLANDC__
		}
#endif
storebyte:
#ifdef __BORLANDC__
		__asm {
#endif
			mov	[es:di],dl
			inc	di		// write a decopmpressed byte out
			mov	bx,[word ptr headptr]	// back to the head node for next bit

			cmp	di,0x10		// normalize es:di
			jb	dinorm
			mov	dx,es
			inc	dx
			mov	es,dx
			xor	di,di
#ifdef __BORLANDC__
		}
#endif
dinorm:
#ifdef __BORLANDC__
		__asm {
#endif
			sub	[WORD PTR ss:length],1
			jnc	expand
			dec	[WORD PTR ss:length+2]
			jns	expand		// when length = ffff ffff, done
		}
	}

	__asm {
		mov	ax,ss
		mov	ds,ax
	}

}



/*
======================
=
= CA_RLEWcompress
=
======================
*/

long CA_RLEWCompress (unsigned huge *source, long length, unsigned huge *dest,
  unsigned rlewtag)
{
  long complength;
  unsigned value,count,i;
  unsigned huge *start,huge *end;

  start = dest;

  end = source + (length+1)/2;

//
// compress it
//
  do
  {
    count = 1;
    value = *source++;
    while (*source == value && source<end)
    {
      count++;
      source++;
    }
    if (count>3 || value == rlewtag)
    {
    //
    // send a tag / count / value string
    //
      *dest++ = rlewtag;
      *dest++ = count;
      *dest++ = value;
    }
    else
    {
    //
    // send word without compressing
    //
      for (i=1;i<=count;i++)
	*dest++ = value;
	}

  } while (source<end);

  complength = 2*(dest-start);
  return complength;
}


/*
======================
=
= CA_RLEWexpand
= length is EXPANDED length
=
======================
*/

void CA_RLEWexpand (unsigned huge *source, unsigned huge *dest,long length,
  unsigned rlewtag)
{
//  unsigned value,count,i;
  unsigned huge *end;
  unsigned sourceseg,sourceoff,destseg,destoff,endseg,endoff;


//
// expand it
//
#if 0
  do
  {
	value = *source++;
	if (value != rlewtag)
	//
	// uncompressed
	//
	  *dest++=value;
	else
	{
	//
	// compressed string
	//
	  count = *source++;
	  value = *source++;
	  for (i=1;i<=count;i++)
	*dest++ = value;
	}
  } while (dest<end);
#endif

  end = dest + (length)/2;
  sourceseg = FP_SEG(source);
  sourceoff = FP_OFF(source);
  destseg = FP_SEG(dest);
  destoff = FP_OFF(dest);
  endseg = FP_SEG(end);
  endoff = FP_OFF(end);


//
// ax = source value
// bx = tag value
// cx = repeat counts
// dx = scratch
//
// NOTE: A repeat count that produces 0xfff0 bytes can blow this!
//

	__asm {
		mov	bx,rlewtag
		mov	si,sourceoff
		mov	di,destoff
		mov	es,destseg
		mov	ds,sourceseg
#ifdef __BORLANDC__
	}
#endif
expand:
#ifdef __BORLANDC__
	__asm {
#endif
		lodsw
		cmp	ax,bx
		je	repeat
		stosw
		jmp	next
#ifdef __BORLANDC__
	}
#endif
repeat:
#ifdef __BORLANDC__
	__asm {
#endif
		lodsw
		mov	cx,ax		// repeat count
		lodsw			// repeat value
		rep stosw
#ifdef __BORLANDC__
	}
#endif
next:
#ifdef __BORLANDC__
	__asm {
#endif
		cmp	si,0x10		// normalize ds:si
		jb	sinorm
		mov	ax,si
		shr	ax,1
		shr	ax,1
		shr	ax,1
		shr	ax,1
		mov	dx,ds
		add	dx,ax
		mov	ds,dx
		and	si,0xf
#ifdef __BORLANDC__
	}
#endif
sinorm:
#ifdef __BORLANDC__
	__asm {
#endif
		cmp	di,0x10		// normalize es:di
		jb	dinorm
		mov	ax,di
		shr	ax,1
		shr	ax,1
		shr	ax,1
		shr	ax,1
		mov	dx,es
		add	dx,ax
		mov	es,dx
		and	di,0xf
#ifdef __BORLANDC__
	}
#endif
dinorm:
#ifdef __BORLANDC__
	__asm {
#endif
		cmp     di,ss:endoff
		jne	expand
		mov	ax,es
		cmp	ax,ss:endseg
		jb	expand

		mov	ax,ss
		mov	ds,ax
	}

}



/*
=============================================================================

					 CACHE MANAGER ROUTINES

=============================================================================
*/


/*
======================
=
= CAL_SetupGrFile
=
======================
*/

void CAL_SetupGrFile (void)
{
	int handle;
	long headersize,length;
	memptr compseg;

#ifdef GRHEADERLINKED

#ifdef GRMODEEGA
	grhuffman = (huffnode *)&EGAdict;
	grstarts = (long _seg *)FP_SEG(&EGAhead);
#endif
#ifdef GRMODECGA
	grhuffman = (huffnode *)&CGAdict;
	grstarts = (long _seg *)FP_SEG(&CGAhead);
#endif

	CAL_OptimizeNodes (grhuffman);

#else

//
// load ???dict.ext (huffman dictionary for graphics files)
//

//	if ((handle = open(GREXT"DICT.",
	if ((handle = open("KDREAMS.EGA",
		 O_RDONLY | O_BINARY, S_IREAD)) == -1)
		Quit ("Can't open KDREAMS.EGA!");

	read(handle, &grhuffman, sizeof(grhuffman));
	close(handle);
	CAL_OptimizeNodes (grhuffman);
//
// load the data offsets from ???head.ext
//
	MM_GetPtr (&(memptr)grstarts,(NUMCHUNKS+1)*4);

	if ((handle = open(GREXT"HEAD."EXTENSION,
		 O_RDONLY | O_BINARY, S_IREAD)) == -1)
		Quit ("Can't open "GREXT"HEAD."EXTENSION"!");

	CA_FarRead(handle, (memptr)grstarts, (NUMCHUNKS+1)*4);

	close(handle);


#endif

//
// Open the graphics file, leaving it open until the game is finished
//
//	grhandle = open(GREXT"GRAPH."EXTENSION, O_RDONLY | O_BINARY); NOLAN
	grhandle = open("KDREAMS.EGA", O_RDONLY | O_BINARY);
	if (grhandle == -1)
		Quit ("Cannot open KDREAMS.EGA!");


//
// load the pic and sprite headers into the arrays in the data segment
//
//	if(NUMPICS>0){
		MM_GetPtr(&(memptr)pictable,NUMPICS*sizeof(pictabletype));
		CAL_GetGrChunkLength(STRUCTPIC);		// position file pointer
		MM_GetPtr(&compseg,chunkcomplen);
		CA_FarRead (grhandle,compseg,chunkcomplen);
		CAL_HuffExpand (compseg, (byte huge *)pictable,NUMPICS*sizeof(pictabletype),grhuffman);
		MM_FreePtr(&compseg);
//	}

//	if(NUMPICM>0){
		MM_GetPtr(&(memptr)picmtable,NUMPICM*sizeof(pictabletype));
		CAL_GetGrChunkLength(STRUCTPICM);		// position file pointer
		MM_GetPtr(&compseg,chunkcomplen);
		CA_FarRead (grhandle,compseg,chunkcomplen);
		CAL_HuffExpand (compseg, (byte huge *)picmtable,NUMPICS*sizeof(pictabletype),grhuffman);
		MM_FreePtr(&compseg);
//	}

//	if(NUMSPRITES>0){
		MM_GetPtr(&(memptr)spritetable,NUMSPRITES*sizeof(spritetabletype));
		CAL_GetGrChunkLength(STRUCTSPRITE);	// position file pointer
		MM_GetPtr(&compseg,chunkcomplen);
		CA_FarRead (grhandle,compseg,chunkcomplen);
		CAL_HuffExpand (compseg, (byte huge *)spritetable,NUMSPRITES*sizeof(spritetabletype),grhuffman);
		MM_FreePtr(&compseg);
//	}

}

//==========================================================================


/*
======================
=
= CAL_SetupMapFile
=
======================
*/

void CAL_SetupMapFile (void)
{
	int handle,i;
	long length;
	byte far *buffer;

//
// load maphead.ext (offsets and tileinfo for map file)
//
#ifndef MAPHEADERLINKED
//	if ((handle = open("MAPHEAD."EXTENSION,
	if ((handle = open("KDREAMS.MAP",
		 O_RDONLY | O_BINARY, S_IREAD)) == -1)
		Quit ("Can't open KDREAMS.MAP!");
	length = filelength(handle);
	MM_GetPtr (&(memptr)tinf,length);
	CA_FarRead(handle, tinf, length);
	close(handle);
#else

	maphuffman = (huffnode *)&mapdict;
	CAL_OptimizeNodes (maphuffman);
	tinf = (byte _seg *)FP_SEG(&maphead);

#endif

//
// open the data file
//
#ifdef MAPHEADERLINKED
	if ((maphandle = open("KDREAMS.MAP",
		 O_RDONLY | O_BINARY, S_IREAD)) == -1)
		Quit ("Can't open KDREAMS.MAP!");
#else
	if ((maphandle = open("MAPTEMP."EXTENSION,
		 O_RDONLY | O_BINARY, S_IREAD)) == -1)
		Quit ("Can't open MAPTEMP."EXTENSION"!");
#endif
}

//==========================================================================


/*
======================
=
= CAL_SetupAudioFile
=
======================
*/

void CAL_SetupAudioFile (void)
{
	int handle,i;
	long length;
	byte far *buffer;

//
// load maphead.ext (offsets and tileinfo for map file)
//
#ifndef AUDIOHEADERLINKED
	if ((handle = open("AUDIOHED."EXTENSION,
		 O_RDONLY | O_BINARY, S_IREAD)) == -1)
		Quit ("Can't open AUDIOHED."EXTENSION"!");
	length = filelength(handle);
	MM_GetPtr (&(memptr)audiostarts,length);
	CA_FarRead(handle, (byte far *)audiostarts, length);
	close(handle);
#else
	audiohuffman = (huffnode *)&audiodict;
	CAL_OptimizeNodes (audiohuffman);
	audiostarts = (long _seg *)FP_SEG(&audiohead);
#endif

//
// open the data file
//
#ifndef AUDIOHEADERLINKED
	if ((audiohandle = open("AUDIOT."EXTENSION,
		 O_RDONLY | O_BINARY, S_IREAD)) == -1)
		Quit ("Can't open AUDIOT."EXTENSION"!");
#else
//	if ((audiohandle = open("AUDIO."EXTENSION,	NOLAN
	if ((audiohandle = open("KDREAMS.AUD",
		 O_RDONLY | O_BINARY, S_IREAD)) == -1)
		Quit ("Can't open KDREAMS.AUD!");
#endif
}

//==========================================================================


/*
======================
=
= CA_Startup
=
= Open all files and load in headers
=
======================
*/

void CA_Startup (void)
{
#ifdef PROFILE
	unlink ("PROFILE.TXT");
	profilehandle = open("PROFILE.TXT", O_CREAT | O_WRONLY | O_TEXT);
#endif

	CAL_SetupMapFile ();
	CAL_SetupGrFile ();
	CAL_SetupAudioFile ();

	mapon = -1;
	ca_levelbit = 1;
	ca_levelnum = 0;
}

//==========================================================================


/*
======================
=
= CA_Shutdown
=
= Closes all files
=
======================
*/

void CA_Shutdown (void)
{
#ifdef PROFILE
	close (profilehandle);
#endif

	close (maphandle);
	close (grhandle);
}

//===========================================================================

/*
======================
=
= CA_CacheAudioChunk
=
======================
*/

void CA_CacheAudioChunk (int chunk)
{
	long	pos,compressed,expanded;
	memptr	bigbufferseg;
	byte	far *source;

	if (audiosegs[chunk])
	{
		MM_SetPurge (&(memptr)audiosegs[chunk],0);
		return;							// allready in memory
	}

//
// load the chunk into a buffer, either the miscbuffer if it fits, or allocate
// a larger buffer
//
	pos = audiostarts[chunk];
	compressed = audiostarts[chunk+1]-pos;

	lseek(audiohandle,pos,SEEK_SET);

#ifndef AUDIOHEADERLINKED

	MM_GetPtr (&(memptr)audiosegs[chunk],compressed);
	CA_FarRead(audiohandle,audiosegs[chunk],compressed);

#else

	if (compressed<=BUFFERSIZE)
	{
		CA_FarRead(audiohandle,bufferseg,compressed);
		source = bufferseg;
	}
	else
	{
		MM_GetPtr(&bigbufferseg,compressed);
		CA_FarRead(audiohandle,bigbufferseg,compressed);
		source = bigbufferseg;
	}

	expanded = *(long far *)source;
	source += 4;			// skip over length
	MM_GetPtr (&(memptr)audiosegs[chunk],expanded);
	CAL_HuffExpand (source,audiosegs[chunk],expanded,audiohuffman);

	if (compressed>BUFFERSIZE)
		MM_FreePtr(&bigbufferseg);
#endif
}

//===========================================================================

/*
======================
=
= CA_LoadAllSounds
=
= Purges all sounds, then loads all new ones (mode switch)
=
======================
*/

void CA_LoadAllSounds (void)
{
	unsigned	start,i;

	switch (oldsoundmode)
	{
	case sdm_Off:
		goto cachein;
	case sdm_PC:
		start = STARTPCSOUNDS;
		break;
	case sdm_AdLib:
		start = STARTADLIBSOUNDS;
		break;
	case sdm_SoundBlaster:
	case sdm_SoundSource:
		start = STARTDIGISOUNDS;
		break;
	}

	for (i=0;i<NUMSOUNDS;i++,start++)
		if (audiosegs[start])
			MM_SetPurge (&(memptr)audiosegs[start],3);		// make purgable

cachein:

	switch (SoundMode)
	{
	case sdm_Off:
		return;
	case sdm_PC:
		start = STARTPCSOUNDS;
		break;
	case sdm_AdLib:
		start = STARTADLIBSOUNDS;
		break;
	case sdm_SoundBlaster:
	case sdm_SoundSource:
		start = STARTDIGISOUNDS;
		break;
	}

	for (i=0;i<NUMSOUNDS;i++,start++)
		CA_CacheAudioChunk (start);

	oldsoundmode = SoundMode;
}

//===========================================================================

#ifdef GRMODEEGA

/*
======================
=
= CAL_ShiftSprite
=
= Make a shifted (one byte wider) copy of a sprite into another area
=
======================
*/

unsigned	static	sheight,swidth;

void CAL_ShiftSprite (unsigned segment,unsigned source,unsigned dest,
	unsigned width, unsigned height, unsigned pixshift)
{

	sheight = height;		// because we are going to reassign bp
	swidth = width;

	__asm {
		mov	ax,[segment]
		mov	ds,ax		// source and dest are in same segment, and all local

		mov	bx,[source]
		mov	di,[dest]

		mov	bp,[pixshift]
		shl	bp,1
		mov	bp,[shifttabletable+bp]	// bp holds pointer to shift table

//
// table shift the mask
//
		mov	dx,[ss:sheight]
#ifdef __BORLANDC__
	}
#endif

domaskrow:

#ifdef __BORLANDC__
	__asm {
#endif
		mov	BYTE PTR [di],255	// 0xff first byte
		mov	cx,ss:[swidth]
#ifdef __BORLANDC__
	}
#endif

domaskbyte:

#ifdef __BORLANDC__
	__asm {
#endif
		mov	al,[bx]				// source
		not	al
		inc	bx					// next source byte
		xor	ah,ah
		shl	ax,1
		mov	si,ax
		mov	ax,[bp+si]			// table shift into two bytes
		not	ax
		and	[di],al				// and with first byte
		inc	di
		mov	[di],ah				// replace next byte

		loop	domaskbyte

		inc	di					// the last shifted byte has 1s in it
		dec	dx
		jnz	domaskrow

//
// table shift the data
//
		mov	dx,ss:[sheight]
		shl	dx,1
		shl	dx,1				// four planes of data
#ifdef __BORLANDC__
	}
#endif

dodatarow:

#ifdef __BORLANDC__
	__asm {
#endif
		mov	BYTE PTR [di],0		// 0 first byte
		mov	cx,ss:[swidth]
#ifdef __BORLANDC__
	}
#endif

dodatabyte:

#ifdef __BORLANDC__
	__asm {
#endif
		mov	al,[bx]				// source
		inc	bx					// next source byte
		xor	ah,ah
		shl	ax,1
		mov	si,ax
		mov	ax,[bp+si]			// table shift into two bytes
		or	[di],al				// or with first byte
		inc	di
		mov	[di],ah				// replace next byte

		loop	dodatabyte

		inc	di					// the last shifted byte has 0s in it
		dec	dx
		jnz	dodatarow

//
// done
//

		mov	ax,ss				// restore data segment
		mov	ds,ax
	}

}

#endif

//===========================================================================

/*
======================
=
= CAL_CacheSprite
=
= Generate shifts and set up sprite structure for a given sprite
=
======================
*/

void CAL_CacheSprite (int chunk, char far *compressed)
{
	int i;
	unsigned shiftstarts[5];
	unsigned smallplane,bigplane,expanded;
	spritetabletype far *spr;
	spritetype _seg *dest;

#ifdef GRMODECGA
//
// CGA has no pel panning, so shifts are never needed
//
	spr = &spritetable[chunk-STARTSPRITES];
	smallplane = spr->width*spr->height;
	MM_GetPtr (&grsegs[chunk],smallplane*2+MAXSHIFTS*6);
	dest = (spritetype _seg *)grsegs[chunk];
	dest->sourceoffset[0] = MAXSHIFTS*6;	// start data after 3 unsigned tables
	dest->planesize[0] = smallplane;
	dest->width[0] = spr->width;

//
// expand the unshifted shape
//
	CAL_HuffExpand (compressed, &dest->data[0],smallplane*2,grhuffman);

#endif


#ifdef GRMODEEGA

//
// calculate sizes
//
	spr = &spritetable[chunk-STARTSPRITES];
	smallplane = spr->width*spr->height;
	bigplane = (spr->width+1)*spr->height;

	shiftstarts[0] = MAXSHIFTS*6;	// start data after 3 unsigned tables
	shiftstarts[1] = shiftstarts[0] + smallplane*5;	// 5 planes in a sprite
	shiftstarts[2] = shiftstarts[1] + bigplane*5;
	shiftstarts[3] = shiftstarts[2] + bigplane*5;
	shiftstarts[4] = shiftstarts[3] + bigplane*5;	// nothing ever put here

	expanded = shiftstarts[spr->shifts];
	MM_GetPtr (&grsegs[chunk],expanded);
	dest = (spritetype _seg *)grsegs[chunk];

//
// expand the unshifted shape
//
	CAL_HuffExpand (compressed, &dest->data[0],smallplane*5,grhuffman);

//
// make the shifts!
//
	switch (spr->shifts)
	{
	case	1:
		for (i=0;i<4;i++)
		{
			dest->sourceoffset[i] = shiftstarts[0];
			dest->planesize[i] = smallplane;
			dest->width[i] = spr->width;
		}
		break;

	case	2:
		for (i=0;i<2;i++)
		{
			dest->sourceoffset[i] = shiftstarts[0];
			dest->planesize[i] = smallplane;
			dest->width[i] = spr->width;
		}
		for (i=2;i<4;i++)
		{
			dest->sourceoffset[i] = shiftstarts[1];
			dest->planesize[i] = bigplane;
			dest->width[i] = spr->width+1;
		}
		CAL_ShiftSprite ((unsigned)grsegs[chunk],dest->sourceoffset[0],
			dest->sourceoffset[2],spr->width,spr->height,4);
		break;

	case	4:
		dest->sourceoffset[0] = shiftstarts[0];
		dest->planesize[0] = smallplane;
		dest->width[0] = spr->width;

		dest->sourceoffset[1] = shiftstarts[1];
		dest->planesize[1] = bigplane;
		dest->width[1] = spr->width+1;
		CAL_ShiftSprite ((unsigned)grsegs[chunk],dest->sourceoffset[0],
			dest->sourceoffset[1],spr->width,spr->height,2);

		dest->sourceoffset[2] = shiftstarts[2];
		dest->planesize[2] = bigplane;
		dest->width[2] = spr->width+1;
		CAL_ShiftSprite ((unsigned)grsegs[chunk],dest->sourceoffset[0],
			dest->sourceoffset[2],spr->width,spr->height,4);

		dest->sourceoffset[3] = shiftstarts[3];
		dest->planesize[3] = bigplane;
		dest->width[3] = spr->width+1;
		CAL_ShiftSprite ((unsigned)grsegs[chunk],dest->sourceoffset[0],
			dest->sourceoffset[3],spr->width,spr->height,6);

		break;

	default:
		Quit ("CAL_CacheSprite: Bad shifts number!");
	}

#endif
}

//===========================================================================


/*
======================
=
= CAL_ExpandGrChunk
=
= Does whatever is needed with a pointer to a compressed chunk
=
======================
*/

void CAL_ExpandGrChunk (int chunk, byte far *source)
{
	long	pos,compressed,expanded;
	int		next;
	spritetabletype	*spr;


	if (chunk>=STARTTILE8)
	{
	//
	// expanded sizes of tile8/16/32 are implicit
	//

#ifdef GRMODEEGA
#define BLOCK		32
#define MASKBLOCK	40
#endif

#ifdef GRMODECGA
#define BLOCK		16
#define MASKBLOCK	32
#endif

		if (chunk<STARTTILE8M)			// tile 8s are all in one chunk!
			expanded = BLOCK*NUMTILE8;
		else if (chunk<STARTTILE16)
			expanded = MASKBLOCK*NUMTILE8M;
		else if (chunk<STARTTILE16M)	// all other tiles are one/chunk
			expanded = BLOCK*4;
		else if (chunk<STARTTILE32)
			expanded = MASKBLOCK*4;
		else if (chunk<STARTTILE32M)
			expanded = BLOCK*16;
		else
			expanded = MASKBLOCK*16;
	}
	else
	{
	//
	// everything else has an explicit size longword
	//
		expanded = *(long far *)source;
		source += 4;			// skip over length
	}

//
// allocate final space, decompress it, and free bigbuffer
// Sprites need to have shifts made and various other junk
//
	if (chunk>=STARTSPRITES && chunk< STARTTILE8)
		CAL_CacheSprite(chunk,source);
	else
	{
		MM_GetPtr (&grsegs[chunk],expanded);
		CAL_HuffExpand (source,grsegs[chunk],expanded,grhuffman);
	}
}


/*
======================
=
= CAL_ReadGrChunk
=
= Gets a chunk off disk, optimizing reads to general buffer
=
======================
*/

void CAL_ReadGrChunk (int chunk)
{
	long	pos,compressed,expanded;
	memptr	bigbufferseg;
	byte	far *source;
	int		next;
	spritetabletype	*spr;

//
// load the chunk into a buffer, either the miscbuffer if it fits, or allocate
// a larger buffer
//
	pos = grstarts[chunk];
	if (pos<0)							// $FFFFFFFF start is a sparse tile
	  return;

	next = chunk +1;
	while (grstarts[next] == -1)		// skip past any sparse tiles
		next++;

	compressed = grstarts[next]-pos;

	lseek(grhandle,pos,SEEK_SET);

	if (compressed<=BUFFERSIZE)
	{
		CA_FarRead(grhandle,bufferseg,compressed);
		source = bufferseg;
	}
	else
	{
		MM_GetPtr(&bigbufferseg,compressed);
		CA_FarRead(grhandle,bigbufferseg,compressed);
		source = bigbufferseg;
	}

	CAL_ExpandGrChunk (chunk,source);

	if (compressed>BUFFERSIZE)
		MM_FreePtr(&bigbufferseg);
}


/*
======================
=
= CA_CacheGrChunk
=
= Makes sure a given chunk is in memory, loadiing it if needed
=
======================
*/

void CA_CacheGrChunk (int chunk)
{
	long	pos,compressed,expanded;
	memptr	bigbufferseg;
	byte	far *source;
	int		next;

	grneeded[chunk] |= ca_levelbit;		// make sure it doesn't get removed
	if (grsegs[chunk])
	  return;							// allready in memory

//
// load the chunk into a buffer, either the miscbuffer if it fits, or allocate
// a larger buffer
//
	pos = grstarts[chunk];
	if (pos<0)							// $FFFFFFFF start is a sparse tile
	  return;

	next = chunk +1;
	while (grstarts[next] == -1)		// skip past any sparse tiles
		next++;

	compressed = grstarts[next]-pos;

	lseek(grhandle,pos,SEEK_SET);

	if (compressed<=BUFFERSIZE)
	{
		CA_FarRead(grhandle,bufferseg,compressed);
		source = bufferseg;
	}
	else
	{
		MM_GetPtr(&bigbufferseg,compressed);
		CA_FarRead(grhandle,bigbufferseg,compressed);
		source = bigbufferseg;
	}

	CAL_ExpandGrChunk (chunk,source);

	if (compressed>BUFFERSIZE)
		MM_FreePtr(&bigbufferseg);
}



//==========================================================================

/*
======================
=
= CA_CacheMap
=
======================
*/

void CA_CacheMap (int mapnum)
{
	long	pos,compressed,expanded;
	int		plane;
	memptr	*dest,bigbufferseg,buffer2seg;
	unsigned	size;
	unsigned	far	*source;


//
// free up memory from last map
//
	if (mapon>-1 && mapheaderseg[mapon])
		MM_SetPurge (&(memptr)mapheaderseg[mapon],3);
	for (plane=0;plane<3;plane++)
		if (mapsegs[plane])
			MM_FreePtr (&(memptr)mapsegs[plane]);

	mapon = mapnum;


//
// load map header
// The header will be cached if it is still around
//
	if (!mapheaderseg[mapnum])
	{
		pos = ((mapfiletype	_seg *)tinf)->headeroffsets[mapnum];
		if (pos<0)						// $FFFFFFFF start is a sparse map
		  Quit ("CA_CacheMap: Tried to load a non existant map!");

		MM_GetPtr(&(memptr)mapheaderseg[mapnum],sizeof(maptype));
		lseek(maphandle,pos,SEEK_SET);

#ifdef MAPHEADERLINKED
//#if BUFFERSIZE < sizeof(maptype)
//		if(BUFFERSIZE < sizeof(maptype))
//			printf("The general buffer size is too small!");
//#endif
		//
		// load in, then unhuffman to the destination
		//
		CA_FarRead (maphandle,bufferseg,((mapfiletype	_seg *)tinf)->headersize[mapnum]);
		CAL_HuffExpand ((byte huge *)bufferseg,
			(byte huge *)mapheaderseg[mapnum],sizeof(maptype),maphuffman);
#else
		CA_FarRead (maphandle,(memptr)mapheaderseg[mapnum],sizeof(maptype));
#endif
	}
	else
		MM_SetPurge (&(memptr)mapheaderseg[mapnum],0);

//
// load the planes in
// If a plane's pointer still exists it will be overwritten (levels are
// allways reloaded, never cached)
//

	size = mapheaderseg[mapnum]->width * mapheaderseg[mapnum]->height * 2;

	for (plane = 0; plane<3; plane++)
	{
		dest = &(memptr)mapsegs[plane];
		MM_GetPtr(dest,size);

		pos = mapheaderseg[mapnum]->planestart[plane];
		compressed = mapheaderseg[mapnum]->planelength[plane];
		lseek(maphandle,pos,SEEK_SET);
		if (compressed<=BUFFERSIZE)
			source = bufferseg;
		else
		{
			MM_GetPtr(&bigbufferseg,compressed);
			source = bigbufferseg;
		}

		CA_FarRead(maphandle,(byte far *)source,compressed);
#ifdef MAPHEADERLINKED
		//
		// unhuffman, then unRLEW
		// The huffman'd chunk has a two byte expanded length first
		// The resulting RLEW chunk also does, even though it's not really
		// needed
		//
		expanded = *source;
		source++;
		MM_GetPtr (&buffer2seg,expanded);
		CAL_HuffExpand ((byte huge *)source, buffer2seg,expanded,maphuffman);
		CA_RLEWexpand (((unsigned far *)buffer2seg)+1,*dest,size,
		((mapfiletype _seg *)tinf)->RLEWtag);
		MM_FreePtr (&buffer2seg);

#else
		//
		// unRLEW, skipping expanded length
		//
		CA_RLEWexpand (source+1, *dest,size,
		((mapfiletype _seg *)tinf)->RLEWtag);
#endif

		if (compressed>BUFFERSIZE)
			MM_FreePtr(&bigbufferseg);
	}
}

//===========================================================================

/*
======================
=
= CA_UpLevel
=
= Goes up a bit level in the needed lists and clears it out.
= Everything is made purgable
=
======================
*/

void CA_UpLevel (void)
{
	int i;

	if (ca_levelnum==7)
		Quit ("CA_UpLevel: Up past level 7!");

//	for (i=0;i<NUMCHUNKS;i++)
//			if (grsegs[i])
//				MM_SetPurge(&grsegs[i],3);

	ca_levelbit<<=1;
	ca_levelnum++;
}

//===========================================================================

/*
======================
=
= CA_DownLevel
=
= Goes down a bit level in the needed lists and recaches
= everything from the lower level
=
======================
*/

void CA_DownLevel (void)
{
	if (!ca_levelnum)
		Quit ("CA_DownLevel: Down past level 0!");
	ca_levelbit>>=1;
	ca_levelnum--;
	CA_CacheMarks(titleptr[ca_levelnum], 1);
}

//===========================================================================

/*
======================
=
= CA_ClearMarks
=
= Clears out all the marks at the current level
=
======================
*/

void CA_ClearMarks (void)
{
	int i;

	for (i=0;i<NUMCHUNKS;i++)
		grneeded[i]&=~ca_levelbit;
}


//===========================================================================

/*
======================
=
= CA_ClearAllMarks
=
= Clears out all the marks on all the levels
=
======================
*/

void CA_ClearAllMarks (void)
{
	memset (grneeded,0,sizeof(grneeded));
	ca_levelbit = 1;
	ca_levelnum = 0;
}


//===========================================================================


/*
======================
=
= CA_CacheMarks
=
======================
*/

#define NUMBARS	(17l*8)
#define BARSTEP	8
#define MAXEMPTYREAD	1024

void CA_CacheMarks (char *title, boolean cachedownlevel)
{
	boolean dialog;
	int 	i,next,homex,homey,x,y,thx,thy,numcache,lastx,xl,xh;
	long	barx,barstep;
	long	pos,endpos,nextpos,nextendpos,compressed;
	long	bufferstart,bufferend;	// file position of general buffer
	byte	far *source;
	memptr	bigbufferseg;

	//
	// save title so cache down level can redraw it
	//
	titleptr[ca_levelnum] = title;

	dialog = (title!=NULL);

	if (cachedownlevel)
		dialog = false;

	if (dialog)
	{
	//
	// draw dialog window (masked tiles 12 - 20 are window borders)
	//
		US_CenterWindow (20,8);
		homex = PrintX;
		homey = PrintY;

		US_CPrint ("Loading");
		fontcolor = F_SECONDCOLOR;
		US_CPrint (title);
		fontcolor = F_BLACK;
		VW_UpdateScreen();
#ifdef PROFILE
		write(profilehandle,title,strlen(title));
		write(profilehandle,"\n",1);
#endif

	}

	numcache = 0;
//
// go through and make everything not needed purgable
//
	for (i=0;i<NUMCHUNKS;i++)
		if (grneeded[i]&ca_levelbit)
		{
			if (grsegs[i])					// its allready in memory, make
				MM_SetPurge(&grsegs[i],0);	// sure it stays there!
			else
				numcache++;
		}
		else
		{
			if (grsegs[i])					// not needed, so make it purgeable
				MM_SetPurge(&grsegs[i],3);
		}

	if (!numcache)			// nothing to cache!
		return;

	if (dialog)
	{
	//
	// draw thermometer bar
	//
		thx = homex + 8;
		thy = homey + 32;
		VWB_DrawTile8(thx,thy,11);
		VWB_DrawTile8(thx,thy+8,14);
		VWB_DrawTile8(thx,thy+16,17);
		VWB_DrawTile8(thx+17*8,thy,13);
		VWB_DrawTile8(thx+17*8,thy+8,16);
		VWB_DrawTile8(thx+17*8,thy+16,19);
		for (x=thx+8;x<thx+17*8;x+=8)
		{
			VWB_DrawTile8(x,thy,12);
			VWB_DrawTile8(x,thy+8,15);
			VWB_DrawTile8(x,thy+16,18);
		}

		thx += 4;		// first line location
		thy += 5;
		barx = (long)thx<<16;
		lastx = thx;
		VW_UpdateScreen();
	}

//
// go through and load in anything still needed
//
	barstep = (NUMBARS<<16)/numcache;
	bufferstart = bufferend = 0;		// nothing good in buffer now

	for (i=0;i<NUMCHUNKS;i++)
		if ( (grneeded[i]&ca_levelbit) && !grsegs[i])
		{
//
// update thermometer
//
			if (dialog)
			{
				barx+=barstep;
				xh = barx>>16;
				if (xh - lastx > BARSTEP)
				{
					for (x=lastx;x<=xh;x++)
#ifdef GRMODEEGA
						VWB_Vlin (thy,thy+13,x,14);
#endif
#ifdef GRMODECGA
						VWB_Vlin (thy,thy+13,x,SECONDCOLOR);
#endif
					lastx = xh;
					VW_UpdateScreen();
				}

			}
			pos = grstarts[i];
			if (pos<0)
				continue;

			next = i +1;
			while (grstarts[next] == -1)		// skip past any sparse tiles
				next++;

			compressed = grstarts[next]-pos;
			endpos = pos+compressed;

			if (compressed<=BUFFERSIZE)
			{
				if (bufferstart<=pos
				&& bufferend>= endpos)
				{
				// data is allready in buffer
					source = (byte _seg *)bufferseg+(pos-bufferstart);
				}
				else
				{
				// load buffer with a new block from disk
				// try to get as many of the needed blocks in as possible
					while ( next < NUMCHUNKS )
					{
						while (next < NUMCHUNKS &&
						!(grneeded[next]&ca_levelbit && !grsegs[next]))
							next++;
						if (next == NUMCHUNKS)
							continue;

						nextpos = grstarts[next];
						while (grstarts[++next] == -1)	// skip past any sparse tiles
							;
						nextendpos = grstarts[next];
						if (nextpos - endpos <= MAXEMPTYREAD
						&& nextendpos-pos <= BUFFERSIZE)
							endpos = nextendpos;
						else
							next = NUMCHUNKS;			// read pos to posend
					}

					lseek(grhandle,pos,SEEK_SET);
					CA_FarRead(grhandle,bufferseg,endpos-pos);
					bufferstart = pos;
					bufferend = endpos;
					source = bufferseg;
				}
			}
			else
			{
			// big chunk, allocate temporary buffer
				MM_GetPtr(&bigbufferseg,compressed);
				lseek(grhandle,pos,SEEK_SET);
				CA_FarRead(grhandle,bigbufferseg,compressed);
				source = bigbufferseg;
			}

			CAL_ExpandGrChunk (i,source);

			if (compressed>BUFFERSIZE)
				MM_FreePtr(&bigbufferseg);

		}

//
// finish up any thermometer remnants
//
		if (dialog)
		{
			xh = thx + NUMBARS;
			for (x=lastx;x<=xh;x++)
#ifdef GRMODEEGA
				VWB_Vlin (thy,thy+13,x,14);
#endif
#ifdef GRMODECGA
				VWB_Vlin (thy,thy+13,x,SECONDCOLOR);
#endif
			VW_UpdateScreen();
		}
}

