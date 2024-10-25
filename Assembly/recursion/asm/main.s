	AREA mainarea,CODE
	EXPORT asm_main
	IMPORT fib
		
asm_main
	MOV r0, #8
	
	PUSH {lr}
	BL fib
	POP {lr}
	

	B .			; Loop forever

	ALIGN
	END
