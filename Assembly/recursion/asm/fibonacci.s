	AREA mainarea,CODE
	EXPORT fib
	


fib
	STMFD sp!, {r3, r4, r5, lr}		; Store link register on stack
	
	CMP r0, #2						; Compare r0 with 2. 
	BLS fib_base_case				; Branch if equal or less than
	
	MOV r4, r0						; Move current index r0 into r4
	
	SUB r0, r4, #1					; r0 = r4 - 1 (n-1)
	BL fib							; Branch to fib with (n-1)
	MOV r5, r0						; Store (n-1) in r5
		
	SUB r0, r4, #2					; r0 = r4 - 2 (n-2)
	BL fib							; Branch to fib with (n-2)
	
	ADD r0, r0, r5					; r0 = r0 (n-2) + r5 (n-1)
	
	LDMFD sp!, {r3, r4, r5, pc}		; Retrieve lr into pc, implicit return
	
	
fib_base_case
	MOV r0, #1						; If n <= 2, r0 is always 1
	LDMFD sp!, {r3, r4, r5, pc}		; Restore lr into pc and implicit return

	END
	

; OLD SOLUTION
;fib
;	STMFD sp!, {lr}		; Store link register on stack
;	
;	CMP r0, #2			; Compare r0 with 2. 
;	BLS fib_base_case	; Branch if equal or less than
;		
;	STMFD sp!, {r0}		; Store r0 on the stack
;	
;	SUB r0, r0, #1		; r0 = r0 - 1 (n-1)
;	BL fib				; Branch to fib with (n-1)
;	MOV r1, r0			; Store (n-1) in r1
;				
;	LDMFD sp!, {r0}		; Retrieve r0 from stack		
;	STMFD sp!, {r1}		; Store r1 on the stack
;	
;	SUB r0, r0, #2		; r0 = r0 - 2 (n-2)
;	BL fib				; Branch to fib with (n-2)
;	LDMFD sp!, {r1}
;	ADD r0, r0, r1		; r0 = r0 (n-2) + r1 (n-1)
;	
;	LDMFD sp!, {pc}		; Pop lr into pc, implicit return
;	
;	
;fib_base_case
;	MOV r0, #1			; If n <= 2, r0 is always 1
;	LDMFD sp!, {pc}		; Restore lr into pc, implicit return
;		
;		
;	END
		