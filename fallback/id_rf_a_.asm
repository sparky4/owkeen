.8087
		PUBLIC	RFL_NewTile_
		PUBLIC	RFL_UpdateTiles_
		PUBLIC	RFL_MaskForegroundTiles_
		EXTRN	_updatestart:BYTE
		EXTRN	_updatemapofs:BYTE
		EXTRN	_originmap:BYTE
		EXTRN	_blockstarts:BYTE
		EXTRN	_masterofs:BYTE
		EXTRN	_mapsegs:BYTE
		EXTRN	_screenseg:BYTE
		EXTRN	_tilecache:BYTE
		EXTRN	_grsegs:BYTE
		EXTRN	_updateptr:BYTE
		EXTRN	_bufferofs:BYTE
		EXTRN	_tinf:BYTE
		EXTRN	_screenstart:BYTE
DGROUP		GROUP	_DATA
ID_RF_A_TEXT		SEGMENT	WORD PUBLIC USE16 'CODE'
		ASSUME CS:ID_RF_A_TEXT, DS:DGROUP, SS:DGROUP
L$1:
	add		byte ptr [bx+si],al
_RFL_NewTile:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		di,word ptr 6[bp]
	mov		bx,word ptr DGROUP:_updatestart
	mov		byte ptr [bx+di],1
	mov		bx,word ptr DGROUP:_updatestart+2
	mov		byte ptr [bx+di],1
	shl		di,1
	mov		si,word ptr DGROUP:_updatemapofs[di]
	add		si,word ptr DGROUP:_originmap
	mov		di,word ptr DGROUP:_blockstarts[di]
	add		di,word ptr DGROUP:_masterofs
	mov		word ptr cs:L$1,di
	mov		es,word ptr DGROUP:_mapsegs+2
	mov		bx,word ptr es:[si]
	mov		es,word ptr DGROUP:_mapsegs
	mov		si,word ptr es:[si]
	mov		es,word ptr DGROUP:_screenseg
	mov		dx,3c4H
	or		bx,bx
	je		L$2
	jmp		near ptr L$5
L$2:
	mov		bx,3eH
	shl		si,1
	mov		ax,word ptr DGROUP:_tilecache[si]
	or		ax,ax
	je		L$3
	mov		si,ax
	mov		ax,0f02H
	out		dx,ax
	mov		dx,3ceH
	mov		ax,105H
	out		dx,ax
	mov		di,word ptr cs:L$1
	mov		ds,word ptr DGROUP:_screenseg
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	add		si,bx
	add		di,bx
	movsb
	movsb
	xor		ah,ah
	out		dx,ax
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
L$3:
	mov		ax,word ptr cs:L$1
	mov		word ptr DGROUP:_tilecache[si],ax
	mov		ds,word ptr DGROUP:_grsegs+2e4H[si]
	xor		si,si
	mov		ax,102H
	mov		cx,4
L$4:
	mov		dx,3c4H
	out		dx,ax
	mov		di,word ptr cs:L$1
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	add		di,bx
	movsw
	shl		ah,1
	loop		L$4
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
L$5:
	cli
	shl		bx,1
	mov		ss,word ptr DGROUP:_grsegs+0e24H[bx]
	shl		si,1
	mov		ds,word ptr DGROUP:_grsegs+2e4H[si]
	xor		si,si
	mov		ax,102H
	mov		di,word ptr cs:L$1
L$6:
	out		dx,ax
	mov		bx,word ptr [si]
	and		bx,word ptr ss:[0]
	or		bx,word ptr ss:20H[si]
	mov		word ptr es:[di],bx
	mov		bx,word ptr 2[si]
	and		bx,word ptr ss:[2]
	or		bx,word ptr ss:22H[si]
	mov		word ptr es:40H[di],bx
	mov		bx,word ptr 4[si]
	and		bx,word ptr ss:[4]
	or		bx,word ptr ss:24H[si]
	mov		word ptr es:80H[di],bx
	mov		bx,word ptr 6[si]
	and		bx,word ptr ss:[6]
	or		bx,word ptr ss:26H[si]
	mov		word ptr es:0c0H[di],bx
	mov		bx,word ptr 8[si]
	and		bx,word ptr ss:[8]
	or		bx,word ptr ss:28H[si]
	mov		word ptr es:100H[di],bx
	mov		bx,word ptr 0aH[si]
	and		bx,word ptr ss:[0aH]
	or		bx,word ptr ss:2aH[si]
	mov		word ptr es:140H[di],bx
	mov		bx,word ptr 0cH[si]
	and		bx,word ptr ss:[0cH]
	or		bx,word ptr ss:2cH[si]
	mov		word ptr es:180H[di],bx
	mov		bx,word ptr 0eH[si]
	and		bx,word ptr ss:[0eH]
	or		bx,word ptr ss:2eH[si]
	mov		word ptr es:1c0H[di],bx
	mov		bx,word ptr 10H[si]
	and		bx,word ptr ss:[10H]
	or		bx,word ptr ss:30H[si]
	mov		word ptr es:200H[di],bx
	mov		bx,word ptr 12H[si]
	and		bx,word ptr ss:[12H]
	or		bx,word ptr ss:32H[si]
	mov		word ptr es:240H[di],bx
	mov		bx,word ptr 14H[si]
	and		bx,word ptr ss:[14H]
	or		bx,word ptr ss:34H[si]
	mov		word ptr es:280H[di],bx
	mov		bx,word ptr 16H[si]
	and		bx,word ptr ss:[16H]
	or		bx,word ptr ss:36H[si]
	mov		word ptr es:2c0H[di],bx
	mov		bx,word ptr 18H[si]
	and		bx,word ptr ss:[18H]
	or		bx,word ptr ss:38H[si]
	mov		word ptr es:300H[di],bx
	mov		bx,word ptr 1aH[si]
	and		bx,word ptr ss:[1aH]
	or		bx,word ptr ss:3aH[si]
	mov		word ptr es:340H[di],bx
	mov		bx,word ptr 1cH[si]
	and		bx,word ptr ss:[1cH]
	or		bx,word ptr ss:3cH[si]
	mov		word ptr es:380H[di],bx
	mov		bx,word ptr 1eH[si]
	and		bx,word ptr ss:[1eH]
	or		bx,word ptr ss:3eH[si]
	mov		word ptr es:3c0H[di],bx
	add		si,20H
	shl		ah,1
	cmp		ah,10H
	je		L$7
	jmp		near ptr L$6
L$7:
	mov		ax,DGROUP
	mov		ss,ax
	sti
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
_RFL_UpdateTiles:
	push		si
	push		di
	push		bp
	jmp		L$9
L$8:
	pop		bp
	pop		di
	pop		si
	retf
L$9:
	mov		di,word ptr DGROUP:_updateptr
	mov		bp,135H
	add		bp,di
	push		di
	mov		cx,0ffffH
L$10:
	pop		di
	mov		ax,ss
	mov		es,ax
	mov		ds,ax
	mov		al,1
	repne scasb
	cmp		di,bp
	je		L$8
	cmp		byte ptr [di],al
	jne		L$11
	jmp		near ptr L$12
	nop
L$11:
	inc		di
	push		di
	sub		di,word ptr DGROUP:_updateptr
	shl		di,1
	mov		di,word ptr DGROUP:_blockstarts-4[di]
	mov		si,di
	add		di,word ptr DGROUP:_bufferofs
	add		si,word ptr DGROUP:_masterofs
	mov		dx,3eH
	mov		ax,word ptr DGROUP:_screenseg
	mov		ds,ax
	mov		es,ax
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	add		si,dx
	add		di,dx
	movsb
	movsb
	jmp		near ptr L$10
	nop
L$12:
	mov		dx,di
	inc		di
	repe scasb
	push		di
	mov		bx,di
	sub		bx,dx
	shl		bx,1
	mov		di,dx
	sub		di,word ptr DGROUP:_updateptr
	shl		di,1
	mov		di,word ptr DGROUP:_blockstarts-2[di]
	mov		si,di
	add		di,word ptr DGROUP:_bufferofs
	add		si,word ptr DGROUP:_masterofs
	mov		dx,40H
	sub		dx,bx
	mov		ax,word ptr DGROUP:_screenseg
	mov		ds,ax
	mov		es,ax
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	add		si,dx
	add		di,dx
	mov		cx,bx
	rep movsb
	dec		cx
	jmp		near ptr L$10
_RFL_MaskForegroundTiles:
	push		si
	push		di
	push		bp
	jmp		L$14
L$13:
	pop		bp
	pop		di
	pop		si
	retf
L$14:
	mov		di,word ptr DGROUP:_updateptr
	mov		bp,136H
	add		bp,di
	push		di
	mov		cx,0ffffH
L$15:
	mov		ax,ss
	mov		es,ax
	mov		al,3
	pop		di
	repne scasb
	cmp		di,bp
	je		L$13
	push		di
	sub		di,word ptr DGROUP:_updateptr
	shl		di,1
	mov		si,word ptr DGROUP:_updatemapofs-2[di]
	add		si,word ptr DGROUP:_originmap
	mov		es,word ptr DGROUP:_mapsegs+2
	mov		si,word ptr es:[si]
	or		si,si
	je		L$15
	mov		bx,si
	add		bx,24c4H
	mov		es,word ptr DGROUP:_tinf
	test		byte ptr es:[bx],80H
	je		L$15
	mov		byte ptr DGROUP:L$18,1
	mov		byte ptr DGROUP:L$19,0
	mov		di,word ptr DGROUP:_blockstarts-2[di]
	add		di,word ptr DGROUP:_bufferofs
	mov		word ptr cs:L$1,di
	mov		es,word ptr DGROUP:_screenseg
	shl		si,1
	mov		ds,word ptr DGROUP:_grsegs+0e24H[si]
	mov		bx,20H
L$16:
	mov		dx,3c4H
	mov		al,2
	mov		ah,byte ptr ss:L$18
	out		dx,ax
	mov		dx,3ceH
	mov		al,4
	mov		ah,byte ptr ss:L$19
	out		dx,ax
	xor		si,si
	mov		di,word ptr cs:L$1
	mov		cx,word ptr es:[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:[di],cx
	mov		cx,word ptr es:40H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:40H[di],cx
	mov		cx,word ptr es:80H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:80H[di],cx
	mov		cx,word ptr es:0c0H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:0c0H[di],cx
	mov		cx,word ptr es:100H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:100H[di],cx
	mov		cx,word ptr es:140H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:140H[di],cx
	mov		cx,word ptr es:180H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:180H[di],cx
	mov		cx,word ptr es:1c0H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:1c0H[di],cx
	mov		cx,word ptr es:200H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:200H[di],cx
	mov		cx,word ptr es:240H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:240H[di],cx
	mov		cx,word ptr es:280H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:280H[di],cx
	mov		cx,word ptr es:2c0H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:2c0H[di],cx
	mov		cx,word ptr es:300H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:300H[di],cx
	mov		cx,word ptr es:340H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:340H[di],cx
	mov		cx,word ptr es:380H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:380H[di],cx
	mov		cx,word ptr es:3c0H[di]
	and		cx,word ptr [si]
	or		cx,word ptr [bx+si]
	inc		si
	inc		si
	mov		word ptr es:3c0H[di],cx
	add		bx,20H
	inc		byte ptr ss:L$19
	shl		byte ptr ss:L$18,1
	cmp		byte ptr ss:L$18,10H
	je		L$17
	jmp		near ptr L$16
L$17:
	mov		ax,ss
	mov		ds,ax
	mov		cx,0ffffH
	jmp		near ptr L$15
ID_RF_A_TEXT		ENDS
_DATA		SEGMENT	WORD PUBLIC USE16 'DATA'
L$18:
    DB	0
L$19:
    DB	0

_DATA		ENDS
		END
