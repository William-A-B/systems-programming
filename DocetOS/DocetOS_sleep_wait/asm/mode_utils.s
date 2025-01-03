	AREA utils, CODE, READONLY
		
	EXPORT getPSR
	EXPORT getCONTROL
		
getPSR
	MRS r0, PSR			; Move PSR into r0
	BX lr				; Return
	
getCONTROL
	MRS r0, CONTROL		; Move CONTROL into r0
	BX lr				; Return

	END
		