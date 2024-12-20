    AREA OS_func, CODE, READONLY

; Export function locations
    EXPORT SVC_Handler
    EXPORT PendSV_Handler
    EXPORT _task_switch
    EXPORT _task_init_switch

; Import global variables
    IMPORT _currentTCB
    IMPORT _OS_schedule

; Import SVC routines
    IMPORT _OS_enable_systick_delegate
    IMPORT _OS_taskExit_delegate
    IMPORT _OS_yield_delegate
    IMPORT _OS_schedule_delegate
	IMPORT check_status_delegate
    
SVC_Handler
	; r7 contains requested handler, on entry
    TST     lr, #4
    ITE     EQ
    MRSEQ   r0, MSP
    MRSNE   r0, PSP
    ; r0 now contains the SP that was in use
    ; Check if requested handler in the table
    CMP     r7, #((SVC_tableEnd - SVC_tableStart)/4)
    ; If not, return
    BXGE    lr
    ; Branch to the right handler
    ; Remember, the SP is in r0
    LDR     r2, =SVC_tableStart
    LDR     pc, [r2, r7, lsl #2]
    
    ALIGN
SVC_tableStart
    DCD _OS_enable_systick_delegate
    DCD _OS_taskExit_delegate
    DCD _OS_yield_delegate
    DCD _OS_schedule_delegate
	DCD check_status_delegate
SVC_tableEnd

    ALIGN
PendSV_Handler
    STMFD   sp!, {r4, lr} ; r4 included for stack alignment
    BL      _OS_schedule
    LDMFD   sp!, {r4, lr}
_task_switch
    ; r0 contains nextTCB (OS_TCB *)
    ; Load r2 = &_currentTCB (OS_TCB **), r1 = _currentTCB (OS_TCB *, == OS_StackFrame **)
    LDR     r2, =_currentTCB
    LDR     r1, [r2]
    ; Compare _currentTCB to nextTCB: if equal, go home
    CMP     r1, r0
    BXEQ    lr
    ; If not, stack remaining process registers (pc, PSR, lr, r0-r3, r12 already stacked)
    MRS     r3, PSP
    STMFD   r3!, {r4-r11}
    ; Store stack pointer
    STR     r3, [r1]
    ; Load new stack pointer
    LDR     r3, [r0]
    ; Unstack process registers
    LDMFD   r3!, {r4-r11}
    MSR     PSP, r3
    ; Update _currentTCB
    STR     r0, [r2]
    ; Clear exclusive access flag
    CLREX
    BX      lr

    ALIGN
_task_init_switch
    ; Assume thread mode on entry
    ; Initial task is the idle task
    ; On entry r0 = OS_idleTCB_p (OS_TCB *)
    ; Load r1 = *(r0) (OS_StackFrame *)
    LDR     r1, [r0]
    ; Update PSP
    MSR     PSP, r1
    ; Update _currentTCB
    LDR     r2, =_currentTCB
    STR     r0, [r2]
    ; Switch to using PSP instead of MSP for thread mode (bit 1 = 1)
    ; Also lose privileges in thread mode (bit 0 = 1) and disable FPU (bit 2 = 0)
    MOV     r2, #3
    MSR     CONTROL, r2
    ; Instruction barrier (stack pointer switch)
    ISB
	; Check to see if the scheduler is preemptive before
    ; This SVC call should be handled by _svc_OS_enable_systick()
	MOV		r7, #0
    SVC     0x00
    ; Continue to the idle task
    
    ALIGN
    ; This SVC call should be handled by _svc_OS_schedule()
    ; It causes a switch to a runnable task, if possible
	MOV 	r7, #3
    SVC     0x00
_idle_task
    ; The following line is commented out because it doesn't play nicely with the debugger.
    ; For deployment, uncomment this line and the CPU will sleep when idling, waking only to
    ; handle interrupts.
;   WFI
    B       _idle_task
    
    ALIGN
    END
