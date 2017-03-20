.8087
		PUBLIC	VW_Plot_
		PUBLIC	VW_Vlin_
		PUBLIC	VW_DrawTile8_
		PUBLIC	VW_MaskBlock_
		PUBLIC	VW_ScreenToScreen_
		PUBLIC	VW_MemToScreen_
		PUBLIC	VW_ScreenToMem_
		PUBLIC	VWL_UpdateScreenBlocks_
		PUBLIC	VW_SetScreen_
		PUBLIC	VW_DrawPropString_
		PUBLIC	VW_WaitVBL_
		PUBLIC	VW_VideoID_
		PUBLIC	_shifttabletable
		PUBLIC	_px
		PUBLIC	_py
		PUBLIC	_pdrawmode
		PUBLIC	_fontcolor
		PUBLIC	_bufferwidth
		PUBLIC	_bufferheight
		EXTRN	_screenseg:BYTE
		EXTRN	_bufferofs:BYTE
		EXTRN	_ylookup:BYTE
		EXTRN	_linewidth:BYTE
		EXTRN	_grsegs:BYTE
		EXTRN	_updateptr:BYTE
		EXTRN	_blockstarts:BYTE
		EXTRN	_displayofs:BYTE
		EXTRN	_panadjust:BYTE
	;	EXTRN	fontspace:BYTE
	;	EXTRN	drawofs:BYTE
DGROUP		GROUP	_DATA
ID_VW_A_TEXT		SEGMENT	WORD PUBLIC USE16 'CODE'
		ASSUME CS:ID_VW_A_TEXT, DS:DGROUP, SS:DGROUP
_VW_Plot:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		es,word ptr DGROUP:_screenseg
	mov		dx,3c4H
	mov		ax,0f02H
	out		dx,ax
	mov		dx,3ceH
	mov		ax,205H
	out		dx,ax
	mov		di,word ptr DGROUP:_bufferofs
	mov		bx,word ptr 8[bp]
	shl		bx,1
	add		di,word ptr DGROUP:_ylookup[bx]
	mov		bx,word ptr 6[bp]
	mov		ax,bx
	shr		ax,1
	shr		ax,1
	shr		ax,1
	add		di,ax
	and		bx,7
	mov		ah,byte ptr DGROUP:L$109[bx]
	mov		al,8
	out		dx,ax
	mov		bl,byte ptr 0aH[bp]
	xchg		byte ptr es:[di],bl
	mov		dx,3ceH
	mov		ah,0ffH
	out		dx,ax
	mov		ax,5
	out		dx,ax
	pop		di
	pop		si
	pop		bp
	retf
_VW_Vlin:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		es,word ptr DGROUP:_screenseg
	mov		dx,3c4H
	mov		ax,0f02H
	out		dx,ax
	mov		dx,3ceH
	mov		ax,205H
	out		dx,ax
	mov		di,word ptr DGROUP:_bufferofs
	mov		bx,word ptr 6[bp]
	shl		bx,1
	add		di,word ptr DGROUP:_ylookup[bx]
	mov		bx,word ptr 0aH[bp]
	mov		ax,bx
	shr		ax,1
	shr		ax,1
	shr		ax,1
	add		di,ax
	and		bx,7
	mov		ah,byte ptr DGROUP:L$109[bx]
	mov		al,8
	out		dx,ax
	mov		cx,word ptr 8[bp]
	sub		cx,word ptr 6[bp]
	inc		cx
	mov		bh,byte ptr 0cH[bp]
	mov		dx,word ptr DGROUP:_linewidth
L$1:
	mov		bl,bh
	xchg		byte ptr es:[di],bl
	add		di,dx
	loop		L$1
	mov		dx,3ceH
	mov		ah,0ffH
	out		dx,ax
	mov		ax,5
	out		dx,ax
	pop		di
	pop		si
	pop		bp
	retf
_VW_DrawTile8:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		es,word ptr DGROUP:_screenseg
	mov		di,word ptr DGROUP:_bufferofs
	add		di,word ptr 6[bp]
	mov		bx,word ptr 8[bp]
	shl		bx,1
	add		di,word ptr DGROUP:_ylookup[bx]
	mov		word ptr ss:L$99,di
	mov		bx,word ptr DGROUP:_linewidth
	dec		bx
	mov		si,word ptr 0aH[bp]
	shl		si,1
	shl		si,1
	shl		si,1
	shl		si,1
	shl		si,1
	mov		ds,word ptr DGROUP:_grsegs+2e0H
	mov		cx,4
	mov		ah,1
	mov		dx,3c4H
	mov		al,2
L$2:
	out		dx,ax
	shl		ah,1
	mov		di,word ptr ss:L$99
	movsb
	add		di,bx
	movsb
	add		di,bx
	movsb
	add		di,bx
	movsb
	add		di,bx
	movsb
	add		di,bx
	movsb
	add		di,bx
	movsb
	add		di,bx
	movsb
	loop		L$2
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
_VW_MaskBlock:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		es,word ptr DGROUP:_screenseg
	mov		byte ptr DGROUP:L$97,1
	mov		byte ptr DGROUP:L$98,0
	mov		di,word ptr 0cH[bp]
	mov		dx,word ptr DGROUP:_linewidth
	sub		dx,word ptr 0cH[bp]
	mov		word ptr DGROUP:L$100,dx
	mov		bx,word ptr 10H[bp]
	cmp		di,0aH
	jbe		L$3
	mov		word ptr DGROUP:L$111,offset L$8
	jmp		L$4
	nop
L$3:
	mov		cx,word ptr 0aH[bp]
	shr		cx,1
	rcl		di,1
	shl		di,1
	mov		ax,word ptr DGROUP:L$110[di]
	mov		word ptr DGROUP:L$111,ax
L$4:
	mov		ds,word ptr 6[bp]
L$5:
	mov		dx,3c4H
	mov		al,2
	mov		ah,byte ptr ss:L$97
	out		dx,ax
	mov		dx,3ceH
	mov		al,4
	mov		ah,byte ptr ss:L$98
	out		dx,ax
	mov		si,word ptr 8[bp]
	mov		di,word ptr 0aH[bp]
	mov		cx,word ptr 0eH[bp]
	mov		dx,word ptr ss:L$100
	jmp		word ptr ss:L$111
L$6:
	add		bx,word ptr 10H[bp]
	inc		byte ptr ss:L$98
	shl		byte ptr ss:L$97,1
	cmp		byte ptr ss:L$97,10H
	jne		L$5
L$7:
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
L$8:
	mov		dx,cx
L$9:
	mov		cx,word ptr 0cH[bp]
L$10:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	loop		L$10
	add		di,word ptr ss:L$100
	dec		dx
	jne		L$9
	jmp		L$6
L$11:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$11
	jmp		L$6
	nop
L$12:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$12
	jmp		L$6
L$13:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$13
	jmp		L$6
L$14:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$14
	jmp		near ptr L$6
L$15:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$15
	jmp		near ptr L$6
L$16:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$16
	jmp		near ptr L$6
	nop
L$17:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$17
	jmp		near ptr L$6
	nop
L$18:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$18
	jmp		near ptr L$6
L$19:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$19
	jmp		near ptr L$6
L$20:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$20
	jmp		near ptr L$6
	nop
L$21:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$21
	jmp		near ptr L$6
	nop
L$22:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$22
	jmp		near ptr L$6
L$23:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$23
	jmp		near ptr L$6
L$24:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$24
	jmp		near ptr L$6
	nop
L$25:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$25
	jmp		near ptr L$6
	nop
L$26:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$26
	jmp		near ptr L$6
L$27:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$27
	jmp		near ptr L$6
L$28:
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	add		di,dx
	loop		L$28
	jmp		near ptr L$6
	nop
L$29:
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		ax,word ptr es:[di]
	and		ax,word ptr [si]
	or		ax,word ptr [bx+si]
	inc		si
	inc		si
	stosw
	mov		al,byte ptr es:[di]
	and		al,byte ptr [si]
	or		al,byte ptr [bx+si]
	inc		si
	stosb
	add		di,dx
	loop		L$29
	jmp		near ptr L$6
_VW_ScreenToScreen:
	push		bp
	mov		bp,sp
	push		si
	push		di
	pushf
	cli
	mov		dx,3c4H
	mov		ax,0f02H
	out		dx,ax
	mov		dx,3ceH
	mov		ax,105H
	out		dx,ax
	popf
	mov		bx,word ptr DGROUP:_linewidth
	sub		bx,word ptr 0aH[bp]
	mov		ax,word ptr DGROUP:_screenseg
	mov		es,ax
	mov		ds,ax
	mov		si,word ptr 6[bp]
	mov		di,word ptr 8[bp]
	mov		dx,word ptr 0cH[bp]
	mov		ax,word ptr 0aH[bp]
L$30:
	mov		cx,ax
	rep movsb
	add		si,bx
	add		di,bx
	dec		dx
	jne		L$30
	mov		dx,3ceH
	mov		ax,5
	out		dx,ax
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
_VW_MemToScreen:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		es,word ptr DGROUP:_screenseg
	mov		bx,word ptr DGROUP:_linewidth
	sub		bx,word ptr 0aH[bp]
	mov		ds,word ptr 6[bp]
	xor		si,si
	xor		di,di
	shr		word ptr 0aH[bp],1
	rcl		di,1
	mov		ax,word ptr 8[bp]
	shr		ax,1
	rcl		di,1
	shl		di,1
	mov		ax,102H
	jmp		word ptr ss:L$112[di]
L$31:
	mov		dx,3c4H
	out		dx,ax
	mov		di,word ptr 8[bp]
	mov		dx,word ptr 0cH[bp]
L$32:
	mov		cx,word ptr 0aH[bp]
	rep movsw
	add		di,bx
	dec		dx
	jne		L$32
	shl		ah,1
	cmp		ah,10H
	jne		L$31
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
L$33:
	mov		dx,3c4H
	out		dx,ax
	mov		di,word ptr 8[bp]
	mov		dx,word ptr 0cH[bp]
L$34:
	mov		cx,word ptr 0aH[bp]
	rep movsw
	movsb
	add		di,bx
	dec		dx
	jne		L$34
	shl		ah,1
	cmp		ah,10H
	jne		L$33
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
L$35:
	dec		word ptr 0aH[bp]
L$36:
	mov		dx,3c4H
	out		dx,ax
	mov		di,word ptr 8[bp]
	mov		dx,word ptr 0cH[bp]
L$37:
	movsb
	mov		cx,word ptr 0aH[bp]
	rep movsw
	movsb
	add		di,bx
	dec		dx
	jne		L$37
	shl		ah,1
	cmp		ah,10H
	jne		L$36
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
L$38:
	mov		dx,3c4H
	out		dx,ax
	mov		di,word ptr 8[bp]
	mov		dx,word ptr 0cH[bp]
L$39:
	movsb
	mov		cx,word ptr 0aH[bp]
	rep movsw
	add		di,bx
	dec		dx
	jne		L$39
	shl		ah,1
	cmp		ah,10H
	jne		L$38
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
_VW_ScreenToMem:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		es,word ptr 8[bp]
	mov		bx,word ptr DGROUP:_linewidth
	sub		bx,word ptr 0aH[bp]
	mov		ds,word ptr DGROUP:_screenseg
	mov		ax,4
	xor		di,di
L$40:
	mov		dx,3ceH
	out		dx,ax
	mov		si,word ptr 6[bp]
	mov		dx,word ptr 0cH[bp]
L$41:
	mov		cx,word ptr 0aH[bp]
	rep movsb
	add		si,bx
	dec		dx
	jne		L$41
	inc		ah
	cmp		ah,4
	jne		L$40
	mov		ax,ss
	mov		ds,ax
	pop		di
	pop		si
	pop		bp
	retf
_VWL_UpdateScreenBlocks:
	push		si
	push		di
	push		bp
	jmp		L$43
L$42:
	mov		dx,3ceH
	mov		ax,5
	out		dx,ax
	xor		ax,ax
	mov		cx,9aH
	mov		di,word ptr DGROUP:_updateptr
	rep stosw
	pop		bp
	pop		di
	pop		si
	retf
L$43:
	mov		dx,3c4H
	mov		ax,0f02H
	out		dx,ax
	mov		dx,3ceH
	mov		ax,105H
	out		dx,ax
	mov		di,word ptr DGROUP:_updateptr
	mov		bp,di
	add		bp,135H
	push		di
	mov		cx,0ffffH
L$44:
	pop		di
	mov		ax,ss
	mov		es,ax
	mov		ds,ax
	mov		al,1
	repne scasb
	cmp		di,bp
	jae		L$42
	cmp		byte ptr [di],al
	jne		L$45
	jmp		near ptr L$46
L$45:
	inc		di
	push		di
	sub		di,word ptr DGROUP:_updateptr
	shl		di,1
	mov		di,word ptr DGROUP:_blockstarts-4[di]
	mov		si,di
	add		si,word ptr DGROUP:_bufferofs
	add		di,word ptr DGROUP:_displayofs
	mov		dx,word ptr DGROUP:_linewidth
	sub		dx,2
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
	jmp		near ptr L$44
L$46:
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
	add		si,word ptr DGROUP:_bufferofs
	add		di,word ptr DGROUP:_displayofs
	mov		dx,word ptr DGROUP:_linewidth
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
	jmp		near ptr L$44
_VW_SetScreen:
	push		bp
	mov		bp,sp
	mov		dx,3daH
	cli
L$47:
	in		al,dx
	test		al,1
	je		L$47
L$48:
	in		al,dx
	test		al,1
	jne		L$48
	mov		cx,word ptr 6[bp]
	mov		dx,3d4H
	mov		al,0cH
	out		dx,al
	inc		dx
	mov		al,ch
	out		dx,al
	dec		dx
	mov		al,0dH
	out		dx,al
	mov		al,cl
	inc		dx
	out		dx,al
	mov		dx,3daH
L$49:
	sti
	jmp		L$50
L$50:
	cli
	in		al,dx
	test		al,8
	je		L$49
	mov		dx,3c0H
	mov		al,33H
	out		dx,al
	jmp		L$51
L$51:
	mov		al,byte ptr 8[bp]
	out		dx,al
	sti
	pop		bp
	retf
L$52:
	mov		es,word ptr DGROUP:_screenseg
	mov		di,word ptr DGROUP:L$149
	mov		bx,word ptr DGROUP:_bufferwidth
	or		bx,bx
	jne		L$53
	ret
L$53:
	mov		ax,word ptr DGROUP:_linewidth
	sub		ax,bx
	mov		word ptr DGROUP:L$151,ax
	mov		ax,32H
	sub		ax,bx
	mov		word ptr DGROUP:L$150,ax
	mov		bx,word ptr DGROUP:_bufferheight
L$54:
	mov		cx,word ptr DGROUP:_bufferwidth
L$55:
	lodsb
	xchg		byte ptr es:[di],al
	inc		di
	loop		L$55
	add		si,word ptr DGROUP:L$150
	add		di,word ptr DGROUP:L$151
	dec		bx
	jne		L$54
	ret
L$56:
	mov		es,word ptr DGROUP:_grsegs+6
	mov		si,word ptr es:202H[bx]
	and		si,0ffH
	shl		bx,1
	mov		bx,word ptr es:2[bx]
	mov		di,word ptr DGROUP:L$148
	shl		di,1
	mov		bp,word ptr DGROUP:_shifttabletable[di]
	mov		di,offset DGROUP:L$115
	add		di,word ptr DGROUP:L$147
	mov		cx,word ptr DGROUP:L$148
	add		cx,si
	mov		ax,cx
	and		ax,7
	mov		word ptr DGROUP:L$148,ax
	mov		ax,cx
	shr		ax,1
	shr		ax,1
	shr		ax,1
	add		word ptr DGROUP:L$147,ax
	add		si,7
	shr		si,1
	shr		si,1
	shr		si,1
	shl		si,1
	mov		cx,word ptr es:[0]
	mov		dx,32H
	jmp		word ptr ss:L$152[si]
L$57:
	dec		dx
	nop
L$58:
	mov		al,byte ptr es:[bx]
	xor		ah,ah
	shl		ax,1
	mov		si,ax
	mov		ax,word ptr [bp+si]
	or		byte ptr [di],al
	inc		di
	mov		byte ptr [di],ah
	inc		bx
	add		di,dx
	loop		L$58
	ret
L$59:
	dec		dx
	dec		dx
L$60:
	mov		al,byte ptr es:[bx]
	xor		ah,ah
	shl		ax,1
	mov		si,ax
	mov		ax,word ptr [bp+si]
	or		byte ptr [di],al
	inc		di
	mov		byte ptr [di],ah
	inc		bx
	mov		al,byte ptr es:[bx]
	xor		ah,ah
	shl		ax,1
	mov		si,ax
	mov		ax,word ptr [bp+si]
	or		byte ptr [di],al
	inc		di
	mov		byte ptr [di],ah
	inc		bx
	add		di,dx
	loop		L$60
	ret
L$61:
	sub		dx,3
L$62:
	mov		al,byte ptr es:[bx]
	xor		ah,ah
	shl		ax,1
	mov		si,ax
	mov		ax,word ptr [bp+si]
	or		byte ptr [di],al
	inc		di
	mov		byte ptr [di],ah
	inc		bx
	mov		al,byte ptr es:[bx]
	xor		ah,ah
	shl		ax,1
	mov		si,ax
	mov		ax,word ptr [bp+si]
	or		byte ptr [di],al
	inc		di
	mov		byte ptr [di],ah
	inc		bx
	mov		al,byte ptr es:[bx]
	xor		ah,ah
	shl		ax,1
	mov		si,ax
	mov		ax,word ptr [bp+si]
	or		byte ptr [di],al
	inc		di
	mov		byte ptr [di],ah
	inc		bx
	add		di,dx
	loop		L$62
	ret
_VW_DrawPropString:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		al,0
	mov		byte ptr DGROUP:L$115,al
	mov		byte ptr DGROUP:L$116,al
	mov		byte ptr DGROUP:L$117,al
	mov		byte ptr DGROUP:L$118,al
	mov		byte ptr DGROUP:L$119,al
	mov		byte ptr DGROUP:L$120,al
	mov		byte ptr DGROUP:L$121,al
	mov		byte ptr DGROUP:L$122,al
	mov		byte ptr DGROUP:L$123,al
	mov		byte ptr DGROUP:L$124,al
	mov		byte ptr DGROUP:L$125,al
	mov		byte ptr DGROUP:L$126,al
	mov		byte ptr DGROUP:L$127,al
	mov		byte ptr DGROUP:L$128,al
	mov		byte ptr DGROUP:L$129,al
	mov		byte ptr DGROUP:L$130,al
	mov		byte ptr DGROUP:L$131,al
	mov		byte ptr DGROUP:L$132,al
	mov		byte ptr DGROUP:L$133,al
	mov		byte ptr DGROUP:L$134,al
	mov		byte ptr DGROUP:L$135,al
	mov		byte ptr DGROUP:L$136,al
	mov		byte ptr DGROUP:L$137,al
	mov		byte ptr DGROUP:L$138,al
	mov		byte ptr DGROUP:L$139,al
	mov		byte ptr DGROUP:L$140,al
	mov		byte ptr DGROUP:L$141,al
	mov		byte ptr DGROUP:L$142,al
	mov		byte ptr DGROUP:L$143,al
	mov		byte ptr DGROUP:L$144,al
	mov		byte ptr DGROUP:L$145,al
	mov		byte ptr DGROUP:L$146,al
	mov		ax,word ptr DGROUP:_px
	and		ax,7
	mov		word ptr DGROUP:L$148,ax
	mov		word ptr DGROUP:L$147,0
	mov		ax,word ptr 6[bp]
	mov		word ptr DGROUP:L$113,ax
	mov		ax,word ptr 8[bp]
	mov		word ptr DGROUP:L$114,ax
L$63:
	mov		es,word ptr DGROUP:L$114
	mov		bx,word ptr DGROUP:L$113
	inc		word ptr DGROUP:L$113
	mov		bx,word ptr es:[bx]
	xor		bh,bh
	or		bl,bl
	je		L$64
	call		near ptr L$56
	jmp		L$63
L$64:
	mov		bx,word ptr DGROUP:_py
	shl		bx,1
	mov		di,word ptr DGROUP:_ylookup[bx]
	add		di,word ptr DGROUP:_bufferofs
	add		di,word ptr DGROUP:_panadjust
	mov		ax,word ptr DGROUP:_px
	shr		ax,1
	shr		ax,1
	shr		ax,1
	add		di,ax
	mov		word ptr DGROUP:L$149,di
	mov		ax,word ptr DGROUP:L$147
	shl		ax,1
	shl		ax,1
	shl		ax,1
	or		ax,word ptr DGROUP:L$148
	add		word ptr DGROUP:_px,ax
	mov		dx,3ceH
	mov		al,3
	mov		ah,byte ptr DGROUP:_pdrawmode
	out		dx,ax
	mov		dx,3c4H
	mov		al,2
	mov		ah,byte ptr DGROUP:_fontcolor
	out		dx,ax
	mov		ax,word ptr DGROUP:L$147
	test		word ptr DGROUP:L$148,7
	je		L$65
	inc		ax
L$65:
	mov		word ptr DGROUP:_bufferwidth,ax
	mov		es,word ptr DGROUP:_grsegs+6
	mov		ax,word ptr es:[0]
	mov		word ptr DGROUP:_bufferheight,ax
	mov		si,offset DGROUP:L$115
	call		near ptr L$52
	mov		dx,3ceH
	mov		ax,3
	out		dx,ax
	mov		dx,3c4H
	mov		ax,0f02H
	out		dx,ax
	pop		di
	pop		si
	pop		bp
	retf
_VW_WaitVBL:
	push		bp
	mov		bp,sp
	mov		dx,3daH
	mov		cx,word ptr 6[bp]
L$66:
	in		al,dx
	test		al,8
	jne		L$66
L$67:
	in		al,dx
	test		al,8
	je		L$67
	loop		L$66
	pop		bp
	retf
L$68:
	add		byte ptr [bx+si],al
	add		byte ptr [bx+si],al
L$69:
	add		al,byte ptr [bp+di]
	add		word ptr [bp+si],ax
	add		ax,word ptr [bx+di]
L$70:
	add		byte ptr [bx+si],al
	add		word ptr [bx+di],ax
	add		al,byte ptr [bp+si]
	add		byte ptr [bx+si],al
	add		ax,word ptr [bp+di]
	add		ax,word ptr [bx+di]
	add		byte ptr [bx+si],al
	add		ax,504H
	add		ax,0
	add		al,3
	add		al,4
	add		al,5
L$71:
	add		word ptr 0aH[di],sp
L$72:
    DB	0
    DW	offset ID_VW_A_TEXT+0ab5H
L$73:
	add		cl,ah
    DB	0aH
L$74:
	add		cl,dh
    DB	0aH
_VW_VideoID:
	push		bp
	mov		bp,sp
	push		ds
	push		si
	push		di
	push		cs
	pop		ds
	mov		di,offset L$68
	mov		word ptr [di],0
	mov		word ptr 2[di],0
	mov		byte ptr L$73,1
	mov		byte ptr L$72,1
	mov		byte ptr L$74,1
	mov		cx,4
	mov		si,offset L$71
L$75:
	lodsb
	test		al,al
	lodsw
	je		L$76
	push		si
	push		cx
	call		ax
	pop		cx
	pop		si
L$76:
	loop		L$75
	call		near ptr L$91
	mov		al,byte ptr L$68
	mov		ah,0
	pop		di
	pop		si
	pop		ds
	mov		sp,bp
	pop		bp
	retf
	mov		ax,1a00H
	int		10H
	cmp		al,1aH
	jne		L$79
	mov		cx,bx
	xor		bh,bh
	or		ch,ch
	je		L$77
	mov		bl,ch
	add		bx,bx
	mov		ax,word ptr L$70[bx]
	mov		word ptr 2[di],ax
	mov		bl,cl
	xor		bh,bh
L$77:
	add		bx,bx
	mov		ax,word ptr L$70[bx]
	mov		word ptr [di],ax
	mov		byte ptr L$73,0
	mov		byte ptr L$72,0
	mov		byte ptr L$74,0
	lea		bx,[di]
	cmp		byte ptr [bx],1
	je		L$78
	lea		bx,2[di]
	cmp		byte ptr [bx],1
	jne		L$79
L$78:
	mov		word ptr [bx],0
	mov		byte ptr L$74,1
L$79:
	ret
	mov		bl,10H
	mov		ah,12H
	int		10H
	cmp		bl,10H
	je		L$81
	mov		al,cl
	shr		al,1
	mov		bx,offset L$69
	xlatb
	mov		ah,al
	mov		al,3
	call		near ptr L$95
	cmp		ah,1
	je		L$80
	mov		byte ptr L$73,0
	jmp		L$81
L$80:
	mov		byte ptr L$74,0
L$81:
	ret
L$82:
	mov		dx,3d4H
	call		near ptr L$88
	jb		L$83
	mov		al,2
	mov		ah,2
	call		near ptr L$95
L$83:
	ret
	mov		dx,3b4H
	call		near ptr L$88
	jb		L$87
	mov		dl,0baH
	in		al,dx
	and		al,80H
	mov		ah,al
	mov		cx,8000H
L$84:
	in		al,dx
	and		al,80H
	cmp		ah,al
	loopz		L$84
	jne		L$85
	mov		al,1
	mov		ah,1
	call		near ptr L$95
	jmp		L$87
L$85:
	in		al,dx
	mov		dl,al
	and		dl,70H
	mov		ah,1
	mov		al,81H
	cmp		dl,10H
	je		L$86
	mov		al,80H
	cmp		dl,50H
	jne		L$86
	mov		al,82H
	mov		ah,3
L$86:
	call		near ptr L$95
L$87:
	ret
L$88:
	mov		al,0fH
	out		dx,al
	inc		dx
	in		al,dx
	mov		ah,al
	mov		al,66H
	out		dx,al
	mov		cx,100H
L$89:
	loop		L$89
	in		al,dx
	xchg		al,ah
	out		dx,al
	cmp		ah,66H
	je		L$90
	stc
L$90:
	ret
L$91:
	cmp		word ptr 2[di],0
	je		L$94
	cmp		byte ptr [di],4
	jge		L$94
	cmp		byte ptr 2[di],4
	jge		L$94
	mov		ah,0fH
	int		10H
	and		al,7
	cmp		al,7
	je		L$92
	cmp		byte ptr 1[di],1
	jne		L$94
	jmp		L$93
L$92:
	cmp		byte ptr 1[di],1
	je		L$94
L$93:
	mov		ax,word ptr [di]
	xchg		word ptr 2[di],ax
	mov		word ptr [di],ax
L$94:
	ret
L$95:
	lea		bx,[di]
	cmp		byte ptr [bx],0
	je		L$96
	lea		bx,2[di]
L$96:
	mov		word ptr [bx],ax
	ret
ID_VW_A_TEXT		ENDS
_DATA		SEGMENT	WORD PUBLIC USE16 'DATA'
L$97:
    DB	0
L$98:
    DB	0
L$99:
    DB	0, 0
L$100:
    DB	0, 0
L$101:
    DB	0, 0, 1, 0, 2, 0, 3, 0
    DB	4, 0, 5, 0, 6, 0, 7, 0
    DB	8, 0, 9, 0, 0aH, 0, 0bH, 0
    DB	0cH, 0, 0dH, 0, 0eH, 0, 0fH, 0
    DB	10H, 0, 11H, 0, 12H, 0, 13H, 0
    DB	14H, 0, 15H, 0, 16H, 0, 17H, 0
    DB	18H, 0, 19H, 0, 1aH, 0, 1bH, 0
    DB	1cH, 0, 1dH, 0, 1eH, 0, 1fH, 0
    DB	20H, 0, 21H, 0, 22H, 0, 23H, 0
    DB	24H, 0, 25H, 0, 26H, 0, 27H, 0
    DB	28H, 0, 29H, 0, 2aH, 0, 2bH, 0
    DB	2cH, 0, 2dH, 0, 2eH, 0, 2fH, 0
    DB	30H, 0, 31H, 0, 32H, 0, 33H, 0
    DB	34H, 0, 35H, 0, 36H, 0, 37H, 0
    DB	38H, 0, 39H, 0, 3aH, 0, 3bH, 0
    DB	3cH, 0, 3dH, 0, 3eH, 0, 3fH, 0
    DB	40H, 0, 41H, 0, 42H, 0, 43H, 0
    DB	44H, 0, 45H, 0, 46H, 0, 47H, 0
    DB	48H, 0, 49H, 0, 4aH, 0, 4bH, 0
    DB	4cH, 0, 4dH, 0, 4eH, 0, 4fH, 0
    DB	50H, 0, 51H, 0, 52H, 0, 53H, 0
    DB	54H, 0, 55H, 0, 56H, 0, 57H, 0
    DB	58H, 0, 59H, 0, 5aH, 0, 5bH, 0
    DB	5cH, 0, 5dH, 0, 5eH, 0, 5fH, 0
    DB	60H, 0, 61H, 0, 62H, 0, 63H, 0
    DB	64H, 0, 65H, 0, 66H, 0, 67H, 0
    DB	68H, 0, 69H, 0, 6aH, 0, 6bH, 0
    DB	6cH, 0, 6dH, 0, 6eH, 0, 6fH, 0
    DB	70H, 0, 71H, 0, 72H, 0, 73H, 0
    DB	74H, 0, 75H, 0, 76H, 0, 77H, 0
    DB	78H, 0, 79H, 0, 7aH, 0, 7bH, 0
    DB	7cH, 0, 7dH, 0, 7eH, 0, 7fH, 0
    DB	80H, 0, 81H, 0, 82H, 0, 83H, 0
    DB	84H, 0, 85H, 0, 86H, 0, 87H, 0
    DB	88H, 0, 89H, 0, 8aH, 0, 8bH, 0
    DB	8cH, 0, 8dH, 0, 8eH, 0, 8fH, 0
    DB	90H, 0, 91H, 0, 92H, 0, 93H, 0
    DB	94H, 0, 95H, 0, 96H, 0, 97H, 0
    DB	98H, 0, 99H, 0, 9aH, 0, 9bH, 0
    DB	9cH, 0, 9dH, 0, 9eH, 0, 9fH, 0
    DB	0a0H, 0, 0a1H, 0, 0a2H, 0, 0a3H, 0
    DB	0a4H, 0, 0a5H, 0, 0a6H, 0, 0a7H, 0
    DB	0a8H, 0, 0a9H, 0, 0aaH, 0, 0abH, 0
    DB	0acH, 0, 0adH, 0, 0aeH, 0, 0afH, 0
    DB	0b0H, 0, 0b1H, 0, 0b2H, 0, 0b3H, 0
    DB	0b4H, 0, 0b5H, 0, 0b6H, 0, 0b7H, 0
    DB	0b8H, 0, 0b9H, 0, 0baH, 0, 0bbH, 0
    DB	0bcH, 0, 0bdH, 0, 0beH, 0, 0bfH, 0
    DB	0c0H, 0, 0c1H, 0, 0c2H, 0, 0c3H, 0
    DB	0c4H, 0, 0c5H, 0, 0c6H, 0, 0c7H, 0
    DB	0c8H, 0, 0c9H, 0, 0caH, 0, 0cbH, 0
    DB	0ccH, 0, 0cdH, 0, 0ceH, 0, 0cfH, 0
    DB	0d0H, 0, 0d1H, 0, 0d2H, 0, 0d3H, 0
    DB	0d4H, 0, 0d5H, 0, 0d6H, 0, 0d7H, 0
    DB	0d8H, 0, 0d9H, 0, 0daH, 0, 0dbH, 0
    DB	0dcH, 0, 0ddH, 0, 0deH, 0, 0dfH, 0
    DB	0e0H, 0, 0e1H, 0, 0e2H, 0, 0e3H, 0
    DB	0e4H, 0, 0e5H, 0, 0e6H, 0, 0e7H, 0
    DB	0e8H, 0, 0e9H, 0, 0eaH, 0, 0ebH, 0
    DB	0ecH, 0, 0edH, 0, 0eeH, 0, 0efH, 0
    DB	0f0H, 0, 0f1H, 0, 0f2H, 0, 0f3H, 0
    DB	0f4H, 0, 0f5H, 0, 0f6H, 0, 0f7H, 0
    DB	0f8H, 0, 0f9H, 0, 0faH, 0, 0fbH, 0
    DB	0fcH, 0, 0fdH, 0, 0feH, 0, 0ffH, 0
L$102:
    DB	0, 0, 0, 80H, 1, 0, 1, 80H
    DB	2, 0, 2, 80H, 3, 0, 3, 80H
    DB	4, 0, 4, 80H, 5, 0, 5, 80H
    DB	6, 0, 6, 80H, 7, 0, 7, 80H
    DB	8, 0, 8, 80H, 9, 0, 9, 80H
    DB	0aH, 0, 0aH, 80H, 0bH, 0, 0bH, 80H
    DB	0cH, 0, 0cH, 80H, 0dH, 0, 0dH, 80H
    DB	0eH, 0, 0eH, 80H, 0fH, 0, 0fH, 80H
    DB	10H, 0, 10H, 80H, 11H, 0, 11H, 80H
    DB	12H, 0, 12H, 80H, 13H, 0, 13H, 80H
    DB	14H, 0, 14H, 80H, 15H, 0, 15H, 80H
    DB	16H, 0, 16H, 80H, 17H, 0, 17H, 80H
    DB	18H, 0, 18H, 80H, 19H, 0, 19H, 80H
    DB	1aH, 0, 1aH, 80H, 1bH, 0, 1bH, 80H
    DB	1cH, 0, 1cH, 80H, 1dH, 0, 1dH, 80H
    DB	1eH, 0, 1eH, 80H, 1fH, 0, 1fH, 80H
    DB	20H, 0, 20H, 80H, 21H, 0, 21H, 80H
    DB	22H, 0, 22H, 80H, 23H, 0, 23H, 80H
    DB	24H, 0, 24H, 80H, 25H, 0, 25H, 80H
    DB	26H, 0, 26H, 80H, 27H, 0, 27H, 80H
    DB	28H, 0, 28H, 80H, 29H, 0, 29H, 80H
    DB	2aH, 0, 2aH, 80H, 2bH, 0, 2bH, 80H
    DB	2cH, 0, 2cH, 80H, 2dH, 0, 2dH, 80H
    DB	2eH, 0, 2eH, 80H, 2fH, 0, 2fH, 80H
    DB	30H, 0, 30H, 80H, 31H, 0, 31H, 80H
    DB	32H, 0, 32H, 80H, 33H, 0, 33H, 80H
    DB	34H, 0, 34H, 80H, 35H, 0, 35H, 80H
    DB	36H, 0, 36H, 80H, 37H, 0, 37H, 80H
    DB	38H, 0, 38H, 80H, 39H, 0, 39H, 80H
    DB	3aH, 0, 3aH, 80H, 3bH, 0, 3bH, 80H
    DB	3cH, 0, 3cH, 80H, 3dH, 0, 3dH, 80H
    DB	3eH, 0, 3eH, 80H, 3fH, 0, 3fH, 80H
    DB	40H, 0, 40H, 80H, 41H, 0, 41H, 80H
    DB	42H, 0, 42H, 80H, 43H, 0, 43H, 80H
    DB	44H, 0, 44H, 80H, 45H, 0, 45H, 80H
    DB	46H, 0, 46H, 80H, 47H, 0, 47H, 80H
    DB	48H, 0, 48H, 80H, 49H, 0, 49H, 80H
    DB	4aH, 0, 4aH, 80H, 4bH, 0, 4bH, 80H
    DB	4cH, 0, 4cH, 80H, 4dH, 0, 4dH, 80H
    DB	4eH, 0, 4eH, 80H, 4fH, 0, 4fH, 80H
    DB	50H, 0, 50H, 80H, 51H, 0, 51H, 80H
    DB	52H, 0, 52H, 80H, 53H, 0, 53H, 80H
    DB	54H, 0, 54H, 80H, 55H, 0, 55H, 80H
    DB	56H, 0, 56H, 80H, 57H, 0, 57H, 80H
    DB	58H, 0, 58H, 80H, 59H, 0, 59H, 80H
    DB	5aH, 0, 5aH, 80H, 5bH, 0, 5bH, 80H
    DB	5cH, 0, 5cH, 80H, 5dH, 0, 5dH, 80H
    DB	5eH, 0, 5eH, 80H, 5fH, 0, 5fH, 80H
    DB	60H, 0, 60H, 80H, 61H, 0, 61H, 80H
    DB	62H, 0, 62H, 80H, 63H, 0, 63H, 80H
    DB	64H, 0, 64H, 80H, 65H, 0, 65H, 80H
    DB	66H, 0, 66H, 80H, 67H, 0, 67H, 80H
    DB	68H, 0, 68H, 80H, 69H, 0, 69H, 80H
    DB	6aH, 0, 6aH, 80H, 6bH, 0, 6bH, 80H
    DB	6cH, 0, 6cH, 80H, 6dH, 0, 6dH, 80H
    DB	6eH, 0, 6eH, 80H, 6fH, 0, 6fH, 80H
    DB	70H, 0, 70H, 80H, 71H, 0, 71H, 80H
    DB	72H, 0, 72H, 80H, 73H, 0, 73H, 80H
    DB	74H, 0, 74H, 80H, 75H, 0, 75H, 80H
    DB	76H, 0, 76H, 80H, 77H, 0, 77H, 80H
    DB	78H, 0, 78H, 80H, 79H, 0, 79H, 80H
    DB	7aH, 0, 7aH, 80H, 7bH, 0, 7bH, 80H
    DB	7cH, 0, 7cH, 80H, 7dH, 0, 7dH, 80H
    DB	7eH, 0, 7eH, 80H, 7fH, 0, 7fH, 80H
L$103:
    DB	0, 0, 0, 40H, 0, 80H, 0, 0c0H
    DB	1, 0, 1, 40H, 1, 80H, 1, 0c0H
    DB	2, 0, 2, 40H, 2, 80H, 2, 0c0H
    DB	3, 0, 3, 40H, 3, 80H, 3, 0c0H
    DB	4, 0, 4, 40H, 4, 80H, 4, 0c0H
    DB	5, 0, 5, 40H, 5, 80H, 5, 0c0H
    DB	6, 0, 6, 40H, 6, 80H, 6, 0c0H
    DB	7, 0, 7, 40H, 7, 80H, 7, 0c0H
    DB	8, 0, 8, 40H, 8, 80H, 8, 0c0H
    DB	9, 0, 9, 40H, 9, 80H, 9, 0c0H
    DB	0aH, 0, 0aH, 40H, 0aH, 80H, 0aH, 0c0H
    DB	0bH, 0, 0bH, 40H, 0bH, 80H, 0bH, 0c0H
    DB	0cH, 0, 0cH, 40H, 0cH, 80H, 0cH, 0c0H
    DB	0dH, 0, 0dH, 40H, 0dH, 80H, 0dH, 0c0H
    DB	0eH, 0, 0eH, 40H, 0eH, 80H, 0eH, 0c0H
    DB	0fH, 0, 0fH, 40H, 0fH, 80H, 0fH, 0c0H
    DB	10H, 0, 10H, 40H, 10H, 80H, 10H, 0c0H
    DB	11H, 0, 11H, 40H, 11H, 80H, 11H, 0c0H
    DB	12H, 0, 12H, 40H, 12H, 80H, 12H, 0c0H
    DB	13H, 0, 13H, 40H, 13H, 80H, 13H, 0c0H
    DB	14H, 0, 14H, 40H, 14H, 80H, 14H, 0c0H
    DB	15H, 0, 15H, 40H, 15H, 80H, 15H, 0c0H
    DB	16H, 0, 16H, 40H, 16H, 80H, 16H, 0c0H
    DB	17H, 0, 17H, 40H, 17H, 80H, 17H, 0c0H
    DB	18H, 0, 18H, 40H, 18H, 80H, 18H, 0c0H
    DB	19H, 0, 19H, 40H, 19H, 80H, 19H, 0c0H
    DB	1aH, 0, 1aH, 40H, 1aH, 80H, 1aH, 0c0H
    DB	1bH, 0, 1bH, 40H, 1bH, 80H, 1bH, 0c0H
    DB	1cH, 0, 1cH, 40H, 1cH, 80H, 1cH, 0c0H
    DB	1dH, 0, 1dH, 40H, 1dH, 80H, 1dH, 0c0H
    DB	1eH, 0, 1eH, 40H, 1eH, 80H, 1eH, 0c0H
    DB	1fH, 0, 1fH, 40H, 1fH, 80H, 1fH, 0c0H
    DB	20H, 0, 20H, 40H, 20H, 80H, 20H, 0c0H
    DB	21H, 0, 21H, 40H, 21H, 80H, 21H, 0c0H
    DB	22H, 0, 22H, 40H, 22H, 80H, 22H, 0c0H
    DB	23H, 0, 23H, 40H, 23H, 80H, 23H, 0c0H
    DB	24H, 0, 24H, 40H, 24H, 80H, 24H, 0c0H
    DB	25H, 0, 25H, 40H, 25H, 80H, 25H, 0c0H
    DB	26H, 0, 26H, 40H, 26H, 80H, 26H, 0c0H
    DB	27H, 0, 27H, 40H, 27H, 80H, 27H, 0c0H
    DB	28H, 0, 28H, 40H, 28H, 80H, 28H, 0c0H
    DB	29H, 0, 29H, 40H, 29H, 80H, 29H, 0c0H
    DB	2aH, 0, 2aH, 40H, 2aH, 80H, 2aH, 0c0H
    DB	2bH, 0, 2bH, 40H, 2bH, 80H, 2bH, 0c0H
    DB	2cH, 0, 2cH, 40H, 2cH, 80H, 2cH, 0c0H
    DB	2dH, 0, 2dH, 40H, 2dH, 80H, 2dH, 0c0H
    DB	2eH, 0, 2eH, 40H, 2eH, 80H, 2eH, 0c0H
    DB	2fH, 0, 2fH, 40H, 2fH, 80H, 2fH, 0c0H
    DB	30H, 0, 30H, 40H, 30H, 80H, 30H, 0c0H
    DB	31H, 0, 31H, 40H, 31H, 80H, 31H, 0c0H
    DB	32H, 0, 32H, 40H, 32H, 80H, 32H, 0c0H
    DB	33H, 0, 33H, 40H, 33H, 80H, 33H, 0c0H
    DB	34H, 0, 34H, 40H, 34H, 80H, 34H, 0c0H
    DB	35H, 0, 35H, 40H, 35H, 80H, 35H, 0c0H
    DB	36H, 0, 36H, 40H, 36H, 80H, 36H, 0c0H
    DB	37H, 0, 37H, 40H, 37H, 80H, 37H, 0c0H
    DB	38H, 0, 38H, 40H, 38H, 80H, 38H, 0c0H
    DB	39H, 0, 39H, 40H, 39H, 80H, 39H, 0c0H
    DB	3aH, 0, 3aH, 40H, 3aH, 80H, 3aH, 0c0H
    DB	3bH, 0, 3bH, 40H, 3bH, 80H, 3bH, 0c0H
    DB	3cH, 0, 3cH, 40H, 3cH, 80H, 3cH, 0c0H
    DB	3dH, 0, 3dH, 40H, 3dH, 80H, 3dH, 0c0H
    DB	3eH, 0, 3eH, 40H, 3eH, 80H, 3eH, 0c0H
    DB	3fH, 0, 3fH, 40H, 3fH, 80H, 3fH, 0c0H
L$104:
    DB	0, 0, 0, 20H, 0, 40H, 0, 60H
    DB	0, 80H, 0, 0a0H, 0, 0c0H, 0, 0e0H
    DB	1, 0, 1, 20H, 1, 40H, 1, 60H
    DB	1, 80H, 1, 0a0H, 1, 0c0H, 1, 0e0H
    DB	2, 0, 2, 20H, 2, 40H, 2, 60H
    DB	2, 80H, 2, 0a0H, 2, 0c0H, 2, 0e0H
    DB	3, 0, 3, 20H, 3, 40H, 3, 60H
    DB	3, 80H, 3, 0a0H, 3, 0c0H, 3, 0e0H
    DB	4, 0, 4, 20H, 4, 40H, 4, 60H
    DB	4, 80H, 4, 0a0H, 4, 0c0H, 4, 0e0H
    DB	5, 0, 5, 20H, 5, 40H, 5, 60H
    DB	5, 80H, 5, 0a0H, 5, 0c0H, 5, 0e0H
    DB	6, 0, 6, 20H, 6, 40H, 6, 60H
    DB	6, 80H, 6, 0a0H, 6, 0c0H, 6, 0e0H
    DB	7, 0, 7, 20H, 7, 40H, 7, 60H
    DB	7, 80H, 7, 0a0H, 7, 0c0H, 7, 0e0H
    DB	8, 0, 8, 20H, 8, 40H, 8, 60H
    DB	8, 80H, 8, 0a0H, 8, 0c0H, 8, 0e0H
    DB	9, 0, 9, 20H, 9, 40H, 9, 60H
    DB	9, 80H, 9, 0a0H, 9, 0c0H, 9, 0e0H
    DB	0aH, 0, 0aH, 20H, 0aH, 40H, 0aH, 60H
    DB	0aH, 80H, 0aH, 0a0H, 0aH, 0c0H, 0aH, 0e0H
    DB	0bH, 0, 0bH, 20H, 0bH, 40H, 0bH, 60H
    DB	0bH, 80H, 0bH, 0a0H, 0bH, 0c0H, 0bH, 0e0H
    DB	0cH, 0, 0cH, 20H, 0cH, 40H, 0cH, 60H
    DB	0cH, 80H, 0cH, 0a0H, 0cH, 0c0H, 0cH, 0e0H
    DB	0dH, 0, 0dH, 20H, 0dH, 40H, 0dH, 60H
    DB	0dH, 80H, 0dH, 0a0H, 0dH, 0c0H, 0dH, 0e0H
    DB	0eH, 0, 0eH, 20H, 0eH, 40H, 0eH, 60H
    DB	0eH, 80H, 0eH, 0a0H, 0eH, 0c0H, 0eH, 0e0H
    DB	0fH, 0, 0fH, 20H, 0fH, 40H, 0fH, 60H
    DB	0fH, 80H, 0fH, 0a0H, 0fH, 0c0H, 0fH, 0e0H
    DB	10H, 0, 10H, 20H, 10H, 40H, 10H, 60H
    DB	10H, 80H, 10H, 0a0H, 10H, 0c0H, 10H, 0e0H
    DB	11H, 0, 11H, 20H, 11H, 40H, 11H, 60H
    DB	11H, 80H, 11H, 0a0H, 11H, 0c0H, 11H, 0e0H
    DB	12H, 0, 12H, 20H, 12H, 40H, 12H, 60H
    DB	12H, 80H, 12H, 0a0H, 12H, 0c0H, 12H, 0e0H
    DB	13H, 0, 13H, 20H, 13H, 40H, 13H, 60H
    DB	13H, 80H, 13H, 0a0H, 13H, 0c0H, 13H, 0e0H
    DB	14H, 0, 14H, 20H, 14H, 40H, 14H, 60H
    DB	14H, 80H, 14H, 0a0H, 14H, 0c0H, 14H, 0e0H
    DB	15H, 0, 15H, 20H, 15H, 40H, 15H, 60H
    DB	15H, 80H, 15H, 0a0H, 15H, 0c0H, 15H, 0e0H
    DB	16H, 0, 16H, 20H, 16H, 40H, 16H, 60H
    DB	16H, 80H, 16H, 0a0H, 16H, 0c0H, 16H, 0e0H
    DB	17H, 0, 17H, 20H, 17H, 40H, 17H, 60H
    DB	17H, 80H, 17H, 0a0H, 17H, 0c0H, 17H, 0e0H
    DB	18H, 0, 18H, 20H, 18H, 40H, 18H, 60H
    DB	18H, 80H, 18H, 0a0H, 18H, 0c0H, 18H, 0e0H
    DB	19H, 0, 19H, 20H, 19H, 40H, 19H, 60H
    DB	19H, 80H, 19H, 0a0H, 19H, 0c0H, 19H, 0e0H
    DB	1aH, 0, 1aH, 20H, 1aH, 40H, 1aH, 60H
    DB	1aH, 80H, 1aH, 0a0H, 1aH, 0c0H, 1aH, 0e0H
    DB	1bH, 0, 1bH, 20H, 1bH, 40H, 1bH, 60H
    DB	1bH, 80H, 1bH, 0a0H, 1bH, 0c0H, 1bH, 0e0H
    DB	1cH, 0, 1cH, 20H, 1cH, 40H, 1cH, 60H
    DB	1cH, 80H, 1cH, 0a0H, 1cH, 0c0H, 1cH, 0e0H
    DB	1dH, 0, 1dH, 20H, 1dH, 40H, 1dH, 60H
    DB	1dH, 80H, 1dH, 0a0H, 1dH, 0c0H, 1dH, 0e0H
    DB	1eH, 0, 1eH, 20H, 1eH, 40H, 1eH, 60H
    DB	1eH, 80H, 1eH, 0a0H, 1eH, 0c0H, 1eH, 0e0H
    DB	1fH, 0, 1fH, 20H, 1fH, 40H, 1fH, 60H
    DB	1fH, 80H, 1fH, 0a0H, 1fH, 0c0H, 1fH, 0e0H
L$105:
    DB	0, 0, 0, 10H, 0, 20H, 0, 30H
    DB	0, 40H, 0, 50H, 0, 60H, 0, 70H
    DB	0, 80H, 0, 90H, 0, 0a0H, 0, 0b0H
    DB	0, 0c0H, 0, 0d0H, 0, 0e0H, 0, 0f0H
    DB	1, 0, 1, 10H, 1, 20H, 1, 30H
    DB	1, 40H, 1, 50H, 1, 60H, 1, 70H
    DB	1, 80H, 1, 90H, 1, 0a0H, 1, 0b0H
    DB	1, 0c0H, 1, 0d0H, 1, 0e0H, 1, 0f0H
    DB	2, 0, 2, 10H, 2, 20H, 2, 30H
    DB	2, 40H, 2, 50H, 2, 60H, 2, 70H
    DB	2, 80H, 2, 90H, 2, 0a0H, 2, 0b0H
    DB	2, 0c0H, 2, 0d0H, 2, 0e0H, 2, 0f0H
    DB	3, 0, 3, 10H, 3, 20H, 3, 30H
    DB	3, 40H, 3, 50H, 3, 60H, 3, 70H
    DB	3, 80H, 3, 90H, 3, 0a0H, 3, 0b0H
    DB	3, 0c0H, 3, 0d0H, 3, 0e0H, 3, 0f0H
    DB	4, 0, 4, 10H, 4, 20H, 4, 30H
    DB	4, 40H, 4, 50H, 4, 60H, 4, 70H
    DB	4, 80H, 4, 90H, 4, 0a0H, 4, 0b0H
    DB	4, 0c0H, 4, 0d0H, 4, 0e0H, 4, 0f0H
    DB	5, 0, 5, 10H, 5, 20H, 5, 30H
    DB	5, 40H, 5, 50H, 5, 60H, 5, 70H
    DB	5, 80H, 5, 90H, 5, 0a0H, 5, 0b0H
    DB	5, 0c0H, 5, 0d0H, 5, 0e0H, 5, 0f0H
    DB	6, 0, 6, 10H, 6, 20H, 6, 30H
    DB	6, 40H, 6, 50H, 6, 60H, 6, 70H
    DB	6, 80H, 6, 90H, 6, 0a0H, 6, 0b0H
    DB	6, 0c0H, 6, 0d0H, 6, 0e0H, 6, 0f0H
    DB	7, 0, 7, 10H, 7, 20H, 7, 30H
    DB	7, 40H, 7, 50H, 7, 60H, 7, 70H
    DB	7, 80H, 7, 90H, 7, 0a0H, 7, 0b0H
    DB	7, 0c0H, 7, 0d0H, 7, 0e0H, 7, 0f0H
    DB	8, 0, 8, 10H, 8, 20H, 8, 30H
    DB	8, 40H, 8, 50H, 8, 60H, 8, 70H
    DB	8, 80H, 8, 90H, 8, 0a0H, 8, 0b0H
    DB	8, 0c0H, 8, 0d0H, 8, 0e0H, 8, 0f0H
    DB	9, 0, 9, 10H, 9, 20H, 9, 30H
    DB	9, 40H, 9, 50H, 9, 60H, 9, 70H
    DB	9, 80H, 9, 90H, 9, 0a0H, 9, 0b0H
    DB	9, 0c0H, 9, 0d0H, 9, 0e0H, 9, 0f0H
    DB	0aH, 0, 0aH, 10H, 0aH, 20H, 0aH, 30H
    DB	0aH, 40H, 0aH, 50H, 0aH, 60H, 0aH, 70H
    DB	0aH, 80H, 0aH, 90H, 0aH, 0a0H, 0aH, 0b0H
    DB	0aH, 0c0H, 0aH, 0d0H, 0aH, 0e0H, 0aH, 0f0H
    DB	0bH, 0, 0bH, 10H, 0bH, 20H, 0bH, 30H
    DB	0bH, 40H, 0bH, 50H, 0bH, 60H, 0bH, 70H
    DB	0bH, 80H, 0bH, 90H, 0bH, 0a0H, 0bH, 0b0H
    DB	0bH, 0c0H, 0bH, 0d0H, 0bH, 0e0H, 0bH, 0f0H
    DB	0cH, 0, 0cH, 10H, 0cH, 20H, 0cH, 30H
    DB	0cH, 40H, 0cH, 50H, 0cH, 60H, 0cH, 70H
    DB	0cH, 80H, 0cH, 90H, 0cH, 0a0H, 0cH, 0b0H
    DB	0cH, 0c0H, 0cH, 0d0H, 0cH, 0e0H, 0cH, 0f0H
    DB	0dH, 0, 0dH, 10H, 0dH, 20H, 0dH, 30H
    DB	0dH, 40H, 0dH, 50H, 0dH, 60H, 0dH, 70H
    DB	0dH, 80H, 0dH, 90H, 0dH, 0a0H, 0dH, 0b0H
    DB	0dH, 0c0H, 0dH, 0d0H, 0dH, 0e0H, 0dH, 0f0H
    DB	0eH, 0, 0eH, 10H, 0eH, 20H, 0eH, 30H
    DB	0eH, 40H, 0eH, 50H, 0eH, 60H, 0eH, 70H
    DB	0eH, 80H, 0eH, 90H, 0eH, 0a0H, 0eH, 0b0H
    DB	0eH, 0c0H, 0eH, 0d0H, 0eH, 0e0H, 0eH, 0f0H
    DB	0fH, 0, 0fH, 10H, 0fH, 20H, 0fH, 30H
    DB	0fH, 40H, 0fH, 50H, 0fH, 60H, 0fH, 70H
    DB	0fH, 80H, 0fH, 90H, 0fH, 0a0H, 0fH, 0b0H
    DB	0fH, 0c0H, 0fH, 0d0H, 0fH, 0e0H, 0fH, 0f0H
L$106:
    DB	0, 0, 0, 8, 0, 10H, 0, 18H
    DB	0, 20H, 0, 28H, 0, 30H, 0, 38H
    DB	0, 40H, 0, 48H, 0, 50H, 0, 58H
    DB	0, 60H, 0, 68H, 0, 70H, 0, 78H
    DB	0, 80H, 0, 88H, 0, 90H, 0, 98H
    DB	0, 0a0H, 0, 0a8H, 0, 0b0H, 0, 0b8H
    DB	0, 0c0H, 0, 0c8H, 0, 0d0H, 0, 0d8H
    DB	0, 0e0H, 0, 0e8H, 0, 0f0H, 0, 0f8H
    DB	1, 0, 1, 8, 1, 10H, 1, 18H
    DB	1, 20H, 1, 28H, 1, 30H, 1, 38H
    DB	1, 40H, 1, 48H, 1, 50H, 1, 58H
    DB	1, 60H, 1, 68H, 1, 70H, 1, 78H
    DB	1, 80H, 1, 88H, 1, 90H, 1, 98H
    DB	1, 0a0H, 1, 0a8H, 1, 0b0H, 1, 0b8H
    DB	1, 0c0H, 1, 0c8H, 1, 0d0H, 1, 0d8H
    DB	1, 0e0H, 1, 0e8H, 1, 0f0H, 1, 0f8H
    DB	2, 0, 2, 8, 2, 10H, 2, 18H
    DB	2, 20H, 2, 28H, 2, 30H, 2, 38H
    DB	2, 40H, 2, 48H, 2, 50H, 2, 58H
    DB	2, 60H, 2, 68H, 2, 70H, 2, 78H
    DB	2, 80H, 2, 88H, 2, 90H, 2, 98H
    DB	2, 0a0H, 2, 0a8H, 2, 0b0H, 2, 0b8H
    DB	2, 0c0H, 2, 0c8H, 2, 0d0H, 2, 0d8H
    DB	2, 0e0H, 2, 0e8H, 2, 0f0H, 2, 0f8H
    DB	3, 0, 3, 8, 3, 10H, 3, 18H
    DB	3, 20H, 3, 28H, 3, 30H, 3, 38H
    DB	3, 40H, 3, 48H, 3, 50H, 3, 58H
    DB	3, 60H, 3, 68H, 3, 70H, 3, 78H
    DB	3, 80H, 3, 88H, 3, 90H, 3, 98H
    DB	3, 0a0H, 3, 0a8H, 3, 0b0H, 3, 0b8H
    DB	3, 0c0H, 3, 0c8H, 3, 0d0H, 3, 0d8H
    DB	3, 0e0H, 3, 0e8H, 3, 0f0H, 3, 0f8H
    DB	4, 0, 4, 8, 4, 10H, 4, 18H
    DB	4, 20H, 4, 28H, 4, 30H, 4, 38H
    DB	4, 40H, 4, 48H, 4, 50H, 4, 58H
    DB	4, 60H, 4, 68H, 4, 70H, 4, 78H
    DB	4, 80H, 4, 88H, 4, 90H, 4, 98H
    DB	4, 0a0H, 4, 0a8H, 4, 0b0H, 4, 0b8H
    DB	4, 0c0H, 4, 0c8H, 4, 0d0H, 4, 0d8H
    DB	4, 0e0H, 4, 0e8H, 4, 0f0H, 4, 0f8H
    DB	5, 0, 5, 8, 5, 10H, 5, 18H
    DB	5, 20H, 5, 28H, 5, 30H, 5, 38H
    DB	5, 40H, 5, 48H, 5, 50H, 5, 58H
    DB	5, 60H, 5, 68H, 5, 70H, 5, 78H
    DB	5, 80H, 5, 88H, 5, 90H, 5, 98H
    DB	5, 0a0H, 5, 0a8H, 5, 0b0H, 5, 0b8H
    DB	5, 0c0H, 5, 0c8H, 5, 0d0H, 5, 0d8H
    DB	5, 0e0H, 5, 0e8H, 5, 0f0H, 5, 0f8H
    DB	6, 0, 6, 8, 6, 10H, 6, 18H
    DB	6, 20H, 6, 28H, 6, 30H, 6, 38H
    DB	6, 40H, 6, 48H, 6, 50H, 6, 58H
    DB	6, 60H, 6, 68H, 6, 70H, 6, 78H
    DB	6, 80H, 6, 88H, 6, 90H, 6, 98H
    DB	6, 0a0H, 6, 0a8H, 6, 0b0H, 6, 0b8H
    DB	6, 0c0H, 6, 0c8H, 6, 0d0H, 6, 0d8H
    DB	6, 0e0H, 6, 0e8H, 6, 0f0H, 6, 0f8H
    DB	7, 0, 7, 8, 7, 10H, 7, 18H
    DB	7, 20H, 7, 28H, 7, 30H, 7, 38H
    DB	7, 40H, 7, 48H, 7, 50H, 7, 58H
    DB	7, 60H, 7, 68H, 7, 70H, 7, 78H
    DB	7, 80H, 7, 88H, 7, 90H, 7, 98H
    DB	7, 0a0H, 7, 0a8H, 7, 0b0H, 7, 0b8H
    DB	7, 0c0H, 7, 0c8H, 7, 0d0H, 7, 0d8H
    DB	7, 0e0H, 7, 0e8H, 7, 0f0H, 7, 0f8H
L$107:
    DB	0, 0, 0, 4, 0, 8, 0, 0cH
    DB	0, 10H, 0, 14H, 0, 18H, 0, 1cH
    DB	0, 20H, 0, 24H, 0, 28H, 0, 2cH
    DB	0, 30H, 0, 34H, 0, 38H, 0, 3cH
    DB	0, 40H, 0, 44H, 0, 48H, 0, 4cH
    DB	0, 50H, 0, 54H, 0, 58H, 0, 5cH
    DB	0, 60H, 0, 64H, 0, 68H, 0, 6cH
    DB	0, 70H, 0, 74H, 0, 78H, 0, 7cH
    DB	0, 80H, 0, 84H, 0, 88H, 0, 8cH
    DB	0, 90H, 0, 94H, 0, 98H, 0, 9cH
    DB	0, 0a0H, 0, 0a4H, 0, 0a8H, 0, 0acH
    DB	0, 0b0H, 0, 0b4H, 0, 0b8H, 0, 0bcH
    DB	0, 0c0H, 0, 0c4H, 0, 0c8H, 0, 0ccH
    DB	0, 0d0H, 0, 0d4H, 0, 0d8H, 0, 0dcH
    DB	0, 0e0H, 0, 0e4H, 0, 0e8H, 0, 0ecH
    DB	0, 0f0H, 0, 0f4H, 0, 0f8H, 0, 0fcH
    DB	1, 0, 1, 4, 1, 8, 1, 0cH
    DB	1, 10H, 1, 14H, 1, 18H, 1, 1cH
    DB	1, 20H, 1, 24H, 1, 28H, 1, 2cH
    DB	1, 30H, 1, 34H, 1, 38H, 1, 3cH
    DB	1, 40H, 1, 44H, 1, 48H, 1, 4cH
    DB	1, 50H, 1, 54H, 1, 58H, 1, 5cH
    DB	1, 60H, 1, 64H, 1, 68H, 1, 6cH
    DB	1, 70H, 1, 74H, 1, 78H, 1, 7cH
    DB	1, 80H, 1, 84H, 1, 88H, 1, 8cH
    DB	1, 90H, 1, 94H, 1, 98H, 1, 9cH
    DB	1, 0a0H, 1, 0a4H, 1, 0a8H, 1, 0acH
    DB	1, 0b0H, 1, 0b4H, 1, 0b8H, 1, 0bcH
    DB	1, 0c0H, 1, 0c4H, 1, 0c8H, 1, 0ccH
    DB	1, 0d0H, 1, 0d4H, 1, 0d8H, 1, 0dcH
    DB	1, 0e0H, 1, 0e4H, 1, 0e8H, 1, 0ecH
    DB	1, 0f0H, 1, 0f4H, 1, 0f8H, 1, 0fcH
    DB	2, 0, 2, 4, 2, 8, 2, 0cH
    DB	2, 10H, 2, 14H, 2, 18H, 2, 1cH
    DB	2, 20H, 2, 24H, 2, 28H, 2, 2cH
    DB	2, 30H, 2, 34H, 2, 38H, 2, 3cH
    DB	2, 40H, 2, 44H, 2, 48H, 2, 4cH
    DB	2, 50H, 2, 54H, 2, 58H, 2, 5cH
    DB	2, 60H, 2, 64H, 2, 68H, 2, 6cH
    DB	2, 70H, 2, 74H, 2, 78H, 2, 7cH
    DB	2, 80H, 2, 84H, 2, 88H, 2, 8cH
    DB	2, 90H, 2, 94H, 2, 98H, 2, 9cH
    DB	2, 0a0H, 2, 0a4H, 2, 0a8H, 2, 0acH
    DB	2, 0b0H, 2, 0b4H, 2, 0b8H, 2, 0bcH
    DB	2, 0c0H, 2, 0c4H, 2, 0c8H, 2, 0ccH
    DB	2, 0d0H, 2, 0d4H, 2, 0d8H, 2, 0dcH
    DB	2, 0e0H, 2, 0e4H, 2, 0e8H, 2, 0ecH
    DB	2, 0f0H, 2, 0f4H, 2, 0f8H, 2, 0fcH
    DB	3, 0, 3, 4, 3, 8, 3, 0cH
    DB	3, 10H, 3, 14H, 3, 18H, 3, 1cH
    DB	3, 20H, 3, 24H, 3, 28H, 3, 2cH
    DB	3, 30H, 3, 34H, 3, 38H, 3, 3cH
    DB	3, 40H, 3, 44H, 3, 48H, 3, 4cH
    DB	3, 50H, 3, 54H, 3, 58H, 3, 5cH
    DB	3, 60H, 3, 64H, 3, 68H, 3, 6cH
    DB	3, 70H, 3, 74H, 3, 78H, 3, 7cH
    DB	3, 80H, 3, 84H, 3, 88H, 3, 8cH
    DB	3, 90H, 3, 94H, 3, 98H, 3, 9cH
    DB	3, 0a0H, 3, 0a4H, 3, 0a8H, 3, 0acH
    DB	3, 0b0H, 3, 0b4H, 3, 0b8H, 3, 0bcH
    DB	3, 0c0H, 3, 0c4H, 3, 0c8H, 3, 0ccH
    DB	3, 0d0H, 3, 0d4H, 3, 0d8H, 3, 0dcH
    DB	3, 0e0H, 3, 0e4H, 3, 0e8H, 3, 0ecH
    DB	3, 0f0H, 3, 0f4H, 3, 0f8H, 3, 0fcH
L$108:
    DB	0, 0, 0, 2, 0, 4, 0, 6
    DB	0, 8, 0, 0aH, 0, 0cH, 0, 0eH
    DB	0, 10H, 0, 12H, 0, 14H, 0, 16H
    DB	0, 18H, 0, 1aH, 0, 1cH, 0, 1eH
    DB	0, 20H, 0, 22H, 0, 24H, 0, 26H
    DB	0, 28H, 0, 2aH, 0, 2cH, 0, 2eH
    DB	0, 30H, 0, 32H, 0, 34H, 0, 36H
    DB	0, 38H, 0, 3aH, 0, 3cH, 0, 3eH
    DB	0, 40H, 0, 42H, 0, 44H, 0, 46H
    DB	0, 48H, 0, 4aH, 0, 4cH, 0, 4eH
    DB	0, 50H, 0, 52H, 0, 54H, 0, 56H
    DB	0, 58H, 0, 5aH, 0, 5cH, 0, 5eH
    DB	0, 60H, 0, 62H, 0, 64H, 0, 66H
    DB	0, 68H, 0, 6aH, 0, 6cH, 0, 6eH
    DB	0, 70H, 0, 72H, 0, 74H, 0, 76H
    DB	0, 78H, 0, 7aH, 0, 7cH, 0, 7eH
    DB	0, 80H, 0, 82H, 0, 84H, 0, 86H
    DB	0, 88H, 0, 8aH, 0, 8cH, 0, 8eH
    DB	0, 90H, 0, 92H, 0, 94H, 0, 96H
    DB	0, 98H, 0, 9aH, 0, 9cH, 0, 9eH
    DB	0, 0a0H, 0, 0a2H, 0, 0a4H, 0, 0a6H
    DB	0, 0a8H, 0, 0aaH, 0, 0acH, 0, 0aeH
    DB	0, 0b0H, 0, 0b2H, 0, 0b4H, 0, 0b6H
    DB	0, 0b8H, 0, 0baH, 0, 0bcH, 0, 0beH
    DB	0, 0c0H, 0, 0c2H, 0, 0c4H, 0, 0c6H
    DB	0, 0c8H, 0, 0caH, 0, 0ccH, 0, 0ceH
    DB	0, 0d0H, 0, 0d2H, 0, 0d4H, 0, 0d6H
    DB	0, 0d8H, 0, 0daH, 0, 0dcH, 0, 0deH
    DB	0, 0e0H, 0, 0e2H, 0, 0e4H, 0, 0e6H
    DB	0, 0e8H, 0, 0eaH, 0, 0ecH, 0, 0eeH
    DB	0, 0f0H, 0, 0f2H, 0, 0f4H, 0, 0f6H
    DB	0, 0f8H, 0, 0faH, 0, 0fcH, 0, 0feH
    DB	1, 0, 1, 2, 1, 4, 1, 6
    DB	1, 8, 1, 0aH, 1, 0cH, 1, 0eH
    DB	1, 10H, 1, 12H, 1, 14H, 1, 16H
    DB	1, 18H, 1, 1aH, 1, 1cH, 1, 1eH
    DB	1, 20H, 1, 22H, 1, 24H, 1, 26H
    DB	1, 28H, 1, 2aH, 1, 2cH, 1, 2eH
    DB	1, 30H, 1, 32H, 1, 34H, 1, 36H
    DB	1, 38H, 1, 3aH, 1, 3cH, 1, 3eH
    DB	1, 40H, 1, 42H, 1, 44H, 1, 46H
    DB	1, 48H, 1, 4aH, 1, 4cH, 1, 4eH
    DB	1, 50H, 1, 52H, 1, 54H, 1, 56H
    DB	1, 58H, 1, 5aH, 1, 5cH, 1, 5eH
    DB	1, 60H, 1, 62H, 1, 64H, 1, 66H
    DB	1, 68H, 1, 6aH, 1, 6cH, 1, 6eH
    DB	1, 70H, 1, 72H, 1, 74H, 1, 76H
    DB	1, 78H, 1, 7aH, 1, 7cH, 1, 7eH
    DB	1, 80H, 1, 82H, 1, 84H, 1, 86H
    DB	1, 88H, 1, 8aH, 1, 8cH, 1, 8eH
    DB	1, 90H, 1, 92H, 1, 94H, 1, 96H
    DB	1, 98H, 1, 9aH, 1, 9cH, 1, 9eH
    DB	1, 0a0H, 1, 0a2H, 1, 0a4H, 1, 0a6H
    DB	1, 0a8H, 1, 0aaH, 1, 0acH, 1, 0aeH
    DB	1, 0b0H, 1, 0b2H, 1, 0b4H, 1, 0b6H
    DB	1, 0b8H, 1, 0baH, 1, 0bcH, 1, 0beH
    DB	1, 0c0H, 1, 0c2H, 1, 0c4H, 1, 0c6H
    DB	1, 0c8H, 1, 0caH, 1, 0ccH, 1, 0ceH
    DB	1, 0d0H, 1, 0d2H, 1, 0d4H, 1, 0d6H
    DB	1, 0d8H, 1, 0daH, 1, 0dcH, 1, 0deH
    DB	1, 0e0H, 1, 0e2H, 1, 0e4H, 1, 0e6H
    DB	1, 0e8H, 1, 0eaH, 1, 0ecH, 1, 0eeH
    DB	1, 0f0H, 1, 0f2H, 1, 0f4H, 1, 0f6H
    DB	1, 0f8H, 1, 0faH, 1, 0fcH, 1, 0feH
_shifttabletable:
    DW	offset DGROUP:L$101
    DW	offset DGROUP:L$102
    DW	offset DGROUP:L$103
    DW	offset DGROUP:L$104
    DW	offset DGROUP:L$105
    DW	offset DGROUP:L$106
    DW	offset DGROUP:L$107
    DW	offset DGROUP:L$108
L$109:
    DB	80H, 40H, 20H, 10H, 8, 4, 2, 1
L$110:
    DW	offset L$7
    DW	offset L$7
    DW	offset L$11
    DW	offset L$11
    DW	offset L$12
    DW	offset L$13
    DW	offset L$14
    DW	offset L$15
    DW	offset L$16
    DW	offset L$17
    DW	offset L$18
    DW	offset L$19
    DW	offset L$20
    DW	offset L$21
    DW	offset L$22
    DW	offset L$23
    DW	offset L$24
    DW	offset L$25
    DW	offset L$26
    DW	offset L$27
    DW	offset L$28
    DW	offset L$29
L$111:
    DB	0, 0
L$112:
    DW	offset L$31
    DW	offset L$35
    DW	offset L$33
    DW	offset L$38
_px:
    DB	0, 0
_py:
    DB	0, 0
_pdrawmode:
    DB	18H
_fontcolor:
    DB	0fH, 0, 0
L$113:
    DB	0, 0
L$114:
    DB	0, 0
L$115:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$116:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$117:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$118:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$119:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$120:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$121:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$122:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$123:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$124:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$125:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$126:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$127:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$128:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$129:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$130:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$131:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$132:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$133:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$134:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$135:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$136:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$137:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$138:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$139:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$140:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$141:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$142:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$143:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$144:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$145:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
L$146:
    DB	06H DUP(0,0,0,0,0,0,0,0)
    DB	0, 0
_bufferwidth:
    DB	0, 0
_bufferheight:
    DB	0, 0
L$147:
    DB	0, 0
L$148:
    DB	0, 0
L$149:
    DB	0, 0
L$150:
    DB	0, 0
L$151:
    DB	0, 0
L$152:
    DB	0, 0
    DW	offset L$57
    DW	offset L$59
    DW	offset L$61

_DATA		ENDS
		END
