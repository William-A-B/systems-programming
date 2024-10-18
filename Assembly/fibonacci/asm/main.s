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
	ADD r3, r3, r2		; Compute next value - current value = current value + previous value
						; Work out a way to update r2 with the previous value without requiring to use r4
	
	

	B .			; Loop forever

	ALIGN
	END
