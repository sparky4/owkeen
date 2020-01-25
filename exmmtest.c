/* Project 16 Source Code~
 * Copyright (C) 2012-2020 sparky4 & pngwen & andrius4669 & joncampbell123 & yakui-lover
 *
 * This file is part of Project 16.
 *
 * Project 16 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Project 16 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>, or
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
/*
	exmm test
*/
#include "exmmtest.h"
#ifndef __ID_CA__
#include "id_ca.h"
#endif

void _seg *buffersegegg;
word drawofs;
word fontspace;
/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

char		str[80],str2[20];
boolean		singlestep,jumpcheat,godmode,tedlevel;
short	tedlevelnum;

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

void	DebugMemory (void);
void	TestSprites(void);
int		DebugKeys (void);
void	ShutdownId (void);
void	Quit (char *error);
void	InitGame (void);
void	main (int argc, char *argv[]);

//===========================================================================

#if FRILLS

/*
==================
=
= DebugMemory
=
==================
*/

void DebugMemory (void)
{
	VW_FixRefreshBuffer ();
	US_CenterWindow (16,7);

	US_CPrint ("Memory Usage");
	US_CPrint ("------------");
	US_Print ("Total     :");
	US_PrintUnsigned (mminfo.mainmem/1024);
	US_Print ("k\nFree      :");
	US_PrintUnsigned (MM_UnusedMemory()/1024);
	US_Print ("k\nWith purge:");
	US_PrintUnsigned (MM_TotalFree()/1024);
	US_Print ("k\n");
	VW_UpdateScreen();
	IN_Ack ();
#if GRMODE == EGAGR
	MM_ShowMemory ();
#endif
}

/*
===================
=
= TestSprites
=
===================
*/

#define DISPWIDTH	110
#define	TEXTWIDTH   40
void TestSprites(void)
{
	int hx,hy,sprite,oldsprite,bottomy,topx,shift;
	spritetabletype far *spr;
	spritetype _seg	*block;
	unsigned	mem,scan;


	VW_FixRefreshBuffer ();
	US_CenterWindow (30,17);

	US_CPrint ("Sprite Test");
	US_CPrint ("-----------");

	hy=PrintY;
	hx=(PrintX+56)&(~7);
	topx = hx+TEXTWIDTH;

	US_Print ("Chunk:\nWidth:\nHeight:\nOrgx:\nOrgy:\nXl:\nYl:\nXh:\nYh:\n"
			  "Shifts:\nMem:\n");

	bottomy = PrintY;

	sprite = STARTSPRITES;
	shift = 0;

	do
	{
		if (sprite>=STARTTILE8)
			sprite = STARTTILE8-1;
		else if (sprite<STARTSPRITES)
			sprite = STARTSPRITES;

		spr = &spritetable[sprite-STARTSPRITES];
		block = (spritetype _seg *)grsegs[sprite];

		VWB_Bar (hx,hy,TEXTWIDTH,bottomy-hy,WHITE);

		PrintX=hx;
		PrintY=hy;
		US_PrintUnsigned (sprite);US_Print ("\n");PrintX=hx;
		US_PrintUnsigned (spr->width);US_Print ("\n");PrintX=hx;
		US_PrintUnsigned (spr->height);US_Print ("\n");PrintX=hx;
		US_PrintSigned (spr->orgx);US_Print ("\n");PrintX=hx;
		US_PrintSigned (spr->orgy);US_Print ("\n");PrintX=hx;
		US_PrintSigned (spr->xl);US_Print ("\n");PrintX=hx;
		US_PrintSigned (spr->yl);US_Print ("\n");PrintX=hx;
		US_PrintSigned (spr->xh);US_Print ("\n");PrintX=hx;
		US_PrintSigned (spr->yh);US_Print ("\n");PrintX=hx;
		US_PrintSigned (spr->shifts);US_Print ("\n");PrintX=hx;
		if (!block)
		{
			US_Print ("-----");
		}
		else
		{
			mem = block->sourceoffset[3]+5*block->planesize[3];
			mem = (mem+15)&(~15);		// round to paragraphs
			US_PrintUnsigned (mem);
		}

		oldsprite = sprite;
		do
		{
		//
		// draw the current shift, then wait for key
		//
			VWB_Bar(topx,hy,DISPWIDTH,bottomy-hy,WHITE);
			if (block)
			{
				PrintX = topx;
				PrintY = hy;
				US_Print ("Shift:");
				US_PrintUnsigned (shift);
				US_Print ("\n");
				VWB_DrawSprite (topx+16+shift*2,PrintY,sprite);
			}

			VW_UpdateScreen();

			scan = IN_WaitForKey ();

			switch (scan)
			{
			case sc_UpArrow:
				sprite++;
				break;
			case sc_DownArrow:
				sprite--;
				break;
			case sc_LeftArrow:
				if (--shift == -1)
					shift = 3;
				break;
			case sc_RightArrow:
				if (++shift == 4)
					shift = 0;
				break;
			case sc_Escape:
				return;
			}

		} while (sprite == oldsprite);

  } while (1);


}

#endif


/*
================
=
= DebugKeys
=
================
*/
int DebugKeys (void)
{}

//===========================================================================

/*
==========================
=
= ShutdownId
=
= Shuts down all ID_?? managers
=
==========================
*/

void ShutdownId (void)
{
  US_Shutdown ();
  SD_Shutdown ();
  IN_Shutdown ();
  RF_Shutdown ();
  VW_Shutdown ();
  CA_Shutdown ();
  MM_Shutdown ();
}

//===========================================================================

/*
==========================
=
= Quit
=
==========================
*/

void Quit (char *error)
{
  ShutdownId ();
  if (error && *error)
  {
	clrscr();
	puts(error);
	puts("\n");
	exit(1);
  }
	exit (0);
}

//===========================================================================

////////////////////////////////////////////////////////////////////////////

#ifdef __WATCOMC__
void segatesuto()
{
	__segment screen;
	char __based( void ) * scrptr;

	screen = 0xB800;
	scrptr = 0;
	printf( "Top left character is '%c'.\n", *(screen:>scrptr) );
// 	printf("Next string is: [");
// 	while(*scrptr<16)
// 	{
// 		printf("%c", *(screen:>scrptr));
// 		//printf("\b");
// 		scrptr++;
//
// 	}
// 	printf("]\n");
//	KEYP
}
#endif
long int
filesize(FILE *fp)
{
	long int save_pos, size_of_file;

	save_pos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size_of_file = ftell(fp);
	fseek(fp, save_pos, SEEK_SET);
	return(size_of_file);
}

#define FILENAME_1 "id_mm.c"
#define FILENAME_2 "id_mm.h"

//===========================================================================//

//=======================================//

//	main

//=======================================//
void
main(int argc, char *argv[])
{
								#ifdef INITBBUF
	INITBBUF
								#endif

	char bakapee1[64] = FILENAME_1;
	char bakapee2[64] = FILENAME_2;

		#ifdef __BORLANDC__
			argc=argc;
		#endif

								#ifdef PRINTBBDUMP
								//0000PRINTBB; KEYP
								#endif

	if(argv[1]){ strcpy(bakapee1, argv[1]);//bakapee1[] = *argv[1];
	if(argv[2]) strcpy(bakapee2, argv[2]); }//bakapee2[] = argv[2]; }

	printf("bakapee1[%s]\n", bakapee1);
	printf("bakapee2[%s]\n", bakapee2);
								#ifdef EXMMVERBOSE__
	printf("coreleft():		%u\n", coreleft());
	printf("farcoreleft():		%ld\n", farcoreleft());
								#endif
	printf("stackavail()=%u\n", stackavail());
	KEYP

	IN_Startup();// printf("IN\n"); KEYP
	MM_Startup();// printf("MM\n"); KEYP
	//CA_Startup(); printf("CA\n"); KEYP

								#ifdef PRINTBBDUMP
								//0000
PRINTBB; KEYP
								#endif

	//IN_Default(0,player[0],ctrl_Keyboard1, &gvar);
	//IN_SetControlType(&gvar.player[0],ctrl_Keyboard1);

	{
	byte w;	word baka;
	w=0;
								#ifdef FILEREADLOAD
								#ifdef FILEREAD
	for(;w<2;w++)
	{
	//	printf("size of big buffer~=%u\n", _bmsize(segu, BBUF));
		if(w>0)
		{
			printf("======================================read=====================================\n");
			if(CA_ReadFile(bakapee2, BBUFPTR)) baka=1; else baka=0;
			printf("====================================read end===================================\n");
		}
								#endif //FILEREAD
		if(w==0)
		{
			printf("======================================load=====================================\n");
			if(CA_LoadFile(bakapee1, BBUFPTR)) baka=1; else baka=0;
			printf("====================================load end===================================\n");
		}
								#ifdef BUFFDUMP
		{
			size_t file_s;
			FILE *fh;

			if(!w)	fh = fopen(bakapee1, "r");
			else	fh = fopen(bakapee2, "r");
			file_s = filesize(fh);
			fclose(fh);
		printf("contents of the buffer\n[\n%.*s\n]\n", file_s, BBUFSTRING);
#if 0
//0000
// 			mmblocktype far *scan;
// 			scan = gvar.mm.mmhead;
// 			while (scan->useptr != &BBUFNAME && scan)
// 			{
// 				scan = scan->next;
// 			}
// 			printf("\n	%Fp	%Fp\n", scan->useptr, &BBUFNAME);
			printf("\nstrlen of buffer = %zu\n", strlen(BBUFSTRING));
			printf("length of buffer = %zu\n", file_s);
//			printf("length of buffer = %lu\n", scan->length);
#endif
		}
								#endif
								#ifdef PRINTBBDUMP
		PRINTBB;
								#endif

		//printf("dark purple = purgable\n");
		//printf("medium blue = non purgable\n");
		//printf("red = locked\n");
	//	KEYP
	//	DebugMemory_(&gvar, 1);
		if(baka) printf("\nyay!\n");
		else printf("\npoo!\n");
								#ifdef BUFFDUMPPAUSE
		KEYP
								#endif
								#ifdef FILEREAD
	}
								#endif
								#endif	//filereadload
	}


	//MM_ShowMemory();
#if 0
	{
	boolean			done;
	ScanCode		scan;
	for (done = false;!done;)
	{
		while (!(scan = gvar.in.inst->LastScan))
		{}
	//			SD_Poll();

		IN_ClearKey(scan);
		switch (scan)
		{
//			case sc_Space:
//				MM_ShowMemory(&gvar);
//			break;
//#ifdef __WATCOMC__
			case sc_O:
				VL_modexPalScramble(&gvar.video.palette); modexpdump(&gvar.video.page);
			break;
			case sc_P:
				modexpdump(&gvar.video.page[0]);
			break;
			case sc_V:
				VL_PatternDraw(&gvar.video, 0, 1, 1);
			break;
//#endif
			default:
			case sc_Escape:
				done = true;
			break;
		}
	}
}
#endif

	//++++DebugMemory(1);
	//++++MM_DumpData();
	KEYP
	//++++MM_Report();
	//printf("bakapee1=%s\n", bakapee1);
	//printf("bakapee2=%s\n", bakapee2);
	MM_FreePtr(BBUFPTR);

	//CA_Shutdown();
	MM_Shutdown();
	IN_Shutdown();
/*	printf("========================================\n");
	printf("near=	%Fp ",	mm.nearheap);
	printf("far=	%Fp",			mm.farheap);
	printf("\n");
	printf("&near=	%Fp ",	&(mm.nearheap));
	printf("&far=	%Fp",		&(mm.farheap));
	printf("\n");
								#ifdef EXMMVERBOSE
	printf("bigb=	%Fp ",	BBUF);
	//printf("bigbr=	%04x",	BBUF);
	//printf("\n");
	printf("&bigb=%Fp ",		BBUFPTR);
	//printf("&bigb=%04x",		BBUFPTR);
	printf("\n");
								#endif
	printf("========================================\n");*/

								#ifdef EXMMVERBOSE__
	printf("coreleft():		%u\n", coreleft());
	printf("farcoreleft():		%ld\n", farcoreleft());
								#endif
#ifdef __WATCOMC__
//this is far	printf("Total free:			%lu\n", (dword)(HC_GetFreeSize()));
//super buggy	printf("HC_coreleft():			%u\n", HC_coreleft());
//	printf("HC_farcoreleft():			%lu\n", (dword)HC_farcoreleft());
	//printf("HC_GetNearFreeSize():		%u\n", HC_GetNearFreeSize());
	//printf("HC_GetFarFreeSize():			%lu\n", (dword)HC_GetFarFreeSize());
//	segatesuto();
#endif
#ifdef __BORLANDC__
//	printf("HC_coreleft:			%lu\n", (dword)HC_coreleft());
//	printf("HC_farcoreleft:			%lu\n", (dword)HC_farcoreleft());
//	printf("HC_Newfarcoreleft():		%lu\n", (dword)HC_Newfarcoreleft());
#endif
	//++++HC_heapdump(&gvar);
	printf("Project 16 ");
#ifdef __WATCOMC__
	printf("exmmtest");
#endif
#ifdef __BORLANDC__
	printf("bcexmm");
#endif
	printf(".exe. This is just a test file!\n");
	//printf("version %s\n", VERSION);

//end of program


#if defined(__DEBUG__) && ( defined(__DEBUG_PM__) || defined(__DEBUG_MM__) )
#ifdef __DEBUG_MM__
	printf("debugmm: %u\t", dbg_debugmm);
#endif
#ifdef __DEBUG_PM__
	printf("debugpm: %u", dbg_debugpm);
#endif
	printf("\n");
#endif
//	printf("curr_mode=%u\n", gvar.video.curr_mode);
//	VL_PrintmodexmemInfo(&gvar.video);
	//printf("old_mode=%u	VL_Started=%u", gvar.video.old_mode, gvar.video.VL_Started);
	//printf("based core left:			%lu\n", (dword)_basedcoreleft());
	//printf("huge core left:			%lu\n", (dword)_hugecoreleft());
}
