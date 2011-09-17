[bits 32]

    times 1023 dd 0x55555555
global kernel_stack
kernel_stack:
    dd 0x55555555
    
global kernel_end
kernel_end:
    dd 0x77777777
