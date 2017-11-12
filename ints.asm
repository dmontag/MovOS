
;----------------------------------------------------------------------
int00_handler:
cli

sti
iret

;----------------------------------------------------------------------
int01_handler:
cli

sti
iret

;----------------------------------------------------------------------
int02_handler:
cli

sti
iret

;----------------------------------------------------------------------
int03_handler:
cli

sti
iret

;----------------------------------------------------------------------
int04_handler:
cli

sti
iret

;----------------------------------------------------------------------
int05_handler:
cli

sti
iret

;----------------------------------------------------------------------
int06_handler:
cli

sti
iret

;----------------------------------------------------------------------
int07_handler:
cli

inc byte [0xB8000]

sti
iret

;----------------------------------------------------------------------
int08_handler:
cli

inc byte [0xB8002]

sti
iret

;----------------------------------------------------------------------
int09_handler:
cli

inc byte [0xB8004]

sti
iret

;----------------------------------------------------------------------
int10_handler:
cli

inc byte [0xB8006]

sti
iret

;----------------------------------------------------------------------
int11_handler:
cli

inc byte [0xB8008]

sti
iret

;----------------------------------------------------------------------
int12_handler:
cli

sti
iret

;----------------------------------------------------------------------
int13_handler:
cli

sti
iret

;----------------------------------------------------------------------
int14_handler:
cli

sti
iret

;----------------------------------------------------------------------
int15_handler:
cli

sti
iret

;----------------------------------------------------------------------
int16_handler:
cli

sti
iret

;----------------------------------------------------------------------
int17_handler:
cli

sti
iret

;----------------------------------------------------------------------
int18_handler:
cli

sti
iret

;----------------------------------------------------------------------
int19_handler:
cli

sti
iret

;----------------------------------------------------------------------
int20_handler:
cli

sti
iret

;----------------------------------------------------------------------
int21_handler:
cli

sti
iret

;----------------------------------------------------------------------
int22_handler:
cli

sti
iret

;----------------------------------------------------------------------
int23_handler:
cli

sti
iret

;----------------------------------------------------------------------
int24_handler:
cli

sti
iret

;----------------------------------------------------------------------
int25_handler:
cli

sti
iret

;----------------------------------------------------------------------
int26_handler:
cli

sti
iret

;----------------------------------------------------------------------
int27_handler:
cli

sti
iret

;----------------------------------------------------------------------
int28_handler:
cli

sti
iret

;----------------------------------------------------------------------
int29_handler:
cli

sti
iret

;----------------------------------------------------------------------
int30_handler:
cli

sti
iret

;----------------------------------------------------------------------
int31_handler:
cli

sti
iret

;----------------------------------------------------------------------
global irq00_retaddr
global irq00_cs
global irq00_flags
global irq00_eax
global irq00_ebx
global irq00_ecx
global irq00_edx
global irq00_edi
global irq00_esi
global irq00_ebp
global irq00_esp
global irq00_cr3

irq00_retaddr dd 0
irq00_cs dd 0
irq00_flags dd 0
irq00_eax dd 0
irq00_ebx dd 0
irq00_ecx dd 0
irq00_edx dd 0
irq00_edi dd 0
irq00_esi dd 0
irq00_ebp dd 0
irq00_esp dd 0
irq00_cr3 dd 0

irq00_tmp_eax dd 0

irq00_handler:
cli
    
    mov [irq00_eax], eax
    mov [irq00_ebx], ebx
    mov [irq00_ecx], ecx
    mov [irq00_edx], edx
    mov [irq00_esi], esi
    mov [irq00_edi], edi
    mov [irq00_ebp], ebp
    mov eax, cr3
    mov [irq00_cr3], eax

    pop eax
    mov [irq00_retaddr], eax
    pop eax
    mov [irq00_cs], eax
    pop eax
    mov [irq00_flags], eax

    mov [irq00_esp], esp
    
    ; Uptime counter
    add dword [os_uptime], 1
    adc dword [os_uptime+4], 0
    
    call kernel_tick
    
    mov esp, [irq00_esp]
    mov eax, [irq00_flags]
    push eax
    mov eax, [irq00_cs]
    push eax
    mov eax, [irq00_retaddr]
    push eax
    mov eax, [irq00_cr3]
    mov cr3, eax
    mov eax, [irq00_eax]
    mov ebx, [irq00_ebx]
    mov ecx, [irq00_ecx]
    mov edx, [irq00_edx]
    mov esi, [irq00_esi]
    mov edi, [irq00_edi]
    mov ebp, [irq00_ebp]
    
sti
mov al, 0x20
out 0x20, al
iretd

;----------------------------------------------------------------------

irq01_handler:
cli

    push eax
    push ebx
    push ecx
    push edx
    push edi
    push esi

    call kernel_key_pressed

    pop esi
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax

    
sti
mov al, 0x20
out 0x20, al
iretd

;----------------------------------------------------------------------
irq02_handler:
cli

inc byte [0xB8016]

sti
mov al, 0x20
out 0x20, al
iretd

;----------------------------------------------------------------------
irq03_handler:
cli

inc byte [0xB801A]

sti
mov al, 0x20
out 0x20, al
iretd

;----------------------------------------------------------------------
irq04_handler:
cli

inc byte [0xB801C]

sti
mov al, 0x20
out 0x20, al
iretd

;----------------------------------------------------------------------
irq05_handler:
cli

inc byte [0xB801E]

sti
mov al, 0x20
out 0x20, al
iretd

;----------------------------------------------------------------------
irq06_handler:
cli

inc byte [0xB8020]

sti
mov al, 0x20
out 0x20, al
iret

;----------------------------------------------------------------------
irq07_handler:
cli

inc byte [0xB8022]

sti
mov al, 0x20
out 0x20, al
iret

;----------------------------------------------------------------------
irq08_handler:
cli
;  mov word [0xB84FC], 0x044C

inc byte [0xB8014]

sti
mov al, 0x20
out 0xA0, al
iret

;----------------------------------------------------------------------
irq09_handler:
cli

inc byte [0xB800A]

sti
mov al, 0x20
out 0xA0, al
iret

;----------------------------------------------------------------------
irq10_handler:
cli

inc byte [0xB800C]

sti
mov al, 0x20
out 0xA0, al
iret

;----------------------------------------------------------------------
irq11_handler:
cli

inc byte [0xB800E]

sti
mov al, 0x20
out 0xA0, al
iret

;----------------------------------------------------------------------
irq12_handler:
cli

inc byte [0xB8010]

sti
mov al, 0x20
out 0xA0, al
iret

;----------------------------------------------------------------------
irq13_handler:
cli

inc byte [0xB8012]

sti
mov al, 0x20
out 0xA0, al
iret

;----------------------------------------------------------------------
irq14_handler:
cli

sti
mov al, 0x20
out 0xA0, al
iret

;----------------------------------------------------------------------
irq15_handler:
cli

sti
mov al, 0x20
out 0xA0, al
iret

