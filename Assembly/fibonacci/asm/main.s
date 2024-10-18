	AREA storage,DATA
array
	SPACE 4*20	; Declares a 20-word storage area
array_end

	AREA mainarea,CODE
	EXPORT asm_main
asm_main
	; Your code goes here
	LDR r0, =array
	LDR r1, =array_end
	MOV r2, #1			; Previous value
	MOV r3, #1			; Current value
loop
	STR r3, [r0], #4	; Store current value in array then increment by 4 bytes
	ADD r4, r3, r2		; Compute next value - current value = current value + previous value
	MOV r2, r3			; Update r2 with r3 (The previous value)
	MOV r3, r4			; Update r3 with the new current value from r4
	CMP r0, r1			; Compare r0 to r1, if r0 < r1 set the N flag
	BLO loop			; If r0 < r1 go to loop label
	

	B .			; Loop forever

	ALIGN
	END
