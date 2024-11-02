	AREA mainarea,CODE
		
	EXPORT calculate


calculate
	;STMFD sp!, {r0, lr}
	ADD r0, r0, r0, LSL #2		; r0 = r0 + 4r0
	ADD r0, r1, r0				; r0 = r1 + r0
	
	BX lr
	
	END
