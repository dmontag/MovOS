
; These must match the ones in process.h
eax_offset equ  0
ebx_offset equ  4
ecx_offset equ  8
edx_offset equ 12
esi_offset equ 16
edi_offset equ 20
ebp_offset equ 24
esp_offset equ 28
eip_offset  equ 32
flags_offset equ 36
cs_offset equ 40
cr3_offset equ 44

global context_switch
global context_set

extern irq00_retaddr
extern irq00_cs
extern irq00_flags
extern irq00_eax
extern irq00_ebx
extern irq00_ecx
extern irq00_edx
extern irq00_esi
extern irq00_edi
extern irq00_ebp
extern irq00_esp
extern irq00_cr3
extern print_int

tmp_eax dd 0

; [esp+4] = current thread
; [esp+8] = next thread
context_switch:

    mov eax, [esp+4]
    
    mov ebx, [irq00_eax]
    mov [eax + eax_offset], ebx
    
    mov ebx, [irq00_ebx]
    mov [eax + ebx_offset], ebx
    
    mov ebx, [irq00_ecx]
    mov [eax + ecx_offset], ebx
    
    mov ebx, [irq00_edx]
    mov [eax + edx_offset], ebx
    
    mov ebx, [irq00_esi]
    mov [eax + esi_offset], ebx
    
    mov ebx, [irq00_edi]
    mov [eax + edi_offset], ebx
    
    mov ebx, [irq00_ebp]
    mov [eax + ebp_offset], ebx
    
    mov ebx, [irq00_esp]
    mov [eax + esp_offset], ebx
    
    mov ebx, [irq00_flags]
    mov [eax + flags_offset], ebx
    
    mov bx, [irq00_cs]
    mov [eax + cs_offset], bx
    
    mov ebx, [irq00_retaddr]
    mov [eax + eip_offset], ebx
    
    mov ebx, [irq00_cr3]
    mov [eax + cr3_offset], ebx
    
    mov eax, [esp+8]            ; Get next process struct
    
    mov ebx, [eax+eax_offset]   ; Stow away eax
    mov [tmp_eax], ebx          ;
    
    mov ecx, [eax+ecx_offset]
    mov edx, [eax+edx_offset]
    mov esi, [eax+esi_offset]
    mov edi, [eax+edi_offset]
    mov ebp, [eax+ebp_offset]
    mov esp, [eax+esp_offset]
    
    mov ebx, [eax+flags_offset] ; Restore flags on stack
    or ebx, 0x00000200          ; sti
    push ebx

    mov ebx, [eax+cs_offset]    ; Restore segment on stack
    push ebx

    mov ebx, [eax+eip_offset]   ; Restore EIP on stack
    push ebx
    
    mov ebx, [eax+cr3_offset]   ; Restore cr3
    mov cr3, ebx

    mov ebx, [eax+ebx_offset]   ; Restore ebx

    mov al, 0x20
    out 0x20, al
    mov eax, [tmp_eax]          ; Restore eax
    
    sti
    iretd


context_set:

    mov eax, [esp+4]
    
    mov ebx, [eax+eax_offset]   ; Stow away eax
    mov [tmp_eax], ebx          ;
    
    mov ecx, [eax+ecx_offset]
    mov edx, [eax+edx_offset]
    mov esi, [eax+esi_offset]
    mov edi, [eax+edi_offset]
    mov ebp, [eax+ebp_offset]
    mov esp, [eax+esp_offset]
    
    mov ebx, [eax+flags_offset] ; Restore flags on stack
    or ebx, 0x00000200          ; sti
    push ebx

    mov ebx, [eax+cs_offset]    ; Restore segment on stack
    push ebx

    mov ebx, [eax+eip_offset]   ; Restore EIP on stack
    push ebx
    
    mov ebx, [eax+cr3_offset]   ; Restore cr3
    mov cr3, ebx

    mov ebx, [eax+ebx_offset]   ; Restore ebx

    mov al, 0x20
    out 0x20, al
    mov eax, [tmp_eax]          ; Restore eax
    
    sti
    iretd
    
