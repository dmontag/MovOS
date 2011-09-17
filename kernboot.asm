
;--START OF KERNEL--

%define COM1 0x3F8
%define MEMORY_START 0x400000
%define KERNEL_START 0x100000

; Okay, pmode! let's go 32-bit.
[bits 32]

; This code will be loaded at 0x8000
[org 0x8000]

align 4

mov ax, 0x10
mov ds, ax			; Set data segment.
mov es, ax			; Set extra segment.
mov ss, ax			; Set stack segment.

mov esp, kernel_stack   ; Let's get the stack set up.

; ----------------------------------------------------------------------------
; Copy kernel code to 0x00100000

    mov ecx, 0
kernel_copy_loop:
    mov eax, [kernel_start_old+ecx]
    mov [KERNEL_START+ecx], eax
    inc ecx
    cmp ecx, 0x00100000                 ; Max kernel size (dwords)
    jne kernel_copy_loop

    jmp KERNEL_START

    times 1023 dd 0x00000000
kernel_stack:
    dd 0x00000000
    
kernel_start_old:
