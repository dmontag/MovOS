
[bits 16]
[org 0]

jmp bs_start

boot_drive db 0

; http://pdos.csail.mit.edu/6.828/2004/readings/i386/s05_01.htm
_gdt_0:			   	; Dummy
dw gdt_size
dd _gdt_0 + 0x7C00
dw 0x0000

_gdt_1:			   	; Entry 1 (8h)  | code, ring0
db 0xFF, 0xFF, 0x00, 0x00
db 0x00, 0x9A, 0xCF, 0x00

_gdt_2:			   	; Entry 2 (10h) | data, ring0
db 0xFF, 0xFF, 0x00, 0x00
db 0x00, 0x92, 0xCF, 0x00

;_gdt_3:			; Entry 3 (18h) | code, ring3
;db 0xFF, 0xFF, 0x00, 0x00
;db 0x00, 0xFA, 0xCF, 0x00

;_gdt_4:			; Entry 4 (20h) | data, ring3
;db 0xFF, 0xFF, 0x00, 0x00
;db 0x00, 0xF2, 0xCF, 0x00


gdt_size EQU $ - _gdt_0 - 1

;data
boot_loading	db 'MovOS bootloader v0.5c loading... ',0
boot_done   	db 'done',0
boot_error      db 'error loading OS',0

;------------------------------------------------------------------

bs_print:			; Print function.
	lodsb
	or al, al
	je bs_done
	mov ah, 0x0E
	mov bx, 7
	int 0x10
	jmp bs_print
bs_done:
	ret

;------------------------------------------------------------------

bs_cls:			; Clear screen function.
	push es	

	mov cx, 0x07D0	; 2000d
	mov dx, 0xB800
	mov es, dx
	xor di, di
	mov ax, 0x0800	; Attribute white on black (07), no character (00).
	
	rep stosw

	mov ah, 2	; Function 2.
	mov bh, 0	; Page 0.
	mov dh, 0	; Row 0.
	mov dl, 0	; Col 0.
	int 0x10	; Move cursor.

	pop es
	ret

;------------------------------------------------------------------

bs_read_tries   db  3
bs_read_die:
    mov si, boot_error
    call bs_print
bs_read_die_loop:
    hlt
    jmp bs_read_die_loop
bs_read_fail:
	dec byte [bs_read_tries]
	jz bs_read_die
bs_read:                    ; Load OS from disk function
	mov dx, 0	
	mov ax, 0
	int 0x13                ; Reset floppy drive.

%define bs_read_blocks 64

	mov ax, 0x07E0
	mov es, ax
	mov bx, 0
	
	mov ah, 2               ; Read function.
	mov al, bs_read_blocks  ; Read as many sectors as possible
	mov ch, 0               ; Cylinder 0.
	mov cl, 2               ; Sector 1 (bootsector).
	mov dh, 0               ; Head 0.
	mov dl, [boot_drive]    ; Drive 0 = floppy, drive 0x80 = HDD0.

    int 0x13                ; Read disk.
    jc bs_read_fail         ; On error, try again.

	ret

;------------------------------------------------------------------

;bs_copy:
;    mov ax, 0xFFFF
;    mov es, ax
;    mov di, 0x0010
;    mov si, bs_end
;bs_copy_loop_init:
;    mov cx, 0xFFFF
;bs_copy_loop:
;    lodsb
;    stosb
;    loop bs_copy_loop
;
;    ret

;------------------------------------------------------------------
;------------------------------------------------------------------

bs_start:
	mov ax, 0x07C0                  ; Set segments,
	mov ds, ax			
	mov es, ax			
	cli                             ; No ints.
	mov ax,0x8000                  	; Under the kernel loading area.
	mov ss,ax                      	
	mov sp,0xFFFF                  	; Start stack at top.

    mov [boot_drive], dl            ; Save which drive we're booting from
    
	call bs_cls	    	            ; Clear the screen.
	mov si, boot_loading            ; Load boot message address.
	call bs_print                   ; Print it.

    cmp byte [bs_end], 0xFE
    jz bs_skip_read
	call bs_read                    ; Read in the kernel.
bs_skip_read:

	mov si, boot_done               ; Load message address.
	call bs_print                   ; Print it.

	; Enable A20 / Thanks Two9A
	call empty_kbuf                 ; Empty keyboard buffer
	mov al, 0xD1                    ; Writing something to output port
	out 0x64, al                    ; Do it.
	call empty_kbuf                 ; Empty keyboard buffer again
	mov al, 0xDF                    ; A20 on
	out 0x60, al                    ; Send to keyboard.
	call empty_kbuf                 ; Empty buffer yet again
	jmp a20_done

empty_kbuf:
	in al, 0x64                     ; Wait for keyboard buffer...
	test al, 2                      ; ...to become empty
	loopnz empty_kbuf
	ret
  
a20_done:
	lgdt [_gdt_0]                   ; Load global descriptor table.
	mov eax, cr0                    ; Fetch system register cr0.
	or al, 1                        ; Set the pe (protect enable) bit.
	mov cr0, eax                    ; Put it back modified.

	jmp dword 0x8:0x00008000       ; Jump to kernel. After the jump 
					                ; we're in pmode.

times 510-($-$$) db 0			    ; Pad sector.
dw 0xAA55
;--END OF BOOTSECTOR--

bs_end:
    db 0xFE                         ; Token for identifying whether we're 
                                    ; netbooted or not.
    times 0x1FF db 0                ; Padding needed to place kernel code at
                                    ; 0x8000.
