	AREA mainarea,CODE
	EXPORT fib
		
		
fib
	;PUSH {lr}			; Store link register on stack
	STMFD sp!, {lr}
	
	CMP r0, #2			; Compare r0 with 2. 
	BLS fib_base_case	; Branch if equal or less than
	
	;PUSH {r0}			; Store r0 on the stack
	STMFD sp!, {r0}
	
	SUB r0, r0, #1		; r0 = r0 - 1 (n-1)
	BL fib				; Branch to fib with (n-1)
	MOV r1, r0			; Store (n-1) in r1
	
	;POP {r0}			; Retrieve r0 from stack
	LDMFD sp!, {r0}
	;PUSH {r1}			; Store r1 on the stack
	STMFD sp!, {r1}
	
	SUB r0, r0, #2		; r0 = r0 - 2 (n-2)
	BL fib				; Branch to fib with (n-2)
	;POP{r1}
	LDMFD sp!, {r1}
	ADD r0, r0, r1		; r0 = r0 (n-2) + r1 (n-1)
	
	LDMFD sp!, {pc}		; Pop lr into pc, so no need to branch as pc will automatically branch to lr
	
	;POP {lr}			; Restore lr
	;BX lr				; Return to caller
	
	
fib_base_case
	MOV r0, #1			; If n <= 2, r0 is always 1
	LDMFD sp!, {pc}
	;POP {lr}			; Restore lr
	;BX lr				; Return to caller
		
		
	END
		