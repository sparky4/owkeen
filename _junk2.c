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
//	unsigned bit,byte,node,code;
	unsigned sourceseg,sourceoff,destseg,destoff,endoff;
	huffnode *nodeon, *headptr;

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
