%define COM1 0x3F8
%define MEMORY_START 0x400000

; Okay, pmode! let's go 32-bit.
[bits 32]

extern c_mode
extern test_int
extern kernel_start_old
extern kernel_start
extern kernel_tick
extern kernel_key_pressed
extern kernel_end
extern print_int
extern kernel_stack

align 4

; ----------------------------------------------------------------------------
; Set up the PICs for the IVT->IDT transition


	; ICW1 - init
	mov al, 0x11		; Init
	out 0x20, al		; PIC1
	mov al, 0x11		; Init
	out 0xA0, al		; PIC2

	; ICW2 - map IRQs to ints
	mov al, 0x20		; Map IRQs 0-7 to ints 32-39
	out 0x21, al		; PIC1
	mov al, 0x28		; Map IRQs 8-15 to ints 40-47
	out 0xA1, al		; PIC2

	; ICW3 - connect slave PIC to master PIC on IRQ2
	mov al, 0x04
	out 0x21, al
	mov al, 0x02
	out 0xA1, al

	; ICW4
	mov al, 0x01
	out 0x21, al
	mov al, 0x01
	out 0xA1, al
	
  ; Mask the PICs
  mov al, 0x00    ; All irqs
  out 0x21, al    ; PIC1
  out 0xA1, al    ; PIC2


	push edi
	mov edi, idt

	%macro ADD_IDT_ENTRY 1

		push eax

		mov eax, %1
		mov word [edi], ax
		add edi, 2

		mov word [edi], 0x0008
		add edi, 2

		mov byte [edi], 0x00
		add edi, 1

		mov byte [edi], 0x8E
		add edi, 1

		shr eax, 16
		mov word [edi], ax
		add edi, 2

		pop eax

	%endmacro

	ADD_IDT_ENTRY int00_handler
	ADD_IDT_ENTRY int01_handler
	ADD_IDT_ENTRY int02_handler
	ADD_IDT_ENTRY int03_handler
	ADD_IDT_ENTRY int04_handler
	ADD_IDT_ENTRY int05_handler
	ADD_IDT_ENTRY int06_handler
	ADD_IDT_ENTRY int07_handler
	ADD_IDT_ENTRY int08_handler
	ADD_IDT_ENTRY int09_handler
	ADD_IDT_ENTRY int10_handler
	ADD_IDT_ENTRY int11_handler
	ADD_IDT_ENTRY int12_handler
	ADD_IDT_ENTRY int13_handler
	ADD_IDT_ENTRY int14_handler
	ADD_IDT_ENTRY int15_handler
	ADD_IDT_ENTRY int16_handler
	ADD_IDT_ENTRY int17_handler
	ADD_IDT_ENTRY int18_handler
	ADD_IDT_ENTRY int19_handler
	ADD_IDT_ENTRY int20_handler
	ADD_IDT_ENTRY int21_handler
	ADD_IDT_ENTRY int22_handler
	ADD_IDT_ENTRY int23_handler
	ADD_IDT_ENTRY int24_handler
	ADD_IDT_ENTRY int25_handler
	ADD_IDT_ENTRY int26_handler
	ADD_IDT_ENTRY int27_handler
	ADD_IDT_ENTRY int28_handler
	ADD_IDT_ENTRY int29_handler
	ADD_IDT_ENTRY int30_handler
	ADD_IDT_ENTRY int31_handler

	ADD_IDT_ENTRY irq00_handler
	ADD_IDT_ENTRY irq01_handler
	ADD_IDT_ENTRY irq02_handler
	ADD_IDT_ENTRY irq03_handler
	ADD_IDT_ENTRY irq04_handler
	ADD_IDT_ENTRY irq05_handler
	ADD_IDT_ENTRY irq06_handler
	ADD_IDT_ENTRY irq07_handler
	ADD_IDT_ENTRY irq08_handler
	ADD_IDT_ENTRY irq09_handler
	ADD_IDT_ENTRY irq10_handler
	ADD_IDT_ENTRY irq11_handler
	ADD_IDT_ENTRY irq12_handler
	ADD_IDT_ENTRY irq13_handler
	ADD_IDT_ENTRY irq14_handler
	ADD_IDT_ENTRY irq15_handler

	pop edi

	lidt [idt_ptr]		; Okay, IDT loaded!


	; Set up the PIT (Programmable Interval Timer)
	; 1193180/59659(0xE90B) = 20

	PIT_INTERVAL equ 0xE90B
	
	mov al, 00110100b	; bit 7-6 (00)  = timer counter 0
				; bit 5-4 (11)  = write LSB then MSB
				; bit 3-1 (010) = count in mode 2
				; bit 0   (0)   = binary counter
	out 0x43, al		; Send to PIT Mode Control Register
	jmp pit_step1
pit_step1:

	mov cx, PIT_INTERVAL

	mov al, cl
	out 0x40, al		; Least Significant Byte of interval
	jmp pit_step2
pit_step2:
	mov al, ch
	out 0x40, al		; Most Significant Byte of interval
	jmp pit_step3
pit_step3:

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;; Set up serial communications ;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; Interrupts off
	mov dx, COM1+1
	mov al, 0
	out dx, al

	; DLAB on. (set baud rate divisor)
	mov dx, COM1+3
	mov al, 0x80
	out dx, al

	;                  Baud Rate                      Baud Rate
	; Baud Rate         Divisor         Baud Rate      Divisor
	;
	;      50            900h              2400          30h
	;     110            417h              3600          20h
	;     150            300h              4800          18h
	;     300            180h              7200          10h
	;     600             C0h              9600          0Ch
	;    1200             60h             19200          06h
	;    1800             40h             38400          03h
	;    2000             3Ah            115200          01h
      
	; Set baud rate's low byte.
	mov dx, COM1+0
	mov al, 0x30		; 2,400 baud
	out dx, al
  
	; Set baud rate's high byte
	mov dx, COM1+1
	mov al, 0x00
	out dx, al
  
	; Set some settings, turn off DLAB
	mov dx, COM1+3
	mov al, 03		; 8 bits, no parity, 1 stop bit
	out dx, al
  
	; FIFO control register.
	mov dx, COM1+2
	mov al, 0xC7
	out dx, al
  
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;; End of serial communications setup ;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
; ----------------------------------------------------------------------------
; Count the available memory before enabling paging.

    mov eax, MEMORY_START
check_mem_loop:
    mov ebx, [eax] ; save whatever was at [eax]
    mov [eax], dword 0xDEADBEEF
    cmp [eax], dword 0xDEADBEEF
    jne check_mem_done
    mov [eax], ebx ; restore [eax]
    add eax, 0x1000
    jmp check_mem_loop
check_mem_done:
    mov [memory_end], eax
    

; ----------------------------------------------------------------------------
; Set up paged mode
    
    ; Add low mem page table to page directory.
    mov eax, low_mem_page_table
    or eax, 3               ; flags
    mov [page_dir], eax     ; add a page table for 0x00000000-0x003FFFFF

    ; Add page space page table to page directory.
    mov eax, scheduler_page_table
    or eax, 3               ; flags
    mov [page_dir+4], eax   ; add a page table for 0x00400000-0x007FFFFF

    ; Add the other pages as non-existent.
    mov ecx, 2
    mov ebx, 2
page_dir_loop:
    mov [page_dir+ecx*4], ebx
    inc ecx
    cmp ecx, 1024
    jl page_dir_loop
    
    
    ; Write the low mem page table entries. Map them directly.
    mov ebx, 3 ; flags
    mov ecx, 0
page_table_loop:
    mov [low_mem_page_table+ecx*4], ebx
    add ebx, 4096
    inc ecx
    cmp ecx, 1024
    jl page_table_loop
    
    ; Write the scheduler mem page table entries. Map them directly.
    ;mov ebx, 3 ; flags
    mov ecx, 0
page_table_loop2:
    mov [scheduler_page_table+ecx*4], ebx
    add ebx, 4096
    inc ecx
    cmp ecx, 1024
    jl page_table_loop2
    


    ; Point cr3 to page directory
    mov eax, page_dir
    mov cr3, eax
    
    ; Start paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    test_label:
    ;mov eax, [page_dir]
    ;xor eax, 0x00000020
    ;mov [page_dir], eax
    ;mov eax, [page_dir]
    ;mov ebx, [page_dir+4]
    ;mov ecx, low_mem_page_table
    ;mov edx, scheduler_page_table
    
    ;mov esi, c_mode
    ;mov edi, test_int
    ;mov eax, [c_mode]
    ;mov ebx, [c_mode+4]
    ;mov ecx, [test_int]
    ;jmp near test_label

    jmp near paging_align
paging_align:

;  --------------------------------------------------------------------------
; | Ok, kernel is started and set up! Let's start doing what we should,      |
; | namely displaying the uptime.                                            |
;  --------------------------------------------------------------------------


    ; Jump to kernel.
    jmp c_mode

global print_esp
print_esp:
    push dword 21
    push dword 0
    push dword 20
    push esp
    call print_int

main_loop:
	times 2 hlt
	jmp main_loop

; ----------------------------------------------------------------------------

idt:
    times 48*8 db 0x0
idt_end:

idt_ptr:
    dw idt_end-idt-1
    dd idt

idt_entry_ptr:
    dd idt

; ----------------------------------------------------------------------------

global os_uptime
os_uptime       dd 0
                dd 0
                
global memory_end
memory_end      dd 0
                
; ----------------------------------------------------------------------------

%include "ints.asm"

; ----------------------------------------------------------------------------
; http://www.google.com/codesearch?hl=en&q=+cr3+cr0+paging+nasm+show:kq7ABRgMycM:psJtMiUrqy8:lF6NCKOGkns&sa=N&cd=7&ct=rc&cs_p=http://kos.enix.org/pub/littleos.zip&cs_f=KERNEL/START.ASM#a0

align 4096
global page_dir
page_dir:
    times 1024 dd 0x11111111

align 4096
global low_mem_page_table
low_mem_page_table:
    times 1024 dd 0x22222222

align 4096
global scheduler_page_table
scheduler_page_table:
    times 1024 dd 0x33333333

;--END OF KERNEL--
;EOF

