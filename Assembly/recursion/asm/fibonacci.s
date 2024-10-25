	AREA mainarea,CODE
	EXPORT fib
		
		
fib
	PUSH {lr}

	CMP r0, #2
	BLS fib_base_case
	
	PUSH {r0}
	
	SUB r0, r0, #1		; r0 = r0 - 1 (n-1)
	BL fib				; Branch to fib with (n-1)
	MOV r1, r0			; Store (n-1) in r1
	
	POP {r0}
	PUSH {r1}
	
	SUB r0, r0, #2		; r0 = r0 - 2 (n-2)
	BL fib				; Branch to fib with (n-2)
	POP{r1}
	ADD r0, r0, r1		; r0 = r0 (n-2) + r1 (n-1)
	
	POP {lr}			; Restore lr
	BX lr				; Return to caller
	
	
fib_base_case
	MOV r0, #1			; If n <= 2, r0 is always 1
	POP {lr}			; Restore lr
	BX lr				; Return to caller
	
		
	
;		
;	CMP r0, #3		; Is index one or two
;	MOV r2, #1		; Set value to 1
;	BMI lr			; Return
;	
;					; Copy current index
;					; Loop recursively
;					; Decrement current index by one each loop
;					; Add previous value and previous previous value
;					; Compare current index to zero
;	STR{IA} lr, [sp]
;	SUB r0, #1
;	CMP r0, #3
;	BEQ check
;check_end
;	
;	BL fib
;	
;	
;	
;	
;	
;check
;	LDR r14, [r13]
;	BL fib
;	
;	
;	
;	
;	
;	
;	
;	
;	MOV r1, #3
;	CMP r1, r0
;	
;	LDR{DB} lr, sp
;	BEQ lr
;	
;	
;	STR{IA} lr, sp
;	BL fib
;	
;	
;	
;	
		
		
	END
		