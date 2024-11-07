	AREA mainarea,CODE

	EXPORT process 
		
		
process
	STMFD sp!, {r3, lr}				; Store link register
	LDMIA r0, {r4, r5, r6, r7}		; Load the structure into scratch registers
	ADD r5, r7, r5					; y = x + y
	STMIA r0, {r4, r5}				; Update the value of y in the structure
	MOV r0, r4						; Move r6 (ptr field) into r0 as branch argument
	BLX r6							; Branch to address at r7 (callback function)
	LDMFD sp!, {r3, pc}				; Implicit return
	

	END
