	AREA mainarea,CODE
	EXPORT asm_main
asm_main

	; Your instructions go here...
	
	MOV r0, #2
	MOV r1, #0
	ADD r1, r0, r0, LSL #1
	ADD r1, r0, r1, LSL #2

	B .			; Loop forever

	END
