	AREA storage,DATA
array
	; Total is 232 for the example list
	DCD 3, 7, 31, 193, 2, -5, 1
array_end

	AREA main,CODE
	EXPORT asm_main
asm_main
	; Your code goes here
	LDR r0, =array
	LDR r1, =array_end
	MOV r2, #0
	MOV r3, #0
	
loop
	LDR r2, [r0], #4	; Load the value at r0 into r2 and increment r0 by 1
	CMP r2, #0			; Compare r2 to 0.
	BLT mod				; Branch to mod if r2 is less than 0 (signed)
mod_ret
	ADD r3, r3, r2		; Add r2 and r3 and place it in r3
	CMP r0, r1			; Compare r0 and r1 and see if r0 is at array_end
	BLO loop			; If r0 < r1 loop
 
	B .			; Loop forever

mod
	RSB r2, r2, #0 		; Subtract r2 from 0
	B mod_ret
 
	ALIGN
	END
