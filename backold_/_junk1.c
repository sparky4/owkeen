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
		mov	bx,[headptr]

		mov	si,[sourceoff]
		mov	di,[destoff]
		mov	es,[destseg]
		mov	ds,[sourceseg]
		mov	ax,[endoff]

		mov	ch,[si]				// load first byte
		inc	si
		mov	cl,1

expandshort:
		test	ch,cl			// bit set?
		jnz	bit1short
		mov	dx,[ss:bx]			// take bit0 path from node
		shl	cl,1				// advance to next bit position
		jc	newbyteshort
		jnc	sourceupshort

bit1short:
		mov	dx,[ss:bx+2]		// take bit1 path
		shl	cl,1				// advance to next bit position
		jnc	sourceupshort

newbyteshort:
		mov	ch,[si]				// load next byte
		inc	si
		mov	cl,1				// back to first bit

sourceupshort:
		or	dh,dh				// if dx<256 its a byte, else move node
		jz	storebyteshort
		mov	bx,dx				// next node = (huffnode *)code
		jmp	expandshort

storebyteshort:
		mov	[es:di],dl
		inc	di					// write a decopmpressed byte out
		mov	bx,[headptr]		// back to the head node for next bit

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
		mov	bx,[headptr]
		mov	cl,1

		mov	si,[sourceoff]
		mov	di,[destoff]
		mov	es,[destseg]
		mov	ds,[sourceseg]

		lodsb			// load first byte

expand:
		test	al,cl		// bit set?
		jnz	bit1
		mov	dx,[ss:bx]	// take bit0 path from node
		jmp	gotcode
bit1:
		mov	dx,[ss:bx+2]	// take bit1 path

gotcode:
		shl	cl,1		// advance to next bit position
		jnc	sourceup
		lodsb
		cmp	si,0x10		// normalize ds:si
			jb	sinorm
		mov	cx,ds
		inc	cx
		mov	ds,cx
		xor	si,si
sinorm:
		mov	cl,1		// back to first bit

sourceup:
		or	dh,dh		// if dx<256 its a byte, else move node
		jz	storebyte
		mov	bx,dx		// next node = (huffnode *)code
		jmp	expand

storebyte:
		mov	[es:di],dl
		inc	di		// write a decopmpressed byte out
		mov	bx,[headptr]	// back to the head node for next bit

		cmp	di,0x10		// normalize es:di
			jb	dinorm
		mov	dx,es
		inc	dx
		mov	es,dx
		xor	di,di
dinorm:

		sub	[WORD PTR ss:length],1
		jnc	expand
			dec	[WORD PTR ss:length+2]
		jns	expand		// when length = ffff ffff, done

	}

		mov	ax,ss
		mov	ds,ax
	}
}
