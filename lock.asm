
global do_lock
global do_unlock

do_lock:
    push eax            ; Save eax
    push ebx            ; Save ebx

    mov eax, [esp+12]   ; Get the argument (lock address)
    mov ebx, 1          ; We'll be putting in a 1 in the lock
do_lock_retry:
    xchg ebx, [eax]     ; Exchange lock for the 1
    cmp ebx, 1          ; Check if the lock was 1 when trying to lock
    jz do_lock_retry    ; If we got a 1 back, try to take the lock again
    
    pop ebx             ; Restore ebx
    pop eax             ; Restore eax
    ret

do_unlock:
    push eax            ; Save eax
    
    mov eax, [esp+8]    ; Get the argument (lock address)
    mov dword [eax], 0  ; Write a 0 to the lock, unlocking it
    
    pop eax             ; Restore eax
    ret

