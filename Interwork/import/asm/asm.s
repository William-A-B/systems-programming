	AREA mainarea,CODE
		
	EXPORT fib
	IMPORT report
	;EXPORT calculate

;calculate
;	;STMFD sp!, {r0, lr}
;	ADD r0, r0, r0, LSL #2		; r0 = r0 + 4r0
;	ADD r0, r1, r0				; r0 = r1 + r0
;	
;	BX lr
;	
;	END
	
fib
	STMFD sp!, {lr, r3}		; Store link register on stack
	
	CMP r0, #2			; Compare r0 with 2. 
	BLS fib_base_case	; Branch if equal or less than
	
	STMFD sp!, {r0, r3}		; Store r0 on the stack
	
	SUB r0, r0, #1		; r0 = r0 - 1 (n-1)
	BL fib				; Branch to fib with (n-1)
	MOV r1, r0			; Store (n-1) in r1
	
	LDMFD sp!, {r0, r3}		; Retrieve r0 from stack
	STMFD sp!, {r1, r3}		; Store r1 on the stack
	
	SUB r0, r0, #2		; r0 = r0 - 2 (n-2)
	BL fib				; Branch to fib with (n-2)
	
	LDMFD sp!, {r1, r3}		; Load r1 from the stack
	ADD r0, r0, r1		; r0 = r0 (n-2) + r1 (n-1)
	
	STMFD sp!, {r0, r1}
	BL report			; Branch to C report function to print the current value of r0
	LDMFD sp!, {r0, r1}
	LDMFD sp!, {pc, r3}		; Retrieve lr into pc, implicit return
	
	
fib_base_case
	MOV r0, #1			; If n <= 2, r0 is always 1
	LDMFD sp!, {pc, r3}		; Restore lr into pc and return

	END
