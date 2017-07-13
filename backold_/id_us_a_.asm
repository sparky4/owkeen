.8087
		PUBLIC	US_InitRndT_
		PUBLIC	US_RndT_
DGROUP		GROUP	_DATA
ID_US_A_TEXT		SEGMENT	WORD PUBLIC USE16 'CODE'
		ASSUME CS:ID_US_A_TEXT, DS:DGROUP, SS:DGROUP
_US_InitRndT:
	push		bp
	mov		bp,sp
	push		si
	push		di
	mov		ax,word ptr 6[bp]
	or		ax,ax
	jne		L$1
	mov		dx,0
	jmp		L$2
	nop
L$1:
	mov		ah,2cH
	int		21H
	and		dx,0ffH
L$2:
	mov		word ptr DGROUP:L$3,dx
	pop		di
	pop		si
	pop		bp
	retf
_US_RndT:
	mov		bx,word ptr DGROUP:L$3
	inc		bx
	and		bx,0ffH
	mov		word ptr DGROUP:L$3,bx
	mov		al,byte ptr DGROUP:L$4[bx]
	xor		ah,ah
	retf
ID_US_A_TEXT		ENDS
_DATA		SEGMENT	WORD PUBLIC USE16 'DATA'
L$3:
    DB	0, 0
L$4:
    DB	0, 8, 6dH, 0dcH, 0deH, 0f1H, 95H, 6bH
    DB	4bH, 0f8H, 0feH, 8cH, 10H, 42H, 4aH, 15H
    DB	0d3H, 2fH, 50H, 0f2H, 9aH, 1bH, 0cdH, 80H
    DB	0a1H, 59H, 4dH, 24H, 5fH, 6eH, 55H, 30H
    DB	0d4H, 8cH, 0d3H, 0f9H, 16H, 4fH, 0c8H, 32H
    DB	1cH, 0bcH, 34H, 8cH, 0caH, 78H, 44H, 91H
    DB	3eH, 46H, 0b8H, 0beH, 5bH, 0c5H, 98H, 0e0H
    DB	95H, 68H, 19H, 0b2H, 0fcH, 0b6H, 0caH, 0b6H
    DB	8dH, 0c5H, 4, 51H, 0b5H, 0f2H, 91H, 2aH
    DB	27H, 0e3H, 9cH, 0c6H, 0e1H, 0c1H, 0dbH, 5dH
    DB	7aH, 0afH, 0f9H, 0, 0afH, 8fH, 46H, 0efH
    DB	2eH, 0f6H, 0a3H, 35H, 0a3H, 6dH, 0a8H, 87H
    DB	2, 0ebH, 19H, 5cH, 14H, 91H, 8aH, 4dH
    DB	45H, 0a6H, 4eH, 0b0H, 0adH, 0d4H, 0a6H, 71H
    DB	5eH, 0a1H, 29H, 32H, 0efH, 31H, 6fH, 0a4H
    DB	46H, 3cH, 2, 25H, 0abH, 4bH, 88H, 9cH
    DB	0bH, 38H, 2aH, 92H, 8aH, 0e5H, 49H, 92H
    DB	4dH, 3dH, 62H, 0c4H, 87H, 6aH, 3fH, 0c5H
    DB	0c3H, 56H, 60H, 0cbH, 71H, 65H, 0aaH, 0f7H
    DB	0b5H, 71H, 50H, 0faH, 6cH, 7, 0ffH, 0edH
    DB	81H, 0e2H, 4fH, 6bH, 70H, 0a6H, 67H, 0f1H
    DB	18H, 0dfH, 0efH, 78H, 0c6H, 3aH, 3cH, 52H
    DB	80H, 3, 0b8H, 42H, 8fH, 0e0H, 91H, 0e0H
    DB	51H, 0ceH, 0a3H, 2dH, 3fH, 5aH, 0a8H, 72H
    DB	3bH, 21H, 9fH, 5fH, 1cH, 8bH, 7bH, 62H
    DB	7dH, 0c4H, 0fH, 46H, 0c2H, 0fdH, 36H, 0eH
    DB	6dH, 0e2H, 47H, 11H, 0a1H, 5dH, 0baH, 57H
    DB	0f4H, 8aH, 14H, 34H, 7bH, 0fbH, 1aH, 24H
    DB	11H, 2eH, 34H, 0e7H, 0e8H, 4cH, 1fH, 0ddH
    DB	54H, 25H, 0d8H, 0a5H, 0d4H, 6aH, 0c5H, 0f2H
    DB	62H, 2bH, 27H, 0afH, 0feH, 91H, 0beH, 54H
    DB	76H, 0deH, 0bbH, 88H, 78H, 0a3H, 0ecH, 0f9H
    DB	0, 0, 0, 0, 0, 0, 0, 0
    DB	0, 0, 0, 0, 0, 0, 0, 0
    DB	0, 0, 0, 0, 0, 0, 0, 0
    DB	0, 0, 0, 0, 0, 0, 0, 0
    DB	0, 0, 0, 0, 0, 0, 0, 0
    DB	1, 0, 1, 0, 2, 0, 3, 0
    DB	5, 0, 8, 0, 0dH, 0, 15H, 0
    DB	36H, 0, 4bH, 0, 81H, 0, 0ccH, 0
    DB	43H, 1, 0fH, 2, 52H, 3, 61H, 5
    DB	0b3H, 8

_DATA		ENDS
		END
